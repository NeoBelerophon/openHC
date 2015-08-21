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

/*! \file protocol.c
    \brief Implementation of the required bus protocol.
*/

#include <stdio.h>
#include "scalar_types.h"
#include "protocol.h"


// CRC across a buffer
uint16_t crc16(const uint8_t* buf, int n)
{
    uint16_t crc = 0xFFFF;

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



// tool functions for the protocol over RS232



// do the HDLC-style padding, dst and src can be identical
int pad_buffer(uint8_t* buf_dst, const uint8_t* buf_src, int* size)
{
    int newsize = *size;
    int oldpos;
    int i;

    // first pass: determine new size
    for (i=1; i<*size-1; i++) // check only between start and stop byte
    {
        if (buf_src[i] == 0x7D || buf_src[i] == 0xC0 || buf_src[i] == 0xC1)
        {
            newsize++;
        }
    }

    // short cut
    if (*size == newsize) // no padding necessary?
    {
        return 0; // just leave it
    }
    
    // second pass: go backwards and do the padding.
    //  this way no extra work buffer is needed

    buf_dst[0] = buf_src[0]; // copy first byte, should be 0xC0
    buf_dst[newsize-1] = buf_src[*size-1]; // copy last byte, should be 0xC1
    oldpos = *size - 2; // start in front of that last byte
    for (i=newsize-2; i>1; i--)
    {
        if (buf_src[oldpos] == 0x7D || buf_src[oldpos] == 0xC0 || buf_src[oldpos] == 0xC1)
        {
            buf_dst[i] = buf_src[oldpos] ^ 0x20;
            i--;
            buf_dst[i] = 0x7D;
        }
        else
        {
            buf_dst[i] = buf_src[oldpos];
        }
        oldpos--;
    }
    // now, oldpos should be 1

    *size = newsize;

    return 0; // OK
}

// undo the HDLC-style padding, dst and src can be identical
int unpad_buffer(uint8_t* buf_dst, const uint8_t* buf_src, int* size)
{
    bool stuff;
    const uint8_t* p_run;
    uint8_t* p_save;

    stuff = false;
    p_save = buf_dst;
    for (p_run = buf_src; p_run < buf_src + *size; p_run++)
    {
        if (*p_run == 0x7D)
        {
            stuff = true;
        }
        else
        {
            if (stuff)
            {
                stuff = false;
                *p_save++ = *p_run ^ 0x20;
            }
            else
            {
                *p_save++ = *p_run;
            }
        }
    }
    *size = p_save - buf_dst; // new, possible reduced size
    return 0; // OK
}

// convert a module bus command to an unpadded (RS232) STM bus command
// both buffer pointers may point to the same buffer, inplace is allowed
int module_to_stm(uint8_t* buffer_stm, const uint8_t* buffer_mod, int* size)
{
    uint16_t crc;
    int newsize = 5 + *size + 1;
    
    memmove(buffer_stm+5, buffer_mod, *size-2); // copy without CRC

    buffer_stm[0] = 0xC0; // start marker
    buffer_stm[1] = 0xFE; // from RS232
    buffer_stm[2] = 0x00; // to STM0
    buffer_stm[3] = 0x06; // opcode: PHC packet; 0x22 is supposed to do better, evaluates ack, but returns no data?
    buffer_stm[4] = 0x00; // sequence #

    crc = crc16(buffer_stm+1, 4+*size-2);

    buffer_stm[4 + *size-2 + 1] = crc & 0xFF; // lowbyte
    buffer_stm[4 + *size-2 + 2] = crc >> 8; // highbyte
    buffer_stm[4 + *size-2 + 3] = 0xC1; // end marker

    pad_buffer(buffer_stm, buffer_stm, &newsize);

    *size = newsize;
    return 0; // OK
}

// convert an unpadded (RS232) STM bus command to a module bus command
// both buffer pointers may point to the same buffer, inplace is allowed
int stm_to_module(uint8_t* buffer_mod, const uint8_t* buffer_stm, int* size)
{
    uint16_t crc;

    crc = crc16(buffer_stm+1, *size - 4);
    if (buffer_stm[*size-3] != (crc & 0xFF) || buffer_stm[*size-2] != crc >> 8)
    {
        return -1; // CRC mismatch
    }

    // check packet
    //if (memcmp(buffer_stm, "\xC0\x00\xFE\x06\x00", 5) != 0) // works only for STM 1.xx ?
    if (memcmp(buffer_stm, "\xC0\x00\xFE\x06", 4) != 0) // STM 2.xx seems to increment the toggle
    {
        return -2;
    }

    memmove(buffer_mod, buffer_stm+5, *size-5-2-1);

    *size = *size-5-2-1;
    return 0; // OK
}
