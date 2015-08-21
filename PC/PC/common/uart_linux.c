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

/*! \file uart_linux.c
    \brief UART wrapper implementation for the Linux platform.
    
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>

#include "scalar_types.h" // (u)int8/16/32_t
#include "uart.h"

// my private info per opened COM port
typedef struct 
{
    struct termios oldtio,newtio;
    int fd;
} com_t;

// COMx for windows, returns NULL on error
tUartHandle UartOpen(char* szPortName)
{
	com_t* p_com = (com_t*) malloc(sizeof(com_t));
    if (p_com == NULL)
        return NULL;

    p_com->fd = open(szPortName, O_RDWR | O_NOCTTY);
    if (p_com->fd <0)
    {
        free (p_com);
        return NULL;
    }

    tcgetattr(p_com->fd, &p_com->oldtio); /* save current port settings */


	return p_com;
}

// returns true on success, false on error
bool UartConfig(tUartHandle handle, long lBaudRate, tParity nParity, tStopBits nStopBits, int nByteSize, int nTimeout)
{
	com_t* p_com = (com_t*)handle;

    // hack: ignore all the given parameters
    bzero(&p_com->newtio, sizeof(p_com->newtio));
    p_com->newtio.c_cflag = B19200 | CRTSCTS | CS8 | CLOCAL | CREAD;
    p_com->newtio.c_iflag = IGNPAR;
    p_com->newtio.c_oflag = 0;
    
    /* set input mode (non-canonical, no echo,...) */
    p_com->newtio.c_lflag = 0;
    
    p_com->newtio.c_cc[VTIME] = nTimeout/100;   /* inter-character timer in 10th of seconds */
    p_com->newtio.c_cc[VMIN]  = 1;   /* blocking read until 1 chars received */
    
    tcflush(p_com->fd, TCIFLUSH);
    tcsetattr(p_com->fd, TCSANOW, &p_com->newtio);

	return true;
}

// returns how much data was actually transmitted
bool UartWrite(tUartHandle handle, uint8_t* pData, int* pnSize)
{
	com_t* p_com = (com_t*)handle;
	int result_nbr;

	result_nbr = write(p_com->fd, pData, *pnSize);

	*pnSize = result_nbr;
	return true;
}

// returns how much data was actually received
bool UartRead(tUartHandle handle, uint8_t* pBuffer, int* pnSize)
{
	com_t* p_com = (com_t*)handle;
	int read_nbr;

	p_com->newtio.c_cc[VMIN] = *pnSize;   /* blocking read until lSize chars received */

	tcsetattr(p_com->fd, TCSANOW, &p_com->newtio);

	read_nbr = read(p_com->fd, pBuffer, *pnSize);
    
	*pnSize = read_nbr;
	return true;
}


void UartClose(tUartHandle handle)
{
    if (handle != NULL)
	{
    	com_t* p_com = (com_t*)handle;

        tcsetattr(p_com->fd, TCSANOW, &p_com->oldtio);
        close(p_com->fd);
        free(p_com);
	}

	return;
}


