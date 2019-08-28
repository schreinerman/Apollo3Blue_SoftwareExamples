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
 ** \brief Example for using Apollo3s IOM as SPI in direct command mode
 **
 ** \details Pin-Configuration:
 **          - pin 5, M0SCK
 **          - pin 6, M0MISO
 **          - pin 7, M0MOSI
 **          - pin 42, M0nCE0
 ** 
 **          pin 6 and 7 are connected by jumper-cable to loopback data 
 **
 ** History:
 **   - 2019-07-30  V1.0  Manuel Schreiner  First Version

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

#define IOM_MSBFIRST FALSE
#define IOM_LSBFIRST TRUE

#define MAX_FIFO_SIZE  64


/*****************************************************************************/
/* Global variable definitions (declared in header file with 'extern')       */
/*****************************************************************************/

uint32_t u32TransferredBytes = 0;
uint32_t au32DataOut[2] = {0x04030201,0x08070605};
uint32_t au32DataIn[2] = {0,0};

/*****************************************************************************/
/* Local type definitions ('typedef')                                        */
/*****************************************************************************/

typedef enum en_spi_mode
{
    SpiRead = 1,
    SpiWrite = 2,
    SpiReadWrite = 3
} en_spi_mode_t;

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
** \brief Send data via IOM as SPI
**  
** \param Handle         IOM handle, for example IOM1
**
** \param enMode         Mode: SpiRead, SpiWrite, SpiReadWrite
** 
** \param u8Chipselect   Chipselect channel
**
** \param pu8Offset      pointer to up to 3 bytes can be sent as offset, NULL if not used
**
** \param u8OffsetLen    length of offset 0..3
** 
** \param bContinued     if TRUE, chipselect keeps low
**
** \param au32DataOut    32-bit alligned data to be send
**
** \param au32DataIn     32-bit alligned data to be read
**
** \param u32LenBytes    size in bytes to transfer
**  
** \param pu32DataTransferred  pointer to data transferred variable
**  
*********************************************************************************/
void SpiSendData(IOM0_Type* Handle, en_spi_mode_t enMode, uint8_t u8Chipselect, uint8_t* pu8Offset, uint8_t u8OffsetLen, boolean_t bContinued, uint32_t* au32DataOut, uint32_t* au32DataIn, uint32_t u32LenBytes, uint32_t* pu32DataTransferred)
{
    uint8_t* au8DataIn = (uint8_t*)au32DataIn;
    uint8_t* au8DataOut = (uint8_t*)au32DataOut;
    uint32_t u32Cmd;
    uint32_t u32Irqbackup;
    int i;

    //
    // make sure interrupts are disabled before issuing commands
    //
    u32Irqbackup = Handle->INTEN;
    Handle->INTEN = 0;

    //
    // Clear all interrupt flags
    //
    Handle->INTCLR = 0xFFFFFFFF;


    u32Cmd = 0;

    //
    //  Check SPI RW mode and prepare command mode
    //
    switch(enMode)
    {
        case SpiWrite:
           u32Cmd |= _VAL2FLD(IOM0_CMD_CMD,1);
           Handle->MSPICFG_b.FULLDUP = 0;
           if (u32LenBytes > MAX_FIFO_SIZE) u32LenBytes = MAX_FIFO_SIZE;
           break;
        case SpiRead:
           u32Cmd |= _VAL2FLD(IOM0_CMD_CMD,2);
           Handle->MSPICFG_b.FULLDUP = 0;
           if (u32LenBytes > MAX_FIFO_SIZE) u32LenBytes = MAX_FIFO_SIZE;
           break;
        case SpiReadWrite:
           u32Cmd |= _VAL2FLD(IOM0_CMD_CMD,1);
           Handle->MSPICFG_b.FULLDUP = 1;
           if (u32LenBytes > MAX_FIFO_SIZE) u32LenBytes = MAX_FIFO_SIZE/2;
           break;
    }

    //
    //  prepare length of bytes to be transferred
    //
    u32Cmd |= _VAL2FLD(IOM0_CMD_TSIZE,u32LenBytes);

    //
    //  prepare chipselect channel
    //
    u32Cmd |= _VAL2FLD(IOM0_CMD_CMDSEL,u8Chipselect);

    //
    //  if offset is 1 and more bytes, write OFFSETLO field with first byte
    //
    if (u8OffsetLen > 0)
    {
        u32Cmd |= _VAL2FLD(IOM0_CMD_OFFSETLO,pu8Offset[0]);
    }

    //
    //  if offset is 2 and more bytes, write lower OFFSETHI register with second byte
    //
    if (u8OffsetLen > 1)
    {
        Handle->OFFSETHI = pu8Offset[1];
    }

    //
    //  if offset is 3 bytes, write higher OFFSETHI register with third byte
    //
    if (u8OffsetLen > 2)
    {
        Handle->OFFSETHI |= pu8Offset[2] << 8;
    }

    //
    //  Check if CS shall keep low after transmission
    //
    if (TRUE == bContinued)
    {
        u32Cmd |= _VAL2FLD(IOM0_CMD_CONT,1);
    }

    //
    // Check fifo has to be filled
    //
    if ((enMode == SpiWrite) || (enMode == SpiReadWrite))
    {
        for(i = 0;i < (u32LenBytes/4);i++)
        {
            Handle->FIFOPUSH = au32DataOut[i];
        }
    }

    //
    //  Issue command
    //
    Handle->CMD = u32Cmd;

    //
    // Restore enabled IRQs
    //
    Handle->INTEN = u32Irqbackup;
    
    //
    //  wait until transaction starts
    //
    while((Handle->STATUS_b.IDLEST == 1) || (Handle->STATUS_b.CMDACT == 0)) __NOP(); 

    //
    //  wait finished
    //
    while((Handle->STATUS_b.IDLEST == 0) || (Handle->STATUS_b.CMDACT == 1)) __NOP(); 

    //
    // Check fifo has to be read
    //
    if ((enMode == SpiRead) || (enMode == SpiReadWrite))
    {
        i = 0;
        while(Handle->FIFOPTR_b.FIFO1SIZ > 0)
        {
            au32DataIn[i] = Handle->FIFOPOP;
            i++;
            if (i > (u32LenBytes/4)) break;
        }
    }

    if (pu32DataTransferred != NULL) *pu32DataTransferred = i*4;
}


