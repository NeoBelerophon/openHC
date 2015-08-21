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

/*! \file bootload.c
    \brief "Middle layer" functions for using the module's bootloader.
*/

#include <stdio.h>
#include "../common/scalar_types.h"
#include "../common/uart.h"
#include "../common/protocol.h"
#include "bootload.h"


#define MAXRETRIES 4 // how many times should we try a command before giving up


//#define MAX_READ 126 // due to packet size limit for raw module bus
//#define MAX_READ 17 // the STM2 can't return results with >17 bytes payload
#define MAX_READ 15 // the STM1 can't return results with >15 bytes payload


#ifdef _DEBUG
#define TRACE(x) printf x
#else
#define TRACE(x)
#endif


// small helper to hexdump
static void dump(unsigned char* buf, int n)
{
    while (n--)
    {   // dump the packet
        TRACE(("%02X ", *buf++));
    }
}



// Send the command in p_command, return response in p_response
//  and its size in p_size. Buffers have to be big enough for escaping!
// Returns 0 on success, positive on timeout, negative on error.
static int send_command(tUartHandle hUart, 
                        uint8_t* p_command, // prepared module bus command, without checksum
                        uint8_t* p_response, // the response will be placed here
                        const uint8_t* p_expect, // compare the response payload against this
                        int compare, // compare this many bytes (0 = no compare)
                        int* p_size, // in: size of command; out: size of response
                        int expectsize, // expected response size; negative: don't check
                        int maxretries) // # of retries in case of error or mismatch
{
    int retval;
    bool success;
    int got;
    int gotnow;
    int retries;
    uint8_t* p_save;
    int size;
    
    // because of other traffic, getting a command through may require some retries
    for (retries = 0; retries < maxretries; retries++)
    {
        got = 0;
        p_save = p_response;
        size = *p_size + 2;
        module_to_stm(p_response, p_command, &size); // abuse the response buffer
        UartWrite(hUart, p_response, &size);

        // collect the response
        do 
        {
            gotnow = 1;
            success = UartRead(hUart, p_save, &gotnow); // addr+len
            if (!success) // fatal error
            {
                TRACE(("UART read error!\n"));
                return -1;
            }
            if (*p_save == 0xC0) // start of packet: reset to start of buffer
            {
                p_save = p_response;
                got = 0;
            }
            got += gotnow;
            p_save += gotnow;
    	    
        } while (gotnow && p_save[-1] != 0xC1 && got < MAX_PACKET_SIZE); // not end

        if (gotnow == 0)
        {
            TRACE(("UART timeout, retrying\n"));
            continue;
        }

        if (got < 5+3) // 5+3 would be zero PHC payload
        {
            TRACE(("Only received %d byte, packet too small, retrying\n", got));
            continue; // too small
        }


        unpad_buffer(p_response, p_response, &got);

        retval = stm_to_module(p_response, p_response, &got);
        if (retval)
        {
            TRACE(("CRC error or other mismatch!\n"));
            continue;
        }

        if (p_response[0] != p_command[0]) // wrong address?
        {
            TRACE(("Response address %d doesn't match command address %d, retrying\n", p_response[0], p_command[0]));
            continue;
        }

        if (got == 3 && p_response[2] == 0xF0) // STM retried and got no response from module
        {
            TRACE(("Command timeout by STM\n"));
            return 1; // pointless to retry, exit
        }

        if (expectsize >= 0 && got != expectsize) // if response size is given, check it
        {
            TRACE(("Response size %d doesn't match expected size %d, retrying\n", got, expectsize));
            continue;
        }

        if (compare && memcmp(p_expect, p_response+2, compare) != 0) // compare response, if desired
        {
            TRACE(("Response payload doesn't match expected, retrying\n"));
            continue;
        }

        break; // done
    }
    if (retries >= maxretries)
    {
        TRACE(("Error: command didn't succeed after %d retries!\n", maxretries));
        return 2;
    }
    
    *p_size = got;
	return 0;
}


// functions using the bootloader via bus communication


