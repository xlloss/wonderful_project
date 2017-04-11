/********************************************************************
  File Information:
    FileName:       ble_glucose_service.c
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
#include "ble_glucose_service.h"

static XDATA BLE_GLUCOSE_RecordsDatabase_Entry m_database[BLE_GLUCOSE_Database_MAX_RECORDS];
static XDATA u8          m_database_reference[BLE_GLUCOSE_Database_MAX_RECORDS];
static XDATA u16        m_num_records;




PUBLIC  MCODE  u8 APP_Defined_GlucoseServices[]={

	/*Glucose APP Service*/
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_SERVICE_2 */
	0x0A,	0x00,	0x00,0x00,	0x02,	UUID_PRIMARY_SERVICE>>8,UUID_PRIMARY_SERVICE,					0x00,0x00,	UUID_GLUCOSE_SERVICE>>8,UUID_GLUCOSE_SERVICE,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */

	0x0B,	0x00,	0x00,0x00,	0x02,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						NOTIFY,	0x00,0x00,	UUID_GLUCOSE_MEASUREMENT>>8,UUID_GLUCOSE_MEASUREMENT,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE */
	0x08,	0x00,	0x00,0x00,	0x02,	UUID_GLUCOSE_MEASUREMENT>>8,UUID_GLUCOSE_MEASUREMENT,	0x01,	0x00,
	0x08,	0x00,	0x00,0x00,	0x02,	UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG>>8,UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG,	0x00,0x00,
	
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */
	0x0B,	0x00,	0x00,0x00,	0x02,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						NOTIFY,	0x00,0x00,	UUID_GLUCOSE_MEASUREMENT_CONTEXT>>8,UUID_GLUCOSE_MEASUREMENT_CONTEXT,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE */
	0x08,	0x00,	0x00,0x00,	0x02,	UUID_GLUCOSE_MEASUREMENT_CONTEXT>>8,UUID_GLUCOSE_MEASUREMENT_CONTEXT,	0x01,	0x00,
	0x08,	0x00,	0x00,0x00,	0x02,	UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG>>8,UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG,	0x00,0x00,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */
	0x0B,	0x00,	0x00,0x00,	0x02,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						READ,	0x00,0x00,	UUID_GLUCOSE_Feature>>8,UUID_GLUCOSE_Feature,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE */
	0x09,	0x00,	0x00,0x00,	0x02,	UUID_GLUCOSE_Feature>>8,UUID_GLUCOSE_Feature,			        0x02,	0x00,0x00,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */
	0x0B,	0x00,	0x00,0x00,	0x02,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						WRITE|INDICATE,	0x00,0x00,	UUID_GLUCOSE_RECORD_ACCESS_CONTROL_POINT>>8,UUID_GLUCOSE_RECORD_ACCESS_CONTROL_POINT,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE */
	0x08,	PM_MANUAL_WRITE_RESP,	0x00,0x00,	0x02,	UUID_GLUCOSE_RECORD_ACCESS_CONTROL_POINT>>8,UUID_GLUCOSE_RECORD_ACCESS_CONTROL_POINT,	0x01,	0x00,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_DESCRIPTOR_CCCD */
	0x08,	0x00,	0x00,0x00,	0x02,	UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG>>8,UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG,	0x00,0x00

};

PUBLIC XDATA GlucoseProcessRoutine	GlucoseProcess;
PUBLIC u16 XDATA  BLE_Glucose_Feature;
PUBLIC u16 XDATA  glucoseFeatureHandle;
PUBLIC u16 XDATA  glucoseMeasurementHandle; 
PUBLIC u16 XDATA  glucoseMeasurementCCDHandle;
PUBLIC u16 XDATA  glucoseMeasurementContextHandle;
PUBLIC u16 XDATA  glucoseMeasurementContextCCDHandle;
PUBLIC u16 XDATA  glucoseRecordAccessControlPointHandle;
PUBLIC u16 XDATA  glucoseRecordAccessControlPointCCDHandle;



