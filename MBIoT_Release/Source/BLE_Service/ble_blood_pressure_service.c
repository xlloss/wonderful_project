/********************************************************************
  File Information:
    FileName:       ble_blood_pressure_service.c
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
#include "ble_blood_pressure_service.h"

PUBLIC  MCODE  u8 APP_Defined_Services[]={

	/*Blood Pressure Service*/
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_SERVICE_2 */
	0x0A,	0x00,	0x00,0x00,	0x02,	UUID_PRIMARY_SERVICE>>8,UUID_PRIMARY_SERVICE,					0x00,0x00,	UUID_BLOOD_PRESSURE_SERVICE>>8,UUID_BLOOD_PRESSURE_SERVICE,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */
	0x0B,	0x00,	0x00,0x00,	0x02,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						INDICATE,	0x00,0x00,	UUID_BLOOD_PRESSURE_MEASUREMENT>>8,UUID_BLOOD_PRESSURE_MEASUREMENT,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE */
	0x08,	0x00,	0x00,0x00,	0x02,	UUID_BLOOD_PRESSURE_MEASUREMENT>>8,UUID_BLOOD_PRESSURE_MEASUREMENT,			0x01,	0x00,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_DESCRIPTOR_CCCD */
	0x08,	0x00,	0x00,0x00,	0x02,	UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG>>8,UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG,	0x00,0x00,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */
	0x0B,	0x00,	0x00,0x00,	0x02,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						NOTIFY,	0x00,0x00,	UUID_INTERMEDIATE_CUFF_PRESSURE>>8,UUID_INTERMEDIATE_CUFF_PRESSURE,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE */
	0x08,	0x00,	0x00,0x00,	0x02,	UUID_INTERMEDIATE_CUFF_PRESSURE>>8,UUID_INTERMEDIATE_CUFF_PRESSURE,	0x01,	0x00,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_DESCRIPTOR_CCCD */
	0x08,	0x00,	0x00,0x00,	0x02,	UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG>>8,UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG,	0x00,0x00,
    /* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */
	0x0B,	0x00,	0x00,0x00,	0x02,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						READ,	0x00,0x00,	UUID_BLOOD_PRESSURE_FEATURE>>8,UUID_BLOOD_PRESSURE_FEATURE,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE */
	0x09,	0x00,	0x00,0x00,	0x02,	UUID_BLOOD_PRESSURE_FEATURE>>8,UUID_BLOOD_PRESSURE_FEATURE,		0x02,	0x00, 0x00,
};

PUBLIC XDATA BPSProcessRoutine	BPSProcess;
PUBLIC u16 XDATA  BLE_BPS_bloodpressureFeature;

PUBLIC u16 XDATA  bloodpressureMeasurementHandle;
PUBLIC u16 XDATA  bloodpressureMeasurementCCCDHandle;
PUBLIC u16 XDATA  intermediatecuffpressureHandle;
PUBLIC u16 XDATA  intermediatecuffpressureCCCDHandle;
PUBLIC u16 XDATA  bloodpressureFeatureHandle;

static void update_blood_pressure_feature() large
{
	u8 XDATA feature[2];
	
	feature[0]=BLE_BPS_bloodpressureFeature;
	feature[1]=BLE_BPS_bloodpressureFeature>>8;
	
	BLE_GATTS_SetHandleValue(bloodpressureFeatureHandle,sizeof(feature),feature);
}

PUBLIC void BLE_BPS_Init(BPSProcessRoutine bpsHandler) large
{
	BPSProcess=bpsHandler;
}

PUBLIC u16 BLE_BPS_Init_Service() large
{
	XDATA u16 startHandle, result;	
	XDATA BLE_GATT_ServicesInfo serviceInfo;

	serviceInfo.p_definedService = (u8 XDATA *)APP_Defined_Services;
	serviceInfo.attributeNum = TOTAL_BPS_ATTR_NUM;
	serviceInfo.serviceSize = sizeof(APP_Defined_Services);
	result=BLE_GATTS_InitServiceTable(&serviceInfo, &startHandle);
	
	if(result==MBIOT_RES_SUCCESS)
	{
		bloodpressureMeasurementHandle=startHandle+INDEX_BLOOD_PRESSURE_MEASUREMENT_VALUE;
		bloodpressureMeasurementCCCDHandle=startHandle+INDEX_BLOOD_PRESSURE_MEASUREMENT_CCCD;
		intermediatecuffpressureHandle = startHandle+INDEX_INTERMEDIATE_CUFF_PRESSURE_VALUE;
		intermediatecuffpressureCCCDHandle = startHandle+INDEX_INTERMEDIATE_CUFF_PRESSURE_CCCD;
		bloodpressureFeatureHandle=startHandle+INDEX_BLOOD_PRESSURE_FEATURE_VALUE;
	}
	
	return result;
}

