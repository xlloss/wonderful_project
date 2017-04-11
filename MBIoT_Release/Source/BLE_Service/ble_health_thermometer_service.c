/********************************************************************
  File Information:
    FileName:       ble_health_thermometer_service.c
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
#include "ble_health_thermometer_service.h"

PUBLIC  MCODE  u8 APP_Defined_Services[]={

	/*Health Thermometer  Service*/
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_SERVICE_2 */
	0x0A,	0x00,	0x00,0x00,	0x02,	UUID_PRIMARY_SERVICE>>8,UUID_PRIMARY_SERVICE,					0x00,0x00,	UUID_HEALTH_THERMOMETER_SERVICE>>8,UUID_HEALTH_THERMOMETER_SERVICE,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */
	0x0B,	0x00,	0x00,0x00,	0x02,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						INDICATE,	0x00,0x00,	UUID_HEALTH_THERMOMETER_TEMPERATURE_MEASUREMENT>>8,UUID_HEALTH_THERMOMETER_TEMPERATURE_MEASUREMENT,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE */
	0x08,	0x00,	0x00,0x00,	0x02,	UUID_HEALTH_THERMOMETER_TEMPERATURE_MEASUREMENT>>8,UUID_HEALTH_THERMOMETER_TEMPERATURE_MEASUREMENT,			0x01,0x00,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_DESCRIPTOR_CCCD */
	0x08,	0x00,	0x00,0x00,	0x02,	UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG>>8,UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG,	0x00,0x00,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */
	0x0B,	0x00,	0x00,0x00,	0x02,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						READ,	0x00,0x00,	UUID_HEALTH_THERMOMETER_TEMPERATURE_TYPE>>8,UUID_HEALTH_THERMOMETER_TEMPERATURE_TYPE,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE */
	0x08,	0x00,	0x00,0x00,	0x02,	UUID_HEALTH_THERMOMETER_TEMPERATURE_TYPE>>8,UUID_HEALTH_THERMOMETER_TEMPERATURE_TYPE,	0x01,	0x00,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */
	0x0B,	0x00,	0x00,0x00,	0x02,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						 NOTIFY,	0x00,0x00,	UUID_HEALTH_THERMOMETER_INTERMMEDIATE_TEMPERATURE>>8,UUID_HEALTH_THERMOMETER_INTERMMEDIATE_TEMPERATURE,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE */
	0x08,	0x00,	0x00,0x00,	0x02,	UUID_HEALTH_THERMOMETER_INTERMMEDIATE_TEMPERATURE>>8,UUID_HEALTH_THERMOMETER_INTERMMEDIATE_TEMPERATURE,			0x01,0x00,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_DESCRIPTOR_CCCD */
	0x08,	0x00,	0x00,0x00,	0x02,	UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG>>8,UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG,	0x00,0x00,
		/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */
	0x0B,	0x00,	0x00,0x00,	0x02,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						READ|WRITE|INDICATE,	0x00,0x00,	UUID_HEALTH_THERMOMETER_TEMPERATURE_INTERVAL>>8,UUID_HEALTH_THERMOMETER_TEMPERATURE_INTERVAL,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE */
	0x09,	0x00,	0x00,0x00,	0x02,	UUID_HEALTH_THERMOMETER_TEMPERATURE_INTERVAL>>8,UUID_HEALTH_THERMOMETER_TEMPERATURE_INTERVAL,	0x02,	0x00,0x00,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_DESCRIPTOR_CCCD */
	0x08,	0x00,	0x00,0x00,	0x02,	UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG>>8,UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG,	0x00,0x00,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_DESCRIPTOR_CCCD */
	0x0B,	0x00,	0x00,0x00,	0x02,	UUID_DESCRIPTOR_VALID_RANGE>>8,UUID_DESCRIPTOR_VALID_RANGE,0x04,0x58 ,0x02, 0x20 ,0x1C
};

