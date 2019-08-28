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
 ** \brief Example to show PWM functionality with Counter/Timer module
 **
 ** Main Module
 **
 ** History:
 ** 2019-03-21  V1.0.0  LEh first version
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

#define DIVISOR 4

/*****************************************************************************/
/* Local variable definitions ('static')                                     */
/*****************************************************************************/

static volatile uint32_t u32Counter;  //ms counter
static volatile uint8_t u8Duty = 1;

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
 *****************************************************************************
 ** 
 **\brief CTIMER interrupt handler to update the duty time
 **
 *****************************************************************************/
void CTIMER_IRQHandler(void)
{
    uint32_t u32Status = CTIMER->INTSTAT;  //backup interrupt status

    if(CTIMER->INTSTAT_b.CTMRB2C1INT == 1) //check interrupt for timer B2
    {
        if(u8Duty == 0) u8Duty = 1;        //Duty register must be not 0!
        CTIMER->CMPRB2_b.CMPR0B2 = u8Duty; //update duty
    }

    CTIMER->INTCLR = u32Status;            //clear interrupts
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

    GPIO->PADREGH_b.PAD30FNCSEL = 2;        //set PWM CT11 on pin 30
    GPIO->PADREGH_b.PAD30INPEN = 1;        //enable input on pin 30
    GPIO->CFGD_b.GPIO30OUTCFG = 1;         //Ouput is push-pull
       
    GPIO->PADKEY = 0;                      //lock pin selection

    //CTIMER Config

    GPIO->CTENCFG_b.EN11 = 0;             //Timer enabled
    CTIMER->OUTCFG1_b.CFG11 = 3;          //Output configuration to ct11


    CTIMER->CTRL2_b.TMRB2CLR = 1;         //clear timer
    CTIMER->CTRL2_b.TMRB2IE1 = 1;         // enable IRQ
 
    CTIMER->CTRL2_b.TMRB2FN = 3;          //repeated pulse count timer
    CTIMER->CTRL2_b.TMRB2CLK = 0x3;       // HFRC div set to 256
    CTIMER->CMPRB2_b.CMPR0B2 = 1;         //periode - on-time timer
    CTIMER->CMPRB2_b.CMPR1B2 = 256;       //compare register 1 - deassert

    CTIMER->INTEN_b.CTMRB2C1INT = 1;      //interrupt when CMPR1B2 is reached

    CTIMER->CTRL2_b.TMRB2CLR = 0;         //release clear timer
    CTIMER->CTRL2_b.TMRB2EN = 1;          //start timer
   

    //    enabling interrupts in the NVIC

    NVIC_DisableIRQ(CTIMER_IRQn);         //disable IRQ in the NVIC
    NVIC_ClearPendingIRQ(CTIMER_IRQn);    //clear pending flag in the NVIC
    NVIC_SetPriority(CTIMER_IRQn,1);      //set the interrupts priority, smaller means higher priority
    NVIC_EnableIRQ(CTIMER_IRQn);          //enable the IRQ in the NVIC

    while(1)
    {
        if(((u32Counter/DIVISOR) % 512)<256)
        {
            u8Duty = 255 - (u32Counter/DIVISOR) % 256;      //adjusts duty cycle => LED brightness up
        }
        else
        {
            u8Duty = (u32Counter/DIVISOR) % 256;            //adjusts duty cycle => LED brightness down
        }
    }
}

/*****************************************************************************/
/* EOF (not truncated)                                                       */
/*****************************************************************************/
