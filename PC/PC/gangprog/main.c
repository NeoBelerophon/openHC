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
    \brief OpenHC flash update application.
*/

#include <stdio.h>
#include "../common/scalar_types.h"
#include "../common/uart.h"
#include "bootload.h"


// types and globals

typedef struct 
{
    bool do_program;
    bool do_verify;
    bool do_dump;
    bool do_clear;
    char* serial_port;
    bool adresses[256];
    bool adr_given;
    char* hexfile;
} commandline_t;

static commandline_t g_cmdline;

typedef struct 
{
    uint32_t id; // hardware ID from bootloader, consists of address, module type, module MCU
    char hexfile[1024]; // filename of the hexfile to use
} hexfile_association_t;

static hexfile_association_t g_file_lut[256]; // should be big enough



// load a .hex file
static int hexload(const char* filename, unsigned char* buf, int size, int* p_minpos, int* p_maxpos)
{
    int got;

    int minpos = 0xFFFF; // set extreme values
    int maxpos = 1;

    int len;
    int addr;
    int data[17]; // data plus checksum
    FILE* file = fopen(filename, "r");

    if (file == NULL)
    {
        printf("Error opening file '%s'\n", filename);
        return -1;
    }

    memset(buf, 0xFF, size);

    do
    {
        int i;
        // ToDo: be able to handle any width
        got = fscanf(file, ":%02X%04X00%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\n",
            &len, &addr,
            data, data+1, data+2, data+3, data+4, data+5, data+6, data+7, data+8,
            data+9, data+10, data+11, data+12, data+13, data+14, data+15, data+16);
        if (got >= 4) // minimum: len, addr, data, checksum
        {

            if (addr < minpos)
                minpos = addr;

            if (addr < size) // the hexfile is allowed to contain the bootloader, too, only advance if below bootloader limit
            {
                if (addr + len > maxpos) 
                    maxpos = addr + len;
            
                for (i=addr; i<addr+len; i++) // copy the line of data
                {
                    if (i<size) // for the unlikely case the line spills into bootloader range
                    {
                        buf[i] = data[i-addr];
                    }
                }
            }
            // ToDo: verify checksum
        }
    } while (got);

    fclose (file);

    // return the range
    *p_minpos = minpos;
    *p_maxpos = maxpos-1; // this was one beyond last byte

    return 0; // success
}



// parse list of addresses
static int get_range(bool tag[256], char* arg)
{
    int range = -1; // inactive
    memset(tag, 0, sizeof(bool)*256);
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
                    tag[range++] = true;
                }
            }


            tag[adr] = true;
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


// process the command line, place result in global g_cmdline
int parse_commandline(int argc, char* argv[])
{
    int errcode = 0;
    int i = 0;
    int retval;

    memset(&g_cmdline, 0 , sizeof(g_cmdline));
    while (++i < argc)
    {
        if (argv[i][0] != '-' // must be an option
            || strlen(argv[i]) != 2) // all options are 2 chars long
        {
            printf("'%s' is no valid action or option!\n", argv[i]);
            return 1;
        }
        switch(argv[i][1])
        {
        case 'p': // program
            g_cmdline.do_program = true;
            break;
        case 'v': // only verify
            g_cmdline.do_verify = true;
            break;
        case 'd': // dump the error log
            g_cmdline.do_dump = true;
            break;
        case 'c': // clear the error log
            g_cmdline.do_clear = true;
            break;
        case 's': // serial port
            if (++i >= argc) // check if a value follows this option
            {   
                printf("-s isn't followed by a serial COM port argument!\n");
                return 2;
            }
            g_cmdline.serial_port = argv[i];
            break;
        case 'a': // address or address range set
            if (++i >= argc) // check if a value follows this option
            {   // check if a value follows this option
                printf("-a isn't followed by an address set argument!\n");
                return 3;
            }
            g_cmdline.adr_given = true;
            retval = get_range(g_cmdline.adresses, argv[i]);
            if (retval)
            {
                printf("'%s' is no valid address set!\n", argv[i]);
                return 4;
            }
            break;
        case 'h': // hexfile
            if (++i >= argc) // check if a value follows this option
            {   // check if a value follows this option
                printf("-h isn't followed by a hexfile name!\n");
                return 5;
            }
            g_cmdline.hexfile = argv[i];
            break;
        default:
            printf("option '%s' is unknown!\n", argv[i]);
            return 6;

        }
    }
    
    // plausibility checks
    if (g_cmdline.serial_port == NULL)
    {
        printf("no serial port given, use option -s to specify!\n");
        return 7;
    }

    if (!g_cmdline.do_program && !g_cmdline.do_verify && !g_cmdline.do_dump && !g_cmdline.do_clear)
    {
        printf("no action specified, use at least one of -p, -v, -d, -c!\n");
        return 8;
    }

    if (g_cmdline.do_program && g_cmdline.do_verify)
    {
        printf("action -p includes a verify, no need to specify -v\n");
        return 9;
    }

    if (g_cmdline.hexfile && !(g_cmdline.do_program || g_cmdline.do_verify))
    {
        printf("-h option is pointless without -p or -v action\n");
        return 10;
    }
    
    return 0;
}


