/********************************************************************
  File Information:
    FileName:       mbiot_error.h
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


#ifndef MBIOT_ERROR_H
#define MBIOT_ERROR_H

#define MBIOT_RES_SUCCESS						0x0000									/**< Execution successfully. */
#define MBIOT_RES_FAIL							0x0001									/**< Execution fail. */
#define MBIOT_RES_OOM							0x0002									/**< Out of memory. */
#define MBIOT_RES_INVALID_PARA					0x0003									/**< Invalid parameters. */
#define MBIOT_RES_NO_RESOURCE					0x0004									/**< No resource. */

#define MBIOT_RES_ATT_BASE						0x0100				/**< ATT based error code. */
#define MBIOT_RES_PERIPHERAL_BASE				0x0800				/**< Peripheral based error code. */
#define MTIOT_RES_PERIPHERAL_I2C_BASE			(MBIOT_RES_PERIPHERAL_BASE+0x0010)		/**< I2C based error code. */

#endif
