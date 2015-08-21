/*  FFMC-16 IO-MAP HEADER FILE      */
/*  ==========================      */
/* SOFTUNE WORKBENCH FORMAT         */
/* C-DEFINITIONS FOR IO-SYMBOLS     */
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


#ifndef   __MB90XXX_H
#  define __MB90XXX_H
/*
- Please define __IO_NEAR in LARGE and COMPACT memory model, if the default
  data bank (DTB) is 00. This will result in better performance in these
  models.
- Please define __IO_FAR in SMALL and MEDIUM memory model, if the default
  data bank (DTB) is other than 00. This might be the case in systems with
  external RAM, which are not using internal RAM as default data area.
- Please define neither __IO_NEAR nor __IO_FAR in all other cases. This
  will work with almost all configurations.
*/

#  ifdef  __IO_NEAR
#    ifdef  __IO_FAR
#      error __IO_NEAR and __IO_FAR must not be defined at the same time
#    else
#      define ___IOWIDTH __near
#    endif
#  else
#    ifdef __IO_FAR
#      define ___IOWIDTH __far
#    else                               /* specified by memory model */
#      define ___IOWIDTH
#    endif
#  endif
#  ifdef  __IO_DEFINE
#    define __IO_EXTERN
#    define __IO_EXTENDED volatile ___IOWIDTH
#  else
#    define __IO_EXTERN   extern      /* for data, which can have __io */
#    define __IO_EXTENDED extern volatile ___IOWIDTH
#  endif

typedef unsigned char		IO_BYTE;
typedef unsigned short		IO_WORD;
typedef unsigned long		IO_LWORD;
typedef const unsigned short	IO_WORD_READ;

/* REGISTER BIT STRUCTURES */

typedef union{   /*  PORT DATA Registers */
    IO_BYTE	byte;
    struct{
    IO_BYTE P00 :1;
    IO_BYTE P01 :1;
    IO_BYTE P02 :1;
    IO_BYTE P03 :1;
    IO_BYTE P04 :1;
    IO_BYTE P05 :1;
    IO_BYTE P06 :1;
    IO_BYTE P07 :1;
  }bit;
 }PDR0STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE P10 :1;
    IO_BYTE P11 :1;
    IO_BYTE P12 :1;
    IO_BYTE P13 :1;
    IO_BYTE P14 :1;
    IO_BYTE P15 :1;
    IO_BYTE P16 :1;
    IO_BYTE P17 :1;
  }bit;
 }PDR1STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE P20 :1;
    IO_BYTE P21 :1;
    IO_BYTE P22 :1;
    IO_BYTE P23 :1;
    IO_BYTE P24 :1;
    IO_BYTE P25 :1;
    IO_BYTE P26 :1;
    IO_BYTE P27 :1;
  }bit;
 }PDR2STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE P30 :1;
    IO_BYTE P31 :1;
    IO_BYTE P32 :1;
    IO_BYTE P33 :1;
    IO_BYTE P34 :1;
    IO_BYTE P35 :1;
    IO_BYTE P36 :1;
    IO_BYTE P37 :1;
  }bit;
 }PDR3STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE P40 :1;
    IO_BYTE P41 :1;
    IO_BYTE P42 :1;
    IO_BYTE P43 :1;
    IO_BYTE P44 :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
  }bit;
 }PDR4STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE P50 :1;
    IO_BYTE P51 :1;
    IO_BYTE P52 :1;
    IO_BYTE P53 :1;
    IO_BYTE P54 :1;
    IO_BYTE P55 :1;
    IO_BYTE P56 :1;
    IO_BYTE P57 :1;
  }bit;
 }PDR5STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE P60 :1;
    IO_BYTE P61 :1;
    IO_BYTE P62 :1;
    IO_BYTE P63 :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
  }bit;
 }PDR6STR;
typedef union{   /* PORT DIR */
    IO_BYTE	byte;
    struct{
    IO_BYTE D00 :1;
    IO_BYTE D01 :1;
    IO_BYTE D02 :1;
    IO_BYTE D03 :1;
    IO_BYTE D04 :1;
    IO_BYTE D05 :1;
    IO_BYTE D06 :1;
    IO_BYTE D07 :1;
  }bit;
 }DDR0STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE D10 :1;
    IO_BYTE D11 :1;
    IO_BYTE D12 :1;
    IO_BYTE D13 :1;
    IO_BYTE D14 :1;
    IO_BYTE D15 :1;
    IO_BYTE D16 :1;
    IO_BYTE D17 :1;
  }bit;
 }DDR1STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE D20 :1;
    IO_BYTE D21 :1;
    IO_BYTE D22 :1;
    IO_BYTE D23 :1;
    IO_BYTE D24 :1;
    IO_BYTE D25 :1;
    IO_BYTE D26 :1;
    IO_BYTE D27 :1;
  }bit;
 }DDR2STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE D30 :1;
    IO_BYTE D31 :1;
    IO_BYTE D32 :1;
    IO_BYTE D33 :1;
    IO_BYTE D34 :1;
    IO_BYTE D35 :1;
    IO_BYTE D36 :1;
    IO_BYTE D37 :1;
  }bit;
 }DDR3STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE D40 :1;
    IO_BYTE D41 :1;
    IO_BYTE D42 :1;
    IO_BYTE D43 :1;
    IO_BYTE D44 :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
  }bit;
 }DDR4STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE D50 :1;
    IO_BYTE D51 :1;
    IO_BYTE D52 :1;
    IO_BYTE D53 :1;
    IO_BYTE D54 :1;
    IO_BYTE D55 :1;
    IO_BYTE D56 :1;
    IO_BYTE D57 :1;
  }bit;
 }DDR5STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE D60 :1;
    IO_BYTE D61 :1;
    IO_BYTE D62 :1;
    IO_BYTE D63 :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
  }bit;
 }DDR6STR;
typedef union{   /* AnalogInputEnable Port 5 */
    IO_BYTE	byte;
    struct{
    IO_BYTE ADE0 :1;
    IO_BYTE ADE1 :1;
    IO_BYTE ADE2 :1;
    IO_BYTE ADE3 :1;
    IO_BYTE ADE4 :1;
    IO_BYTE ADE5 :1;
    IO_BYTE ADE6 :1;
    IO_BYTE ADE7 :1;
  }bit;
 }ADERSTR;
typedef union{   /* UART0 */
    IO_BYTE	byte;
    struct{
    IO_BYTE SOE :1;
    IO_BYTE SCKE :1;
    IO_BYTE  :1;
    IO_BYTE CS0 :1;
    IO_BYTE CS1 :1;
    IO_BYTE CS2 :1;
    IO_BYTE MD0 :1;
    IO_BYTE MD1 :1;
  }bit;
  struct{
    IO_BYTE :1;
    IO_BYTE :1;
    IO_BYTE :1;
    IO_BYTE CS :3;
    IO_BYTE MD :2;
  }bitc;
 }SMR0STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE TXE :1;
    IO_BYTE RXE :1;
    IO_BYTE REC :1;
    IO_BYTE AD :1;
    IO_BYTE CL :1;
    IO_BYTE SBL :1;
    IO_BYTE P :1;
    IO_BYTE PEN :1;
  }bit;
 }SCR0STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE D0 :1;
    IO_BYTE D1 :1;
    IO_BYTE D2 :1;
    IO_BYTE D3 :1;
    IO_BYTE D4 :1;
    IO_BYTE D5 :1;
    IO_BYTE D6 :1;
    IO_BYTE D7 :1;
  }bit;
 }SIDR0STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE D0 :1;
    IO_BYTE D1 :1;
    IO_BYTE D2 :1;
    IO_BYTE D3 :1;
    IO_BYTE D4 :1;
    IO_BYTE D5 :1;
    IO_BYTE D6 :1;
    IO_BYTE D7 :1;
  }bit;
 }SODR0STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE TIE :1;
    IO_BYTE RIE :1;
    IO_BYTE  :1;
    IO_BYTE TDRE :1;
    IO_BYTE RDRF :1;
    IO_BYTE FRE :1;
    IO_BYTE ORE :1;
    IO_BYTE PE :1;
  }bit;
 }SSR0STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE DIV0 :1;
    IO_BYTE DIV1 :1;
    IO_BYTE DIV2 :1;
    IO_BYTE DIV3 :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE MD :1;
  }bit;
  struct{
    IO_BYTE DIV :4;
  }bitc;
 }CDCR0STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE NEG :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
  }bit;
 }SES0STR;
typedef union{   /* UART1 */
    IO_BYTE	byte;
    struct{
    IO_BYTE SOE :1;
    IO_BYTE SCKE :1;
    IO_BYTE UPCL :1;
    IO_BYTE CS0 :1;
    IO_BYTE CS1 :1;
    IO_BYTE CS2 :1;
    IO_BYTE MD0 :1;
    IO_BYTE MD1 :1;
  }bit;
  struct{
    IO_BYTE :1;
    IO_BYTE :1;
    IO_BYTE :1;
    IO_BYTE CS :3;
    IO_BYTE MD :2;
  }bitc;
 }SMR1STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE TXE :1;
    IO_BYTE RXE :1;
    IO_BYTE REC :1;
    IO_BYTE AD :1;
    IO_BYTE CL :1;
    IO_BYTE SBL :1;
    IO_BYTE P :1;
    IO_BYTE PEN :1;
  }bit;
 }SCR1STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE D0 :1;
    IO_BYTE D1 :1;
    IO_BYTE D2 :1;
    IO_BYTE D3 :1;
    IO_BYTE D4 :1;
    IO_BYTE D5 :1;
    IO_BYTE D6 :1;
    IO_BYTE D7 :1;
  }bit;
 }SIDR1STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE D0 :1;
    IO_BYTE D1 :1;
    IO_BYTE D2 :1;
    IO_BYTE D3 :1;
    IO_BYTE D4 :1;
    IO_BYTE D5 :1;
    IO_BYTE D6 :1;
    IO_BYTE D7 :1;
  }bit;
 }SODR1STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE TIE :1;
    IO_BYTE RIE :1;
    IO_BYTE BDS :1;
    IO_BYTE TDRE :1;
    IO_BYTE RDRF :1;
    IO_BYTE FRE :1;
    IO_BYTE ORE :1;
    IO_BYTE PE :1;
  }bit;
 }SSR1STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE DIV0 :1;
    IO_BYTE DIV1 :1;
    IO_BYTE DIV2 :1;
    IO_BYTE DIV3 :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE MD :1;
  }bit;
  struct{
    IO_BYTE DIV :4;
  }bitc;
 }CDCR1STR;
typedef union{   /* DTP, External Interrupts */
    IO_BYTE	byte;
    struct{
    IO_BYTE EN0 :1;
    IO_BYTE EN1 :1;
    IO_BYTE EN2 :1;
    IO_BYTE EN3 :1;
    IO_BYTE EN4 :1;
    IO_BYTE EN5 :1;
    IO_BYTE EN6 :1;
    IO_BYTE EN7 :1;
  }bit;
 }ENIRSTR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE ER0 :1;
    IO_BYTE ER1 :1;
    IO_BYTE ER2 :1;
    IO_BYTE ER3 :1;
    IO_BYTE ER4 :1;
    IO_BYTE ER5 :1;
    IO_BYTE ER6 :1;
    IO_BYTE ER7 :1;
  }bit;
 }EIRRSTR;
typedef union{  
    IO_WORD	word;
    struct{
    IO_WORD LA0 :1;
    IO_WORD LB0 :1;
    IO_WORD LA1 :1;
    IO_WORD LB1 :1;
    IO_WORD LA2 :1;
    IO_WORD LB2 :1;
    IO_WORD LA3 :1;
    IO_WORD LB3 :1;
    IO_WORD LA4 :1;
    IO_WORD LB4 :1;
    IO_WORD LA5 :1;
    IO_WORD LB5 :1;
    IO_WORD LA6 :1;
    IO_WORD LB6 :1;
    IO_WORD LA7 :1;
    IO_WORD LB7 :1;
  }bit;
 }ELVRSTR;
typedef union{   /* Puls Pattern Generator Channel 0,1 */
    IO_BYTE	byte;
    struct{
    IO_BYTE RESV :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE PUF0 :1;
    IO_BYTE PIE0 :1;
    IO_BYTE PE00 :1;
    IO_BYTE  :1;
    IO_BYTE PEN0 :1;
  }bit;
 }PPGC0STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE RESV :1;
    IO_BYTE MD0 :1;
    IO_BYTE MD1 :1;
    IO_BYTE PUF1 :1;
    IO_BYTE PIE1 :1;
    IO_BYTE PE10 :1;
    IO_BYTE  :1;
    IO_BYTE PEN1 :1;
  }bit;
  struct{
    IO_BYTE :1;
    IO_BYTE MD :2;
  }bitc;
 }PPGC1STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE PCM0 :1;
    IO_BYTE PCM1 :1;
    IO_BYTE PCM2 :1;
    IO_BYTE PCS0 :1;
    IO_BYTE PCS1 :1;
    IO_BYTE PCS2 :1;
  }bit;
  struct{
    IO_BYTE :1;
    IO_BYTE :1;
    IO_BYTE PCM :3;
    IO_BYTE PCS :3;
  }bitc;
 }PPG01STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE RESV :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE PUF2 :1;
    IO_BYTE PIE2 :1;
    IO_BYTE PE20 :1;
    IO_BYTE  :1;
    IO_BYTE PEN2 :1;
  }bit;
 }PPGC2STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE RESV :1;
    IO_BYTE MD0 :1;
    IO_BYTE MD1 :1;
    IO_BYTE PUF3 :1;
    IO_BYTE PIE3 :1;
    IO_BYTE PE30 :1;
    IO_BYTE  :1;
    IO_BYTE PEN3 :1;
  }bit;
  struct{
    IO_BYTE :1;
    IO_BYTE MD :2;
  }bitc;
 }PPGC3STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE PCM0 :1;
    IO_BYTE PCM1 :1;
    IO_BYTE PCM2 :1;
    IO_BYTE PCS0 :1;
    IO_BYTE PCS1 :1;
    IO_BYTE PCS2 :1;
  }bit;
  struct{
    IO_BYTE :1;
    IO_BYTE :1;
    IO_BYTE PCM :3;
    IO_BYTE PCS :3;
  }bitc;
 }PPG23STR;
typedef union{   /* ICU unit 0/1 */
    IO_WORD	word;
    struct{
    IO_WORD CP00 :1;
    IO_WORD CP01 :1;
    IO_WORD CP02 :1;
    IO_WORD CP03 :1;
    IO_WORD CP04 :1;
    IO_WORD CP05 :1;
    IO_WORD CP06 :1;
    IO_WORD CP07 :1;
    IO_WORD CP08 :1;
    IO_WORD CP09 :1;
    IO_WORD CP10 :1;
    IO_WORD CP11 :1;
    IO_WORD CP12 :1;
    IO_WORD CP13 :1;
    IO_WORD CP14 :1;
    IO_WORD CP15 :1;
  }bit;
 }IPCP0STR;
