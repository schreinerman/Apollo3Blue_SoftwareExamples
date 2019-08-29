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
/** \file RTE_Device.h
 **
 ** A detailed description is available at 
 ** @link RTE_DeviceGroup  description @endlink
 **
 ** History:

 *****************************************************************************/
#ifndef __RTE_DEVICE_H__
#define __RTE_DEVICE_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
 ******************************************************************************
 ** \defgroup RTE_DeviceGroup 
 **
 ** Provided functions of RTE_Device:
 ** 
 **   
 ******************************************************************************/
//@{

/**
 ******************************************************************************    
 ** \page rte_device_module_includes Required includes in main application
 ** \brief Following includes are required
 ** @code   
 ** #include "rte_device.h"   
 ** @endcode
 **
 ******************************************************************************/
    
/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/


/*****************************************************************************/
/* Global pre-processor symbols/macros ('#define')                            */
/*****************************************************************************/
#define RTE_DEVICE_AMA3B1KK_KCR 2
#define RTE_DEVICE_AMA3B1KK_KBR 1

#warning Please select your device here or in the compiler defines section (default set):
#define RTE_DEVICE_SELECTION RTE_DEVICE_AMA3B1KK_KCR

//Defines for AMA3B1KK-KBR
#if RTE_DEVICE_SELECTION == RTE_DEVICE_AMA3B1KK_KBR
    #if !defined(APOLLO3_1024)
        #define APOLLO3_1024
    #endif /* end APOLLO3_1024*/
#endif

//Defines for AMA3B1KK-KBR
#if RTE_DEVICE_SELECTION == RTE_DEVICE_AMA3B1KK_KBR
    #if !defined(APOLLO3_1024)
        #define APOLLO3_1024
    #endif /* end APOLLO3_1024*/
#endif

//Defines for AMA3B1KK-KCR
#if RTE_DEVICE_SELECTION == RTE_DEVICE_AMA3B1KK_KCR
    #if !defined(APOLLO3_1024)
        #define APOLLO3_1024
    #endif /* end APOLLO3_1024*/
#endif

//Defines for AMA3B1KK-KCR
#if RTE_DEVICE_SELECTION == RTE_DEVICE_AMA3B1KK_KCR
    #if !defined(APOLLO3_1024)
        #define APOLLO3_1024
    #endif /* end APOLLO3_1024*/
#endif


/*****************************************************************************/
/* Global type definitions ('typedef')                                        */
/*****************************************************************************/


/*****************************************************************************/
/* Global variable declarations ('extern', definition in C source)           */
/*****************************************************************************/



/*****************************************************************************/
/* Global function prototypes ('extern', definition in C source)             */
/*****************************************************************************/


#ifdef __cplusplus
}
#endif

//@} // RTE_DeviceGroup

#endif /*__RTE_DEVICE_H__*/

/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/

