/***************************************************************************
 *
 * OpenHC:                          ___                 _  _  ___
 *  Open source                    / _ \ _ __  ___ _ _ | || |/ __|
 *  Home                          | (_) | '_ \/ -_) ' \| __ | (__ 
 *  Control                        \___/| .__/\___|_||_|_||_|\___|
 * http://openhc.sourceforge.net/       |_| 
 *
 * Copyright (C) 2005 by Joerg Hohensohn
 *
 * All files in this archive are subject to the GNU General Public License.
 * See http://www.gnu.org/licenses/gpl-3.0.txt for full license agreement.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/ 

/*! \file jrm.c
    \brief Application layer of the JRM module
    
     This file is derived from "Src_AM/output.c". 
     Make sure to back-port relevant changes there, too!
*/

#define __FILENUM__ 18 // every file needs to have a unique (8bit) ID

#include <stdint.h>
#include "msg.h"
#include "timer.h"
#include "hal.h"
#include "phc.h"
#include "uart.h"
#include "random.h"
#include "jrm.h"


#define BOOST_TICKS (HZ/25) // how long 100% power to relays switching on
#define DIR_TICKS (HZ*6/10) // short pause for direction change, 0.6 sec
//#define START_TICKS (HZ*2/10) // short pause before driving, 0.2 sec
#define START_TICKS 0 // personally, I don't like the start delay, disable it

#define PWM_ADDR 0x100 // EEPROM location of PWM fraction for relay holding

#define BUFSIZE 32 // event buffer size (must be power of 2)

#define BUF_MASK (BUFSIZE - 1)
#if (BUFSIZE & BUF_MASK)
	#error BUFSIZE is not a power of 2
#endif

#define UP_BITS   0x55 // the lower port bit of each pair drives up
#define DOWN_BITS 0xAA // the higher port bit of each pair drives down

// dynamic state per channel
enum e_channelstate
{
	act_idle,  // doing nothing 
    act_delay, // initial delay, "Versatzzeit" running
    act_run,   // runtime
    act_turn,  // short delay for switching direction
    act_flip,  // runtime in opposite direction
    act_stop,  // final stop

    act_on,    // delayed on
    act_off,   // delayed off

	act_last	
};

// PHC receiver state
enum e_jrmstate
{
	st_idle, // not expecting any ack or timeout
	st_ack, // expecting plain event ack
	st_cfg, // expecting configuration
	st_last
};


/*************** JRM module state ***************/
static struct _global_jrm_context
{   // codesize saver: place the most frequented on top, cluster them like locally used
    
    // state machine and runtime of the phases
    enum e_channelstate channelstate[8]; // dynamic state, per channel
    uint32_t timer[8]; // the 4 shutter times plus 4 timer channels
    uint32_t time_delay[4]; // pre-action time
    uint32_t time_run[4]; // runtime
    uint32_t time_flip[4]; // time in opposite direction

    uint8_t modul_addr; // class and module address byte
    uint8_t lines; // state of the physical output lines (up/down bit pairs)
    uint8_t timers; // state of the timers, upper 4 bit
    uint8_t directions; // nominal direction of each channel, as up/down bit pairs
    uint8_t feedback_enable[JRM_FB_TIMER_OFF+1]; // which output shall send what feedback, index is function
    uint8_t new_enable[JRM_FB_TIMER_OFF+1]; // same, but working copy during ISR

    uint8_t lock[4]; // priority locks per channel
    uint8_t priority[4]; // currently set priority
    uint8_t restore_priority[4]; // priority to be restored after run, 0xFF if none

    uint32_t new_time[3]; // time(s) as received in ISR, a command can have up to 3
    uint8_t new_priority; // priority bits and change-at-runtime as received by ISR

    uint8_t toggle; // toggle bit for self-initiated packets
    uint8_t is_bootcmd; // special flag to test for bootloader mode cmd, too
    uint8_t len; // store the command length to aid final validation
    //uint8_t disabled; // instructed to shut up
    int8_t boost_ticks; // high-power time for relays, during switch-on
    uint8_t pwm; // nominal PWM ratio in 1/256 units

