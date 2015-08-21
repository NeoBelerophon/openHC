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

/*! \file decoder.c
    \brief In this file, the decoding into plain text is done.
*/

#include <stdio.h>
#include "../common/scalar_types.h" // (u)int8/16/32_t

static int gClass[256]; // which module is what, currently only used for JRM distiction


// note: this returns a pointer to internal static data, can only be usd once per statement
static char* byte2bin(uint8_t b)
{
    static char out[] = { '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0' };
    int i;
    int mask = 0x80;

    for (i=0; i<8; i++)
    {
        if (b & mask)
            out[i] = '1';
        else
            out[i] = '0';

        mask >>= 1;

    }
    return out;
}


// parse list of addresses
int get_range(char* arg)
{
    int range = -1; // inactive
    memset(gClass, 0, sizeof(int)*256);
    while (strlen(arg))
    {
        int adr;
        int assigned = sscanf(arg, "%d", &adr);
        if (assigned)
        {
            if (adr< 0 || adr > 255)
            {
                return -1;
            }

            if (range >=0)
            {
                while (range<adr)
                {
                    gClass[range++] = 1;
                }
            }


            gClass[adr] = 1;
            while (*arg >= '0' && *arg <='9')
            {
                arg++;
            }
            if (*arg == '-')
            {
                range = adr; // start of range
                arg++;
            }
            else if (*arg == ',')
            {
                range = -1; // no range
                arg++;
            }
            else if (*arg == '\0')
            {
                break;
            }
            else
            {
                return -1; // illegal separator
            }
        }
        else
        {
             return -1; // illegal syntax
        }
    }
    return 0;
}


static int decode_input(uint8_t* buf, int length)
{
    int expect = -1; // return value, default to unknown
    uint8_t channel = (buf[0] & 0xF0) >> 4; // upper 4 bit
    uint8_t cmd = buf[0] & 0x0F; // lower 4 bit

    if (length == 1 && cmd == 1) // simple event from controller
    {
        expect = 1; // expected size
        printf ("%02d Ping", channel);
    }
    else if (length >= 1 && cmd >= 2 && cmd <= 6) // simple event(s) from module
    {
        int i;
        expect = length; // expected size
        for (i=0; i<length; i++)
        {
            channel = (buf[i] & 0xF0) >> 4; // upper 4 bit
            cmd = buf[i] & 0x0F; // lower 4 bit

            printf ("%02d ", channel);
            switch (cmd)
            {
            case 2: // oder LED an, wenn von Strg.
                printf("Ein > 0 sec.%s", length>1 ? "" : " | LED an");
                break;

            case 3: // oder LED aus, wenn von Strg.
                printf("Aus < 1 sec.%s", length>1 ? "" : " | LED aus");
                break;

            case 4:
                printf("Ein > 1 sec.");
                break;

            case 5:
                printf("Aus > 1 sec.");
                break;

            case 6:
                printf("Ein > 2 sec.");
                break;

            case 7:
                printf("Aus");
                break;

            default:
                printf("???");
                expect = -1; // set to unknown again

            } // switch
            if (i<length-1)
                printf(", "); // separator
        }
    }

    if (expect < 0) // not decoded by above?
    {
        switch (buf[0])
        {
        case 0x00:
            if (length == 1) // from controller
            {
                printf("Strg-Ack");
                expect = 1;
            }
            else if (length == 2) // from 8 channel module
            {
                printf("LED-Ack. %s", byte2bin(buf[1]));
                expect = 2;
            }
            else if (length == 3) // from 16 channel module
            {
                printf("LED-Ack. %s", byte2bin(buf[2]));
                printf(" %s",byte2bin(buf[1]));
                expect = 3;
            }
            break;

        case 0x01:
            if (length == 1)
            {
                printf("Strg-Ack");
                expect = 1;
            }
            else if (length == 4)
            {
                printf("Ping-Ack, LEDs=%s", byte2bin(buf[1]));
                printf(", in=%s", byte2bin(buf[2]));
                expect = 4;
            }
            break;

        case 0xFE: // from controller
            printf("Konfiguration LED=%02X in0-7=%02X in8-15=%02X", buf[1], buf[2], buf[3]);
            expect = length;
            break;

        case 0xFC: // from controller, STM v2.x
            printf("Konfiguration LED0-7=%02X LED8-15=%02X in0-7=%02X in8-15=%02X", buf[1], buf[4], buf[2], buf[3]);
            expect = length;
            break;

        case 0xFF:
            if (length == 1)
            {   // from controller
                printf("Reset");
                expect = 1;
            }
            else if (length == 2)
            {   // from module, a while after powerup or reset
                printf("Boot");
                expect = 2;
            }
            break;
        }
    }

    return expect;
}