PUBLIC XDATA HTMSProcessRoutine	HTMSProcess;
PUBLIC u16 XDATA  temperatureMeasurementHandle;
PUBLIC u16 XDATA  temperatureMeasurementHandleCCCDHandle;
PUBLIC u16 XDATA  temperatureTypeHandle;
PUBLIC u16 XDATA  intermediateTemperatureHandle;
PUBLIC u16 XDATA  intermediateTemperatureCCCDHandle;
PUBLIC u16 XDATA  measurementIntervalHandle;
PUBLIC u16 XDATA  measurementIntervalCCDHandle;
PUBLIC u16 XDATA  measurementIntervalValidRangeHandle;
PUBLIC XDATA u16 MeasurementInterval;                      /**< Temperature Measurement Interval. */


PUBLIC void BLE_HTMS_Init(HTMSProcessRoutine htmsHandler) large
{
	HTMSProcess = htmsHandler;
}

PUBLIC u16 BLE_HTMS_Init_Service() large
{
	XDATA u16 startHandle, result;	
	XDATA BLE_GATT_ServicesInfo serviceInfo;
	MeasurementInterval = 0;

	serviceInfo.p_definedService = (u8 XDATA *)APP_Defined_Services;
	serviceInfo.attributeNum = TOTAL_HTMS_ATTR_NUM;
	serviceInfo.serviceSize = sizeof(APP_Defined_Services);
	result=BLE_GATTS_InitServiceTable(&serviceInfo, &startHandle);
	
	if(result==MBIOT_RES_SUCCESS)
	{
		temperatureMeasurementHandle=startHandle+INDEX_HEALTH_THERMOMETER_TEMPERATURE_MEASUREMENT_VALUE;
		temperatureMeasurementHandleCCCDHandle=startHandle+INDEX_HEALTH_THERMOMETER_TEMPERATURE_MEASUREMENT_CCCD;
		temperatureTypeHandle=startHandle+INDEX_HEALTH_THERMOMETER_TEMPERATURE_TYPE_VALUE;
		intermediateTemperatureHandle = startHandle+INDEX_HEALTH_THERMOMETER_INTERMMEDIATE_TEMPERATURE_VALUE;
		intermediateTemperatureCCCDHandle = startHandle+INDEX_HEALTH_THERMOMETER_INTERMMEDIATE_TEMPERATURE_CCCD;
		measurementIntervalHandle = startHandle+INDEX_HEALTH_THERMOMETER_TEMPERATURE_INTERVAL_VALUE;
		measurementIntervalCCDHandle = startHandle+INDEX_HEALTH_THERMOMETER_TEMPERATURE_INTERVAL_CCCD;
		measurementIntervalValidRangeHandle = startHandle+INDEX_HEALTH_THERMOMETER_TEMPERATURE_INTERVAL_VALID_RANGE;
	}

	return result;
}

PUBLIC void BLE_HTMS_Catch_Event(BLE_GATT_Event XDATA * p_event) large
{
	switch(p_event->eventID)
	{
		case BLE_GATTS_EVT_WRITE:
		{
		XDATA u16 handle;
		handle = p_event->eventField.onWrite.charHandle;
		if((handle == intermediateTemperatureCCCDHandle)|| (handle == measurementIntervalCCDHandle)|| (handle == temperatureMeasurementHandleCCCDHandle)||(handle == measurementIntervalHandle)||(handle == measurementIntervalValidRangeHandle))
		{
			XDATA u16 write_value;
			BLE_HTMS_Event XDATA *p_htmsEvent;
			xmemcpy((u8 XDATA *)&write_value, p_event->eventField.onWrite.p_charValue, p_event->eventField.onWrite.writeDataLength);
			p_htmsEvent = (BLE_HTMS_Event XDATA *)APP_HEAP_Alloc(sizeof(BLE_HTMS_Event));
			if(p_htmsEvent!= NULL)
			{
				p_htmsEvent->handle = handle;
				p_htmsEvent->write_value = write_value;
				HTMSProcess(p_htmsEvent);
				APP_HEAP_Free((u8 XDATA *)p_htmsEvent);
			}
		}
		}
		break;
	    default:
		break;
	}
}

PUBLIC u16 BLE_HTMS_Set_TemperatureType(u8 typeDescription) large
{
	u16 XDATA len = sizeof(u8);
	u8 XDATA temperaturetype = typeDescription;
	if(temperaturetype > BLE_HTMS_TEMPERATURE_TYPE_EAR_DRUM)
	{
		return MBIOT_RES_INVALID_PARA;
	}
	return BLE_GATTS_SetHandleValue(temperatureTypeHandle, len, &temperaturetype);
}

