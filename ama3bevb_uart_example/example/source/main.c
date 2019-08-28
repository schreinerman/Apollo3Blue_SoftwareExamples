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
 ** \brief Simple UART example
 **
 ** Main Module
 **
 ** History:
 ** 2019-03-19  V1.0.0  LEh first version
 **
 ******************************************************************************/

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "mcu.h"
#include "base_types.h"

#define NEWLINE "\r\n"

/******************************************************************************/
/* Prototype functions                                                        */
/******************************************************************************/

void ConfigureBaudrate(UART0_Type* pstcUart,uint32_t u32Baudrate, uint32_t u32UartClkFreq);
void PutCharUart(UART0_Type* pstcUart, uint8_t u8Char);
void PutStringUart(UART0_Type* pstcUart, char_t *pu8Buffer);
uint8_t GetCharUart(UART0_Type* pstcUart);
void UartInit(UART0_Type* pstcUart,uint32_t u32Baudrate);

/******************************************************************************/
/* Procedures and functions                                                   */
/******************************************************************************/

/**
 ******************************************************************************
 ** \brief  Init UART...
 **
 ** \param  pstcUart         UART pointer
 **
 ** \param  u32Baudrate      Baudrate
 **
 *****************************************************************************/
void UartInit(UART0_Type* pstcUart,uint32_t u32Baudrate)
{
    //
    // Enable UART clocking
    //
  
    PWRCTRL->DEVPWREN |= (1 << PWRCTRL_DEVPWREN_PWRUART0_Pos);

    while(PWRCTRL->DEVPWRSTATUS_b.HCPA == 0) __NOP();

    //
    // Enable clock / select clock...
    //
    pstcUart->CR = 0;
    pstcUart->CR_b.CLKEN = 1;                 //enable clock
    pstcUart->CR_b.CLKSEL = 1;                //use 24MHz clock
    
    //
    // Disable UART before config...
    //
    pstcUart->CR_b.UARTEN = 0;                //disable UART
    pstcUart->CR_b.RXE = 0;                   //disable receiver
    pstcUart->CR_b.TXE = 0;                   //disable transmitter
    
    
    //
    // Starting UART config...
    //
    
    // initialize baudrate before all other settings, otherwise UART will not be initialized
    SystemCoreClockUpdate();
    ConfigureBaudrate(pstcUart,u32Baudrate,24000000UL);
    
    // initialize line coding...
    pstcUart->LCRH = 0;
    pstcUart->LCRH_b.WLEN = 3;                //3 = 8 data bits (2..0 = 7..5 data bits)
    pstcUart->LCRH_b.STP2 = 0;                //1 stop bit
    pstcUart->LCRH_b.PEN = 0;                 //no parity
    
    //
    // Enable UART after config...
    //
    pstcUart->CR_b.UARTEN = 1;                //enable UART
    pstcUart->CR_b.RXE = 1;                   //enable receiver
    pstcUart->CR_b.TXE = 1;                   //enable transmitter
}

/**
 ******************************************************************************
 ** \brief  Set baudrate
 **
 ** \param  pstcUart         UART pointer
 **
 ** \param  u32Baudrate      Baudrate
 **
 ** \param  u32UartClkFreq   UART clock
 **
 *****************************************************************************/
void ConfigureBaudrate(UART0_Type* pstcUart,uint32_t u32Baudrate, uint32_t u32UartClkFreq)
{
    uint64_t u64FractionDivisorLong;
    uint64_t u64IntermediateLong;
    uint32_t u32IntegerDivisor;
    uint32_t u32FractionDivisor;
    uint32_t u32BaudClk;
    
    //
    // Calculate register values.
    //
    u32BaudClk = 16 * u32Baudrate;
    u32IntegerDivisor = (uint32_t)(u32UartClkFreq / u32BaudClk);
    u64IntermediateLong = (u32UartClkFreq * 64) / u32BaudClk;
    u64FractionDivisorLong = u64IntermediateLong - (u32IntegerDivisor * 64);
    u32FractionDivisor = (uint32_t)u64FractionDivisorLong;
    
    //
    // Integer divisor MUST be greater than or equal to 1.
    //
    if(u32IntegerDivisor == 0)
    {
        //
        // Spin in a while because the selected baudrate is not possible.
        //
        while(1);
    }
    //
    // Write the UART regs.
    //
    pstcUart->IBRD = u32IntegerDivisor;
    pstcUart->IBRD = u32IntegerDivisor;
    pstcUart->FBRD = u32FractionDivisor;
}

/**
 ******************************************************************************
 ** \brief  sends a single character  (no timeout !)
 **
 ** \param  pstcUart  UART pointer
 **
 ** \param  u8Char    Data to send
 **
 *****************************************************************************/
void PutCharUart(UART0_Type* pstcUart, uint8_t u8Char)
{
    while(pstcUart->FR_b.TXFF) __NOP();
    pstcUart->DR = u8Char;
}

/**
 ******************************************************************************
 ** \brief  sends a complete string (0-terminated) 
 **
 ** \param  pstcUart  UART pointer
 **
 ** \param  Pointer to (constant) file of bytes in mem
 **
 *****************************************************************************/
void PutStringUart(UART0_Type* pstcUart, char_t *pu8Buffer)
{
  while (*pu8Buffer != '\0')
  { 
    PutCharUart(pstcUart,*pu8Buffer++);        // send every char of string
  }
}

/**
 ******************************************************************************
 ** \brief  returns incoming character, if received on UART0
 **
 ** \param  pstcUart  UART pointer
 **
 ** \return Character or 0xFF (Error) or 0 (Nothing)
 *****************************************************************************/
uint8_t GetCharUart(UART0_Type* pstcUart)   
{
  uint8_t u8Char;
  
  //
  // Wait for data, i.e. RX FIFO NOT EMPTY.
  //
  while(pstcUart->FR_b.RXFE) __NOP();

  //
  // Save the char.
  //
  u8Char = pstcUart->DR;
  return u8Char;
}

/**
 ******************************************************************************
 ** \brief  Main function
 **
 ** \return int return value, if needed
 ******************************************************************************/
int main(void)
{
    uint8_t u8Char;

    UartInit(UART0,115200);                //init UART with 115200 baud

    GPIO->PADKEY = 0x00000073;            //unlock pin selection
    
    GPIO->PADREGF_b.PAD22FNCSEL = 0;      //set UARTTX on pin 22
    GPIO->PADREGF_b.PAD22INPEN = 1;       //enable input on pin 22
    GPIO->CFGC_b.GPIO22OUTCFG = 1;        //output is push-pull
    

    GPIO->PADREGF_b.PAD23FNCSEL = 0;      //set UARTRX on pin 23
    GPIO->PADREGF_b.PAD23INPEN = 1;       //enable input on pin 23
    GPIO->CFGC_b.GPIO23OUTCFG = 0;        //output is disabled
    
    GPIO->PADKEY = 0;                     //lock pin selection
    
    PutStringUart(UART0,NEWLINE);
    PutStringUart(UART0,"Hello World!"NEWLINE);
    PutStringUart(UART0,"Typed text will be echoed..."NEWLINE);

    while(1)
    {
        // Repeat UART input
        u8Char = GetCharUart(UART0);
        if (u8Char != 0)
        {
          PutCharUart(UART0,u8Char);
        }
        
    }
    return 0;
}

/*****************************************************************************/
/* EOF (not truncated)                                                       */
/*****************************************************************************/