/**
*********************************************************************************
** \brief Configure IOM as SPI
**  
** \param Handle         IOM handle, for example IOM1
**
** \param bSPHA          SPHA setting (0/1)
** 
** \param bSPOL          SPOL setting (0/1)
**
** \param bLsbFirst      Can be IOM_MSBFIRST or IOM_LSBFIRST
**
** \param bFullDuplex    TRUE to enable fullduplex
** 
*********************************************************************************/
void SpiConfig(IOM0_Type* Handle, boolean_t bSPHA, boolean_t bSPOL, boolean_t bLsbFirst, boolean_t bFullDuplex)
{
    uint32_t u32Cfg, u32ClkCfg;
    
    PWRCTRL->DEVPWREN &= ~(1 << PWRCTRL_DEVPWREN_PWRIOM0_Pos);
    PWRCTRL->DEVPWREN |= (1 << PWRCTRL_DEVPWREN_PWRIOM0_Pos);

    //IOM modules 0,1 and 2 are contained in HCPB, while IOM modules 3,4 and 5 are contained in HCPC power domain
    //for IOM0, wait HCPB comes stable
    while(PWRCTRL->DEVPWRSTATUS_b.HCPB == 0) __NOP();

    Handle->SUBMODCTRL &= ~(1 << IOM0_SUBMODCTRL_SMOD0EN_Pos);
    Handle->SUBMODCTRL &= ~(1 << IOM0_SUBMODCTRL_SMOD1EN_Pos);

    u32ClkCfg = 0;
    u32ClkCfg |= _VAL2FLD(IOM0_CLKCFG_FSEL,IOM0_CLKCFG_FSEL_HFRC_DIV8); //using HFRC/8 as clock input --> 6MHz
    u32ClkCfg |= _VAL2FLD(IOM0_CLKCFG_DIV3,0); //no division by 3
    u32ClkCfg |= _VAL2FLD(IOM0_CLKCFG_DIVEN,0); //Disable TOTPER division
    u32ClkCfg |= _VAL2FLD(IOM0_CLKCFG_LOWPER,0);
    u32ClkCfg |= _VAL2FLD(IOM0_CLKCFG_TOTPER,0);
    u32ClkCfg |= _VAL2FLD(IOM0_CLKCFG_IOCLKEN,1); //enable clock
    Handle->CLKCFG = u32ClkCfg;

    Handle->MSPICFG = 0;
    Handle->MSPICFG_b.SPHA = bSPHA;
    Handle->MSPICFG_b.SPOL = bSPOL;
    Handle->MSPICFG_b.FULLDUP = bFullDuplex;
    Handle->MSPICFG_b.SPILSB = bLsbFirst;

    Handle->FIFOTHR = _VAL2FLD(IOM0_FIFOTHR_FIFOWTHR,8) | _VAL2FLD(IOM0_FIFOTHR_FIFORTHR,8);
    Handle->FIFOCTRL_b.FIFORSTN = 0;    
    Handle->FIFOCTRL_b.FIFORSTN = 1;

    if (Handle->SUBMODCTRL_b.SMOD0TYPE == IOM0_SUBMODCTRL_SMOD0TYPE_SPI_MASTER)
    {
         Handle->SUBMODCTRL_b.SMOD1EN = 0;
         Handle->SUBMODCTRL_b.SMOD0EN = 1; 
    } else
    { 
         Handle->SUBMODCTRL_b.SMOD1EN = 1;
         Handle->SUBMODCTRL_b.SMOD0EN = 0; 
    }
}