typedef union{  
    IO_WORD	word;
    struct{
    IO_WORD CP00 :1;
    IO_WORD CP01 :1;
    IO_WORD CP02 :1;
    IO_WORD CP03 :1;
    IO_WORD CP04 :1;
    IO_WORD CP05 :1;
    IO_WORD CP06 :1;
    IO_WORD CP07 :1;
    IO_WORD CP08 :1;
    IO_WORD CP09 :1;
    IO_WORD CP10 :1;
    IO_WORD CP11 :1;
    IO_WORD CP12 :1;
    IO_WORD CP13 :1;
    IO_WORD CP14 :1;
    IO_WORD CP15 :1;
  }bit;
 }IPCP1STR;
typedef union{   /* Input Capture 0/1/2/3 */
    IO_BYTE	byte;
    struct{
    IO_BYTE EG00 :1;
    IO_BYTE EG01 :1;
    IO_BYTE EG10 :1;
    IO_BYTE EG11 :1;
    IO_BYTE ICE0 :1;
    IO_BYTE ICE1 :1;
    IO_BYTE ICP0 :1;
    IO_BYTE ICP1 :1;
  }bit;
  struct{
    IO_BYTE EG0 :2;
    IO_BYTE EG1 :2;
  }bitc;
 }ICS01STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE EG20 :1;
    IO_BYTE EG21 :1;
    IO_BYTE EG30 :1;
    IO_BYTE EG31 :1;
    IO_BYTE ICE2 :1;
    IO_BYTE ICE3 :1;
    IO_BYTE ICP2 :1;
    IO_BYTE ICP3 :1;
  }bit;
  struct{
    IO_BYTE EG2 :2;
    IO_BYTE EG3 :2;
  }bitc;
 }ICS23STR;
typedef union{   /* I/O Timer */
    IO_WORD	word;
    struct{
    IO_WORD T00 :1;
    IO_WORD T01 :1;
    IO_WORD T02 :1;
    IO_WORD T03 :1;
    IO_WORD T04 :1;
    IO_WORD T05 :1;
    IO_WORD T06 :1;
    IO_WORD T07 :1;
    IO_WORD T08 :1;
    IO_WORD T09 :1;
    IO_WORD T10 :1;
    IO_WORD T11 :1;
    IO_WORD T12 :1;
    IO_WORD T13 :1;
    IO_WORD T14 :1;
    IO_WORD T15 :1;
  }bit;
 }TCDTSTR;
typedef union{  
    IO_WORD	word;
    struct{
    IO_WORD CLK0 :1;
    IO_WORD CLK1 :1;
    IO_WORD CLK2 :1;
    IO_WORD CLR :1;
    IO_WORD MODE :1;
    IO_WORD STOP :1;
    IO_WORD IVFE :1;
    IO_WORD IVF :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD ECKE :1;
  }bit;
  struct{
    IO_WORD CLK :3;
  }bitc;
 }TCCSSTR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE CLK0 :1;
    IO_BYTE CLK1 :1;
    IO_BYTE CLK2 :1;
    IO_BYTE CLR :1;
    IO_BYTE MODE :1;
    IO_BYTE STOP :1;
    IO_BYTE IVFE :1;
    IO_BYTE IVF :1;
  }bit;
  struct{
    IO_BYTE CLK :3;
  }bitc;
 }TCCSLSTR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE ECKE :1;
  }bit;
 }TCCSHSTR;
typedef union{   /* ICU unit 2/3 */
    IO_WORD	word;
    struct{
    IO_WORD CP00 :1;
    IO_WORD CP01 :1;
    IO_WORD CP02 :1;
    IO_WORD CP03 :1;
    IO_WORD CP04 :1;
    IO_WORD CP05 :1;
    IO_WORD CP06 :1;
    IO_WORD CP07 :1;
    IO_WORD CP08 :1;
    IO_WORD CP09 :1;
    IO_WORD CP10 :1;
    IO_WORD CP11 :1;
    IO_WORD CP12 :1;
    IO_WORD CP13 :1;
    IO_WORD CP14 :1;
    IO_WORD CP15 :1;
  }bit;
 }IPCP2STR;
typedef union{  
    IO_WORD	word;
    struct{
    IO_WORD CP00 :1;
    IO_WORD CP01 :1;
    IO_WORD CP02 :1;
    IO_WORD CP03 :1;
    IO_WORD CP04 :1;
    IO_WORD CP05 :1;
    IO_WORD CP06 :1;
    IO_WORD CP07 :1;
    IO_WORD CP08 :1;
    IO_WORD CP09 :1;
    IO_WORD CP10 :1;
    IO_WORD CP11 :1;
    IO_WORD CP12 :1;
    IO_WORD CP13 :1;
    IO_WORD CP14 :1;
    IO_WORD CP15 :1;
  }bit;
 }IPCP3STR;
typedef union{   /* Reload Timer 0/1 */
    IO_WORD	word;
    struct{
    IO_WORD TRG :1;
    IO_WORD CNTE :1;
    IO_WORD UF :1;
    IO_WORD INTE :1;
    IO_WORD RELD :1;
    IO_WORD OUTL :1;
    IO_WORD OUTE :1;
    IO_WORD MOD0 :1;
    IO_WORD MOD1 :1;
    IO_WORD MOD2 :1;
    IO_WORD CSL0 :1;
    IO_WORD CSL1 :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
  }bit;
 }TMCSR0STR;
typedef union{  
    IO_WORD	word;
    struct{
    IO_WORD TRG :1;
    IO_WORD CNTE :1;
    IO_WORD UF :1;
    IO_WORD INTE :1;
    IO_WORD RELD :1;
    IO_WORD OUTL :1;
    IO_WORD OUTE :1;
    IO_WORD MOD0 :1;
    IO_WORD MOD1 :1;
    IO_WORD MOD2 :1;
    IO_WORD CSL0 :1;
    IO_WORD CSL1 :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
  }bit;
 }TMCSR1STR;
typedef union{   /* Delayed Interrupt */
    IO_BYTE	byte;
    struct{
    IO_BYTE R0 :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
  }bit;
 }DIRRSTR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE CS0 :1;
    IO_BYTE CS1 :1;
    IO_BYTE MCS :1;
    IO_BYTE SCS :1;
    IO_BYTE WS0 :1;
    IO_BYTE WS1 :1;
    IO_BYTE MCM :1;
    IO_BYTE SCM :1;
  }bit;
  struct{
    IO_BYTE CS :2;
    IO_BYTE :1;
    IO_BYTE :1;
    IO_BYTE WS :2;
  }bitc;
 }CKSCRSTR;
typedef union{   /* Watch Timer */
    IO_BYTE	byte;
    struct{
    IO_BYTE WTC0 :1;
    IO_BYTE WTC1 :1;
    IO_BYTE WTC2 :1;
    IO_BYTE WTR :1;
    IO_BYTE WTOF :1;
    IO_BYTE WTIE :1;
    IO_BYTE SCE :1;
    IO_BYTE WDCS :1;
  }bit;
 }WTCSTR;
typedef union{   /* Flash Control Register */
    IO_BYTE	byte;
    struct{
    IO_BYTE LPM0 :1;
    IO_BYTE  :1;
    IO_BYTE LPM1 :1;
    IO_BYTE  :1;
    IO_BYTE RDY :1;
    IO_BYTE WE :1;
    IO_BYTE RDYINT :1;
    IO_BYTE INTE :1;
  }bit;
 }FMCSSTR;
typedef union{   /* Reload Timer 0/1 */
    IO_WORD	word;
    struct{
    IO_WORD D0 :1;
    IO_WORD D1 :1;
    IO_WORD D2 :1;
    IO_WORD D3 :1;
    IO_WORD D4 :1;
    IO_WORD D5 :1;
    IO_WORD D6 :1;
    IO_WORD D7 :1;
    IO_WORD D8 :1;
    IO_WORD D9 :1;
    IO_WORD D10 :1;
    IO_WORD D11 :1;
    IO_WORD D12 :1;
    IO_WORD D13 :1;
    IO_WORD D14 :1;
    IO_WORD D15 :1;
  }bit;
 }TMR0STR;
typedef union{  
    IO_WORD	word;
    struct{
    IO_WORD D0 :1;
    IO_WORD D1 :1;
    IO_WORD D2 :1;
    IO_WORD D3 :1;
    IO_WORD D4 :1;
    IO_WORD D5 :1;
    IO_WORD D6 :1;
    IO_WORD D7 :1;
    IO_WORD D8 :1;
    IO_WORD D9 :1;
    IO_WORD D10 :1;
    IO_WORD D11 :1;
    IO_WORD D12 :1;
    IO_WORD D13 :1;
    IO_WORD D14 :1;
    IO_WORD D15 :1;
  }bit;
 }TMRLR0STR;
typedef union{  
    IO_WORD	word;
    struct{
    IO_WORD D0 :1;
    IO_WORD D1 :1;
    IO_WORD D2 :1;
    IO_WORD D3 :1;
    IO_WORD D4 :1;
    IO_WORD D5 :1;
    IO_WORD D6 :1;
    IO_WORD D7 :1;
    IO_WORD D8 :1;
    IO_WORD D9 :1;
    IO_WORD D10 :1;
    IO_WORD D11 :1;
    IO_WORD D12 :1;
    IO_WORD D13 :1;
    IO_WORD D14 :1;
    IO_WORD D15 :1;
  }bit;
 }TMR1STR;
typedef union{  
    IO_WORD	word;
    struct{
    IO_WORD D0 :1;
    IO_WORD D1 :1;
    IO_WORD D2 :1;
    IO_WORD D3 :1;
    IO_WORD D4 :1;
    IO_WORD D5 :1;
    IO_WORD D6 :1;
    IO_WORD D7 :1;
    IO_WORD D8 :1;
    IO_WORD D9 :1;
    IO_WORD D10 :1;
    IO_WORD D11 :1;
    IO_WORD D12 :1;
    IO_WORD D13 :1;
    IO_WORD D14 :1;
    IO_WORD D15 :1;
  }bit;
 }TMRLR1STR;

/* C-DECLARATIONS */

