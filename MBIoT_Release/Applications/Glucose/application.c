/********************************************************************
  File Information:
    FileName:       application.c
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


/*******************************************************************/
/***                       Include Files                         ***/
/*******************************************************************/
#include "mbiot.h"
#include "application.h"

/**@brief BLE Service Include File. */
#include "ble_glucose_service.h"

/*******************************************************************/
/***                 Public Variable Declaration                 ***/
/*******************************************************************/
PUBLIC  MCODE    u8   APP_VERSION[]={0x00,0x00};

#ifdef APP_OPEN_DEBUG_MODE
PUBLIC XDATA u8 App_Debug_Tx_Buffer[APP_DEBUG_TX_BUFFER_LEN];
PUBLIC XDATA u8 App_Debug_Rx_Buffer[APP_DEBUG_RX_BUFFER_LEN];
#endif
/**@brief Definition of dynamic memory. The size can be configured in "application.h". */
PUBLIC  XDATA APP_HEAP_STRUCT         App_Heap[APP_HEAP_BLOCK_NUMBER];
PUBLIC  XDATA APP_HEAP_INDEX          App_Heap_Index[APP_HEAP_BLOCK_NUMBER];

/**@brief Global Variables for Application Used. */
static XDATA u8 AppRxBuffer[APP_RX_BUFFER_LEN];			/**< Application UART RX Buffer. */
static XDATA u8 AppTxBuffer[APP_TX_BUFFER_LEN];			/**< Application UART TX Buffer. */
static XDATA u8 uart_cmd_buffer[MAX_UART_CMD_BUFFER];	/**< Application Receiving UART Command Buffer. */
static XDATA u8 uart_cmd_index;							/**< Application Buffer Index for Receiving UART Command Buffer. */
static XDATA u8 uart_cmd_wait;							/**< APPlication Variable for Recording Receiving UART Command Status. */
static XDATA APP_CONN_STRUCT XDATA *p_app_conn;			/**< Pointer to Application Connection Parameters Structure. */
static XDATA BLE_Glucose_RACPstate   XDATA m_gls_RACPstate;       /**< Current Glucose RACP communication state. */
static XDATA u16           m_next_seq_num;
static XDATA u8            m_racp_proc_operator;                          /**< Operator of current request. */
static XDATA u16           m_racp_proc_seq_num;                           /**< Sequence number of current request. */
static XDATA u8            m_racp_proc_record_ndx;                        /**< Current record index. */
static XDATA u8            m_racp_proc_records_reported;                  /**< Number of reported records. */
static XDATA u8            m_racp_proc_records_reported_since_txcomplete; /**< Number of reported records since last TX_COMPLETE event. */
//static XDATA BLE_Glucose_RACP XDATA *m_pending_racp_response;    /**< RACP response to be sent. */
static XDATA u16           m_racp_handle;                           /**< Racp handle  */
static XDATA bool NotificatioEnable;





/**@brief Definition of events. */
static MCODE APP_EVENT_STRUCT app_event[]=
{
	{{EVT_LEN_ADVERTISING},			{'A','d','v','e','r','t','i','s','i','n','g',UART_END_PATTERN>>8,UART_END_PATTERN}},
	{{EVT_LEN_UNSPECIFIC_ERROR},	{'U','n','s','p','e','c','i','f','i','c',' ','E','r','r','o','r',UART_END_PATTERN>>8,UART_END_PATTERN}},
	{{EVT_LEN_UNKNOWN_COMMAND},		{'U','n','k','n','o','w',' ','C','o','m','m','a','n','d',UART_END_PATTERN>>8,UART_END_PATTERN}},
	{{EVT_LEN_CONNECTED},			{'C','o','n','n','e','c','t','e','d',UART_END_PATTERN>>8,UART_END_PATTERN}},
	{{EVT_LEN_DISCONNECTED},		{'D','i','s','c','o','n','n','e','c','t','e','d',UART_END_PATTERN>>8,UART_END_PATTERN}},
	{{EVT_LEN_CHANNEL_ENABLED},		{'D','a','t','a',' ','C','h','a','n','n','e','l',' ','E','n','a','b','l','e','d',UART_END_PATTERN>>8,UART_END_PATTERN}},
	{{EVT_LEN_CHANNEL_DISABLED},	{'D','a','t','a',' ','C','h','a','n','n','e','l',' ','D','i','s','a','b','l','e','d',UART_END_PATTERN>>8,UART_END_PATTERN}},
};

static MCODE APP_CMD_STRUCT app_cmd[]=
{
	{{CMD_LEN_GLUCOSE_MEASUREMENT},			{"GlucoseM"}},
	{{CMD_LEN_GLUCOSE_MEASUREMENT_CONTEXT},			{"MEASUREMENT"}},
	{{CMD_LEN_DISCONNECT},		{"Disconnect"}},
};


/*******************************************************************/
/***                    Application Functions                    ***/
/*******************************************************************/

/**@brief Function for starting advertising. */
static u16 app_start_advertising()large reentrant
{
	BLE_GAP_AdvertisingParas XDATA advpara;
	BLE_GAP_SetAdvertisingDataParas	XDATA advData;
	XDATA u16 result;
	 XDATA u8 temp[]={0x02,0x01,0x05,0x08,0x09,0x47,0x6c,0x75,0x63,0x6f,0x73,0x65,0x03,0x03,0x08,0x18};

	advpara.type=BLE_GAP_ADV_TYPE_ADV_IND;
	advpara.interval=0x0020;		
	advData.adLen=sizeof(temp);
	xmemcpy(advData.adData,temp,advData.adLen );
	BLE_GAP_Set_AdvertisingData(&advData);
	result=BLE_GAP_Advertising_Start(&advpara);
	return result;
}

