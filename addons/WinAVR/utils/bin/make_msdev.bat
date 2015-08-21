@echo off
rem This converts gcc errors to MSDev error format

set PERL_PATH="C:\Programme\Developer\Perl\bin\perl.exe"
set SCRIPT_PATH="c:\Programme\Developer\WinAVR\utils\bin\gcc2vs6Error.pl"

if "%1"=="clean" make clean
if "%1"=="clean" shift

make %1 %2 %3 %4 2>&1 | %PERL_PATH% %SCRIPT_PATH%
