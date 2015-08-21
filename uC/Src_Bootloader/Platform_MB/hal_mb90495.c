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

/*! \file hal_mb90495.c
    \brief Hardware abstraction layer, Fujitsu MB90F495 implementation
*/

#define __FILENUM__ 3 // every file needs to have a unique (8bit) ID

#include <stdint.h>
#include "mb90495.h"
#include "monitor.h" // for host tracing
#include "hal.h"

#ifdef DEBUG

static __far char hal_msg_buf[150];

void hal_assert(const char* file, uint16_t line)
{
	acc_printf(ACC_TYPE_LISTBOX, ACC_TYPE_STRING, (const __far void *)"ASSERT failed line:", 0, 1);
	acc_printf(ACC_TYPE_LISTBOX, ACC_TYPE_USHORT, (const __far void *)&line, 0, 1);
	acc_printf(ACC_TYPE_LISTBOX, ACC_TYPE_STRING, (const __far void *)file, 0, 1);
	hal_panic();
}
#endif

void hal_sysinit(void)
{
	InitIrqLevels();

	DDR1 = 0xFF; // port 1 all outputs
	PDR1 = 0x00; // all LEDs off
	TBTC = 0x92; // tick interrupt every 16.384 ms (~61 Hz)

	__set_il(7); // allow all levels
	__EI(); // globaly enable interrupts
	
#ifdef DEBUG
	acc_InitMonitorMsgBuffer(hal_msg_buf, sizeof(hal_msg_buf));
#endif
}

// set the CPU into sleep mode, until next interrupt
void hal_suspend(void)
{
	LPMCR |= 0x40; // set the sleep mode bit
}

// prepare sleeping
void hal_sleep_enable(void)
{
	// not existing
}

// the actual sleep instruction
void hal_sleep_cpu(void)
{
	hal_suspend();
}

// prevent inadvertent sleeping
void hal_sleep_disable(void)
{
	// not existing
}


// signal a fatal error
void hal_panic(void)
{
    // __DI(); // disable interrupts
	while (1)
	{	// wild flashing LEDs
		volatile long i;
		for (i=0; i<30000; i++);
		PDR1 = ~PDR1;
	}
}

// read the module address from the DIP switches
uint8_t hal_get_addr(void)
{
	return (PDR3 >> 3) ^ 0x1F; // inverted P33...P37
}

// set the feedback LEDs
void hal_set_led(uint8_t leds)
{
	PDR1 = leds;
}

// read the timer value
uint8_t hal_get_timer(void)
{
	return 0; // ToDo
}

void hal_timer_clear_irq(void)
{
    TBTC &= 0xF7; // clear interrupt request
}

// full restart, into bootloader if present
void hal_reboot_app(void)
{
	// ToDo
}


// program flash access dummies
void hal_load_flash(uint16_t* buf, uint8_t address)
{
	(void) buf;
	(void) address;
}

void hal_write_flash(uint16_t PageAddress)
{
	(void) PageAddress;
}

uint8_t hal_check_flash(uint16_t PageAddress, uint8_t* buf)
{
	(void) PageAddress;
	(void) buf;
	return 0; // OK
}

// eeprom access dummies
uint8_t hal_eeprom_read (uint16_t addr)
{
	return addr & 0xFF;
}

void hal_eeprom_write (uint16_t addr, uint8_t val)
{
	(void) addr;
	(void) val;
}


// crude hacks for unabstracted gnu/avr functions
void cli(void)
{
}

void sei(void)
{
}
