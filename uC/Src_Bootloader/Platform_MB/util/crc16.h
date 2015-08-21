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

/*! \file crc16.h
    \brief CRC implementation, compatible to the AVR library.

    This is for non-AVR compilers, which lack that library function.
    Bootloader copy here.
*/

#ifndef _UTIL_CRC16_H_
#define _UTIL_CRC16_H_

#include <stdint.h>


// incremental CRC, usage: start with 0xFFFF, invert result

#pragma inline _crc_ccitt_update
static uint16_t _crc_ccitt_update(uint16_t crc, const uint8_t byte)
{
    uint8_t i;

    crc ^= byte;
    for (i=8; i>0; i--)							      
    {									      
        if (crc & 0x0001)	      
            crc = (crc >> 1) ^ 0x8408; // reverse 0x1021 poly 
        else								      
            crc >>= 1;						      
    }									      

    return crc;
}



#endif /* _UTIL_CRC16_H_ */
