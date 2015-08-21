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

/*! \file common.c
    \brief Tool functions for the PHC module bus protocol.
*/

#include "../common/hal.h"
#include "../common/scalar_types.h"
#include "../common/uart.h"

extern tUartHandle ghUart; // a bit dirty

// CRC across a buffer
unsigned short crc16(uint8_t* buf, int n)
{
    unsigned short crc = 0xFFFF;

    while(n--)
    {
        int i;

        crc ^= *buf++;
        for (i=8; i>0; i--)							      
        {									      
            if (crc & 0x0001)	      
                crc = (crc >> 1) ^ 0x8408; // reverse 0x1021 poly 
            else								      
                crc >>= 1;						      
        }									      
    }
    
    return crc ^ 0xFFFF;
}


// add checksum + send
int assemble_packet(uint8_t* buf, int n)
{
    unsigned short crc;
    buf[1] = (buf[1] & 0x80) | n;
    crc = crc16(buf, n+2);
    buf[n + 2] = crc & 0xFF; // lowbyte
    buf[n + 1] = crc >> 8;   // highbyte

    n += 4;
    UartWrite(ghUart, buf, &n);
    return 0;
}
