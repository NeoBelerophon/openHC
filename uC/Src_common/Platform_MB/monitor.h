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

/*! \file monitor.h
    \brief Interface to ROM monitor, Fujitsu MB90F49x part.

    This is used for debugging with the Accemic debugger.
*/

#ifndef MONFUNC_H
#define MONFUNC_H

/************************************************************************
acc_InitMonitorMsgBuffer

Initilisation of message buffer

Parameters:

aMsgBuf:		Pointer to buffer.
aMaxBufItems: 	Size of buffer in bytes(min. 12 Bytes max: 255 Bytes)
*************************************************************************/

extern void __far (* __far acc_InitMonitorMsgBuffer)
	(__far char* MsgBuf,char BufferSize);

/************************************************************************
acc_printf
Use to show text in the Target Message Box

Parameters:
Flag:		See above
Data:		Pointer to data to transmit
Position:	Position of row (if type==ACC_TYPE_ROW)
Wait:		0 Do not wait for tranmission end
			1 Wait for transmission end

Returnvalue:
-1 :Error
0  :PC not connected or transmission not possible
1  :transmitted

*************************************************************************/
extern char __far (* __far acc_printf)
	(char Location,char Type,const __far void* Data,unsigned char Position,char Wait);
//Values for Location:
#define ACC_TYPE_LISTBOX				0x00
#define ACC_TYPE_ROW_TEXT               0x08
#define ACC_TYPE_ROW_CAPTION			0xF0
//Defintions for types to display (specify if ACC_TYPE_LISTBOX or  ACC_TYPE_ROW_TEXT)
#define ACC_TYPE_STRING					0x10
#define ACC_TYPE_UCHAR   				0x30	//1 Byte
#define ACC_TYPE_CHAR   				0x40  	//1 Byte
#define ACC_TYPE_USHORT   				0x50	//2 Bytes
#define ACC_TYPE_SHORT   				0x60  	//2 Bytes
#define ACC_TYPE_ULONG     				0x70   	//4 Bytes
#define ACC_TYPE_LONG   				0x80    //4 Bytes
#define ACC_TYPE_FLOAT   				0x90	//4 Bytes
#define ACC_TYPE_DOUBLE   				0xA0	//8 Bytes

/************************************************************************
acc_MessageBox
Use to show a message box in MDE

Parameters:

Msg:		Message text to show
Flags:		See above

Returnvalue:
See above
*************************************************************************/
extern char __far (* __far acc_MessageBox)
	(const __far char* Msg,char Flag);

//Values for Flag
//Indicates what kind of button to show
#define MB_OK				0
#define MB_OKCANCEL			1
#define MB_ABORTRETRYIGNORE	2
#define MB_YESNOCANCEL		3
#define MB_YESNO			4
#define MB_RETRYCANCEL		5


//Return value of DebugMsg
#define IDERROR          -1 //Error 
#define IDNOTCONNTETED   0  //PC not connected or transmission not possible
#define IDOK	         1	//Button OK was pressed
#define IDCANCEL	     2	//Button Cancel  was pressed
#define IDABORT	         3	//Button Abort  was pressed
#define IDRETRY	         4	//Button Retry  was pressed
#define IDIGNORE	     5  //Button Ignore  was pressed
#define IDYES	         6	//Button Yes  was pressed
#define IDNO	         7	//Button No  was pressed


/************************************************************************
acc_WatchdogEnable

Enable Watchdog support of Accemic MDE
*************************************************************************/

extern void __far (* __far acc_WatchdogEnable)(void);


/************************************************************************
acc_KernelUART

Disable/Enables the Kernel UART for application use
*************************************************************************/

extern void __far (* __far acc_KernelUART)(char Disable);

#endif