__IO_EXTERN __io PDR0STR _pdr0;   /*  PORT DATA Registers */
#define PDR0 _pdr0.byte
#define PDR0_P00 _pdr0.bit.P00
#define PDR0_P01 _pdr0.bit.P01
#define PDR0_P02 _pdr0.bit.P02
#define PDR0_P03 _pdr0.bit.P03
#define PDR0_P04 _pdr0.bit.P04
#define PDR0_P05 _pdr0.bit.P05
#define PDR0_P06 _pdr0.bit.P06
#define PDR0_P07 _pdr0.bit.P07
__IO_EXTERN __io PDR1STR _pdr1;  
#define PDR1 _pdr1.byte
#define PDR1_P10 _pdr1.bit.P10
#define PDR1_P11 _pdr1.bit.P11
#define PDR1_P12 _pdr1.bit.P12
#define PDR1_P13 _pdr1.bit.P13
#define PDR1_P14 _pdr1.bit.P14
#define PDR1_P15 _pdr1.bit.P15
#define PDR1_P16 _pdr1.bit.P16
#define PDR1_P17 _pdr1.bit.P17
__IO_EXTERN __io PDR2STR _pdr2;  
#define PDR2 _pdr2.byte
#define PDR2_P20 _pdr2.bit.P20
#define PDR2_P21 _pdr2.bit.P21
#define PDR2_P22 _pdr2.bit.P22
#define PDR2_P23 _pdr2.bit.P23
#define PDR2_P24 _pdr2.bit.P24
#define PDR2_P25 _pdr2.bit.P25
#define PDR2_P26 _pdr2.bit.P26
#define PDR2_P27 _pdr2.bit.P27
__IO_EXTERN __io PDR3STR _pdr3;  
#define PDR3 _pdr3.byte
#define PDR3_P30 _pdr3.bit.P30
#define PDR3_P31 _pdr3.bit.P31
#define PDR3_P32 _pdr3.bit.P32
#define PDR3_P33 _pdr3.bit.P33
#define PDR3_P34 _pdr3.bit.P34
#define PDR3_P35 _pdr3.bit.P35
#define PDR3_P36 _pdr3.bit.P36
#define PDR3_P37 _pdr3.bit.P37
__IO_EXTERN __io PDR4STR _pdr4;  
#define PDR4 _pdr4.byte
#define PDR4_P40 _pdr4.bit.P40
#define PDR4_P41 _pdr4.bit.P41
#define PDR4_P42 _pdr4.bit.P42
#define PDR4_P43 _pdr4.bit.P43
#define PDR4_P44 _pdr4.bit.P44
__IO_EXTERN __io PDR5STR _pdr5;  
#define PDR5 _pdr5.byte
#define PDR5_P50 _pdr5.bit.P50
#define PDR5_P51 _pdr5.bit.P51
#define PDR5_P52 _pdr5.bit.P52
#define PDR5_P53 _pdr5.bit.P53
#define PDR5_P54 _pdr5.bit.P54
#define PDR5_P55 _pdr5.bit.P55
#define PDR5_P56 _pdr5.bit.P56
#define PDR5_P57 _pdr5.bit.P57
__IO_EXTERN __io PDR6STR _pdr6;  
#define PDR6 _pdr6.byte
#define PDR6_P60 _pdr6.bit.P60
#define PDR6_P61 _pdr6.bit.P61
#define PDR6_P62 _pdr6.bit.P62
#define PDR6_P63 _pdr6.bit.P63
__IO_EXTERN __io DDR0STR _ddr0;   /* PORT DIR */
#define DDR0 _ddr0.byte
#define DDR0_D00 _ddr0.bit.D00
#define DDR0_D01 _ddr0.bit.D01
#define DDR0_D02 _ddr0.bit.D02
#define DDR0_D03 _ddr0.bit.D03
#define DDR0_D04 _ddr0.bit.D04
#define DDR0_D05 _ddr0.bit.D05
#define DDR0_D06 _ddr0.bit.D06
#define DDR0_D07 _ddr0.bit.D07
__IO_EXTERN __io DDR1STR _ddr1;  
#define DDR1 _ddr1.byte
#define DDR1_D10 _ddr1.bit.D10
#define DDR1_D11 _ddr1.bit.D11
#define DDR1_D12 _ddr1.bit.D12
#define DDR1_D13 _ddr1.bit.D13
#define DDR1_D14 _ddr1.bit.D14
#define DDR1_D15 _ddr1.bit.D15
#define DDR1_D16 _ddr1.bit.D16
#define DDR1_D17 _ddr1.bit.D17
__IO_EXTERN __io DDR2STR _ddr2;  
#define DDR2 _ddr2.byte
#define DDR2_D20 _ddr2.bit.D20
#define DDR2_D21 _ddr2.bit.D21
#define DDR2_D22 _ddr2.bit.D22
#define DDR2_D23 _ddr2.bit.D23
#define DDR2_D24 _ddr2.bit.D24
#define DDR2_D25 _ddr2.bit.D25
#define DDR2_D26 _ddr2.bit.D26
#define DDR2_D27 _ddr2.bit.D27
__IO_EXTERN __io DDR3STR _ddr3;  
#define DDR3 _ddr3.byte
#define DDR3_D30 _ddr3.bit.D30
#define DDR3_D31 _ddr3.bit.D31
#define DDR3_D32 _ddr3.bit.D32
#define DDR3_D33 _ddr3.bit.D33
#define DDR3_D34 _ddr3.bit.D34
#define DDR3_D35 _ddr3.bit.D35
#define DDR3_D36 _ddr3.bit.D36
#define DDR3_D37 _ddr3.bit.D37
__IO_EXTERN __io DDR4STR _ddr4;  
#define DDR4 _ddr4.byte
#define DDR4_D40 _ddr4.bit.D40
#define DDR4_D41 _ddr4.bit.D41
#define DDR4_D42 _ddr4.bit.D42
#define DDR4_D43 _ddr4.bit.D43
#define DDR4_D44 _ddr4.bit.D44
__IO_EXTERN __io DDR5STR _ddr5;  
#define DDR5 _ddr5.byte
#define DDR5_D50 _ddr5.bit.D50
#define DDR5_D51 _ddr5.bit.D51
#define DDR5_D52 _ddr5.bit.D52
#define DDR5_D53 _ddr5.bit.D53
#define DDR5_D54 _ddr5.bit.D54
#define DDR5_D55 _ddr5.bit.D55
#define DDR5_D56 _ddr5.bit.D56
#define DDR5_D57 _ddr5.bit.D57
__IO_EXTERN __io DDR6STR _ddr6;  
#define DDR6 _ddr6.byte
#define DDR6_D60 _ddr6.bit.D60
#define DDR6_D61 _ddr6.bit.D61
#define DDR6_D62 _ddr6.bit.D62
#define DDR6_D63 _ddr6.bit.D63
__IO_EXTERN __io ADERSTR _ader;   /* AnalogInputEnable Port 5 */
#define ADER _ader.byte
#define ADER_ADE0 _ader.bit.ADE0
#define ADER_ADE1 _ader.bit.ADE1
#define ADER_ADE2 _ader.bit.ADE2
#define ADER_ADE3 _ader.bit.ADE3
#define ADER_ADE4 _ader.bit.ADE4
#define ADER_ADE5 _ader.bit.ADE5
#define ADER_ADE6 _ader.bit.ADE6
#define ADER_ADE7 _ader.bit.ADE7
__IO_EXTERN __io SMR0STR _smr0;   /* UART0 */
#define SMR0 _smr0.byte
#define SMR0_SOE _smr0.bit.SOE
#define SMR0_SCKE _smr0.bit.SCKE
#define SMR0_CS0 _smr0.bit.CS0
#define SMR0_CS1 _smr0.bit.CS1
#define SMR0_CS2 _smr0.bit.CS2
#define SMR0_MD0 _smr0.bit.MD0
#define SMR0_MD1 _smr0.bit.MD1
#define SMR0_CS _smr0.bitc.CS
#define SMR0_MD _smr0.bitc.MD
__IO_EXTERN __io SCR0STR _scr0;  
#define SCR0 _scr0.byte
#define SCR0_TXE _scr0.bit.TXE
#define SCR0_RXE _scr0.bit.RXE
#define SCR0_REC _scr0.bit.REC
#define SCR0_AD _scr0.bit.AD
#define SCR0_CL _scr0.bit.CL
#define SCR0_SBL _scr0.bit.SBL
#define SCR0_P _scr0.bit.P
#define SCR0_PEN _scr0.bit.PEN
__IO_EXTERN __io SIDR0STR _sidr0;  
#define SIDR0 _sidr0.byte
#define SIDR0_D0 _sidr0.bit.D0
#define SIDR0_D1 _sidr0.bit.D1
#define SIDR0_D2 _sidr0.bit.D2
#define SIDR0_D3 _sidr0.bit.D3
#define SIDR0_D4 _sidr0.bit.D4
#define SIDR0_D5 _sidr0.bit.D5
#define SIDR0_D6 _sidr0.bit.D6
#define SIDR0_D7 _sidr0.bit.D7
__IO_EXTERN __io SODR0STR _sodr0;  
#define SODR0 _sodr0.byte
#define SODR0_D0 _sodr0.bit.D0
#define SODR0_D1 _sodr0.bit.D1
#define SODR0_D2 _sodr0.bit.D2
#define SODR0_D3 _sodr0.bit.D3
#define SODR0_D4 _sodr0.bit.D4
#define SODR0_D5 _sodr0.bit.D5
#define SODR0_D6 _sodr0.bit.D6
#define SODR0_D7 _sodr0.bit.D7
__IO_EXTERN __io SSR0STR _ssr0;  
#define SSR0 _ssr0.byte
#define SSR0_TIE _ssr0.bit.TIE
#define SSR0_RIE _ssr0.bit.RIE
#define SSR0_TDRE _ssr0.bit.TDRE
#define SSR0_RDRF _ssr0.bit.RDRF
#define SSR0_FRE _ssr0.bit.FRE
#define SSR0_ORE _ssr0.bit.ORE
#define SSR0_PE _ssr0.bit.PE
__IO_EXTERN __io CDCR0STR _cdcr0;  
#define CDCR0 _cdcr0.byte
#define CDCR0_DIV0 _cdcr0.bit.DIV0
#define CDCR0_DIV1 _cdcr0.bit.DIV1
#define CDCR0_DIV2 _cdcr0.bit.DIV2
#define CDCR0_DIV3 _cdcr0.bit.DIV3
#define CDCR0_MD _cdcr0.bit.MD
#define CDCR0_DIV _cdcr0.bitc.DIV
__IO_EXTERN __io SES0STR _ses0;  
#define SES0 _ses0.byte
#define SES0_NEG _ses0.bit.NEG
__IO_EXTERN __io SMR1STR _smr1;   /* UART1 */
#define SMR1 _smr1.byte
#define SMR1_SOE _smr1.bit.SOE
#define SMR1_SCKE _smr1.bit.SCKE
#define SMR1_UPCL _smr1.bit.UPCL
#define SMR1_CS0 _smr1.bit.CS0
#define SMR1_CS1 _smr1.bit.CS1
#define SMR1_CS2 _smr1.bit.CS2
#define SMR1_MD0 _smr1.bit.MD0
#define SMR1_MD1 _smr1.bit.MD1
#define SMR1_CS _smr1.bitc.CS
#define SMR1_MD _smr1.bitc.MD
__IO_EXTERN __io SCR1STR _scr1;  
#define SCR1 _scr1.byte
#define SCR1_TXE _scr1.bit.TXE
#define SCR1_RXE _scr1.bit.RXE
#define SCR1_REC _scr1.bit.REC
#define SCR1_AD _scr1.bit.AD
#define SCR1_CL _scr1.bit.CL
#define SCR1_SBL _scr1.bit.SBL
#define SCR1_P _scr1.bit.P
#define SCR1_PEN _scr1.bit.PEN
__IO_EXTERN __io SIDR1STR _sidr1;  
#define SIDR1 _sidr1.byte
#define SIDR1_D0 _sidr1.bit.D0
#define SIDR1_D1 _sidr1.bit.D1
#define SIDR1_D2 _sidr1.bit.D2
#define SIDR1_D3 _sidr1.bit.D3
#define SIDR1_D4 _sidr1.bit.D4
#define SIDR1_D5 _sidr1.bit.D5
#define SIDR1_D6 _sidr1.bit.D6
#define SIDR1_D7 _sidr1.bit.D7
__IO_EXTERN __io SODR1STR _sodr1;  
#define SODR1 _sodr1.byte
#define SODR1_D0 _sodr1.bit.D0
#define SODR1_D1 _sodr1.bit.D1
#define SODR1_D2 _sodr1.bit.D2
#define SODR1_D3 _sodr1.bit.D3
#define SODR1_D4 _sodr1.bit.D4
#define SODR1_D5 _sodr1.bit.D5
#define SODR1_D6 _sodr1.bit.D6
#define SODR1_D7 _sodr1.bit.D7
__IO_EXTERN __io SSR1STR _ssr1;  
#define SSR1 _ssr1.byte
#define SSR1_TIE _ssr1.bit.TIE
#define SSR1_RIE _ssr1.bit.RIE
#define SSR1_BDS _ssr1.bit.BDS
#define SSR1_TDRE _ssr1.bit.TDRE
#define SSR1_RDRF _ssr1.bit.RDRF
#define SSR1_FRE _ssr1.bit.FRE
#define SSR1_ORE _ssr1.bit.ORE
#define SSR1_PE _ssr1.bit.PE
__IO_EXTERN __io CDCR1STR _cdcr1;  
#define CDCR1 _cdcr1.byte
#define CDCR1_DIV0 _cdcr1.bit.DIV0
#define CDCR1_DIV1 _cdcr1.bit.DIV1
#define CDCR1_DIV2 _cdcr1.bit.DIV2
#define CDCR1_DIV3 _cdcr1.bit.DIV3
#define CDCR1_MD _cdcr1.bit.MD
#define CDCR1_DIV _cdcr1.bitc.DIV
__IO_EXTERN __io ENIRSTR _enir;   /* DTP, External Interrupts */
#define ENIR _enir.byte
#define ENIR_EN0 _enir.bit.EN0
#define ENIR_EN1 _enir.bit.EN1
#define ENIR_EN2 _enir.bit.EN2
#define ENIR_EN3 _enir.bit.EN3
#define ENIR_EN4 _enir.bit.EN4
#define ENIR_EN5 _enir.bit.EN5
#define ENIR_EN6 _enir.bit.EN6
#define ENIR_EN7 _enir.bit.EN7
__IO_EXTERN __io EIRRSTR _eirr;  
#define EIRR _eirr.byte
#define EIRR_ER0 _eirr.bit.ER0
#define EIRR_ER1 _eirr.bit.ER1
#define EIRR_ER2 _eirr.bit.ER2
#define EIRR_ER3 _eirr.bit.ER3
#define EIRR_ER4 _eirr.bit.ER4
#define EIRR_ER5 _eirr.bit.ER5
#define EIRR_ER6 _eirr.bit.ER6
#define EIRR_ER7 _eirr.bit.ER7
__IO_EXTERN __io ELVRSTR _elvr;  
#define ELVR _elvr.word
#define ELVR_LA0 _elvr.bit.LA0
#define ELVR_LB0 _elvr.bit.LB0
#define ELVR_LA1 _elvr.bit.LA1
#define ELVR_LB1 _elvr.bit.LB1
#define ELVR_LA2 _elvr.bit.LA2
#define ELVR_LB2 _elvr.bit.LB2
#define ELVR_LA3 _elvr.bit.LA3
#define ELVR_LB3 _elvr.bit.LB3
#define ELVR_LA4 _elvr.bit.LA4
#define ELVR_LB4 _elvr.bit.LB4
#define ELVR_LA5 _elvr.bit.LA5
#define ELVR_LB5 _elvr.bit.LB5
#define ELVR_LA6 _elvr.bit.LA6
#define ELVR_LB6 _elvr.bit.LB6
#define ELVR_LA7 _elvr.bit.LA7
#define ELVR_LB7 _elvr.bit.LB7
/*******************************************************************
 *
 *    DESCRIPTION:  Interrupt Control Register Declaration
 *
 *    AUTHOR:       Fujitsu Mikroelektronik GmbH
 *
 *    HISTORY:    
 * Version 1.0      03.07.00   : original version
 *         1.1      01.11.00   : Structure alignment
 * Version 1.2      22.07.2002 : HW  ADCS0, ADCR-Bitdefinitions as const, no RMV allowed
 *
 *******************************************************************/

#ifndef __ADC_H
#define __ADC_H

typedef union{   /*  */
    IO_WORD	word;
    struct{
    const IO_WORD ANE0 :1;
    const IO_WORD ANE1 :1;
    const IO_WORD ANE2 :1;
    const IO_WORD ANS0 :1;
    const IO_WORD ANS1 :1;
    const IO_WORD ANS2 :1;
    const IO_WORD MD0  :1;
    const IO_WORD MD1  :1;
    const IO_WORD RESV :1; 
    const IO_WORD STRT :1;
    const IO_WORD STS0 :1;
    const IO_WORD STS1 :1;
    const IO_WORD PAUS :1;
    const IO_WORD INTE :1;
    const IO_WORD INT  :1;
    const IO_WORD BUSY :1;
   }bit;
  struct{
    const IO_WORD ANE :3;
    const IO_WORD ANS :3;
    const IO_WORD MD :2;
    const IO_WORD :1;
    const IO_WORD :1;
    const IO_WORD STS :2;
  }bitc;
 }ADCSSTR;

typedef union{  
    IO_BYTE	byte;
    struct{
    const IO_BYTE ANE0 :1;
    const IO_BYTE ANE1 :1;
    const IO_BYTE ANE2 :1;
    const IO_BYTE ANS0 :1;
    const IO_BYTE ANS1 :1;
    const IO_BYTE ANS2 :1;
    const IO_BYTE MD0 :1;
    const IO_BYTE MD1 :1;
  }bit;
  struct{
    const IO_BYTE ANE :3;
    const IO_BYTE ANS :3;
    const IO_BYTE MD :2;
  }bitc;
 }ADCS0STR;

typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE RESV :1;
    IO_BYTE STRT :1;
    IO_BYTE STS0 :1;
    IO_BYTE STS1 :1;
    IO_BYTE PAUS :1;
    IO_BYTE INTE :1;
    IO_BYTE INT :1;
    IO_BYTE BUSY :1;
  }bit;
  struct{
    IO_BYTE :1;
    IO_BYTE :1;
    IO_BYTE STS :2;
  }bitc;
 }ADCS1STR;

typedef union{  
    IO_WORD	word;
    struct{
    const IO_WORD D0 :1;
    const IO_WORD D1 :1;
    const IO_WORD D2 :1;
    const IO_WORD D3 :1;
    const IO_WORD D4 :1;
    const IO_WORD D5 :1;
    const IO_WORD D6 :1;
    const IO_WORD D7 :1;
    const IO_WORD D8 :1;
    const IO_WORD D9 :1;
    const IO_WORD  :1;
    const IO_WORD CT0 :1;
    const IO_WORD CT1 :1;
    const IO_WORD ST0 :1;
    const IO_WORD ST1 :1;
    const IO_WORD S10 :1;
  }bit;
  struct{
    const IO_WORD :1;
    const IO_WORD :1;
    const IO_WORD :1;
    const IO_WORD :1;
    const IO_WORD :1;
    const IO_WORD :1;
    const IO_WORD :1;
    const IO_WORD :1;
    const IO_WORD :1;
    const IO_WORD :1;
    const IO_WORD :1;
    const IO_WORD CT :2;
    const IO_WORD ST :2;
  }bitc;
 }ADCRSTR;