/**@brief Function for sending events by UART.
 *
 * @param[in] index	  The index of app_event	
 */
static void app_send_event(u8 index)large reentrant
{
	XDATA u8 event[MAX_UART_EVT_BUFFER];			
	comemcpy(event, app_event[index].content,app_event[index].length);
	APP_UART_Put(app_event[index].length, event);				
}

/**@brief Function for initializing application. */
static void app_init()large reentrant
{
	XDATA u16 result;
	
	result=app_start_advertising();
	
	if(result==MBIOT_RES_SUCCESS)
	{
		app_send_event(EVT_ADVERTISING);
	}
	else
	{
		app_send_event(EVT_UNSPECIFIC_ERROR);
	}	
}

/**@brief Function for setting the Glucose RACP  communication state.
 *
 * @param[in] new_state  New communication state.
 */

static void BLE_Send_RACP_Response(BLE_Glucose_RACP XDATA *p_recordAccessPointResponse)large
{
			u8 XDATA ans_log3[] = "BLE_Send_RACP_Response";
			u8 XDATA racpData[3]=0;
	        XDATA u16 result;
			XDATA BLE_GATTS_HandleValueParams XDATA *p_value;
			APP_UART_Put(sizeof(ans_log3), ans_log3);
			racpData[0] =p_recordAccessPointResponse->opCode;	   
			racpData[1]= p_recordAccessPointResponse->oPerator;
			racpData[2]=p_recordAccessPointResponse->operand & 0xFF; 
			racpData[3]= p_recordAccessPointResponse->operand>>8;			
			p_value = (BLE_GATTS_HandleValueParams*)APP_HEAP_Alloc(sizeof(BLE_GATTS_HandleValueParams));
			if (p_value!= NULL){
			   p_value->charHandle=glucoseRecordAccessControlPointHandle;
			   p_value->charLength=4;
			   p_value->sendType=BLE_GATT_HV_INDICATION;
			   xmemcpy_u16(p_value->charValue, racpData, p_value->charLength);
			   APP_UART_Put(sizeof(ans_log3), ans_log3);
			   result = BLE_GATTS_SendHandleValue(p_app_conn->handle, p_value);
			   APP_HEAP_Free((u8 XDATA *)p_value);
			   }


}
static void BLE_Read_Glucose_measurement(void)
{
    XDATA BLE_GLUCOSE_Measurement XDATA *p_glucoseMeasurement;
  	//XDATA BLE_Glucose_RACP XDATA *m_recordAccessPoint;
	XDATA  u16 s_mantissa = 550;
    XDATA  u8 s_exponent = 0;
    XDATA  u8 s_secs     = 5;
	XDATA u8 uartLog[]="RDRDRD";
//	XDATA result;
	APP_UART_Put(sizeof(uartLog), uartLog);
	p_glucoseMeasurement=(BLE_GLUCOSE_Measurement XDATA *)APP_HEAP_Alloc(sizeof(BLE_GLUCOSE_Measurement));
	if(p_glucoseMeasurement!=NULL)
	{

    p_glucoseMeasurement->IsContextInformationFollows = 0;
	p_glucoseMeasurement->IsGlucoseConcentrationTypeSampleLocationPresent = 1;
	p_glucoseMeasurement->IsGlucoseConcentrationUnits = 1;
	p_glucoseMeasurement->IsSensorStatusAnnunciationPresent = 1;
	p_glucoseMeasurement->IsTimeOffsetEnabled = 1;
	p_glucoseMeasurement->SequenceNumber = m_next_seq_num++;
    p_glucoseMeasurement->basetime.year  = 2016;
    p_glucoseMeasurement->basetime.month                = 4;
    p_glucoseMeasurement->basetime.day                  = 4;
    p_glucoseMeasurement->basetime.hours                = 12;
    p_glucoseMeasurement->basetime.minutes              = 30;
    p_glucoseMeasurement->basetime.seconds              = 15;
    p_glucoseMeasurement->glucoseConcentrationUnitsPerLitre.exponent = s_exponent;
    p_glucoseMeasurement->glucoseConcentrationUnitsPerLitre.mantissa = s_mantissa;
    p_glucoseMeasurement->timeoffset= 0;
    p_glucoseMeasurement->typeFieldN     = GLUCOSE_ARTERIAL_WHOLE_BLOOD;
    p_glucoseMeasurement->sampleLocationN= 0x01;
    p_glucoseMeasurement->sensorStatus= 0;
	BLE_Add_Glucose_MeasurementDatabase((BLE_GLUCOSE_RecordsDatabase XDATA *)p_glucoseMeasurement);
	//result= BLE_Send_Glucose_Measurement(p_app_conn->handle,p_glucoseMeasurement);
	/*switch(result)
		{
		case MBIOT_RES_SUCCESS:
				{
				XDATA u8 uartLog[]="SUCCESS";
				APP_UART_Put(sizeof(uartLog), uartLog);
				m_recordAccessPoint = (BLE_Glucose_RACP XDATA *)APP_HEAP_Alloc(sizeof(BLE_Glucose_RACP));
				if(m_recordAccessPoint!=NULL){
				m_recordAccessPoint->opCode = GLUCOSE_REQUEST_OP_CODE;
				m_recordAccessPoint->oPerator = GLUCOSE_NULL;
				m_recordAccessPoint->operand = GLUCOSE_SUCCESS_NORMAL_RESPONSE;
				BLE_Send_RACP_Response(m_recordAccessPoint);
	}
				 }
			break;
	    case MBIOT_RES_INVALID_PARA:
	    	{
	    	{
				 XDATA u8 uartLog[]="INVALID";
				 APP_UART_Put(sizeof(uartLog), uartLog);
			}
   		}
			break;
		default:
			break;
		
		}*/
		}
    // change values for next read.
    s_mantissa += 23;
    if (s_mantissa > 939)
    {
        s_mantissa -= 434;
    }

    s_secs += 3;
    if (s_secs > 59)
    {
        s_secs = 0;
    }

   }