PUBLIC u16 BLE_GLUCOSE_Init(GlucoseProcessRoutine glucoseHandler)large
{
	XDATA u16 result;

	if(glucoseHandler==NULL)
	{
		return MBIOT_RES_INVALID_PARA;
	}
	GlucoseProcess =glucoseHandler;
	result = BLE_GLUCOSE_Database_Init;
		
	return MBIOT_RES_SUCCESS;
}

PUBLIC u16 BLE_GLUCOSE_Init_Service()large
{
	XDATA u16 startGlucoseHandle , result;
	XDATA BLE_GATT_ServicesInfo serviceInfo;
	serviceInfo.p_definedService = (u8 XDATA *)APP_Defined_GlucoseServices;
	serviceInfo.attributeNum = TOTAL_GLUCOSE_ATTR_NUM;
	serviceInfo.serviceSize = sizeof(APP_Defined_GlucoseServices);
    result = BLE_GATTS_InitServiceTable(&serviceInfo, &startGlucoseHandle);
	if(result==MBIOT_RES_SUCCESS)
	{
	glucoseMeasurementHandle=startGlucoseHandle+INDEX_GLUCOSE_MEASUREMENT_VALUE;
	glucoseMeasurementCCDHandle=startGlucoseHandle+INDEX_GLUCOSE_MEASUREMENT_CCCD;
	glucoseMeasurementContextHandle = startGlucoseHandle+INDEX_GLUCOSE_MEASUREMENT_CONTEXT_VALUE;
	glucoseMeasurementContextCCDHandle= startGlucoseHandle+INDEX_GLUCOSE_MEASUREMENT_CONTEXT_CCCD;
	glucoseFeatureHandle=startGlucoseHandle+INDEX_GLUCOSE_FEATURE_VALUE;
	glucoseRecordAccessControlPointHandle = startGlucoseHandle+INDEX_GLUCOSE_RECORD_ACCESS_CONTROL_POINT_VALUE;
	glucoseRecordAccessControlPointCCDHandle=startGlucoseHandle+INDEX_GLUCOSE_RECORD_ACCESS_CONTROL_POINT_CCCD;
		}

	return result;
}

PUBLIC u16 BLE_GLUCOSE_Database_Init(void)large
{
    u8 XDATA i;
   for (i = 0; i < BLE_GLUCOSE_Database_MAX_RECORDS; i++)
    {
        m_database[i].in_use_flag = FALSE;
        m_database_reference[i]    = 0xFF;
    }

    m_num_records = 0;

    return MBIOT_RES_SUCCESS;
}

PUBLIC void BLE_Glucose_Catch_Event(BLE_GATT_Event XDATA * p_event)large
{
    
	switch(p_event->eventID)
	{
		case BLE_GATTS_EVT_WRITE:
		{
		u16 XDATA handle;
		handle = p_event->eventField.onWrite.charHandle;
		if((handle == glucoseMeasurementCCDHandle)|| (handle == glucoseMeasurementContextCCDHandle)|| (handle == glucoseRecordAccessControlPointCCDHandle)||(handle == glucoseRecordAccessControlPointHandle))
		{
		if(GlucoseProcess){
			u8 XDATA *write_value;
			//u8 XDATA ans_log[] = "SecondStage";
			BLE_GLUCOSE_Event XDATA *p_glucoseEvent;
		    p_glucoseEvent = (BLE_GLUCOSE_Event XDATA *)APP_HEAP_Alloc(sizeof(BLE_GLUCOSE_Event));
		    //APP_UART_Put(sizeof(ans_log), ans_log); 
			write_value=APP_HEAP_Alloc(p_event->eventField.onWrite.writeDataLength);
			xmemcpy(write_value, p_event->eventField.onWrite.p_charValue, p_event->eventField.onWrite.writeDataLength);
			p_glucoseEvent = (BLE_GLUCOSE_Event XDATA *)APP_HEAP_Alloc(sizeof(BLE_GLUCOSE_Event));
			if(p_glucoseEvent!= NULL)
			{
			    
				p_glucoseEvent->handle = handle;
				p_glucoseEvent->write_value = write_value;
				p_glucoseEvent->receivedDataLength = p_event->eventField.onWrite.writeDataLength;
				APP_UART_Put(p_glucoseEvent->receivedDataLength , p_glucoseEvent->write_value); 
				GlucoseProcess(p_glucoseEvent);
				APP_HEAP_Free((u8 XDATA *)p_glucoseEvent);
			}
		}
		}
		}
		break;
		
		default:
		break;
	}
}