static int decode_output(uint8_t* buf, int length)
{
    int expect = -1; // return value, default to unknown
    uint8_t channel = (buf[0] & 0xE0) >> 5; // upper 3 bit
    uint8_t cmd = buf[0] & 0x1F; // lower 5 bit

    if (cmd >= 0x02 && cmd <= 0x12 && (length == 1 || length == 3))
    {   // simple command, or simple command + time
        printf ("%02d ", channel);

        switch (cmd)
        {
        case 2:
            printf("Einschalten");
            expect = 1;
            break;

        case 3:
            printf("Ausschalten");
            expect = 1;
            break;

        case 4:
            printf("Einschalten verriegelt");
            expect = 1;
            break;

        case 5:
            printf("Ausschalten verriegelt");
            expect = 1;
            break;

        case 6:
            printf("Umschalten");
            expect = 1;
            break;

        case 7:
            printf("Entriegeln");
            expect = 1;
            break;

        case 8:
            printf("Einschaltverzögerung");
            expect = 3;
            break;

        case 9:
            printf("Ausschaltverzögerung");
            expect = 3;
            break;

        case 10:
            printf("Einschalten mit Zeitglied");
            expect = 3;
            break;

        case 11:
            printf("Ausschalten mit Zeitglied");
            expect = 3;
            break;

        case 12:
            printf("Verzögert umschalten");
            expect = 3;
            break;

        case 13:
            printf("Umschalten mit Zeitglied");
            expect = 3;
            break;

        case 14:
            printf("Fest verriegeln");
            expect = 1;
            break;

        case 15:
            printf("Verriegeln für laufende Zeit");
            expect = 1;
            break;

        case 16:
            printf("Zeitaddition auf laufende Zeit");
            expect =3;
            break;

        case 17:
            printf("Zeit neu setzen");
            expect = 3;
            break;

        case 18:
            printf("Zeitabbruch");
            expect = 1;
            break;
        }
        if (expect == 3)
        {
            int sec = buf[1] + buf[2]*256;
            printf (" %d sec ", sec);
        }

    }
    else if (length == 2)
    {
        switch (cmd)
        {
        case 2:
            printf("%02d Rückmeldung EIN, Zustand=%s", channel, byte2bin(buf[1]));
            expect = 2;
            break;

        case 3:
            printf("%02d Rückmeldung AUS, Zustand=%s", channel, byte2bin(buf[1]));
            expect = 2;
            break;
        }
    }
    
    if (expect < 0) // not decoded by above?
    {
        switch (buf[0])
        {
        case 0x00: // from input
            
            if (length == 1)
            {
                printf("Strg-Ack");
                expect = 1;
            }
            else if (length == 2)
            {
                printf("Ack, Zustand=%s", byte2bin(buf[1]));
                expect = 2;
            }
            break;


        case 0x01:
            if (length == 1)
            {
                printf("Ping");
                expect = 1;
            }
            else if (length == 2)
            {
                printf("Ping Ack, Zustand=%s", byte2bin(buf[1]));
                expect = 2;
            }
            break;

        case 0xFD:
            printf(" Zeitablauf, Zustand=%s", byte2bin(buf[1]));
            expect = 2;
            break;

        case 0xFE:
            printf("Konfiguration POR=%02X %02X", buf[1], buf[2]);
            if (length<3)
            {
                printf(" Format???");
            }
           
            expect = length;
            break;

        case 0xFF:
            printf("Boot");
            expect = 1;
            break;

        }
    }
    return expect;
}

