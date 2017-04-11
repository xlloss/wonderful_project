/********************************************************************
  File Information:
    FileName:       ble_weight_scale_service.c
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
#include "ble_ancs_service.h"

PUBLIC  MCODE  u8 ANCS_Defined_Services[]={	
	/* Apple Notification Center Service is a primary service whose service UUID is 7905F431-B5CE-4E99-A40F-4B1E122D00D0. */
	/* ATT_ATTRUBUTE_HEADER_16 */																    /* ATT_ATTRIBUTE_VALUE_SERVICE_16 */
	0x18, 0x00,	0x00,0x00, BLE_ATT_UUID_LENGTH_16, UUID_PRIMARY_SERVICE>>8, UUID_PRIMARY_SERVICE,   0x00, 0x00, 0xD0, 0x00, 0x2D, 0x12, 0x1E, 0x4B, 0x0F, 0xA4, 0x99, 0x4E, 0xCE, 0xB5, 0x31, 0xF4, 0x05, 0x79,
	
	/* Notification Source: UUID 9FBF120D-6301-42D9-8C58-25E699A21DBD (notifiable) */
	/* ATT_ATTRUBUTE_HEADER_16 */																	 /* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_16 */				
	0x19,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_16, UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,  NOTIFY, 0x00, 0x00, 0xBD, 0x1D, 0xA2, 0x99, 0xE6, 0x25, 0x58, 0x8C, 0xD9, 0x42, 0x01, 0x63, 0x0D, 0x12, 0xBF, 0x9F, 

	/* Control Point: UUID 69D1D8F3-45E1-49A8-9821-9BBDFDAAD9D9 (writeable with response) */
	/* ATT_ATTRUBUTE_HEADER_16 */																	 /* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_16 */				
	0x19,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_16, UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,  WRITE, 0x00, 0x00, 0xD9, 0xD9, 0xAA, 0xFD, 0xBD, 0x9B, 0x21, 0x98, 0xA8, 0x49, 0xE1, 0x45, 0xF3, 0xD8, 0xD1, 0x69,
	
	/* Data Source: UUID 22EAC6E9-24D6-4BB5-BE44-B36ACE7C7BFB (notifiable) */
	/* ATT_ATTRUBUTE_HEADER_16 */																	 /* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_16 */
	0x19,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_16, UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,  NOTIFY, 0x00, 0x00, 0xFB, 0x7B, 0x7C, 0xCE, 0x6A, 0xB3, 0x44, 0xBE, 0xB5, 0x4B, 0xD6, 0x24, 0xE9, 0xC6, 0xEA, 0x22,
};

PUBLIC XDATA ANCSProcessRoutine	ANCSProcess;

PUBLIC u16 BLE_ANCS_Init(ANCSProcessRoutine ancsHandler) large
{
	if(ancsHandler==NULL)
	{
		return MBIOT_RES_INVALID_PARA;
	}
	ANCSProcess=ancsHandler;
	
	return MBIOT_RES_SUCCESS;
}

PUBLIC u16 BLE_ANCS_Init_Service() large
{
	XDATA u16 startHandle, result;	
	XDATA BLE_GATT_ServicesInfo serviceInfo;
	XDATA u8 ancs_log[] = "BLE_ANCS_Init_Service";

	serviceInfo.p_definedService = (u8 XDATA *)ANCS_Defined_Services;
	serviceInfo.attributeNum = 4;
	serviceInfo.serviceSize = sizeof(ANCS_Defined_Services);
	result=BLE_GATTS_InitServiceTable(&serviceInfo, &startHandle);
	
	if(result==MBIOT_RES_SUCCESS)
	{
			APP_UART_Put(sizeof(ancs_log), ancs_log);
	}

	return result;
}