static void update_Glucose_feature()large
{
	u8 XDATA feature[2];
	
	feature[0] =BLE_Glucose_Feature;      
	feature[1]= BLE_Glucose_Feature>>8;   
	BLE_GATTS_SetHandleValue(glucoseFeatureHandle,sizeof(feature),feature);
}

PUBLIC u16 BLE_Glucose_Low_Battery_Detection(bool isLowBatterySupported) large
{
	(isLowBatterySupported)?(BLE_Glucose_Feature|=GLUCOSE_LOW_BATTERY_DETECTION_DURING_MEASUREMENT_SUPPORTED):(BLE_Glucose_Feature&=(~GLUCOSE_LOW_BATTERY_DETECTION_DURING_MEASUREMENT_SUPPORTED));
	 update_Glucose_feature();
	 return MBIOT_RES_SUCCESS;
}

PUBLIC u16 BLE_Glucose_Set_Sensor_Manfunction_Detection(bool isMalfunctionSupported) large
{
	(isMalfunctionSupported)?(BLE_Glucose_Feature|=GLUCOSE_SENSOR_MALFUNCTION_DETECTION_SUPPORTED):(BLE_Glucose_Feature&=(~GLUCOSE_SENSOR_MALFUNCTION_DETECTION_SUPPORTED));
	 update_Glucose_feature();
	 return MBIOT_RES_SUCCESS;
}

PUBLIC u16 BLE_Glucose_Set_SampleSize(bool isSamplesizeSupported) large
{
	(isSamplesizeSupported)?(BLE_Glucose_Feature|=GLUCOSE_SENSOR_SAMPLE_SIZE_SUPPORTED):(BLE_Glucose_Feature&=(~GLUCOSE_SENSOR_SAMPLE_SIZE_SUPPORTED));
	 update_Glucose_feature();
	 return MBIOT_RES_SUCCESS;
}

PUBLIC u16 BLE_Glucose_Set_Strip_Insertion_Error_Detection(bool isStripInsertionSupported) large
{
	(isStripInsertionSupported)?(BLE_Glucose_Feature|=GLUCOSE_SENSOR_STRIP_INSERTION_ERROR_DETECTION_SUPPORTED):(BLE_Glucose_Feature&=(~GLUCOSE_SENSOR_STRIP_INSERTION_ERROR_DETECTION_SUPPORTED));
	 update_Glucose_feature();
	 return MBIOT_RES_SUCCESS;
}

PUBLIC u16 BLE_Glucose_Set_Strip_Type(bool isStripTypeSupported) large
{
	(isStripTypeSupported)?(BLE_Glucose_Feature|=GLUCOSE_SENSOR_STRIP_TYPE_ERROR_DETECTION_SUPPORTED):(BLE_Glucose_Feature&=(~GLUCOSE_SENSOR_STRIP_TYPE_ERROR_DETECTION_SUPPORTED));
	 update_Glucose_feature();
	 return MBIOT_RES_SUCCESS;
}

PUBLIC u16 BLE_Glucose_Set_Result_High_Low_Detection(bool isHighLowResultSupported) large
{
	(isHighLowResultSupported)?(BLE_Glucose_Feature|=GLUCOSE_SENSOR_RESULT_HIGH_LOW_DETECTION_SUPPORTED):(BLE_Glucose_Feature&=(~GLUCOSE_SENSOR_RESULT_HIGH_LOW_DETECTION_SUPPORTED));
	 update_Glucose_feature();
	 return MBIOT_RES_SUCCESS;
}

PUBLIC u16 BLE_Glucose_Set_Temperature_High_Low_Detection(bool isHighLowTempSupported) large
{
	(isHighLowTempSupported)?(BLE_Glucose_Feature|=GLUCOSE_SENSOR_TEMPERATURE_HIGH_LOW_DETECTION_SUPPORTED):(BLE_Glucose_Feature&=(~GLUCOSE_SENSOR_TEMPERATURE_HIGH_LOW_DETECTION_SUPPORTED));
	 update_Glucose_feature();
	 return MBIOT_RES_SUCCESS;
}

