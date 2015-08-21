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

/*! \file rc5.c
    \brief Application helper part of the IR module.
    
    The RC5 infrared decoder and the switch emulation.
    This file is derived from "Src_EM/switch.c". 
    Make sure to back-port relevant changes there, too!
*/

#define __FILENUM__ 21 // every file needs to have a unique (8bit) ID

#include <stdint.h>
#include "hal.h"
#include "uart.h"
#include "msg.h"
#include "timer.h"
#include "phc.h"
#include "rc5.h"
#ifdef CPU_MB
#include "monitor.h" // for debug output!!!
#endif

#define CODE_ADDR ((uint16_t*)0x100) // EEPROM location of RC5 code table

#define BUFSIZE 8 // event buffer size, for IR we barely need it

#define BUF_MASK (BUFSIZE - 1)
#if (BUFSIZE & BUF_MASK) // must be power of 2
	#error BUFSIZE is not a power of 2
#endif


// RC5 timing (should we place this in EEPROM, for easy in-the-field tuning?)
#define HALFBIT_US   889  // microseconds for a half bit
#define TOLERANCE_US 250  // tolerated jitter in us
#define TIMEOUT (HZ/4) // time after which a key is considered released

// carefully avoid intermediate 32 bit overflow
#define HALFBIT_CLKS   (HALFBIT_US   * (F_CPU/100) / 10000UL)
#define TOLERANCE_CLKS (TOLERANCE_US * (F_CPU/100) / 10000UL)

#define RC5_SHORT_MIN (HALFBIT_CLKS   - TOLERANCE_CLKS)
#define RC5_SHORT_MAX (HALFBIT_CLKS   + TOLERANCE_CLKS)
#define RC5_LONG_MIN  (HALFBIT_CLKS*2 - TOLERANCE_CLKS)
#define RC5_LONG_MAX  (HALFBIT_CLKS*2 + TOLERANCE_CLKS)


#define RAW_NONE 0xFFFF // an illegal raw code to specify no current
#define DEC_NONE 0xFF // an illegal decoded code (switch 0...15)

/*************** module state ***************/

static struct _global_switch_context
{   // codesize saver: place the most frequented on top, cluster them like locally used

    // state of the event queue
    // ToDo: check if all these index memories are necessary
    uint8_t eventqueue[BUFSIZE]; 
    uint8_t queue_head; // write position, maintained by ISR (timer tick)
    uint8_t queue_post; // posted up to here, to detect new entries, maintained by ISR (timer tick)
    uint8_t queue_sent; // handed out up to here, maintained by main loop
    uint8_t queue_tail; // acknowledged up to here, maintained by main loop
    uint8_t wait_for_ack; // block further messages, wait for roundtrip ack

    // configuration data
    switch_t event_enable[INPUT_OFF+1]; // which input shall send what, index is function offset
} switches;

static struct _global_rc5_context
{   // codesize saver: place the most frequented on top, cluster them like locally used

    // state of the RC5 receiver
    uint32_t last_ticks; // timer ticks of last rc5 edge
    uint16_t last_time;  // capture value of last edge
    uint8_t  halfbitpos; // bit counter in half bits
    uint16_t word;       // received word
    #ifdef DEBUG
    uint16_t log[28];  // test trace of intervals
    #endif

    // state of RC5 decoding
    uint16_t current_raw;  // currently seen code, raw
    uint8_t  current_dec;  // decoded into input number 0...15
    uint32_t last_rcv;   // timestamp of previous code
    uint16_t on_since; // measure keypress time
    uint16_t codes[CFG_INPUT]; // code table

    uint8_t disabled;      // set if in learning mode
    uint8_t learning;      // set if in learning mode while no code received yet
    uint8_t learn_channel; // set to the channel # to be learned
} rc5;

/*************** private internal functions ***************/