PUBLIC void BLE_ANCS_Catch_Event(BLE_GATT_Event XDATA * p_event) large
{
	u8 XDATA ancs_log[] = "BLE_ANCS_Catch_Event";
	APP_UART_Put(sizeof(ancs_log), ancs_log);
	APP_UART_Put(sizeof(p_event->eventID), (XDATA u8)&p_event->eventID);
	
	switch(p_event->eventID)
	{
		case BLE_GATTS_EVT_WRITE:
		{
				u8 XDATA ancs_log[] = "BLE_GATTS_EVT_WRITE";
				APP_UART_Put(sizeof(ancs_log), ancs_log);
		}
		break;
		
		case BLE_GATTC_EVT_HV_NOTIFY:
		{
			BLE_ATT_EVT_ReceiveHV_t XDATA *p_onReceiveHV = (BLE_ATT_EVT_ReceiveHV_t	XDATA	*)&p_event->eventField;
			u8 XDATA ancs_log[] = "BLE_GATTC_EVT_HV_NOTIFY";
			APP_UART_Put(sizeof(ancs_log), ancs_log);
			ANCSProcess(p_onReceiveHV);
		}
		break;

		case BLE_GATTC_EVT_HV_INDICATE:
		{
			//BLE_ATT_EVT_ReceiveHV_t *p_onReceiveHV;
			u8 XDATA ancs_log[] = "BLE_GATTC_EVT_HV_INDICATE";
			APP_UART_Put(sizeof(ancs_log), ancs_log);
		}
		break;
		
		default:
		{
				u8 XDATA ans_log[] = "BLE_ANCS_Catch_Event Default";
				APP_UART_Put(sizeof(ancs_log), ancs_log);						
		}
		break;
	}
}

PUBLIC u16 BLE_ANCS_Subscribe_Notification_Source(u8 connHandle, BLE_ANCS_Characteristic *p_ANCS_Notify_Source) large
{
	XDATA BLE_GATTC_WriteParams  *p_wParams;
	XDATA u8 ancs_log[] = "BLE_ANCS_Subscribe_Notification_Source";
	APP_UART_Put(sizeof(ancs_log), ancs_log);
	
	p_wParams = (BLE_GATTC_WriteParams*)APP_HEAP_Alloc(sizeof(BLE_GATTC_WriteParams));
	if(p_wParams!=NULL)
	{
		//p_wParams->charHandle = p_ANCS_Notify_Source->handleValue;
		p_wParams->charHandle = p_ANCS_Notify_Source->attributeHandle;
		p_wParams->charLength = 2;
		p_wParams->charValue[0] = 0x01;
		p_wParams->charValue[1] = 0x00;
		p_wParams->writeType = BLE_GATT_TYPE_WRITE_REQ;
		p_wParams->valueOffset = 0x0000;
		p_wParams->flags = 0;
		BLE_GATTC_Write(connHandle, p_wParams);	
	}
	return MBIOT_RES_SUCCESS;
}

PUBLIC u16 BLE_ANCS_Subscribe_Data_Source(u8 connHandle, BLE_ANCS_Characteristic *p_ANCS_Data_Source) large
{
	XDATA BLE_GATTC_WriteParams  *p_wParams;
	XDATA u8 ancs_log[] = "BLE_ANCS_Subscribe_Data_Source";
	APP_UART_Put(sizeof(ancs_log), ancs_log);
	
	p_wParams = (BLE_GATTC_WriteParams*)APP_HEAP_Alloc(sizeof(BLE_GATTC_WriteParams));
	if(p_wParams!=NULL)
	{
		//p_wParams->charHandle = p_ANCS_Data_Source->handleValue;
		p_wParams->charHandle = p_ANCS_Data_Source->attributeHandle;
		p_wParams->charLength = 2;
		p_wParams->charValue[0] = 1;
		p_wParams->charValue[1] = 0;
		p_wParams->writeType = BLE_GATT_TYPE_WRITE_REQ;
		p_wParams->valueOffset = 0x0000;
		p_wParams->flags = 0;
		BLE_GATTC_Write(connHandle, p_wParams);	
	}
	return MBIOT_RES_SUCCESS;
}