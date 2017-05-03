/********************************************************************
  File Information:
    FileName:       ble_heart_rate_service.c
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
#include "ble_heart_rate_service.h"

extern int my_printf(const char *format, ...)large reentrant;

PUBLIC  MCODE  u8 APP_Defined_Services[]={

	/*Heart Scale Service*/
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_SERVICE_2 */
	0x0A,	0x00,	0x00,0x00,	0x02,	UUID_PRIMARY_SERVICE>>8,UUID_PRIMARY_SERVICE,					0x00,0x00,	UUID_HEART_RATE_SERVICE>>8,UUID_HEART_RATE_SERVICE,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */
	0x0B,	0x00,	0x00,0x00,	0x02,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						NOTIFY,	0x00,0x00,	UUID_HEART_RATE_MEASUREMENT>>8,UUID_HEART_RATE_MEASUREMENT,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE */
	0x08,	0x00,	0x00,0x00,	0x02,	UUID_HEART_RATE_MEASUREMENT>>8,UUID_HEART_RATE_MEASUREMENT,			0x01,	0x00,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_DESCRIPTOR_CCCD */
	0x08,	0x00,	0x00,0x00,	0x02,	UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG>>8,UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG,	0x00,0x00,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */
	0x0B,	0x00,	0x00,0x00,	0x02,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						READ,	0x00,0x00,	UUID_BODY_SENSOR_LOCATION>>8,UUID_BODY_SENSOR_LOCATION,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE */
	0x08,	0x00,	0x00,0x00,	0x02,	UUID_BODY_SENSOR_LOCATION>>8,UUID_BODY_SENSOR_LOCATION,	0x01,	0x04,
	/* ATT_ATTRUBUTE_HEADER_2 */
  0x0B,	0x00,	0x00,0x00,	0x02,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						WRITE,	0x00,0x00,	UUID_HEART_RATE_CONTROL_POINT>>8,UUID_HEART_RATE_CONTROL_POINT,
  /* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE */
	0x08,	0x00/*PM_MANUAL_WRITE_RESP*/,	0x00,0x00,	0x02,	UUID_HEART_RATE_CONTROL_POINT>>8,UUID_HEART_RATE_CONTROL_POINT,			0x01,	0x00,
};

PUBLIC XDATA HRSProcessRoutine	HRSProcess;
PUBLIC u16 XDATA  BLE_HR_BSL;
PUBLIC u16 XDATA  HRMearsuementHandle;
PUBLIC u16 XDATA  HRMearsuementCCCDHandle;
PUBLIC u16 XDATA  HRBodySensorLocationHandle;
PUBLIC u16 XDATA  HRControlPointHandle;
//PUBLIC u8 XDATA reset_energy_flag;

PUBLIC u16 BLE_HRS_Init(HRSProcessRoutine hrsHandler)large
{
	if(hrsHandler==NULL)
	{
		return MBIOT_RES_INVALID_PARA;
	}
	HRSProcess=hrsHandler;
		
	return MBIOT_RES_SUCCESS;
}

PUBLIC u16 BLE_HRS_Init_Service()large
{
	XDATA u16 startHandle, result;	
	XDATA BLE_GATT_ServicesInfo serviceInfo;

	serviceInfo.p_definedService = (u8 XDATA *)APP_Defined_Services;
	serviceInfo.attributeNum = TOTAL_HRS_ATTR_NUM;
	serviceInfo.serviceSize = sizeof(APP_Defined_Services);
	result=BLE_GATTS_InitServiceTable(&serviceInfo, &startHandle);
	
	if(result==MBIOT_RES_SUCCESS)
	{
		HRMearsuementHandle=startHandle+INDEX_HEART_RATE_MEASUREMENT_VALUE;
		HRMearsuementCCCDHandle=startHandle+INDEX_HEART_RATE_MEASUREMENT_CCCD;
		HRBodySensorLocationHandle=startHandle+INDEX_BODY_SENSOR_LOCATION_VALUE;
		HRControlPointHandle=startHandle+INDEX_HEART_RATE_CONTROL_POINT_VALUE;
	}

	return result;
}

