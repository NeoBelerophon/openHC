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

/*! \file hal_platform.h
    \brief Timer hardware abstraction, Atmel Mega8 part.
*/

#ifndef __AVR
#error "Wrong include path, this is for Atmel AVR only!"
#endif

#ifndef _TIMER_PLATFORM_H
#define _TIMER_PLATFORM_H

#include <avr/io.h>     // for device register definitions
#include <avr/interrupt.h> // for interrupt handlers
#include "hal.h"

#define HZ 100UL // ticks per second

#define TIMER_TICK_ISR    SIGNAL(TIMER0_OVF_vect)
#define TIMER_HIGHRES_ISR SIGNAL(TIMER1_OVF_vect)
#define TIMER_CAPTURE_ISR SIGNAL(TIMER1_CAPT1_vect)

inline static void hal_timer_init(uint8_t initvalue)
{

    TCCR0 = _BV(CS02) | _BV(CS00); // prescaler to /1024, as large as possible while resolving HZ
    TCNT0 = 255 - initvalue;
    
	// note: TIMSK0 and TIMSK1 are the same register at Mega8, be careful about direct assignments!

    TIMSK0 |= _BV(TOIE0); // enable overflow interrupt

    // timer1 init
    TCCR1B = _BV(ICNC1) | _BV(CS10); // capture filter, set prescale=1, (free running)
#ifdef CFG_IR
    TIMSK1 |= _BV(TOIE1) | _BV(TICIE1); // overflow + capture interrupt, ToDo: move this?
#else
	TIMSK1 |= _BV(TOIE1); // overflow interrupt
#endif // CFG_IR

}

// high resolution counter, lower 16 bit
inline static uint16_t hal_timer_hires_lsb(void)
{
    return TCNT1;
}

inline static void hal_timer_clear_irq(void)
{   
    // no clear necessary on AVR, but here we correct the counter to retrigger in 1/HZ
    TCNT0 -= F_CPU/1024/HZ; // set it back by 1/HZ
    // there is a chance for a glitch if the timer moves during the setback,
    //  but only if the ISR latency is ~1024 cycles, hope to be much faster
}


// return != 0 if an unhandled overflow interrupt is pending
inline static uint8_t hal_timer_hires_ovf(void)
{
	return TIFR1 & _BV(TOV1);
}
 


#endif // #ifndef _TIMER_PLATFORM_H
