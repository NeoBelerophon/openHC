/***************************************************************************
 *
 * OpenHC:                          ___                 _  _  ___
 *  Open source                    / _ \ _ __  ___ _ _ | || |/ __|
 *  Home                          | (_) | '_ \/ -_) ' \| __ | (__ 
 *  Control                        \___/| .__/\___|_||_|_||_|\___|
 * http://openhc.sourceforge.net/       |_| 
 *
 * THIS SAMPLE CODE IS PROVIDED AS IS AND IS SUBJECT TO ALTERATIONS. FUJITSU
 * MICROELECTRONICS ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR
 * ELIGIBILITY FOR ANY PURPOSES.
 *                 (C) Fujitsu Microelectronics Europe GmbH
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/ 

/*! \file vectors.c
    \brief Interrupt vector definitions for Fujitsu MB90F495 controller

  - Interrupt level (priority) setting
  - Interrupt vector definition
  03.11.99  1.00   wpe    V30L20   original version, no resource assignment
*/

#define __FILENUM__ 9 // every file needs to have a unique (8bit) ID

#include "mb90495.h"

/*------------------------------------------------------------------------
   InitIrqLevels()

   This function  pre-sets all interrupt control registers. It can be used
   to set all interrupt priorities in static applications. If this file
   contains assignments to dedicated resources, verify  that the
   appropriate controller is used.

   NOTE: value 7 disables the interrupt and value 0 sets highest priority.
   NOTE: Two resource interrupts always share one ICR register.
*/
void InitIrqLevels(void)
{
/*  ICRxx               shared IRQs for ICR */

    ICR00 = 7;      /*  IRQ11
                        IRQ12 */
    ICR01 = 7;      /*  IRQ13     
                        IRQ14 */
    ICR02 = 6;      /*  IRQ15
                        IRQ16 (timebase timer) */
    ICR03 = 4;      /*  IRQ17 (16 bit reload timer0, tx enable)
                        IRQ18 */
    ICR04 = 7;      /*  IRQ19
                        IRQ20 */
    ICR05 = 7;      /*  IRQ22 */

    ICR06 = 7;      /*  IRQ23 (capture 0)
                        IRQ24 */
    ICR07 = 7;      /*  IRQ25
                        IRQ26 */
    ICR08 = 7;      /*  IRQ27
                        IRQ28 */
    ICR09 = 6;      /*  IRQ30 (capture 2/3) */

    ICR12 = 5;      /*  IRQ36 (16 bit reload timer1, rx timeout) */

#if 0
    /* original */
    ICR13 = 7;      /*  IRQ37
                        IRQ38 */
#else
    /* priority for Accemic debugger on UART1 */
    ICR13 = 2;      /*  IRQ37
                        IRQ48 */
#endif
    ICR14 = 3;      /*  IRQ39    (UART0)
                        IRQ40 */
    ICR15 = 7;      /*  IRQ41
                        IRQ42 */
}

/*------------------------------------------------------------------------
   Prototypes
   
   Add your own prototypes here. Each vector definition needs is proto-
   type. Either do it here or include a header file containing them.

*/
__interrupt void DefaultIRQHandler (void);
__interrupt void uart_irq_rx(void);
__interrupt void uart_irq_tx(void);
__interrupt void uart_irq_tx_end(void);
__interrupt void uart_irq_rx_timeout(void);
__interrupt void timer_irq_tick(void);
__interrupt void rc5_irq_capture(void);


/*------------------------------------------------------------------------
   Vector definiton

   Use following statements to define vectors. All resource related
   vectors are predefined. Remaining software interrupts can be added here
   as well.
   NOTE: If software interrupts 0 to 7 are defined here, this might 
   conflict with the reset vector in the start-up file.
*/

#pragma intvect DefaultIRQHandler    9    /* software interrupt 9         */
#pragma intvect DefaultIRQHandler   10    /* exeception handler           */
#pragma intvect DefaultIRQHandler   11    /* CAN #0 (receive complete)    */
#pragma intvect DefaultIRQHandler   12    /* CAN #0 (transmission complete
                                               /node status transition) */
#pragma intvect DefaultIRQHandler   15    /* external interrupt INT0/INT1 */
#pragma intvect timer_irq_tick      16    /* timebase timer               */
#pragma intvect uart_irq_tx_end     17    /* 16-bit reload timer #0       */
#pragma intvect DefaultIRQHandler   18    /* A/D converter                */
#pragma intvect DefaultIRQHandler   19    /* I/O timer                    */
#pragma intvect DefaultIRQHandler   20    /* external interrupt INT2/INT3 */
#pragma intvect DefaultIRQHandler   22    /* PPG #0/1                     */
#pragma intvect DefaultIRQHandler   23    /* input capture CH.0           */
#pragma intvect DefaultIRQHandler   24    /* external interrupt INT4/INT5 */
#pragma intvect DefaultIRQHandler   25    /* input capture CH.1           */
#pragma intvect DefaultIRQHandler   26    /* PPG #2/3                     */
#pragma intvect DefaultIRQHandler   27    /* external interrupt INT6/INT7 */
#pragma intvect DefaultIRQHandler   28    /* watch timer                  */
#ifdef CFG_IR
#pragma intvect rc5_irq_capture     30    /* input capture CH.2/3         */
#else
#pragma intvect DefaultIRQHandler   30    /* input capture CH.2/3         */
#endif
#pragma intvect uart_irq_rx_timeout 36    /* 16-bit reload timer #1       */
#pragma intvect DefaultIRQHandler   37    /* UART #1 (receive complete)   */
#pragma intvect DefaultIRQHandler   38    /* UART #1 (transmission compl.)*/
#pragma intvect uart_irq_rx         39    /* UART #0 (receive complete)   */
#pragma intvect uart_irq_tx         40    /* UART #0 (transmission compl.)*/
#pragma intvect DefaultIRQHandler   41    /* Flash Memory                 */
#pragma intvect DefaultIRQHandler   42    /* delayed interrupt            */

/*------------------------------------------------------------------------
   DefaultIRQHandler()

   This function is a placeholder for all vector definitions. Either use
   your own placeholder or add necessary code here. 
*/
__interrupt 
void DefaultIRQHandler (void)
{
    __DI();                              /* disable interrupts */
    while(1)
        __wait_nop();                    /* halt system */
}
