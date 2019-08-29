/******************************************************************************

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

 ******************************************************************************/
/******************************************************************************/
/** \file main.c
 **
 ** This example using the STIMER to blink 3 LEDs at AMA3BEVB.
 ** Every LED is triggered by a different STIMER channel,
 ** so every LED is like a task managed in hardware.
 **
 ** Current consumption is around 80µA only while MCU can put to deepsleep
 ** while the STIMER is still running
 **
 ** History:
 **   - 2019-03-29  V1.0  MSc  First version

 *****************************************************************************/
#define __MAIN_C__
/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "mcu.h"
#include "base_types.h"


/*****************************************************************************/
/* Local pre-processor symbols/macros ('#define')                            */
/*****************************************************************************/
#define SYNC_READ       0x5FFF0000
#define BUS_WRITE_FLUSH()                                \
    if ( 1 )                                                            \
    {                                                                   \
        uint32_t *pui32Flush = (uint32_t*)SYNC_READ;                    \
        ui32BusWriteFlush = *pui32Flush;                              \
    }

/*****************************************************************************/
/* Global variable definitions (declared in header file with 'extern')       */
/*****************************************************************************/



/*****************************************************************************/
/* Local type definitions ('typedef')                                        */
/*****************************************************************************/


/*****************************************************************************/
/* Local variable definitions ('static')                                     */
/*****************************************************************************/
static volatile uint32_t u32Counter;  //ms counter
uint32_t ui32BusWriteFlush;


/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/
//interrupt prototype SysTick_Handler defined in startup file
//prototype main defined in startup file
void delay(uint32_t delayMs);


/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/


/**
 *****************************************************************************
 ** 
 **\brief Systick interrupt handler defined by CMSIS
 **
 *****************************************************************************/
void SysTick_Handler(void)
{
	u32Counter++;
}


/**
*********************************************************************************
** \brief delay function used with SysTick IRQ
**  
** \param [in] delayMs Delay in ms
**  
** \details delay function used with SysTick IRQ
**  
*********************************************************************************/
void delay(uint32_t delayMs)
{
    uint32_t u32End = u32Counter;
    u32End += delayMs;
    while(u32End != u32Counter) __NOP();
}

/**
*********************************************************************************
** \brief Go to deepsleep
**  
*********************************************************************************/
void Deepsleep(void)
{
    //
    // Before executing WFI, flush any buffered core and peripheral writes.
    //
    __DSB();

    BUS_WRITE_FLUSH();

    SCB->SCR |= _VAL2FLD(SCB_SCR_SLEEPDEEP, 1);
    __WFI();

    //
    // Upon wake, execute the Instruction Sync Barrier instruction.
    //
    __ISB();
}


/**
*********************************************************************************
** \brief STIMER CMPR0 interrupt handler
**  
*********************************************************************************/
void STIMER_CMPR0_IRQHandler(void)
{
    CTIMER->STMINTCLR_b.COMPAREA = 1;
    //
    // Slot 0
    // Next wakeup in ms (250ms)
    //                      |
    CTIMER->SCMPR0 =      (250) * (SystemCoreClock / 16) * 1e-3;

    GPIO->WTA = GPIO->WTA ^ (1 << 17); //toggle LED D5
}

/**
*********************************************************************************
** \brief STIMER CMPR1 interrupt handler
**  
*********************************************************************************/
void STIMER_CMPR1_IRQHandler(void)
{
    CTIMER->STMINTCLR_b.COMPAREB = 1;
    //
    // Slot 1
    // Next wakeup in ms (500ms)
    //                      |
    CTIMER->SCMPR1 =      (500) * (SystemCoreClock / 16) * 1e-3;

    GPIO->WTA = GPIO->WTA ^ (1 << 14); //toggle LED D6
}

/**
*********************************************************************************
** \brief STIMER CMPR2 interrupt handler
**  
*********************************************************************************/
void STIMER_CMPR2_IRQHandler(void)
{
    CTIMER->STMINTCLR_b.COMPAREC = 1;
    //
    // Slot 2
    // Next wakeup in ms (1000ms)
    //                      |
    CTIMER->SCMPR2 =     (1000) * (SystemCoreClock / 16) * 1e-3;

    GPIO->WTA = GPIO->WTA ^ (1 << 15); //toggle LED D7
}

