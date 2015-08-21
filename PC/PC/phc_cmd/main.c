/***************************************************************************
 *
 * OpenHC:                          ___                 _  _  ___
 *  Open source                    / _ \ _ __  ___ _ _ | || |/ __|
 *  Home                          | (_) | '_ \/ -_) ' \| __ | (__ 
 *  Control                        \___/| .__/\___|_||_|_||_|\___|
 * http://openhc.sourceforge.net/       |_| 
 *
 * Copyright (C) 2007 by Joerg Hohensohn
 *
 * All files in this archive are subject to the GNU General Public License.
 * See http://www.gnu.org/licenses/gpl-3.0.txt for full license agreement.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/ 

/*! \file main.c
    \brief A small utility to send an arbitrary packet to the module bus,
           across the STM, using its passtrough feature.
*/

#include <stdio.h>
#include <sys/timeb.h> // for _timeb, _ftime()
#include <time.h> // for ctime()
#include "../common/scalar_types.h"
#include "../common/uart.h"
#include "../common/hal.h"
#include "../common/protocol.h"

#define MAX_PACKET_SIZE (2+128+2) // worst-case PHC packet size

tUartHandle ghUart; // global, so the HAL can use it for sending

// small helper to hexdump
static void dump(uint8_t* buf, int n)
{
    while (n--)
    {   // dump the packet
        printf("%02X ", *buf++);
    }
}

// return 0..15 for a hex char, -1 if none
static int read_hex_digit(char hex)
{
	if (hex >= '0' && hex <= '9')
		return hex - '0';
	else if (hex >= 'A' && hex <= 'F')
		return hex - 'A' + 0xA;
	else if (hex >= 'a' && hex <= 'f')
		return hex - 'a' + 0xA;
	else
		return -1;
}

// Convert the ASCII hex data from p_hex to p_command, write size into p_size.
// Return 0 on success, else an error value.
static int parse_command(char* p_hex, uint8_t* p_command, int* p_size)
{
	int size = 0; // count in nibbles (half bytes)
	int retval = 0;
	int byte;

	while (*p_hex != '\0' && size < MAX_PACKET_SIZE*2)
	{
		int nibble = read_hex_digit(*p_hex++);
		if (nibble >= 0)
		{
			if (size & 0x01) // byte finished
			{
				byte |= nibble;
				*p_command++ = (uint8_t)byte;
			}
			else
			{
				byte = nibble << 4; // upper position
			}
			size++;
		}
		// else ignore: whitespace, comma, etc.
	}
	
	*p_size = size/2; // return full bytes
	return retval;
}


// Send the command in p_command, return response in p_response
//  and its size in p_size.
// Return 0 on success, else an error value.
static int send_command(uint8_t* p_command, uint8_t* p_response, int* p_size)
{
    int retval;
    int got = 0;
    int gotnow;
    uint8_t* p_save = p_response;

    int size = *p_size + 2;
    module_to_stm(p_response, p_command, &size);
    UartWrite(ghUart, p_response, &size);

    // collect the response
    do 
    {
        gotnow = 1;
        UartRead(ghUart, p_save, &gotnow); // addr+len
        if (*p_save == 0xC0) // start of packet: reset to start of buffer
        {
            p_save = p_response;
            got = 0;
        }
        got += gotnow;
        p_save += gotnow;
    	
    } while (gotnow && p_save[-1] != 0xC1 && got < MAX_PACKET_SIZE);

    if (got < 5+3) // 5+3 would be zero PHC payload
    {
        return -1; // too small
    }

    *p_size = got;

    unpad_buffer(p_response, p_response, p_size);

    retval = stm_to_module(p_response, p_response, p_size);

	return retval;
}


int main(int argc, char* argv[])
{
    int retval = 0;
	int size;
	uint8_t command [MAX_PACKET_SIZE];
	uint8_t response[MAX_PACKET_SIZE];

    if (argc < 2)
    {
        printf("This program passes a module bus command via STM, outputs the response.\n");
        printf("Usage: phc_log <COM port> <hex command>\n");
#ifdef WIN32
        printf("       <COM port> is e.g. COM1, COM2, etc.\n");
#else
        printf("       <COM port> is e.g. /dev/ttyS0, /dev/ttyS1, etc.\n");
#endif
        printf("       <hex command> is a hexadecimal string with the command, without checksum.\n");
        printf("Example: phc_cmd COM1 400106  will toggle AMD00.00\n");
		printf("The hex string may contain spaces, but then needs to be quoted: \"40 01 06\"\n");
		printf("The second byte (toggle+length) doesn't need to be valid, will be calculated.\n");
        
        return -1;
    }

    if (parse_command(argv[2], command, &size) || size < 2)
    {
        printf("Command %s is no valid hex string\n", argv[2]);
        return -2;
	}
    command[1] = (uint8_t)(size-2); // set length, toggle must be 0
    
    ghUart = UartOpen(argv[1]);
    if (!ghUart)
    {
        printf("Error opening %s\n", argv[1]);
        return -3;
    }

    UartConfig(ghUart, 19200, eNOPARITY, eONESTOPBIT, 8, 1000);

	retval = send_command(command, response, &size); // send command, collect response
    if (!retval)
    {
        response[1] &= 0x7F; // clear toggle, to avoid confusion
        dump(response, size); // print the response to stdout
    }
	
    UartClose(ghUart);

    return retval;
}