// enter boot mode
int bootmode(tUartHandle hUart, int addr)
{
    int retval;
    uint8_t bootcmd[256] = { addr, 3, CMD_PING, 'B', 'L' };
    uint8_t response[256];
    int size = bootcmd[1] + 2; // populated part

    int retries;
    for (retries = 0; retries < MAXRETRIES; retries++)
    {
        retval = send_command(hUart, bootcmd, response, NULL, 0, &size, -1, MAXRETRIES);
        if (retval)
        {
            return -1; // return something negative
        }
    
        // check the response size
        if (size != 3) // response with payload?
        {
            return 1; // yes: probably a PEHA module responding to a ping
        }
        
        if (response[2] == 0x01)
        {   // success, OpenHC found
            return 0;
        }
        else if (response[2] == 0xF0)
        {   // timeout, no module was responding
            return -1;
        }
        else
        { // not the expected answer, can happen during scan
            TRACE(("Bootmode answer mismatch (size=%d, return=%d)!\n", size, response[2]));
            continue;
        }
    }

    return 1; // some undefined answer, but not from OpenHC module
}

// ping a module, return its ID
int ping(tUartHandle hUart, int addr, uint32_t* p_id)
{
    int retval;
    uint8_t pingcmd[256] = { addr, 1, CMD_PING };
    uint8_t response[256];
    int size = pingcmd[1] + 2; // populated part
    const uint8_t expected[] = { 0x01 };

    retval = send_command(hUart, pingcmd, response, expected, sizeof(expected), &size, 6, MAXRETRIES);
    if (retval)
    {
        return retval;
    }

    *p_id = ((uint32_t)response[3] << 16)
          | ((uint32_t)response[4] <<  8)
          |  (uint32_t)response[5];
    return 0;
}

// exit the bootloader, restart the application
int start_app(tUartHandle hUart, int addr, uint32_t id)
{
    int retval;
    uint8_t bootcmd[256] = { addr, 1, CMD_REBOOT };
    uint8_t response[256];
    int size = bootcmd[1] + 2; // populated part
    int retries;

    /* Better don't let send_command() do the retries in this case,
       (can be confused with a command if the application start *did* happen,
        but we just somehow missed the response)
       but send a ping to check if still in boot mode, retry then.
    */

    for (retries = 0; retries < MAXRETRIES; retries++)
    {
        retval = send_command(hUart, bootcmd, response, NULL, 0, &size, -1, 1);
        if (retval == 1) // STM timeout, maybe define some standard responses
        {
            break; // exit with error
        }
        if (retval || size != 4 || response[2] != 0x00 || response[3] != 0x00)
        {   
            uint8_t pingcmd[256] = { addr, 1, CMD_PING };
            uint32_t test_id;

            TRACE(("Bad reboot answer, tying a ping to clarify.\n"));
            // something is wrong, send a ping to find out if still in bootloader
            retval = send_command(hUart, pingcmd, response, NULL, 0, &size, -1, MAXRETRIES);
            if (retval) 
            {
                break; // exit with error
            }
            if (size != 6) // not matching the bootloader ping size, assume the app is running
            {
                TRACE(("No bootloader ping, assuming application is running.\n"));
                break;  // exit OK
            }
            
            // since the app ping response may be same size as bootloader response,
            // compare the payload to resolve ambiguity (with minimum uncertainty)
            test_id = ((uint32_t)response[3] << 16)
                    | ((uint32_t)response[4] <<  8)
                    |  (uint32_t)response[5];
            if (id != test_id)
            {
                break;  // exit OK
            }
            
            TRACE(("Got bootloader ping, retrying.\n"));
            continue; // still in bootloader, retry
        }
        break; // OK
    }

    // The STM v1 seems to have a "blind spot" while handling the module's boot configuration.
    // Without a sleep, there's a chance for the next command to timeout.

    return retval;
}