static void BLE_GlucoseRACPstate_set(BLE_Glucose_RACPstate new_state)large
{
    m_gls_RACPstate = new_state;
}

/**@brief Function for parsing received UART data.
 *
 * @param[in] p_payload	  Pointer to the payload buffer	
 */
static void app_uart_parsing(u8 XDATA *p_payload)large reentrant
{
	XDATA u8 i;

	for(i=0;i<CMD_END;i++)
	{
		if(!comemcmp(p_payload, app_cmd[i].content,app_cmd[i].length))
		{
			break;
		}
	}
	
	switch(i)
	{
		case CMD_SEND_GLUCOSE_MEASUREMENT:
			{
			u8 XDATA ans_log[] = "DMD_SEND_GLUCOSE_MEASUREMENT";
			XDATA BLE_GLUCOSE_Measurement XDATA *p_glucoseMeasurement;
			u8 XDATA glucoseMeasurementFlag, payloadIndex=CMD_LEN_GLUCOSE_MEASUREMENT;
			APP_UART_Put(sizeof(ans_log), ans_log);
			p_glucoseMeasurement=(BLE_GLUCOSE_Measurement XDATA *)APP_HEAP_Alloc(sizeof(BLE_GLUCOSE_Measurement));
			if(p_glucoseMeasurement!=NULL)
			{
			    xmemset((u8 XDATA *)p_glucoseMeasurement,0,sizeof(BLE_GLUCOSE_Measurement));
				glucoseMeasurementFlag = *(p_payload+payloadIndex);
				payloadIndex++;
				xmemcpy((u8 XDATA *)&p_glucoseMeasurement->SequenceNumber,p_payload+payloadIndex,2);
				//p_glucoseMeasurement->SequenceNumber = m_next_seq_num++;
				payloadIndex+=2; 
		        xmemcpy((u8 XDATA *)&p_glucoseMeasurement->basetime.year,p_payload+payloadIndex,2);
				payloadIndex+=2;
				p_glucoseMeasurement->basetime.month= *(p_payload+payloadIndex);
				payloadIndex+=1;
				p_glucoseMeasurement->basetime.day= *(p_payload+payloadIndex);
				payloadIndex+=1;
				p_glucoseMeasurement->basetime.hours= *(p_payload+payloadIndex);
				payloadIndex+=1;
				p_glucoseMeasurement->basetime.minutes= *(p_payload+payloadIndex);
				payloadIndex+=1;
				p_glucoseMeasurement->basetime.seconds= *(p_payload+payloadIndex);
				payloadIndex+=1;
				
				
			if(glucoseMeasurementFlag&0x01){
				xmemcpy((u8 XDATA *)&p_glucoseMeasurement->timeoffset,(p_payload+payloadIndex),2);
			    p_glucoseMeasurement->IsTimeOffsetEnabled= 1;
				payloadIndex+=2;
				
				}
			if(glucoseMeasurementFlag&0x02){
				p_glucoseMeasurement->IsGlucoseConcentrationTypeSampleLocationPresent=1;
				p_glucoseMeasurement->glucoseConcentrationUnitsPerLitre.exponent=*(p_payload+payloadIndex);
				payloadIndex+=1;
				xmemcpy((u8 XDATA *)&p_glucoseMeasurement->glucoseConcentrationUnitsPerLitre.mantissa,(p_payload+payloadIndex),2);
				payloadIndex+=2;
				if(glucoseMeasurementFlag&0x04){
				   p_glucoseMeasurement->IsGlucoseConcentrationUnits= GLUCOSE_CONCENTRATION_UNIT_MOL;
			  	}
			    else{
				p_glucoseMeasurement->IsGlucoseConcentrationUnits = GLUCOSE_CONCENTRATION_UNIT_KGL;
				}
				p_glucoseMeasurement->sampleLocationN= *(p_payload+payloadIndex);
				payloadIndex+=1;
			    p_glucoseMeasurement->typeFieldN= *(p_payload+payloadIndex);
				payloadIndex+=1;
				}
			if(glucoseMeasurementFlag&0x08){
				p_glucoseMeasurement->IsSensorStatusAnnunciationPresent= 1;
			    xmemcpy((u8 XDATA *)&p_glucoseMeasurement->sensorStatus,(p_payload+payloadIndex),2);
				payloadIndex+=2;
			    }
			    p_glucoseMeasurement->IsContextInformationFollows = 0;
				//APP_UART_Put(sizeof(BLE_GLUCOSE_Measurement),(u8 XDATA *) p_glucoseMeasurement);
				BLE_Add_Glucose_MeasurementDatabase((BLE_GLUCOSE_RecordsDatabase XDATA *)p_glucoseMeasurement);
				BLE_Send_Glucose_Measurement(p_app_conn->handle,p_glucoseMeasurement);
			   }
		}				
		break;
		case CMD_SEND_GLUCOSE_MEASUREMENT_CONTEXT:
			{
			u8 XDATA ans_log[] = "CMD_SEND_GLUCOSE_MEASUREMENT_CONTEXT";
			XDATA BLE_GLUCOSE_Measurement_Context XDATA *p_glucoseMeasurementContext;
			u8 XDATA glucoseMeasurementContextFlag, payloadIndex=CMD_LEN_GLUCOSE_MEASUREMENT_CONTEXT;
			APP_UART_Put(sizeof(ans_log), ans_log);
			p_glucoseMeasurementContext=(BLE_GLUCOSE_Measurement_Context XDATA *)APP_HEAP_Alloc(sizeof(BLE_GLUCOSE_Measurement_Context));
			if(p_glucoseMeasurementContext!=NULL)
			{
			    u8 XDATA ans_log[] = "MEASUREMENT_CONTEXT_DATA";
				APP_UART_Put(sizeof(ans_log), ans_log);
			    xmemset((u8 XDATA *)p_glucoseMeasurementContext,0,sizeof(BLE_GLUCOSE_Measurement_Context));
				glucoseMeasurementContextFlag = *(p_payload+payloadIndex);
				payloadIndex++;
				xmemcpy((u8 XDATA *)&p_glucoseMeasurementContext->SequenceNumber,p_payload+payloadIndex,2);
				payloadIndex+=2;
				
			if(glucoseMeasurementContextFlag&0x01){
				p_glucoseMeasurementContext->CarbohydrateID = *(p_payload+payloadIndex);
				payloadIndex++;
				p_glucoseMeasurementContext->CarbohydrateUnit.exponent=*(p_payload+payloadIndex);
				payloadIndex++;
				xmemcpy((u8 XDATA *)&p_glucoseMeasurementContext->CarbohydrateUnit.mantissa,(p_payload+payloadIndex),2);
				payloadIndex+=2;
			}
			if(glucoseMeasurementContextFlag&0x02){
				p_glucoseMeasurementContext->Meal = *(p_payload+payloadIndex);				
			}
			if(glucoseMeasurementContextFlag&0x04){
				p_glucoseMeasurementContext->TesterN= *(p_payload+payloadIndex);
				payloadIndex+=1;
			    p_glucoseMeasurementContext->HealthN= *(p_payload+payloadIndex);
				payloadIndex+=1;				
			}
			if(glucoseMeasurementContextFlag&0x08){
				xmemcpy((u8 XDATA *)&p_glucoseMeasurementContext->ExerciseDuration,p_payload+payloadIndex,2);
				payloadIndex+=2;
				p_glucoseMeasurementContext->ExerciseIntensity= *(p_payload+payloadIndex);
				payloadIndex++;
				
			}
			if(glucoseMeasurementContextFlag&0x10){
				p_glucoseMeasurementContext->MedicationID = *(p_payload+payloadIndex);
				payloadIndex++;
				p_glucoseMeasurementContext->MedicationPresent.exponent = *(p_payload+payloadIndex);
				payloadIndex++;
				xmemcpy((u8 XDATA *)&p_glucoseMeasurementContext->MedicationPresent.mantissa,(p_payload+payloadIndex),2);
				payloadIndex+=2;			
			}
			if(glucoseMeasurementContextFlag&0x20){
				p_glucoseMeasurementContext->MedicationUnit.exponent = *(p_payload+payloadIndex);
				payloadIndex++;
				xmemcpy((u8 XDATA *)&p_glucoseMeasurementContext->MedicationUnit.mantissa,(p_payload+payloadIndex),2);
				payloadIndex+=2;
			}
			if(glucoseMeasurementContextFlag&0x40){
				p_glucoseMeasurementContext->HbA1cS.exponent= *(p_payload+payloadIndex);
				xmemcpy((u8 XDATA *)&p_glucoseMeasurementContext->HbA1cS.mantissa,(p_payload+payloadIndex),2);						
			}
			APP_UART_Put(sizeof(ans_log), ans_log);
			BLE_Send_Glucose_Measurement_Context(p_app_conn->handle,p_glucoseMeasurementContext);
			//BLE_Add_Glucose_MeasurementDatabase((BLE_GLUCOSE_RecordsDatabase XDATA *)p_glucoseMeasurement);
			
			}

			}


		break;
		
		case CMD_DISCONNECT:
			BLE_GAP_TerminateConnection(p_app_conn->handle,BLE_GAP_DISCONNECT_REASON_REMOTE_TERMINATE);
		break;
		
		default:
			app_send_event(EVT_UNKNOWN_COMMAND);
		break;
	}
}


