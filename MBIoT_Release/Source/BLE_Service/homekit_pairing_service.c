/********************************************************************
  File Information:
    FileName:       homekit_pairing_service.c
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
#include "homekit_pairing_service.h"

PUBLIC MCODE u8 HPS_Defined_Services[] = {
	/*Pairing Service*/
	/* ATT_ATTRUBUTE_HEADER_2 */																						/* ATT_ATTRIBUTE_VALUE_SERVICE_2 */												
	0x18,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_16,	UUID_PRIMARY_SERVICE>>8,UUID_PRIMARY_SERVICE,					0x00,0x00,	UUID_HOMEKIT_PAIRING_SERVICE,
	/* ATT_ATTRUBUTE_HEADER_2 */																						/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_16 */
	0x19,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						READ,	0x00,0x00,	UUID_HOMEKIT_PAIRING_SERVICE_INSTANCE_ID,
	/* ATT_ATTRUBUTE_HEADER_16 */																						/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE */
	0x17,	PM_MANUAL_READ_RESP,	0x00,0x00,	BLE_ATT_UUID_LENGTH_16,	UUID_HOMEKIT_PAIRING_SERVICE_INSTANCE_ID,		0x02,	0x31,0x31,
	/* ATT_ATTRUBUTE_HEADER_16 */																						/* ATT_ATTRIBUTE_GENERIC_DESCRIPTOR */
	0x17,	PM_MANUAL_READ_RESP,	0x00,0x00,	BLE_ATT_UUID_LENGTH_16,	UUID_HOMEKIT_PAIRING_CHAR_INSTANCE_ID,			0x02,	0x00,0x0C,
	/* ATT_ATTRUBUTE_HEADER_2 */																						/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_16 */
	0x19,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						READ|WRITE,	0x00,0x00,	UUID_HOMEKIT_PAIRING_PAIR_SETUP,
	/* ATT_ATTRUBUTE_HEADER_16 */																						/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE */
	0x16,	PM_MANUAL_WRITE_RESP|PM_MANUAL_READ_RESP,	0x00,0x00,	BLE_ATT_UUID_LENGTH_16, UUID_HOMEKIT_PAIRING_PAIR_SETUP,								0x01,0x00,
	/* ATT_ATTRUBUTE_HEADER_16 */																						/* ATT_ATTRIBUTE_GENERIC_DESCRIPTOR */
	0x17,	PM_MANUAL_READ_RESP,	0x00,0x00,	BLE_ATT_UUID_LENGTH_16,	UUID_HOMEKIT_PAIRING_CHAR_INSTANCE_ID,			0x02,	0x00,0x0D,
	/* ATT_ATTRUBUTE_HEADER_2 */																						/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_16 */
	0x19,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						READ|WRITE,	0x00,0x00,	UUID_HOMEKIT_PAIRING_PAIR_VERIFY,		
	/* ATT_ATTRUBUTE_HEADER_16 */																						/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE */
	0x16,	PM_MANUAL_WRITE_RESP|PM_MANUAL_READ_RESP,	0x00,0x00,	BLE_ATT_UUID_LENGTH_16, UUID_HOMEKIT_PAIRING_PAIR_VERIFY,								0x01,0x00,
	/* ATT_ATTRUBUTE_HEADER_16 */																						/* ATT_ATTRIBUTE_GENERIC_DESCRIPTOR */
	0x17,	PM_MANUAL_READ_RESP,	0x00,0x00,	BLE_ATT_UUID_LENGTH_16,	UUID_HOMEKIT_PAIRING_CHAR_INSTANCE_ID,			0x02,	0x00,0x0E,
	/* ATT_ATTRUBUTE_HEADER_2 */																						/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_16 */
	0x19,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						READ,	0x00,0x00,	UUID_HOMEKIT_PAIRING_FEATURES,		
	/* ATT_ATTRUBUTE_HEADER_16 */																						/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE */
	0x16,	PM_MANUAL_READ_RESP,	0x00,0x00,	BLE_ATT_UUID_LENGTH_16, UUID_HOMEKIT_PAIRING_FEATURES,					0x01,0x01,
	/* ATT_ATTRUBUTE_HEADER_16 */																						/* ATT_ATTRIBUTE_GENERIC_DESCRIPTOR */
	0x17,	PM_MANUAL_READ_RESP,	0x00,0x00,	BLE_ATT_UUID_LENGTH_16,	UUID_HOMEKIT_PAIRING_CHAR_INSTANCE_ID,			0x02,	0x00,0x0F,
	/* ATT_ATTRUBUTE_HEADER_2 */																						/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_16 */
	0x19,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						READ|WRITE,	0x00,0x00,	UUID_HOMEKIT_PAIRING_PAIRINGS,		
	/* ATT_ATTRUBUTE_HEADER_16 */																						/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE */	
	0x16,	PM_MANUAL_WRITE_RESP|PM_MANUAL_READ_RESP,	0x00,0x00,	BLE_ATT_UUID_LENGTH_16, UUID_HOMEKIT_PAIRING_PAIRINGS,									0x01,0x00,
	/* ATT_ATTRUBUTE_HEADER_16 */																						/* ATT_ATTRIBUTE_GENERIC_DESCRIPTOR */
	0x17,	PM_MANUAL_READ_RESP,	0x00,0x00,	BLE_ATT_UUID_LENGTH_16,	UUID_HOMEKIT_PAIRING_CHAR_INSTANCE_ID,			0x02,	0x00,0x10,
};

PUBLIC XDATA Homekit_Pairing_Handle_List	homekitPairingHandle;

PUBLIC u16 HOMEKIT_Pairing_Init_Service() large
{
	XDATA u16 startHandle, result;	
	XDATA BLE_GATT_ServicesInfo serviceInfo;

	serviceInfo.p_definedService = (u8 XDATA *)HPS_Defined_Services;
	serviceInfo.attributeNum = TOTAL_HPS_ATTR_NUM;
	serviceInfo.serviceSize = sizeof(HPS_Defined_Services);
	result=BLE_GATTS_InitServiceTable(&serviceInfo, &startHandle);
	
	if(result==MBIOT_RES_SUCCESS)
	{
		homekitPairingHandle.pairSetup=startHandle+INDEX_PAIRING_PAIR_SETUP_VALUE;
		homekitPairingHandle.pairVerify=startHandle+INDEX_PAIRING_PAIR_VERIFY_VALUE;
		homekitPairingHandle.pairPairings=startHandle+INDEX_PAIRING_PAIRINGS_VALUE;		
	}
	return result;
}