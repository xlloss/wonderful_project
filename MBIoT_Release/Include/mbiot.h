/********************************************************************
  File Information:
    FileName:       mbiot.h
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
 
#ifndef MBIOT_H
#define MBIOT_H

#include "mbiot_symbdefs.h"
#include "mbiot_xmemlib.h"
#include "mbiot_error.h"
#include "mbiot_calculatelib.h"

#include "ble_l2cap.h"
#include "ble_smp.h"
#include "ble_gatt.h"
#include "ble_gatts_services_table.h"
#include "ble_gap.h"
#include "peripheral.h"
#include "common.h"

//#define APP_OPEN_DEBUG_MODE

/**
 *@brief MBIoT main init entry point.
 */
PUBLIC extern void Application_Init() large reentrant;

#endif    
