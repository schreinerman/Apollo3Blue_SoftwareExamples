// SEGGER Embedded Studio, runtime support.
//
// Copyright (c) 2014-2018 SEGGER Microcontroller GmbH & Co KG
// Copyright (c) 2001-2018 Rowley Associates Limited.
//
// This file may be distributed under the terms of the License Agreement
// provided with this software.
//
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

.macro ISR_HANDLER name=
  .section .vectors, "ax"
  .word \name
  .section .init, "ax"
  .thumb_func
  .weak \name
\name:
1: b 1b /* endless loop */
.endm

.macro ISR_RESERVED
  .section .vectors, "ax"
  .word 0
.endm

  .syntax unified
  .global reset_handler

  .section .vectors, "ax"
  .code 16 
  .global _vectors

.macro DEFAULT_ISR_HANDLER name=
  .thumb_func
  .weak \name
\name:
1: b 1b /* endless loop */
.endm

_vectors:
  .word __stack_end__
  .word reset_handler
ISR_HANDLER NMI_Handler                 /* NMI Handler                  */
ISR_HANDLER HardFault_Handler           /* Hard Fault Handler           */
ISR_HANDLER MemManage_Handler           /* MPU Fault Handler            */
ISR_HANDLER BusFault_Handler            /* Bus Fault Handler            */
ISR_HANDLER UsageFault_Handler          /* Usage Fault Handler          */
ISR_RESERVED
ISR_RESERVED
ISR_RESERVED
ISR_RESERVED
ISR_HANDLER SVC_Handler                 /* SVCall Handler               */
ISR_HANDLER DebugMon_Handler            /* Debug Monitor Handler        */
ISR_RESERVED
ISR_HANDLER PendSV_Handler              /* PendSV Handler               */
ISR_HANDLER SysTick_Handler             /* SysTick Handler              */

    /* External Interrupts */

ISR_HANDLER   BROWNOUT_IRQHandler                     /*  0  BROWNOUT                                                                */
ISR_HANDLER   WDT_IRQHandler                          /*  1  WDT                                                                     */
ISR_HANDLER   RTC_IRQHandler                          /*  2  RTC                                                                     */
ISR_HANDLER   VCOMP_IRQHandler                        /*  3  VCOMP                                                                   */
ISR_HANDLER   IOSLAVE_IRQHandler                      /*  4  IOSLAVE                                                                 */
ISR_HANDLER   IOSLAVEACC_IRQHandler                   /*  5  IOSLAVEACC                                                              */
ISR_HANDLER   IOMSTR0_IRQHandler                      /*  6  IOMSTR0                                                                 */
ISR_HANDLER   IOMSTR1_IRQHandler                      /*  7  IOMSTR1                                                                 */
ISR_HANDLER   IOMSTR2_IRQHandler                      /*  8  IOMSTR2                                                                 */
ISR_HANDLER   IOMSTR3_IRQHandler                      /*  9  IOMSTR3                                                                 */
ISR_HANDLER   IOMSTR4_IRQHandler                      /*  10 IOMSTR4                                                                 */
ISR_HANDLER   IOMSTR5_IRQHandler                      /*  11 IOMSTR5                                                                 */
ISR_HANDLER   BLE_IRQHandler                          /*  12 BLE                                                                     */
ISR_HANDLER   GPIO_IRQHandler                         /*  13 GPIO                                                                    */
ISR_HANDLER   CTIMER_IRQHandler                       /*  14 CTIMER                                                                  */
ISR_HANDLER   UART0_IRQHandler                        /*  15 UART0                                                                   */
ISR_HANDLER   UART1_IRQHandler                        /*  16 UART1                                                                   */
ISR_HANDLER   SCARD_IRQHandler                        /*  17 SCARD                                                                   */
ISR_HANDLER   ADC_IRQHandler                          /*  18 ADC                                                                     */
ISR_HANDLER   PDM_IRQHandler                          /*  19 PDM                                                                     */
ISR_HANDLER   MSPI_IRQHandler                         /*  20 MSPI                                                                    */
ISR_RESERVED    		/* IRQ #21            */
ISR_HANDLER   STIMER_IRQHandler                       /*  22 STIMER                                                                  */
ISR_HANDLER   STIMER_CMPR0_IRQHandler                 /*  23 STIMER_CMPR0                                                            */
ISR_HANDLER   STIMER_CMPR1_IRQHandler                 /*  24 STIMER_CMPR1                                                            */
ISR_HANDLER   STIMER_CMPR2_IRQHandler                 /*  25 STIMER_CMPR2                                                            */
ISR_HANDLER   STIMER_CMPR3_IRQHandler                 /*  26 STIMER_CMPR3                                                            */
ISR_HANDLER   STIMER_CMPR4_IRQHandler                 /*  27 STIMER_CMPR4                                                            */
ISR_HANDLER   STIMER_CMPR5_IRQHandler                 /*  28 STIMER_CMPR5                                                            */
ISR_HANDLER   STIMER_CMPR6_IRQHandler                 /*  29 STIMER_CMPR6                                                            */
ISR_HANDLER   STIMER_CMPR7_IRQHandler                 /*  30 STIMER_CMPR7                                                            */
ISR_HANDLER   CLKGEN_IRQHandler                       /*  31 CLKGEN                                                                  */


  .section .vectors, "ax"
_vectors_end:

  .section .init, "ax"
  .thumb_func

  reset_handler:
  // Check for bootloader configuration. If yes, relocate SCB->VTOR to __vector_table
  LDR.W    R0, =_vectors     
  CMP      R0,#0                   // Check vector table is at 0x00000000
  BEQ.N    _end_vector_relocation  // If not a 0x00000000 continue initialization
  // relocate vector offset
  LDR.W    R1,=0x1FFFFF80          // AND __vector_table with 0x1FFFFF80         
  ANDS     R0,R0,R1                // AND __vector_table with 0x1FFFFF80
  LDR.W    R1, =0xE000ED08         // Load SCB->VTOR Address
  STR      R0,[R1]                 // Store new vector table offset

_end_vector_relocation:
#ifndef __NO_SYSTEM_INIT
  ldr r0, =__SRAM_segment_end__
  mov sp, r0
  bl SystemInit
#endif

#if !defined(__SOFTFP__)
  // Enable CP11 and CP10 with CPACR |= (0xf<<20)
  movw r0, 0xED88
  movt r0, 0xE000
  ldr r1, [r0]
  orrs r1, r1, #(0xf << 20)
  str r1, [r0]
#endif

  b _start

#ifndef __NO_SYSTEM_INIT
  .thumb_func
  .weak SystemInit
SystemInit:
  bx lr
#endif
