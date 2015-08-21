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

/*! \file timer_platform.h
    \brief Timer hardware abstraction layer, Fujitsu MB90F49x part.
*/

#ifndef CPU_MB
#error "Wrong include path, this is for Fujitsu MB90F49x only!"
#endif

#ifndef _TIMER_PLATFORM_H
#define _TIMER_PLATFORM_H

#include "mb90495.h"

#define HZ 61L // ticks per second
#define F_CPU 16000000UL

#define TIMER_TICK_ISR    __interrupt void timer_irq_tick(void)
#define TIMER_CAPTURE_ISR __interrupt void rc5_irq_capture(void)

#pragma inline hal_timer_init
static void hal_timer_init(uint8_t initvalue) 
{
	TBTC = 0x92; // tick interrupt every 16.384 ms (~61 Hz)
    // ToDo: init timer value
    (void)initvalue;
}

#pragma inline hal_timer_performance_counter
static uint16_t hal_timer_performance_counter(void)
{
	return 0; // ToDo
}

#pragma inline hal_timer_clear_irq
static void hal_timer_clear_irq(void)
{
	TBTC &= 0xF7; // clear interrupt request
}



#endif // #ifndef _TIMER_PLATFORM_H
