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
 ** Use ADC to read 14-bit ADC value at PAD 13 using interrupts
 **
 ** History:
 **   - 2019-08-29  V1.0  MSc  First version

 *****************************************************************************/
#define __MAIN_C__
/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "mcu.h"
#include "base_types.h"
#include "stdio.h"


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

void ADC_IRQHandler(void)
{
    uint32_t u32Status = ADC->INTSTAT;
    static uint32_t u32Data;
    //
    // check scan conversion is complete
    //
    if (ADC->INTSTAT_b.CNVCMP == 1)
    {
        while (_FLD2VAL(ADC_FIFO_COUNT,ADC->FIFO) > 0)
        {
            //
            // Check for data in FIFO and channel 0 before pop data from FIFO
            //
            switch(_FLD2VAL(ADC_FIFO_SLOTNUM,ADC->FIFO))
            {
                case 0:
                    u32Data = _FLD2VAL(ADC_FIFO_DATA,ADC->FIFO) >> 6;
                    printf("Slot: %d, Value: %d, ", _FLD2VAL(ADC_FIFO_SLOTNUM,ADC->FIFO),u32Data);
                    ADC->FIFO = 0; // pop FIFO
                    printf("Data-Left: %d\r\n", _FLD2VAL(ADC_FIFO_COUNT,ADC->FIFO));
                    break;
                default:
                    ADC->FIFO = 0; // pop FIFO
                    break;
            }
        }
    } 

    //
    // re-trigger scan if scan was completed
    //
    if (ADC->INTSTAT_b.SCNCMP == 1)
    {
        ADC->SWT = 0x37;
    }
    ADC->INTCLR = u32Status;
}

/**
*********************************************************************************
** \brief Main function
**  
*********************************************************************************/
int main(void)
{
    uint32_t u32Cfg;
    

    SystemCoreClockUpdate();                //update clock variable SystemCoreClock (defined by CMSIS)
    SysTick_Config(SystemCoreClock / 1000); //setup 1ms SysTick (defined by CMSIS)
    //application initialization area

    GPIO->PADKEY = 0x73; //unlock GPIO configuration
    GPIO->PADREGD_b.PAD13FNCSEL = GPIO_PADREGD_PAD13FNCSEL_ADCD0PSE8; //select ADC8 at PAD13
    GPIO->PADKEY = 0;

    //
    // Enable power for ADC and wait power is enabled
    //
    PWRCTRL->DEVPWREN_b.PWRADC = 1;
    while(PWRCTRL->ADCSTATUS_b.ADCPWD == 0) __NOP();

    ADC->CFG = 0;

    u32Cfg =      _VAL2FLD(ADC_CFG_CKMODE,ADC_CFG_CKMODE_LLCKMODE)   //Low Latency Clock Mode
               |  _VAL2FLD(ADC_CFG_CLKSEL,ADC_CFG_CLKSEL_HFRC)       //HFRC clock
               |  _VAL2FLD(ADC_CFG_TRIGSEL,ADC_CFG_TRIGSEL_SWT)      //Software trigger
               |  _VAL2FLD(ADC_CFG_REFSEL,ADC_CFG_REFSEL_INT2P0)     //internal 2V reference
               |  _VAL2FLD(ADC_CFG_LPMODE,ADC_CFG_LPMODE_MODE1)      //Low Power Mode 1
               |  _VAL2FLD(ADC_CFG_RPTEN,ADC_CFG_RPTEN_SINGLE_SCAN)  //Single scan
               |  _VAL2FLD(ADC_CFG_ADCEN,1);                         //Enable ADC

    //
    // Using slot 0
    //
    ADC->SL0CFG = 0; 
    ADC->SL0CFG_b.CHSEL0 = ADC_SL0CFG_CHSEL0_SE8; //use ADCSE8 as input;
    ADC->SL0CFG_b.ADSEL0 = ADC_SL0CFG_ADSEL0_AVG_16_MSRMTS; //Avaerage over 16 measurements
    ADC->SL0CFG_b.PRMODE0 = ADC_SL0CFG_PRMODE0_P14B; //use 14-bit 1.2MS/s
    ADC->SL0CFG_b.WCEN0 = 0; //window comparator mode disabled
    ADC->SL0CFG_b.SLEN0 = 1; //slot enabled

    ADC->WULIM_b.ULIM = 0; //Window comparator upper limit (not used)
    ADC->WLLIM_b.LLIM = 0; //Window comparator lower limit (not used)

    ADC->INTEN_b.WCINC = 0;               //disable window comparator voltage incursion interrupt
    ADC->INTEN_b.WCEXC = 0;               //disable window comparator voltage excursion interrupt
    ADC->INTEN_b.FIFOOVR1 = 0;            //disable FIFO 100% full interrupt
    ADC->INTEN_b.FIFOOVR2 = 0;            //disable FIFO 75% full interrupt
    ADC->INTEN_b.SCNCMP = 1;              //enable ADC scan complete interrupt
    ADC->INTEN_b.CNVCMP = 1;              //enable ADC conversion complete interrupt

    ADC->CFG = u32Cfg;
    
    ADC->INTCLR = 0xFFFFFFFF;             //clear interrupts

    NVIC_DisableIRQ(ADC_IRQn);            //Disable NVIC for ADC
    NVIC_ClearPendingIRQ(ADC_IRQn);       //Clear pending interrupts for ADC
    NVIC_SetPriority(ADC_IRQn,1);         //Set priority (0 == highest)
    NVIC_EnableIRQ(ADC_IRQn);             //Enable NVIC for ADC

    ADC->SWT = 0x37;                      //trigger ADC

    
    while(1)
    {
        //application code
        __WFI(); // go to deepsleep
    }
}



/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/