    enum e_jrmstate rcv_state; // waiting for which response
    uint8_t retry_wait; // packet retry time

    // state for interrupt context packet parsing
    uint8_t addr;
    uint8_t rcv_toggle; // toggle bit of received packet
    uint8_t listening; // flag if we're interested
    uint8_t cmd; // channel/command byte

    // state of the event queue
    uint8_t eventqueue[BUFSIZE]; 
    uint8_t queue_head; // write position, maintained by ISR (timer tick)
    uint8_t queue_tail; // acknowledged up to here, maintained by main loop
    uint8_t queue_wait4ack; // notify message is sent, waiting for main loop
} jrm;


/*************** private internal functions ***************/

// send an internal message, from ISR to main loop
static void send_msg_reply(uint8_t id)
{
	struct msg message;
	message.id = id;
	message.data = jrm.rcv_toggle;
	if (msg_post(&message) != 0) // overflow, problem
	{
		ASSERT(0);
	}
}

// add an entry to the event queue
static void post_event(uint8_t event, uint8_t channel)
{
    uint8_t mask = (uint8_t)1 << channel;

    if (jrm.feedback_enable[event] & mask) // enabled by configuration?
    {
        uint8_t newhead = (jrm.queue_head + 1) & BUF_MASK;
        if (newhead != jrm.queue_tail) // no overflow
        {
            event |= channel << 5; // add the channel #
            jrm.eventqueue[newhead] = event;
            jrm.queue_head = newhead;
        }
        else
        {
        	ASSERT(0); // queue overflow
        }
    }
}

// send a message on non-empty event queue and idle
static void notify_event(void)
{
    if (!jrm.queue_wait4ack // not already pending message
        && jrm.queue_head != jrm.queue_tail // queue not empty
        && !uart_is_busy()) // not busy receiving other stuff
    {
        struct msg message;

	    message.id = e_event;
	    message.data = 0; // ToDo: useful payload?
	    if (msg_post(&message) == 0) 
        {   // success: place in the event queue
            jrm.queue_wait4ack = 1;
        }
        else
	    {   // overflow, problem
		    ASSERT(0);
	    }
        
    }

}

// set new output value
static void set_output(uint8_t new_lines)
{
    if (new_lines == jrm.lines)
    {
        return; // nothing to do
    }

    if (new_lines & (uint8_t)~jrm.lines) // new set bit(s)?
    {
        hal_set_pwm(255); // full power while activating relays
        jrm.boost_ticks = BOOST_TICKS;
    }
    else if (new_lines == 0)
    {
        hal_set_pwm(0); // DC on to reduce EMI, no pulses
        jrm.boost_ticks = -1;
    }
    
    jrm.lines = new_lines; // finally, set the new levels
    hal_set_output(jrm.lines);
}

/*************** public API functions ***************/

// assign a bus address
void jrm_init(uint8_t addr)
{
	uint8_t i;

    for (i=0; i<4; i++)
    {   
        jrm.restore_priority[i] = 0xFF; // indicate no restore
    }
	jrm.modul_addr = addr; // class and address
	
	jrm.rcv_state = st_cfg;
    jrm.boost_ticks = -1;
    jrm.retry_wait = ACK_TO;

    jrm.pwm = eeprom_read_byte((void*)PWM_ADDR);
    hal_set_pwm(0); // DC power to relays
	
	//timer_msg(HZ*3); // send the boot message in 3 seconds
	
    // send the boot message almost immediately, 3 sec wait was done by bootloader
    timer_msg(2); 
}

