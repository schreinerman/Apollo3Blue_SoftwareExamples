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
 ** A detailed description is available at 
 ** @link mainGroup  description @endlink
 **
 ** History:
 **   - 2019-08-01 Manuel Schreiner First Version

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
** \brief Write a pattern to the timer registers
**  
** \param [in] delayMs Delay in ms
**  
** \details delay function used with SysTick IRQ
**  
*********************************************************************************/
void WritePattern(uint8_t* pu8Data, uint32_t u32DataLen)
{
    int i;
    CTIMER->CMPRA0 = 0;
    for(i = 0;i < 4;i++)
    {
        if (u32DataLen > (8*i))
        {
            CTIMER->CMPRA0 |= (pu8Data[i] << (8*i));
        }
    }
    CTIMER->CMPRAUXA0 = 0;
    for(i = 0;i < 4;i++)
    {
        if (u32DataLen > (32 + 8*i))
        {
            CTIMER->CMPRAUXA0 |= (pu8Data[i+4] << (8*i));
        }
    }
}

/**
*********************************************************************************
** \brief Main
**  
*********************************************************************************/
int main(void)
{
    const uint8_t au8Pattern[8] = {0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7F,0xFF};
    SystemCoreClockUpdate();                //update clock variable SystemCoreClock (defined by CMSIS)
    SysTick_Config(SystemCoreClock / 1000); //setup 1ms SysTick (defined by CMSIS)

    GPIO->PADKEY = 0x00000073;  //unlock pin selection

    //use pin 12 for timer output
    GPIO->PADREGD_b.PAD12FNCSEL = GPIO_PADREGD_PAD12FNCSEL_CT0;

    //ouput is push-pull
    GPIO->CFGB_b.GPIO12OUTCFG = GPIO_CFGB_GPIO12OUTCFG_PUSHPULL;   
          
    GPIO->PADKEY = 0;  //lock pin selection

    CTIMER->OUTCFG0_b.CFG0 = CTIMER_OUTCFG0_CFG0_A0OUT;          //Output configuration to ct0

    GPIO->CTENCFG_b.EN0 = 0;             //Timer disabled

    CTIMER->CTRL0_b.TMRA0CLR = 1;         //clear timer
 
    CTIMER->CTRL0_b.TMRA0FN = CTIMER_CTRL0_TMRA0FN_REPEATPATTERN;

    CTIMER->CTRL0_b.TMRA0CLK = CTIMER_CTRL0_TMRA0CLK_HFRC_DIV256;
     
    WritePattern((uint8_t*)au8Pattern,64);     //write 64-bit pattern
    CTIMER->AUX0_b.TMRA0LMT = 63;              //set limit to 64

    CTIMER->CTRL0_b.TMRA0CLR = 0;         //release clear timer
    CTIMER->CTRL0_b.TMRA0EN = 1;          //start timer

    //application initialization area
    while(1)
    {
        //application code
    }
}


/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/