typedef union{  
    const IO_BYTE	byte;
    struct{
    const IO_BYTE D0 :1;
    const IO_BYTE D1 :1;
    const IO_BYTE D2 :1;
    const IO_BYTE D3 :1;
    const IO_BYTE D4 :1;
    const IO_BYTE D5 :1;
    const IO_BYTE D6 :1;
    const IO_BYTE D7 :1;
  }bit;
 }ADCR0STR;

typedef union{  
    IO_BYTE	byte;
    struct{
    const IO_BYTE D8 :1;
    const IO_BYTE D9 :1;
    const IO_BYTE  :1;
    const IO_BYTE CT0 :1;
    const IO_BYTE CT1 :1;
    const IO_BYTE ST0 :1;
    const IO_BYTE ST1 :1;
    const IO_BYTE S10 :1;
  }bit;
  struct{
    const IO_BYTE :1;
    const IO_BYTE :1;
    const IO_BYTE :1;
    const IO_BYTE CT :2;
    const IO_BYTE ST :2;
  }bitc;
 }ADCR1STR;

__IO_EXTERN __io ADCSSTR _adcs;
#define ADCS _adcs.word
#define ADCS_ANE0 _adcs.bit.ANE0
#define ADCS_ANE1 _adcs.bit.ANE1
#define ADCS_ANE2 _adcs.bit.ANE2
#define ADCS_ANS0 _adcs.bit.ANS0
#define ADCS_ANS1 _adcs.bit.ANS1
#define ADCS_ANS2 _adcs.bit.ANS2
#define ADCS_MD0 _adcs.bit.MD0
#define ADCS_MD1 _adcs.bit.MD1
#define ADCS_STRT _adcs.bit.STRT
#define ADCS_STS0 _adcs.bit.STS0
#define ADCS_STS1 _adcs.bit.STS1
#define ADCS_PAUS _adcs.bit.PAUS
#define ADCS_INTE _adcs.bit.INTE
#define ADCS_INT _adcs.bit.INT
#define ADCS_BUSY _adcs.bit.BUSY
#define ADCS_ANE _adcs.bitc.ANE
#define ADCS_ANS _adcs.bitc.ANS
#define ADCS_MD _adcs.bitc.MD
#define ADCS_STS _adcs.bitc.STS
__IO_EXTERN __io ADCS0STR _adcs0;  
#define ADCS0 _adcs0.byte
#define ADCS0_ANE0 _adcs0.bit.ANE0
#define ADCS0_ANE1 _adcs0.bit.ANE1
#define ADCS0_ANE2 _adcs0.bit.ANE2
#define ADCS0_ANS0 _adcs0.bit.ANS0
#define ADCS0_ANS1 _adcs0.bit.ANS1
#define ADCS0_ANS2 _adcs0.bit.ANS2
#define ADCS0_MD0 _adcs0.bit.MD0
#define ADCS0_MD1 _adcs0.bit.MD1
#define ADCS0_ANE _adcs0.bitc.ANE
#define ADCS0_ANS _adcs0.bitc.ANS
#define ADCS0_MD _adcs0.bitc.MD
__IO_EXTERN __io ADCS1STR _adcs1;  
#define ADCS1 _adcs1.byte
#define ADCS1_STRT _adcs1.bit.STRT
#define ADCS1_STS0 _adcs1.bit.STS0
#define ADCS1_STS1 _adcs1.bit.STS1
#define ADCS1_PAUS _adcs1.bit.PAUS
#define ADCS1_INTE _adcs1.bit.INTE
#define ADCS1_INT _adcs1.bit.INT
#define ADCS1_BUSY _adcs1.bit.BUSY
#define ADCS1_STS _adcs1.bitc.STS
__IO_EXTERN __io ADCRSTR _adcr;  
#define ADCR _adcr.word
#define ADCR_D0 _adcr.bit.D0
#define ADCR_D1 _adcr.bit.D1
#define ADCR_D2 _adcr.bit.D2
#define ADCR_D3 _adcr.bit.D3
#define ADCR_D4 _adcr.bit.D4
#define ADCR_D5 _adcr.bit.D5
#define ADCR_D6 _adcr.bit.D6
#define ADCR_D7 _adcr.bit.D7
#define ADCR_D8 _adcr.bit.D8
#define ADCR_D9 _adcr.bit.D9
#define ADCR_CT0 _adcr.bit.CT0
#define ADCR_CT1 _adcr.bit.CT1
#define ADCR_ST0 _adcr.bit.ST0
#define ADCR_ST1 _adcr.bit.ST1
#define ADCR_S10 _adcr.bit.S10
#define ADCR_CT _adcr.bitc.CT
#define ADCR_ST _adcr.bitc.ST
__IO_EXTERN __io ADCR0STR _adcr0;  
#define ADCR0 _adcr0.byte
#define ADCR0_D0 _adcr0.bit.D0
#define ADCR0_D1 _adcr0.bit.D1
#define ADCR0_D2 _adcr0.bit.D2
#define ADCR0_D3 _adcr0.bit.D3
#define ADCR0_D4 _adcr0.bit.D4
#define ADCR0_D5 _adcr0.bit.D5
#define ADCR0_D6 _adcr0.bit.D6
#define ADCR0_D7 _adcr0.bit.D7
__IO_EXTERN __io ADCR1STR _adcr1;  
#define ADCR1 _adcr1.byte
#define ADCR1_D8 _adcr1.bit.D8
#define ADCR1_D9 _adcr1.bit.D9
#define ADCR1_CT0 _adcr1.bit.CT0
#define ADCR1_CT1 _adcr1.bit.CT1
#define ADCR1_ST0 _adcr1.bit.ST0
#define ADCR1_ST1 _adcr1.bit.ST1
#define ADCR1_S10 _adcr1.bit.S10
#define ADCR1_CT _adcr1.bitc.CT
#define ADCR1_ST _adcr1.bitc.ST

#endif   /* __ADC_H  */
__IO_EXTERN __io PPGC0STR _ppgc0;   /* Puls Pattern Generator Channel 0,1 */
#define PPGC0 _ppgc0.byte
#define PPGC0_RESV _ppgc0.bit.RESV
#define PPGC0_PUF0 _ppgc0.bit.PUF0
#define PPGC0_PIE0 _ppgc0.bit.PIE0
#define PPGC0_PE00 _ppgc0.bit.PE00
#define PPGC0_PEN0 _ppgc0.bit.PEN0
__IO_EXTERN __io PPGC1STR _ppgc1;  
#define PPGC1 _ppgc1.byte
#define PPGC1_RESV _ppgc1.bit.RESV
#define PPGC1_MD0 _ppgc1.bit.MD0
#define PPGC1_MD1 _ppgc1.bit.MD1
#define PPGC1_PUF1 _ppgc1.bit.PUF1
#define PPGC1_PIE1 _ppgc1.bit.PIE1
#define PPGC1_PE10 _ppgc1.bit.PE10
#define PPGC1_PEN1 _ppgc1.bit.PEN1
#define PPGC1_MD _ppgc1.bitc.MD
__IO_EXTERN __io PPG01STR _ppg01;  
#define PPG01 _ppg01.byte
#define PPG01_PCM0 _ppg01.bit.PCM0
#define PPG01_PCM1 _ppg01.bit.PCM1
#define PPG01_PCM2 _ppg01.bit.PCM2
#define PPG01_PCS0 _ppg01.bit.PCS0
#define PPG01_PCS1 _ppg01.bit.PCS1
#define PPG01_PCS2 _ppg01.bit.PCS2
#define PPG01_PCM _ppg01.bitc.PCM
#define PPG01_PCS _ppg01.bitc.PCS
__IO_EXTERN __io PPGC2STR _ppgc2;  
#define PPGC2 _ppgc2.byte
#define PPGC2_RESV _ppgc2.bit.RESV
#define PPGC2_PUF2 _ppgc2.bit.PUF2
#define PPGC2_PIE2 _ppgc2.bit.PIE2
#define PPGC2_PE20 _ppgc2.bit.PE20
#define PPGC2_PEN2 _ppgc2.bit.PEN2
__IO_EXTERN __io PPGC3STR _ppgc3;  
#define PPGC3 _ppgc3.byte
#define PPGC3_RESV _ppgc3.bit.RESV
#define PPGC3_MD0 _ppgc3.bit.MD0
#define PPGC3_MD1 _ppgc3.bit.MD1
#define PPGC3_PUF3 _ppgc3.bit.PUF3
#define PPGC3_PIE3 _ppgc3.bit.PIE3
#define PPGC3_PE30 _ppgc3.bit.PE30
#define PPGC3_PEN3 _ppgc3.bit.PEN3
#define PPGC3_MD _ppgc3.bitc.MD
__IO_EXTERN __io PPG23STR _ppg23;  
#define PPG23 _ppg23.byte
#define PPG23_PCM0 _ppg23.bit.PCM0
#define PPG23_PCM1 _ppg23.bit.PCM1
#define PPG23_PCM2 _ppg23.bit.PCM2
#define PPG23_PCS0 _ppg23.bit.PCS0
#define PPG23_PCS1 _ppg23.bit.PCS1
#define PPG23_PCS2 _ppg23.bit.PCS2
#define PPG23_PCM _ppg23.bitc.PCM
#define PPG23_PCS _ppg23.bitc.PCS
__IO_EXTERN __io IPCP0STR _ipcp0;   /* ICU unit 0/1 */
#define IPCP0 _ipcp0.word
#define IPCP0_CP00 _ipcp0.bit.CP00
#define IPCP0_CP01 _ipcp0.bit.CP01
#define IPCP0_CP02 _ipcp0.bit.CP02
#define IPCP0_CP03 _ipcp0.bit.CP03
#define IPCP0_CP04 _ipcp0.bit.CP04
#define IPCP0_CP05 _ipcp0.bit.CP05
#define IPCP0_CP06 _ipcp0.bit.CP06
#define IPCP0_CP07 _ipcp0.bit.CP07
#define IPCP0_CP08 _ipcp0.bit.CP08
#define IPCP0_CP09 _ipcp0.bit.CP09
#define IPCP0_CP10 _ipcp0.bit.CP10
#define IPCP0_CP11 _ipcp0.bit.CP11
#define IPCP0_CP12 _ipcp0.bit.CP12
#define IPCP0_CP13 _ipcp0.bit.CP13
#define IPCP0_CP14 _ipcp0.bit.CP14
#define IPCP0_CP15 _ipcp0.bit.CP15
__IO_EXTERN __io IPCP1STR _ipcp1;  
#define IPCP1 _ipcp1.word
#define IPCP1_CP00 _ipcp1.bit.CP00
#define IPCP1_CP01 _ipcp1.bit.CP01
#define IPCP1_CP02 _ipcp1.bit.CP02
#define IPCP1_CP03 _ipcp1.bit.CP03
#define IPCP1_CP04 _ipcp1.bit.CP04
#define IPCP1_CP05 _ipcp1.bit.CP05
#define IPCP1_CP06 _ipcp1.bit.CP06
#define IPCP1_CP07 _ipcp1.bit.CP07
#define IPCP1_CP08 _ipcp1.bit.CP08
#define IPCP1_CP09 _ipcp1.bit.CP09
#define IPCP1_CP10 _ipcp1.bit.CP10
#define IPCP1_CP11 _ipcp1.bit.CP11
#define IPCP1_CP12 _ipcp1.bit.CP12
#define IPCP1_CP13 _ipcp1.bit.CP13
#define IPCP1_CP14 _ipcp1.bit.CP14
#define IPCP1_CP15 _ipcp1.bit.CP15
__IO_EXTERN __io ICS01STR _ics01;   /* Input Capture 0/1/2/3 */
#define ICS01 _ics01.byte
#define ICS01_EG00 _ics01.bit.EG00
#define ICS01_EG01 _ics01.bit.EG01
#define ICS01_EG10 _ics01.bit.EG10
#define ICS01_EG11 _ics01.bit.EG11
#define ICS01_ICE0 _ics01.bit.ICE0
#define ICS01_ICE1 _ics01.bit.ICE1
#define ICS01_ICP0 _ics01.bit.ICP0
#define ICS01_ICP1 _ics01.bit.ICP1
#define ICS01_EG0 _ics01.bitc.EG0
#define ICS01_EG1 _ics01.bitc.EG1
__IO_EXTERN __io ICS23STR _ics23;  
#define ICS23 _ics23.byte
#define ICS23_EG20 _ics23.bit.EG20
#define ICS23_EG21 _ics23.bit.EG21
#define ICS23_EG30 _ics23.bit.EG30
#define ICS23_EG31 _ics23.bit.EG31
#define ICS23_ICE2 _ics23.bit.ICE2
#define ICS23_ICE3 _ics23.bit.ICE3
#define ICS23_ICP2 _ics23.bit.ICP2
#define ICS23_ICP3 _ics23.bit.ICP3
#define ICS23_EG2 _ics23.bitc.EG2
#define ICS23_EG3 _ics23.bitc.EG3
__IO_EXTERN __io TCDTSTR _tcdt;   /* I/O Timer */
#define TCDT _tcdt.word
#define TCDT_T00 _tcdt.bit.T00
#define TCDT_T01 _tcdt.bit.T01
#define TCDT_T02 _tcdt.bit.T02
#define TCDT_T03 _tcdt.bit.T03
#define TCDT_T04 _tcdt.bit.T04
#define TCDT_T05 _tcdt.bit.T05
#define TCDT_T06 _tcdt.bit.T06
#define TCDT_T07 _tcdt.bit.T07
#define TCDT_T08 _tcdt.bit.T08
#define TCDT_T09 _tcdt.bit.T09
#define TCDT_T10 _tcdt.bit.T10
#define TCDT_T11 _tcdt.bit.T11
#define TCDT_T12 _tcdt.bit.T12
#define TCDT_T13 _tcdt.bit.T13
#define TCDT_T14 _tcdt.bit.T14
#define TCDT_T15 _tcdt.bit.T15
__IO_EXTERN __io IO_BYTE _tcdtl;
#define TCDTL _tcdtl   
__IO_EXTERN __io IO_BYTE _tcdth;
#define TCDTH _tcdth   
__IO_EXTERN __io TCCSSTR _tccs;  
#define TCCS _tccs.word
#define TCCS_CLK0 _tccs.bit.CLK0
#define TCCS_CLK1 _tccs.bit.CLK1
#define TCCS_CLK2 _tccs.bit.CLK2
#define TCCS_CLR _tccs.bit.CLR
#define TCCS_MODE _tccs.bit.MODE
#define TCCS_STOP _tccs.bit.STOP
#define TCCS_IVFE _tccs.bit.IVFE
#define TCCS_IVF _tccs.bit.IVF
#define TCCS_ECKE _tccs.bit.ECKE
#define TCCS_CLK _tccs.bitc.CLK
__IO_EXTERN __io TCCSLSTR _tccsl;  
#define TCCSL _tccsl.byte
#define TCCSL_CLK0 _tccsl.bit.CLK0
#define TCCSL_CLK1 _tccsl.bit.CLK1
#define TCCSL_CLK2 _tccsl.bit.CLK2
#define TCCSL_CLR _tccsl.bit.CLR
#define TCCSL_MODE _tccsl.bit.MODE
#define TCCSL_STOP _tccsl.bit.STOP
#define TCCSL_IVFE _tccsl.bit.IVFE
#define TCCSL_IVF _tccsl.bit.IVF
#define TCCSL_CLK _tccsl.bitc.CLK
__IO_EXTERN __io TCCSHSTR _tccsh;  
#define TCCSH _tccsh.byte
#define TCCSH_ECKE _tccsh.bit.ECKE
__IO_EXTERN __io IPCP2STR _ipcp2;   /* ICU unit 2/3 */
#define IPCP2 _ipcp2.word
#define IPCP2_CP00 _ipcp2.bit.CP00
#define IPCP2_CP01 _ipcp2.bit.CP01
#define IPCP2_CP02 _ipcp2.bit.CP02
#define IPCP2_CP03 _ipcp2.bit.CP03
#define IPCP2_CP04 _ipcp2.bit.CP04
#define IPCP2_CP05 _ipcp2.bit.CP05
#define IPCP2_CP06 _ipcp2.bit.CP06
#define IPCP2_CP07 _ipcp2.bit.CP07
#define IPCP2_CP08 _ipcp2.bit.CP08
#define IPCP2_CP09 _ipcp2.bit.CP09
#define IPCP2_CP10 _ipcp2.bit.CP10
#define IPCP2_CP11 _ipcp2.bit.CP11
#define IPCP2_CP12 _ipcp2.bit.CP12
#define IPCP2_CP13 _ipcp2.bit.CP13
#define IPCP2_CP14 _ipcp2.bit.CP14
#define IPCP2_CP15 _ipcp2.bit.CP15
__IO_EXTERN __io IPCP3STR _ipcp3;  
#define IPCP3 _ipcp3.word
#define IPCP3_CP00 _ipcp3.bit.CP00
#define IPCP3_CP01 _ipcp3.bit.CP01
#define IPCP3_CP02 _ipcp3.bit.CP02
#define IPCP3_CP03 _ipcp3.bit.CP03
#define IPCP3_CP04 _ipcp3.bit.CP04
#define IPCP3_CP05 _ipcp3.bit.CP05
#define IPCP3_CP06 _ipcp3.bit.CP06
#define IPCP3_CP07 _ipcp3.bit.CP07
#define IPCP3_CP08 _ipcp3.bit.CP08
#define IPCP3_CP09 _ipcp3.bit.CP09
#define IPCP3_CP10 _ipcp3.bit.CP10
#define IPCP3_CP11 _ipcp3.bit.CP11
#define IPCP3_CP12 _ipcp3.bit.CP12
#define IPCP3_CP13 _ipcp3.bit.CP13
#define IPCP3_CP14 _ipcp3.bit.CP14
#define IPCP3_CP15 _ipcp3.bit.CP15
__IO_EXTERN __io TMCSR0STR _tmcsr0;   /* Reload Timer 0/1 */
#define TMCSR0 _tmcsr0.word
#define TMCSR0_TRG _tmcsr0.bit.TRG
#define TMCSR0_CNTE _tmcsr0.bit.CNTE
#define TMCSR0_UF _tmcsr0.bit.UF
#define TMCSR0_INTE _tmcsr0.bit.INTE
#define TMCSR0_RELD _tmcsr0.bit.RELD
#define TMCSR0_OUTL _tmcsr0.bit.OUTL
#define TMCSR0_OUTE _tmcsr0.bit.OUTE
#define TMCSR0_MOD0 _tmcsr0.bit.MOD0
#define TMCSR0_MOD1 _tmcsr0.bit.MOD1
#define TMCSR0_MOD2 _tmcsr0.bit.MOD2
#define TMCSR0_CSL0 _tmcsr0.bit.CSL0
#define TMCSR0_CSL1 _tmcsr0.bit.CSL1
__IO_EXTERN __io TMCSR1STR _tmcsr1;  
#define TMCSR1 _tmcsr1.word
#define TMCSR1_TRG _tmcsr1.bit.TRG
#define TMCSR1_CNTE _tmcsr1.bit.CNTE
#define TMCSR1_UF _tmcsr1.bit.UF
#define TMCSR1_INTE _tmcsr1.bit.INTE
#define TMCSR1_RELD _tmcsr1.bit.RELD
#define TMCSR1_OUTL _tmcsr1.bit.OUTL
#define TMCSR1_OUTE _tmcsr1.bit.OUTE
#define TMCSR1_MOD0 _tmcsr1.bit.MOD0
#define TMCSR1_MOD1 _tmcsr1.bit.MOD1
#define TMCSR1_MOD2 _tmcsr1.bit.MOD2
#define TMCSR1_CSL0 _tmcsr1.bit.CSL0
#define TMCSR1_CSL1 _tmcsr1.bit.CSL1
__IO_EXTERN __io IO_BYTE _romm;
#define ROMM _romm    /* ROM mirror function */
/*
 * CANIO: control structures of CAN for LX-controllers
 *        This version is limited to 8 message bufers

 * Version: 1.0            08.02.00     HL
 *      - original version 
 *      - only 8 message buffers
 *      - reduced registers changed to byte access
 * Version: 1.1            09.02.00     HL
 *      - RC7 was missing
 * Version: 1.1            10.08.00     HL
 *      - _reserved members enlarged to original register size
 *        because alignment was not correct for AMR0 and AMR1
 */