static u16 BLE_RACP_Report_GreaterOrEqual_Record(void)large
{
	u16 total_records = BLE_Get_NumberOfRecords_Glucose_MeasurementRecord();
	while (m_racp_proc_record_ndx < total_records)
    {
        u16 result ;
        BLE_GLUCOSE_RecordsDatabase XDATA *rec;
        rec=(BLE_GLUCOSE_RecordsDatabase XDATA *)APP_HEAP_Alloc(sizeof(BLE_GLUCOSE_RecordsDatabase));
        result = BLE_Get_Glucose_MeasurementRecord(m_racp_proc_record_ndx, rec);
        if (result != MBIOT_RES_SUCCESS)
        {
            return result;
        }

        if (rec->meas.SequenceNumber >= m_racp_proc_seq_num)
        {
            result = BLE_Send_Glucose_Measurement(p_app_conn->handle, (BLE_GLUCOSE_Measurement XDATA *)rec);
            if (result != MBIOT_RES_SUCCESS)
            {
                return result;
            }
            break;
        }
        m_racp_proc_record_ndx++;
    }
    if (m_racp_proc_record_ndx == total_records)
    {
        BLE_GlucoseRACPstate_set(STATE_NO_COMM);
    }

    return MBIOT_RES_SUCCESS;


}

