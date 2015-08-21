/* Hardware abstraction layer, Fujitsu MB part */
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

#pragma inline hal_uart_init_receive_timeout
void hal_uart_init_receive_timeout(uint8_t timeout)
{
	TMCSR1 = 0x0808; // phi/32, single shot, interrupt enable
    // the gap between 2 retries of the STM can be as short as 25 bits!
	// reload = bit clock * (# of bits per byte + timeout bits)
	TMRLR1 = 2 * 13 * (1+8+2 + 5); // tested minimum 3, maximum 7 timeout bits
    (void)timeout; // ToDo: add something derived from user value
	//TMRLR1 = 2 * 13 * (1+8+2) * 6; //  // reload = bit clock * # of bits * timeout bytes
}

#pragma inline hal_uart_init_transmit_timeout
void hal_uart_init_transmit_timeout(void)
{
	TMCSR0 = 0x0808; // phi/32, single shot, interrupt enable
	TMRLR0 = 2 * 13 * (1+8+2); // reload = bit clock * # of bits
}

#pragma inline hal_uart_start_transmit_timeout
void hal_uart_start_transmit_timeout(void)
{
	TMCSR0 |= 0x03; // enable counter, set trigger
}

#pragma inline hal_uart_stop_transmit_timeout
void hal_uart_stop_transmit_timeout(void)
{
    TMCSR0_CNTE = 0;
}

#pragma inline hal_uart_clear_transmit_timeout
void hal_uart_clear_transmit_timeout(void)
{
	TMCSR0_UF = 0; // clear interrupt
}

#pragma inline hal_uart_start_receive_timeout
void hal_uart_start_receive_timeout(void)
{
	TMCSR1 |= 0x03; // enable counter, set trigger
}

#pragma inline hal_uart_clear_receive_timeout
void hal_uart_clear_receive_timeout(void)
{
	TMCSR1_UF = 0; // clear interrupt
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

#pragma inline hal_uart_start_tx_irq
void hal_uart_start_tx_irq(void)
{
	SSR0_TIE = 1; // enable tx interrupt
}

#pragma inline hal_uart_stop_tx_irq
void hal_uart_stop_tx_irq(void)
{
	SSR0_TIE = 0; // disable tx interrupt
}

#pragma inline hal_uart_rx_edge_enable
void hal_uart_rx_edge_enable(void)
{
}

#pragma inline hal_uart_rx_edge_disable
void hal_uart_rx_edge_disable(void)
{
}


#endif // #ifndef _HAL_MB90495_H
