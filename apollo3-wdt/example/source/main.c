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
 ** This example demonstrates the watchdog peripheral with use
 ** of AMA3BEVBs LEDs. 
 **
 ** In normal operation, the application is
 ** executing a binary counter updated every 500ms.
 **
 ** After 5 seconds an application stall is simulated,
 ** with interrupt generated after 2 seconds flickering 
 ** LED9. After additional 1 second the WDT will reset.
 **
 **
 ** History:
 **   - 2019-08-29  V1.0  MSc  Automatic Created

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
** \brief Watchdog interrupt handler
**  
*********************************************************************************/
void WDT_IRQHandler(void)
{
    int i;
    //
    // Flickering LED D9 to show WDT interrupt
    //
    while(1)
    {
        GPIO->WTSA = (1 << 10);
        for(i = 0;i < 100000;i++) __NOP();
        GPIO->WTCA = (1 << 10);
        for(i = 0;i < 100000;i++) __NOP();
    }
}

/**
*********************************************************************************
** \brief Set LEDs
**  
*********************************************************************************/
void SetLeds(uint8_t value)
{
    if (value & (1 << 4))
    {
        GPIO->WTSA = (1 << 10);
    } else
    {
        GPIO->WTCA = (1 << 10);
    }
    if (value & (1 << 3))
    {
        GPIO->WTSA = (1 << 30);
    } else
    {
        GPIO->WTCA = (1 << 30);
    }
    if (value & (1 << 2))
    {
        GPIO->WTSA = (1 << 15);
    } else
    {
        GPIO->WTCA = (1 << 15);
    }
    if (value & (1 << 1))
    {
        GPIO->WTSA = (1 << 14);
    } else
    {
        GPIO->WTCA = (1 << 14);
    }
    if (value & (1 << 0))
    {
        GPIO->WTSA = (1 << 17);
    } else
    {
        GPIO->WTCA = (1 << 17);
    }
}

/**
*********************************************************************************
** \brief Main
**  
*********************************************************************************/
int main(void)
{
    volatile uint8_t u8Counter = 0;
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

    SetLeds(0xFF); //enable all LEDs for 500ms to show reset condition
    delay(500);

    WDT->CFG_b.CLKSEL = WDT_CFG_CLKSEL_1HZ; //set 1Hz WDT clock
    WDT->CFG_b.INTVAL = 2;   //generate interrupt after 2 seconds
    WDT->CFG_b.RESVAL = 3;   //generate reset after 3 seconds
    WDT->CFG_b.RESEN = 1;    //enable reset generation (limit set in RESVAL)
    WDT->CFG_b.INTEN = 1;    //enable interrupt generation (limit set in INTVAL)
    WDT->INTEN_b.WDTINT = 1; //enable interrupt
    WDT->RSTRT = 0x82;       //reset watchdog counter

    NVIC_ClearPendingIRQ(WDT_IRQn); //clear pending flags in NVIC for WDT
    NVIC_SetPriority(WDT_IRQn,0);   //set priority wor WDT interrupt (0 = highest)
    NVIC_EnableIRQ(WDT_IRQn);       //enable WDT IRW
 
    
    WDT->CFG_b.WDTEN = 1;           //enable WDT, call WDT->LOCK = 0x3A; for enable and lock 
   
    RSTGEN->CFG_b.WDREN = 1;        //enable reset generation by WDT

    while(1)
    {
        //application code
        
        //
        // Run program normal for 5 seconds
        //
        if (((u32Counter % 500) == 0) && (u32Counter <= 5000))
        {
            WDT->RSTRT = 0xB2;
            SetLeds(u8Counter++);
            delay(1);
        } else if (u32Counter > 5000)
        {
            __WFI(); //goto deep sleep
        }
    }
}



/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/

