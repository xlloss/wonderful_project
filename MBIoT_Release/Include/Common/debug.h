/********************************************************************
  File Information:
    FileName:       debug.h
    Processor:      8051
    Hardware:
    Complier:       Keil C51
    Company:        Microchip Technology, Inc.

    Software License Agreement:

    The software supplied herewith by Microchip Technology Incorporated
    (the "Company") for 8051 Microcontroller is intended and supplied 
    to you, the Company's customer, for use solely and exclusively on 
    Microchip 8051 Microcontroller products. The software is owned by 
    the Company and/or its supplier, and is protected under applicable 
    copyright laws. All rights are reserved. Any use in violation of 
    the foregoing restrictions may subject the user to criminal 
    sanctions under applicable laws, as well as to civil liability for 
    the breach of the terms and conditions of this license.

    THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
    WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
    TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
    IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
    CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

********************************************************************/

/**
  @addtogroup DEBUG Debug
  @{
  @brief Definitions and prototypes for the Debug application programming interface.
 */
 
#ifndef DEBUG_H
#define DEBUG_H

/**@brief Maximum length of log text. */
#define MAX_DEBUG_LOG_LENGTH			64

/**
 *@brief Print out debug information
 * 
 *@param[in] length		Length of debug information.
 *@param[in] p_buffer	Pointer to debug information.
 *
 *@return MBIOT_RES_SUCCESS Successfully send out the log information.
 *@return MBIOT_RES_NO_RESOURCE 	UART tx buffer is full.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 */
PUBLIC extern u16 APP_DEBUG_Log(u8 length, u8 XDATA *p_buffer) large;

#endif 
 /**
  @}
*/