PUBLIC void BLE_BPS_Catch_Event(BLE_GATT_Event XDATA * p_event) large
{
	switch(p_event->eventID)
	{
		case BLE_GATTS_EVT_WRITE:
		{
			if(((p_event->eventField.onWrite.charHandle==bloodpressureMeasurementCCCDHandle)||
				(p_event->eventField.onWrite.charHandle==intermediatecuffpressureCCCDHandle))&&(BPSProcess))
			{
				u16 XDATA cccd;
				xmemcpy((u8 XDATA *)&cccd, p_event->eventField.onWrite.p_charValue, p_event->eventField.onWrite.writeDataLength);
				BPSProcess(cccd);
			}
		}
		break;
		case BLE_GATTC_EVT_ERROR_RESP:
		{
		}
		break;
		case BLE_GATTC_EVT_WRITE_RESP:
		{
		}
		break;
		case BLE_GATTC_EVT_READ_RESP:
		{
		}
		break;
		case BLE_GATTC_EVT_HV_NOTIFY:
		{
		}
		break;
		case BLE_GATTC_EVT_HV_INDICATE:
		{
		}
		break;
		case BLE_GATTS_EVT_READ:
		{
		}
		break;
		case BLE_GATTS_EVT_HV_CONFIRM:
		{			
		}
		break;				
		default:
		{
		}
		break;
	}
}

PUBLIC u16 BLE_BPS_Body_Movement_Detection_Support(bool isBodyMovementDetectionSupportbit) large
{
 (isBodyMovementDetectionSupportbit)?(BLE_BPS_bloodpressureFeature|=BP_FEATURE_BODY_MOVEMENT_DETECT_MASK):(BLE_BPS_bloodpressureFeature&=(~BP_FEATURE_BODY_MOVEMENT_DETECT_MASK));
	update_blood_pressure_feature();
	return MBIOT_RES_SUCCESS;
}

PUBLIC u16 BLE_BPS_Cuff_Fit_Detection_Support(bool isCuffFitDetectionSupportbit) large
{
 (isCuffFitDetectionSupportbit)?(BLE_BPS_bloodpressureFeature|=BP_FEATURE_COFF_FIT_DETECT_MASK):(BLE_BPS_bloodpressureFeature&=(~BP_FEATURE_COFF_FIT_DETECT_MASK));
	update_blood_pressure_feature();
	return MBIOT_RES_SUCCESS;
}
PUBLIC u16 BLE_BPS_Irregular_Pulse_Detection_Support(bool isIrregularPulseDetectionSupportbit) large
{
 (isIrregularPulseDetectionSupportbit)?(BLE_BPS_bloodpressureFeature|=BP_FEATURE_IRREGULAR_PULSE_DETECT_MASK):(BLE_BPS_bloodpressureFeature&=(~BP_FEATURE_IRREGULAR_PULSE_DETECT_MASK));
	update_blood_pressure_feature();
	return MBIOT_RES_SUCCESS;
}
PUBLIC u16 BLE_BPS_Pulse_Rate_Range_Detection_Support(bool isPulseRateRangeDetectionSupportbit) large
{
 (isPulseRateRangeDetectionSupportbit)?(BLE_BPS_bloodpressureFeature|=BP_FEATURE_PULSE_RATE_RANGE_DETECT_MASK):(BLE_BPS_bloodpressureFeature&=(~BP_FEATURE_PULSE_RATE_RANGE_DETECT_MASK));
	update_blood_pressure_feature();
	return MBIOT_RES_SUCCESS;
}
PUBLIC u16 BLE_BPS_Measurement_Position_Detection_Support(bool isMeasurementPositionDetectionSupportbit) large
{
 (isMeasurementPositionDetectionSupportbit)?(BLE_BPS_bloodpressureFeature|=BP_FEATURE_MEASUREMENT_POSITION_DETECT_MASK):(BLE_BPS_bloodpressureFeature&=(~BP_FEATURE_MEASUREMENT_POSITION_DETECT_MASK));
	update_blood_pressure_feature();
	return MBIOT_RES_SUCCESS;
}
PUBLIC u16 BLE_BPS_Multiple_Bond_Support(bool isMultipleBondSupportbit) large
{
 (isMultipleBondSupportbit)?(BLE_BPS_bloodpressureFeature|=BP_FEATURE_MULTIPLE_BOND_MASK):(BLE_BPS_bloodpressureFeature&=(~BP_FEATURE_MULTIPLE_BOND_MASK));
	update_blood_pressure_feature();
	return MBIOT_RES_SUCCESS;
}