PUBLIC extern u16 BLE_HTMS_Set_Min_Valid_Range(u16 MinRange) large
{
  u16 XDATA len = sizeof(u16);
  if(MinRange < HEALTH_THERMOMETER_MIN_RANGE)
	{
		return MBIOT_RES_INVALID_PARA;
	}
	return BLE_GATTS_SetHandleValue(measurementIntervalValidRangeHandle, len,(u8*) &MinRange);


}
PUBLIC extern u16 BLE_HTMS_Set_Max_Valid_Range(u16 MaxRange) large
{
  u16 XDATA len = sizeof(u16);
  if(MaxRange > HEALTH_THERMOMETER_MAX_RANGE)
	{
		return MBIOT_RES_INVALID_PARA;
	}
	return BLE_GATTS_SetHandleValue(measurementIntervalValidRangeHandle, len,(u8*) &MaxRange);


}

PUBLIC u16 BLE_HTMS_Set_Temperature_Interval(u16 measurementInterval) large{
    u16 XDATA len = sizeof(u16);
	u8 XDATA tempMeasurementInterval = measurementInterval;
	MeasurementInterval = measurementInterval;
	if(tempMeasurementInterval > HEALTH_THERMOMETER_MAX_MEASUREMENT_INTERVAL)
	{
		tempMeasurementInterval = OUT_OF_RANGE;
	}
	return BLE_GATTS_SetHandleValue(measurementIntervalHandle, len, (u8*)&tempMeasurementInterval);


}
PUBLIC u16 BLE_HTMS_Send_Temperature_Interval(u8 connHandle,u16 measurementInterval) large
{
	XDATA u16 result , m_interval=0;
  XDATA BLE_GATTS_HandleValueParams XDATA *p_value;
	m_interval = measurementInterval;
	if(m_interval > HEALTH_THERMOMETER_MAX_MEASUREMENT_INTERVAL)
	{
	    m_interval = OUT_OF_RANGE ;
	}
	p_value = (BLE_GATTS_HandleValueParams*)APP_HEAP_Alloc(sizeof(BLE_GATTS_HandleValueParams));
	if(p_value!=NULL)
	{
		p_value->charHandle=measurementIntervalHandle;
		p_value->charLength=sizeof(u16);
		p_value->sendType=BLE_GATT_HV_INDICATION;
		xmemcpy_u16(p_value->charValue, (u8 XDATA*)&m_interval, p_value->charLength);
		result=BLE_GATTS_SendHandleValue(connHandle, p_value);
		APP_HEAP_Free((u8 XDATA *)p_value);
		return result;
	}
	return MBIOT_RES_OOM;
}
PUBLIC u16 BLE_HTMS_Send_Temperature_Measurement(u8 connHandle, BLE_HTMS_Measurement XDATA *p_measurement) large
{
  XDATA u16 result;
  XDATA BLE_GATTS_HandleValueParams XDATA *p_value;
	XDATA u8 measurement[BLE_ATT_ATTRIBUTE_VALUE_LEN]=0, index=0;
	//XDATA float temp = 2.5;
		
	//Configure Flags
	(p_measurement->IsTemperatureUnitAvailable)?(measurement[index]|=HEALTH_THERMOMETER_TEMPERATURE_MEASUREMENT_FLAG_TEMPERATURE_UNIT):(measurement[index]&=(~HEALTH_THERMOMETER_TEMPERATURE_MEASUREMENT_FLAG_TEMPERATURE_UNIT));
	(p_measurement->IsTimestampAvailable)?(measurement[index]|=HEALTH_THERMOMETER_TEMPERATURE_MEASUREMENT_FLAG_TIMESTAMP):(measurement[index]&=(~HEALTH_THERMOMETER_TEMPERATURE_MEASUREMENT_FLAG_TIMESTAMP));
	(p_measurement->IsTemperatureTypeAvailable)?(measurement[index]|=HEALTH_THERMOMETER_TEMPERATURE_MEASUREMENT_FLAG_TEMPERATURE_TYPE):(measurement[index]&=(~HEALTH_THERMOMETER_TEMPERATURE_MEASUREMENT_FLAG_TEMPERATURE_TYPE));
	 index++;
  
    xmemcpy(&measurement[index],(u8 XDATA*)&p_measurement->temperatureMeasurementValue, 4);
    index+=4;	
	if (p_measurement->IsTimestampAvailable)
	{
		xmemcpy(&measurement[index],(u8 XDATA*)&p_measurement->timeStamp,sizeof(Time_Field));
		index+=sizeof(Time_Field);
	}
	if (p_measurement->temperatureType)
	{
	measurement[index] = p_measurement->temperatureType;
	index++;
	}

    p_value = (BLE_GATTS_HandleValueParams*)APP_HEAP_Alloc(sizeof(BLE_GATTS_HandleValueParams));
	if(p_value!=NULL)
	{
		p_value->charHandle=temperatureMeasurementHandle;
		p_value->charLength=index;
		p_value->sendType=BLE_GATT_HV_INDICATION;
		xmemcpy_u16(p_value->charValue, measurement, p_value->charLength);
		result=BLE_GATTS_SendHandleValue(connHandle, p_value);
		APP_HEAP_Free((u8 XDATA *)p_value);
		return result;
	}
	return MBIOT_RES_OOM;
}

