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
 ** Basic RTC example to set / get time
 **
 ** History:
 **   - 2019-08-29  V1.0  MSc First Version

 *****************************************************************************/
#define __MAIN_C__
/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "mcu.h"
#include "base_types.h"
#include "string.h"
#include "stdio.h"


/*****************************************************************************/
/* Local pre-processor symbols/macros ('#define')                            */
/*****************************************************************************/

#define DEC_TO_BCD(x)             ((((x) / 10) << 4) | ((x) % 10))
#define BCD_TO_DEC(x)             (((((x) & 0xF0) >> 4) * 10) + ((x) & 0x0F))

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
    uint8_t u8Second;
    uint8_t u8Minute;
    uint8_t u8Hour;
    uint8_t u8Day;
    uint8_t u8Month;
    uint8_t u8Year;
    uint8_t u8WeekDay;

    uint32_t u32TmpLower;
    uint32_t u32TmpUpper;

    SystemCoreClockUpdate();                //update clock variable SystemCoreClock (defined by CMSIS)
    SysTick_Config(SystemCoreClock / 1000); //setup 1ms SysTick (defined by CMSIS)
    //application initialization area
    if (RTC->RTCCTL_b.RSTOP == 1)
    {
        printf("Compiled at: %s, %s\r\n",__TIME__,__DATE__);
        printf("RTC stopped, transferring compile date/time...\r\n");
        //
        // Convert pre-processor variable __DATE__ to month number 1..12
        //
        if (strncmp("Jan",__DATE__,3) == 0)
        {
            u8Month = 1;
        } else if (strncmp("Feb",__DATE__,3) == 0)
        {
            u8Month = 2;
        } else if (strncmp("Mar",__DATE__,3) == 0)
        {
            u8Month = 3;
        } else if (strncmp("Apr",__DATE__,3) == 0)
        {
            u8Month = 4;
        } else if (strncmp("May",__DATE__,3) == 0)
        {
            u8Month = 5;
        } else if (strncmp("Jun",__DATE__,3) == 0)
        {
            u8Month = 6;
        } else if (strncmp("Jul",__DATE__,3) == 0)
        {
            u8Month = 7;
        } else if (strncmp("Aug",__DATE__,3) == 0)
        {
            u8Month = 8;
        } else if (strncmp("Sep",__DATE__,3) == 0)
        {
            u8Month = 9;
        } else if (strncmp("Oct",__DATE__,3) == 0)
        {
            u8Month = 10;
        } else if (strncmp("Nov",__DATE__,3) == 0)
        {
            u8Month = 11;
        } else if (strncmp("Dec",__DATE__,3) == 0)
        {
            u8Month = 12;
        }
        
        //
        // Convert pre-processor variable __DATE__ to day number 1..31
        //
        u8Day = (__DATE__[4] - '0') * 10 + (__DATE__[5] - '0');

        //
        // Convert pre-processor variable __DATE__ to year number
        //
        u8Year =  (__DATE__[9] - '0') * 10 + (__DATE__[10] - '0');

        //
        // Convert pre-processor variable __DATE__ to hour number 0..23
        //
        u8Hour = (__TIME__[0] - '0') * 10 + (__TIME__[1] - '0');

        //
        // Convert pre-processor variable __DATE__ to minute number 0..59
        //
        u8Minute = (__TIME__[3] - '0') * 10 + (__TIME__[4] - '0');

        //
        // Convert pre-processor variable __DATE__ to second number 0..59
        //
        u8Second = (__TIME__[6] - '0') * 10 + (__TIME__[7] - '0');

        //
        //  Convert decimal to BCD
        //
        u8Month = DEC_TO_BCD(u8Month);
        u8Day = DEC_TO_BCD(u8Day);
        u8Year = DEC_TO_BCD(u8Year);
        u8Second = DEC_TO_BCD(u8Second);
        u8Minute = DEC_TO_BCD(u8Minute);
        u8Hour = DEC_TO_BCD(u8Hour);

        //
        //  Start RTC
        //
        RTC->RTCCTL_b.RSTOP = 0;

        //
        //  Make writable
        //
        RTC->RTCCTL_b.WRTC = 1;

        //
        //  Store time/date
        //
        RTC->CTRLOW =  _VAL2FLD(RTC_CTRLOW_CTRSEC,u8Second)
                    | _VAL2FLD(RTC_CTRLOW_CTRMIN,u8Minute)
                    | _VAL2FLD(RTC_CTRLOW_CTRHR,u8Hour);
        RTC->CTRUP =  _VAL2FLD(RTC_CTRUP_CTRDATE,u8Day)
                    | _VAL2FLD(RTC_CTRUP_CTRMO,u8Month)
                    | _VAL2FLD(RTC_CTRUP_CTRYR,u8Year);

        //
        //  Make read only
        //
        RTC->RTCCTL_b.WRTC = 0;
    }
    
    while(1)
    {
        //application code

        //
        //  Read time/date
        //
        u32TmpLower = RTC->CTRLOW;
        u32TmpUpper = RTC->CTRUP;

        u8Second = _FLD2VAL(RTC_CTRLOW_CTRSEC,u32TmpLower);
        u8Second = BCD_TO_DEC(u8Second);

        u8Minute = _FLD2VAL(RTC_CTRLOW_CTRMIN,u32TmpLower);
        u8Minute = BCD_TO_DEC(u8Minute);

        u8Hour = _FLD2VAL(RTC_CTRLOW_CTRHR,u32TmpLower);
        u8Hour = BCD_TO_DEC(u8Hour);

        u8Day = _FLD2VAL(RTC_CTRUP_CTRDATE,u32TmpUpper);
        u8Day = BCD_TO_DEC(u8Day);

        u8Month = _FLD2VAL(RTC_CTRUP_CTRMO,u32TmpUpper);
        u8Month = BCD_TO_DEC(u8Month);

        u8Year = _FLD2VAL(RTC_CTRUP_CTRYR,u32TmpUpper);
        u8Year = BCD_TO_DEC(u8Year);

        printf ("Time: %02d:%02d:%02d, ", u8Hour, u8Minute, u8Second);
        printf ("Date: %04d/%02d/%02d\r\n", 2000 + u8Year, u8Month, u8Day);
        
        delay(1000);

    }
}



/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/