static int decode_dimmer(uint8_t* buf, int length)
{
    int expect = -1; // return value, default to unknown
    uint8_t channel = (buf[0] & 0xE0) >> 5; // upper 3 bit
    uint8_t cmd = buf[0] & 0x1F; // lower 5 bit
    if (cmd >= 0x02 && cmd <= 0x16 && (length == 1 || length == 3))
    {   // simple command, or simple command + time
        printf ("%02d ", channel);

        switch (cmd)
        {
        case 2:
            printf("Einschalten max. Helligkeit mit Memory");
            expect = 1;
            break;

        case 3:
            printf("Einschalten max. Helligkeit ohne Memory");
            expect = 1;
            break;

        case 4:
            printf("Ausschalten");
            expect = 1;
            break;

        case 5:
            printf("Umschalten max. Licht EIN und AUS");
            expect = 1;
            break;

        case 6:
            printf("Umschalten max. Licht ohne Memory");
            expect = 1;
            break;

        case 7:
            printf("Dimmen in Gegenrichtung");
            expect = 3;
            break;

        case 8:
            printf("Heller Dimmen");
            expect = 3;
            break;

        case 9:
            printf("Dunkler Dimmen");
            expect = 3;
            break;

        case 10:
            printf("Speichern Memory");
            expect = 1;
            break;

        case 11:
            printf("Umschalten Memory EIN und AUS");
            expect = 1;
            break;

        case 12:
            printf("Einschalten Lichtwert Memory");
            expect = 1;
            break;

        case 13:
            printf("Speichern Dia 1");
            expect = 1;
            break;

        case 14:
            printf("Umschalten Dia 1 EIN und AUS");
            expect = 1;
            break;

        case 15:
            printf("Einschalten Dia 1");
            expect = 1;
            break;

        case 16:
            printf("Speichern Dia 2");
            expect = 1;
            break;

        case 17:
            printf("Umschalten Dia 2 EIN und AUS");
            expect = 1;
            break;

        case 18:
            printf("Einschalten Dia 2");
            expect = 1;
            break;

        case 19:
            printf("Speichern Dia 3");
            expect = 1;
            break;

        case 20:
            printf("Umschalten Dia 3 EIN und AUS");
            expect = 1;
            break;

        case 21:
            printf("Einschalten Dia 3");
            expect = 1;
            break;

        case 22:
            printf("Dimmwinkel und Zeit setzen");
            printf("%d deg, %d sec", buf[1], (int)(0.64*buf[2]+.5));
            expect = 3;
            break;
        }
        if (expect == 3 && cmd != 22)
        {
            int sec = (int)(0.64 * (buf[1] + buf[2]*256) + 0.5);
            printf (" %d sec ", sec);
        }

    }

    if (expect < 0) // not decoded by above?
    {
        switch (buf[0])
        {
        case 0:
            printf("Ack, Zustand=%d", buf[1]);
            expect = 2;
            break;

        case 1:
            if (length == 1)
            {
                printf("Ping");
                expect = 1;
            }
            else if (length == 4)
            {
                printf("Ping-Ack. Hell(0)=%d Hell(1)=%d, Zustand=%d", buf[1], buf[2], buf[3]);
                expect = 4;
            }
            break;

        case 0xFB:
        case 0xFC:
            printf("%02d Kennlinie, Teil %d", buf[0] - 0xFB, buf[1]);
            expect = 18;
            break;

        case 0xFE: // from controller
            printf("Konfiguration POR=%02X %02X", buf[1], buf[2]);
            expect = 3;
            break;

        case 0xFF:
            printf("Boot");
            expect = 1;
            break;
            
        }
    }

    return expect;
}