// store a single event
static void post_event(uint8_t channel, uint8_t id)
{
    switch_t mask;
    
    if (rc5.disabled)
    {
    	return; // ignore
    }
    
    mask = (switch_t)1 << channel;
    if (switches.event_enable[id] & mask) // enabled by configuration?
    {
        uint8_t tmphead;
        uint8_t entry = channel << 4 | id;

	    tmphead = (switches.queue_head + 1) & BUF_MASK;
        if (tmphead != switches.queue_tail)
	    {
		    switches.queue_head = tmphead; // store new index
		    switches.eventqueue[tmphead] = entry; // store new entry in queue
	    }
        else
	    {	// ERROR! Message queue full
		    ASSERT(0); // log this condition
	    }

    }
}

// send a message if the event queue is "dirty" (has new entries)
static void flush_events(void)
{
	struct msg message;
    
    if (switches.queue_post == switches.queue_head)
        return; // nothing to do
    
	message.id = e_event;
	message.data = switches.queue_head;
	if (msg_post(&message) == 0) // success in posting
    {
        switches.queue_post = switches.queue_head;
        switches.wait_for_ack = 1;
    }
    // else it will be retried next tick
}

// reset the RC5 receiver state
static void reset_rc5_state(void)
{
#ifdef DEBUG
    uint8_t i;
	for (i=0; i<28; i++)
		rc5.log[i] = 0;
#endif	
	rc5.halfbitpos = 0;
	rc5.word = 0;
	hal_set_edge(0); // start bit is falling edge
}

// new key has been pressed (may be invalid)
static void press(uint8_t channel)
{
	if (channel != DEC_NONE) // valid key?
	{
		post_event(channel, INPUT_ON_GREATER_0);
		rc5.on_since = 0;
	}
		
	rc5.current_dec = channel;
}
	
// the key has been released (may have been invalid)
static void release(void)
{	// released
	if (rc5.current_dec != DEC_NONE) // there was a valid key?
	{
		post_event(rc5.current_dec, INPUT_OFF);
		if (rc5.on_since > HZ*1)
        {
			post_event(rc5.current_dec, INPUT_OFF_GREATER_1);
        }
        else
        {
			post_event(rc5.current_dec, INPUT_OFF_LESS_1);
        }

		rc5.current_dec = DEC_NONE;
	}
}		

// try to assign a channel to a received code, DEC_NONE if not found
static uint8_t decode(uint16_t code)
{
	uint8_t i;
	
	code &= ~0x0800; // strip toggle bit
	for (i=0; i<CFG_INPUT; i++)
	{
		if (rc5.codes[i] == code) // match?
		{
			return i;
		}
	}
	
	return DEC_NONE; // not found
}

/*************** public API functions ***************/


void switch_init(void)
{
	uint8_t i;

	reset_rc5_state();
	rc5.current_raw = RAW_NONE;
	rc5.current_dec = DEC_NONE;
    
    for (i=0; i<CFG_INPUT; i++)
    {
        rc5.codes[i] = hal_eeprom_read_word(CODE_ADDR + i);
    }
}

// manage key press time and timeouts
void switch_tick(void)
{
    // RC5 receiver part
    if (rc5.halfbitpos && (timer_ticks - rc5.last_ticks > 1)) // stuck reception?
    {	// timeout
    	reset_rc5_state(); // cancel current, start anew
    }
    
	if (rc5.current_dec != DEC_NONE) // active key?
	{
		if (timer_ticks - rc5.last_rcv >= TIMEOUT) // timeout, no repeat received
		{
			release(); // send key release
			rc5.current_raw = RAW_NONE; // activate the decoder again
		}
		else
		{	// key is held pressed
			rc5.on_since++; // increment the time
			if (rc5.on_since == HZ*1)
			{
				post_event(rc5.current_dec, INPUT_ON_GREATER_1);
			}
			else if (rc5.on_since == HZ*2)
			{
				post_event(rc5.current_dec, INPUT_ON_GREATER_2);
			}
		}
	}
    
	if (!switches.wait_for_ack // no pending msg?
	 && !uart_is_busy()) // no other traffic?
	{
        flush_events(); // if new entries, this will send a message to main loop
    } // if (!switches.wait_for_ack)

}


// set the enable bits, the filter configuration
void switch_set_enable(switch_t* p_enables)
{
    uint8_t i;
    for (i=INPUT_ON_GREATER_0; i<=INPUT_OFF; i++)
    {
        switches.event_enable[i] = p_enables[i];
    }
}