void GpioConfig(void)
{
    GPIO->PADKEY = 0x73; //unlock pad configuration

    //pin 5, M0SCK , FN1
    //pin 6, M0MISO, FN1
    //pin 7, M0MOSI, FN1
    //pin 42, M0nCE0, FN1
    GPIO->PADREGB_b.PAD5FNCSEL = GPIO_PADREGB_PAD5FNCSEL_M0SCK; //select function 1 (M0SCK) at PAD5 
    GPIO->PADREGB_b.PAD6FNCSEL = GPIO_PADREGB_PAD6FNCSEL_M0MISO; //select function 1 (M0MISO) at PAD6
    GPIO->PADREGB_b.PAD7FNCSEL = GPIO_PADREGB_PAD7FNCSEL_M0MOSI; //select function 1 (M0MOSI) at PAD7
    GPIO->PADREGK_b.PAD42FNCSEL = GPIO_PADREGK_PAD42FNCSEL_NCE42; //select function 1 (M0nCE0) at PAD42
    

    GPIO->CFGA_b.GPIO5OUTCFG = GPIO_CFGA_GPIO7OUTCFG_PUSHPULL;   //out-config push-pull
    GPIO->PADREGB_b.PAD5INPEN = 1;  //don't forget to input enable SCK, otherwise clock is generated continiously
    GPIO->PADREGB_b.PAD6INPEN = 1;  //input enable
    GPIO->CFGA_b.GPIO7OUTCFG = GPIO_CFGA_GPIO7OUTCFG_PUSHPULL;   //out-config push-pull
    GPIO->CFGF_b.GPIO42OUTCFG = 0; //M0nCE0 = 0x0 - FNCSEL = 0x1 - IOM0 nCE, Channel 0

    GPIO->PADKEY = 0; //lock pad configuration
}

void PrintDataOut(void)
{
    int i;
    printf("Sending data: \r\n");
    for(i = 0;i < 8;i++)
    {
         printf("TX: au8Data[%d]=%d\r\n",i,((uint8_t*) &au32DataOut[0])[i]);
    }
}

void PrintDataIn(void)
{
    int i;
    printf("Received data: \r\n");
    for(i = 0;i < 8;i++)
    {
         printf("RX: au8Data[%d]=%d\r\n",i,((uint8_t*) &au32DataIn[0])[i]);
    }
}


/**
 *****************************************************************************
 ** 
 **\brief Main function
 **
 *****************************************************************************/
int main(void)
{
    SystemCoreClockUpdate();                //update clock variable SystemCoreClock (defined by CMSIS)
    SysTick_Config(SystemCoreClock / 1000); //setup 1ms SysTick (defined by CMSIS)
    
    //application initialization area
    GpioConfig();

    //        Handle, SPHA, SPOL, MSB/LSB first, Fullduplex
    SpiConfig(IOM0,   0,    0,    IOM_MSBFIRST,  TRUE);


    //application code

    printf("\r\nLoopback MOSI pin7 -> MISO pin6 via jumper wire:\r\n");
    PrintDataOut();
    //          Handle, Mode,         Chipselect, Offsetbuffer, Offsetsize, Continue (keep CS low), Dataout,         Datain,         Len
    SpiSendData(IOM0,   SpiReadWrite, 0,          NULL,         0,          FALSE,                  &au32DataOut[0], &au32DataIn[0], 8, &u32TransferredBytes);
    PrintDataIn();



    //switch MOSI high / low (use as GPIO)
    GPIO->PADKEY = 0x73; //unlock pad configuration
    GPIO->CFGA_b.GPIO7OUTCFG = GPIO_CFGA_GPIO7OUTCFG_PUSHPULL;   //out-config push-pull
    GPIO->PADREGB_b.PAD7FNCSEL = GPIO_PADREGB_PAD7FNCSEL_GPIO7; //disable MOSI and use as GPIO
    GPIO->PADKEY = 0; //lock pad configuration



    GPIO->WTSA = 1 << 7; //set GPIO7 high
    printf("\r\nSetting MOSI all high:\r\n");
    //          Handle, Mode,         Chipselect, Offsetbuffer, Offsetsize, Continue (keep CS low), Dataout,         Datain,         Len
    SpiSendData(IOM0,   SpiReadWrite, 0,          NULL,         0,          FALSE,                  &au32DataOut[0], &au32DataIn[0], 8, &u32TransferredBytes);
    PrintDataIn();




    GPIO->WTCA = 1 << 7; //set GPIO7 low
    printf("\r\nSetting MOSI all low:\r\n");
    //          Handle, Mode,         Chipselect, Offsetbuffer, Offsetsize, Continue (keep CS low), Dataout,         Datain,         Len
    SpiSendData(IOM0,   SpiReadWrite, 0,          NULL,         0,          FALSE,                  &au32DataOut[0], &au32DataIn[0], 8, &u32TransferredBytes);
    PrintDataIn();




    //restore GPIO config
    GpioConfig();



    while(1)
    {
        //application code
    }
}



/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/

