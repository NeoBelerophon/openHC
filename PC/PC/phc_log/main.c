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

/*! \file main.c
    \brief An application to log and dump traffic on the PHC module bus.
*/

#include <stdio.h>
#include <sys/timeb.h> // for _timeb, _ftime()
#include <time.h> // for ctime()
#include "../common/scalar_types.h"
#include "../common/uart.h"
#include "../common/hal.h"
#include "decoder.h"
#include "common.h"

tUartHandle ghUart; // global, so the HAL can use it for sending

// small helper to hexdump
static void dump(uint8_t* buf, int n)
{
    while (n--)
    {   // dump the packet
        printf("%02X ", *buf++);
    }
}


static void log_loop(tUartHandle hUart)
{
    double last, curr;
    int count = 0;
    uint8_t msg[1000];
  
    last = hires_time();        

    do 
    {
        double waited;

        uint8_t receive;
        int got = 1;

        UartRead(hUart, &receive, &got);
        curr = hires_time();        
        waited = curr - last;
        last = curr;
        if (got == 0)
            continue; // should never happen
        
        if (waited > 0.01)
        {
            if (count)
            {
                dump(msg, count);
                printf("  (stale bytes)");
                count = 0;
            }
            printf("\n"); // block separator, for nicer readability
        }

        msg[count] = receive;
        count++;

        if (count >= 4) // minimum packet length
        {
            int i;
            // CRC candidate
            uint16_t cand = msg[count-2] + msg[count-1] * 256; // little endian

            // search for size + CRC match, assume packet end if both matches
            for (i=0; i<count-2; i++)
            {
                int packetlen = msg[i+1] & 0x7F; // from msg candidate
                if (packetlen == count-i-4 && crc16(msg+i, count-2-i) == cand)
                {   
                    if (i>0)
                    { // dump heading garbage
                        dump(msg, i);
                        printf(" Garbage (possible collision)\n");
                    }
                    {
                        // print timestamp
                        char *timeline;
#ifdef WIN32
                        struct _timeb timebuffer;
                        _ftime( &timebuffer );
#else
                        struct timeb timebuffer;
                        ftime( &timebuffer );
#endif
                        timeline = ctime( & ( timebuffer.time ) );
                        printf( "%.8s.%03hu  ", &timeline[11], timebuffer.millitm);
                    }

                    dump (msg+i, count-i); // dump the packet
                    decode_packet(msg+i, count-i);
                    printf("\n");
                    //out_rcv_packet(&out, msg+i, count-i); // pass to emulation
                    count = 0;
                } // if crc match
            } // for i
        } // if (count > 2)


    } while(1);
}


int main(int argc, char* argv[])
{
    int retval = 0;

    if (argc < 2)
    {
        printf("Usage: phc_log <COM port> {<JRM list>}\n");
#ifdef WIN32
        printf("       <COM port> is e.g. COM1, COM2, etc.\n");
#else
        printf("       <COM port> is e.g. /dev/ttyS0, /dev/ttyS1, etc.\n");
#endif
        printf("       <JRM list> is a comma/dash separated list of JRM modules\n");
        printf("Example: phc_log COM5 24-27,30\n");
        return -1;
    }

    if (argc >= 3)
    {
        // which AM module(s) to treat as JRM
        retval = get_range(argv[2]);
        if (retval)
        {
            printf("Error: bad address specified %s\n", argv[2]);
            return retval;
        }
        
    }
    
    ghUart = UartOpen(argv[1]);
    if (!ghUart)
    {
        printf("Error opening %s\n", argv[1]);
        return -2;
    }

    //UartConfig(ghUart, 19200, eNOPARITY, eTWOSTOPBITS, 8, 0);
    UartConfig(ghUart, 19200, eNOPARITY, eONESTOPBIT, 8, 0);

    log_loop(ghUart); // do the action

    UartClose(ghUart);

    return retval;
}