static u16 BLE_RACP_Report_First_Last_Record(void)large
{
	u8 XDATA ans_log3[] = "BLE_RACP_Report_First_Last_Record";
	XDATA u16      result;
    BLE_GLUCOSE_RecordsDatabase XDATA *rec;
    XDATA u16  total_records=0;
    APP_UART_Put(sizeof(ans_log3), ans_log3);
	rec=(BLE_GLUCOSE_RecordsDatabase XDATA *)APP_HEAP_Alloc(sizeof(BLE_GLUCOSE_RecordsDatabase));
    total_records = BLE_Get_NumberOfRecords_Glucose_MeasurementRecord();

    if ((m_racp_proc_records_reported != 0) || (total_records == 0))
    {
        BLE_GlucoseRACPstate_set(STATE_NO_COMM);
    }
    else
    {
        if (m_racp_proc_operator == GLUCOSE_FIRST_RECORD)
        {
            result = BLE_Get_Glucose_MeasurementRecord(0, rec);
			      //APP_UART_Put(sizeof(BLE_GLUCOSE_Measurement),(u8 XDATA *)rec);
            if (result != MBIOT_RES_SUCCESS)
            {
                return result;
            }
        }
        else if (m_racp_proc_operator == GLUCOSE_LAST_RECORD)
        {
            result = BLE_Get_Glucose_MeasurementRecord(total_records - 1, rec);
			      //APP_UART_Put(sizeof(BLE_GLUCOSE_Measurement),(u8 XDATA *)rec);
            if (result != MBIOT_RES_SUCCESS)
            {
                return result;
            }
        }

         result = BLE_Send_Glucose_Measurement(p_app_conn->handle,(BLE_GLUCOSE_Measurement XDATA *)rec);
        if (result != MBIOT_RES_SUCCESS)
        {
            return result;
        }
		else{
         m_racp_proc_records_reported++;
		}
    	}
	return MBIOT_RES_SUCCESS;

}
static void BLE_Report_Number_Stored_Records(BLE_Glucose_RACP XDATA *p_recordAccessPoint)large
{
    XDATA u16 total_records=0;
    XDATA u16 num_records=0;
	u8 XDATA ans_log[] = "#########";
	XDATA BLE_Glucose_RACP XDATA *m_pending_racp_response;
	APP_UART_Put(sizeof(ans_log), ans_log); 
    total_records = BLE_Get_NumberOfRecords_Glucose_MeasurementRecord();
    num_records   = 0;
    if (p_recordAccessPoint->oPerator == GLUCOSE_ALL_RECORDS)
    {
        num_records = total_records;
    }
    else if (p_recordAccessPoint->oPerator == GLUCOSE_GREATER_THAN_OR_EQUAL_TO)
    {
        u16 seq_num;
        u16 i;

        seq_num = p_recordAccessPoint->operand;

        for (i = 0; i < total_records; i++)
        {
            u16 result;
            BLE_GLUCOSE_RecordsDatabase XDATA *rec;

            result = BLE_Get_Glucose_MeasurementRecord(i, rec);
            if (rec->meas.SequenceNumber>= seq_num)
            {
                num_records++;
            }
        }
    }
    else if ((p_recordAccessPoint->oPerator == GLUCOSE_FIRST_RECORD) ||
             (p_recordAccessPoint->oPerator == GLUCOSE_LAST_RECORD))
    {
        if (total_records > 0)
        {
            num_records = 1;
        }
    }
	
    m_pending_racp_response = (BLE_Glucose_RACP XDATA *)APP_HEAP_Alloc(sizeof(BLE_Glucose_RACP));
	if(m_pending_racp_response!=NULL)
    {
    m_pending_racp_response->opCode      = GLUCOSE_NUMBER_OF_STORED_RECORDS_RESPONSE;
    m_pending_racp_response->oPerator    = GLUCOSE_NULL;
    m_pending_racp_response->operand     = num_records;     

    BLE_Send_RACP_Response(m_pending_racp_response);
	}
}
static u16 BLE_RACP_Report_AllRecords(void)large
{
      XDATA u16 total_records=0;
	 total_records = BLE_Get_NumberOfRecords_Glucose_MeasurementRecord();
	if (m_racp_proc_record_ndx >= total_records)
    {
        BLE_GlucoseRACPstate_set(STATE_NO_COMM);
    }
    else
    {
        XDATA u16      result;
        BLE_GLUCOSE_RecordsDatabase XDATA *rec;
        rec = (BLE_GLUCOSE_RecordsDatabase XDATA *)APP_HEAP_Alloc(sizeof(BLE_GLUCOSE_RecordsDatabase));
        result = BLE_Get_Glucose_MeasurementRecord(m_racp_proc_record_ndx, rec);
    	//APP_UART_Put(sizeof(BLE_GLUCOSE_RecordsDatabase), (u8 XDATA*)rec);
    	if (result!= MBIOT_RES_SUCCESS)
    		{
          return MBIOT_RES_FAIL;
		}
        result = BLE_Send_Glucose_Measurement(p_app_conn->handle, (BLE_GLUCOSE_Measurement XDATA *)rec);
		     //APP_UART_Put(sizeof(BLE_GLUCOSE_Measurement),(u8 XDATA *) rec);
        if (result != MBIOT_RES_SUCCESS)
        {
            return MBIOT_RES_FAIL;
        }
	 }
    return MBIOT_RES_SUCCESS;


}
static void BLE_Report_Stored_Records(BLE_Glucose_RACP XDATA *p_recordAccessPoint)large
{
	XDATA u16 result;
    XDATA u16 seq_num = p_recordAccessPoint->operand;
	XDATA u8 uartLog[]="RRRRR";
	m_racp_proc_record_ndx       = 0;
    m_racp_proc_operator         = p_recordAccessPoint->oPerator;
    m_racp_proc_records_reported = 0;
    m_racp_proc_seq_num          = seq_num;
	APP_UART_Put(sizeof(uartLog),uartLog);
	BLE_GlucoseRACPstate_set(STATE_RACP_PROC_ACTIVE);
	while (m_gls_RACPstate == STATE_RACP_PROC_ACTIVE)
    {
	switch(m_racp_proc_operator)
		{
		case GLUCOSE_ALL_RECORDS:
			result=BLE_RACP_Report_AllRecords();
			 break;
		case GLUCOSE_GREATER_THAN_OR_EQUAL_TO:
			result = BLE_RACP_Report_GreaterOrEqual_Record();
			 break;
		case GLUCOSE_FIRST_RECORD:
		case GLUCOSE_LAST_RECORD:
			result = BLE_RACP_Report_First_Last_Record();
			break;
		default:
			BLE_GlucoseRACPstate_set(STATE_NO_COMM);
			app_send_event(EVT_UNKNOWN_COMMAND);
			return;
		}
	switch(result)
		{
		case MBIOT_RES_SUCCESS:
			if(m_gls_RACPstate == STATE_RACP_PROC_ACTIVE){				
			m_racp_proc_record_ndx++;
				}
			else{
				XDATA BLE_Glucose_RACP XDATA *m_recordAccessPoint;
				u8 XDATA uartLog[] = "**********";
				APP_UART_Put(sizeof(uartLog), uartLog); 
				m_recordAccessPoint = (BLE_Glucose_RACP XDATA *)APP_HEAP_Alloc(sizeof(BLE_Glucose_RACP));
				if(m_recordAccessPoint!=NULL){
				m_recordAccessPoint->opCode = GLUCOSE_REQUEST_OP_CODE;
				m_recordAccessPoint->oPerator = GLUCOSE_NULL;
				m_recordAccessPoint->operand = GLUCOSE_SUCCESS_NORMAL_RESPONSE;
				BLE_Send_RACP_Response(m_recordAccessPoint);
			 	}
				}
				
			break;
		 case MBIOT_RES_FAIL:
		 	BLE_GlucoseRACPstate_set(STATE_NO_COMM);
			break;
		 case MBIOT_RES_INVALID_PARA:
		 	{
			u8 XDATA uartLog[] = "!!!!!!!!";
			APP_UART_Put(sizeof(uartLog), uartLog); 
		 	BLE_GlucoseRACPstate_set(STATE_NO_COMM);
		 	}
		 	break;
		 default:
			BLE_GlucoseRACPstate_set(STATE_NO_COMM);
			return;


	    }
	}
}
/**@brief Function for handle glucose event.
 *
 * @param[in] eventType	  glucose service events	
 */
	static void glucose_event_handler(BLE_GLUCOSE_Event XDATA *p_glucoseEvent)large
	{		u8 XDATA *p_buffer;
			u16 XDATA handle=p_glucoseEvent->handle;
			u8 XDATA ans_log[] = "MCCD";
			p_buffer=APP_HEAP_Alloc(p_glucoseEvent->receivedDataLength);
			xmemcpy(p_buffer,p_glucoseEvent->write_value,p_glucoseEvent->receivedDataLength);
			if((handle==glucoseMeasurementCCDHandle)|| (handle == glucoseMeasurementContextCCDHandle))
			{
				 
				if(*((u16 XDATA *)p_buffer)==GLUCOSE_MEASUREMENT_NOTIFY_DISABLED)
				 {
				 NotificatioEnable = FALSE;
					app_send_event(EVT_CHANNEL_DISABLED);
				 }
				 else if(*((u16 XDATA *)p_buffer)==GLUCOSE_MEASUREMENT_NOTIFY_ENABLED)
				 {
				 NotificatioEnable = TRUE;
				    APP_UART_Put(sizeof(ans_log), ans_log);
					app_send_event(EVT_CHANNEL_ENABLED);
				 }
			}
			else if(handle==glucoseRecordAccessControlPointCCDHandle)
			{
			u8 XDATA ans_log[] = "RACP";
			APP_UART_Put(sizeof(ans_log), ans_log);
			APP_UART_Put(p_glucoseEvent->receivedDataLength, p_buffer);
			if(*((u16 XDATA *)p_buffer)==GLUCOSE_MEASUREMENT_INDICATE_DISABLED)
			 {
				 app_send_event(EVT_CHANNEL_DISABLED);
			 }
			 else if(*((u16 XDATA *)p_buffer)==GLUCOSE_MEASUREMENT_INDICATE_ENABLED)
			 {
				app_send_event(EVT_CHANNEL_ENABLED);
			 }
			 }
			else if(handle==glucoseRecordAccessControlPointHandle)
			{
			u8 XDATA uartlog[] = "OPC01";
			u8 XDATA uartLog[] = "OPC04";
			BLE_Glucose_RACP XDATA *p_recordAccessPoint;
			m_racp_handle = handle;
			if(*((u16 XDATA *)p_buffer)==GLUCOSE_MEASUREMENT_INDICATE_DISABLED)
			 {
				 u8 XDATA ans_log[] = "DDDD";
				APP_UART_Put(sizeof(ans_log), ans_log);
				app_send_event(EVT_CHANNEL_DISABLED);
			 }
			 else if(*((u16 XDATA *)p_buffer)==GLUCOSE_MEASUREMENT_INDICATE_ENABLED)
			 {
				u8 XDATA ans_log[] = "EEEE";
				APP_UART_Put(sizeof(ans_log), ans_log);
				app_send_event(EVT_CHANNEL_ENABLED);
			 }
			 p_recordAccessPoint = (BLE_Glucose_RACP XDATA *)APP_HEAP_Alloc(sizeof(BLE_Glucose_RACP));
			if(p_recordAccessPoint != NULL) { 
			 p_recordAccessPoint->opCode = *p_buffer;
			 APP_UART_Put(sizeof(p_recordAccessPoint->opCode),&p_recordAccessPoint->opCode);
			 p_recordAccessPoint->oPerator =*(p_buffer+1);
			 APP_UART_Put(sizeof(u16), (u8 XDATA *)p_recordAccessPoint->oPerator);
			 p_recordAccessPoint->operand = *(p_buffer+2);
			 }
			switch(p_recordAccessPoint->opCode)
			{
			
			case GLUCOSE_REPORT_STORED_RECORDS:
				    APP_UART_Put(sizeof(uartlog),uartlog);
					if(NotificatioEnable==TRUE){
				  	BLE_Report_Stored_Records(p_recordAccessPoint);
					}
					BLE_Send_Glucose_Write_Response(p_app_conn->handle, BLE_GATT_TYPE_WRITE_RESP);
					break;
				 
			case GLUCOSE_REPORT_NUMBER_OF_STORED_RECORDS:
				    APP_UART_Put(sizeof(uartLog),uartLog);
				 	BLE_Report_Number_Stored_Records(p_recordAccessPoint);
					BLE_Send_Glucose_Write_Response(p_app_conn->handle, BLE_GATT_TYPE_WRITE_RESP);
			   	break;	 
        			
			case GLUCOSE_DELETE_STORED_RECORDS:
				  app_send_event(EVT_UNKNOWN_COMMAND);
				  BLE_Send_Glucose_Error_Response(p_app_conn->handle, BLE_GLUCOSE_COMMAND_ERROR);
				 break;

			case GLUCOSE_ABORT_OPERATION:
				BLE_Send_Glucose_Error_Response(p_app_conn->handle, BLE_GLUCOSE_COMMAND_ERROR);
				 app_send_event(EVT_UNKNOWN_COMMAND);
				 break;
			case GLUCOSE_NUMBER_OF_STORED_RECORDS_RESPONSE:
				BLE_Send_Glucose_Write_Response(p_app_conn->handle, BLE_GATT_TYPE_WRITE_RESP);
				//UINT16 containing number of records //Mandatory if the Report Number of Stored Records Op Code is supported, otherwise excluded
				break;
			case GLUCOSE_RESPONSE_CODE:
				BLE_Send_Glucose_Write_Response(p_app_conn->handle, BLE_GATT_TYPE_WRITE_RESP);
				 //Request Op Code, Response Code Value
				 break;
			default:
			app_send_event(EVT_UNKNOWN_COMMAND);
			BLE_Send_Glucose_Error_Response(p_app_conn->handle, BLE_GLUCOSE_COMMAND_ERROR);
		break;
			 }
				//else BLE_Send_Glucose_Error_Response(p_app_conn->handle, BLE_GLUCOSE_COMMAND_ERROR);
	    
		
		}
}