// flash a module's application program memory
int flash(tUartHandle hUart, int addr, uint8_t* buf, int minpos, int maxpos, cb_progress_t fn_progress)
{
    int retval = 0;
    int page;
    uint8_t cmd[256] = { addr }; // preload addr
    uint8_t response[256];
    int total = (maxpos+PAGESIZE-1)/PAGESIZE - minpos / PAGESIZE; // number of pages
    int current = 0; // progress

    fn_progress(current++, total); // progress bar init

    for (page = minpos / PAGESIZE; page < (maxpos+PAGESIZE-1)/PAGESIZE; page++)
    {
        int retries;
        for (retries = 0; retries < MAXRETRIES; retries++)
        {
            const uint8_t expected[] = { 0x00, 0x00 };
            int block;
            int size;
            int memaddr;
            bool error; 

            // the STM can't issue commands with >16 bytes payload
            error = false;
            for (block=0; block < PAGESIZE/16; block++)
            {
                size = BLOCKSIZE;
                cmd[1] = size + 2;
                cmd[2] = CMD_LOAD_PAGE; // flash page transfer command
                cmd[3] = block;
                memcpy(cmd+4, buf + page*PAGESIZE + block*size, size);
                //TRACE(("transferring block %d \n", block));
                size += 4; // payload size: cmd, block, data
                retval = send_command(hUart, cmd, response, expected, sizeof(expected), &size, 4, MAXRETRIES);
                if (retval)
                {
                    TRACE(("Page load answer mismatch (return=%d)!\n", retval));
                    error = true;
                    break; // exit page load loop, continue retry loop
                }
            }
            if (error) // page verify error
            {
                continue; // retry the page
            }
        
            cmd[1] = 4;
            cmd[2] = CMD_FLASH_PAGE; // flash page command
            cmd[3] = page;
            //TRACE(("flashing page %d \n", page));
            size = 4; // payload size: cmd, page
            retval = send_command(hUart, cmd, response, expected, sizeof(expected), &size, 4, 1);
            if (retval)
            {
                TRACE(("Page flash answer mismatch (return=%d)!\n", retval));
                continue; // we have to redo the page loading
            }

            // verify the page
            error = false;
            for (memaddr = page*PAGESIZE; memaddr < (page+1)*PAGESIZE; memaddr += MAX_READ)
            {
                int len = MIN(MAX_READ, (page+1)*PAGESIZE - memaddr); // remaining size
                cmd[1] = 4;
                cmd[2] = CMD_FLASH_READ; // flash page transfer command
                cmd[3] = len;
                cmd[4] = memaddr & 0xFF;
                cmd[5] = memaddr >> 8;
                //TRACE(("verifying %d bytes @ %d \n", len, memaddr));
                size = 6;
                retval = send_command(hUart, cmd, response, expected, 1, &size, len+3, MAXRETRIES);
                if (retval!= 0)
                {
                    TRACE(("Page load answer mismatch (return=%d)!\n", retval));
                    error = true;
                    break; // exit verify loop, continue retry loop
                }
                if (memcmp(buf+memaddr, response+3, len) != 0)
                {
                    TRACE(("\nPage verify error, retrying\n"));
                    error = true;
                    break; // exit verify loop, continue retry loop
                }
            }
            if (error) // page verify error
            {
                continue; // retry the page
            }

            break; // success, exit retry loop
        }
        if (retries >= MAXRETRIES)
        {
            TRACE(("Error: page flashing didn't succeed after %d retries!\n", MAXRETRIES));
            return -1;
        }
        fn_progress(current++, total); // advance the progress bar
    }

    return 0;
}

// verify a module's application program memory by reading it back and comparing
int verify(tUartHandle hUart, int addr, unsigned char* buf, int minpos, int maxpos, cb_progress_t fn_progress)
{
    int retval = 0;
    int memaddr;
    int len;
    uint8_t cmd[256] = { addr, 4, CMD_FLASH_READ }; // preload addr,length,command
    uint8_t response[256];
    const uint8_t expected[] = { 0x00 };
    int size;
    int total = (maxpos - minpos + (MAX_READ-1)) / MAX_READ;
    int current = 0;

    fn_progress(current++, total);

    for (memaddr = minpos; memaddr <= maxpos; memaddr += MAX_READ)
    {
        len = MIN(MAX_READ, maxpos+1 - memaddr); // may read less at the end
        
        cmd[3] = len;
        cmd[4] = memaddr & 0xFF;
        cmd[5] = memaddr >> 8;
        //TRACE(("verifying %d bytes @ %d \n", len, memaddr));
        size = 6;
        retval = send_command(hUart, cmd, response, expected, sizeof(expected), &size, len+3, MAXRETRIES);
        if (retval)
        {   // ToDo: retry
            TRACE(("Page load answer mismatch (return=%d)!\n", retval));
            break;
        }

        fn_progress(current++, total);

        if (memcmp(buf+memaddr, response+3, len) != 0)
        {
            retval = 1;
            TRACE(("\nVerify Error!\n"));
            break;
        }
    }

    return retval;
}