// the main loop of the PHC JRM application
void jrm_mainloop(void)
{
	uint8_t response[2+3+2]; // worst-case size with addr+length, payload, CRC
	uint8_t event; // our to-be-generated switch message
	
	while (1)
	{
		struct msg message;

		event = 0; // we check for change later
		message = msg_get();

		switch(message.id)
		{
		case e_send_state: // send updown line state as ack
            response[2] = 0x00;
			response[3] = jrm.lines;
            // no error handling necessary, if no success in sending our reply, the sender will re-post
			phc_send(jrm.modul_addr, response, 2, message.data);
			break;
            
		case e_send_timer: // send timer state as ack
            response[2] = 0x00;
			response[3] = jrm.lines;
            // no error handling necessary, if no success in sending our reply, the sender will re-post
			phc_send(jrm.modul_addr, response, 2, message.data);
			break;
            
		case e_send_ping: // send ping response
            response[2] = 0x01;
			response[3] = jrm.timers; // most likely timer state
			response[4] = jrm.lines;  //  and updown state
            // no error handling necessary, if no success in sending our reply, the sender will re-post
			phc_send(jrm.modul_addr, response, 3, message.data);
			break;
		
        case e_event:
            //if (!jrm.disabled)
            {
                uint8_t newtail = (jrm.queue_tail + 1) & BUF_MASK;
                ASSERT(jrm.queue_head != jrm.queue_tail); // not empty
                response[2] = jrm.eventqueue[newtail];
                response[3] = (response[2] & 0x80) ? jrm.lines : jrm.timers;
			    if (!uart_is_busy() && 
                    phc_send(jrm.modul_addr, response, 2, jrm.toggle) == 0)
                {   // successsful sent
			        jrm.rcv_state = st_ack;
	                timer_msg(jrm.retry_wait); // ack timeout with backoff period
                }
                else
                {
                    jrm.queue_wait4ack = 0; // event will be re-posted in tick ISR
                }
            }
			break;

		case e_timer:
            jrm.retry_wait += 1 + (rand() & 0x03); // back off, increase timeout (wraps on purpose at 255)

            if (jrm.retry_wait < ACK_TO)
            {   // most of all, must not become zero, that would cancel the timer
                jrm.retry_wait += ACK_TO;
            }
        
			if (jrm.rcv_state == st_cfg)
			{
                if (!uart_is_busy()) // about to send unsolicited: only with idle line
                {
                    response[2] = 0xFF;
                    phc_send(jrm.modul_addr, response, 1, jrm.toggle); // boot message
                }

                timer_msg(jrm.retry_wait); // send ourself a message in a few ticks, response timeout
			}
	        else if (jrm.rcv_state == st_ack) // waiting for ack
	        {
                jrm.queue_wait4ack = 0; // allow next posting, within timer
                jrm.rcv_state = st_idle;
	        }
			break;

		default: // unhandled message
			ASSERT(0);
		} // case
	} // while(1)
}

