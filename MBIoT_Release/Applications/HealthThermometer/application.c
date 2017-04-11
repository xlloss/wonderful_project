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

/**@brief Project Setting Include File. */
#include "mbiot.h"

/**@brief Application Include File. */
#include "application.h"

/**@brief BLE Service Include File. */
#include "ble_health_thermometer_service.h"

/**@brief Definition of Application Version. */
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
static XDATA APP_CONN_STRUCT XDATA *p_app_conn;				/**< Pointer to Application Connection Parameters Structure. */
static XDATA bool IndicationEnable;
//extern  MeasurementInterval;                      /**< Temperature Measurement Interval. */
/**@brief Definition of events. */
static MCODE APP_EVENT_STRUCT app_event[]=
{
	{{EVT_LEN_ADVERTISING},			{'A','d','v','e','r','t','i','s','i','n','g',UART_END_PATTERN>>8,UART_END_PATTERN}},//41 64 76 65 72 74 69 73 69 6e 67 
	{{EVT_LEN_UNSPECIFIC_ERROR},	{'U','n','s','p','e','c','i','f','i','c',' ','E','r','r','o','r',UART_END_PATTERN>>8,UART_END_PATTERN}},
	{{EVT_LEN_UNKNOWN_COMMAND},		{'U','n','k','n','o','w',' ','C','o','m','m','a','n','d',UART_END_PATTERN>>8,UART_END_PATTERN}},
	{{EVT_LEN_CONNECTED},			{'C','o','n','n','e','c','t','e','d',UART_END_PATTERN>>8,UART_END_PATTERN}},
	{{EVT_LEN_DISCONNECTED},		{'D','i','s','c','o','n','n','e','c','t','e','d',UART_END_PATTERN>>8,UART_END_PATTERN}},
	{{EVT_LEN_CHANNEL_ENABLED},		{'D','a','t','a',' ','C','h','a','n','n','e','l',' ','E','n','a','b','l','e','d',UART_END_PATTERN>>8,UART_END_PATTERN}},
	{{EVT_LEN_CHANNEL_DISABLED},	{'D','a','t','a',' ','C','h','a','n','n','e','l',' ','D','i','s','a','b','l','e','d',UART_END_PATTERN>>8,UART_END_PATTERN}},
};

/**@brief Definition of commands. */
static MCODE APP_CMD_STRUCT app_cmd[]=
{
	{{CMD_LEN_TEMP_MEASUREMENT},			{"TempM"}},
	{{CMD_LEN_INTERMEDIATE_MEASUREMENT},			{"InterTemp"}},
	{{CMD_LEN_TEMPERATURE_MEAUSREMENT_INTERVAL},  {"TempInterval"}},
	{{CMD_STOP_TEMPERATURE_MEAUSREMENT_INTERVAL_INDICATION},  {"Stop"}},
	{{CMD_LEN_DISCONNECT},		{"Disconnect"}},
};


