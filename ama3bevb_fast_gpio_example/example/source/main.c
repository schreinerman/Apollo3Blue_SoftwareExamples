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
 ** Simple example for FAST-GPIO use.
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

#define FAST_BIT(b)   (b   % 8)
#define FAST_LED(led) (led % 8)
#define FAST_BTN(btn) (btn % 8)

/*****************************************************************************/
/* Global variable definitions (declared in header file with 'extern')       */
/*****************************************************************************/



/*****************************************************************************/
/* Local type definitions ('typedef')                                        */
/*****************************************************************************/


/*****************************************************************************/
/* Local variable definitions ('static')                                     */
/*****************************************************************************/


/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/


/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/

en_result_t ApolloSysctrl_EnableCache(void)
{
    uint32_t u32Conf;
    CACHECTRL->CACHECFG &=
        ~(CACHECTRL_CACHECFG_DCACHE_ENABLE_Msk  |
          CACHECTRL_CACHECFG_ICACHE_ENABLE_Msk);

    u32Conf = _VAL2FLD(CACHECTRL_CACHECFG_ENABLE, 1)                              |
        _VAL2FLD(CACHECTRL_CACHECFG_CACHE_CLKGATE, 1)                       |
        _VAL2FLD(CACHECTRL_CACHECFG_CACHE_LS, 0)                            |
        _VAL2FLD(CACHECTRL_CACHECFG_DATA_CLKGATE, 1)                        |
        _VAL2FLD(CACHECTRL_CACHECFG_ENABLE_MONITOR, 0)                      |
        _VAL2FLD(CACHECTRL_CACHECFG_LRU, 1)                    |
        _VAL2FLD(CACHECTRL_CACHECFG_CONFIG, CACHECTRL_CACHECFG_CONFIG_W1_128B_1024E)            |
        ((0 << CACHECTRL_CACHECFG_ENABLE_NC0_Pos) &
            (CACHECTRL_CACHECFG_ENABLE_NC1_Msk      |
             CACHECTRL_CACHECFG_ENABLE_NC0_Msk))                            |
        ((3 << CACHECTRL_CACHECFG_ICACHE_ENABLE_Pos) &
            (CACHECTRL_CACHECFG_DCACHE_ENABLE_Msk   |
             CACHECTRL_CACHECFG_ICACHE_ENABLE_Msk));
    CACHECTRL->CACHECFG = u32Conf;

    return Ok;
}

/**
 *****************************************************************************
 ** 
 **\brief Main function
 **
 *****************************************************************************/
