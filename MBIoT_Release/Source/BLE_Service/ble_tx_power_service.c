/********************************************************************
  File Information:
    FileName:       ble_tx_power_level_service.c
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
#include "ble_tx_power_service.h"

PUBLIC  MCODE  u8 APP_Defined_Tx_Power_Services[]={	
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_SERVICE_2 */
	0x0A,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_PRIMARY_SERVICE>>8,UUID_PRIMARY_SERVICE,					0x00,0x00,	UUID_TX_POWER>>8,UUID_TX_POWER,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */
	0x0B,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						READ,	0x00,0x00,	UUID_TX_POWER_LEVEL>>8,UUID_TX_POWER_LEVEL,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE */
	0x08,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_TX_POWER_LEVEL>>8,UUID_TX_POWER_LEVEL,			0x01,	0x00,
};	
PUBLIC u16 XDATA txPowerLevelHandle;

PUBLIC u16 BLE_TPS_Init_Service() large
{	
	XDATA u16 startTPSHandle;
	XDATA BLE_GATT_ServicesInfo serviceInfo;
	
	serviceInfo.p_definedService = (u8 XDATA *)APP_Defined_Tx_Power_Services;
	serviceInfo.attributeNum = TOTAL_TPS_ATTR_NUM;
	serviceInfo.serviceSize = sizeof(APP_Defined_Tx_Power_Services);
	BLE_GATTS_InitServiceTable(&serviceInfo, &startTPSHandle);
	
	txPowerLevelHandle = startTPSHandle+INDEX_TPS_TX_POWER_LEVEL;
	
	return MBIOT_RES_SUCCESS;
}

static u16 update_txPowerLevel(s8 txPowerLevel) large
{
	XDATA u16 result;
	XDATA s8 level = txPowerLevel;
	
	result = BLE_GATTS_SetHandleValue(txPowerLevelHandle,sizeof(level),&level);
	
	return result;
}

PUBLIC void BLE_TPS_Catch_Event(BLE_GATT_Event XDATA * p_event) large
{
	switch(p_event->eventID)
	{
		default:
		{
		}
		break;
	}
}

PUBLIC u16 BLE_TPS_Set_Tx_Power_Level(s8 txPowerLevel) large
{	
	return update_txPowerLevel(txPowerLevel);
}
