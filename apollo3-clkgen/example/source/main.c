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
 ** Example how to clock-out one of the different clock-sources with or without divider
 **
 ** This example generates 48MHz divided by 4 (=12MHz) at PAD7.
 **
 ** History:
 **   - 2019-08-21  V1.0  MSc  Automatic Created

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

int main(void)
{
    SystemCoreClockUpdate();                //update clock variable SystemCoreClock (defined by CMSIS)
    SysTick_Config(SystemCoreClock / 1000); //setup 1ms SysTick (defined by CMSIS)
    //application initialization area


    //
    // Use one of those clock settings:
    //
    // CLKGEN_CLKOUT_CKSEL_LFRC             - LFRC : LFRC                                                               
    // CLKGEN_CLKOUT_CKSEL_XT_DIV2          - XT_DIV2 : XT / 2                                                          
    // CLKGEN_CLKOUT_CKSEL_XT_DIV4          - XT_DIV4 : XT / 4                                                          
    // CLKGEN_CLKOUT_CKSEL_XT_DIV8          - XT_DIV8 : XT / 8                                                          
    // CLKGEN_CLKOUT_CKSEL_XT_DIV16         - XT_DIV16 : XT / 16                                                        
    // CLKGEN_CLKOUT_CKSEL_XT_DIV32         - XT_DIV32 : XT / 32                                                        
    // CLKGEN_CLKOUT_CKSEL_RTC_1Hz          - RTC_1Hz : 1 Hz as selected in RTC                                         
    // CLKGEN_CLKOUT_CKSEL_XT_DIV2M         - XT_DIV2M : XT / 2^21                                                      
    // CLKGEN_CLKOUT_CKSEL_XT               - XT : XT                                                                   
    // CLKGEN_CLKOUT_CKSEL_CG_100Hz         - CG_100Hz : 100 Hz as selected in CLKGEN                                   
    // CLKGEN_CLKOUT_CKSEL_HFRC             - HFRC : HFRC                                                               
    // CLKGEN_CLKOUT_CKSEL_HFRC_DIV4        - HFRC_DIV4 : HFRC / 4                                                      
    // CLKGEN_CLKOUT_CKSEL_HFRC_DIV8        - HFRC_DIV8 : HFRC / 8                                                      
    // CLKGEN_CLKOUT_CKSEL_HFRC_DIV16       - HFRC_DIV16 : HFRC / 16                                                    
    // CLKGEN_CLKOUT_CKSEL_HFRC_DIV64       - HFRC_DIV64 : HFRC / 64                                                    
    // CLKGEN_CLKOUT_CKSEL_HFRC_DIV128      - HFRC_DIV128 : HFRC / 128                                                  
    // CLKGEN_CLKOUT_CKSEL_HFRC_DIV256      - HFRC_DIV256 : HFRC / 256                                                  
    // CLKGEN_CLKOUT_CKSEL_HFRC_DIV512      - HFRC_DIV512 : HFRC / 512                                                  
    // CLKGEN_CLKOUT_CKSEL_FLASH_CLK        - FLASH_CLK : Flash Clock                                                   
    // CLKGEN_CLKOUT_CKSEL_LFRC_DIV2        - LFRC_DIV2 : LFRC / 2                                                      
    // CLKGEN_CLKOUT_CKSEL_LFRC_DIV32       - LFRC_DIV32 : LFRC / 32                                                    
    // CLKGEN_CLKOUT_CKSEL_LFRC_DIV512      - LFRC_DIV512 : LFRC / 512                                                  
    // CLKGEN_CLKOUT_CKSEL_LFRC_DIV32K      - LFRC_DIV32K : LFRC / 32768                                                
    // CLKGEN_CLKOUT_CKSEL_XT_DIV256        - XT_DIV256 : XT / 256                                                      
    // CLKGEN_CLKOUT_CKSEL_XT_DIV8K         - XT_DIV8K : XT / 8192                                                      
    // CLKGEN_CLKOUT_CKSEL_XT_DIV64K        - XT_DIV64K : XT / 2^16                                                     
    // CLKGEN_CLKOUT_CKSEL_ULFRC_DIV16      - ULFRC_DIV16 : Uncal LFRC / 16                                             
    // CLKGEN_CLKOUT_CKSEL_ULFRC_DIV128     - ULFRC_DIV128 : Uncal LFRC / 128                                           
    // CLKGEN_CLKOUT_CKSEL_ULFRC_1Hz        - ULFRC_1Hz : Uncal LFRC / 1024                                             
    // CLKGEN_CLKOUT_CKSEL_ULFRC_DIV4K      - ULFRC_DIV4K : Uncal LFRC / 4096                                           
    // CLKGEN_CLKOUT_CKSEL_ULFRC_DIV1M      - ULFRC_DIV1M : Uncal LFRC / 2^20                                           
    // CLKGEN_CLKOUT_CKSEL_HFRC_DIV64K      - HFRC_DIV64K : HFRC / 2^16                                                 
    // CLKGEN_CLKOUT_CKSEL_HFRC_DIV16M      - HFRC_DIV16M : HFRC / 2^24                                                 
    // CLKGEN_CLKOUT_CKSEL_LFRC_DIV1M       - LFRC_DIV1M : LFRC / 2^20                                                  
    // CLKGEN_CLKOUT_CKSEL_HFRCNE           - HFRCNE : HFRC (not autoenabled)                                           
    // CLKGEN_CLKOUT_CKSEL_HFRCNE_DIV8      - HFRCNE_DIV8 : HFRC / 8 (not autoenabled)                                  
    // CLKGEN_CLKOUT_CKSEL_XTNE             - XTNE : XT (not autoenabled)                                               
    // CLKGEN_CLKOUT_CKSEL_XTNE_DIV16       - XTNE_DIV16 : XT / 16 (not autoenabled)                                    
    // CLKGEN_CLKOUT_CKSEL_LFRCNE_DIV32     - LFRCNE_DIV32 : LFRC / 32 (not autoenabled)                                
    // CLKGEN_CLKOUT_CKSEL_LFRCNE           - LFRCNE : LFRC (not autoenabled) - Default for undefined values            

    //select HFRC divided by 4
    CLKGEN->CLKOUT_b.CKSEL = CLKGEN_CLKOUT_CKSEL_HFRC_DIV4;

    //enable clock-out 
    CLKGEN->CLKOUT_b.CKEN = 1;

    GPIO->PADKEY = 0x73; //unlock pad configuration
    //select PAD7 for clock output (or PAD0)
    GPIO->PADREGB_b.PAD7FNCSEL = GPIO_PADREGB_PAD7FNCSEL_CLKOUT;
    GPIO->PADKEY = 0; //lock  pad configuration

    while(1)
    {
        //application code
    }
}



/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/

