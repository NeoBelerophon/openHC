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

/*! \file uart.h
    \brief A general definition for the required UART functionality.
           This is used to get platform abstraction.
*/

#ifndef _UART_H
#define _UART_H

// data types

typedef void* tUartHandle;
#define INVALID_UART_HANDLE (tUartHandle)-1

typedef enum
{
	eNOPARITY,
	eODDPARITY, 
	eEVENPARITY,
	eMARKPARITY,
	eSPACEPARITY,
} tParity;

typedef enum
{
	eONESTOPBIT,
	eONE5STOPBITS,
	eTWOSTOPBITS,
} tStopBits;


// prototypes

tUartHandle UartOpen(   // returns NULL on error
	char* szPortName);  // COMx for windows, ttySx for Linux

bool UartConfig(         // returns true on success, false on error
	tUartHandle handle,  // the handle returned from UartOpen()
	long lBaudRate,      // must be one of the "standard" baudrates
	tParity nParity,     // what kind of parity
	tStopBits nStopBits, // how many stop bits
	int nByteSize,       // size of the "payload", can be 5 to 8
    int nTimeout);       // timeout in milliseconds

bool UartWrite(           // returns true on success, false on error
	tUartHandle handle,   // the handle returned from UartOpen()
	uint8_t* pData,       // pointer to the data to be transmitted
	int* pnSize);         // in: how many bytes, out: how many transmitted

bool UartRead(            // returns true on success, false on error
	tUartHandle handle,   // the handle returned from UartOpen()
	uint8_t* pBuffer,     // pointer to the destination
	int* pnSize);         // in: how many bytes requested, out: how many received


void UartClose(tUartHandle handle);



#endif // _UART_H

