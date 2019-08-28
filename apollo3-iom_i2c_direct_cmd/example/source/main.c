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
 ** \brief Example for using Apollo3s IOM as I2C in direct command mode
 **
 ** \details Pin-Configuration:
 **          - pin 8, M0SCK
 **          - pin 9, M0SDA
 ** 
 **          I2C is used to scan for devices at the bus
 **
 ** History:
 **   - 2019-08-6  V1.0  Manuel Schreiner  First Version

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


/*****************************************************************************/
/* Local type definitions ('typedef')                                        */
/*****************************************************************************/

typedef enum en_i2c_mode
{
    I2CRead = 1,
    I2CWrite = 2
} en_i2c_mode_t;

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
** \brief Transfer data via IOM as I2C
**  
** \param Handle         IOM handle, for example IOM1
**
** \param enMode         Mode: I2CRead, I2CWrite
** 
** \param u16Address     Device Address
**
** \param pu8Offset      pointer to up to 3 bytes can be sent as offset, NULL if not used
**
** \param u8OffsetLen    length of offset 0..3
** 
** \param bNoStop        if TRUE, no stop condition is sent
**
** \param au32Data       32-bit alligned data to be send
**
** \param u32LenBytes    size in bytes to transfer
**  
** \param pu32DataTransferred  pointer to data transferred variable
**  
*********************************************************************************/
void I2CTransfer(IOM0_Type* Handle, en_i2c_mode_t enMode, uint16_t u16Address, uint8_t* pu8Offset, uint8_t u8OffsetLen, boolean_t bNoStop, uint32_t* au32Data, uint32_t u32LenBytes, uint32_t* pu32DataTransferred)
{
    uint32_t u32Cmd;
    uint32_t u32Irqbackup;
    volatile uint32_t u32Timeout;
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

    Handle->DEVCFG_b.DEVADDR = u16Address;

    u32Cmd = 0;

    //
    //  Check I2C RW mode and prepare command mode
    //
    switch(enMode)
    {
        case I2CWrite:
           u32Cmd |= _VAL2FLD(IOM0_CMD_CMD,1);
           Handle->MSPICFG_b.FULLDUP = 0;
           if (u32LenBytes > MAX_FIFO_SIZE) u32LenBytes = MAX_FIFO_SIZE;
           break;
        case I2CRead:
           u32Cmd |= _VAL2FLD(IOM0_CMD_CMD,2);
           Handle->MSPICFG_b.FULLDUP = 0;
           if (u32LenBytes > MAX_FIFO_SIZE) u32LenBytes = MAX_FIFO_SIZE;
           break;
    }

    //
    //  prepare length of bytes to be transferred
    //
    u32Cmd |= _VAL2FLD(IOM0_CMD_TSIZE,u32LenBytes);

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
    if (TRUE == bNoStop)
    {
        u32Cmd |= _VAL2FLD(IOM0_CMD_CONT,1);
    }

    //
    // Check fifo has to be filled
    //
    if (enMode == I2CWrite)
    {
        for(i = 0;i < (u32LenBytes/4);i++)
        {
            Handle->FIFOPUSH = au32Data[i];
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
    u32Timeout = 100000;
    while((Handle->STATUS_b.IDLEST == 0) || (Handle->STATUS_b.CMDACT == 1))
    {
        u32Timeout--;
        if (u32Timeout == 0) break;
    }

    //
    // Check fifo has to be read
    //
    if (enMode == I2CRead)
    {
        i = 0;
        while(Handle->FIFOPTR_b.FIFO1SIZ > 0)
        {
            au32Data[i] = Handle->FIFOPOP;
            i++;
            if (i > (u32LenBytes/4)) break;
        }
    }

    if (pu32DataTransferred != NULL) *pu32DataTransferred = i*4; 
}

/**
*********************************************************************************
** \brief Write 1 byte on data
**  
** \param Handle         IOM handle, for example IOM1
** 
** \param u16Address     Device Address
**
** \param u8Val          Value
**
** \param pbNak          Pointer to where the NAK information will be stored (FALSE == ACK)
**  
*********************************************************************************/
void I2CWrite8(IOM0_Type* Handle,uint16_t u16Address,uint32_t u8Val, boolean_t* pbNak)
{
    uint32_t u32TransferredBytes;
    I2CTransfer(Handle,I2CWrite,u16Address,NULL,0,FALSE,&u8Val,1,&u32TransferredBytes);
    if (pbNak != NULL)
    {
        *pbNak = Handle->INTSTAT_b.NAK;
        if (u32TransferredBytes == 0) *pbNak = TRUE;
    }
}

/**
*********************************************************************************
** \brief Write 1 byte on data into a 8-bit register
**  
** \param Handle         IOM handle, for example IOM1
** 
** \param u16Address     Device Address
**
** \param u8Reg          Register
**
** \param u8Val          Value
**
** \param pbNak          Pointer to where the NAK information will be stored (FALSE == ACK)
**  
*********************************************************************************/
void I2CWriteReg8Val8(IOM0_Type* Handle,uint16_t u16Address,uint32_t u8Reg,uint32_t u8Val, boolean_t* pbNak)
{
    uint32_t u32TransferredBytes;
    I2CTransfer(Handle,I2CWrite,u16Address,(uint8_t*)&u8Reg,1,FALSE,&u8Val,1,&u32TransferredBytes);
    if (pbNak != NULL)
    {
        *pbNak = Handle->INTSTAT_b.NAK;
        if (u32TransferredBytes == 0) *pbNak = TRUE;
    }
}

/**
*********************************************************************************
** \brief Write 1 16-bit word on data into a 16-bit register
**  
** \param Handle         IOM handle, for example IOM1
** 
** \param u16Address     Device Address
**
** \param u16Reg          Register
**
** \param u16Val          Value
**
** \param pbNak          Pointer to where the NAK information will be stored (FALSE == ACK)
**  
*********************************************************************************/
void I2CWriteReg16Val16(IOM0_Type* Handle,uint16_t u16Address,uint32_t u16Reg,uint32_t u16Val, boolean_t* pbNak)
{
    uint32_t u32TransferredBytes;
    I2CTransfer(Handle,I2CWrite,u16Address,(uint8_t*)&u16Reg,2,FALSE,&u16Val,2,&u32TransferredBytes);
    if (pbNak != NULL)
    {
        *pbNak = Handle->INTSTAT_b.NAK;
        if (u32TransferredBytes == 0) *pbNak = TRUE;
    }
}

/**
*********************************************************************************
** \brief Read 1 byte on data
**  
** \param Handle         IOM handle, for example IOM1
** 
** \param u16Address     Device Address
**
** \param pbNak          Pointer to where the NAK information will be stored (FALSE == ACK)
**
** \return   data
**  
*********************************************************************************/
uint8_t I2CRead8(IOM0_Type* Handle,uint16_t u16Address,boolean_t* pbNak)
{
    uint32_t u32TransferredBytes;
    uint32_t u32Tmp;
    I2CTransfer(Handle,I2CRead,u16Address,NULL,0,FALSE,&u32Tmp,1,&u32TransferredBytes);
    if (pbNak != NULL)
    {
        *pbNak = Handle->INTSTAT_b.NAK;
        if (u32TransferredBytes == 0) *pbNak = TRUE;
    }
    return u32Tmp;
}

/**
*********************************************************************************
** \brief Read 1 byte on data into a 8-bit register
**  
** \param Handle         IOM handle, for example IOM1
** 
** \param u16Address     Device Address
**
** \param u8Reg          Register
**
** \param pbNak          Pointer to where the NAK information will be stored (FALSE == ACK)
**
** \return   data
**  
*********************************************************************************/
uint8_t I2CReadReg8Val8(IOM0_Type* Handle,uint16_t u16Address,uint32_t u8Reg, boolean_t* pbNak)
{
    uint32_t u32TransferredBytes;
    uint32_t u32Tmp;
    I2CTransfer(Handle,I2CRead,u16Address,(uint8_t*)&u8Reg,1,FALSE,&u32Tmp,1,&u32TransferredBytes);
    if (pbNak != NULL)
    {
        *pbNak = Handle->INTSTAT_b.NAK;
        if (u32TransferredBytes == 0) *pbNak = TRUE;
    }
    return u32Tmp;
}

/**
*********************************************************************************
** \brief Read 1 16-bit word on data into a 16-bit register
**  
** \param Handle         IOM handle, for example IOM1
** 
** \param u16Address     Device Address
**
** \param u16Reg          Register
**
** \param pbNak          Pointer to where the NAK information will be stored (FALSE == ACK)
**
** \return   data
**  
*********************************************************************************/
uint16_t I2CReadReg16Val16(IOM0_Type* Handle,uint16_t u16Address,uint32_t u16Reg, boolean_t* pbNak)
{
    uint32_t u32TransferredBytes;
    uint32_t u32Tmp;
    I2CTransfer(Handle,I2CRead,u16Address,(uint8_t*)&u16Reg,2,FALSE,&u32Tmp,2,&u32TransferredBytes);
    if (pbNak != NULL)
    {
        *pbNak = Handle->INTSTAT_b.NAK;
        if (u32TransferredBytes == 0) *pbNak = TRUE;
    }
    return u32Tmp;
}

/**
*********************************************************************************
** \brief Configure IOM as SPI
**  
** \param Handle             IOM handle, for example IOM1
**
** \param u32Frequency       Frequency
**
** \param b10BitAddresses    TRUE to enable 10-bit addressing (else 7-bit)
** 
*********************************************************************************/
void I2CConfig(IOM0_Type* Handle, uint32_t u32Frequency, boolean_t b10BitAddresses)
{
    uint32_t u32Cfg, u32ClkCfg;

    //
    // Power Control
    //
    switch((int)Handle)
    {
        case (int)IOM0:
            PWRCTRL->DEVPWREN &= ~(1 << PWRCTRL_DEVPWREN_PWRIOM0_Pos);
            PWRCTRL->DEVPWREN |= (1 << PWRCTRL_DEVPWREN_PWRIOM0_Pos);
            //IOM modules 0,1 and 2 are contained in HCPB, while IOM modules 3,4 and 5 are contained in HCPC power domain
            //for IOM0, wait HCPB comes stable
            while(PWRCTRL->DEVPWRSTATUS_b.HCPB == 0) __NOP();
            break;
        case (int)IOM1:
            PWRCTRL->DEVPWREN &= ~(1 << PWRCTRL_DEVPWREN_PWRIOM1_Pos);
            PWRCTRL->DEVPWREN |= (1 << PWRCTRL_DEVPWREN_PWRIOM1_Pos);
            //IOM modules 0,1 and 2 are contained in HCPB, while IOM modules 3,4 and 5 are contained in HCPC power domain
            //for IOM1, wait HCPB comes stable
            while(PWRCTRL->DEVPWRSTATUS_b.HCPB == 0) __NOP();
            break;
        case (int)IOM2:
            PWRCTRL->DEVPWREN &= ~(1 << PWRCTRL_DEVPWREN_PWRIOM2_Pos);
            PWRCTRL->DEVPWREN |= (1 << PWRCTRL_DEVPWREN_PWRIOM2_Pos);
            //IOM modules 0,1 and 2 are contained in HCPB, while IOM modules 3,4 and 5 are contained in HCPC power domain
            //for IOM2, wait HCPB comes stable
            while(PWRCTRL->DEVPWRSTATUS_b.HCPB == 0) __NOP();
            break;
        case (int)IOM3:
            PWRCTRL->DEVPWREN &= ~(1 << PWRCTRL_DEVPWREN_PWRIOM3_Pos);
            PWRCTRL->DEVPWREN |= (1 << PWRCTRL_DEVPWREN_PWRIOM3_Pos);
            //IOM modules 0,1 and 2 are contained in HCPB, while IOM modules 3,4 and 5 are contained in HCPC power domain
            //for IOM3, wait HCPB comes stable
            while(PWRCTRL->DEVPWRSTATUS_b.HCPC == 0) __NOP();
            break;
        case (int)IOM4:
            PWRCTRL->DEVPWREN &= ~(1 << PWRCTRL_DEVPWREN_PWRIOM4_Pos);
            PWRCTRL->DEVPWREN |= (1 << PWRCTRL_DEVPWREN_PWRIOM4_Pos);
            //IOM modules 0,1 and 2 are contained in HCPB, while IOM modules 3,4 and 5 are contained in HCPC power domain
            //for IOM4, wait HCPB comes stable
            while(PWRCTRL->DEVPWRSTATUS_b.HCPC == 0) __NOP();
            break;
        case (int)IOM5:
            PWRCTRL->DEVPWREN &= ~(1 << PWRCTRL_DEVPWREN_PWRIOM5_Pos);
            PWRCTRL->DEVPWREN |= (1 << PWRCTRL_DEVPWREN_PWRIOM5_Pos);
            //IOM modules 0,1 and 2 are contained in HCPB, while IOM modules 3,4 and 5 are contained in HCPC power domain
            //for IOM4, wait HCPB comes stable
            while(PWRCTRL->DEVPWRSTATUS_b.HCPC == 0) __NOP();
            break;
    }
 
    //
    //make sure IOM is disabled
    //
    Handle->SUBMODCTRL &= ~(1 << IOM0_SUBMODCTRL_SMOD0EN_Pos);
    Handle->SUBMODCTRL &= ~(1 << IOM0_SUBMODCTRL_SMOD1EN_Pos);

    u32ClkCfg = 0;
    //
    // Clockseetings can be 100KHz, 400KHz or 1MHz
    //
    if (u32Frequency <= 100000UL)
    {
        u32ClkCfg = 2 << IOM0_CLKCFG_FSEL_Pos;        
        u32ClkCfg |= 1 << IOM0_CLKCFG_DIVEN_Pos;      
        u32ClkCfg |= 0x77 << IOM0_CLKCFG_TOTPER_Pos;   
        u32ClkCfg |= 0x3B << IOM0_CLKCFG_LOWPER_Pos;    
        Handle->CLKCFG = u32ClkCfg;          
        Handle->MI2CCFG = 0;
        Handle->MI2CCFG_b.SMPCNT = 3;             
        Handle->MI2CCFG_b.SDAENDLY = 15;          
        Handle->MI2CCFG_b.SCLENDLY = 0;          
        Handle->MI2CCFG_b.MI2CRST = 0;           
    } else if (u32Frequency <= 400000UL)
    {
        u32ClkCfg = 2 << IOM0_CLKCFG_FSEL_Pos;       
        u32ClkCfg |= 1 << IOM0_CLKCFG_DIVEN_Pos;      
        u32ClkCfg |= 0x1D << IOM0_CLKCFG_TOTPER_Pos;    
        u32ClkCfg |= 0x0E << IOM0_CLKCFG_LOWPER_Pos;   
        Handle->CLKCFG = u32ClkCfg;
        Handle->MI2CCFG = 0;
        Handle->MI2CCFG_b.SMPCNT = 15;           
        Handle->MI2CCFG_b.SDAENDLY = 15;          
        Handle->MI2CCFG_b.SCLENDLY = 2;        
        Handle->MI2CCFG_b.MI2CRST = 0;          
       
    } else
    {
        u32ClkCfg = 3 << IOM0_CLKCFG_FSEL_Pos; 
        u32ClkCfg |= 1 << IOM0_CLKCFG_DIVEN_Pos;
        u32ClkCfg |= 6 << IOM0_CLKCFG_TOTPER_Pos;
        u32ClkCfg |= 3 << IOM0_CLKCFG_LOWPER_Pos;
        u32ClkCfg |= (1 << IOM0_CLKCFG_IOCLKEN_Pos);
        Handle->CLKCFG = u32ClkCfg;
        Handle->MI2CCFG = 0;
        Handle->MI2CCFG_b.SMPCNT = 0x21;            
        Handle->MI2CCFG_b.SDAENDLY = 3;
        Handle->MI2CCFG_b.SCLENDLY = 0;
        Handle->MI2CCFG_b.MI2CRST = 0;           
    }
    u32ClkCfg |= (1 << IOM0_CLKCFG_IOCLKEN_Pos);
    Handle->CLKCFG = u32ClkCfg;

    Handle->MI2CCFG_b.ADDRSZ = b10BitAddresses;
    Handle->MSPICFG = 0;

    Handle->FIFOTHR = _VAL2FLD(IOM0_FIFOTHR_FIFOWTHR,8) | _VAL2FLD(IOM0_FIFOTHR_FIFORTHR,8);
    Handle->FIFOCTRL_b.FIFORSTN = 0;    
    Handle->FIFOCTRL_b.FIFORSTN = 1;

    //
    // Enable IOM
    //
    if (Handle->SUBMODCTRL_b.SMOD0TYPE == IOM0_SUBMODCTRL_SMOD0TYPE_I2C_MASTER)
    {
         Handle->SUBMODCTRL_b.SMOD1EN = 0;
         Handle->SUBMODCTRL_b.SMOD0EN = 1; 
    } else
    { 
         Handle->SUBMODCTRL_b.SMOD1EN = 1;
         Handle->SUBMODCTRL_b.SMOD0EN = 0; 
    }
}

/**
 *****************************************************************************
 ** 
 **\brief Setup GPIOs
 **
 *****************************************************************************/
void GpioConfig(void)
{
    GPIO->PADKEY = 0x73; //unlock pad configuration

    //pin 8, M1SCL , FN1
    //pin 9, M1SDAWIR3, FN1
    GPIO->PADREGC_b.PAD8FNCSEL = GPIO_PADREGC_PAD8FNCSEL_M1SCL; //select function 1 (M0SCK) at PAD5 
    GPIO->PADREGC_b.PAD9FNCSEL = GPIO_PADREGC_PAD9FNCSEL_M1SDAWIR3; //select function 1 (M0MISO) at PAD6

    GPIO->CFGB_b.GPIO8OUTCFG = GPIO_CFGB_GPIO8OUTCFG_OD;   //out-config open-drain
    GPIO->CFGB_b.GPIO9OUTCFG = GPIO_CFGB_GPIO9OUTCFG_OD;   //out-config open-drain
    GPIO->PADREGC_b.PAD8INPEN = 1;  //input enable
    GPIO->PADREGC_b.PAD9INPEN = 1;  //input enable
    
    //
    // Enable 1.5k pull-ups
    //
    GPIO->PADREGC_b.PAD8RSEL = GPIO_PADREGC_PAD8RSEL_PULL1_5K; 
    GPIO->PADREGC_b.PAD8PULL = 1;
    GPIO->PADREGC_b.PAD9RSEL = GPIO_PADREGC_PAD9RSEL_PULL1_5K; 
    GPIO->PADREGC_b.PAD9PULL = 1;

    GPIO->PADKEY = 0; //lock pad configuration
}



/**
 *****************************************************************************
 ** 
 **\brief Main function
 **
 *****************************************************************************/
int main(void)
{
    boolean_t bNak;
    int i;
    SystemCoreClockUpdate();                //update clock variable SystemCoreClock (defined by CMSIS)
    SysTick_Config(SystemCoreClock / 1000); //setup 1ms SysTick (defined by CMSIS)
    
    //application initialization area
    GpioConfig();

    I2CConfig(IOM1, 400000, FALSE);


    for(i = 1;i < 128;i++)
    {
        printf("Device at %d?  ",i);
        I2CReadReg8Val8(IOM1,i,0,&bNak);
        if (bNak == FALSE)
        {
            printf("YES\r\n");
        } else
        {
            printf("NO\r\n");
        }
    }


    while(1)
    {
        //application code
    }
}



/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/