// the state machine which runs the shutter sequence
// driven by command or timer, both are ISRs and won't compete (re-enter)
static void statemachine(uint8_t channel)
{
    uint8_t new_lines = jrm.lines; // copy current state
    uint8_t pairmask = 0x03 << channel * 2; // proper bit pair
    uint8_t npairmask = ~pairmask;
    uint8_t changed_lines;
    
    switch (jrm.channelstate[channel]) // the state we're about to enter
    {
    case act_delay:
        jrm.channelstate[channel] = act_run;
        if (jrm.time_delay[channel] < START_TICKS)
        {
            jrm.time_delay[channel] = START_TICKS;
        }
        
        if (jrm.time_delay[channel])
        {   // start initial delay
            jrm.timer[channel] = jrm.time_delay[channel];
            break;
        } // else fall through, proceed with next state

    case act_run: // end of initial delay
        jrm.channelstate[channel] = act_turn;
        if (jrm.time_run[channel])
        {
            jrm.timer[channel] = jrm.time_run[channel];
            // start the run in nominal direction
            new_lines = (new_lines & npairmask) | (jrm.directions & pairmask);
            break;
        } // else fall through, proceed with next state

    case act_turn: // end of run
        jrm.channelstate[channel] = act_flip;
        new_lines &= npairmask; // stop the run
        //rise = fall = 0;
        if (jrm.time_flip[channel])
        {
            jrm.timer[channel] = DIR_TICKS; // small pause before changing direction
            break;
        } // else fall through

    case act_flip: // end of direction change pause
        jrm.channelstate[channel] = act_stop;
        if (jrm.time_flip[channel])
        {
            // start the run in opposite direction
            new_lines = (new_lines & npairmask) | (~jrm.directions & pairmask);
            jrm.timer[channel] = jrm.time_flip[channel];
            break;
        } // else fall through

    case act_stop:
        new_lines = new_lines & npairmask; // stop the run
        if (jrm.restore_priority[channel] != 0xFF)
        {
            jrm.priority[channel] = jrm.restore_priority[channel];
            jrm.restore_priority[channel] = 0xFF; // ToDo: necessary?
        }
        jrm.channelstate[channel] = act_idle; // end of state machine
        // ToDo: restore priority
        break;
 
    default:
        ASSERT(jrm.channelstate[channel] == act_idle); // else illegal
        break;
    }

    // send feedback, if enabled and changed
    changed_lines = jrm.lines ^ new_lines;
    if (changed_lines & UP_BITS)
    {   // lift state changed
        if (new_lines & pairmask & UP_BITS)
        {   // now set
            post_event(JRM_FB_LIFT_ON, channel);
        }
        else
        {   // now reset
            post_event(JRM_FB_LIFT_OFF, channel);
        }
    }
    if (changed_lines & DOWN_BITS)
    {   // lower state changed
        if (new_lines & pairmask & DOWN_BITS)
        {   // now set
            post_event(JRM_FB_LOWER_ON, channel);
        }
        else
        {   // now reset
            post_event(JRM_FB_LOWER_OFF, channel);
        }
    }

    set_output(new_lines); // also sets jrm.lines
}


