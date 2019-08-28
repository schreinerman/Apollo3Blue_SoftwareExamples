/*******************************************************************************
*                                                                              * 
*   Abstract    : This file contains interrupt vector and startup code.        *
*                                                                              *
*   Functions   : Reset_Handler                                                *
*                                                                              *
*   Target      : microcontrollers                                                   *
*                                                                              * 
*   Environment : GNU Toolchain(Make)                                          *
*                                                                              *
*   Distribution: The file is distributed "as is," without any warranty        *
*                 of any kind.                                                 *
*                                                                              *
*******************************************************************************/

/*
Copyright (C) 2013-2017, Fujitsu Electronics Europe GmbH or a               
subsidiary of Fujitsu Electronics Europe GmbH.  All rights reserved.        
                                                                            
This software, including source code, documentation and related             
materials ("Software"), is owned by Fujitsu Electronics Europe GmbH or      
one of its subsidiaries ("Fujitsu").
                                                                            
If no EULA applies, Fujitsu hereby grants you a personal, non-exclusive,    
non-transferable license to copy, modify, and compile the                   
Software source code solely for use in connection with Fujitsu's            
integrated circuit products.  Any reproduction, modification, translation,  
compilation, or representation of this Software except as specified         
above is prohibited without the express written permission of Fujitsu.      
                                                                            
Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO                        
WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING,                        
BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED                                
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A                             
PARTICULAR PURPOSE. Fujitsu reserves the right to make                      
changes to the Software without notice. Fujitsu does not assume any         
liability arising out of the application or use of the Software or any      
product or circuit described in the Software. Fujitsu does not              
authorize its products for use in any products where a malfunction or       
failure of the Fujitsu product may reasonably be expected to result in      
significant property damage, injury or death ("High Risk Product"). By      
including Fujitsu's product in a High Risk Product, the manufacturer        
of such system or application assumes all risk of such use and in doing     
so agrees to indemnify Fujitsu against all liability.                       

*/


/*
// <h> Stack Configuration
//   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
// </h>
*/

 .equ    Stack_Size, 0x00000400
    .section ".stack", "w"
    .align  3
    .globl  __FM_stack_mem
    .globl  __FM_stack_size
__MCU_stack_mem:
    .if     Stack_Size
    .space  Stack_Size
    .endif
    .size   __MCU_stack_mem,  . - __MCU_stack_mem
    .set    __MCU_stack_size, . - __MCU_stack_mem


/*
// <h> Heap Configuration
//   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
// </h>
*/

    .equ    Heap_Size,  0x00000800
    
    .section ".heap", "w"
    .align  3
    .globl  __MCU_heap_start
    .globl  __MCU_heap_end
__MCU_heap_start:
    .if     Heap_Size
    .space  Heap_Size
    .endif
__MCU_heap_end:


/* Vector Table */

    .section ".MCU.interrupt_vector"
    .globl  __MCU_interrupt_vector_cortex_m
    .type   __MCU_interrupt_vector_cortex_m, %object

