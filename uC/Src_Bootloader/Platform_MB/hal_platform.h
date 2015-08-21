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
    \brief Hardware abstraction layer, Fujitsu inline implementations.
*/

#ifndef CPU_MB
#error "Wrong include path, this is for Fujitsu MB90F49x only!"
#endif

#ifndef _HAL_PLATFORM_H
#define _HAL_PLATFORM_H

#ifdef DEBUG
#include "monitor.h" // for debug dump
#endif


#define HZ 61 // ticks per second

#define TIMER_TICK_ISR __interrupt void timer_irq_tick(void)


// nothing inlined by now


#endif // #ifndef _HAL_PLATFORM_H

