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

/*! \file hal_win.c
    \brief Hardware abstraction layer, Win32 implementation.
*/

#include "scalar_types.h"
#include "hal.h"


void sleep(int ms)
{
    Sleep(ms);
}

double hires_time()
{
    LARGE_INTEGER curr, freq;
    double seconds;

    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&curr);
    
    seconds = (double)curr.QuadPart / (double)freq.QuadPart;

    return seconds;
}