PUBLIC extern u16 BLE_HTMS_Send_Intermediate_Temperature(u8 connHandle, BLE_HTMS_Measurement XDATA *p_intermediateTemperature) large
{
  XDATA u16 result;
	//XDATA float temp = 2.5;
  XDATA BLE_GATTS_HandleValueParams XDATA *p_value;
	XDATA u8 measurement[BLE_ATT_ATTRIBUTE_VALUE_LEN]=0, index;
	index = 0;	
	//Configure Flags
	(p_intermediateTemperature->IsTemperatureUnitAvailable)?(measurement[index]|=HEALTH_THERMOMETER_TEMPERATURE_MEASUREMENT_FLAG_TEMPERATURE_UNIT):(measurement[index]&=(~HEALTH_THERMOMETER_TEMPERATURE_MEASUREMENT_FLAG_TEMPERATURE_UNIT));
	(p_intermediateTemperature->IsTimestampAvailable)?(measurement[index]|=HEALTH_THERMOMETER_TEMPERATURE_MEASUREMENT_FLAG_TIMESTAMP):(measurement[index]&=(~HEALTH_THERMOMETER_TEMPERATURE_MEASUREMENT_FLAG_TIMESTAMP));
	(p_intermediateTemperature->IsTemperatureTypeAvailable)?(measurement[index]|=HEALTH_THERMOMETER_TEMPERATURE_MEASUREMENT_FLAG_TEMPERATURE_TYPE):(measurement[index]&=(~HEALTH_THERMOMETER_TEMPERATURE_MEASUREMENT_FLAG_TEMPERATURE_TYPE));
	index++;
    xmemcpy(&measurement[index],(u8 XDATA*)&p_intermediateTemperature->temperatureMeasurementValue, 4);
    index+=4;	
	
	if (p_intermediateTemperature->IsTimestampAvailable)
	{
	xmemcpy(&measurement[index],(u8 XDATA*)&p_intermediateTemperature->timeStamp,sizeof(Time_Field));
	index+=sizeof(Time_Field);
	}
	if (p_intermediateTemperature->temperatureType)
	{
	measurement[index] = p_intermediateTemperature->temperatureType;
	index++;
	}

    p_value = (BLE_GATTS_HandleValueParams*)APP_HEAP_Alloc(sizeof(BLE_GATTS_HandleValueParams));
	if(p_value!=NULL)
	{
		p_value->charHandle=intermediateTemperatureHandle;
		p_value->charLength=index;
		p_value->sendType=BLE_GATT_HV_NOTIFICATION;
		xmemcpy_u16(p_value->charValue, measurement, p_value->charLength);
		result=BLE_GATTS_SendHandleValue(connHandle, p_value);
		APP_HEAP_Free((u8 XDATA *)p_value);
		return result;
	}
	return MBIOT_RES_OOM;

}