/**
*********************************************************************************
** \brief Main
**  
*********************************************************************************/
int main(void)
{
    SystemCoreClockUpdate();                //update clock variable SystemCoreClock (defined by CMSIS)
    SysTick_Config(SystemCoreClock / 1000); //setup 1ms SysTick (defined by CMSIS)
    //application initialization area

    GPIO->PADKEY = 0x00000073;            //unlock pin selection
    GPIO->CFGC_b.GPIO17OUTCFG = 1;        //output is push-pull LED D5
    GPIO->CFGB_b.GPIO14OUTCFG = 1;        //output is push-pull LED D6
    GPIO->CFGB_b.GPIO15OUTCFG = 1;        //output is push-pull LED D7
    GPIO->CFGD_b.GPIO30OUTCFG = 1;        //output is push-pull LED D8
    GPIO->CFGB_b.GPIO10OUTCFG = 1;        //output is push-pull LED D9
    GPIO->PADKEY = 0;

    //
    // Stop STIMER
    //
    CTIMER->STCFG_b.CLEAR = 1;           
    CTIMER->STCFG_b.FREEZE = 1;

    CTIMER->STCFG_b.CLKSEL = CTIMER_STCFG_CLKSEL_HFRC_DIV16;
    SystemCoreClockUpdate();

    //
    // Slot 0
    // Next wakeup in ms (250ms)
    //                      |
    CTIMER->SCMPR0 =      (250) * (SystemCoreClock / 16) * 1e-3;
    CTIMER->STCFG_b.COMPARE_A_EN = 1;
    CTIMER->STMINTEN_b.COMPAREA = 1;
    
    //
    // Slot 1
    // Next wakeup in ms (500ms)
    //                      |
    CTIMER->SCMPR1 =      (500) * (SystemCoreClock / 16) * 1e-3;
    CTIMER->STCFG_b.COMPARE_B_EN = 1;
    CTIMER->STMINTEN_b.COMPAREB = 1;

    //
    // Slot 2
    // Next wakeup in ms (1000ms)
    //                      |
    CTIMER->SCMPR2 =      (1000) * (SystemCoreClock / 16) * 1e-3;
    CTIMER->STCFG_b.COMPARE_C_EN = 1;
    CTIMER->STMINTEN_b.COMPAREC = 1;
    
    //
    // Setup NVIC
    //

    //slot 0
    NVIC_ClearPendingIRQ(STIMER_CMPR0_IRQn); //clear pending flag
    NVIC_EnableIRQ(STIMER_CMPR0_IRQn); //enable IRQ
    NVIC_SetPriority(STIMER_CMPR0_IRQn,1); //set priority, smaller value means higher priority

    //slot 1
    NVIC_ClearPendingIRQ(STIMER_CMPR1_IRQn); //clear pending flag
    NVIC_EnableIRQ(STIMER_CMPR1_IRQn); //enable IRQ
    NVIC_SetPriority(STIMER_CMPR1_IRQn,1); //set priority, smaller value means higher priority

    //slot 2
    NVIC_ClearPendingIRQ(STIMER_CMPR2_IRQn); //clear pending flag
    NVIC_EnableIRQ(STIMER_CMPR2_IRQn); //enable IRQ
    NVIC_SetPriority(STIMER_CMPR2_IRQn,1); //set priority, smaller value means higher priority

    //
    // Start STIMER
    //
    CTIMER->STCFG_b.CLEAR = 0;
    CTIMER->STCFG_b.FREEZE = 0;

    //
    // Disable unused peripherals
    //

    //disable RTC / 32KHz oscillator
    RTC->RTCCTL_b.RSTOP = 1;

    //disable flash 1
    PWRCTRL->MEMPWREN_b.FLASH1 = 0;  

    while(1)
    {
        //application code

        Deepsleep();
    }
}



/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/