// read n bytes from module's EEPROM
int eeprom_read(tUartHandle hUart, int addr, int start, int len, unsigned char* buf)
{
    int retval;
    unsigned char read_cmd[256] = { addr, 4, CMD_EEPROM_READ, len , start & 0xFF, start >> 8}; // preload addr,length,command,pos,size
    uint8_t response[256];
    int size = read_cmd[1] + 2; // populated part
    const uint8_t expected[] = { 0x00 };

    retval = send_command(hUart, read_cmd, response, expected, sizeof(expected), &size, len+3, MAXRETRIES);
    if (retval)
    {   // ToDo: retry
        TRACE(("Read EEPROM answer mismatch (return=%d)!\n", retval));
        return retval;
    }

    memcpy(buf, response+3, len);

    return 0;
}

// write n bytes into module's EEPROM
int eeprom_write(tUartHandle hUart, int addr, int start, const unsigned char* buf, int len)
{
    int retval;
    unsigned char write_cmd[256] = { addr, len+3, CMD_EEPROM_WRITE, start & 0xFF, start >> 8 }; // preload addr,length,command,pos
    uint8_t response[256];
    int size = write_cmd[1] + 2; // populated part
    const uint8_t expected[] = { 0x00, 0x00 };

    memcpy(write_cmd+5, buf, len);

    retval = send_command(hUart, write_cmd, response, expected, sizeof(expected), &size, 4, MAXRETRIES);
    if (retval)
    {
        TRACE(("Write EEPROM answer mismatch (return=%d)!\n", retval));
        return retval;
    }
    
    return 0;
}


// higher level functions

// dump the error log contained in a module's EEPROM
int dump_errlog(tUartHandle hUart, int addr)
{
    int retval;
    int entries;
    int i;
    unsigned char buf[3];

    // this isn't exactly sophisticated, maintaining a hard-coded table here
    static const char* names[] = { "(zero)", 
        "errnum.c",      // 1
        "hal_atmel.c",   // 2
        "hal_mb90495.c", // 3
        "msg.c",         // 4
        "phc.c",         // 5
        "random.c",      // 6
        "timer.c",       // 7
        "uart.c",        // 8
        "vectors.c",     // 9

        "input.c",        // 10
        "main.c (input)", // 11
        "switch.c",       // 12

        "main.c (output)", // 13
        "output.c",        // 14
    
        "boot.c",             // 15
        "hal_atmel.c (boot)", // 16
        "main.c (boot)",      // 17

        "jrm.c",        // 18
        "main.c (jrm)", // 19
        
        "input.c (im)", // 20
        "rc5.c",        // 21
        "main.c (im)"   // 22

        "input.c (lux)", // 23
        "lux.c",         // 24
        "main.c (lux)"   // 25
    };

    retval = eeprom_read(hUart, addr, 0, 1, buf); // first byte gives # of entries
    if (retval)
    {
        return retval;
    }

    entries = buf[0];
    if (entries == 0xFF) // virgin byte also means zero
    {
        entries = 0;
    }

    for (i=0; i<entries; i++)
    {
        const char* file;
        int line;

        eeprom_read(hUart, addr, 1+i*3, 3, buf); // skip entry #, each has 3 bytes
        file = buf[0] < sizeof(names)/sizeof(names[0]) ? names[buf[0]] : "unknown";
        line = buf[1]*256 + buf[2]; // high and low part of line
        printf("module %d, file '%s' line %d\n", addr, file, line);
    }

    return 0;
}


// clear a module's error log
int clear_errlog(tUartHandle hUart, int addr)
{
    // clearing first byte is enough, reset the count
    return eeprom_write(hUart, addr, 0, "\xFF", 1);
}