PUBLIC u16 BLE_Glucose_Set_Read_Interrupt_Detection(bool isReadInterruptSupported) large
{
	(isReadInterruptSupported)?(BLE_Glucose_Feature|=GLUCOSE_SENSOR_READ_INTERRUPT_DETECTION_SUPPORTED):(BLE_Glucose_Feature&=(~GLUCOSE_SENSOR_READ_INTERRUPT_DETECTION_SUPPORTED));
	 update_Glucose_feature();
	 return MBIOT_RES_SUCCESS;
}


PUBLIC u16 BLE_Glucose_Set_Device_Fault_Supported(bool isDeviceFaultSupported) large
{
	(isDeviceFaultSupported)?(BLE_Glucose_Feature|=GLUCOSE_GENERAL_DEVICE_FAULT_SUPPORTED):(BLE_Glucose_Feature&=(~GLUCOSE_GENERAL_DEVICE_FAULT_SUPPORTED));
	 update_Glucose_feature();
	 return MBIOT_RES_SUCCESS;
}

PUBLIC u16 BLE_Glucose_Set_Time_Fault_Supported(bool isTimeFaultSupported) large
{
	(isTimeFaultSupported)?(BLE_Glucose_Feature|=GLUCOSE_TIME_FAULT_SUPPORTED):(BLE_Glucose_Feature&=(~GLUCOSE_TIME_FAULT_SUPPORTED));
	 update_Glucose_feature();
	 return MBIOT_RES_SUCCESS;
}

PUBLIC u16 BLE_Glucose_Set_Glucose_MultiBond(bool isMultiBondSupported) large
{
	(isMultiBondSupported)?(BLE_Glucose_Feature|=GLUCOSE_MULTIPLE_BOND_SUPPORTED):(BLE_Glucose_Feature&=(~GLUCOSE_MULTIPLE_BOND_SUPPORTED));
	 update_Glucose_feature();
	 return MBIOT_RES_SUCCESS;
}

PUBLIC u16 BLE_Add_Glucose_MeasurementDatabase(BLE_GLUCOSE_RecordsDatabase XDATA * p_recorddatabase)large
{
	    XDATA u8 i;
		//u8 XDATA ans_log[] = "BLE_Add_Glucose_MeasurementDatabase";
		//APP_UART_Put(sizeof(ans_log), ans_log);	
	    //APP_UART_Put(sizeof(BLE_GLUCOSE_RecordsDatabase),(u8 XDATA*)p_recorddatabase);
		if (m_num_records == BLE_GLUCOSE_Database_MAX_RECORDS)
		{
			return MBIOT_RES_INVALID_PARA;
		}

			// find next available database entry
		for (i = 0; i < BLE_GLUCOSE_Database_MAX_RECORDS; i++)
		{
			if (!m_database[i].in_use_flag)
			{
				m_database[i].in_use_flag = TRUE;
				if(p_recorddatabase->meas.IsContextInformationFollows==0){

	         m_database[i].record.meas= *(&p_recorddatabase->meas);
		     }
				 else{
				m_database[i].record	  = *p_recorddatabase;}
	      m_database_reference[m_num_records] = i;
				m_num_records++;
	
				return MBIOT_RES_SUCCESS;
			}
		}
	
		return MBIOT_RES_SUCCESS;


}
PUBLIC  u16 BLE_Delete_Glucose_MeasurementRecord(u8 XDATA record_index)large
{
		int i;
		u8 XDATA ans_log[] = "BLE_Delete_Glucose_MeasurementRecord";
		APP_UART_Put(sizeof(ans_log), ans_log);	
	
		if (record_index >= m_num_records)
		{
			return MBIOT_RES_INVALID_PARA;
		}
	
		// free entry
		m_database[m_database_reference[record_index]].in_use_flag = FALSE;
	
		// decrease number of records
		m_num_records--;
	
		// remove cross reference index
		for (i = record_index; i < m_num_records; i++)
		{
			m_database_reference[i] = m_database_reference[i + 1];
		}
	
		return MBIOT_RES_SUCCESS;
	
}
PUBLIC u16 BLE_Get_NumberOfRecords_Glucose_MeasurementRecord(void)large
{
	return m_num_records;
}


