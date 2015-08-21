/*  FFMC-16 IO-MAP HEADER FILE      */
/*  ==========================      */
/* CREATED BY IO-WIZARD V2.8   */
/* DATE: 19/08/02 TIME: 1:08:52 PM */
/* *********************************************************** */
/*               FUJITSU MIKROELEKTRONIK GMBH                  */
/*               Am Siebenstein 6-10, 63303 Dreieich           */
/*               Tel.:++49/6103/690-0,Fax - 122                */
/*                                                             */
/* The following software is for demonstration purposes only.  */
/* It is not fully tested, nor validated in order to fullfill  */
/* its task under all circumstances. Therefore, this software  */
/* or any part of it must only be used in an evaluation        */
/* laboratory environment.                                     */
/* This software is subject to the rules of our standard       */
/* DISCLAIMER, that is delivered with our SW-tools (on the CD  */
/* or see our Internet Page -                                     */
/* http://www.fujitsu-ede.com/products/micro/disclaimer.html)  */
/* *********************************************************** */
/* History: */
/* Date     Version     Author  Description */
/* 21.10.99 1.0         VSA     created */
/* 16.11.99 1.1         VSA     ... */
/* 17.11.99 1.2         VSA     ... */
/* 18.11.99 1.3         WP      Rom Mirror + WTC added + O>0 at PPG */
/* 29.11.99 1.4         WP      IPCP2+3 added UART1 register SMC1 > SMR1 */
/* 08.02.00 2.0         HLO     added: !D to canh, canm was missing */
/*                              canmac.h, canstr.h -> canmac8.h, canstr.h */
/* 09.02.00 2.1         HLO     CANSTR8.H updated */
/* 13.04.00 2.2		VSA	TCCSH, TCCSL added */
/* 18.05.00 2.3		VSA	write-only bits verified */
/* 03.07.00 2.4         VSA     ADC unit included (adc_01.h) */
/* 10.08.00 2.5         HLO     CANHSTR changed, wrong alignment of AMR */
/* 11.10.00 2.6         MST     missing CLK2 bit in TCCS Register (IOTimer) added */
/* 27.10.00 2.7         NMP     ADCS RESV bit location corrected */
/*                              PPGn Control Register bit description completed */
/*                              16bit timer TCCSH_ECKE description corrected from FRCK */
/*                              SCR0 and SCR1 bit descriptions added */
/* 08.11.00 2.8         NMP     Register bit TCCS_ICLR corrected */
/* 22.02.01 2.9         NMP     Register bit TCCS bit corrected */
/* 28.03.01 2.10        TKA     Section IOXTND splitted into IOXTND and IOXTND2 because of area in case of external bus used */
/* 11.04.01 2.11        HWE     UART1: SMR1 UPCL Bit added */
/*                              UART1: SCR1 REC  Bit added */
/*                              UART1: SSR1 BDS  Bit added */
/*                              UART0: SCR0 REC  Bit added */
/* 10.12.01 2.12        HWE     TCCS: Bit ECKE added */
/* 23.07.02 2.13        HWe     new adc_01.h, icr.h (RMW-Problem) */
/* 19.08.02 2.14        HWE     Bitdefinition PPGC1/3 corrected (MD0, MD1) */

 .PROGRAM MB90495
 .TITLE   MB90495