#ifndef __CANSTR
#define __CANSTR

typedef struct
{
  union{   
    IO_BYTE byte;
    IO_WORD _reserved;
    struct{
      IO_WORD BVAL0 :1;
      IO_WORD BVAL1 :1;
      IO_WORD BVAL2 :1;
      IO_WORD BVAL3 :1;
      IO_WORD BVAL4 :1;
      IO_WORD BVAL5 :1;
      IO_WORD BVAL6 :1;
      IO_WORD BVAL7 :1;
    } bit;
  } bvalr;
  union{  
    IO_BYTE byte;
    IO_WORD _reserved;
    struct{
      IO_WORD TREQ0 :1;
      IO_WORD TREQ1 :1;
      IO_WORD TREQ2 :1;
      IO_WORD TREQ3 :1;
      IO_WORD TREQ4 :1;
      IO_WORD TREQ5 :1;
      IO_WORD TREQ6 :1;
      IO_WORD TREQ7 :1;
    } bit;
  } treqr;
  union{  
    IO_BYTE byte;
    IO_WORD _reserved;
    struct{
      IO_WORD TCAN0 :1;
      IO_WORD TCAN1 :1;
      IO_WORD TCAN2 :1;
      IO_WORD TCAN3 :1;
      IO_WORD TCAN4 :1;
      IO_WORD TCAN5 :1;
      IO_WORD TCAN6 :1;
      IO_WORD TCAN7 :1;
    } bit;
  } tcanr;
  union{  
    IO_BYTE byte;
    IO_WORD _reserved;
    struct{
      IO_WORD TC0 :1;
      IO_WORD TC1 :1;
      IO_WORD TC2 :1;
      IO_WORD TC3 :1;
      IO_WORD TC4 :1;
      IO_WORD TC5 :1;
      IO_WORD TC6 :1;
      IO_WORD TC7 :1;
    } bit;
 } tcr;
  union{  
    IO_BYTE byte;
    IO_WORD _reserved;
    struct{
      IO_WORD RC0 :1;
      IO_WORD RC1 :1;
      IO_WORD RC2 :1;
      IO_WORD RC3 :1;
      IO_WORD RC4 :1;
      IO_WORD RC5 :1;
      IO_WORD RC6 :1;
      IO_WORD RC7 :1;
    } bit;
  } rcr;
  union{  
    IO_BYTE byte;
    IO_WORD _reserved;
    struct{
      IO_WORD RRTR0 :1;
      IO_WORD RRTR1 :1;
      IO_WORD RRTR2 :1;
      IO_WORD RRTR3 :1;
      IO_WORD RRTR4 :1;
      IO_WORD RRTR5 :1;
      IO_WORD RRTR6 :1;
      IO_WORD RRTR7 :1;
    } bit;
  } rrtrr;
  union{  
    IO_BYTE byte;
    IO_WORD _reserved;
    struct{
      IO_WORD ROVR0 :1;
      IO_WORD ROVR1 :1;
      IO_WORD ROVR2 :1;
      IO_WORD ROVR3 :1;
      IO_WORD ROVR4 :1;
      IO_WORD ROVR5 :1;
      IO_WORD ROVR6 :1;
      IO_WORD ROVR7 :1;
    } bit;
  } rovrr;
  union{  
    IO_BYTE byte;
    IO_WORD _reserved;
    struct{
      IO_WORD RIE0 :1;
      IO_WORD RIE1 :1;
      IO_WORD RIE2 :1;
      IO_WORD RIE3 :1;
      IO_WORD RIE4 :1;
      IO_WORD RIE5 :1;
      IO_WORD RIE6 :1;
      IO_WORD RIE7 :1;
    } bit;
  } rier;
} CANLSTR;

typedef struct
{
  union{  
    IO_WORD word;
      struct{
      IO_WORD HALT :1;
      IO_WORD      :1;
      IO_WORD NIE  :1;
      IO_WORD      :1;
      IO_WORD      :1;
      IO_WORD      :1;
      IO_WORD  	:1;
      IO_WORD TOE 	:1;
      IO_WORD NS0 	:1;
      IO_WORD NS1 	:1;
      IO_WORD NT 	:1;
      IO_WORD  	:1;
      IO_WORD  	:1;
      IO_WORD  	:1;
      IO_WORD RS 	:1;
      IO_WORD TS 	:1;
    }bit;
    struct{
      IO_WORD 		:1;
      IO_WORD 		:1;
      IO_WORD 		:1;
      IO_WORD 		:1;
      IO_WORD 		:1;
      IO_WORD 		:1;
      IO_WORD 		:1;
      IO_WORD 		:1;
      IO_WORD NS 	:2;
    }bitc;
  }csr;
  union{  
    IO_WORD word;
    IO_WORD byte;
    struct{
      IO_WORD MBP0 :1;
      IO_WORD MBP1 :1;
      IO_WORD MBP2 :1;
      IO_WORD MBP3 :1;
      IO_WORD      :1;
      IO_WORD RCE  :1;
      IO_WORD TCE  :1;
      IO_WORD NTE  :1;
    } bit;
    struct{
      IO_WORD MBP  :4;
    } bitc;
  } leir;
  union{  
    IO_WORD word;
    struct{
      IO_WORD REC0 :1;
      IO_WORD REC1 :1;
      IO_WORD REC2 :1;
      IO_WORD REC3 :1;
      IO_WORD REC4 :1;
      IO_WORD REC5 :1;
      IO_WORD REC6 :1;
      IO_WORD REC7 :1;
      IO_WORD TEC0 :1;
      IO_WORD TEC1 :1;
      IO_WORD TEC2 :1;
      IO_WORD TEC3 :1;
      IO_WORD TEC4 :1;
      IO_WORD TEC5 :1;
      IO_WORD TEC6 :1;
      IO_WORD TEC7 :1;
    }bit;
    struct{
      IO_BYTE REC;
      IO_BYTE TEC;
    }bitc;
  }rtec;
  union{  
    IO_WORD word;
    struct{
      IO_WORD PSC0 :1;
      IO_WORD PSC1 :1;
      IO_WORD PSC2 :1;
      IO_WORD PSC3 :1;
      IO_WORD PSC4 :1;
      IO_WORD PSC5 :1;
      IO_WORD RSJ0 :1;
      IO_WORD RSJ1 :1;
      IO_WORD TS10 :1;
      IO_WORD TS11 :1;
      IO_WORD TS12 :1;
      IO_WORD TS13 :1;
      IO_WORD TS20 :1;
      IO_WORD TS21 :1;
      IO_WORD TS22 :1;
      IO_WORD      :1;
    } bit;
    struct{
      IO_WORD PSC :6;
      IO_WORD RSJ :2;
      IO_WORD TS1 :4;
      IO_WORD TS2 :3;
    } bitc;
  } btr;
  union{  
    IO_BYTE byte;
    IO_WORD _reserved;
      struct{
      IO_WORD IDE0 :1;
      IO_WORD IDE1 :1;
      IO_WORD IDE2 :1;
      IO_WORD IDE3 :1;
      IO_WORD IDE4 :1;
      IO_WORD IDE5 :1;
      IO_WORD IDE6 :1;
      IO_WORD IDE7 :1;
    } bit;
  } ider;
  union{  
    IO_BYTE byte;
    IO_WORD _reserved;
    struct{
      IO_WORD TRTR0 :1;
      IO_WORD TRTR1 :1;
      IO_WORD TRTR2 :1;
      IO_WORD TRTR3 :1;
      IO_WORD TRTR4 :1;
      IO_WORD TRTR5 :1;
      IO_WORD TRTR6 :1;
      IO_WORD TRTR7 :1;
    } bit;
  } trtrr;
  union{  
    IO_BYTE byte;
    IO_WORD _reserved;
    struct{
      IO_WORD RFWT0 :1;
      IO_WORD RFWT1 :1;
      IO_WORD RFWT2 :1;
      IO_WORD RFWT3 :1;
      IO_WORD RFWT4 :1;
      IO_WORD RFWT5 :1;
      IO_WORD RFWT6 :1;
      IO_WORD RFWT7 :1;
    } bit;
  } rfwtr;
  union{  
    IO_BYTE byte;
    IO_WORD _reserved;
    struct{
    IO_WORD TIE0 :1;
    IO_WORD TIE1 :1;
    IO_WORD TIE2 :1;
    IO_WORD TIE3 :1;
    IO_WORD TIE4 :1;
    IO_WORD TIE5 :1;
    IO_WORD TIE6 :1;
    IO_WORD TIE7 :1;
  } bit;
 } tier;
  union{
    IO_WORD     word;
	IO_LWORD     _reserved;
    struct{
      IO_WORD    AMS00:1;
      IO_WORD    AMS01:1;
      IO_WORD    AMS10:1;
      IO_WORD    AMS11:1;
      IO_WORD    AMS20:1;
      IO_WORD    AMS21:1;
      IO_WORD    AMS30:1;
      IO_WORD    AMS31:1;
      IO_WORD    AMS40:1;
      IO_WORD    AMS41:1;
      IO_WORD    AMS50:1;
      IO_WORD    AMS51:1;
      IO_WORD    AMS60:1;
      IO_WORD    AMS61:1;
      IO_WORD    AMS70:1;
      IO_WORD    AMS71:1;
    }bit;
    struct{
      IO_WORD    AMS0:2;
      IO_WORD    AMS1:2;
      IO_WORD    AMS2:2;
      IO_WORD    AMS3:2;
      IO_WORD    AMS4:2;
      IO_WORD    AMS5:2;
      IO_WORD    AMS6:2;
      IO_WORD    AMS7:2;
    } bitc;
  } amsr;
  union {
    IO_LWORD   dword;
    IO_WORD     word;
    struct {
        IO_WORD AM21 :1;
        IO_WORD AM22 :1;
        IO_WORD AM23 :1;
        IO_WORD AM24 :1;
        IO_WORD AM25 :1;
        IO_WORD AM26 :1;
        IO_WORD AM27 :1;
        IO_WORD AM28 :1;

        IO_WORD AM13 :1;
        IO_WORD AM14 :1;
        IO_WORD AM15 :1;
        IO_WORD AM16 :1;
        IO_WORD AM17 :1;
        IO_WORD AM18 :1;
        IO_WORD AM19 :1;
        IO_WORD AM20 :1;

        IO_WORD AM5 :1;
        IO_WORD AM6 :1;
        IO_WORD AM7 :1;
        IO_WORD AM8 :1;
        IO_WORD AM9 :1;
        IO_WORD AM10 :1;
        IO_WORD AM11 :1;
        IO_WORD AM12 :1;

        IO_WORD      :3;
        IO_WORD AM0 :1;
        IO_WORD AM1 :1;
        IO_WORD AM2 :1;
        IO_WORD AM3 :1;
        IO_WORD AM4 :1;
    } bit_ext;
    struct {
        IO_WORD AM3 :1;
        IO_WORD AM4 :1;
        IO_WORD AM5 :1;
        IO_WORD AM6 :1;
        IO_WORD AM7 :1;
        IO_WORD AM8 :1;
        IO_WORD AM9 :1;
        IO_WORD AM10 :1;

        IO_WORD     :5;
        IO_WORD AM0 :1;
        IO_WORD AM1 :1;
        IO_WORD AM2 :1;
    } bit_std;
    struct {
        IO_WORD AM21_28    : 8;
        IO_WORD AM13_20    : 8;
        IO_WORD AM5_12     : 8;
        IO_WORD unused     : 3;            /* unused bits */
        IO_WORD AM0_4      : 5;
    } bitc_ext;
    struct {
        IO_WORD AM3_10 : 8;
        IO_WORD unused : 5;                /* unused bits   */
        IO_WORD AM0_2  : 3;
        /*IO_WORD upper    :16;                /* unused bits */
    } bitc_std;
  } amr[2];
} CANHSTR;