PUBLIC u16 BLE_Get_Glucose_MeasurementRecord(u8  record_index,BLE_GLUCOSE_RecordsDatabase XDATA * p_recorddatabase )large
{
    u8 XDATA ans_log[] = "BLE_Get_Glucose_MeasurementRecord";
	APP_UART_Put(sizeof(ans_log), ans_log); 
	if (record_index >= m_num_records)
		{
			return MBIOT_RES_INVALID_PARA;
		}
	
		// copy record to the specified memory
		*p_recorddatabase = m_database[m_database_reference[record_index]].record;
	
		return MBIOT_RES_SUCCESS;
}

PUBLIC u16 BLE_Send_Glucose_Write_Response(u8 connHandle, u8 respType)
{
	XDATA BLE_GATTS_SendWriteResponseParams XDATA *p_write_resp;
	XDATA u16 result;
	u8 XDATA ans_log[] = "BLE_Send_Glucose_Write_Response";
	
	p_write_resp = (BLE_GATTS_SendWriteResponseParams*)APP_HEAP_Alloc(sizeof(BLE_GATTS_SendWriteResponseParams));
	if(p_write_resp!=NULL)
	{
		APP_UART_Put(sizeof(ans_log), ans_log);
		p_write_resp->responseType = respType;
		result = BLE_GATTS_SendWriteResponse(connHandle, p_write_resp);
		APP_HEAP_Free((u8 XDATA *)p_write_resp);
		return result;
	}
	return MBIOT_RES_OOM;
}



PUBLIC u16 BLE_Send_Glucose_Error_Response(u8 connHandle, u8 errCode)
{
	BLE_GATTS_SendErrResponseParams XDATA * p_errParams;
	XDATA u16 result;
	u8 XDATA ans_log[] = "BLE_ANS_Send_Error_Response";
	
	p_errParams = (BLE_GATTS_SendErrResponseParams XDATA *)APP_HEAP_Alloc(sizeof(BLE_GATTS_SendErrResponseParams));
	if(p_errParams!=NULL) {	
		APP_UART_Put(sizeof(ans_log), ans_log);		
		p_errParams->reqOpcode = ATT_WRITE_REQUEST;
		p_errParams->charHandle = glucoseRecordAccessControlPointHandle;
		p_errParams->errorCode = errCode;
		result = BLE_GATTS_SendErrorResponse(connHandle, p_errParams);
		APP_HEAP_Free((u8 XDATA *)p_errParams);
		return result;
	}
	return MBIOT_RES_OOM;
}