static int decode_analog(uint8_t* buf, int length)
{
    int expect = -1; // return value, default to unknown
    uint8_t channel = (buf[0] & 0xE0) >> 5; // upper 3 bit
    uint8_t cmd = buf[0] & 0x1F; // lower 5 bit
    if (cmd >= 0x02 && cmd <= 0x1B && (length == 1 || length == 3))
    {   // simple command, or simple command + time
        printf ("%02d ", channel);

        switch (cmd)
        {
        case 2:
            printf("Einschalten max. Helligkeit mit Memory");
            expect = 1;
            break;

        case 3:
            printf("Einschalten max. Helligkeit ohne Memory");
            expect = 1;
            break;

        case 4:
            printf("Ausschalten");
            expect = 1;
            break;

        case 5:
            printf("Umschalten max. Licht EIN und AUS");
            expect = 1;
            break;

        case 6:
            printf("Umschalten max. Licht ohne Memory");
            expect = 1;
            break;

        case 7:
            printf("Dimmen in Gegenrichtung");
            expect = 3;
            break;

        case 8:
            printf("Heller Dimmen");
            expect = 3;
            break;

        case 9:
            printf("Dunkler Dimmen");
            expect = 3;
            break;

        case 10:
            printf("Speichern Memory");
            expect = 1;
            break;

        case 11:
            printf("Umschalten Memory EIN und AUS");
            expect = 1;
            break;

        case 12:
            printf("Einschalten Lichtwert Memory");
            expect = 1;
            break;

        case 13:
            printf("Speichern Dia 1");
            expect = 1;
            break;

        case 14:
            printf("Umschalten Dia 1 EIN und AUS");
            expect = 1;
            break;

        case 15:
            printf("Einschalten Dia 1");
            expect = 1;
            break;

        case 16:
            printf("Speichern Dia 2");
            expect = 1;
            break;

        case 17:
            printf("Umschalten Dia 2 EIN und AUS");
            expect = 1;
            break;

        case 18:
            printf("Einschalten Dia 2");
            expect = 1;
            break;

        case 19:
            printf("Speichern Dia 3");
            expect = 1;
            break;

        case 20:
            printf("Umschalten Dia 3 EIN und AUS");
            expect = 1;
            break;

        case 21:
            printf("Einschalten Dia 3");
            expect = 1;
            break;

        case 22:
            printf("Dimmwinkel und Zeit setzen");
            printf("%d deg, %d sec", buf[1], (int)(0.64*buf[2]+.5));
            expect = 3;
            break;

        case 23:
            printf("Soll=Analog, Regelung ein");
            expect = 1;
            break;

        case 24:
            printf("Soll setzen, Regelung ein");
            expect = 1;
            break;

        case 25:
            printf("Regelung umschalten");
            expect = 1;
            break;

        case 26:
            printf("Regelung ausschalten");
            expect = 1;
            break;

        case 27:
            printf("Regelung einschalten");
            expect = 1;
            break;


        }
        if (expect == 3 && cmd != 22)
        {
            int sec = (int)(0.64 * (buf[1] + buf[2]*256) + 0.5);
            printf (" %d sec ", sec);
        }

    }

    if (expect < 0) // not decoded by above?
    {
        switch (buf[0])
        {
        case 0:
            printf("Ack, Zustand=%d", buf[1]);
            expect = 2;
            break;

        case 1:
            if (length == 1)
            {
                printf("Ping");
                expect = 1;
            }
            else if (length == 4)
            {
                printf("Ping-Ack. Hell(0)=%d Hell(1)=%d, Zustand=%d", buf[1], buf[2], buf[3]);
                expect = 4;
            }
            break;

        case 0xFB:
        case 0xFC:
            printf("%02d Kennlinie, Teil %d", buf[0] - 0xFB, buf[1]);
            expect = 18;
            break;

        case 0xFE:
            printf("Konfiguration %02X %02X", buf[1], buf[2]);
            if (length<3 || buf[1] != 0x00 || buf[2] != 0xFF)
            {
                printf(" Format???");
            }
           
            expect = length;
            break;

        case 0xFF:
            printf("Boot");
            expect = 1;
            break;

        }
    }

    return expect;
}