PUBLIC void BLE_HRS_Catch_Event(BLE_GATT_Event XDATA * p_event) large
{
	XDATA APP_PWM_ConfParas pwm = {PWM_CLOCK_1024K, 24, 12, POSITIVE_OUTPUT};

	my_printf("BLE_HRS_Catch_Event\n");

	switch(p_event->eventID)
	{
			XDATA u8 pwm_value;

			case BLE_GATTS_EVT_WRITE:
			{
				if (p_event->eventField.onWrite.charHandle == HRControlPointHandle) {
					my_printf ("HRControlPointHandle\n");
					xmemcpy((u8 XDATA *)&pwm_value, p_event->eventField.onWrite.p_charValue, 
						p_event->eventField.onWrite.writeDataLength);
					
					pwm.compareValue = (pwm_value * 24) / 100;
					if (pwm.compareValue > 24 )
						pwm.compareValue = 24;

					APP_PWM_Stop (PWM1_MASK);
					APP_PWM_Config((PWM1_MASK), &pwm);
					APP_PWM_Start(PWM1_MASK);

					APP_PWM_Stop (PWM2_MASK);
					APP_PWM_Config((PWM2_MASK), &pwm);
					APP_PWM_Start(PWM2_MASK);
				}
				break;
			}

//		case BLE_GATTS_EVT_WRITE:
//		{
//			XDATA u16 cccd_value;
//			XDATA u8 cp_value;
//			BLE_HRS_Event XDATA *p_hrsEvent;
//
//			my_printf("BLE_GATTS_EVT_WRITE\n");
//			p_hrsEvent = (BLE_HRS_Event XDATA *)APP_HEAP_Alloc(sizeof(BLE_HRS_Event));
//
//			if(p_hrsEvent!= NULL)
//			{
//				if(p_event->eventField.onWrite.charHandle == HRMearsuementCCCDHandle)
//				{
//					my_printf("HRMearsuementCCCDHandle\n");
//					xmemcpy((u8 XDATA *)&cccd_value, p_event->eventField.onWrite.p_charValue, p_event->eventField.onWrite.writeDataLength);
//					p_hrsEvent->eventType = cccd_value;
//					p_hrsEvent->write_value = cccd_value;
//					HRSProcess(p_hrsEvent);
//
//				}
//				else if (p_event->eventField.onWrite.charHandle == HRControlPointHandle)
//				{
//					my_printf("HRControlPointHandle\n");
//					xmemcpy((u8 XDATA *)&cp_value, p_event->eventField.onWrite.p_charValue, p_event->eventField.onWrite.writeDataLength);
//					p_hrsEvent->eventType = HEART_RATE_CONTROL_POINT_WRITE;
//					p_hrsEvent->write_value = cp_value;
//					HRSProcess(p_hrsEvent);
//				}
//				APP_HEAP_Free((u8 XDATA *)p_hrsEvent);
//			}
//		}
//		break;
		
		default:
		break;
	}
}

PUBLIC u16	BLE_HRS_Body_Sensor_Location_Set(u8 body_sensor_location) large
{
    u16 XDATA len = sizeof(u8);
	u8 XDATA sensor_loc = body_sensor_location;
	if(sensor_loc > BLE_HRS_BODY_SENSOR_LOCATION_FOOT)
	{
		return MBIOT_RES_INVALID_PARA;
	}

	return BLE_GATTS_SetHandleValue(HRBodySensorLocationHandle, len, &sensor_loc);
}

PUBLIC u16 BLE_HRS_Send_Write_Response(u8 connHandle, u8 respType)
{
	XDATA BLE_GATTS_SendWriteResponseParams XDATA *p_write_resp;
	XDATA u16 result;
	
	p_write_resp = (BLE_GATTS_SendWriteResponseParams*)APP_HEAP_Alloc(sizeof(BLE_GATTS_SendWriteResponseParams));
	if(p_write_resp!=NULL)
	{
		p_write_resp->responseType = respType;
		result = BLE_GATTS_SendWriteResponse(connHandle, p_write_resp);
		APP_HEAP_Free((u8 XDATA *)p_write_resp);
		return result;
	}
	return MBIOT_RES_OOM;
}

PUBLIC u16 BLE_HRS_Send_Error_Response(u8 connHandle, u8 errCode)
{
	BLE_GATTS_SendErrResponseParams XDATA * p_errParams;
	XDATA u16 result;
	
	p_errParams = (BLE_GATTS_SendErrResponseParams XDATA *)APP_HEAP_Alloc(sizeof(BLE_GATTS_SendErrResponseParams));
	if(p_errParams!=NULL) {		
		p_errParams->reqOpcode = ATT_WRITE_REQUEST;
		p_errParams->charHandle = HRControlPointHandle;
		p_errParams->errorCode = errCode;
		result = BLE_GATTS_SendErrorResponse(connHandle, p_errParams);
		APP_HEAP_Free((u8 XDATA *)p_errParams);
		return result;
	}
	return MBIOT_RES_OOM;
}		