PUBLIC u16 BLE_Send_Glucose_Measurement(u8 connHandle,BLE_GLUCOSE_Measurement XDATA * p_measurement)large
{
	u16 XDATA result;
	u16 XDATA concentrationUnit;
	u8  XDATA typeSampleLocation;
    u8 XDATA ans_log[]= "$$$$";
	 u8 XDATA ans_log1[]= "RETURN";
		XDATA BLE_GATTS_HandleValueParams XDATA *p_value;
		XDATA u8 measurement[MAX_GLUCOSE_MEASUREMENT_LEN]= 0, index;
		index = 0;
		APP_UART_Put(sizeof(ans_log), ans_log);
		//APP_UART_Put(sizeof(BLE_GLUCOSE_Measurement),(u8 XDATA *) p_measurement);
//Configure Flags
		(p_measurement->IsTimeOffsetEnabled)?(measurement[index]|=GLUCOSE_MEASUREMENT_FLAG_TIMEOFFSET):(measurement[index]&=(~GLUCOSE_MEASUREMENT_FLAG_TIMEOFFSET));
		(p_measurement->IsGlucoseConcentrationTypeSampleLocationPresent)?(measurement[index]|=GLUCOSE_MEASUREMENT_FLAG_GLUCOSE_CONCENTRATION_TYPE_LOCATION):(measurement[index]&=(~GLUCOSE_MEASUREMENT_FLAG_GLUCOSE_CONCENTRATION_TYPE_LOCATION));
		(p_measurement->IsGlucoseConcentrationUnits)?(measurement[index]|=GLUCOSE_MEASUREMENT_FLAG_GLUCOSE_CONCENTRATION_UNIT):(measurement[index]&=(~GLUCOSE_MEASUREMENT_FLAG_GLUCOSE_CONCENTRATION_UNIT));
		(p_measurement->IsSensorStatusAnnunciationPresent)?(measurement[index]|=GLUCOSE_MEASUREMENT_FLAG_SENSOR_STATUS_ANNUCIATION_PRESENT):(measurement[index]&=(~GLUCOSE_MEASUREMENT_FLAG_SENSOR_STATUS_ANNUCIATION_PRESENT));
		(p_measurement->IsContextInformationFollows)?(measurement[index]|=GLUCOSE_MEASUREMENT_FLAG_CONTEXT_INFORMATION):(measurement[index]&=(~GLUCOSE_MEASUREMENT_FLAG_CONTEXT_INFORMATION));
		index++;
		//APP_UART_Put(MAX_GLUCOSE_MEASUREMENT_LEN,measurement); 
//Sequence number
		EQU_Swap_u16pt(&measurement[index],&p_measurement->SequenceNumber);
		index = index+2;
		
//Base Time		
		xmemcpy(&measurement[index],(u8 XDATA*)&p_measurement->basetime,sizeof(Time_Field));
		index+=sizeof(Time_Field);

//timeoffset
		if(p_measurement->IsTimeOffsetEnabled){
			EQU_Swap_u16pt(&measurement[index],&p_measurement->timeoffset);
			index+=2;
		}
		
//Glucose Concentration, Type and Sample Location Present
		if((p_measurement->IsGlucoseConcentrationTypeSampleLocationPresent)){
			concentrationUnit= ((p_measurement->glucoseConcentrationUnitsPerLitre.exponent << 12) & 0xF000) |
                                ((p_measurement->glucoseConcentrationUnitsPerLitre.mantissa <<  0) & 0x0FFF);

		    //xmemcpy(&measurement[index],(u8 XDATA*)&concentrationUnit,2);
		    EQU_Swap_u16pt(&measurement[index],&concentrationUnit);
			index+=2;
			p_measurement->typeFieldN&=(0x0F);
			typeSampleLocation = ((p_measurement->sampleLocationN)<<4)|(p_measurement->typeFieldN);
			measurement[index] = typeSampleLocation;
			index+=1;
			}
//Glucose Concentration Units
			//p_measurement->glucoseConcentrationUnitsPerLitre.mantissa&=(~0xF000);
			//concentrationUnit =(((u16)p_measurement->glucoseConcentrationUnitsPerLitre.exponent)<<12)|((p_measurement->glucoseConcentrationUnitsPerLitre.mantissa));
			
//SensorStatus

		if(p_measurement->IsSensorStatusAnnunciationPresent){
			EQU_Swap_u16pt(&measurement[index],&p_measurement->sensorStatus);
			index+=2;
		    }
		//APP_UART_Put(sizeof(measurement),measurement);
		p_value = (BLE_GATTS_HandleValueParams*)APP_HEAP_Alloc(sizeof(BLE_GATTS_HandleValueParams));
		if(p_value!=NULL)
		{
			p_value->charHandle=glucoseMeasurementHandle;
			p_value->charLength=index;
			p_value->sendType=BLE_GATT_HV_NOTIFICATION;
			xmemcpy_u16(p_value->charValue, measurement, p_value->charLength);
			//APP_UART_Put(MAX_GLUCOSE_MEASUREMENT_LEN,measurement);
			result = BLE_GATTS_SendHandleValue(connHandle, p_value);
			APP_HEAP_Free((u8 XDATA *)p_value);
			APP_UART_Put(sizeof(ans_log1), ans_log1);		
		return result;
	}
	return MBIOT_RES_SUCCESS;
}