;------------------------
; IO-AREA DEFINITIONS :
;------------------------



 .section IOBASE, IO, locate=0x0000  ; /*  PORT DATA Registers */
 .GLOBAL __pdr0,     __pdr1,     __pdr2,     __pdr3,     __pdr4,     __pdr5
 .GLOBAL __pdr6,     __ddr0,     __ddr1,     __ddr2,     __ddr3,     __ddr4
 .GLOBAL __ddr5,     __ddr6,     __ader,     __smr0,     __scr0,     __sidr0
 .GLOBAL __sodr0,    __ssr0,     __cdcr0,    __ses0,     __smr1,     __scr1
 .GLOBAL __sidr1,    __sodr1,    __ssr1,     __cdcr1,    __enir,     __eirr
 .GLOBAL __elvr,     __adcs,     __adcs0,    __adcs1,    __adcr,     __adcr0
 .GLOBAL __adcr1,    __ppgc0,    __ppgc1,    __ppg01,    __ppgc2,    __ppgc3
 .GLOBAL __ppg23,    __ipcp0,    __ipcp1,    __ics01,    __ics23,    __tcdt
 .GLOBAL __tcdtl,    __tcdth,    __tccs,     __tccsl,    __tccsh,    __ipcp2
 .GLOBAL __ipcp3,    __tmcsr0,   __tmcsr1,   __romm,     __canl,     __pacsr
 .GLOBAL __dirr,     __lpmcr,    __ckscr,    __arsr,     __hacr,     __ecsr
 .GLOBAL __wdtc,     __tbtc,     __wtc,      __fmcs,     __icr

__pdr0   .res.b 1             ;000000  /*  PORT DATA Registers */
PDR0    .equ 0x0000
__pdr1   .res.b 1             ;000001
PDR1    .equ 0x0001
__pdr2   .res.b 1             ;000002
PDR2    .equ 0x0002
__pdr3   .res.b 1             ;000003
PDR3    .equ 0x0003
__pdr4   .res.b 1             ;000004
PDR4    .equ 0x0004
__pdr5   .res.b 1             ;000005
PDR5    .equ 0x0005
__pdr6   .res.b 1             ;000006
PDR6    .equ 0x0006
 .org 0x0010
__ddr0   .res.b 1             ;000010  /* PORT DIR */
DDR0    .equ 0x0010
__ddr1   .res.b 1             ;000011
DDR1    .equ 0x0011
__ddr2   .res.b 1             ;000012
DDR2    .equ 0x0012
__ddr3   .res.b 1             ;000013
DDR3    .equ 0x0013
__ddr4   .res.b 1             ;000014
DDR4    .equ 0x0014
__ddr5   .res.b 1             ;000015
DDR5    .equ 0x0015
__ddr6   .res.b 1             ;000016
DDR6    .equ 0x0016
 .org 0x001B
__ader   .res.b 1             ;00001B  /* AnalogInputEnable Port 5 */
ADER    .equ 0x001B
 .org 0x0020
__smr0   .res.b 1             ;000020  /* UART0 */
SMR0    .equ 0x0020
__scr0   .res.b 1             ;000021
SCR0    .equ 0x0021
__sidr0   .res.b 1             ;000022
SIDR0    .equ 0x0022
 .org 0x0022
__sodr0   .res.b 1             ;000022
SODR0    .equ 0x0022
__ssr0   .res.b 1             ;000023
SSR0    .equ 0x0023
__cdcr0   .res.b 1             ;000024
CDCR0    .equ 0x0024
__ses0   .res.b 1             ;000025
SES0    .equ 0x0025
__smr1   .res.b 1             ;000026  /* UART1 */
SMR1    .equ 0x0026
__scr1   .res.b 1             ;000027
SCR1    .equ 0x0027
__sidr1   .res.b 1             ;000028
SIDR1    .equ 0x0028
 .org 0x0028
__sodr1   .res.b 1             ;000028
SODR1    .equ 0x0028
__ssr1   .res.b 1             ;000029
SSR1    .equ 0x0029
 .org 0x002B
__cdcr1   .res.b 1             ;00002B
CDCR1    .equ 0x002B
 .org 0x0030
__enir   .res.b 1             ;000030  /* DTP, External Interrupts */
ENIR    .equ 0x0030
__eirr   .res.b 1             ;000031
EIRR    .equ 0x0031
__elvr   .res.b 2             ;000032
ELVR    .equ 0x0032
__adcs   .res.b 2             ;000034  /* AD Converter */
ADCS    .equ 0x0034
 .org 0x0034