// handle the command stored in jrm.cmd, already identified as non-timer
static void command_jrm(void)
{
    uint8_t channel = (jrm.cmd >> 5) & 0x03; // "real" channel
    uint8_t priority = jrm.new_priority & 0x3F;
	uint8_t function = jrm.cmd & 0x1F;
    uint8_t pairmask, npairmask; // the selected two up/down bits

    
    // default command answer, post it now before any feedbacks below
    send_msg_reply(e_send_state);

    
    if (function != JRM_PRIO_LOCK && function != JRM_PRIO_UNLOCK)
    {
        if (priority & jrm.lock[channel]) // access to locked priority layer
        {
            return; // no access to locked priority layer
        }

        if (function != JRM_PRIO_SET && function != JRM_PRIO_CLEAR)
        {
            // this relies on cmd prio having relevant and all lower bits set
            if (jrm.priority[channel] & (priority >> 1))
            {
                return; // priority was too low
            }
            
            if (jrm.new_priority & 0x40) // set prio while running
            {
                if (jrm.restore_priority[channel] == 0xFF) // none set yet
                {   // restore it after the run
                    jrm.restore_priority[channel] = jrm.priority[channel];
                    // (else, the potentially lower first one will be restored)
                }
                // the higher prio LSB bits are also set, don't save those
                jrm.priority[channel] = priority ^ (priority >> 1);
            }
        }
    }
    
    pairmask = 0x03 << channel * 2; // masks both up and down bit
    npairmask = ~pairmask; // negative mask

    switch (function)
	{
    case JRM_STOP: // "Lauf stoppen"
        jrm.channelstate[channel] = act_stop;
        statemachine(channel);
        break;

    case JRM_TOGGLE_LIFT_STOP: // "Umschalten heben/aus"
        if (jrm.lines & pairmask) // running?
        { // stop it
            jrm.channelstate[channel] = act_stop;
            statemachine(channel);
            break;
        } // else fall through

    case JRM_LIFT: // "Einschalten heben"
    case JRM_FLIP_UP: // "Tippbetrieb heben"
        jrm.directions = (jrm.directions & npairmask) | (pairmask & UP_BITS);
        jrm.time_delay[channel] = 0;
        jrm.time_run  [channel] = jrm.new_time[0];
        jrm.time_flip [channel] = 0;
        jrm.channelstate[channel] = act_delay; // first state
        statemachine(channel);
        break;

    case JRM_TOGGLE_LOWER_STOP: // "Umschalten senken/aus"
        if (jrm.lines & pairmask) // running?
        { // stop it
            jrm.channelstate[channel] = act_stop;
            statemachine(channel);
            break;
        } // else fall through

    case JRM_LOWER: // "Einschalten senken"
    case JRM_FLIP_DOWN: // "Tippbetrieb senken"
        jrm.directions = (jrm.directions & npairmask) | (pairmask & DOWN_BITS);
        jrm.time_delay[channel] = 0;
        jrm.time_run  [channel] = jrm.new_time[0];
        jrm.time_flip [channel] = 0;
        jrm.channelstate[channel] = act_delay; // first state
        statemachine(channel);
        break;

    case JRM_PRIO_LOCK: // "Prioritätsebenen verriegeln"
        jrm.lock[channel] |= priority;
        break;

    case JRM_PRIO_UNLOCK: // "Prioritätsebenen entriegeln"
        jrm.lock[channel] &= ~priority;
        break;

    case JRM_LEARN_ON: // "Lernfunktion einschalten"
    case JRM_LEARN_OFF: // "Lernfunktion ausschalten"
        break; // not implemented

    case JRM_PRIO_SET: // "Prioritätsebenen setzen"
        jrm.priority[channel] |= priority;
        break;

    case JRM_PRIO_CLEAR: // "Prioritätsebenen löschen"
        jrm.priority[channel] &= ~priority;
        break;

    case JRM_SENSOR_LIFT: // "Sensorik Rolladen heben"
    case JRM_SENSOR_LIFT_FLIP: // "Sensorik Jalousie heben"
        jrm.directions = (jrm.directions & npairmask) | (pairmask & UP_BITS);
        jrm.time_delay[channel] = jrm.new_time[0];
        jrm.time_run  [channel] = jrm.new_time[1];
        jrm.time_flip [channel] = jrm.new_time[2]; // 0 in case of JRM_SENSOR_LIFT
        jrm.channelstate[channel] = act_delay; // first state
        statemachine(channel);
        break;

    case JRM_SENSOR_LOWER: // "Sensorik Rolladen senken"
    case JRM_SENSOR_LOWER_FLIP: // "Sensorik Jalousie senken"
        jrm.directions = (jrm.directions & npairmask) | (pairmask & DOWN_BITS);
        jrm.time_delay[channel] = jrm.new_time[0];
        jrm.time_run  [channel] = jrm.new_time[1];
        jrm.time_flip [channel] = jrm.new_time[2]; // 0 in case of JRM_SENSOR_LOWER
        jrm.channelstate[channel] = act_delay; // first state
        statemachine(channel);
        break;
    }
}


// handle the command stored in jrm.cmd, already identified as a timer command
static void command_timer(void)
{
    uint8_t rawchannel = jrm.cmd >> 5; // raw: upper bit is timer flag, lower 2 are channel
	uint8_t function = jrm.cmd & 0x1F;
    uint8_t mask = (uint8_t)1 << rawchannel;

    // default command answer, post it now before any feedbacks below
    send_msg_reply(e_send_timer);

    switch (function)
	{
    case JRM_TIMER_DELAY_ON: // "Zeitmessung verzögert Ein"
        jrm.timer[rawchannel] = jrm.new_time[0];
        jrm.channelstate[rawchannel] = act_on;
        break;

    case JRM_TIMER_ON_OFF: // "Zeitmessung Ein mit Zeitglied"
        if (!(jrm.timers & mask))
        {
            jrm.timers |= mask; // set status flag to "on"
            post_event(JRM_FB_TIMER_ON, rawchannel);
        }
        // fall through
    case JRM_TIMER_DELAY_OFF: // "Zeitmessung verzögert Aus"
        jrm.timer[rawchannel] = jrm.new_time[0];
        jrm.channelstate[rawchannel] = act_off;
        break;

    case JRM_TIMER_CANCEL: // "Zeitmessung abbrechen"
        // don't change the status flag
        jrm.timer[rawchannel] = 0;
        jrm.channelstate[rawchannel] = act_idle;
        post_event(JRM_FB_TIMER_CANCEL, rawchannel);
        break;
    }
}