/**@brief Function for starting advertising. */
static u16 app_start_advertising()large reentrant
{
	BLE_GAP_AdvertisingParas XDATA advpara;
	BLE_GAP_SetAdvertisingDataParas	XDATA advData;
	XDATA u16 result;
	XDATA u8 temp[]={0x02,0x01,0x05,0x13,0x09,'H','e','a','l','t','h',' ','T','h','e','r','m','o','m','e','t','e','r',0x03,0x03,0x09,0x18};
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
		case CMD_SEND_TEMP_MEASUREMENT:
		{
			XDATA BLE_HTMS_Measurement XDATA *p_temperatureMeasurement;
			u8 XDATA temperatureMeasurementFlag, payloadIndex=CMD_LEN_TEMP_MEASUREMENT;
			p_temperatureMeasurement=(BLE_HTMS_Measurement XDATA *)APP_HEAP_Alloc(sizeof(BLE_HTMS_Measurement));
			if(p_temperatureMeasurement!=NULL)
			{
			  xmemset((u8 XDATA *)p_temperatureMeasurement,0,sizeof(BLE_HTMS_Measurement));
				temperatureMeasurementFlag = *(p_payload+payloadIndex);
				payloadIndex++;
								
			    if(temperatureMeasurementFlag&0x01){
				p_temperatureMeasurement->IsTemperatureUnitAvailable= 1;
				}
			    else{
			    p_temperatureMeasurement->IsTemperatureUnitAvailable= 0;
				}
				xmemcpy((u8 XDATA *)&p_temperatureMeasurement->temperatureMeasurementValue,(p_payload+payloadIndex),4);
			  payloadIndex+=4;
				if(temperatureMeasurementFlag&0x02){
				p_temperatureMeasurement->IsTimestampAvailable=1;
				xmemcpy((u8 XDATA *)&p_temperatureMeasurement->timeStamp.year,p_payload+payloadIndex,2);
				payloadIndex+=2;
				p_temperatureMeasurement->timeStamp.month= *(p_payload+payloadIndex);
				payloadIndex+=1;
				p_temperatureMeasurement->timeStamp.day= *(p_payload+payloadIndex);
				payloadIndex+=1;
				p_temperatureMeasurement->timeStamp.hours= *(p_payload+payloadIndex);
				payloadIndex+=1;
				p_temperatureMeasurement->timeStamp.minutes= *(p_payload+payloadIndex);
				payloadIndex+=1;
				p_temperatureMeasurement->timeStamp.seconds= *(p_payload+payloadIndex);
				payloadIndex+=1;
				}
			if(temperatureMeasurementFlag&0x04){
				p_temperatureMeasurement->IsTemperatureTypeAvailable= 1;
				p_temperatureMeasurement->temperatureType= *(p_payload+payloadIndex);
				payloadIndex+=1;
			  	}
						
				BLE_HTMS_Send_Temperature_Measurement(p_app_conn->handle,p_temperatureMeasurement);
				}
		}
		break;
		case CMD_SEND_INTERMEDIATE_MEASUREMENT:
		{
			XDATA BLE_HTMS_Measurement XDATA *p_IntermediatetemperatureMeasurement;
			u8 XDATA temperatureMeasurementFlag, payloadIndex=CMD_LEN_INTERMEDIATE_MEASUREMENT;
			p_IntermediatetemperatureMeasurement=(BLE_HTMS_Measurement XDATA *)APP_HEAP_Alloc(sizeof(BLE_HTMS_Measurement));
			if(p_IntermediatetemperatureMeasurement!=NULL)
			{
			    xmemset((u8 XDATA *)p_IntermediatetemperatureMeasurement,0,sizeof(BLE_HTMS_Measurement));
				temperatureMeasurementFlag = *(p_payload+payloadIndex);
				payloadIndex++;
								
			if(temperatureMeasurementFlag&0x01){
				p_IntermediatetemperatureMeasurement->IsTemperatureUnitAvailable= 1;
				}
			else{
				p_IntermediatetemperatureMeasurement->IsTemperatureUnitAvailable= 0;

			}
				xmemcpy((u8 XDATA *)&p_IntermediatetemperatureMeasurement->temperatureMeasurementValue,(p_payload+payloadIndex),4);
			    payloadIndex+=4;
				
			if(temperatureMeasurementFlag&0x02){
				p_IntermediatetemperatureMeasurement->IsTimestampAvailable=1;
			    xmemcpy((u8 XDATA *)&p_IntermediatetemperatureMeasurement->timeStamp.year,p_payload+payloadIndex,2);
				payloadIndex+=2;
				p_IntermediatetemperatureMeasurement->timeStamp.month= *(p_payload+payloadIndex);
				payloadIndex+=1;
				p_IntermediatetemperatureMeasurement->timeStamp.day= *(p_payload+payloadIndex);
				payloadIndex+=1;
				p_IntermediatetemperatureMeasurement->timeStamp.hours= *(p_payload+payloadIndex);
				payloadIndex+=1;
				p_IntermediatetemperatureMeasurement->timeStamp.minutes= *(p_payload+payloadIndex);
				payloadIndex+=1;
				p_IntermediatetemperatureMeasurement->timeStamp.seconds= *(p_payload+payloadIndex);
				payloadIndex+=1;
				}
			if(temperatureMeasurementFlag&0x04){
				p_IntermediatetemperatureMeasurement->IsTemperatureTypeAvailable= 1;
				p_IntermediatetemperatureMeasurement->temperatureType= *(p_payload+payloadIndex);
				payloadIndex+=1;
			  	}
						
				BLE_HTMS_Send_Intermediate_Temperature(p_app_conn->handle,p_IntermediatetemperatureMeasurement);
			}
		}
		break;
		case CMD_SEND_TEMPERATURE_MEAUSREMENT_INTERVAL:
		{
			u16 XDATA interval;
			u8 XDATA payloadIndex=CMD_LEN_TEMPERATURE_MEAUSREMENT_INTERVAL;
			xmemcpy((u8 XDATA *)&interval,(p_payload+payloadIndex),2);
			MeasurementInterval = interval;
			BLE_HTMS_Send_Temperature_Interval(p_app_conn->handle,MeasurementInterval);
			BLE_HTMS_Set_Temperature_Interval(MeasurementInterval);
		}			
			break;
		case CMD_STOP_TEMPERATURE_MEAUSREMENT_INTERVAL_INDICATION:
		{
			u8 XDATA ans_log[] = "STOP";
			APP_UART_Put(sizeof(ans_log), ans_log);
			APP_TIMER_FastTimer_Stop(APP_TIMER_ID_3);
			
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

/**@brief Function for handling Weight Scale Service events.
 *
 * @param[in] eventType	  Weight Scale Service Events	
 */
static void htms_event_handler(BLE_HTMS_Event XDATA *p_htmsEvent)large reentrant
{
	XDATA u16 handle=p_htmsEvent->handle;
	u8 XDATA ans_log[] = "^^^^^^^^^^^^^^^^^^^";
	if(handle==temperatureMeasurementHandleCCCDHandle)
	{
		XDATA u16 eventType;
		eventType = p_htmsEvent->write_value;
		
		if(eventType==HEALTH_THERMOMETER_INDICATE_DISABLED)
		{
		 	IndicationEnable = FALSE;
			APP_TIMER_FastTimer_Stop(APP_TIMER_ID_3);
			app_send_event(EVT_CHANNEL_DISABLED);
		}
		else if(eventType==HEALTH_THERMOMETER_INDICATE_ENABLED)
		{
		    IndicationEnable = TRUE;
			app_send_event(EVT_CHANNEL_ENABLED);
			if((MeasurementInterval!=0)&&(MeasurementInterval<=HEALTH_THERMOMETER_MAX_MEASUREMENT_INTERVAL)){
			//APP_TIMER_SlowTimer_Start(APP_TIMER_ID_3,((MeasurementInterval*1000)/640));	
			APP_TIMER_FastTimer_Start(APP_TIMER_ID_3,((MeasurementInterval*1000)*(0.625)));
			}
		}
	}
	else if(handle==intermediateTemperatureCCCDHandle)
	{
		XDATA u16 eventType;
		eventType=p_htmsEvent->write_value;
		
		if(eventType==HEALTH_THERMOMETER_NOTIFY_DISABLED)
		{
			app_send_event(EVT_CHANNEL_DISABLED);
		}
		else if(eventType==HEALTH_THERMOMETER_NOTIFY_ENABLED)
		{
			app_send_event(EVT_CHANNEL_ENABLED);
		 }
	}
	else if(handle==measurementIntervalCCDHandle)
	{
		XDATA u16 eventType;
		eventType=p_htmsEvent->write_value;
		if(eventType==HEALTH_THERMOMETER_INDICATE_DISABLED)
		{
		    APP_TIMER_FastTimer_Stop(APP_TIMER_ID_3);
			app_send_event(EVT_CHANNEL_DISABLED);
		}
		else if(eventType==HEALTH_THERMOMETER_INDICATE_ENABLED)
		{
			app_send_event(EVT_CHANNEL_ENABLED);
		}
	}
	else if(handle==measurementIntervalHandle)
	{
		APP_UART_Put(sizeof(ans_log), ans_log);
		MeasurementInterval=p_htmsEvent->write_value;
		if((MeasurementInterval!=0)&&(MeasurementInterval<=HEALTH_THERMOMETER_MAX_MEASUREMENT_INTERVAL)){
		APP_TIMER_FastTimer_Start(APP_TIMER_ID_3,((MeasurementInterval*1000)*(0.625)));
		BLE_HTMS_Send_Temperature_Interval(p_app_conn->handle,MeasurementInterval);
		BLE_HTMS_Set_Temperature_Interval(MeasurementInterval);
		}
		else{
		BLE_HTMS_Send_Temperature_Interval(p_app_conn->handle,OUT_OF_RANGE);
		app_send_event(EVT_UNKNOWN_COMMAND);

}		
	}		
	/*else if(handle==measurementIntervalValidRangeHandle)
	{
		XDATA u16 measurementIntervalRange;
		APP_UART_Put(sizeof(ans_log), ans_log);
		measurementIntervalRange=p_htmsEvent->write_value;
		if((measurementIntervalRange<0x0258)&&(measurementIntervalRange>0x1C20))
		{
     BLE_HTMS_Send_Temperature_Interval(p_app_conn->handle,OUT_OF_RANGE);
    }
	}*/
	
}



/**@brief Function for handling BLE GAP Stack events.
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


/**@brief Function for handling BLE SMP Stack events.
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


/**@brief Function for handling BLE GATT Stack events.
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
	BLE_HTMS_Catch_Event(p_event);	
}


/**@brief Function for handle BLE L2CAP Stack events.
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


/**@brief Function for handling COMMON events.
 *
 * @param[in] p_event	COMMON events	
 */
PUBLIC void common_event_handler(APP_Common_Event XDATA * p_event) large reentrant
{
	XDATA BLE_HTMS_Measurement XDATA *p_temperatureMeasurement;
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
				case APP_TIMER_ID_3:
					p_temperatureMeasurement=(BLE_HTMS_Measurement XDATA *)APP_HEAP_Alloc(sizeof(BLE_HTMS_Measurement));
					if(p_temperatureMeasurement!=NULL){
						p_temperatureMeasurement->IsTemperatureTypeAvailable = 1;
						p_temperatureMeasurement->IsTemperatureUnitAvailable= 1;
						p_temperatureMeasurement->IsTimestampAvailable = 1;
						p_temperatureMeasurement->temperatureMeasurementValue =10000000;
						p_temperatureMeasurement->temperatureType=BLE_HTMS_TEMPERATURE_TYPE_FINGER;
						p_temperatureMeasurement->timeStamp.year=0xE007; //0xDF070C1C093831;
						p_temperatureMeasurement->timeStamp.month= 0x03;
						p_temperatureMeasurement->timeStamp.day = 0x08;
						p_temperatureMeasurement->timeStamp.hours = 0x0C;
						p_temperatureMeasurement->timeStamp.minutes = 0x05;
						p_temperatureMeasurement->timeStamp.seconds = 0x32;
						BLE_HTMS_Send_Temperature_Measurement(p_app_conn->handle,p_temperatureMeasurement);
						if((IndicationEnable==TRUE)&&(MeasurementInterval!=0)&&(MeasurementInterval<=HEALTH_THERMOMETER_MAX_MEASUREMENT_INTERVAL)){
							//APP_TIMER_SlowTimer_Start(APP_TIMER_ID_3,((MeasurementInterval*1000)/640));	
							APP_TIMER_FastTimer_Start(APP_TIMER_ID_3,((MeasurementInterval*1000)*(0.625)));
							}
						
					}
					break;
			}
		}
		break;
	}
}


