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
 ** Simple example for GPIO use.
 ** 
 **
 ** History:
 **   - 2019-03-21  V1.0  LEh  First Version
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

//connection of buttons/leds
//all LEDs are between 0..31, so A register is used
//Note: if using 32..49, B register must be used
#define LED_D5 17
#define LED_D6 14
#define LED_D7 15
#define LED_D8 30
#define LED_D9 10

//all buttons are between 0..31, so A register is used
//Note: if using 32..49, B register must be used
#define BUTTON_BTN2 16
#define BUTTON_BTN3 18
#define BUTTON_BTN4 19

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
 *****************************************************************************
 ** 
 **\brief Main function
 **
 *****************************************************************************/
int main(void)
{
    uint32_t u32Speed = 500;
    SystemCoreClockUpdate();                //update clock variable SystemCoreClock (defined by CMSIS)
    SysTick_Config(SystemCoreClock / 1000); //setup 1ms SysTick (defined by CMSIS)

    //application initialization area
    
    GPIO->PADKEY = 0x00000073;            //unlock pin selection
    
    GPIO->PADREGE_b.PAD16INPEN = 1;       //input enable button BTN2
    GPIO->PADREGE_b.PAD18INPEN = 1;       //input enable button BTN3
    GPIO->PADREGE_b.PAD19INPEN = 1;       //input enable button BTN4
    

    GPIO->CFGC_b.GPIO17OUTCFG = 1;        //output is push-pull LED D5
    GPIO->CFGB_b.GPIO14OUTCFG = 1;        //output is push-pull LED D6
    GPIO->CFGB_b.GPIO15OUTCFG = 1;        //output is push-pull LED D7
    GPIO->CFGD_b.GPIO30OUTCFG = 1;        //output is push-pull LED D8
    GPIO->CFGB_b.GPIO10OUTCFG = 1;        //output is push-pull LED D9
    
    GPIO->PADKEY = 0;                     //lock pin selection
    
    
        
    while(1)
    {
        //all LEDs and buttons are between 0..31, so A register is used
        //Note: if using 32..49, B register must be used
        //Example: read       GPIO via GPIO->RDA or GPIO->RDB
        //         write      GPIO via GPIO->WTA or GPIO->WTB
        //         set bit    at GPIO via GPIO->WTSA or GPIO->WTSB
        //         clear bit  at GPIO via GPIO->WTCA or GPIO->WTCB

        //if BTN3 is/was pressed, LEDs blinking slower
        if ((GPIO->RDA & (1 << BUTTON_BTN3)) == 0)
        {
            u32Speed = 1000;
        }
        
        //if BTN4 is/was pressed, LEDs blinking faster
        if ((GPIO->RDA & (1 << BUTTON_BTN4)) == 0)
        {
            u32Speed = 200;
        }
        
        //if BTN2 is released LEDs blinking and stopped while pressed
        if (GPIO->RDA & (1 << BUTTON_BTN2))
        {
            if ((u32Counter % (u32Speed * 2)) == 0)
            {
                GPIO->WTCA = (1 << LED_D5) | (1 << LED_D7) | (1 << LED_D9); //clear bits in output register
                GPIO->WTSA = (1 << LED_D6) | (1 << LED_D8);                 //set bits in output register
            } else if ((u32Counter % u32Speed) == 0)
            {
                GPIO->WTSA = (1 << LED_D5) | (1 << LED_D7) | (1 << LED_D9); //set bits in output register
                GPIO->WTCA = (1 << LED_D6) | (1 << LED_D8);                 //clear bits in output register
            }
        }
    }
}



/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
