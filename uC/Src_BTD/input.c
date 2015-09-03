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

/*! \file input.c
    \brief Application layer of the IM module.
    
    This file is derived from "Src_EM/input.c". 
    Make sure to back-port relevant changes there, too!
*/

#define __FILENUM__ 20 // every file needs to have a unique (8bit) ID

#include <stdint.h>
#include "dht.h"
#include "msg.h"
#include "timer.h"
#include "hal.h"
#include "phc.h"
#include "uart.h"
#include "random.h"
#include "input.h"

// PHC receiver state
enum e_inputstate
{
	st_idle, // not expecting any ack or timeout
	st_ack, // expecting plain event ack
	st_cfg, // expecting configuration
	st_last
};

/*************** module input_state ***************/

static struct _global_input_context
{   // codesize saver: place the most frequented on top, cluster them like locally used
    uint8_t modul_addr; // class and module address byte
    switch_t leds; // state of the feedback LEDs
    switch_t new_enable[INPUT_OFF+1]; // which input shall send what, index is function offset
    uint8_t toggle; // toggle bit
    uint8_t is_bootcmd; // special flag to test for bootloader mode cmd, too
    uint8_t len; // store the command length to aid final validation
    // uint8_t disabled; // instructed to shut up
    uint8_t retry_wait; // packet retry time
    uint8_t ping_reply_len; // 4 or 5, larger if 16 inputs and talking to an STM v2.x
    enum e_inputstate state;
    uint16_t temperature;

    // state for interrupt context packet parsing
    uint8_t addr;
    uint8_t rcv_toggle;
    uint8_t listening; // flag if we're interested
    uint8_t cmd; // channel/command byte
} input;


/*************** private internal functions ***************/



/*************** public API functions ***************/

// assign a bus address
void input_init(uint8_t addr)
{
	input.modul_addr = addr; // class and address
	
	//input.disabled = 0;
    input.state = st_cfg;
    input.retry_wait = ACK_TO;

    switch_init();
	
	//timer_msg(HZ*3); // send the boot message in 3 seconds

    // send the boot message almost immediately, 3 sec wait was done by bootloader
    timer_msg(2); 
}

// the main loop of the PHC input application
void input_mainloop(void)
{
	uint8_t response[2+CFG_INPUT+2]; // worst-case size with addr+length, payload, CRC
	
	while (1)
	{
		struct msg message;

		message = msg_get();

		switch(message.id)
		{
		case e_learned:
			input.leds = 0; // clear the feedback
		    hal_set_led(4); // main indication off, but still in muted mode
			// break;
            // Fall through, send state to indicate learned. 
            // This is an experimental extension to the normal protocol.

		case e_send_state: // send LED state
			response[2] = 0x00;
#if CFG_INPUT == 8
			response[3] = input.leds;
			phc_send(input.modul_addr, response, 2, message.data); // led response
#else
			response[3] = input.leds & 0xFF;
			response[4] = input.leds >> 8;
			phc_send(input.modul_addr, response, 3, message.data); // led response
#endif	
			break;
		
		case e_send_ping: // send ping response
            {
                switch_t current = switch_getcurrent();
			    response[2] = 0x01;
			    response[3] = 0x00; // TODO: Find out
			    response[4] = current & 0xFF;
			    response[5] = current >> 8;
			    response[6] = input.leds & 0xFF;
			    response[7] = input.leds >> 8; // unused if 8 inputs or STM1
			    response[8] = 0x57;			// temp1 ?
			    response[9] = 0x6c;
			    response[10] = 0x04;
			    response[11] = 0x57;		// temp2 ?
			    response[12] = 0xe4;
			    response[13] = 0x04;
			    response[14] = 0x01;
			    response[15] = 0x08;
			    response[16] = 0x20;
			    phc_send(input.modul_addr, response, input.ping_reply_len, message.data);		
            }
			break;
		
		case e_event: // input event(s)
            //if (!input.disabled)
            {
                uint8_t size = switch_getevents(
                    response+2, // behind addr+length field
                    sizeof(response)-2-2, // without addr, length, CRC
                    message.data);

                if (!uart_is_busy() 
                    && phc_send(input.modul_addr, response, size, input.toggle) == 0)
                {
    			    input.state = st_ack;
	                timer_msg(input.retry_wait); // ack timeout with backoff period
                }
                else
                {
                    switch_nack(); // restore the event queue, event will be re-posted
                }
            }
			break;
			
		case e_timer:
            input.retry_wait += 1 + (rand() & 0x03); // back off, increase timeout (wraps on purpose at 255)

            if (input.retry_wait < ACK_TO)
            {   // most of all, must not become zero, that would cancel the timer
                input.retry_wait += ACK_TO;
            }
        
			if (input.state == st_cfg)
			{
                if (!uart_is_busy()) // about to send unsolicited: only with idle line
                {
			        response[2] = 0xFF;
#if CFG_INPUT == 16
			        response[3] = 0xFC; // we have 16 inputs, an STM2 will reply with extended configuration
#else
			        response[3] = 0x00; // tell the STM we have 8 inputs
#endif	
				    phc_send(input.modul_addr, response, 2, input.toggle); // boot message
                }
                timer_msg(input.retry_wait); // send ourself a message in a few ticks, response timeout
			}
			else if (input.state == st_ack) // waiting for ack
			{
                switch_nack(); // restore the event queue, event will be re-posted
			}
			break;
		case e_temperature:
			if (!uart_is_busy()) // about to send unsolicited: only with idle line
			{
				response[2] = 0x87;
				response[3] = 0x57;
				response[4] = 0xEE;
				response[5] = 0x04;

				phc_send(input.modul_addr, response, 4, input.toggle);
			}
			break;
		default: // unhandled message
			ASSERT(0);
		} // case
		
		dht_tick();

	} // while(1)
}


