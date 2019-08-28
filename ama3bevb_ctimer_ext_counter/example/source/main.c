/*******************************************************************************
* Copyright (C) 2016, Fujitsu Electronics Europe GmbH or a                     *
* subsidiary of Fujitsu Electronics Europe GmbH.                               *
*                                                                              *
* This software, including source code, documentation and related materials    *
* ("Software") is developed by Fujitsu Electronics Europe GmbH ("Fujitsu")     *
* unless identified differently hereinafter for Open Source Software.          *
* All rights reserved.                                                         *
*                                                                              *
* This software is provided free of charge and not for sale, providing test    *
* and sandbox applications. Fujitsu reserves the right to make changes to      *
* the Software without notice. Before use please check with Fujitsu            *
* for the most recent software.                                                *
*                                                                              *
* If no specific Open Source License Agreement applies (see hereinafter),      *
* Fujitsu hereby grants you a personal, non-exclusive,                         *
* non-transferable license to copy, modify and compile the                     *
* Software source code solely for use in connection with products              *
* supplied by Fujitsu. Any reproduction, modification, translation,            *
* compilation, or representation of this Software requires written             *
* permission of Fujitsu.                                                       *
*                                                                              *
* NO WARRANTY: This software is provided “as-is” with no warranty of any kind  *
* (German “Unter Ausschluss jeglicher Gewähleistung”), express or implied,     *
* including but not limited to non-infringement of third party rights,         *
* merchantability and fitness for use.                                         *
*                                                                              *
* In the event the software deliverable includes the use of                    *
* open source components, the provisions of the respective governing           *
* open source license agreement shall apply with respect to such software      *
* deliverable. Open source components are identified in the read-me files      *
* of each section / subsection.                                                *
*                                                                              *
* German law applies with the exclusion of the rules of conflict of law.       *
*                                                                              *
*                                      ***                                     *
* September 2016                                                               *
* FUJITSU ELECTRONICS Europe GmbH                                              *
*                                                                              *
*******************************************************************************/
/******************************************************************************/
/** \file main.c
 **
 ** \brief Generate 10 pulses with BTN3 (middle button) 
 **        to pulse LED D8 for about 500ms
 **        Note: buttons are not debounced, so it could be also less than
 **        10 pulses are resulting in pulse LED D8
 **
 ** Main Module
 **
 ** History:
 ** 2019-06-28  V1.0.0  MSc first version
 **
 ******************************************************************************/

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
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
#define LED_D8 30   //CT11, B2OUT, INCFG 3
#define LED_D9 10

//all buttons are between 0..31, so A register is used
//Note: if using 32..49, B register must be used
#define BUTTON_BTN2 16
#define BUTTON_BTN3 18  //CT4, A1OUT, INCFG 2
#define BUTTON_BTN4 19  //CT6, B1OUT, INCFG 2

/*****************************************************************************/
/* Local variable definitions ('static')                                     */
/*****************************************************************************/

static volatile uint32_t u32Counter;  //ms counter

/******************************************************************************/
/* Prototype functions                                                        */
/******************************************************************************/


/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/


/**
 *****************************************************************************
 ** 
 **\brief delay function that uses u32Counter to count in ms
 **
 *****************************************************************************/

void delay(uint32_t delayMs) 
{
    uint32_t u32End = u32Counter;
    u32End += delayMs;
    while(u32End != u32Counter) __NOP();
}



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
 ******************************************************************************
 ** \brief  Main function
 **
 ** \return int return value, if needed
 ******************************************************************************/
int main(void)
{     
    SystemCoreClockUpdate();                //update clock variable SystemCoreClock (defined by CMSIS)
    SysTick_Config(SystemCoreClock / 1000); //setup 1ms SysTick (defined by CMSIS)   

    GPIO->PADKEY = 0x00000073;             //unlock pin selection

    GPIO->PADREGH_b.PAD30FNCSEL = GPIO_PADREGH_PAD30FNCSEL_CT11;   //set PWM CT11 on pin 30
    GPIO->PADREGH_b.PAD30INPEN = 1;                                //enable input on pin 30
    GPIO->CFGD_b.GPIO30OUTCFG = GPIO_CFGD_GPIO30OUTCFG_PUSHPULL;   //Ouput is enabled


    GPIO->PADREGE_b.PAD18FNCSEL = GPIO_PADREGE_PAD18FNCSEL_CT4;   //set PWM CT11 on pin 30
    GPIO->PADREGE_b.PAD18PULL = 1;                                //enable pull-up
    GPIO->PADREGE_b.PAD18INPEN = 1;                               //enable input
   
    GPIO->PADKEY = 0;                      //lock pin selection

    //
    // Using CTIMER A1 as input on PAD18, CT4 (BTN3) for counting 10 times
    //

    GPIO->CTENCFG_b.EN4 = 1;
    CTIMER->OUTCFG0_b.CFG4 = CTIMER_OUTCFG0_CFG5_A1OUT; //use A1OUT for CT4

    CTIMER->CTRL1_b.TMRA1CLR = 1;         //clear timer

    CTIMER->CTRL1_b.TMRA1FN = CTIMER_CTRL1_TMRA1FN_REPEATEDCOUNT;  
    CTIMER->CMPRA1_b.CMPR0A1 = 10;

    CTIMER->INTEN_b.CTMRA1C0INT = 1;      //interrupt when CMPR0A1 is reached

    CTIMER->CTRL1_b.TMRA1CLR = 0;         //release clear timer
    CTIMER->CTRL1_b.TMRA1EN = 1;          //start timer


    //
    // Using CTIMER B2 as output to pulse LED for about 500ms
    //

    GPIO->CTENCFG_b.EN11 = 0;
    CTIMER->OUTCFG1_b.CFG11 = CTIMER_OUTCFG1_CFG11_B2OUT; //Output configuration to ct11

    CTIMER->CTRL2_b.TMRB2CLR = 1;         //clear timer
    CTIMER->CTRL2_b.TMRB2IE1 = 1;         // enable IRQ

    CTIMER->CTRL2_b.TMRB2FN = CTIMER_CTRL2_TMRB2FN_PULSE_ONCE;
    CTIMER->CTRL2_b.TMRB2CLK = CTIMER_CTRL2_TMRB2CLK_HFRC_DIV4K;
    CTIMER->CMPRB2_b.CMPR0B2 = 6000;         //periode - on-time timer
    CTIMER->CMPRB2_b.CMPR1B2 = 12000;        //periode

    CTIMER->AUX2_b.TMRB2TRIG = CTIMER_AUX2_TMRB2TRIG_A1OUT;

    CTIMER->CTRL2_b.TMRB2CLR = 0;         //release clear timer
    CTIMER->CTRL2_b.TMRB2EN = 1;          //start timer

    
    while(1)
    {
        __WFI(); //go to deep sleep
    }
}

/*****************************************************************************/
/* EOF (not truncated)                                                       */
/*****************************************************************************/