typedef struct
{
    IO_WORD ram[8];
    union {
    IO_LWORD   dword;
    IO_WORD     word;
    struct {
        const IO_WORD ID21 :1;
        const IO_WORD ID22 :1;
        const IO_WORD ID23 :1;
        const IO_WORD ID24 :1;
        const IO_WORD ID25 :1;
        const IO_WORD ID26 :1;
        const IO_WORD ID27 :1;
        const IO_WORD ID28 :1;

        const IO_WORD ID13 :1;
        const IO_WORD ID14 :1;
        const IO_WORD ID15 :1;
        const IO_WORD ID16 :1;
        const IO_WORD ID17 :1;
        const IO_WORD ID18 :1;
        const IO_WORD ID19 :1;
        const IO_WORD ID20 :1;

        const IO_WORD ID5 :1;
        const IO_WORD ID6 :1;
        const IO_WORD ID7 :1;
        const IO_WORD ID8 :1;
        const IO_WORD ID9 :1;
        const IO_WORD ID10 :1;
        const IO_WORD ID11 :1;
        const IO_WORD ID12 :1;

        const IO_WORD     :3;
        const IO_WORD ID0 :1;
        const IO_WORD ID1 :1;
        const IO_WORD ID2 :1;
        const IO_WORD ID3 :1;
        const IO_WORD ID4 :1;
    } bit_ext;
    struct {
        const IO_WORD ID3 :1;
        const IO_WORD ID4 :1;
        const IO_WORD ID5 :1;
        const IO_WORD ID6 :1;
        const IO_WORD ID7 :1;
        const IO_WORD ID8 :1;
        const IO_WORD ID9 :1;
        const IO_WORD ID10:1;

        const IO_WORD ID  :5;
        const IO_WORD ID0 :1;
        const IO_WORD ID1 :1;
        const IO_WORD ID2 :1;
    } bit_std;
    struct {
        const IO_WORD ID21_28 : 8;
        const IO_WORD ID13_20 : 8;
        const IO_WORD ID5_12  : 8;
        const IO_WORD unused  : 3;         /* unused bits */
        const IO_WORD ID0_4   : 5;
    } bitc_ext;
    struct {
        const IO_WORD ID3_10  : 8;
        const IO_WORD unused  : 5;             /* unused bits   */
        const IO_WORD ID0_2   : 3;
    } bitc_std;
  } idr[8];
  union{
    IO_WORD         word;
    IO_BYTE         byte;
    struct {
        IO_WORD     DLC0:1;
        IO_WORD     DLC1:1;
        IO_WORD     DLC2:1;
        IO_WORD     DLC3:1;
    } bit;
    struct {
        IO_WORD     DLC:4;
    } bitc;
  } dlcr[8];
  union{
          IO_LWORD      dwords [2];
          long          longs  [2];
          IO_WORD       words  [4];
          int           ints   [4];
    const IO_BYTE       bytes  [8]; /* because of word access*/
    const char          chars  [8]; /* because of word access*/
          float         floats [2];
          double        adouble;
  }dtr[8];
} CANMSTR;

#endif	/* CANSTR */
__IO_EXTERN __io DIRRSTR _dirr;   /* Delayed Interrupt */
#define DIRR _dirr.byte
#define DIRR_R0 _dirr.bit.R0
__IO_EXTERN __io IO_BYTE _lpmcr;
#define LPMCR _lpmcr    /* Low Power Control Registers */
__IO_EXTERN __io CKSCRSTR _ckscr;  
#define CKSCR _ckscr.byte
#define CKSCR_CS0 _ckscr.bit.CS0
#define CKSCR_CS1 _ckscr.bit.CS1
#define CKSCR_MCS _ckscr.bit.MCS
#define CKSCR_SCS _ckscr.bit.SCS
#define CKSCR_WS0 _ckscr.bit.WS0
#define CKSCR_WS1 _ckscr.bit.WS1
#define CKSCR_MCM _ckscr.bit.MCM
#define CKSCR_SCM _ckscr.bit.SCM
#define CKSCR_CS _ckscr.bitc.CS
#define CKSCR_WS _ckscr.bitc.WS
__IO_EXTERN __io IO_BYTE _arsr;
#define ARSR _arsr    /* Bus Pin Control Circuit */
__IO_EXTERN __io IO_BYTE _hacr;
#define HACR _hacr   
__IO_EXTERN __io IO_BYTE _ecsr;
#define ECSR _ecsr   
__IO_EXTERN __io IO_BYTE _wdtc;
#define WDTC _wdtc    /* Watch Dog */
__IO_EXTERN __io IO_BYTE _tbtc;
#define TBTC _tbtc    /* Time Base Timer */
__IO_EXTERN __io WTCSTR _wtc;   /* Watch Timer */
#define WTC _wtc.byte
#define WTC_WTC0 _wtc.bit.WTC0
#define WTC_WTC1 _wtc.bit.WTC1
#define WTC_WTC2 _wtc.bit.WTC2
#define WTC_WTR _wtc.bit.WTR
#define WTC_WTOF _wtc.bit.WTOF
#define WTC_WTIE _wtc.bit.WTIE
#define WTC_SCE _wtc.bit.SCE
#define WTC_WDCS _wtc.bit.WDCS
__IO_EXTERN __io FMCSSTR _fmcs;   /* Flash Control Register */
#define FMCS _fmcs.byte
#define FMCS_LPM0 _fmcs.bit.LPM0
#define FMCS_LPM1 _fmcs.bit.LPM1
#define FMCS_RDY _fmcs.bit.RDY
#define FMCS_WE _fmcs.bit.WE
#define FMCS_RDYINT _fmcs.bit.RDYINT
#define FMCS_INTE _fmcs.bit.INTE
/*******************************************************************
 *
 *    DESCRIPTION:  Interrupt Control Register Declaration
 *
 *    AUTHOR:       Fujitsu Mikroelektronik GmbH
 *
 *    HISTORY:    
 * Version 1.0      26.01.99: 
 *      - original version
 * Version 1.2      11.02.99
 *      - "extern" changed to pre-defined macro of IO-Wizard 
 *        (__IO_EXTERN), requires IO-Wizard 1.7 or later
 *
 * Version 1.3      17.07.2002  HW  Bitdefinitions as const, no RMV allowed
 *
 *******************************************************************/

#ifndef __ICR_H
#define __ICR_H

typedef union{  
    unsigned char    byte;
    struct{
      const unsigned char IL0      :1;
      const unsigned char IL1      :1;
      const unsigned char IL2      :1;
      const unsigned char ISE      :1;
      const unsigned char ICS0     :1;
      const unsigned char ICS1     :1;
      const unsigned char ICS2     :1;
      const unsigned char ICS3     :1;
    }bit;
    struct{
      const unsigned char    :4;
      const unsigned char S0 :1;
      const unsigned char S1 :1;
    }bitrd;
    struct{
      const unsigned char IL       :3;
      const unsigned char :1;
      const unsigned char ICS      :4;
    }bitc;
    struct{
      const unsigned char    :4;
      const unsigned char S  :2;
    }bitcrd;
 }ICRSTR;

__IO_EXTERN __io ICRSTR _icr[16];

#define ICR(i)        _icr[i].byte
#define ICR_IL0(i)    _icr[i].bit.IL0
#define ICR_IL1(i)    _icr[i].bit.IL1
#define ICR_IL2(i)    _icr[i].bit.IL2
#define ICR_ISE(i)    _icr[i].bit.ISE
#define ICR_ICS0(i)   _icr[i].bit.ICS0
#define ICR_ICS1(i)   _icr[i].bit.ICS1
#define ICR_ICS2(i)   _icr[i].bit.ICS2
#define ICR_ICS3(i)   _icr[i].bit.ICS3
#define ICR_IL(i)     _icr[i].bitc.IL
#define ICR_ICS(i)    _icr[i].bitc.ICS
#define ICR_S0(i)     _icr[i].bitrd.S0
#define ICR_S1(i)     _icr[i].bitrd.S1
#define ICR_S(i)      _icr[i].bitcrd.S

#define ICR00       _icr[0].byte
#define ICR00_IL0   _icr[0].bit.IL0
#define ICR00_IL1   _icr[0].bit.IL1
#define ICR00_IL2   _icr[0].bit.IL2
#define ICR00_ISE   _icr[0].bit.ISE
#define ICR00_ICS0  _icr[0].bit.ICS0
#define ICR00_ICS1  _icr[0].bit.ICS1
#define ICR00_ICS2  _icr[0].bit.ICS2
#define ICR00_ICS3  _icr[0].bit.ICS3
#define ICR00_IL    _icr[0].bitc.IL
#define ICR00_ICS   _icr[0].bitc.ICS
#define ICR00_S0    _icr[0].bitrd.S0
#define ICR00_S1    _icr[0].bitrd.S1
#define ICR00_S     _icr[0].bitcrd.S

#define ICR01       _icr[1].byte
#define ICR01_IL0   _icr[1].bit.IL0
#define ICR01_IL1   _icr[1].bit.IL1
#define ICR01_IL2   _icr[1].bit.IL2
#define ICR01_ISE   _icr[1].bit.ISE
#define ICR01_ICS0  _icr[1].bit.ICS0
#define ICR01_ICS1  _icr[1].bit.ICS1
#define ICR01_ICS2  _icr[1].bit.ICS2
#define ICR01_ICS3  _icr[1].bit.ICS3
#define ICR01_IL    _icr[1].bitc.IL
#define ICR01_ICS   _icr[1].bitc.ICS
#define ICR01_S0    _icr[1].bitrd.S0
#define ICR01_S1    _icr[1].bitrd.S1
#define ICR01_S     _icr[1].bitcrd.S

#define ICR02       _icr[2].byte
#define ICR02_IL0   _icr[2].bit.IL0
#define ICR02_IL1   _icr[2].bit.IL1
#define ICR02_IL2   _icr[2].bit.IL2
#define ICR02_ISE   _icr[2].bit.ISE
#define ICR02_ICS0  _icr[2].bit.ICS0
#define ICR02_ICS1  _icr[2].bit.ICS1
#define ICR02_ICS2  _icr[2].bit.ICS2
#define ICR02_ICS3  _icr[2].bit.ICS3
#define ICR02_IL    _icr[2].bitc.IL
#define ICR02_ICS   _icr[2].bitc.ICS
#define ICR02_S0    _icr[2].bitrd.S0
#define ICR02_S1    _icr[2].bitrd.S1
#define ICR02_S     _icr[2].bitcrd.S

#define ICR03       _icr[3].byte
#define ICR03_IL0   _icr[3].bit.IL0
#define ICR03_IL1   _icr[3].bit.IL1
#define ICR03_IL2   _icr[3].bit.IL2
#define ICR03_ISE   _icr[3].bit.ISE
#define ICR03_ICS0  _icr[3].bit.ICS0
#define ICR03_ICS1  _icr[3].bit.ICS1
#define ICR03_ICS2  _icr[3].bit.ICS2
#define ICR03_ICS3  _icr[3].bit.ICS3
#define ICR03_IL    _icr[3].bitc.IL
#define ICR03_ICS   _icr[3].bitc.ICS
#define ICR03_S0    _icr[3].bitrd.S0
#define ICR03_S1    _icr[3].bitrd.S1
#define ICR03_S     _icr[3].bitcrd.S

#define ICR04       _icr[4].byte
#define ICR04_IL4   _icr[4].bit.IL0
#define ICR04_IL1   _icr[4].bit.IL1
#define ICR04_IL2   _icr[4].bit.IL2
#define ICR04_ISE   _icr[4].bit.ISE
#define ICR04_ICS4  _icr[4].bit.ICS0
#define ICR04_ICS1  _icr[4].bit.ICS1
#define ICR04_ICS2  _icr[4].bit.ICS2
#define ICR04_ICS3  _icr[4].bit.ICS3
#define ICR04_IL    _icr[4].bitc.IL
#define ICR04_ICS   _icr[4].bitc.ICS
#define ICR04_S4    _icr[4].bitrd.S0
#define ICR04_S1    _icr[4].bitrd.S1
#define ICR04_S     _icr[4].bitcrd.S