// print instructions
static void usage(void)
{
    printf("OpenHC module firmware updater\n");
    printf("Start within 2.5 seconds after module powerup in case one is 'sticky'\n");
    
    printf("Usage: gangprog <actions> <options>\n");
    printf("actions are -p  (program and verify firmware),\n");
    printf("            -v  (just verify, no programming)\n");
    printf("            -d  (dump error log),\n");
    printf("            -c  (clear error log)\n");
    printf("options are -s <COM port> to which the STM is connected\n");
    printf("            -a <address set>, auto-detect if not specified\n");
    printf("            -h <hexfile to use>, auto-detect if none specified\n");
    printf("            -n  (no verify)\n");
    printf("option -s is mandatory, all others optional, only needed in special cases\n");
    printf("options -h and -n only make sense with action -p\n");
    printf("\n");
    printf("Examples:\n");
    printf("normal update procedure:\n");
    printf(" gangprog -s COM1 -p\n");
    printf("dump error logs and reset them:\n");
    printf(" gangprog -s COM1 -d -c\n");
    printf("program certain modules with a specific firmware:\n");
    printf(" gangprog -s COM1 -a 0,4,7-10 -h myfile.hex \n");
}


// read the configuration file
static int read_controlfile(char* filename)
{
    int i;
    FILE* p_file;
    int retval = 0;

    // zero init
    for (i=0; i<sizeof(g_file_lut)/sizeof(g_file_lut[0]); i++)
    {
        g_file_lut[i].id = 0;
    }

    p_file = fopen(filename, "r");
    if (p_file == NULL)
    {
        printf("Can't open control file '%s'!", filename);
        return 1;
    }

    i = 0;
    while (!feof(p_file))
    {
        char linebuf[1024];
        
        if (fgets(linebuf, sizeof(linebuf)-1, p_file) // read a line
         && linebuf[0] != '#') // doesn't start with comment char
        {
            int got;
            // ToDo: this won't work when the filename contains spaces
            got = sscanf(linebuf, "%06x %1024s\n", &g_file_lut[i].id, g_file_lut[i].hexfile);
            if (got == 2)
            {
                //printf("ID=0x%06X, file='%s'\n", g_file_lut[i].id, g_file_lut[i].hexfile);
                i++;
                if (i >= sizeof(g_file_lut)/sizeof(g_file_lut[0]))
                {   // very unlikely, but be sure
                    printf("Warning: control file '%s' contains more than %d entries from !", filename, i);
                    retval = 1;
                    break;
                }
            }
        }
    }

    fclose(p_file);

    return retval;
}