// interrupt context functions for incoming packet processing

// packet start
void jrm_cmd_start(uint8_t address, uint8_t toggle, uint8_t len)
{
	jrm.addr = address;
	jrm.rcv_toggle = toggle;
	jrm.listening = (address == jrm.modul_addr || (address & 0xE0) == 0xE0); // listen for POR, too
    jrm.is_bootcmd = (len == 3); // to be refined during payload
    jrm.len = len; // store length for later validation
}

void jrm_payload(uint8_t pos, uint8_t byte)
{
	if (!jrm.listening)
	{
		return; // short cut
	}
	
	if (pos == 0) // channel/command byte
	{
		jrm.cmd = byte;
        if (jrm.cmd != 0x01) // ping, could become magic boot ping
        {
            jrm.is_bootcmd = 0; // no: falsify
        }
		
		// prepare non-simple commands
        jrm.new_time[0] = jrm.new_time[1] = jrm.new_time[2] = 0;  // clean, so it'll be 0 if no time given
		if (jrm.cmd == 0xFE) // configuration
		{
			uint8_t i;
            for (i=JRM_FB_LIFT_ON; i<=JRM_FB_TIMER_OFF; i++)
            {
                jrm.new_enable[i] = 0;
            }
		}
		return;
	}

    if (jrm.is_bootcmd) // candidate for magic boot ping
    {
    	if ((pos == 1 && byte != 'B')
         || (pos == 2 && byte != 'L'))
        {
            jrm.is_bootcmd = 0; // mismatch: falsify
        }
    }

    // preliminary process the non-simple commands, everything with payload
	// the CRC may prove wrong later on, so don't do anything permanent	

	if (jrm.cmd == 0xFE) // configuration
	{
		if (pos > 2) // skip the POR part, original JRM seems to ignore it
		{
			uint8_t channel = byte >> 5;
			uint8_t function = (byte & 0x1F);
			if (function <= JRM_FB_TIMER_OFF) // was 2...8 legal range
			{
				jrm.new_enable[function] |= (uint8_t)1 << channel;
			}
		}
	}
    else if (pos >= 1)
    {
        uint8_t timepos_min = 2; // part of payload which may be time info
        uint8_t timepos_max = 7;
        if (jrm.cmd & 0x80) // timer command
        {
            timepos_min = 1; // time info right after command
            timepos_max = 2;
        }
        else // shutter command
        {
            if (pos == 1)
            {
                jrm.new_priority = byte;
            }
        }
        
        if (pos >= timepos_min && pos <= timepos_max) // larger commands carry time(s)
        {
            uint8_t msb = (pos - timepos_min) & 1;
            uint8_t idx = (pos - timepos_min) / 2;
            if (!msb)
            {
                jrm.new_time[idx] = byte; // LSB
            }
            else
            {
                jrm.new_time[idx] |= (uint32_t)byte << 8; // MSB
                jrm.new_time[idx] *= (uint32_t)(HZ/10); // convert to ticks
            }
        }
    } // if pos >= 1
}