#define ICR05       _icr[5].byte
#define ICR05_IL0   _icr[5].bit.IL0
#define ICR05_IL1   _icr[5].bit.IL1
#define ICR05_IL2   _icr[5].bit.IL2
#define ICR05_ISE   _icr[5].bit.ISE
#define ICR05_ICS0  _icr[5].bit.ICS0
#define ICR05_ICS1  _icr[5].bit.ICS1
#define ICR05_ICS2  _icr[5].bit.ICS2
#define ICR05_ICS3  _icr[5].bit.ICS3
#define ICR05_IL    _icr[5].bitc.IL
#define ICR05_ICS   _icr[5].bitc.ICS
#define ICR05_S0    _icr[5].bitrd.S0
#define ICR05_S1    _icr[5].bitrd.S1
#define ICR05_S     _icr[5].bitcrd.S

#define ICR06       _icr[6].byte
#define ICR06_IL0   _icr[6].bit.IL0
#define ICR06_IL1   _icr[6].bit.IL1
#define ICR06_IL2   _icr[6].bit.IL2
#define ICR06_ISE   _icr[6].bit.ISE
#define ICR06_ICS0  _icr[6].bit.ICS0
#define ICR06_ICS1  _icr[6].bit.ICS1
#define ICR06_ICS2  _icr[6].bit.ICS2
#define ICR06_ICS3  _icr[6].bit.ICS3
#define ICR06_IL    _icr[6].bitc.IL
#define ICR06_ICS   _icr[6].bitc.ICS
#define ICR06_S0    _icr[6].bitrd.S0
#define ICR06_S1    _icr[6].bitrd.S1
#define ICR06_S     _icr[6].bitcrd.S

#define ICR07       _icr[7].byte
#define ICR07_IL0   _icr[7].bit.IL0
#define ICR07_IL1   _icr[7].bit.IL1
#define ICR07_IL2   _icr[7].bit.IL2
#define ICR07_ISE   _icr[7].bit.ISE
#define ICR07_ICS0  _icr[7].bit.ICS0
#define ICR07_ICS1  _icr[7].bit.ICS1
#define ICR07_ICS2  _icr[7].bit.ICS2
#define ICR07_ICS3  _icr[7].bit.ICS3
#define ICR07_IL    _icr[7].bitc.IL
#define ICR07_ICS   _icr[7].bitc.ICS
#define ICR07_S0    _icr[7].bitrd.S0
#define ICR07_S1    _icr[7].bitrd.S1
#define ICR07_S     _icr[7].bitcrd.S

#define ICR08       _icr[8].byte
#define ICR08_IL0   _icr[8].bit.IL0
#define ICR08_IL1   _icr[8].bit.IL1
#define ICR08_IL2   _icr[8].bit.IL2
#define ICR08_ISE   _icr[8].bit.ISE
#define ICR08_ICS0  _icr[8].bit.ICS0
#define ICR08_ICS1  _icr[8].bit.ICS1
#define ICR08_ICS2  _icr[8].bit.ICS2
#define ICR08_ICS3  _icr[8].bit.ICS3
#define ICR08_IL    _icr[8].bitc.IL
#define ICR08_ICS   _icr[8].bitc.ICS
#define ICR08_S0    _icr[8].bitrd.S0
#define ICR08_S1    _icr[8].bitrd.S1
#define ICR08_S     _icr[8].bitcrd.S

#define ICR09       _icr[9].byte
#define ICR09_IL0   _icr[9].bit.IL0
#define ICR09_IL1   _icr[9].bit.IL1
#define ICR09_IL2   _icr[9].bit.IL2
#define ICR09_ISE   _icr[9].bit.ISE
#define ICR09_ICS0  _icr[9].bit.ICS0
#define ICR09_ICS1  _icr[9].bit.ICS1
#define ICR09_ICS2  _icr[9].bit.ICS2
#define ICR09_ICS3  _icr[9].bit.ICS3
#define ICR09_IL    _icr[9].bitc.IL
#define ICR09_ICS   _icr[9].bitc.ICS
#define ICR09_S0    _icr[9].bitrd.S0
#define ICR09_S1    _icr[9].bitrd.S1
#define ICR09_S     _icr[9].bitcrd.S

#define ICR10       _icr[10].byte
#define ICR10_IL0   _icr[10].bit.IL0
#define ICR10_IL1   _icr[10].bit.IL1
#define ICR10_IL2   _icr[10].bit.IL2
#define ICR10_ISE   _icr[10].bit.ISE
#define ICR10_ICS0  _icr[10].bit.ICS0
#define ICR10_ICS1  _icr[10].bit.ICS1
#define ICR10_ICS2  _icr[10].bit.ICS2
#define ICR10_ICS3  _icr[10].bit.ICS3
#define ICR10_IL    _icr[10].bitc.IL
#define ICR10_ICS   _icr[10].bitc.ICS
#define ICR10_S0    _icr[10].bitrd.S0
#define ICR10_S1    _icr[10].bitrd.S1
#define ICR10_S     _icr[10].bitcrd.S

#define ICR11       _icr[11].byte
#define ICR11_IL0   _icr[11].bit.IL0
#define ICR11_IL1   _icr[11].bit.IL1
#define ICR11_IL2   _icr[11].bit.IL2
#define ICR11_ISE   _icr[11].bit.ISE
#define ICR11_ICS0  _icr[11].bit.ICS0
#define ICR11_ICS1  _icr[11].bit.ICS1
#define ICR11_ICS2  _icr[11].bit.ICS2
#define ICR11_ICS3  _icr[11].bit.ICS3
#define ICR11_IL    _icr[11].bitc.IL
#define ICR11_ICS   _icr[11].bitc.ICS
#define ICR11_S0    _icr[11].bitrd.S0
#define ICR11_S1    _icr[11].bitrd.S1
#define ICR11_S     _icr[11].bitcrd.S

#define ICR12       _icr[12].byte
#define ICR12_IL0   _icr[12].bit.IL0
#define ICR12_IL1   _icr[12].bit.IL1
#define ICR12_IL2   _icr[12].bit.IL2
#define ICR12_ISE   _icr[12].bit.ISE
#define ICR12_ICS0  _icr[12].bit.ICS0
#define ICR12_ICS1  _icr[12].bit.ICS1
#define ICR12_ICS2  _icr[12].bit.ICS2
#define ICR12_ICS3  _icr[12].bit.ICS3
#define ICR12_IL    _icr[12].bitc.IL
#define ICR12_ICS   _icr[12].bitc.ICS
#define ICR12_S0    _icr[12].bitrd.S0
#define ICR12_S1    _icr[12].bitrd.S1
#define ICR12_S     _icr[12].bitcrd.S

#define ICR13       _icr[13].byte
#define ICR13_IL0   _icr[13].bit.IL0
#define ICR13_IL1   _icr[13].bit.IL1
#define ICR13_IL2   _icr[13].bit.IL2
#define ICR13_ISE   _icr[13].bit.ISE
#define ICR13_ICS0  _icr[13].bit.ICS0
#define ICR13_ICS1  _icr[13].bit.ICS1
#define ICR13_ICS2  _icr[13].bit.ICS2
#define ICR13_ICS3  _icr[13].bit.ICS3
#define ICR13_IL    _icr[13].bitc.IL
#define ICR13_ICS   _icr[13].bitc.ICS
#define ICR13_S0    _icr[13].bitrd.S0
#define ICR13_S1    _icr[13].bitrd.S1
#define ICR13_S     _icr[13].bitcrd.S

#define ICR14       _icr[14].byte
#define ICR14_IL0   _icr[14].bit.IL0
#define ICR14_IL1   _icr[14].bit.IL1
#define ICR14_IL2   _icr[14].bit.IL2
#define ICR14_ISE   _icr[14].bit.ISE
#define ICR14_ICS0  _icr[14].bit.ICS0
#define ICR14_ICS1  _icr[14].bit.ICS1
#define ICR14_ICS2  _icr[14].bit.ICS2
#define ICR14_ICS3  _icr[14].bit.ICS3
#define ICR14_IL    _icr[14].bitc.IL
#define ICR14_ICS   _icr[14].bitc.ICS
#define ICR14_S0    _icr[14].bitrd.S0
#define ICR14_S1    _icr[14].bitrd.S1
#define ICR14_S     _icr[14].bitcrd.S

#define ICR15       _icr[15].byte
#define ICR15_IL0   _icr[15].bit.IL0
#define ICR15_IL1   _icr[15].bit.IL1
#define ICR15_IL2   _icr[15].bit.IL2
#define ICR15_ISE   _icr[15].bit.ISE
#define ICR15_ICS0  _icr[15].bit.ICS0
#define ICR15_ICS1  _icr[15].bit.ICS1
#define ICR15_ICS2  _icr[15].bit.ICS2
#define ICR15_ICS3  _icr[15].bit.ICS3
#define ICR15_IL    _icr[15].bitc.IL
#define ICR15_ICS   _icr[15].bitc.ICS
#define ICR15_S0    _icr[15].bitrd.S0
#define ICR15_S1    _icr[15].bitrd.S1
#define ICR15_S     _icr[15].bitcrd.S


#endif /*  __ICR_H */
__IO_EXTENDED IO_BYTE _padr0_l;
#define PADR0_L _padr0_l    /* ROM CORRECTION */
__IO_EXTENDED IO_BYTE _padr0_m;
#define PADR0_M _padr0_m   
__IO_EXTENDED IO_BYTE _padr0_h;
#define PADR0_H _padr0_h   
__IO_EXTENDED IO_BYTE _padr1_l;
#define PADR1_L _padr1_l   
__IO_EXTENDED IO_BYTE _padr1_m;
#define PADR1_M _padr1_m   
__IO_EXTENDED IO_BYTE _padr1_h;
#define PADR1_H _padr1_h   
__IO_EXTENDED TMR0STR _tmr0;   /* Reload Timer 0/1 */
#define TMR0 _tmr0.word
#define TMR0_D0 _tmr0.bit.D0
#define TMR0_D1 _tmr0.bit.D1
#define TMR0_D2 _tmr0.bit.D2
#define TMR0_D3 _tmr0.bit.D3
#define TMR0_D4 _tmr0.bit.D4
#define TMR0_D5 _tmr0.bit.D5
#define TMR0_D6 _tmr0.bit.D6
#define TMR0_D7 _tmr0.bit.D7
#define TMR0_D8 _tmr0.bit.D8
#define TMR0_D9 _tmr0.bit.D9
#define TMR0_D10 _tmr0.bit.D10
#define TMR0_D11 _tmr0.bit.D11
#define TMR0_D12 _tmr0.bit.D12
#define TMR0_D13 _tmr0.bit.D13
#define TMR0_D14 _tmr0.bit.D14
#define TMR0_D15 _tmr0.bit.D15
__IO_EXTENDED TMRLR0STR _tmrlr0;  
#define TMRLR0 _tmrlr0.word
#define TMRLR0_D0 _tmrlr0.bit.D0
#define TMRLR0_D1 _tmrlr0.bit.D1
#define TMRLR0_D2 _tmrlr0.bit.D2
#define TMRLR0_D3 _tmrlr0.bit.D3
#define TMRLR0_D4 _tmrlr0.bit.D4
#define TMRLR0_D5 _tmrlr0.bit.D5
#define TMRLR0_D6 _tmrlr0.bit.D6
#define TMRLR0_D7 _tmrlr0.bit.D7
#define TMRLR0_D8 _tmrlr0.bit.D8
#define TMRLR0_D9 _tmrlr0.bit.D9
#define TMRLR0_D10 _tmrlr0.bit.D10
#define TMRLR0_D11 _tmrlr0.bit.D11
#define TMRLR0_D12 _tmrlr0.bit.D12
#define TMRLR0_D13 _tmrlr0.bit.D13
#define TMRLR0_D14 _tmrlr0.bit.D14
#define TMRLR0_D15 _tmrlr0.bit.D15
__IO_EXTENDED TMR1STR _tmr1;  
#define TMR1 _tmr1.word
#define TMR1_D0 _tmr1.bit.D0
#define TMR1_D1 _tmr1.bit.D1
#define TMR1_D2 _tmr1.bit.D2
#define TMR1_D3 _tmr1.bit.D3
#define TMR1_D4 _tmr1.bit.D4
#define TMR1_D5 _tmr1.bit.D5
#define TMR1_D6 _tmr1.bit.D6
#define TMR1_D7 _tmr1.bit.D7
#define TMR1_D8 _tmr1.bit.D8
#define TMR1_D9 _tmr1.bit.D9
#define TMR1_D10 _tmr1.bit.D10
#define TMR1_D11 _tmr1.bit.D11
#define TMR1_D12 _tmr1.bit.D12
#define TMR1_D13 _tmr1.bit.D13
#define TMR1_D14 _tmr1.bit.D14
#define TMR1_D15 _tmr1.bit.D15
__IO_EXTENDED TMRLR1STR _tmrlr1;  
#define TMRLR1 _tmrlr1.word
#define TMRLR1_D0 _tmrlr1.bit.D0
#define TMRLR1_D1 _tmrlr1.bit.D1
#define TMRLR1_D2 _tmrlr1.bit.D2
#define TMRLR1_D3 _tmrlr1.bit.D3
#define TMRLR1_D4 _tmrlr1.bit.D4
#define TMRLR1_D5 _tmrlr1.bit.D5
#define TMRLR1_D6 _tmrlr1.bit.D6
#define TMRLR1_D7 _tmrlr1.bit.D7
#define TMRLR1_D8 _tmrlr1.bit.D8
#define TMRLR1_D9 _tmrlr1.bit.D9
#define TMRLR1_D10 _tmrlr1.bit.D10
#define TMRLR1_D11 _tmrlr1.bit.D11
#define TMRLR1_D12 _tmrlr1.bit.D12
#define TMRLR1_D13 _tmrlr1.bit.D13
#define TMRLR1_D14 _tmrlr1.bit.D14
#define TMRLR1_D15 _tmrlr1.bit.D15
__IO_EXTENDED IO_WORD _prl0;
#define PRL0 _prl0    /* PPG 0/1/2/3 */
__IO_EXTENDED IO_BYTE _prll0;
#define PRLL0 _prll0   
__IO_EXTENDED IO_BYTE _prlh0;
#define PRLH0 _prlh0   
__IO_EXTENDED IO_WORD _prl1;
#define PRL1 _prl1   
__IO_EXTENDED IO_BYTE _prll1;
#define PRLL1 _prll1   
__IO_EXTENDED IO_BYTE _prlh1;
#define PRLH1 _prlh1   
__IO_EXTENDED IO_WORD _prl2;
#define PRL2 _prl2   
__IO_EXTENDED IO_BYTE _prll2;
#define PRLL2 _prll2   
__IO_EXTENDED IO_BYTE _prlh2;
#define PRLH2 _prlh2   
__IO_EXTENDED IO_WORD _prl3;
#define PRL3 _prl3   
__IO_EXTENDED IO_BYTE _prll3;
#define PRLL3 _prll3   
__IO_EXTENDED IO_BYTE _prlh3;
#define PRLH3 _prlh3   
/*
 * CANIO: control macros single CAN for LX-controllers
 *        This version supports 8 message buffers
 * Version: 1.0            08.02.00     HL
 *      - original version
 *      - only 8 message buffers
 *      - reduced registers changed to byte access
 */