// check which hexfile to use
static const char* getHexfile(uint32_t id)
{
    int i;
    const char* retval = NULL;

    if (g_cmdline.hexfile != NULL) // command line override
    {
        return g_cmdline.hexfile; // use the user specified one
    }

    // search the table from the control file
    for (i=0; i<sizeof(g_file_lut)/sizeof(g_file_lut[0]); i++)
    {
        if (g_file_lut[i].id == 0) // ID=0 means end of table
        {
            break;
        }

        if (g_file_lut[i].id == id)
        {
            return g_file_lut[i].hexfile; // found
        }
    }

    return NULL;    
}

// flag all supported addresses as to be used
static void use_all_addr(void)
{
    int i;

    for (i=0; i<sizeof(g_file_lut)/sizeof(g_file_lut[0]); i++)
    {
        int addr;

        if (g_file_lut[i].id == 0) // ID=0 means end of table
        {
            break;
        }

        addr = (g_file_lut[i].id & 0x00E00000) >> 16; // extract address byte, module class
        if (!g_cmdline.adresses[addr]) // prevent double work, assuming all have been zero
        {
            int j;
            for (j=addr; j<addr+32; j++)
            {   // flag all addresses of that class
                g_cmdline.adresses[j] = true;
            }
        }
    }
}

/*
// callback function to visualise progress
static void cb_progress(int done, int total)
{
    if (done) // not the first?
    {
        printf("\x08\x08\x08\x08"); // erase the previous
    }
    printf("%3d%%", done * 100 / total);
    if (done >= total)
    {
        printf("\n");
    }
}
*/

// callback function to visualise progress
static void cb_progress(int done, int total)
{
    const int maxdots = 79; // how many dots should be 100%
    int dots = done * maxdots / total;
    static int last_dots;
    int i;

    if (done==0)
    {
        last_dots = 0;
    }
    
    for (i=last_dots; i<dots; i++)
    {
        printf(".");
    }
    last_dots = dots;

    if (done >= total)
    {
        printf("\n");
    }
}


// process a given module
static int actions (tUartHandle hUart, int addr, uint32_t id)
{
    int retval; // return value of called sub-functions
    int result = 0; // our return value

    uint32_t key; // the information needed to determine the proper firmware file
    int memlimit; // controller application program size
    int minpos, maxpos; // range to flash
    uint8_t buf[16*1024]; // program memory
    const char* hexfile; // pathname of hexfile to be used

    if (g_cmdline.do_dump)
    {
        printf("Dumping module %d\n", addr);
        retval = dump_errlog(hUart, addr);
        if (retval)
        {
            printf("Error while dumping module %d!\n", addr);
            return retval;
        }
    }

    if (g_cmdline.do_clear)
    {
        printf("Clearing module %d\n", addr);
        retval = clear_errlog(hUart, addr);
        if (retval)
        {
            printf("Error while clearing error log of module %d!\n", addr);
            return retval;
        }
    }

    // load the Hex file data for the module
    if (g_cmdline.do_program || g_cmdline.do_verify)
    {
        key = ((addr & 0xE0) << 16) | (id & 0xFFFF); // compose file key from address class and ID (containing type and MCU)
        memlimit = 0; // default to unsupported

        
        // check the target MCU, make memory limit dependent on that
        switch (id & 0xFF)
        {
        case MCU_AVR_MEGA8:
        case MCU_AVR_MEGA88:
            memlimit = 7*1024; // flash size minus bootloader
            break;
        case MCU_AVR_MEGA168:
            memlimit = 15*1024; // flash size minus bootloader
            break;
        case MCU_AVR_MEGA48:
        case MCU_MB90495:
        default: // currently unsupported
            break;
        }

        hexfile = getHexfile(key);
        if (hexfile == NULL)
        {
            printf("No hexfile known for module with ID %06X!\n", key);
            return retval;
        }

        printf("Using hexfile '%s' for module %d\n", hexfile, addr);

        // load the .hex file
        retval = hexload(hexfile, buf, memlimit, &minpos, &maxpos);
        if (retval)
        {
            printf("Error loading hexfile '%s'!\n", hexfile);
            return retval;
        }

        // check the image range
        printf("File '%s': binary range from 0x%X to 0x%X\n", hexfile, minpos, maxpos);
        if (maxpos >= memlimit) 
        {
            printf("Image too big\n");
            return -1;
        }
    }

    // flash the binary into the module, with verify
    if (g_cmdline.do_program)
    {
        printf("Programming and verifying module %d\n", addr);
        retval = flash(hUart, addr, buf, minpos, maxpos, cb_progress);
        if (retval)
        {
            printf("Error while flashing module %d!\n", addr);
            return retval;
        }
    }

    // verify the programming
    if (g_cmdline.do_verify)
    {
        printf("Verifying module %d\n", addr);
        retval = verify(hUart, addr, buf, minpos, maxpos, cb_progress);
        if (retval)
        {
            printf("Error while verifying module %d!\n", addr);
            result = 1;
        }
    }
    
    return 0;
}