switch_t switch_getcurrent(void)
{
    if (rc5.current_dec != DEC_NONE) // current key?
    {
    	return (switch_t)1 << rc5.current_dec; // return it at bit position
    }
    else
    {
    	return 0;
    }
}

// copy the queue content into a caller-supplied buffer, returns size
uint8_t switch_getevents(uint8_t* dest, uint8_t bufsize, uint8_t pos)
{
    uint8_t count = 0;
    uint8_t i = switches.queue_tail;
    while (i != pos && count < bufsize)
    {
        i = (i + 1) & BUF_MASK;
        *dest++ = switches.eventqueue[i];
        count++;
    }
    switches.queue_sent = i; // store how far we gave
    return count;
}

// roundtrip ack, we now have seen the ack from the control unit
void switch_ack(void) 
{
    switches.queue_tail = switches.queue_sent; // advance the tail
    switches.queue_post = switches.queue_tail; // re-post if only a part was taken
	switches.wait_for_ack = 0; // allow next message
}

// roundtrip negative ack, we can't confirm the control unit got it
void switch_nack(void) 
{
    switches.queue_post = switches.queue_tail; // the ISR will re-post
    switches.wait_for_ack = 0; // allow next message
}

// timer capture interrupt
TIMER_CAPTURE_ISR
{
	uint16_t time; // since last edge
	uint8_t edge; // edge polarity
	
	edge = !hal_get_edge(); // read and flip edge
	time = hal_get_capture(); // this also acknowledges the interrupt

	if (rc5.halfbitpos++) // not the first, we care about intervals
	{
        uint16_t diff;
		diff = time - rc5.last_time;
#ifdef DEBUG
        if (rc5.halfbitpos < 28) 
		{
			rc5.log[rc5.halfbitpos] = diff;
		}
#endif
		if (diff >= RC5_LONG_MIN && diff <= RC5_LONG_MAX // double time
		 && (rc5.halfbitpos & 0x01) == 0) // only allowed at former bit center
		{
			rc5.halfbitpos++;   // add an extra halfbit
		}
		else if (diff < RC5_SHORT_MIN || diff > RC5_SHORT_MAX)
		{	// no double and no single time: junk received
			reset_rc5_state(); // try again
			return;
		}

		if (rc5.halfbitpos & 0x01) // at bit center, the edge is the information
		{
			rc5.word = (rc5.word << 1) | edge; // add to result

			if (rc5.halfbitpos >= 27) // word completed?
			{
#ifdef CPU_MB
				acc_printf(ACC_TYPE_LISTBOX, ACC_TYPE_USHORT, &rc5.word, 0, 1);
#endif
				if (rc5.word != rc5.current_raw)
				{
					if (rc5.learning)
					{
						struct msg message = { e_learned };
						rc5.learning = 0;
						rc5.codes[rc5.learn_channel] = rc5.word & ~0x0800; // store without toggle bit
                        hal_eeprom_write_word(CODE_ADDR + rc5.learn_channel, rc5.word & ~0x0800);
                        msg_post(&message); // post to main loop that we're trained
					}
					else
					{					
						uint8_t decoded = decode(rc5.word); // decoded value
						release(); // release old, if any
						press(decoded); // press new, if any
					}
					rc5.current_raw = rc5.word; // remember the raw code
				}
				rc5.last_rcv = timer_ticks; // remember receive time for timeout
				
				reset_rc5_state(); // start over
				return;
			}
		}
	}
	
	hal_set_edge(edge); // write for next time
	rc5.last_time = time; // remember capture time for diff
	rc5.last_ticks = timer_ticks; // remember for timeout
}

// IR code learning mode: assign the first seen IR code to this channel
void rc5_learn(uint8_t channel)
{
	rc5.learn_channel = channel;
	rc5.disabled = 1; // disable the normal event sending
	rc5.learning = 1; // got no code yet
}

// back to normal mode
void rc5_normal(void)
{
	rc5.disabled = 0;
	rc5.learning = 0;
}
