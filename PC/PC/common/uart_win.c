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

/*! \file uart_win.c
    \brief UART wrapper implementation for the Win32 platform.
    
*/

#include <windows.h>
#include "scalar_types.h" // (u)int8/16/32_t
#include "uart.h"

// COMx for windows, returns NULL on error
tUartHandle UartOpen(char* szPortName)
{
	HANDLE serial_handle; 
	DCB  dcb;

	memset(&dcb,0,sizeof(dcb));

	/* -------------------------------------------------------------------- */
	// set DCB to configure the serial port
	dcb.DCBlength       = sizeof(dcb);

	dcb.fOutxCtsFlow    = 0;
	dcb.fOutxDsrFlow    = 0;
	dcb.fDtrControl     = DTR_CONTROL_ENABLE; // enable for power
	dcb.fDsrSensitivity = 0;
	dcb.fRtsControl     = RTS_CONTROL_ENABLE; // enable for power
	dcb.fOutX           = 0;
	dcb.fInX            = 0;

	/* ----------------- misc parameters ----- */
	dcb.fErrorChar      = 0;
	dcb.fBinary         = 1;
	dcb.fNull           = 0;
	dcb.fAbortOnError   = 0;
	dcb.wReserved       = 0;
	dcb.XonLim          = 2;
	dcb.XoffLim         = 4;
	dcb.XonChar         = 0x13;
	dcb.XoffChar        = 0x19;
	dcb.EvtChar         = 0;

	/* ----------------- defaults ----- */
	dcb.BaudRate = 4800;
	dcb.Parity   = NOPARITY;
	dcb.fParity  = 0;
	dcb.StopBits = ONESTOPBIT;
	dcb.ByteSize = 8;


	/* -------------------------------------------------------------------- */
	// opening serial port
	serial_handle = CreateFile(szPortName, GENERIC_READ | GENERIC_WRITE,
		0, NULL, OPEN_EXISTING, FILE_FLAG_WRITE_THROUGH, NULL);

	if (serial_handle == INVALID_HANDLE_VALUE)
	{
		//printf("Cannot open port \n");
		return NULL;
	}

	SetCommMask(serial_handle, 0);

	if(!SetCommState(serial_handle, &dcb))
	{
		//printf("Error setting up COM params\n");
		CloseHandle(serial_handle);
		return NULL;
	}

	return serial_handle;
}

// returns true on success, false on error
bool UartConfig(tUartHandle handle, long lBaudRate, tParity nParity, tStopBits nStopBits, int nByteSize, int nTimeoutMilliseconds)
{
	DCB  dcb;
	COMMTIMEOUTS cto;
    float bits;
	
	if (!GetCommState (handle, &dcb))
	{
		return false;
	}

	dcb.BaudRate = lBaudRate;
	dcb.Parity	 = nParity;
	dcb.StopBits = nStopBits;
	dcb.ByteSize = nByteSize;

	if(!SetCommState(handle, &dcb))
	{
		//DWORD dwErr = GetLastError();
		//printf("Error %d setting up COM params for baudrate byte\n", dwErr);
		return false;
	}

    bits = 1; // start bit
    bits += (float)nByteSize;
    bits += (nParity == eNOPARITY) ? 0 : 1;
    bits += (nStopBits == eONESTOPBIT) ? 1 : ((nStopBits == eONE5STOPBITS) ? 1.5f : 2);

    cto.ReadIntervalTimeout = (DWORD)(bits * 1000 / lBaudRate + 2);
    cto.ReadTotalTimeoutMultiplier = (DWORD)(bits * 1000 / lBaudRate + 1);
    cto.ReadTotalTimeoutConstant = nTimeoutMilliseconds;
    cto.WriteTotalTimeoutMultiplier = (DWORD)(bits * 1000 / lBaudRate + 1);
    cto.WriteTotalTimeoutConstant = nTimeoutMilliseconds;
	
    SetCommTimeouts(handle, &cto);

    return true;
}

// returns how much data was actually transmitted
BOOL UartWrite(tUartHandle handle, uint8_t* pData, int* pnSize)
{
	BOOL success;
	DWORD result_nbr;

	success = WriteFile(handle, pData, *pnSize, &result_nbr, NULL);
	*pnSize = result_nbr;

	return success;
}

// returns how much data was actually received
BOOL UartRead(tUartHandle handle, uint8_t* pBuffer, int* pnSize)
{
	BOOL success;
	DWORD read_nbr;

	success = ReadFile(handle, pBuffer, *pnSize, &read_nbr, NULL);
    *pnSize = read_nbr;

    return success;
}


void UartClose(tUartHandle handle)
{
	if (handle != NULL)
	{
		CloseHandle(handle);
	}

	return;
}