static int decode_jalousie(uint8_t* buf, int length)
{
    int expect = -1; // return value, default to unknown
    uint8_t channel = (buf[0] & 0x60) >> 5; // bits 5+6
    uint8_t cmd = buf[0] & 0x1F; // lower 5 bit

    if (cmd >= 2 && cmd <= 22 && length >= 1 && length <= 8)
    {   // simple command, or simple command + time
        printf ("%02d ", channel);

        switch (cmd)
        {
        case 2:
            printf("Lauf stoppen | Rückmeldung Heben ein, Zustand=%s", byte2bin(buf[1]));
            expect = 2;
            break;

        case 3:
            if (length == 4) // resolve ambiguity with feedback msg
            {
                printf("Umschalten heben/aus");
                expect = 4;
            }
            break;

        case 4:
            if (length == 4) // resolve ambiguity with feedback msg
            {
                printf("Umschalten senken/aus");
                expect = 4;
            }
            break;

        case 5:
            if (length == 4) // resolve ambiguity with feedback msg
            {
                printf("Einschalten heben");
                expect = 4;
            }
            break;

        case 6:
            if (length == 4) // resolve ambiguity with feedback msg
            {
                printf("Einschalten senken");
                expect = 4;
            }
            break;

        case 7:
            if (length == 4) // resolve ambiguity with feedback msg
            {
                printf("Tippbetrieb heben");
                expect = 4;
            }
            break;

        case 8:
            if (length == 4) // resolve ambiguity with feedback msg
            {
                printf("Tippbetrieb senken");
                expect = 4;
            }
            break;

        case 9:
            printf("Prioritätsebenen verriegeln");
            expect = 2;
            break;

        case 10:
            printf("Prioritätsebenen entriegeln");
            expect = 2;
            break;

        case 11:
            printf("Lernfunktion einschalten");
            expect = 1;
            break;

        case 12:
            printf("Lernfunktion ausschalten");
            expect = 1;
            break;

        case 13:
            printf("Prioritätsebenen setzen");
            expect = 2;
            break;

        case 14:
            printf("Prioritätsebenen löschen");
            expect = 2;
            break;

        case 15:
            printf("Sensorik Rollade heben");
            expect = 6;
            break;

        case 16:
            printf("Sensorik Jalousie heben");
            expect = 8;
            break;

        case 17:
            printf("Sensorik Rollade senken");
            expect = 6;
            break;

        case 18:
            printf("Sensorik Jalousie senken");
            expect = 8;
            break;
 
        case 19:
            printf("Zeitmessung verzögert ein");
            expect = 3;
            break;
        
        case 20:
            printf("Zeitmessung verzögert aus");
            expect = 3;
            break;
        
        case 21:
            printf("Zeitmessung verzögert ein mit Zeitglied");
            expect = 3;
            break;
        
        case 22:
            printf("Zeitmessung abbrechen");
            expect = 1;
            break;
        
        
        }
        if (expect == 3)
        {
            int sec = buf[1] + buf[2]*256;
            printf (" %d/10 sec", sec);
        }
        if (expect >= 3)
        {
            printf (" prio=%s", byte2bin(buf[1]));
        }
        if (expect >= 4)
        {
            int sec = buf[2] + buf[3]*256;
            printf (" %d/10 sec", sec);
        }
        if (expect >= 6)
        {
            int sec = buf[4] + buf[5]*256;
            printf (" %d/10 sec", sec);
        }
        if (expect >= 8)
        {
            int sec = buf[6] + buf[7]*256;
            printf (" %d/10 sec", sec);
        }

    }
  
    if (expect < 0 && length == 2)
    {
        switch (cmd)
        {
        case 2:
            printf("%02d Rückmeldung Heben ein, Zustand=%s", channel, byte2bin(buf[1]));
            expect = 2;
            break;

        case 3:
            printf("%02d Rückmeldung Senken ein, Zustand=%s", channel, byte2bin(buf[1]));
            expect = 2;
            break;

        case 4:
            printf("%02d Rückmeldung Heben aus, Zustand=%s", channel, byte2bin(buf[1]));
            expect = 2;
            break;

        case 5:
            printf("%02d Rückmeldung Senken aus, Zustand=%s", channel, byte2bin(buf[1]));
            expect = 2;
            break;

        case 6:
            printf("%02d Rückmeldung EIN, Zustand=%s", channel, byte2bin(buf[1]));
            expect = 2;
            break;

        case 7:
            printf("%02d Rückmeldung Abbruch, Zustand=%s", channel, byte2bin(buf[1]));
            expect = 2;
            break;

        case 8:
            printf("%02d Rückmeldung AUS, Zustand=%s", channel, byte2bin(buf[1]));
            expect = 2;
            break;
        }
    }
    
    if (expect < 0) // not decoded by above?
    {
        switch (buf[0])
        {
        case 0x00: // from input
            
            if (length == 1)
            {
                printf("Strg-Ack");
                expect = 1;
            }
            else if (length == 2)
            {
                printf("Ack, Zustand=%s", byte2bin(buf[1]));
                expect = 2;
            }
            break;


        case 0x01:
            if (length == 1)
            {
                printf("Ping");
                expect = 1;
            }
            else if (length == 3)
            {
                printf("Ping Ack, Zustand=%s", byte2bin(buf[1]));
                expect = 3;
            }
            break;

        case 0xFD:
            printf(" Zeitablauf, Zustand=%s", byte2bin(buf[1]));
            expect = 2;
            break;

        case 0xFE:
            printf("Konfiguration POR=%02X %02X", buf[1], buf[2]);
            if (length<3)
            {
                printf(" Format???");
            }
           
            expect = length;
            break;

        case 0xFF:
            printf("Boot");
            expect = 1;
            break;

        }
    }
    return expect;
}



