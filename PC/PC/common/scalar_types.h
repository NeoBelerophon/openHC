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

/*! \file scalar_types.h
    \brief This resolves platform dependency of simple types and macros.
*/

#ifndef _SCALAR_TYPES_H
#define _SCALAR_TYPES_H


#ifdef WIN32
#include <windows.h>
typedef signed char       int8_t;
typedef unsigned char    uint8_t;
typedef signed short      int16_t;
typedef unsigned short   uint16_t;
typedef signed long       int32_t;
typedef unsigned long    uint32_t;
typedef __int64           int64_t;
typedef unsigned __int64 uint64_t;
#else
#include <stdint.h> // decent compilers have this
#endif


#ifndef bool
#define bool int
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

#ifndef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))
#endif

#ifndef MAX
#define MAX(a,b) ((a)>(b)?(a):(b))
#endif


#endif // #ifndef _SCALAR_TYPES_H