__MCU_interrupt_vector_cortex_m:
    .long   __MCU_stack                 /* Top of Stack                 */
    .long   __MCU_reset                 /* Reset Handler                */
    .long   NMI_Handler                 /* NMI Handler                  */
    .long   HardFault_Handler           /* Hard Fault Handler           */
    .long   MemManage_Handler           /* MPU Fault Handler            */
    .long   BusFault_Handler            /* Bus Fault Handler            */
    .long   UsageFault_Handler          /* Usage Fault Handler          */
    .long   0                           /* Reserved                     */
    .long   0                           /* Reserved                     */
    .long   0                           /* Reserved                     */
    .long   0                           /* Reserved                     */
    .long   SVC_Handler                 /* SVCall Handler               */
    .long   DebugMon_Handler            /* Debug Monitor Handler        */
    .long   0                           /* Reserved                     */
    .long   PendSV_Handler              /* PendSV Handler               */
    .long   SysTick_Handler             /* SysTick Handler              */

    /* External Interrupts */

    .long   BROWNOUT_IRQHandler                     /*  0  BROWNOUT                                                                */
    .long   WDT_IRQHandler                          /*  1  WDT                                                                     */
    .long   RTC_IRQHandler                          /*  2  RTC                                                                     */
    .long   VCOMP_IRQHandler                        /*  3  VCOMP                                                                   */
    .long   IOSLAVE_IRQHandler                      /*  4  IOSLAVE                                                                 */
    .long   IOSLAVEACC_IRQHandler                   /*  5  IOSLAVEACC                                                              */
    .long   IOMSTR0_IRQHandler                      /*  6  IOMSTR0                                                                 */
    .long   IOMSTR1_IRQHandler                      /*  7  IOMSTR1                                                                 */
    .long   IOMSTR2_IRQHandler                      /*  8  IOMSTR2                                                                 */
    .long   IOMSTR3_IRQHandler                      /*  9  IOMSTR3                                                                 */
    .long   IOMSTR4_IRQHandler                      /*  10 IOMSTR4                                                                 */
    .long   IOMSTR5_IRQHandler                      /*  11 IOMSTR5                                                                 */
    .long   BLE_IRQHandler                          /*  12 BLE                                                                     */
    .long   GPIO_IRQHandler                         /*  13 GPIO                                                                    */
    .long   CTIMER_IRQHandler                       /*  14 CTIMER                                                                  */
    .long   UART0_IRQHandler                        /*  15 UART0                                                                   */
    .long   UART1_IRQHandler                        /*  16 UART1                                                                   */
    .long   SCARD_IRQHandler                        /*  17 SCARD                                                                   */
    .long   ADC_IRQHandler                          /*  18 ADC                                                                     */
    .long   PDM_IRQHandler                          /*  19 PDM                                                                     */
    .long   MSPI_IRQHandler                         /*  20 MSPI                                                                    */
    .long   Default_Handler		/* IRQ #21 */
    .long   STIMER_IRQHandler                       /*  22 STIMER                                                                  */
    .long   STIMER_CMPR0_IRQHandler                 /*  23 STIMER_CMPR0                                                            */
    .long   STIMER_CMPR1_IRQHandler                 /*  24 STIMER_CMPR1                                                            */
    .long   STIMER_CMPR2_IRQHandler                 /*  25 STIMER_CMPR2                                                            */
    .long   STIMER_CMPR3_IRQHandler                 /*  26 STIMER_CMPR3                                                            */
    .long   STIMER_CMPR4_IRQHandler                 /*  27 STIMER_CMPR4                                                            */
    .long   STIMER_CMPR5_IRQHandler                 /*  28 STIMER_CMPR5                                                            */
    .long   STIMER_CMPR6_IRQHandler                 /*  29 STIMER_CMPR6                                                            */
    .long   STIMER_CMPR7_IRQHandler                 /*  30 STIMER_CMPR7                                                            */
    .long   CLKGEN_IRQHandler                       /*  31 CLKGEN                                                                  */


    .size   __MCU_interrupt_vector_cortex_m, . - __MCU_interrupt_vector_cortex_m


    .thumb


/* Reset Handler */

    .section .MCU.reset,"x",%progbits
    .thumb_func
    .globl  __MCU_reset_cortex_m
    .type   __MCU_reset_cortex_m, %function
__MCU_reset_cortex_m:
    .fnstart
.ifdef Debug_RAM
/* this routine is used for .bss sections initialization by RAM debugging "Debug_RAM = 1" */
/* Clear .bss section (Zero init) */
 MOV     R0, #0
 LDR     R1, =_sbss
 LDR     R2, =_ebss
 CMP     R1,R2
 BEQ     start_main
Loop_bss:
 CMP     R1, R2
 BHS  start_main
 STR    R0, [R1]
 ADD  R1, #4
 BLO     Loop_bss

.else
/* this routine is used for .data and .bss sections initialization by ROM debugging "Debug_RAM = 0" */
init_data:
 LDR  R0, =_sdata
 LDR  R1, =_etext
 LDR  R2, =data_size
 MOV  R3, #0
