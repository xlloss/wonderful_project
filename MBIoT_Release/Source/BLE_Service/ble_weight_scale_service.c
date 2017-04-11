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
#include "ble_weight_scale_service.h"

PUBLIC  MCODE  u8 APP_Defined_Services[]={

	/*Weight Scale Service*/
	/* ATT_ATTRUBUTE_HEADER_2 */																						/* ATT_ATTRIBUTE_VALUE_SERVICE_2 */
	0x0A,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_PRIMARY_SERVICE>>8,UUID_PRIMARY_SERVICE,					0x00,0x00,	UUID_WEIGHT_SCALE_SERVICE>>8,UUID_WEIGHT_SCALE_SERVICE,
	/* ATT_ATTRUBUTE_HEADER_2 */																						/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */
	0x0B,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						READ,	0x00,0x00,	UUID_WEIGHT_SCALE_FEATURE>>8,UUID_WEIGHT_SCALE_FEATURE,
	/* ATT_ATTRUBUTE_HEADER_2 */																						/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE */
	0x0B,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_WEIGHT_SCALE_FEATURE>>8,UUID_WEIGHT_SCALE_FEATURE,			0x04,	0x00,0x00,0x00,0x00,
	/* ATT_ATTRUBUTE_HEADER_2 */																						/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */
	0x0B,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						INDICATE,	0x00,0x00,	UUID_WEIGHT_MEASUREMENT>>8,UUID_WEIGHT_MEASUREMENT,
	/* ATT_ATTRUBUTE_HEADER_2 */																						/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE */
	0x08,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_WEIGHT_MEASUREMENT>>8,UUID_WEIGHT_MEASUREMENT,	0x01,	0x00,
	/* ATT_ATTRUBUTE_HEADER_2 */																							/* ATT_ATTRIBUTE_DESCRIPTOR_CCCD */
	0x08,	PM_WRITABLE,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG>>8,UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG,	0x00,0x00
};

PUBLIC XDATA WSSProcessRoutine	WSSProcess;
PUBLIC u16 XDATA  BLE_WSS_weightFeature;
PUBLIC u16 XDATA  weightFeatureHandle;
PUBLIC u16 XDATA  weightMeasurementHandle;
PUBLIC u16 XDATA  weightMeasurementCCCDHandle;

static u16 update_WEIGHT_SCALE_FEATURE()large
{
	XDATA u8 feature[4];	
	
	feature[0]=BLE_WSS_weightFeature;
	feature[1]=BLE_WSS_weightFeature>>8;
	feature[2]=0;
	feature[3]=0;
	
	return BLE_GATTS_SetHandleValue(weightFeatureHandle,sizeof(feature),feature);
}

PUBLIC void BLE_WSS_Init(WSSProcessRoutine wssHandler)large
{
	WSSProcess=wssHandler;
}

PUBLIC u16 BLE_WSS_Init_Service() large
{
	XDATA u16 startHandle, result;	
	XDATA BLE_GATT_ServicesInfo serviceInfo;

	serviceInfo.p_definedService = (u8 XDATA *)APP_Defined_Services;
	serviceInfo.attributeNum = TOTAL_WSS_ATTR_NUM;
	serviceInfo.serviceSize = sizeof(APP_Defined_Services);
	result=BLE_GATTS_InitServiceTable(&serviceInfo, &startHandle);
	
	if(result==MBIOT_RES_SUCCESS)
	{
		weightFeatureHandle=startHandle+INDEX_WEIGHT_SCALE_FEATURE_VALUE;
		weightMeasurementHandle=startHandle+INDEX_WEIGHT_MEASUREMENT_VALUE;
		weightMeasurementCCCDHandle=startHandle+INDEX_WEIGHT_MEASUREMENT_CCCD;
	}

	return result;
}

PUBLIC void BLE_WSS_Catch_Event(BLE_GATT_Event XDATA * p_event) large
{
	switch(p_event->eventID)
	{
		case BLE_GATTS_EVT_WRITE:
		{
			if((p_event->eventField.onWrite.charHandle==weightMeasurementCCCDHandle)&&(WSSProcess))
			{
				u16 XDATA cccd;
				xmemcpy((u8 XDATA *)&cccd, p_event->eventField.onWrite.p_charValue, p_event->eventField.onWrite.writeDataLength);
				WSSProcess(cccd);
			}
		}
		break;
		
		default:
		break;
	}
}

PUBLIC u16 BLE_WSS_Set_Time_Stamp(bool isTimeStampSupported) large
{
	(isTimeStampSupported)?(BLE_WSS_weightFeature|=WEIGHT_SCALE_FEATURE_TIMESTAMP_MASK):(BLE_WSS_weightFeature&=(~WEIGHT_SCALE_FEATURE_TIMESTAMP_MASK));
	return update_WEIGHT_SCALE_FEATURE();
}

