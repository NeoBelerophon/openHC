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

/*! \file hal_linux.c
    \brief Hardware abstraction layer, Linux implementation.
*/

#include <sys/time.h>
#include "scalar_types.h"
#include "hal.h"


void sleep(int ms)
{
    struct timespec rqtp;

    rqtp.tv_sec = ms / 1000;
    rqtp.tv_nsec = ms % 1000000L;

    nanosleep(&rqtp, NULL);
}

double hires_time()
{
    struct timeval tv;
    double seconds;

    gettimeofday(&tv, NULL);
    seconds = (double)(tv.tv_sec) + (double)(tv.tv_usec) * 1e-6;

    return seconds;
}