loop_init_data:
 CMP  R3, R2
 BCS  zero_bss
    LDR  R4,[R1,#0]
 STR  R4,[R0,#0]
 ADD  R3, #4
 ADD  R1, #4
 ADD  R0, #4
 B  loop_init_data

/* Clear .bss section (Zero init) */

zero_bss:
 LDR  R0, =_sbss
 LDR  R2, =bss_size
 MOV  R3, #0
 MOV  R4, #0
loop_zero_bss:
 CMP  R3, R2
 BCS  start_main
 STR  R4,[R0,#0]
 ADD  R3, #4
 ADD  R0, #4
 B  loop_zero_bss
.endif
start_main:

  /*enable fpu begin*/
  /*; enable cp10,cp11 */
  
  
  ldr     r0, =0xe000ed88           
  ldr     r1,[r0]
  ldr     r2, =0xf00000
  orr     r1,r1,r2
  str     r1,[r0]
  
  
  /*enable fpu end*/ 
  
 LDR     R0, =SystemInit
    BLX     R0
    LDR     R0,=main
    BX      R0

    .pool
    .cantunwind
    .fnend
    .size   __MCU_reset_cortex_m,.-__MCU_reset_cortex_m

    .section ".text"

/* Exception Handlers */

    .weak   NMI_Handler
    .type   NMI_Handler, %function
NMI_Handler:
    B       .
    .size   NMI_Handler, . - NMI_Handler

    .weak   HardFault_Handler
    .type   HardFault_Handler, %function
HardFault_Handler:
    B       .
    .size   HardFault_Handler, . - HardFault_Handler

    .weak   MemManage_Handler
    .type   MemManage_Handler, %function
MemManage_Handler:
    B       .
    .size   MemManage_Handler, . - MemManage_Handler

    .weak   BusFault_Handler
    .type   BusFault_Handler, %function
BusFault_Handler:
    B       .
    .size   BusFault_Handler, . - BusFault_Handler

    .weak   UsageFault_Handler
    .type   UsageFault_Handler, %function
UsageFault_Handler:
    B       .
    .size   UsageFault_Handler, . - UsageFault_Handler

    .weak   SVC_Handler
    .type   SVC_Handler, %function
SVC_Handler:
    B       .
    .size   SVC_Handler, . - SVC_Handler

    .weak   DebugMon_Handler
    .type   DebugMon_Handler, %function
DebugMon_Handler:
    B       .
    .size   DebugMon_Handler, . - DebugMon_Handler

    .weak   PendSV_Handler
    .type   PendSV_Handler, %function
PendSV_Handler:
    B       .
    .size   PendSV_Handler, . - PendSV_Handler

    .weak   SysTick_Handler
    .type   SysTick_Handler, %function
SysTick_Handler:
    B       .
    .size   SysTick_Handler, . - SysTick_Handler


/* IRQ Handlers */

    .globl  Default_Handler
    .type   Default_Handler, %function
Default_Handler:
    B       .
    .size   Default_Handler, . - Default_Handler

    .macro  IRQ handler
    .weak   \handler
    .set    \handler, Default_Handler
    .endm

	IRQ     BROWNOUT_IRQHandler		/*  0  BROWNOUT                                                                */
	IRQ     WDT_IRQHandler		/*  1  WDT                                                                     */
	IRQ     RTC_IRQHandler		/*  2  RTC                                                                     */
	IRQ     VCOMP_IRQHandler		/*  3  VCOMP                                                                   */
	IRQ     IOSLAVE_IRQHandler		/*  4  IOSLAVE                                                                 */
	IRQ     IOSLAVEACC_IRQHandler		/*  5  IOSLAVEACC                                                              */
	IRQ     IOMSTR0_IRQHandler		/*  6  IOMSTR0                                                                 */
	IRQ     IOMSTR1_IRQHandler		/*  7  IOMSTR1                                                                 */
	IRQ     IOMSTR2_IRQHandler		/*  8  IOMSTR2                                                                 */
	IRQ     IOMSTR3_IRQHandler		/*  9  IOMSTR3                                                                 */
	IRQ     IOMSTR4_IRQHandler		/*  10 IOMSTR4                                                                 */
	IRQ     IOMSTR5_IRQHandler		/*  11 IOMSTR5                                                                 */
	IRQ     BLE_IRQHandler		/*  12 BLE                                                                     */
	IRQ     GPIO_IRQHandler		/*  13 GPIO                                                                    */
	IRQ     CTIMER_IRQHandler		/*  14 CTIMER                                                                  */
	IRQ     UART0_IRQHandler		/*  15 UART0                                                                   */
	IRQ     UART1_IRQHandler		/*  16 UART1                                                                   */
	IRQ     SCARD_IRQHandler		/*  17 SCARD                                                                   */
	IRQ     ADC_IRQHandler		/*  18 ADC                                                                     */
	IRQ     PDM_IRQHandler		/*  19 PDM                                                                     */
	IRQ     MSPI_IRQHandler		/*  20 MSPI                                                                    */
	IRQ     STIMER_IRQHandler		/*  22 STIMER                                                                  */
	IRQ     STIMER_CMPR0_IRQHandler		/*  23 STIMER_CMPR0                                                            */
	IRQ     STIMER_CMPR1_IRQHandler		/*  24 STIMER_CMPR1                                                            */
	IRQ     STIMER_CMPR2_IRQHandler		/*  25 STIMER_CMPR2                                                            */
	IRQ     STIMER_CMPR3_IRQHandler		/*  26 STIMER_CMPR3                                                            */
	IRQ     STIMER_CMPR4_IRQHandler		/*  27 STIMER_CMPR4                                                            */
	IRQ     STIMER_CMPR5_IRQHandler		/*  28 STIMER_CMPR5                                                            */
	IRQ     STIMER_CMPR6_IRQHandler		/*  29 STIMER_CMPR6                                                            */
	IRQ     STIMER_CMPR7_IRQHandler		/*  30 STIMER_CMPR7                                                            */
	IRQ     CLKGEN_IRQHandler		/*  31 CLKGEN                                                                  */


    .end
