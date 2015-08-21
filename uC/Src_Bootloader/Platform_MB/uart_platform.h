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

/*! \file uart_platform.h
    \brief UART hardware abstraction layer, Fujitsu MB part.
*/

#ifndef _UART_MB90495_H
#define _UART_MB90495_H

#include "mb90495.h"

#define UART_RX_ISR         __interrupt void uart_irq_rx(void)
#define UART_TX_ISR         __interrupt void uart_irq_tx(void)
#define UART_TX_TIMEOUT_ISR __interrupt void uart_irq_tx_end(void)
#define UART_RX_TIMEOUT_ISR __interrupt void uart_irq_rx_timeout(void)

#pragma inline hal_uart_rs485_disable
void hal_uart_rs485_disable(void) 
{
    PDR3_P31 = 0; 
}

#pragma inline hal_uart_rs485_enable
void hal_uart_rs485_enable(void) 
{
	PDR3_P31 = 1; // enable the RS485 driver
}

#pragma inline hal_uart_init_hardware
void hal_uart_init_hardware(void)
{
	DDR3_D31 = 1; // output for RS485 driver enable = inactive

	// init UART0 hardware
	CDCR0 = 0x8E; // prescaler = 2   (phi = 16MHz)
	SCR0 = 0x33; // 8N2, enable RX+TX
	SMR0 = 0x09; // async, enable output, 19231 baud (div=8*13*2*2)
	SSR0 = 0x02; // enable rx interrupt
}

#pragma inline hal_uart_has_errors
uint8_t hal_uart_has_errors(void)
{
    return SSR0 & 0xE0;
}

#pragma inline hal_uart_clear_errors
void hal_uart_clear_errors(void)
{
    SCR0_REC = 0; // clear error flags (and interrupt)
}

#pragma inline hal_uart_has_data
uint8_t hal_uart_has_data(void)
{
    return SSR0_RDRF;
}

#pragma inline hal_uart_read_data
uint8_t hal_uart_read_data(void)
{
    return SIDR0; // read the received data, clear interrupt
}

#pragma inline hal_uart_send_data
void hal_uart_send_data(uint8_t b)
{
     SODR0 = b;
}

#pragma inline hal_uart_baudrate
void hal_uart_baudrate(uint8_t ubrrl)
{
    (void)ubrrl; // dummy
}


#endif // #ifndef _HAL_MB90495_H