/*******************************************************************/
/***                     Callback Functions                      ***/
/*******************************************************************/

/**@brief Function for handle GAP events.
 *
 * @param[in] p_event	BLE GAP events	
 */



PUBLIC void gap_event_handler(BLE_GAP_Event XDATA * p_event) large reentrant
{
	switch(p_event->eventID)
	{
		case BLE_GAP_EVT_CONNECTED:
		{
			//BLE link is established
			app_send_event(EVT_CONNECTED);
						
			p_app_conn=(APP_CONN_STRUCT XDATA *)APP_HEAP_Alloc(sizeof(APP_CONN_STRUCT));
			p_app_conn->handle=p_event->eventField.evtConnect.connHandle;
		}
		break;
		
		case BLE_GAP_EVT_DISCONNECTED:
		{
			//BLE link is terminated
			app_send_event(EVT_DISCONNECTED);			
			APP_HEAP_Free((u8 XDATA*)p_app_conn);
			APP_TIMER_FastTimer_Start(APP_TIMER_ID_0,4);
		}
		break;
		
		case BLE_GAP_EVT_CONN_PARA_UPDATE:
		{
			//The connection parameters of BLE link are updated
		}
		break;
		
		case BLE_GAP_EVT_ENCRYPT_STATUS:
		{
			//Received advertising report
		}
		break;
		
		case BLE_GAP_EVT_ADVERTISING_REPORT:
		{
			//The encryption status of BLE link
		}
		break;
		
		default:
		break;
	}
}


