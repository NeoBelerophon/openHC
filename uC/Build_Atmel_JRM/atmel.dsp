# Microsoft Developer Studio Project File - Name="atmel" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=atmel - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "atmel.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "atmel.mak" CFG="atmel - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "atmel - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "atmel - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "atmel - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f atmel.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "atmel.exe"
# PROP BASE Bsc_Name "atmel.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "make_msdev.bat"
# PROP Rebuild_Opt "clean"
# PROP Target_File "dummy.exe"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "atmel - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f atmel.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "atmel.exe"
# PROP BASE Bsc_Name "atmel.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "make_msdev.bat"
# PROP Rebuild_Opt "clean"
# PROP Target_File "dummy.exe"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "atmel - Win32 Release"
# Name "atmel - Win32 Debug"

!IF  "$(CFG)" == "atmel - Win32 Release"

!ELSEIF  "$(CFG)" == "atmel - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\Src_common\errlog.c
# End Source File
# Begin Source File

SOURCE=..\Src_common\platform_avr\hal_atmel.c
# End Source File
# Begin Source File

SOURCE=..\Src_JRM\jrm.c
# End Source File
# Begin Source File

SOURCE=..\Src_JRM\main.c
# End Source File
# Begin Source File

SOURCE=..\Src_common\msg.c
# End Source File
# Begin Source File

SOURCE=..\Src_common\phc.c
# End Source File
# Begin Source File

SOURCE=..\Src_common\random.c
# End Source File
# Begin Source File

SOURCE=..\Src_common\timer.c
# End Source File
# Begin Source File

SOURCE=..\Src_common\uart.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "AVR"

# PROP Default_Filter ""
# Begin Source File

SOURCE=c:\Programme\Developer\WinAVR\avr\include\avr\crc16.h
# End Source File
# Begin Source File

SOURCE=c:\Programme\Developer\WinAVR\avr\include\avr\delay.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\..\..\..\Programme\Developer\WinAVR\avr\include\avr\eeprom.h
# End Source File
# Begin Source File

SOURCE=c:\Programme\Developer\WinAVR\avr\include\avr\interrupt.h
# End Source File
# Begin Source File

SOURCE=c:\Programme\Developer\WinAVR\avr\include\avr\io.h
# End Source File
# Begin Source File

SOURCE=c:\Programme\Developer\WinAVR\avr\include\avr\iom8.h
# End Source File
# Begin Source File

SOURCE=c:\Programme\Developer\WinAVR\avr\include\avr\sleep.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\..\..\..\Programme\Developer\WinAVR\avr\include\avr\wdt.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\Src_common\errlog.h
# End Source File
# Begin Source File

SOURCE=..\Src_common\hal.h
# End Source File
# Begin Source File

SOURCE=..\Src_common\Platform_AVR\hal_platform.h
# End Source File
# Begin Source File

SOURCE=..\Src_JRM\jrm.h
# End Source File
# Begin Source File

SOURCE=..\Src_common\monitor.h
# End Source File
# Begin Source File

SOURCE=..\Src_common\msg.h
# End Source File
# Begin Source File

SOURCE=..\Src_common\phc.h
# End Source File
# Begin Source File

SOURCE=..\Src_common\random.h
# End Source File
# Begin Source File

SOURCE=..\Src_common\timer.h
# End Source File
# Begin Source File

SOURCE=..\Src_common\Platform_AVR\timer_platform.h
# End Source File
# Begin Source File

SOURCE=..\Src_common\uart.h
# End Source File
# Begin Source File

SOURCE=..\Src_common\Platform_AVR\uart_platform.h
# End Source File
# End Group
# Begin Group "Specification"

# PROP Default_Filter "txt"
# Begin Source File

SOURCE=..\Src_JRM\spec.txt
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "Make file"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\isp.e2s
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\makefile

!IF  "$(CFG)" == "atmel - Win32 Release"

!ELSEIF  "$(CFG)" == "atmel - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\Src_common\Platform_AVR\makefile.common
# PROP Exclude_From_Build 1
# End Source File
# End Group
# End Target
# End Project
