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

/*! \file protocol.h
    \brief Interface of the bus protocol functions
*/

#ifndef _PROTOCOL_H

#define MAX_PACKET_SIZE (2+128+2) // worst-case PHC packet size


 // CRC across a buffer
uint16_t crc16(const uint8_t* buf, int n);

// do the HDLC-style padding, dst and src can be identical
int pad_buffer(uint8_t* buf_dst, const uint8_t* buf_src, int* size);

// undo the HDLC-style padding, dst and src can be identical
int unpad_buffer(uint8_t* buf_dst, const uint8_t* buf_src, int* size);

// convert a module bus command to a (RS232) STM bus command
// both buffer pointers may point to the same buffer, inplace is allowed
int module_to_stm(uint8_t* buffer_stm, const uint8_t* buffer_mod, int* size);

// convert a (RS232) STM bus command bus command to a module bus command
// both buffer pointers may point to the same buffer, inplace is allowed
int stm_to_module(uint8_t* buffer_mod, const uint8_t* buffer_stm, int* size);



#endif // #ifndef _PROTOCOL_H