#ifndef __CANMAC
#define __CANMAC

__IO_EXTERN __io CANLSTR _canl;

#define BVALR           _canl.bvalr.byte
#define BVALR_BVAL0     _canl.bvalr.bit.BVAL0
#define BVALR_BVAL1     _canl.bvalr.bit.BVAL1
#define BVALR_BVAL2     _canl.bvalr.bit.BVAL2
#define BVALR_BVAL3     _canl.bvalr.bit.BVAL3
#define BVALR_BVAL4     _canl.bvalr.bit.BVAL4
#define BVALR_BVAL5     _canl.bvalr.bit.BVAL5
#define BVALR_BVAL6     _canl.bvalr.bit.BVAL6
#define BVALR_BVAL7     _canl.bvalr.bit.BVAL7

#define TREQR           _canl.treqr.byte
#define TREQR_TREQ0     _canl.treqr.bit.TREQ0
#define TREQR_TREQ1     _canl.treqr.bit.TREQ1
#define TREQR_TREQ2     _canl.treqr.bit.TREQ2
#define TREQR_TREQ3     _canl.treqr.bit.TREQ3
#define TREQR_TREQ4     _canl.treqr.bit.TREQ4
#define TREQR_TREQ5     _canl.treqr.bit.TREQ5
#define TREQR_TREQ6     _canl.treqr.bit.TREQ6
#define TREQR_TREQ7     _canl.treqr.bit.TREQ7

#define TCANR           _canl.tcanr.byte
#define TCANR_TCAN0     _canl.tcanr.bit.TCAN0
#define TCANR_TCAN1     _canl.tcanr.bit.TCAN1
#define TCANR_TCAN2     _canl.tcanr.bit.TCAN2
#define TCANR_TCAN3     _canl.tcanr.bit.TCAN3
#define TCANR_TCAN4     _canl.tcanr.bit.TCAN4
#define TCANR_TCAN5     _canl.tcanr.bit.TCAN5
#define TCANR_TCAN6     _canl.tcanr.bit.TCAN6
#define TCANR_TCAN7     _canl.tcanr.bit.TCAN7

#define TCR             _canl.tcr.byte
#define TCR_TC0         _canl.tcr.bit.TC0
#define TCR_TC1         _canl.tcr.bit.TC1
#define TCR_TC2         _canl.tcr.bit.TC2
#define TCR_TC3         _canl.tcr.bit.TC3
#define TCR_TC4         _canl.tcr.bit.TC4
#define TCR_TC5         _canl.tcr.bit.TC5
#define TCR_TC6         _canl.tcr.bit.TC6
#define TCR_TC7         _canl.tcr.bit.TC7

#define RCR             _canl.rcr.byte
#define RCR_RC0         _canl.rcr.bit.RC0
#define RCR_RC1         _canl.rcr.bit.RC1
#define RCR_RC2         _canl.rcr.bit.RC2
#define RCR_RC3         _canl.rcr.bit.RC3
#define RCR_RC4         _canl.rcr.bit.RC4
#define RCR_RC5         _canl.rcr.bit.RC5
#define RCR_RC6         _canl.rcr.bit.RC6
#define RCR_RC7         _canl.rcr.bit.RC7

#define RRTRR           _canl.rrtrr.byte
#define RRTRR_RRTR0     _canl.rrtrr.bit.RRTR0
#define RRTRR_RRTR1     _canl.rrtrr.bit.RRTR1
#define RRTRR_RRTR2     _canl.rrtrr.bit.RRTR2
#define RRTRR_RRTR3     _canl.rrtrr.bit.RRTR3
#define RRTRR_RRTR4     _canl.rrtrr.bit.RRTR4
#define RRTRR_RRTR5     _canl.rrtrr.bit.RRTR5
#define RRTRR_RRTR6     _canl.rrtrr.bit.RRTR6
#define RRTRR_RRTR7     _canl.rrtrr.bit.RRTR7

#define ROVRR           _canl.rovrr.byte
#define ROVRR_ROVR0     _canl.rovrr.bit.ROVR0
#define ROVRR_ROVR1     _canl.rovrr.bit.ROVR1
#define ROVRR_ROVR2     _canl.rovrr.bit.ROVR2
#define ROVRR_ROVR3     _canl.rovrr.bit.ROVR3
#define ROVRR_ROVR4     _canl.rovrr.bit.ROVR4
#define ROVRR_ROVR5     _canl.rovrr.bit.ROVR5
#define ROVRR_ROVR6     _canl.rovrr.bit.ROVR6
#define ROVRR_ROVR7     _canl.rovrr.bit.ROVR7

#define RIER            _canl.rier.byte
#define RIER_RIE0       _canl.rier.bit.RIE0
#define RIER_RIE1       _canl.rier.bit.RIE1
#define RIER_RIE2       _canl.rier.bit.RIE2
#define RIER_RIE3       _canl.rier.bit.RIE3
#define RIER_RIE4       _canl.rier.bit.RIE4
#define RIER_RIE5       _canl.rier.bit.RIE5
#define RIER_RIE6       _canl.rier.bit.RIE6
#define RIER_RIE7       _canl.rier.bit.RIE7

__IO_EXTENDED CANHSTR _canh;

#define CSR             _canh.csr.word
#define CSR_HALT        _canh.csr.bit.HALT
#define CSR_NIE         _canh.csr.bit.NIE
#define CSR_TOE         _canh.csr.bit.TOE
#define CSR_NS0         _canh.csr.bit.NS0
#define CSR_NS1         _canh.csr.bit.NS1
#define CSR_NT          _canh.csr.bit.NT
#define CSR_RS          _canh.csr.bit.RS
#define CSR_TS          _canh.csr.bit.TS
#define CSR_NS          _canh.csr.bitc.NS

#define LEIR            _canh.leir.byte
#define LEIR_MBP0       _canh.leir.bit.MBP0
#define LEIR_MBP1       _canh.leir.bit.MBP1
#define LEIR_MBP2       _canh.leir.bit.MBP2
#define LEIR_MBP3       _canh.leir.bit.MBP3
#define LEIR_RCE        _canh.leir.bit.RCE
#define LEIR_TCE        _canh.leir.bit.TCE
#define LEIR_NTE        _canh.leir.bit.NTE
#define LEIR_MBP        _canh.leir.bitc.MBP

#define RTEC            _canh.rtec.word
#define RTEC_REC0       _canh.rtec.bit.REC0
#define RTEC_REC1       _canh.rtec.bit.REC1
#define RTEC_REC2       _canh.rtec.bit.REC2
#define RTEC_REC3       _canh.rtec.bit.REC3
#define RTEC_REC4       _canh.rtec.bit.REC4
#define RTEC_REC5       _canh.rtec.bit.REC5
#define RTEC_REC6       _canh.rtec.bit.REC6
#define RTEC_REC7       _canh.rtec.bit.REC7
#define RTEC_TEC0       _canh.rtec.bit.TEC0
#define RTEC_TEC1       _canh.rtec.bit.TEC1
#define RTEC_TEC2       _canh.rtec.bit.TEC2
#define RTEC_TEC3       _canh.rtec.bit.TEC3
#define RTEC_TEC4       _canh.rtec.bit.TEC4
#define RTEC_TEC5       _canh.rtec.bit.TEC5
#define RTEC_TEC6       _canh.rtec.bit.TEC6
#define RTEC_TEC7       _canh.rtec.bit.TEC7
#define RTEC_REC        _canh.rtec.bitc.REC
#define RTEC_TEC        _canh.rtec.bitc.TEC

#define BTR             _canh.btr.word
#define BTR_PSC0        _canh.btr.bit.PSC0
#define BTR_PSC1        _canh.btr.bit.PSC1
#define BTR_PSC2        _canh.btr.bit.PSC2
#define BTR_PSC3        _canh.btr.bit.PSC3
#define BTR_PSC4        _canh.btr.bit.PSC4
#define BTR_PSC5        _canh.btr.bit.PSC5
#define BTR_RSJ0        _canh.btr.bit.RSJ0
#define BTR_RSJ1        _canh.btr.bit.RSJ1
#define BTR_TS10        _canh.btr.bit.TS10
#define BTR_TS11        _canh.btr.bit.TS11
#define BTR_TS12        _canh.btr.bit.TS12
#define BTR_TS13        _canh.btr.bit.TS13
#define BTR_TS20        _canh.btr.bit.TS20
#define BTR_TS21        _canh.btr.bit.TS21
#define BTR_TS22        _canh.btr.bit.TS22
#define BTR_PSC         _canh.btr.bitc.PSC
#define BTR_RSJ         _canh.btr.bitc.RSJ
#define BTR_TS1         _canh.btr.bitc.TS1
#define BTR_TS2         _canh.btr.bitc.TS2

#define IDER            _canh.ider.byte
#define IDER_IDE0       _canh.ider.bit.IDE0
#define IDER_IDE1       _canh.ider.bit.IDE1
#define IDER_IDE2       _canh.ider.bit.IDE2
#define IDER_IDE3       _canh.ider.bit.IDE3
#define IDER_IDE4       _canh.ider.bit.IDE4
#define IDER_IDE5       _canh.ider.bit.IDE5
#define IDER_IDE6       _canh.ider.bit.IDE6
#define IDER_IDE7       _canh.ider.bit.IDE7

#define TRTRR           _canh.trtrr.byte
#define TRTRR_TRTR0     _canh.trtrr.bit.TRTR0
#define TRTRR_TRTR1     _canh.trtrr.bit.TRTR1
#define TRTRR_TRTR2     _canh.trtrr.bit.TRTR2
#define TRTRR_TRTR3     _canh.trtrr.bit.TRTR3
#define TRTRR_TRTR4     _canh.trtrr.bit.TRTR4
#define TRTRR_TRTR5     _canh.trtrr.bit.TRTR5
#define TRTRR_TRTR6     _canh.trtrr.bit.TRTR6
#define TRTRR_TRTR7     _canh.trtrr.bit.TRTR7

#define RFWTR           _canh.rfwtr.byte
#define RFWTR_RFWT0     _canh.rfwtr.bit.RFWT0
#define RFWTR_RFWT1     _canh.rfwtr.bit.RFWT1
#define RFWTR_RFWT2     _canh.rfwtr.bit.RFWT2
#define RFWTR_RFWT3     _canh.rfwtr.bit.RFWT3
#define RFWTR_RFWT4     _canh.rfwtr.bit.RFWT4
#define RFWTR_RFWT5     _canh.rfwtr.bit.RFWT5
#define RFWTR_RFWT6     _canh.rfwtr.bit.RFWT6
#define RFWTR_RFWT7     _canh.rfwtr.bit.RFWT7

#define TIER            _canh.tier.byte
#define TIER_TIE0       _canh.tier.bit.TIE0
#define TIER_TIE1       _canh.tier.bit.TIE1
#define TIER_TIE2       _canh.tier.bit.TIE2
#define TIER_TIE3       _canh.tier.bit.TIE3
#define TIER_TIE4       _canh.tier.bit.TIE4
#define TIER_TIE5       _canh.tier.bit.TIE5
#define TIER_TIE6       _canh.tier.bit.TIE6
#define TIER_TIE7       _canh.tier.bit.TIE7

#define AMSR            _canh.amsr.word
#define AMSR_AMS0       _canh.amsr.bitc.AMS0
#define AMSR_AMS1       _canh.amsr.bitc.AMS1
#define AMSR_AMS2       _canh.amsr.bitc.AMS2
#define AMSR_AMS3       _canh.amsr.bitc.AMS3
#define AMSR_AMS4       _canh.amsr.bitc.AMS4
#define AMSR_AMS5       _canh.amsr.bitc.AMS5
#define AMSR_AMS6       _canh.amsr.bitc.AMS6
#define AMSR_AMS7       _canh.amsr.bitc.AMS7
                     
#define AMR0            _canh.amr[0].word  /* AMR0 (std ID bits only) */
#define AMR1            _canh.amr[1].word  /* AMR1 (std ID bits only) */
#define AMRX0           _canh.amr[0].dword /* AMR0 (ext ID bits only) */
#define AMRX1           _canh.amr[1].dword /* AMR1 (ext ID bits only) */

__IO_EXTENDED CANMSTR _canm;

#define IDR(buf)            (_canm.idr[buf].word)
#define IDRX(buf)           (_canm.idr[buf].dword)

#define DLCR(buf)           (_canm.dlcr[buf].byte)
#define DLCR_DLC0(buf)      (_canm.dlcr[buf].bit.DLC0)
#define DLCR_DLC1(buf)      (_canm.dlcr[buf].bit.DLC1)
#define DLCR_DLC2(buf)      (_canm.dlcr[buf].bit.DLC2)
#define DLCR_DLC3(buf)      (_canm.dlcr[buf].bit.DLC3)

#define DTR_BPTR(  buf)     (_canm.dtr[buf].bytes )
#define DTR_WPTR(  buf)     (_canm.dtr[buf].words )
#define DTR_DWPTR( buf)     (_canm.dtr[buf].dwords)
#define DTR_CPTR(  buf)     (_canm.dtr[buf].chars )
#define DTR_IPTR(  buf)     (_canm.dtr[buf].ints  )
#define DTR_LPTR(  buf)     (_canm.dtr[buf].longs )

#define DTR_BYTE(  buf,i)   (_canm.dtr[buf].bytes  [i])
#define DTR_WORD(  buf,i)   (_canm.dtr[buf].words  [i])
#define DTR_DWORD( buf,i)   (_canm.dtr[buf].dwords [i])
#define DTR_CHAR(  buf,i)   (_canm.dtr[buf].chars  [i])
#define DTR_INT(   buf,i)   (_canm.dtr[buf].ints   [i])
#define DTR_LONGS( buf,i)   (_canm.dtr[buf].longs  [i])
#define DTR_FLOATS(buf,i)   (_canm.dtr[buf].floats [i])
#define DTR_DOUBLE(buf  )   (_canm.dtr[buf].adouble   )

#endif /* CANMAC */
#  undef ___IOWIDTH
#endif                   /* __MB90XXX_H */