int main(void)
{
    uint8_t u8Data;
    ApolloSysctrl_EnableCache();
    
    GPIO->PADKEY = 0x00000073;            //unlock pin selection

    // Up to 8-pins can be used in parallel as FAST GPIOs.
    //
    // For every row only one column can be selected by enable 
    // set the tristate enable bit in GPIO->ENSA for GPIO0..31
    // or GPIO->ENSB for GPIO32..49
    //
    //     FAST-BIT   PIN / GPIOn
    //     ---       ---------------------------
    //      0          0   8  16  24  32  40  48
    //      1          1   9  17  25  33  41  49
    //      2          2  10  18  26  34  42
    //      3          3  11  19  27  35  43
    //      4          4  12  20  28  36  44
    //      5          5  13  21  29  37  45
    //      6          6  14  22  30  38  46
    //      7          7  15  23  31  39  47
    //                \---- ENA ----/\-- ENB --/  
    //
    //
    //   LEDs 5..8 can be used in parallel:
    //
    //     FAST-BIT   
    //     ---       ---------------------------
    //      0          .     .    BTN2   .     .     .     .
    //      1          .     .    D5     .     .     .     .
    //      2          .    D9    BTN3   .     .     .   
    //      3          .     .    BTN4   .     .     .   
    //      4          .     .     .     .     .     .   
    //      5          .     .     .     .     .     .   
    //      6          .    D6     .    D8     .     .   
    //      7          .    D7     .     .     .     .   
    // 
    //   LED D6 and D8 can't be used in parallel, so using D6
    //
    GPIO->ENCA |= (1 << LED_D5) | (1 << LED_D6) | (1 << LED_D7) | (1 << LED_D9) | (1 << BUTTON_BTN2); //disbale fast IOs
    GPIO->ENSA |= (1 << LED_D5) | (1 << LED_D6) | (1 << LED_D7) | (1 << LED_D9) | (1 << BUTTON_BTN2); //enable tristate / fast IO

    GPIO->PADREGE_b.PAD16INPEN = 1;       //input enable button BTN2
    GPIO->PADREGE_b.PAD18INPEN = 1;       //input enable button BTN3
    GPIO->PADREGE_b.PAD19INPEN = 1;       //input enable button BTN4

    GPIO->CFGC_b.GPIO17OUTCFG = 1;        //output is push-pull LED D5
    GPIO->CFGB_b.GPIO14OUTCFG = 1;        //output is push-pull LED D6
    GPIO->CFGB_b.GPIO15OUTCFG = 1;        //output is push-pull LED D7
    //LED D6 and D8 can't be used in parallel, so not using D8
    //GPIO->CFGD_b.GPIO30OUTCFG = 1;        //output is push-pull LED D8
    GPIO->CFGB_b.GPIO10OUTCFG = 1;        //output is push-pull LED D9

    GPIO->PADKEY = 0;                     //lock pin selection
    
    while(1)
    {
        //code needs speed optimization enabled to run with 25MHz pin-toggle
        APBDMA->BBSETCLEAR = _VAL2FLD(APBDMA_BBSETCLEAR_SET,    (1 << FAST_LED(LED_D5)) );  //set
        APBDMA->BBSETCLEAR = _VAL2FLD(APBDMA_BBSETCLEAR_CLEAR,  (1 << FAST_LED(LED_D5)) );  //clear
        APBDMA->BBSETCLEAR = _VAL2FLD(APBDMA_BBSETCLEAR_SET,    (1 << FAST_LED(LED_D5)) );  //set
        APBDMA->BBSETCLEAR = _VAL2FLD(APBDMA_BBSETCLEAR_CLEAR,  (1 << FAST_LED(LED_D5)) );  //clear        
        APBDMA->BBSETCLEAR = _VAL2FLD(APBDMA_BBSETCLEAR_SET,    (1 << FAST_LED(LED_D5)) );  //set
        APBDMA->BBSETCLEAR = _VAL2FLD(APBDMA_BBSETCLEAR_CLEAR,  (1 << FAST_LED(LED_D5)) );  //clear        
        APBDMA->BBSETCLEAR = _VAL2FLD(APBDMA_BBSETCLEAR_SET,    (1 << FAST_LED(LED_D5)) );  //set
        APBDMA->BBSETCLEAR = _VAL2FLD(APBDMA_BBSETCLEAR_CLEAR,  (1 << FAST_LED(LED_D5)) );  //clear
        APBDMA->BBSETCLEAR = _VAL2FLD(APBDMA_BBSETCLEAR_SET,    (1 << FAST_LED(LED_D5)) );  //set
        APBDMA->BBSETCLEAR = _VAL2FLD(APBDMA_BBSETCLEAR_CLEAR,  (1 << FAST_LED(LED_D5)) );  //clear

        //fast read BTN2 and switching D6 with button
        if (APBDMA->BBINPUT & (1 << FAST_BTN(BUTTON_BTN2)))
        {
            APBDMA->BBSETCLEAR = _VAL2FLD(APBDMA_BBSETCLEAR_CLEAR,    (1 << FAST_LED(LED_D6)) );  //set
        } else
        {
            APBDMA->BBSETCLEAR = _VAL2FLD(APBDMA_BBSETCLEAR_SET,    (1 << FAST_LED(LED_D6)) );  //set
        }
    }
}



/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