// packet end
void jrm_cmd_end(uint8_t valid, uint8_t retry)
{
	if (!jrm.listening)
	{
		return; // short cut
	}

	if (!valid)
	{	// we don't care, nothing to be un-done
		return; 
	}

    if ((jrm.addr & 0xE0) == 0xE0) // broadcast command
    {
        // ToDo: validate the global commands
        switch (jrm.cmd)
        {
        case 0xFF: // POR, ToDo: meaning of payload byte, hard/soft POR?
            hal_reboot();
		    break; // won't continue here, but with a break the code gets smaller

        case 0xFE: // unknown???
            // ToDo
            break;
        }
    }
    else // command to our specific address
    {
        if (jrm.is_bootcmd)
        {
            // execute bootloader
            hal_reboot();
            // won't continue here
        }

	    switch (jrm.cmd) // first look at the whole command byte
	    {
	    case 0x00: // ack
		    if (jrm.rcv_state == st_ack)
		    {
			    jrm.rcv_state = st_idle;
		        jrm.toggle = !jrm.toggle; // late toggle, on roundtrip success
                jrm.retry_wait = ACK_TO; // reset the timeout
			    timer_msg(0); // cancel timer

                // Fixme: better do this in main loop, via msg?
                jrm.queue_tail = (jrm.queue_tail + 1) & BUF_MASK; // remove tail entry
                jrm.queue_wait4ack = 0; // allow next posting
		    }
		    break;

	    case 0x01: // ping
		    send_msg_reply(e_send_ping);
		    break;
	    
	    case 0xFE: // configuration
		    {
			    uint8_t i;
                // copy the new config
                for (i=JRM_FB_LIFT_ON; i<=JRM_FB_TIMER_OFF; i++)
                {
    			    jrm.feedback_enable[i] = jrm.new_enable[i];
                }

			    if (jrm.rcv_state == st_cfg)
			    {
				    jrm.rcv_state = st_idle;
				    timer_msg(0); // cancel timer
			    }
		    }
		    break;
		    
	    case 0xFF: // reset
            if (jrm.len == 1) // length 2 could be a boot msg. of a module with same adr.!
            {
                hal_reboot(); // warm reset, goes through bootloader if avail.
            }
		    break;
		    
	    default: // then check for channel/function pair
			if (jrm.cmd & 0x80)
            {
                command_timer();
            }
            else
            {
                command_jrm();
            }
                
        } // switch (jrm.cmd)
    } // if no broadcast cmd
		
	// Still waiting for an ack: treat any new command as negative ack,
	//  the real one may have gotten lost.
	if (jrm.rcv_state == st_ack)
	{
        jrm.queue_wait4ack = 0; // allow next posting, within timer

        jrm.rcv_state = st_idle;
		timer_msg(0); // cancel timer
	}
}

// also an ISR, so it won't compete against jrm_cmd_end()
void jrm_tick(void)
{
    uint8_t i;
    
#ifdef CFG_ADC // constant monitoring
    if (jrm.boost_ticks > 0)
        jrm.boost_ticks--; // only count down to 0

    if (jrm.boost_ticks == 0) // expired?
    {
        uint32_t pwm = jrm.pwm; // reduced power with PWM
        // tune the PWM value by actually measured supply voltage
        if (pwm < 255)
        {   // no full power intended
            pwm = (pwm * 24000L) / hal_read_voltage();
            if (pwm > 255) // cap if it became too large, >100%
            {
                pwm = 255;
            }
       }
       hal_set_pwm(pwm);
    }
#else // only interested in edge
    if (jrm.boost_ticks > 0 && --jrm.boost_ticks == 0)
    {
       hal_set_pwm(jrm.pwm);
    }
#endif

    for (i=0; i<4; i++) // shutter channels
    {
        if (jrm.timer[i] && --jrm.timer[i] == 0)
        {   // was running, now expired
            statemachine(i);

            break; // exit the loop, do one at a time (avoid problem of multiple expirations)
                   //  check, might be possible with event queue now?
        }

    } // for (i=0; i<4; i++)

    for (i=4; i<8; i++) // timer channel states
    {
        if (jrm.timer[i] && --jrm.timer[i] == 0)
        {   // was running, now expired
            uint8_t mask = (uint8_t)1 << i;
            
            switch (jrm.channelstate[i])
            {
            case act_on:
                jrm.timers |= mask;
                jrm.channelstate[i] = act_idle;
                post_event(JRM_FB_TIMER_ON, i);
                break;

            case act_off:
                jrm.timers &= ~mask;
                jrm.channelstate[i] = act_idle;
                post_event(JRM_FB_TIMER_OFF, i);
                break;
            
            default:
                ASSERT(jrm.channelstate[i] == act_idle); // else illegal
                break;
            }

            break; // exit the loop, do one at a time (avoid problem of multiple expirations)
                   //  check, might be possible with event queue now?
        }

    } // for (i=4; i<8; i++)

    notify_event();
}