//PUBLIC u16 BLE_BPS_Send_Measurement(u8 connHandle, BLE_BPS_Measurement XDATA *p_measurement, BLE_GATTS_HandleValueParams XDATA *p_value) large
PUBLIC u16 BLE_BPS_Send_Measurement(u8 connHandle, BLE_BPS_Measurement XDATA *p_measurement) large
{
	XDATA BLE_GATTS_HandleValueParams XDATA *p_value;
	XDATA u8 measurement[BLE_ATT_ATTRIBUTE_VALUE_LEN]=0, index=0;
	XDATA u16 result;
	XDATA u16 encoded_sfloat;

	//Configure Flags
	(p_measurement->blood_pressure_units_in_kpa)?(measurement[index]|=BP_MEASUREMENT_FLAG_UNITS):(measurement[index]&=(~BP_MEASUREMENT_FLAG_UNITS));
	(p_measurement->time_stamp_present)?(measurement[index]|=BP_MEASUREMENT_FLAG_TIMESTAMP):(measurement[index]&=(~BP_MEASUREMENT_FLAG_TIMESTAMP));
	(p_measurement->pulse_rate_present)?(measurement[index]|=BP_MEASUREMENT_FLAG_PULSE_RATE):(measurement[index]&=(~BP_MEASUREMENT_FLAG_PULSE_RATE));
	(p_measurement->user_id_present)?(measurement[index]|=BP_MEASUREMENT_FLAG_USER_ID):(measurement[index]&=(~BP_MEASUREMENT_FLAG_USER_ID));
	(p_measurement->measurement_status_present)?(measurement[index]|=BP_MEASUREMENT_FLAG_STATUS):(measurement[index]&=(~BP_MEASUREMENT_FLAG_STATUS));
	index += 1;
		
	// Blood Pressure Measurement - Systolic 
	encoded_sfloat = ((p_measurement->blood_pressure_systolic.exponent << 12) & 0xF000) |
									((p_measurement->blood_pressure_systolic.mantissa <<  0) & 0x0FFF);
	EQU_Swap_u16pt(&measurement[index],&encoded_sfloat);
	index+=sizeof(encoded_sfloat);
		 
	// Blood Pressure Measurement - Diastolic 
	encoded_sfloat = ((p_measurement->blood_pressure_diastolic.exponent << 12) & 0xF000) |
									((p_measurement->blood_pressure_diastolic.mantissa <<  0) & 0x0FFF);
	EQU_Swap_u16pt(&measurement[index],&encoded_sfloat);
	index+=sizeof(encoded_sfloat);
		
	// Blood Pressure Measurement - Mean Arterial Pressure
	encoded_sfloat = ((p_measurement->mean_arterial_pressure.exponent << 12) & 0xF000) |
									((p_measurement->mean_arterial_pressure.mantissa <<  0) & 0x0FFF);
	EQU_Swap_u16pt(&measurement[index],&encoded_sfloat);
	index+=sizeof(encoded_sfloat);
		   	
	if(p_measurement->time_stamp_present)
	{
		xmemcpy(&measurement[index],(u8 XDATA*)&p_measurement->time_stamp, sizeof(Time_Field));
		index+=sizeof(Time_Field);			
	}

	if(p_measurement->pulse_rate_present)
	{
		encoded_sfloat = (((p_measurement->pulse_rate.exponent << 12) & 0xF000) |
						 ((p_measurement->pulse_rate.mantissa) <<0) & 0x0FFF);
		EQU_Swap_u16pt(&measurement[index],&encoded_sfloat);
		index+=sizeof(encoded_sfloat);			
	}
		
	if(p_measurement->user_id_present)
	{
		xmemcpy(&measurement[index],(u8 XDATA*)&p_measurement->user_id, sizeof(p_measurement->user_id));
		index++;			
	}
		
	if(p_measurement->measurement_status_present)
	{
		EQU_Swap_u16pt(&measurement[index],&p_measurement->measurement_status);
		index+=sizeof(p_measurement->measurement_status);
	}

	p_value = (BLE_GATTS_HandleValueParams*)APP_HEAP_Alloc(sizeof(BLE_GATTS_HandleValueParams));
	if(p_value!=NULL)
	{
		p_value->charHandle=bloodpressureMeasurementHandle;
		p_value->charLength=index;
		p_value->sendType=BLE_GATT_HV_INDICATION;
		xmemcpy_u16(p_value->charValue, measurement, p_value->charLength);
		result=BLE_GATTS_SendHandleValue(connHandle, p_value);
		APP_HEAP_Free((u8 XDATA *)p_value);
		return result;
	}	
	return MBIOT_RES_OOM;
}

