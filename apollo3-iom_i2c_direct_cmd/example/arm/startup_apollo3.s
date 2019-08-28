;/*******************************************************************************
;*                                                                              *
;*   Abstract    : This file contains interrupt vector and startup code.        *
;*                                                                              *
;*   Functions   : Reset_Handler                                                *
;*                                                                              *
;*   Target      : microcontrollers                                             *
;*                                                                              *
;*   Environment : KEIL µVISION	                                                *
;*                                                                              *
;*   Distribution: The file is distributed "as is," without any warranty        *
;*                 of any kind.                                                 *
;*                                                                              *
;********************************************************************************
;
;Copyright (C) 2013-2017, Fujitsu Electronics Europe GmbH or a               
;subsidiary of Fujitsu Electronics Europe GmbH.  All rights reserved.        
;                                                                            
;This software, including source code, documentation and related             
;materials ("Software"), is owned by Fujitsu Electronics Europe GmbH or      
;one of its subsidiaries ("Fujitsu").
;                                                                            
;If no EULA applies, Fujitsu hereby grants you a personal, non-exclusive,    
;non-transferable license to copy, modify, and compile the                   
;Software source code solely for use in connection with Fujitsu's            
;integrated circuit products.  Any reproduction, modification, translation,  
;compilation, or representation of this Software except as specified         
;above is prohibited without the express written permission of Fujitsu.      
;                                                                            
;Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO                        
;WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING,                        
;BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED                                
;WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A                             
;PARTICULAR PURPOSE. Fujitsu reserves the right to make                      
;changes to the Software without notice. Fujitsu does not assume any         
;liability arising out of the application or use of the Software or any      
;product or circuit described in the Software. Fujitsu does not              
;authorize its products for use in any products where a malfunction or       
;failure of the Fujitsu product may reasonably be expected to result in      
;significant property damage, injury or death ("High Risk Product"). By      
;including Fujitsu's product in a High Risk Product, the manufacturer        
;of such system or application assumes all risk of such use and in doing     
;so agrees to indemnify Fujitsu against all liability.                       
;

; Stack Configuration
;  Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>

Stack_Size      EQU     0x00000400

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; Heap Configuration
;  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>

Heap_Size       EQU     0x00000800

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     MemManage_Handler         ; MPU Fault Handler
                DCD     BusFault_Handler          ; Bus Fault Handler
                DCD     UsageFault_Handler        ; Usage Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     DebugMon_Handler          ; Debug Monitor Handler
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

; Numbered IRQ handler vectors				
				
                DCD     BROWNOUT_IRQHandler		; IRQ #0
                DCD     WDT_IRQHandler		; IRQ #1
                DCD     RTC_IRQHandler		; IRQ #2
                DCD     VCOMP_IRQHandler		; IRQ #3
                DCD     IOSLAVE_IRQHandler		; IRQ #4
                DCD     IOSLAVEACC_IRQHandler		; IRQ #5
                DCD     IOMSTR0_IRQHandler		; IRQ #6
                DCD     IOMSTR1_IRQHandler		; IRQ #7
                DCD     IOMSTR2_IRQHandler		; IRQ #8
                DCD     IOMSTR3_IRQHandler		; IRQ #9
                DCD     IOMSTR4_IRQHandler		; IRQ #10
                DCD     IOMSTR5_IRQHandler		; IRQ #11
                DCD     BLE_IRQHandler		; IRQ #12
                DCD     GPIO_IRQHandler		; IRQ #13
                DCD     CTIMER_IRQHandler		; IRQ #14
                DCD     UART0_IRQHandler		; IRQ #15
                DCD     UART1_IRQHandler		; IRQ #16
                DCD     SCARD_IRQHandler		; IRQ #17
                DCD     ADC_IRQHandler		; IRQ #18
                DCD     PDM_IRQHandler		; IRQ #19
                DCD     MSPI_IRQHandler		; IRQ #20
                DCD     Dummy		; IRQ #21
                DCD     STIMER_IRQHandler		; IRQ #22
                DCD     STIMER_CMPR0_IRQHandler		; IRQ #23
                DCD     STIMER_CMPR1_IRQHandler		; IRQ #24
                DCD     STIMER_CMPR2_IRQHandler		; IRQ #25
                DCD     STIMER_CMPR3_IRQHandler		; IRQ #26
                DCD     STIMER_CMPR4_IRQHandler		; IRQ #27
                DCD     STIMER_CMPR5_IRQHandler		; IRQ #28
                DCD     STIMER_CMPR6_IRQHandler		; IRQ #29
                DCD     STIMER_CMPR7_IRQHandler		; IRQ #30
                DCD     CLKGEN_IRQHandler		; IRQ #31


__Vectors_End

__Vectors_Size 	EQU 	__Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  __main

                ; Check for bootloader configuration. If yes, relocate SCB->VTOR to __Vectors
                LDR.W    R0, =__Vectors     
                CMP      R0,#0                   ; Check vector table is at 0x00000000
                BEQ.N    _end_vector_relocation  ; If not a 0x00000000 continue initialization
                ; relocate vector offset
                LDR.W    R1,=0x1FFFFF80          ; AND __vector_table with 0x1FFFFF80         
                ANDS     R0,R0,R1                ; AND __vector_table with 0x1FFFFF80
                LDR.W    R1, =0xE000ED08         ; Load SCB->VTOR Address
                STR      R0,[R1]                 ; Store new vector table offset
_end_vector_relocation  
                LDR.W R0, =0xE000ED88            ; CPACR is located at address 0xE000ED88
                LDR R1, [R0]                     ; Read CPACR
                ORR R1, R1, #(0xF << 20)         ; Set bits 20-23 to enable CP10 and CP11 coprocessors
                STR R1, [R0]                     ; Write back the modified value to the CPACR

                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__main
                BX      R0
                ENDP


; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler         [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler          [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler        [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler          [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP

Default_Handler PROC

                EXPORT  BROWNOUT_IRQHandler    [WEAK]
                EXPORT  WDT_IRQHandler    [WEAK]
                EXPORT  RTC_IRQHandler    [WEAK]
                EXPORT  VCOMP_IRQHandler    [WEAK]
                EXPORT  IOSLAVE_IRQHandler    [WEAK]
                EXPORT  IOSLAVEACC_IRQHandler    [WEAK]
                EXPORT  IOMSTR0_IRQHandler    [WEAK]
                EXPORT  IOMSTR1_IRQHandler    [WEAK]
                EXPORT  IOMSTR2_IRQHandler    [WEAK]
                EXPORT  IOMSTR3_IRQHandler    [WEAK]
                EXPORT  IOMSTR4_IRQHandler    [WEAK]
                EXPORT  IOMSTR5_IRQHandler    [WEAK]
                EXPORT  BLE_IRQHandler    [WEAK]
                EXPORT  GPIO_IRQHandler    [WEAK]
                EXPORT  CTIMER_IRQHandler    [WEAK]
                EXPORT  UART0_IRQHandler    [WEAK]
                EXPORT  UART1_IRQHandler    [WEAK]
                EXPORT  SCARD_IRQHandler    [WEAK]
                EXPORT  ADC_IRQHandler    [WEAK]
                EXPORT  PDM_IRQHandler    [WEAK]
                EXPORT  MSPI_IRQHandler    [WEAK]
                EXPORT  STIMER_IRQHandler    [WEAK]
                EXPORT  STIMER_CMPR0_IRQHandler    [WEAK]
                EXPORT  STIMER_CMPR1_IRQHandler    [WEAK]
                EXPORT  STIMER_CMPR2_IRQHandler    [WEAK]
                EXPORT  STIMER_CMPR3_IRQHandler    [WEAK]
                EXPORT  STIMER_CMPR4_IRQHandler    [WEAK]
                EXPORT  STIMER_CMPR5_IRQHandler    [WEAK]
                EXPORT  STIMER_CMPR6_IRQHandler    [WEAK]
                EXPORT  STIMER_CMPR7_IRQHandler    [WEAK]
                EXPORT  CLKGEN_IRQHandler    [WEAK]

                
                EXPORT  Dummy	          [WEAK]

BROWNOUT_IRQHandler		; 0  BROWNOUT                                                               
WDT_IRQHandler		; 1  WDT                                                                    
RTC_IRQHandler		; 2  RTC                                                                    
VCOMP_IRQHandler		; 3  VCOMP                                                                  
IOSLAVE_IRQHandler		; 4  IOSLAVE                                                                
IOSLAVEACC_IRQHandler		; 5  IOSLAVEACC                                                             
IOMSTR0_IRQHandler		; 6  IOMSTR0                                                                
IOMSTR1_IRQHandler		; 7  IOMSTR1                                                                
IOMSTR2_IRQHandler		; 8  IOMSTR2                                                                
IOMSTR3_IRQHandler		; 9  IOMSTR3                                                                
IOMSTR4_IRQHandler		; 10 IOMSTR4                                                                
IOMSTR5_IRQHandler		; 11 IOMSTR5                                                                
BLE_IRQHandler		; 12 BLE                                                                    
GPIO_IRQHandler		; 13 GPIO                                                                   
CTIMER_IRQHandler		; 14 CTIMER                                                                 
UART0_IRQHandler		; 15 UART0                                                                  
UART1_IRQHandler		; 16 UART1                                                                  
SCARD_IRQHandler		; 17 SCARD                                                                  
ADC_IRQHandler		; 18 ADC                                                                    
PDM_IRQHandler		; 19 PDM                                                                    
MSPI_IRQHandler		; 20 MSPI                                                                   
STIMER_IRQHandler		; 22 STIMER                                                                 
STIMER_CMPR0_IRQHandler		; 23 STIMER_CMPR0                                                           
STIMER_CMPR1_IRQHandler		; 24 STIMER_CMPR1                                                           
STIMER_CMPR2_IRQHandler		; 25 STIMER_CMPR2                                                           
STIMER_CMPR3_IRQHandler		; 26 STIMER_CMPR3                                                           
STIMER_CMPR4_IRQHandler		; 27 STIMER_CMPR4                                                           
STIMER_CMPR5_IRQHandler		; 28 STIMER_CMPR5                                                           
STIMER_CMPR6_IRQHandler		; 29 STIMER_CMPR6                                                           
STIMER_CMPR7_IRQHandler		; 30 STIMER_CMPR7                                                           
CLKGEN_IRQHandler		; 31 CLKGEN                                                                 

Dummy


                B       .

                ENDP


                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB
                
                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit
                
                ELSE
                
                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap

                LDR     R0, = Heap_Mem
                LDR     R1, = (Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem + Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF


                END