PUBLIC u16 BLE_WSS_Set_Users(bool isMultipleUserSupported) large
{
	(isMultipleUserSupported)?(BLE_WSS_weightFeature|=WEIGHT_SCALE_FEATURE_MULTIPLE_USER_MASK):(BLE_WSS_weightFeature&=(~WEIGHT_SCALE_FEATURE_MULTIPLE_USER_MASK));
	return update_WEIGHT_SCALE_FEATURE();
}

PUBLIC u16 BLE_WSS_Set_BMI(bool isBmiSupported)large
{
	(isBmiSupported)?(BLE_WSS_weightFeature|=WEIGHT_SCALE_FEATURE_BMI_MASK):(BLE_WSS_weightFeature&=(~WEIGHT_SCALE_FEATURE_BMI_MASK));
	return update_WEIGHT_SCALE_FEATURE();
}

PUBLIC u16 BLE_WSS_Set_Weight_Resolution(u8 weightResolution) large
{
	if(weightResolution>WEIGHT_RESOLUTION_0005KG_OR_001IB)
	{
		return MBIOT_RES_INVALID_PARA;
	}
	
	BLE_WSS_weightFeature&=(~WEIGHT_SCALE_FEATURE_WEIGHT_RESOLUTION_MASK);
	BLE_WSS_weightFeature|=(weightResolution<<WEIGHT_SCALE_FEATURE_WEIGHT_RESOLUTION_BIT);
	return update_WEIGHT_SCALE_FEATURE();
}

PUBLIC u16 BLE_WSS_Set_Height_Resolution(u8 heightResolution) large
{
	if(heightResolution>HEIGHT_RESOLUTION_0001METER_OR_01INCH)
	{
		return MBIOT_RES_INVALID_PARA;
	}
	
	BLE_WSS_weightFeature&=(~WEIGHT_SCALE_FEATURE_HEIGHT_RESOLUTION_MASK);
	BLE_WSS_weightFeature|=(heightResolution<<WEIGHT_SCALE_FEATURE_HEIGHT_RESOLUTION_BIT);
	return update_WEIGHT_SCALE_FEATURE();
}

PUBLIC u16 BLE_WSS_Send_Weight_Measurement(u8 connHandle, BLE_WSS_Measurement XDATA *p_measurement) large
{
	XDATA BLE_GATTS_HandleValueParams XDATA *p_value;
	XDATA u8 measurement[BLE_ATT_ATTRIBUTE_VALUE_LEN], index=0;
	XDATA u16 result;
	
	//Configure Flags
	(p_measurement->measurementUnits)?(measurement[index]|=WEIGHT_MEASUREMENT_FLAG_UNITS):(measurement[index]&=(~WEIGHT_MEASUREMENT_FLAG_UNITS));
	(p_measurement->isTimeStampSupported)?(measurement[index]|=WEIGHT_MEASUREMENT_FLAG_TIMESTAMP):(measurement[index]&=(~WEIGHT_MEASUREMENT_FLAG_TIMESTAMP));
	(p_measurement->isMultipleUserSupported)?(measurement[index]|=WEIGHT_MEASUREMENT_FLAG_USER_ID):(measurement[index]&=(~WEIGHT_MEASUREMENT_FLAG_USER_ID));
	(p_measurement->isBmiSupported)?(measurement[index]|=WEIGHT_MEASUREMENT_FLAG_BMI_HEIGHT):(measurement[index]&=(~WEIGHT_MEASUREMENT_FLAG_BMI_HEIGHT));
	index++;
	EQU_Swap_u16pt(&measurement[index],&p_measurement->weight);
	index+=sizeof(p_measurement->weight);
	if(p_measurement->isTimeStampSupported)
	{
		xmemcpy(&measurement[index],(u8 XDATA*)&p_measurement->timeStamp, sizeof(Time_Field));
		index+=sizeof(Time_Field);
	}
	
	if(p_measurement->isMultipleUserSupported)
	{
		measurement[index]=p_measurement->userID;
		index+=sizeof(p_measurement->userID);
	}
	
	if(p_measurement->isBmiSupported)
	{
		EQU_Swap_u16pt(&measurement[index],&p_measurement->bmi);
		index+=sizeof(p_measurement->bmi);
		EQU_Swap_u16pt(&measurement[index],&p_measurement->height);
		index+=sizeof(p_measurement->height);
	}

	p_value = (BLE_GATTS_HandleValueParams*)APP_HEAP_Alloc(sizeof(BLE_GATTS_HandleValueParams));
	if(p_value!=NULL)
	{
		p_value->charHandle=weightMeasurementHandle;
		p_value->charLength=index;
		p_value->sendType=BLE_GATT_HV_INDICATION;
		xmemcpy_u16(p_value->charValue, measurement, p_value->charLength);
		result=BLE_GATTS_SendHandleValue(connHandle, p_value);
		APP_HEAP_Free((u8 XDATA *)p_value);
		return result;
	}
	return MBIOT_RES_OOM;
}