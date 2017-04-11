/********************************************************************
  File Information:
    FileName:       ble_immediate_alert_service.c
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
#include "mbiot.h"
#include "ble_gatt.h"
#include "ble_immediate_alert_service.h"

PUBLIC  MCODE  u8 APP_Defined_Immediate_Alert_Services[]={	
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_SERVICE_2 */
	0x0A,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_PRIMARY_SERVICE>>8,UUID_PRIMARY_SERVICE,					0x00,0x00,	UUID_IMMEDIATE_ALERT>>8,UUID_IMMEDIATE_ALERT,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */
	0x0B,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						WRITE_WITHOUT_RESPONSE,	0x00,0x00,	UUID_ALERT_LEVEL>>8,UUID_ALERT_LEVEL,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE */
	0x08,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_ALERT_LEVEL>>8,UUID_ALERT_LEVEL,			0x01,	0x00,
};

PUBLIC XDATA IASProcessRoutine	IASProcess;

PUBLIC u16 XDATA immeadiateAlertHandle;

PUBLIC u16 BLE_IAS_Init(IASProcessRoutine iasHandler) large
{
	if(iasHandler==NULL)
	{
		return MBIOT_RES_INVALID_PARA;
	}
	IASProcess=iasHandler;
	
	return MBIOT_RES_SUCCESS;
}

PUBLIC u16 BLE_IAS_Init_Service() large
{	
	XDATA u16 startIASHandle;
	XDATA BLE_GATT_ServicesInfo serviceInfo;
	
	serviceInfo.p_definedService = (u8 XDATA *)APP_Defined_Immediate_Alert_Services;
	serviceInfo.attributeNum = TOTAL_IAS_ATTR_NUM;
	serviceInfo.serviceSize = sizeof(APP_Defined_Immediate_Alert_Services);
	BLE_GATTS_InitServiceTable(&serviceInfo, &startIASHandle);
	
	immeadiateAlertHandle = startIASHandle+INDEX_IAS_ALERT_LEVEL;
	
	return MBIOT_RES_SUCCESS;
}

static void BLE_IAS_Event_Handler(BLE_GATT_Event XDATA * p_event) large
{	
	XDATA u8 alertLevel;
	XDATA u16 handle;
	
	handle = p_event->eventField.onWrite.charHandle;
	if(handle == immeadiateAlertHandle)
	{
		alertLevel = p_event->eventField.onWrite.p_charValue[0];
		IASProcess(alertLevel);
	}
}

PUBLIC void BLE_IAS_Catch_Event(BLE_GATT_Event XDATA * p_event) large
{	
	switch(p_event->eventID)
	{
		case BLE_GATTS_EVT_WRITE:
		{
			BLE_IAS_Event_Handler(p_event);
		}
		break;
		default:
		{
		}
		break;
	}
}