PUBLIC u16 BLE_BPS_Send_Inter_cuff(u8 connHandle, BLE_BPS_Cuff_Pressure XDATA *p_CuffPressure) large
{
	XDATA BLE_GATTS_HandleValueParams XDATA *p_value;
	XDATA u8 measurement[BLE_ATT_ATTRIBUTE_VALUE_LEN], index=0;
	XDATA u16 result;
	XDATA u16 encoded_sfloat;
	
	//Configure Flags
	(p_CuffPressure->blood_pressure_units_in_kpa)?(measurement[index]|=BP_MEASUREMENT_FLAG_UNITS):(measurement[index]&=(~BP_MEASUREMENT_FLAG_UNITS));
	(p_CuffPressure->time_stamp_present)?(measurement[index]|=BP_MEASUREMENT_FLAG_TIMESTAMP):(measurement[index]&=(~BP_MEASUREMENT_FLAG_TIMESTAMP));
	(p_CuffPressure->pulse_rate_present)?(measurement[index]|=BP_MEASUREMENT_FLAG_PULSE_RATE):(measurement[index]&=(~BP_MEASUREMENT_FLAG_PULSE_RATE));
	(p_CuffPressure->user_id_present)?(measurement[index]|=BP_MEASUREMENT_FLAG_USER_ID):(measurement[index]&=(~BP_MEASUREMENT_FLAG_USER_ID));
	(p_CuffPressure->measurement_status_present)?(measurement[index]|=BP_MEASUREMENT_FLAG_STATUS):(measurement[index]&=(~BP_MEASUREMENT_FLAG_STATUS));
	index += 1;
		
	// Blood Pressure Inter Cuff Pressure - Current Cuff Pressure 
	encoded_sfloat = ((p_CuffPressure->blood_pressure_systolic.exponent << 12) & 0xF000) |
									((p_CuffPressure->blood_pressure_systolic.mantissa <<  0) & 0x0FFF);
	EQU_Swap_u16pt(&measurement[index],&encoded_sfloat);
	index+=sizeof(encoded_sfloat);

	// Blood Pressure Inter Cuff Pressure - Diastolic (UNUSED)
	encoded_sfloat = 0x0000;
	EQU_Swap_u16pt(&measurement[index],&encoded_sfloat);
	index+=sizeof(encoded_sfloat);
		
	// Blood Pressure Inter Cuff Pressure - Mean Arterial Pressure (UNUSED)
	encoded_sfloat = 0x0000;
	EQU_Swap_u16pt(&measurement[index],&encoded_sfloat);
	index+=sizeof(encoded_sfloat);
	
	if(p_CuffPressure->time_stamp_present)
	{
		xmemcpy(&measurement[index],(u8 XDATA*)&p_CuffPressure->time_stamp, sizeof(Time_Field));
		index+=sizeof(Time_Field);			
	}

	if(p_CuffPressure->pulse_rate_present)
	{
		encoded_sfloat = (((p_CuffPressure->pulse_rate.exponent << 12) & 0xF000) |
						 ((p_CuffPressure->pulse_rate.mantissa) <<0) & 0x0FFF);
		EQU_Swap_u16pt(&measurement[index],&encoded_sfloat);
		index+=sizeof(encoded_sfloat);			
	}
		
	if(p_CuffPressure->user_id_present)
	{
		xmemcpy(&measurement[index],(u8 XDATA*)&p_CuffPressure->user_id, sizeof(p_CuffPressure->user_id));
		index++;			
	}
		
	if(p_CuffPressure->measurement_status_present)
	{
		EQU_Swap_u16pt(&measurement[index],&p_CuffPressure->measurement_status);
		index+=sizeof(p_CuffPressure->measurement_status);
	}

	p_value = (BLE_GATTS_HandleValueParams*)APP_HEAP_Alloc(sizeof(BLE_GATTS_HandleValueParams));
	if(p_value!=NULL)
	{
		p_value->charHandle=intermediatecuffpressureHandle;
		p_value->charLength=index;
		p_value->sendType=BLE_GATT_HV_NOTIFICATION;
		xmemcpy_u16(p_value->charValue, measurement, p_value->charLength);
		result=BLE_GATTS_SendHandleValue(connHandle, p_value);
		APP_HEAP_Free((u8 XDATA *)p_value);
		return result;
	}	
	return MBIOT_RES_OOM;
}
