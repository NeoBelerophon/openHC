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

/*! \file bootload.h
    \brief Interface definition of the middleware.
*/

#ifndef _BOOTLOAD_H


// bootloader command definitions, repeated from the uC header file

#define CMD_ACK          0
#define CMD_PING         1
#define CMD_REBOOT       2
#define CMD_EEPROM_WRITE 3
#define CMD_LOAD_PAGE    4
#define CMD_FLASH_PAGE   5
#define CMD_EEPROM_READ  6
#define CMD_FLASH_READ   7


#define MCU_MB90495     1 // Fujitsu MB90495 / MB90495
#define MCU_AVR_MEGA8   2 // Atmel AVR Mega8
#define MCU_AVR_MEGA48  3 // Atmel AVR Mega48
#define MCU_AVR_MEGA88  4 // Atmel AVR Mega88
#define MCU_AVR_MEGA168 5 // Atmel AVR Mega168


#define BLOCKSIZE        16 // how many bytes to transfer in CMD_LOAD_PAGE

#define PAGESIZE 64 // size of a flash page, in bytes



// definition of a user's progress report function
typedef void (*cb_progress_t)(int done, int total);


int bootmode(tUartHandle hUart, int addr); // enter boot mode

int ping(tUartHandle hUart, int addr, uint32_t* p_id); // returns the module identifier

int start_app(tUartHandle hUart, int addr, uint32_t id);

int flash(tUartHandle hUart, int addr, unsigned char* buf, int minpos, int maxpos, cb_progress_t fn_progress);

int verify(tUartHandle hUart, int addr, unsigned char* buf, int minpos, int maxpos, cb_progress_t fn_progress);

int eeprom_read(tUartHandle hUart, int addr, int start, int len, unsigned char* buf);

int eeprom_write(tUartHandle hUart, int addr, int start, const unsigned char* buf, int len);

int dump_errlog(tUartHandle hUart, int addr);

int clear_errlog(tUartHandle hUart, int addr);


#endif // #ifndef _BOOTLOAD_H