__adcs0   .res.b 1             ;000034
ADCS0    .equ 0x0034
__adcs1   .res.b 1             ;000035
ADCS1    .equ 0x0035
__adcr   .res.b 2             ;000036
ADCR    .equ 0x0036
 .org 0x0036
__adcr0   .res.b 1             ;000036
ADCR0    .equ 0x0036
__adcr1   .res.b 1             ;000037
ADCR1    .equ 0x0037
 .org 0x0040
__ppgc0   .res.b 1             ;000040  /* Puls Pattern Generator Channel 0,1 */
PPGC0    .equ 0x0040
__ppgc1   .res.b 1             ;000041
PPGC1    .equ 0x0041
__ppg01   .res.b 1             ;000042
PPG01    .equ 0x0042
 .org 0x0044
__ppgc2   .res.b 1             ;000044
PPGC2    .equ 0x0044
__ppgc3   .res.b 1             ;000045
PPGC3    .equ 0x0045
__ppg23   .res.b 1             ;000046
PPG23    .equ 0x0046
 .org 0x0050
__ipcp0   .res.b 2             ;000050  /* ICU unit 0/1 */
IPCP0    .equ 0x0050
__ipcp1   .res.b 2             ;000052
IPCP1    .equ 0x0052
__ics01   .res.b 1             ;000054  /* Input Capture 0/1/2/3 */
ICS01    .equ 0x0054
__ics23   .res.b 1             ;000055
ICS23    .equ 0x0055
__tcdt   .res.b 2             ;000056  /* I/O Timer */
TCDT    .equ 0x0056
 .org 0x0056
__tcdtl   .res.b 1             ;000056
TCDTL    .equ 0x0056
__tcdth   .res.b 1             ;000057
TCDTH    .equ 0x0057
__tccs   .res.b 2             ;000058
TCCS    .equ 0x0058
 .org 0x0058
__tccsl   .res.b 1             ;000058
TCCSL    .equ 0x0058
__tccsh   .res.b 1             ;000059
TCCSH    .equ 0x0059
__ipcp2   .res.b 2             ;00005A  /* ICU unit 2/3 */
IPCP2    .equ 0x005A
__ipcp3   .res.b 2             ;00005C
IPCP3    .equ 0x005C
 .org 0x0066
__tmcsr0   .res.b 2             ;000066  /* Reload Timer 0/1 */
TMCSR0    .equ 0x0066
__tmcsr1   .res.b 2             ;000068
TMCSR1    .equ 0x0068
 .org 0x006F
__romm   .res.b 1             ;00006F  /* ROM mirror function */
ROMM    .equ 0x006F
 .org 0x0080
__canl   .res.b 0x10          ;000080  /*  CAN buffer control, lower part */
CANL    .equ 0x0080
 .org 0x009E
__pacsr   .res.b 1             ;00009E  /* Program Patch Function */
PACSR    .equ 0x009E
__dirr   .res.b 1             ;00009F  /* Delayed Interrupt */
DIRR    .equ 0x009F
__lpmcr   .res.b 1             ;0000A0  /* Low Power Control Registers */
LPMCR    .equ 0x00A0
__ckscr   .res.b 1             ;0000A1
CKSCR    .equ 0x00A1
 .org 0x00A5
__arsr   .res.b 1             ;0000A5  /* Bus Pin Control Circuit */
ARSR    .equ 0x00A5
__hacr   .res.b 1             ;0000A6
HACR    .equ 0x00A6
__ecsr   .res.b 1             ;0000A7
ECSR    .equ 0x00A7
__wdtc   .res.b 1             ;0000A8  /* Watch Dog */
WDTC    .equ 0x00A8
__tbtc   .res.b 1             ;0000A9  /* Time Base Timer */
TBTC    .equ 0x00A9
__wtc   .res.b 1             ;0000AA  /* Watch Timer */
WTC    .equ 0x00AA
 .org 0x00AE