PUBLIC u16 BLE_HRS_Send_Measurement(u8 connHandle) large
{
	return MBIOT_RES_OOM;
}

//PUBLIC u16 BLE_HRS_Send_Measurement(u8 connHandle, BLE_HRS_Measurement XDATA *p_measurement) large
//{
//	u16 XDATA result;
//	XDATA BLE_GATTS_HandleValueParams XDATA *p_value;
//	XDATA u8 measurement[MAX_HEART_RATE_MEASUREMENT_LEN]=0, index=0, i;
//		
//	//Configure Flags
//	(p_measurement->isHeartRateValueFormatsupportedBit)?(measurement[index]|=HEART_RATE_MEASUREMENT_FLAG_VALUE_FORMAT):(measurement[index]&=(~HEART_RATE_MEASUREMENT_FLAG_VALUE_FORMAT));
//	(p_measurement->isSensorContactSupportedBit)?(measurement[index]|=HEART_RATE_MEASUREMENT_FLAG_SENSOR_SUPPORTED):(measurement[index]&=(~HEART_RATE_MEASUREMENT_FLAG_SENSOR_SUPPORTED));//these are not conditional 
//	(p_measurement->isSensorContactDetectedBit)?(measurement[index]|=HEART_RATE_MEASUREMENT_FLAG_SENSOR_DETECTED):(measurement[index]&=(~HEART_RATE_MEASUREMENT_FLAG_SENSOR_DETECTED));// these are not condition
//	(p_measurement->isEnergyExpendedStatusBit)?(measurement[index]|=HEART_RATE_MEASUREMENT_FLAG_ENERGY_EXPENDED):(measurement[index]&=(~HEART_RATE_MEASUREMENT_FLAG_ENERGY_EXPENDED));
//	(p_measurement->isRRIntervalBit)?(measurement[index]|=HEART_RATE_MEASUREMENT_FLAG_RR_INTERVAL):(measurement[index]&=(~HEART_RATE_MEASUREMENT_FLAG_RR_INTERVAL));
//		
//  index++;
//  
//  if(p_measurement->isHeartRateValueFormatsupportedBit)
//	{
//		EQU_Swap_u16pt(&measurement[index],&p_measurement->heart_rate);
//		index+=2;//u16 bit
//	}
//	else
//	{
//		xmemcpy(&measurement[index],(u8 XDATA*)&p_measurement->heart_rate, 1);
//		index+=1;// u8 bit
//	}
//		
//	//Energy expended
//	if (p_measurement->isEnergyExpendedStatusBit)
//	{
//		//if(reset_energy_flag)
//		//{
//		//	memset((u8 XDATA *)&p_measurement->energy_expended,0,2);
//		//	reset_energy_flag = 0;//0
//		//}//This logic moved to application.c
//		EQU_Swap_u16pt(&measurement[index],&p_measurement->energy_expended);
//		index+=sizeof(p_measurement->energy_expended);
//	}
//
//    // Encode rr_interval values
//  if (p_measurement->isRRIntervalBit)			
//  {
//		if(p_measurement->rr_interval_count > BLE_HRS_MAX_BUFFERED_RR_INTERVALS)
//		{
//			p_measurement->rr_interval_count = BLE_HRS_MAX_BUFFERED_RR_INTERVALS;
//		}
//    for (i = 0; i < p_measurement->rr_interval_count; i++)
//    {
//			EQU_Swap_u16pt(&measurement[index],&p_measurement->rr_interval[i]);
//			index+=2;
//		}
//	}
//	p_value = (BLE_GATTS_HandleValueParams*)APP_HEAP_Alloc(sizeof(BLE_GATTS_HandleValueParams));
//	if(p_value!=NULL)
//	{
//		p_value->charHandle=HRMearsuementHandle;
//		p_value->charLength=index;
//		p_value->sendType=BLE_GATT_HV_NOTIFICATION;
//		xmemcpy_u16(p_value->charValue, measurement, p_value->charLength);
//		result=BLE_GATTS_SendHandleValue(connHandle, p_value);
//		APP_HEAP_Free((u8 XDATA *)p_value);
//		return result;
//	}
//	return MBIOT_RES_OOM;
//}