/**@brief Function for handle SMP events.
 *
 * @param[in] p_event	BLE SMP events	
 */
PUBLIC void smp_event_handler(BLE_SMP_Event XDATA * p_event) large reentrant
{
	switch(p_event->eventID)
	{
		case BLE_SMP_EVT_PAIRING_COMPLETE:
		{
			//Pairing process is done
		}
		break;
		
		case BLE_SMP_EVT_SECURITY_REQUEST:
		{
			//Pairing request is received from remote device
		}
		break;
		
		case BLE_SMP_EVT_NUMERIC_COMPARISON_CONFIRM_REQUEST:
		{
			//Compare value is needed to display for pairing process
		}
		break;
		
		case BLE_SMP_EVT_INPUT_PASSKEY:
		{
			//Passkey is requested to input for pairing process
		}
		break;
		
		case BLE_SMP_EVT_DISPLAY_PASSKEY_REQUEST:
		{
			//Passkey is needed to display for pairing process
		}
		break;

		default:
		break;
	}
	
}


/**@brief Function for handle GATT events.
 *
 * @param[in] p_event	BLE GATT events	
 */
PUBLIC void gatt_event_handler(BLE_GATT_Event XDATA * p_event) large reentrant
{
	switch(p_event->eventID)
	{
		case BLE_GATTC_EVT_ERROR_RESP:
		{
			// Receive error response
		}
		case BLE_GATTS_EVT_READ:
		{
			// Send read response by connection handle
		}
		break;
		
		case BLE_GATTS_EVT_WRITE:
		{
			// Write event
		}
		break;
		
		case BLE_GATTS_EVT_HV_CONFIRM:
		{
			// Indication ACK
		}
		break;
		
		case BLE_ATT_EVT_TIMEOUT:
		{
			// Timeout handling
		}
		break;
		
		default:
		break;
	}
	BLE_Glucose_Catch_Event(p_event);	
}