__fmcs   .res.b 1             ;0000AE  /* Flash Control Register */
FMCS    .equ 0x00AE
 .org 0x00B0
__icr   .res.b 0x10          ;0000B0  /* Interrupt Control Registers */
ICR    .equ 0x00B0

 .section IOXTND, DATA, locate=0x1FF0  ; /* ROM CORRECTION */
 .GLOBAL __padr0_l,  __padr0_m,  __padr0_h,  __padr1_l,  __padr1_m,  __padr1_h

__padr0_l   .res.b 1             ;001FF0  /* ROM CORRECTION */
PADR0_L    .equ 0x1FF0
__padr0_m   .res.b 1             ;001FF1
PADR0_M    .equ 0x1FF1
__padr0_h   .res.b 1             ;001FF2
PADR0_H    .equ 0x1FF2
__padr1_l   .res.b 1             ;001FF3
PADR1_L    .equ 0x1FF3
__padr1_m   .res.b 1             ;001FF4
PADR1_M    .equ 0x1FF4
__padr1_h   .res.b 1             ;001FF5
PADR1_H    .equ 0x1FF5

 .section IOXTND2, DATA, locate=0x3900  ; /* Reload Timer 0/1 */
 .GLOBAL __tmr0,     __tmrlr0,   __tmr1,     __tmrlr1,   __prl0,     __prll0
 .GLOBAL __prlh0,    __prl1,     __prll1,    __prlh1,    __prl2,     __prll2
 .GLOBAL __prlh2,    __prl3,     __prll3,    __prlh3,    __canm,     ___dmyc0
 .GLOBAL ___dmyc1,   ___dmyc2,   __canh,     ___endio

__tmr0   .res.b 2             ;003900  /* Reload Timer 0/1 */
TMR0    .equ 0x3900
 .org 0x3900
__tmrlr0   .res.b 2             ;003900
TMRLR0    .equ 0x3900
__tmr1   .res.b 2             ;003902
TMR1    .equ 0x3902
 .org 0x3902
__tmrlr1   .res.b 2             ;003902
TMRLR1    .equ 0x3902
 .org 0x3910
__prl0   .res.b 2             ;003910  /* PPG 0/1/2/3 */
PRL0    .equ 0x3910
 .org 0x3910
__prll0   .res.b 1             ;003910
PRLL0    .equ 0x3910
__prlh0   .res.b 1             ;003911
PRLH0    .equ 0x3911
__prl1   .res.b 2             ;003912
PRL1    .equ 0x3912
 .org 0x3912
__prll1   .res.b 1             ;003912
PRLL1    .equ 0x3912
__prlh1   .res.b 1             ;003913
PRLH1    .equ 0x3913
__prl2   .res.b 2             ;003914
PRL2    .equ 0x3914
 .org 0x3914
__prll2   .res.b 1             ;003914
PRLL2    .equ 0x3914
__prlh2   .res.b 1             ;003915
PRLH2    .equ 0x3915
__prl3   .res.b 2             ;003916
PRL3    .equ 0x3916
 .org 0x3916
__prll3   .res.b 1             ;003916
PRLL3    .equ 0x3916
__prlh3   .res.b 1             ;003917
PRLH3    .equ 0x3917
 .org 0x3C00
__canm   .res.b 0x40          ;003C00  /* CAN message buffer */
CANM    .equ 0x3C00
___dmyc0   .res.b 0x40          ;003C40
_DMYC0    .equ 0x3C40
___dmyc1   .res.b 0x40          ;003C80
_DMYC1    .equ 0x3C80
___dmyc2   .res.b 0x40          ;003CC0
_DMYC2    .equ 0x3CC0
__canh   .res.b 0x1C          ;003D00  /* Status and control register, higher part */
CANH    .equ 0x3D00
 .org 0x3FFF
___endio   .res.b 1             ;003FFF  /*  dummy end for IO segment area IOBASE */
_ENDIO    .equ 0x3FFF


 .end