static int decode_system(uint8_t* buf, int length)
{
    int expect = -1; // return value, default to unknown
    switch (buf[-2]) // looking at what used to be class/address
    {
    case 0xFE:
        printf("System Reset?");
        expect = 0;
        break;

    case 0xFF:

        // my special extension: boot mode command
        if (strncmp(buf, "Boot", 4) == 0)
        {
            expect = 4;
            printf("System Bootloader");
        }
        else
        {
            printf("System POR 0x%02X", buf[0]);
            expect = 1;
        }
        break;
    }
    return expect;
}

// Hack, JRM and AM share the same class, this depends on sys config
// ToDo: come up with a better way
static int is_jalousie(uint8_t adr)
{
    return gClass[adr]; 
}

void decode_packet(uint8_t* buf, int size)
{
    uint8_t class = (buf[0] & 0xE0) >> 5; // upper 3 bit
    uint8_t adr = buf[0] & 0x1F; // lower 5 bit
    int toggle = (buf[1] & 0x80) >> 7; // MSB is the toggle bit
    int len = buf[1] & 0x7F; // lower bits are payload length
    int expect; // expected payload size

    if (size < 4 || len != size-4)
    {
        printf ("Länge falsch!!!");
        return;
    }

    switch(class)
    {
    case 0: // input
        printf (" EMD.%02d.", adr);
        expect = decode_input(buf+2, len);
        break;

    case 2: // output
        if (is_jalousie(adr))
        {
            printf (" JRM.%02d.", adr);
            expect = decode_jalousie(buf+2, len);
        }
        else
        {
            printf (" AMD.%02d.", adr);
            expect = decode_output(buf+2, len);
        }
        break;

    case 3: // analog module
        printf (" AMA.%02d.", adr);
        expect = decode_analog(buf+2, len); // very similar to dimmer
        break;

    case 5: // Dimmer
        printf (" DIM.%02d.", adr);
        expect = decode_dimmer(buf+2, len);
        break;

    case 7: // System?
        printf (" SYS ");
        expect = decode_system(buf+2, len);
        break;

    default:
        printf ("??%d??.%02d.", class, adr);
        expect = len;

    }

    if (expect < 0)
    {
        printf(" unbekannt!!!");
    }
    else if (expect != len)
    {
        printf (" Len != erwarted!!!");
    }
}