// interrupt context functions for incoming packet processing

// packet start
void input_cmd_start(uint8_t address, uint8_t toggle, uint8_t len)
{
	input.addr = address;
	input.rcv_toggle = toggle;
	input.listening = (address == input.modul_addr || (address & 0xE0) == 0xE0); // listen for POR, too
    input.is_bootcmd = (len == 3); // to be refined during payload
    input.len = len; // store length for later validation
}

void input_payload(uint8_t pos, uint8_t byte)
{
    if (!input.listening)
	{
		return; // short cut
	}

	if (pos == 0) // channel/command byte
	{
		input.cmd = byte;
        if (input.cmd != 0x01) // ping, could become magic boot ping
        {
            input.is_bootcmd = 0; // no: falsify
        }
		
		// prepare our only non-simple command
        if (input.cmd == 0xFE || input.cmd == 0xFC) // configuration
		{
			uint8_t i;	
			for (i=INPUT_ON_GREATER_0; i<=INPUT_OFF; i++)
			{
				input.new_enable[i] = 0;
			}
		}
		return;
	}

    if (input.is_bootcmd) // candidate for magic boot ping
    {
    	if ((pos == 1 && byte != 'B')
         || (pos == 2 && byte != 'L'))
        {
            input.is_bootcmd = 0; // mismatch: falsify
        }
    }
    
	// preliminary process the non-simple commands, everything with payload
	// the CRC may prove wrong later on, so don't do anything permanent	

    if (input.cmd == 0xFE || input.cmd == 0xFC) // configuration, normal or extended
	{
        uint8_t por_size = (input.cmd == 0xFC) ? 4 : 3; // size of POR part
        
		if (pos > por_size) // skip the POR part, not relevant for us
		{
			uint8_t channel = byte >> 4;
			uint8_t function = (byte & 0x0F);
			if (function <= INPUT_OFF) // was 2...6 legal range
			{
				input.new_enable[function] |= (switch_t)1 << channel;
			}
		}
	}
}

// packet end
void input_cmd_end(uint8_t valid, uint8_t retry)
{
	enum msg_id id = e_nop;
	
	if (!input.listening)
	{
		return; // short cut
	}

	if (!valid)
	{	// we don't care, nothing to be un-done
		return; 
	}

    if ((input.addr & 0xE0) == 0xE0) // broadcast command
    {
        // ToDo: validate the global commands
        switch (input.cmd)
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
        if (input.is_bootcmd) // magic bootloader ping
        {
            // execute bootloader
            hal_reboot();
            // won't continue here
        }

	    switch (input.cmd) // first look at the whole command byte
	    {
	    case 0x00: // ack
		    if (input.state == st_ack)
		    {
			    input.state = st_idle;
	            input.toggle = !input.toggle; // late toggle, on roundtrip success
                input.retry_wait = ACK_TO; // reset the timeout
			    timer_msg(0); // cancel timer
			    switch_ack(); // enable next event
		    }
		    break;

	    case 0x01: // ping
		    id = e_send_ping;
		    break;
	    
        case 0xFC: // configuration from STM2
	    case 0xFE: // configuration
		    {
                input.ping_reply_len =  15;

    		    // copy the new config
                switch_set_enable(input.new_enable);

			    if (input.state == st_cfg)
			    {
				    input.state = st_idle;
				    timer_msg(0); // cancel timer
			    }
		    }
		    break;
		    
	    case 0xFF: // reset
            if (input.len == 1) // length 2 could be a boot msg. of a module with same adr.!
            {
                hal_reboot(); // warm reset, goes through bootloader if avail.
            }
		    break;
		    
	    default: // then check for channel/function pair
		    {
			    uint8_t channel = input.cmd >> 4;
			    uint8_t function = input.cmd & 0x0F;

                // not strictly necessary, not disturbing to do the command again
                if (retry) // repeated command: our reply wasn't received
                {
                    function = 0; // now do nothing, action already happened last time
				    id = e_send_state;
                }

                switch (function)
			    {
			    case 2: // LED on, enable IR code learning for a channel
				    input.leds = (switch_t)1 << channel;
				    hal_set_led(7); // indicate learning mode and muted
			    	//rc5_learn(channel); // set learning
				    id = e_send_state;
				    break;	

			    case 3: // LED off, back to normal function
				    input.leds = 0;
				    hal_set_led(0);
				    //rc5_normal(); // learning mode off and muted off
				    id = e_send_state;
				    break;	
			    }
		    }		
        } // switch (input.cmd)
    } // if no broadcast cmd
	
	if (id != e_nop) // changed by above: someone wants to send a message
	{
		struct msg message;
		message.id = id;
		message.data = input.rcv_toggle;
		if (msg_post(&message) != 0) // overflow, problem
		{
			ASSERT(0);
		}
	}

	
	// Still waiting for an ack: treat any new command as negative ack,
	//  the real one may have gotten lost. (this avoids deadlock)
    // Input events will be re-posted, no information loss.
	if (input.state == st_ack)
	{
		input.state = st_idle;
		switch_nack(); // negative, but enable next event
		timer_msg(0); // cancel timer
	}
}