/**@brief Function for handle L2CAP events.
 *
 * @param[in] p_event	BLE L2CAP events	
 */
PUBLIC void l2cap_event_handler(BLE_L2CAP_Event XDATA * p_event) large reentrant
{
	switch(p_event->eventID)
	{
		case BLE_L2CAP_EVT_CONN_PARA_UPDATE_RESULT:
		{
			//The result of L2cap connection parameters update process
		}
		break;

		default:
		break;
	}
}


/**@brief Function for handle COMMON events.
 *
 * @param[in] p_event	BLE COMMON events	
 */
PUBLIC void common_event_handler(APP_Common_Event XDATA * p_event) large reentrant
{
	switch(p_event->eventID)
	{
		case APP_TIMER_EVT:
		{
			switch(p_event->eventField.evtTimer.timerID)
			{
				case APP_TIMER_ID_0:
				{
					UTILITY_Shutdown(GPIO_P17);
				}
				break;
					
				case APP_TIMER_ID_1:
				break;
			}
		}
		break;
	}
}


/**@brief Function for handle PERIPHERAL events.
 *
 * @param[in] p_event	BLE PERIPHERAL events	
 */
PUBLIC void peripheral_event_handler(APP_Peripheral_Event XDATA * p_event) large reentrant
{
	switch (p_event->eventID)
	{
		case APP_UART_EVT_RECEIVE_DATA:
		{
			XDATA u8 tempBCH;
			XDATA u16 data_len,index=0;
			XDATA u8 XDATA *p_buffer;
				
			APP_UART_GetReceivedDataLength((u8 XDATA *)&data_len);
			p_buffer=APP_HEAP_Alloc(data_len);
            	
			if(p_buffer!=NULL)
			{
				APP_UART_GetReceivedData(data_len,p_buffer);
			}
			
			if(uart_cmd_wait)
			{
				return;
			}
		
			while(data_len > 0)
			{
				tempBCH = p_buffer[index++];
				uart_cmd_buffer[uart_cmd_index]=tempBCH;

				if((uart_cmd_buffer[uart_cmd_index-1]==(UART_END_PATTERN>>8))&&(uart_cmd_buffer[uart_cmd_index]==(u8)(UART_END_PATTERN)))
				{
					uart_cmd_wait=UART_STATE_WAIT;
					app_uart_parsing(uart_cmd_buffer);
					uart_cmd_index=0;
					uart_cmd_wait=UART_STATE_IDLE;
					break;
				}
				uart_cmd_index++;
				data_len--;
			}

			APP_HEAP_Free(p_buffer);
		}
		break;
	}
}

/**@brief Function for MBIoT main init entry. */
PUBLIC void Application_Init() large reentrant
{
     XDATA u8 i;
	 XDATA u8 newName[]="Glucose";
    /* BLE_GAP_Addr XDATA maddr;
	
	maddr.addr_type = 0x00;
	maddr.addr[0] = 0x00;
	maddr.addr[1] = 0x11;
	maddr.addr[2] = 0xca;
	maddr.addr[3] = 0x6c;
	maddr.addr[4] = 0x03;
	maddr.addr[5] = 0xef;*/
	//BLE stack call back function initialize
	BLE_GAP_Init(gap_event_handler);
	BLE_L2CAP_Init(l2cap_event_handler);
	BLE_GATT_Init(gatt_event_handler);
	BLE_SMP_Init(smp_event_handler);
	
	//Set Mac Address
	//BLE_GAP_Set_Addr(&maddr);
	
	//MBIoT call back function initialize
	APP_PERIPHERAL_Init(peripheral_event_handler);
	APP_COMMON_Init(common_event_handler);
	APP_HEAP_Init();
	//Application initialize
	BLE_GLUCOSE_Init(glucose_event_handler);
	BLE_GATTS_StartCreatingServiceTable();
  BLE_GLUCOSE_Init_Service();
	BLE_GATTS_EndCreatingServiceTable();
	BLE_Glucose_Set_Device_Fault_Supported(1);
	BLE_Glucose_Set_Glucose_MultiBond(1);
	 m_next_seq_num = 0;
	 for(i=0;i<3;i++){
	BLE_Read_Glucose_measurement();
	 	}
	uart_cmd_index=0;
	uart_cmd_wait=0;
	{
		XDATA APP_UART_ConfParas uartParas;
		
		uartParas.baudrate=APP_UART_BAUD_RATE_115200;
    	uartParas.flowContol=APP_UART_FLOW_CONTROL_DISABLE;
    	uartParas.rxNotifyLength=MAX_UART_CMD_BUFFER;
    	uartParas.rxBufferSize=APP_RX_BUFFER_LEN;
    	uartParas.rxBuffer=AppRxBuffer;
    	uartParas.txBufferSize=APP_TX_BUFFER_LEN;
    	uartParas.txBuffer=AppTxBuffer;
    	uartParas.txIndicateInterval=0;
    	uartParas.rxWakeupPinNum=GPIO_P35;
    			
		APP_UART_Init(&uartParas);
	}
	
	//MBIoT initalize
	UTILITY_LowPowerModeControl(LOW_POWER_MODE_DISALLOW);
	BLE_GAP_Set_DeviceName(sizeof(newName),newName);
	app_init();	
}