/**@brief Function for handling Peripheral events.
 *
 * @param[in] p_event	BLE Peripheral events	
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

/**@brief Function for Application init entry. */
PUBLIC void Application_Init() large reentrant
{
	XDATA u8 newName[]="Health Thermometer";
	BLE_GAP_Addr XDATA maddr;
	APP_UART_Put(sizeof(newName), newName);
	maddr.addr_type = 0x00;
	maddr.addr[0] = 0x16;
	maddr.addr[1] = 0x21;
	maddr.addr[2] = 0x2c;
	maddr.addr[3] = 0x37;
	maddr.addr[4] = 0x42;
	maddr.addr[5] = 0x4d;

   //Set Mac Address
   BLE_GAP_Set_Addr(&maddr);

	//BLE stack call back function initialize
	BLE_GAP_Init(gap_event_handler);
	BLE_L2CAP_Init(l2cap_event_handler);
	BLE_GATT_Init(gatt_event_handler);
	BLE_SMP_Init(smp_event_handler);
	//Set Mac Address
	BLE_GAP_Set_Addr(&maddr);
	
	//MBIoT call back function initialize
	APP_PERIPHERAL_Init(peripheral_event_handler);
	APP_COMMON_Init(common_event_handler);
	APP_HEAP_Init();
	
	//Application initialize
	BLE_HTMS_Init(htms_event_handler);
	
	//GATT Service Table initialize
	BLE_GATTS_StartCreatingServiceTable();
	BLE_HTMS_Init_Service();
	BLE_GATTS_EndCreatingServiceTable();
	BLE_HTMS_Set_TemperatureType(BLE_HTMS_TEMPERATURE_TYPE_BODY);
	BLE_HTMS_Set_Temperature_Interval(5);
		
	//UART initialize
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
	
	//Application initial
	app_init();	
}