PUBLIC u16 BLE_Send_Glucose_Measurement_Context(u8 connHandle,BLE_GLUCOSE_Measurement_Context XDATA * p_measurementContext) large
{
        XDATA u16 result;
	  	u16 XDATA carbohydrateUnit;
		u8	XDATA testerHealth;
		u16 XDATA medicationIdPresent;
		u16 XDATA medicationValueUnit;
		u16 XDATA hbA1CprecisePercentagePresent;
		u8 XDATA ans_log[] = "BLE_Send_Glucose_Measurement_Context";
		XDATA BLE_GATTS_HandleValueParams XDATA *p_value;
		XDATA u8 measurementContext [MAX_GLUCOSE_MEASUREMENT_CONTEXT_LEN], index;
		index = 0;
	     APP_UART_Put(sizeof(ans_log), ans_log);
//Configure Flags
			(p_measurementContext->IsCarbohydrateIDPresent)?(measurementContext[index]|=GLUCOSE_MEASUREMENTCONTEXT_FLAG_CARBOHYDRATE_ID_PRESENT):(measurementContext[index]&=(~GLUCOSE_MEASUREMENTCONTEXT_FLAG_CARBOHYDRATE_ID_PRESENT));
			(p_measurementContext->IsMealPresent)?(measurementContext[index]|=GLUCOSE_MEASUREMENTCONTEXT_FLAG_MEAL_PRESENT):(measurementContext[index]&=(~GLUCOSE_MEASUREMENTCONTEXT_FLAG_MEAL_PRESENT));
			(p_measurementContext->IsTesterHealthEnabled)?(measurementContext[index]|=GLUCOSE_MEASUREMENTCONTEXT_FLAG_TESTER_HEALTH_PRESENT):(measurementContext[index]&=(~GLUCOSE_MEASUREMENTCONTEXT_FLAG_TESTER_HEALTH_PRESENT));
			(p_measurementContext->IsExerciseDurationIntensityPresent)?(measurementContext[index]|=GLUCOSE_MEASUREMENTCONTEXT_FLAG_EXERCISE_DURATION_INTENSITY_PRESENT):(measurementContext[index]&=(~GLUCOSE_MEASUREMENTCONTEXT_FLAG_EXERCISE_DURATION_INTENSITY_PRESENT));
			(p_measurementContext->IsMedicationIDPresent)?(measurementContext[index]|=GLUCOSE_MEASUREMENTCONTEXT_FLAG_MDICATION_ID_PRESENT):(measurementContext[index]&=(~GLUCOSE_MEASUREMENTCONTEXT_FLAG_MDICATION_ID_PRESENT));
			(p_measurementContext->IsMedicationValueUnit)?(measurementContext[index]|=GLUCOSE_MEASUREMENTCONTEXT_FLAG_MDICATION_VALUE_PRESENT):(measurementContext[index]&=(~GLUCOSE_MEASUREMENTCONTEXT_FLAG_MDICATION_VALUE_PRESENT));
			(p_measurementContext->IsHbA1CprecisePercentagePresent)?(measurementContext[index]|=GLUCOSE_MEASUREMENTCONTEXT_FLAG_HBA1CPRESENT):(measurementContext[index]&=(~GLUCOSE_MEASUREMENTCONTEXT_FLAG_HBA1CPRESENT));
			(p_measurementContext->IsExtendedFlagPresent)?(measurementContext[index]|=GLUCOSE_MEASUREMENTCONTEXT_FLAG_EXTENDED_FLAG_PRESENT):(measurementContext[index]&=(~GLUCOSE_MEASUREMENTCONTEXT_FLAG_EXTENDED_FLAG_PRESENT));
			 index++;
//Sequence number
			EQU_Swap_u16pt(&measurementContext[index],&p_measurementContext->SequenceNumber);
			index = index+2;

//CarbohydrateID and carbohydrate unit 
			if(p_measurementContext->IsCarbohydrateIDPresent){
			measurementContext[index] = p_measurementContext->CarbohydrateID;
			index++;
			//p_measurementContext->CarbohydrateUnit.mantissa&=(~0xF000);
		    //carbohydrateUnit =(((u16)p_measurementContext->CarbohydrateUnit.exponent)<<12)|((p_measurementContext->CarbohydrateUnit.mantissa));
		    carbohydrateUnit = ((p_measurementContext->CarbohydrateUnit.exponent << 12) & 0xF000) |
                                ((p_measurementContext->CarbohydrateUnit.mantissa <<  0) & 0x0FFF);
    	    EQU_Swap_u16pt(&measurementContext[index],&carbohydrateUnit);
		    index+=2;			
			}
//Meal 
            if(p_measurementContext->IsMealPresent){
			measurementContext[index] = p_measurementContext->Meal;
			index++;
			}
//TesterHealth
            if(p_measurementContext->IsTesterHealthEnabled){
			p_measurementContext->TesterN&=(0x0F);
			testerHealth= ((p_measurementContext->TesterN)<<4)|(p_measurementContext->HealthN);
			measurementContext[index] = testerHealth;
			index+=1;
			}
//ExerciseDurationAndIntensity 
			if(p_measurementContext->IsExerciseDurationIntensityPresent){
			EQU_Swap_u16pt(&measurementContext[index],&p_measurementContext->ExerciseDuration);
			index = index+2;
			measurementContext[index] = p_measurementContext->ExerciseIntensity;
			index++;		
			}
//MedicationIdandUnit
            if(p_measurementContext->IsMedicationIDPresent){
			measurementContext[index] = p_measurementContext->MedicationID;
			index++;
			//p_measurementContext->MedicationPresent.mantissa&=(~0xF000);
		    //medicationIdPresent =(((u16)p_measurementContext->MedicationPresent.exponent)<<12)|((p_measurementContext->MedicationPresent.mantissa));
		    medicationIdPresent = ((p_measurementContext->MedicationPresent.exponent << 12) & 0xF000) |((p_measurementContext->MedicationPresent.mantissa <<  0) & 0x0FFF);
    	    EQU_Swap_u16pt(&measurementContext[index],&medicationIdPresent);
		    index+=2;				
			}
//MedicationUnit
			if(p_measurementContext->IsMedicationValueUnit){
			//p_measurementContext->MedicationUnit.mantissa&=(~0xF000);
		   // medicationValueUnit =(((u16)p_measurementContext->MedicationUnit.exponent)<<12)|((p_measurementContext->MedicationUnit.mantissa));
		    medicationValueUnit = ((p_measurementContext->MedicationUnit.exponent << 12) & 0xF000) |((p_measurementContext->MedicationUnit.mantissa <<  0) & 0x0FFF);
    	    EQU_Swap_u16pt(&measurementContext[index],&medicationIdPresent);
		    index+=2;					
			}
//Hba1c
            if(p_measurementContext->IsHbA1CprecisePercentagePresent){
			//p_measurementContext->HbA1cS.mantissa&=(~0xF000);
		    //hbA1CprecisePercentagePresent =(((u16)p_measurementContext->HbA1cS.exponent)<<12)|((p_measurementContext->HbA1cS.mantissa));
		    hbA1CprecisePercentagePresent = ((p_measurementContext->HbA1cS.exponent << 12) & 0xF000) |((p_measurementContext->HbA1cS.mantissa <<  0) & 0x0FFF);
    	    EQU_Swap_u16pt(&measurementContext[index],&hbA1CprecisePercentagePresent);
		    index+=2;
				
            }
			p_value = (BLE_GATTS_HandleValueParams*)APP_HEAP_Alloc(sizeof(BLE_GATTS_HandleValueParams));
			if(p_value!=NULL)
			{
			APP_UART_Put(sizeof(ans_log), ans_log);
			p_value->charHandle=glucoseMeasurementContextHandle;
			p_value->charLength=index;
			p_value->sendType=BLE_GATT_HV_NOTIFICATION;
			xmemcpy_u16(p_value->charValue, measurementContext, p_value->charLength);
			result=BLE_GATTS_SendHandleValue(connHandle, p_value);
			APP_HEAP_Free((u8 XDATA *)p_value);						
			return result;
			
			}
return MBIOT_RES_OOM;
	
}