// entry point
int main(int argc, char* argv[])
{
    int retval;
    int i;
    int errcount = 0;
    int modcount = 0;
    tUartHandle hUart; // COM port candle

    retval = parse_commandline(argc, argv);
    if (retval)
    {
        usage();
        exit(retval);
    }

    if ((g_cmdline.do_program || g_cmdline.do_verify) && g_cmdline.hexfile == NULL || !g_cmdline.adr_given)
    {   // normal case: no hexfile given, read the config for automatic mode
        retval = read_controlfile("files.cfg");
        if (retval)
        {
            printf("Problem with configuration file 'config.cfg', exiting!\n");
            exit(retval);
        }
    }

    if (!g_cmdline.adr_given) // no addresses given?
    {
        use_all_addr(); // default: flag all supported addresses as to be used
    }
    
    hUart = UartOpen(g_cmdline.serial_port); // open the serial port
    if (!hUart)
    {
        printf("Error opening serial port %s\n", g_cmdline.serial_port);
        return -2;
    }

    UartConfig(hUart, 19200, eNOPARITY, eTWOSTOPBITS, 8, 1000);

    for (i=0; i<256; i++)
    {
        if (g_cmdline.adresses[i])
        {
            uint32_t id;

            printf("Checking module %d\n", i);

            // bring module into boot mode
            retval = bootmode(hUart,i);
            if (retval > 0)
            {
                printf("PEHA PHC module found on address %d, skipping it.\n", i);
                continue;
            }
            else if (retval < 0) // didn't work?
            {
                if (g_cmdline.adr_given) // considered severe if not auto-scanning but address explicitely given
                {
                    printf("Module %d didn't enter bootmode, skipping it!\n", i);
                    errcount++;
                }
                continue; // carry on

            }
            printf("OpenHC module found on address %d.\n", i);
            modcount++;

            // query the module for its ID
            id = 0;
            retval = ping(hUart, i, &id);
            if (retval)
            {
                printf("Module %d didn't identify, skipping it\n", i);
                errcount++;
            }
            else
            {
                uint32_t version = ((id >> 16) & 0xFF);

                printf("Module %d has ID %06X.\n", i, id);

                // check bootloader version
                if (version == 0x20 || version == 0x21) // so far, 2.0 and 2.1 are known and supported
                {   // OK: process the module
                    retval = actions(hUart, i, id); // here's the action
                    if (retval)
                    {   // error output has been done by actions() already
                        errcount++;
                    }
                }
                else
                { // not 2.x
                    printf("Module %d states unsupported bootloader version %d.%d, skipping it\n",
                        i, version >> 4, version & 0xF); // major and minor
                    errcount++;
                }
            }
            
            // restart the module
            retval = start_app(hUart, i, id);
            // probably pointless to check return value
        }
    }
    
    printf("\nSummary: %d modules processed, %d errors.\n", modcount, errcount);

    UartClose(hUart);

    return errcount;
}
