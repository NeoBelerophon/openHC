/***************************************************************************
 *
 * OpenHC:                          ___                 _  _  ___
 *  Open source                    / _ \ _ __  ___ _ _ | || |/ __|
 *  Home                          | (_) | '_ \/ -_) ' \| __ | (__ 
 *  Control                        \___/| .__/\___|_||_|_||_|\___|
 * http://openhc.sourceforge.net/       |_| 
 *
 * Copyright (C) 2005 by Joerg Hohensohn
 * Copyright (C) 2005 by Jörg Hohensohn
 *
 * All files in this archive are subject to the GNU General Public License.
 * See http://www.gnu.org/licenses/gpl-3.0.txt for full license agreement.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/ 

/*! \file hal_mb90495.c
    \brief Hardware abstraction layer, Fujitsu MB90F495 implementation.
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
	acc_printf(ACC_TYPE_LISTBOX, ACC_TYPE_STRING, "ASSERT failed line:", 0, 1);
	acc_printf(ACC_TYPE_LISTBOX, ACC_TYPE_USHORT, &line, 0, 1);
	acc_printf(ACC_TYPE_LISTBOX, ACC_TYPE_STRING, file, 0, 1);
	hal_panic();
}
#endif

void hal_sysinit(void)
{
	InitIrqLevels();
#ifdef CFG_IR
	DDR1 = 0xFB; // leave bit 2 as input, for capture input
	ICS23 = 0x12; // capture falling edge, enable interrupt
#else
	DDR1 = 0xFF; // port 1 all outputs
#endif
	PDR1 = 0x00; // all LEDs off
	
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

#ifdef CFG_INPUT
// read the input switches
uint16_t hal_get_switch(void)
{
	return PDR2 ^ 0xFF; // 8 bit, low active
}
#endif

// enable watchdog timer
void hal_watchdog_enable(void)
{
	// ToDo
}

// reset watchdog timer
void hal_watchdog_reset(void)
{
	// ToDo
}

// full restart, into bootloader if present
void hal_reboot(void)
{
	// ToDo
}

#ifdef CFG_OUTPUT
// set the outputs
void hal_set_output(uint8_t byte)
{
	hal_set_led(byte);
}

// pulsed PWM output to save relay power
void hal_set_pwm(uint8_t byte)
{
	(void)byte; // ToDo: absent feature
}
#endif

#ifdef CFG_IR
// read the timer capture
uint16_t hal_get_capture(void)
{
	ICS23 &= 0xBF; // this clears the edge flag, the interrupt request
	return IPCP2;
}

// query the capture edge, returns 0=falling, 1=rising
uint8_t hal_get_edge(void)
{
	uint8_t value = ICS23;
	return value & 0x01; // return edge setting
}

// set the capture edge, 0=falling, 1=rising
void hal_set_edge(uint8_t edge)
{
	uint8_t value = ICS23 & 0xFC;
	if (edge)
		value |= 0x01; // rising
	else
		value |= 0x02; // falling
	
	ICS23 = value;
}
#endif

uint8_t hal_eeprom_read_byte(uint8_t* addr)
{
	(void)addr;
	return 0x40; // default PWM ration
}

uint16_t hal_eeprom_read_word(uint16_t* addr)
{
    return 0x1000 + (addr - (uint16_t*)0x100); // default RC5 codes for TV device
}


void hal_eeprom_write_byte(uint8_t* addr, uint8_t data)
{
	(void)addr;
	(void)data;
}

void hal_eeprom_write_word(uint16_t* addr, uint16_t data)
{
	(void)addr;
	(void)data;
}


// crude hacks for unabstracted gnu/avr functions
void cli(void)
{
}

void sei(void)
{
}
