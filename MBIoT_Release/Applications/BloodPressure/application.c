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
#include "ble_blood_pressure_service.h"

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

/**@brief Definition of commands. */
static MCODE APP_CMD_STRUCT app_cmd[]=
{
	{{CMD_LEN_BLOOD_PRESSURE},			{"Blood Pressure"}},
	{{CMD_LEN_INTER_CUFF_PRESSURE},	{"Inter cuff"}},
	{{CMD_LEN_DISCONNECT},		      {"Disconnect"}},
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
	u8 XDATA temp[]={0x02,0x01,0x05,0x0F,0x09,'B','l','o','o','d',' ','P','r','e','s','s','u','r','e',0x03,0x03,0x10,0x18};

	advpara.type=BLE_GAP_ADV_TYPE_ADV_IND;
	advpara.interval=0x0020;		
	advData.adLen=sizeof(temp);
	advData.adScanResLen=0;
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
	u8 XDATA event[MAX_UART_EVT_BUFFER];			
	comemcpy(event, app_event[index].content,app_event[index].length);
	APP_UART_Put(app_event[index].length, event);				
}

/**@brief Function for initializing application. */
static void app_init()large reentrant
{
	u16 XDATA result;
	
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
	u8 XDATA i;
	u8 XDATA flag;
	u8 XDATA index;

	for(i=0;i<CMD_END;i++)
	{
		if(!comemcmp(p_payload, app_cmd[i].content,app_cmd[i].length))
		{
			break;
		}
	}

	switch(i)
	{
		case CMD_SEND_BLOOD_PRESSURE:
		{
			XDATA BLE_BPS_Measurement XDATA *p_bpsMeasurement;
			index = CMD_LEN_BLOOD_PRESSURE;

			p_bpsMeasurement=(BLE_BPS_Measurement XDATA *)APP_HEAP_Alloc(sizeof(BLE_BPS_Measurement));

			if(p_bpsMeasurement!=NULL)
			{
				xmemset((u8 XDATA *)p_bpsMeasurement,0,sizeof(BLE_BPS_Measurement));
				/*Process the first byte for the flags and fill the structure accordingly*/
				flag = *(p_payload+CMD_LEN_BLOOD_PRESSURE);
				index += 1;
				if(flag & BP_MEASUREMENT_FLAG_UNITS)
				{
					p_bpsMeasurement->blood_pressure_units_in_kpa = BLOOD_PRESSURE_UNIT_KPA;
				}
				else
				{
					p_bpsMeasurement->blood_pressure_units_in_kpa = BLOOD_PRESSURE_UNIT_MMHG;
				}
					
				//Systolic value
				xmemcpy((u8 XDATA *)&p_bpsMeasurement->blood_pressure_systolic.exponent,p_payload+index,1);
				index += 1;
				xmemcpy((u8 XDATA *)&p_bpsMeasurement->blood_pressure_systolic.mantissa,(p_payload+index),2);
				index += 2;
				
				//Diastolic Value
				xmemcpy((u8 XDATA *)&p_bpsMeasurement->blood_pressure_diastolic.exponent,p_payload+index,1);
				index += 1;
				xmemcpy((u8 XDATA *)&p_bpsMeasurement->blood_pressure_diastolic.mantissa,(p_payload+index),2);
				index += 2;
				
				//Mean Arterial Pressure Value
				xmemcpy((u8 XDATA *)&p_bpsMeasurement->mean_arterial_pressure.exponent,p_payload+index,1);
				index += 1;
				xmemcpy((u8 XDATA *)&p_bpsMeasurement->mean_arterial_pressure.mantissa,(p_payload+index),2);
				index += 2;
				
				if(flag & BP_MEASUREMENT_FLAG_TIMESTAMP)
				{
					p_bpsMeasurement->time_stamp_present = 1;
					//Timestamp Values
					xmemcpy((u8 XDATA *)&p_bpsMeasurement->time_stamp.year,p_payload+index,2);
					index+=2;
					p_bpsMeasurement->time_stamp.month= *(p_payload+index);
					index+=1;
					p_bpsMeasurement->time_stamp.day= *(p_payload+index);
					index+=1;
					p_bpsMeasurement->time_stamp.hours= *(p_payload+index);
					index+=1;
					p_bpsMeasurement->time_stamp.minutes= *(p_payload+index);
					index+=1;
					p_bpsMeasurement->time_stamp.seconds= *(p_payload+index);
					index+=1;
				}
				else
				{
					p_bpsMeasurement->time_stamp_present = 0;
				}
				if(flag & BP_MEASUREMENT_FLAG_PULSE_RATE)
				{
					p_bpsMeasurement->pulse_rate_present = 1;
					//Pulse Rate Values
					p_bpsMeasurement->pulse_rate.exponent = *(p_payload+index);
					index += 1;
					xmemcpy((u8 XDATA *)&p_bpsMeasurement->pulse_rate.mantissa,(p_payload+index),2);
					index += 2;					
				}
				else
				{
					p_bpsMeasurement->pulse_rate_present = 0;
				}
				if(flag & BP_MEASUREMENT_FLAG_USER_ID)
				{
					p_bpsMeasurement->user_id_present = 1;
					p_bpsMeasurement->user_id = *(p_payload+index);
					index += 1;
				}
				else
				{
					p_bpsMeasurement->user_id_present = 0;
				}
				if(flag & BP_MEASUREMENT_FLAG_STATUS)
				{
					p_bpsMeasurement->measurement_status_present = 1;
					xmemcpy((u8 XDATA *)&p_bpsMeasurement->measurement_status,(p_payload+index),2);
					index += 2;
				}
				else
				{
					p_bpsMeasurement->measurement_status_present = 0;
				}	

				BLE_BPS_Send_Measurement(p_app_conn->handle,p_bpsMeasurement);			
				APP_HEAP_Free((u8 XDATA*)p_bpsMeasurement);
			}
		}
		break;
		
		case CMD_SEND_INTER_CUFF_PRESSURE:
		{
			XDATA BLE_BPS_Cuff_Pressure XDATA *p_bpsCuffPressure;
			index = CMD_LEN_INTER_CUFF_PRESSURE;
			
			p_bpsCuffPressure=(BLE_BPS_Cuff_Pressure XDATA *)APP_HEAP_Alloc(sizeof(BLE_BPS_Cuff_Pressure));
			if(p_bpsCuffPressure!=NULL)
			{
				xmemset((u8 XDATA *)p_bpsCuffPressure,0,sizeof(BLE_BPS_Cuff_Pressure));
				/*Process the first byte for the flags and fill the structure accordingly*/
				flag = *(p_payload+CMD_LEN_INTER_CUFF_PRESSURE);
				index += 1;
				if(flag & BP_MEASUREMENT_FLAG_UNITS)
				{
					p_bpsCuffPressure->blood_pressure_units_in_kpa = BLOOD_PRESSURE_UNIT_KPA;
				}
				else
				{
					p_bpsCuffPressure->blood_pressure_units_in_kpa = BLOOD_PRESSURE_UNIT_MMHG;
				}
					
				//Curretn cuff Pressure value
				p_bpsCuffPressure->blood_pressure_systolic.exponent = *(p_payload+index);
				index += 1;
				xmemcpy((u8 XDATA *)&p_bpsCuffPressure->blood_pressure_systolic.mantissa,(p_payload+index),2);
				index += 2;
				
				if(flag & BP_MEASUREMENT_FLAG_TIMESTAMP)
				{
					p_bpsCuffPressure->time_stamp_present = 1;
					//Timestamp Values
					xmemcpy((u8 XDATA *)&p_bpsCuffPressure->time_stamp.year,p_payload+index,2);
					index+=2;
					p_bpsCuffPressure->time_stamp.month= *(p_payload+index);
					index+=1;
					p_bpsCuffPressure->time_stamp.day= *(p_payload+index);
					index+=1;
					p_bpsCuffPressure->time_stamp.hours= *(p_payload+index);
					index+=1;
					p_bpsCuffPressure->time_stamp.minutes= *(p_payload+index);
					index+=1;
					p_bpsCuffPressure->time_stamp.seconds= *(p_payload+index);
					index+=1;
				}
				else
				{
					p_bpsCuffPressure->time_stamp_present = 0;
				}
				
				if(flag & BP_MEASUREMENT_FLAG_PULSE_RATE)
				{
					p_bpsCuffPressure->pulse_rate_present = 1;
					//Pulse Rate Values
					p_bpsCuffPressure->pulse_rate.exponent = *(p_payload+index);
					index += 1;
					xmemcpy((u8 XDATA *)&p_bpsCuffPressure->pulse_rate.mantissa,(p_payload+index),2);
					index += 2;					
				}
				else
				{
					p_bpsCuffPressure->pulse_rate_present = 0;
				}
				
				if(flag & BP_MEASUREMENT_FLAG_USER_ID)
				{
					p_bpsCuffPressure->user_id_present = 1;
					p_bpsCuffPressure->user_id = *(p_payload+index);
					index += 1;
				}
				else
				{
					p_bpsCuffPressure->user_id_present = 0;
				}
				
				if(flag & BP_MEASUREMENT_FLAG_STATUS)
				{
					p_bpsCuffPressure->measurement_status_present = 1;
					xmemcpy((u8 XDATA *)&p_bpsCuffPressure->measurement_status,(p_payload+index),2);
					index += 2;
				}
				else
				{
					p_bpsCuffPressure->measurement_status_present = 0;
				}			
				
				BLE_BPS_Send_Inter_cuff(p_app_conn->handle,p_bpsCuffPressure);
				APP_HEAP_Free((u8 XDATA*)p_bpsCuffPressure);
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

/**@brief Function for handle  blood pressure event.
 *
 * @param[in] eventType	  Blood pressure service events	
 */
static void bps_event_handler(u16 eventType)large reentrant
{
	if(eventType==BPS_MEASUREMENT_INDICATE_DISABLED)
	{
		app_send_event(EVT_CHANNEL_DISABLED);
	}
	else if(eventType==BPS_MEASUREMENT_INDICATE_ENABLED)
	{
		app_send_event(EVT_CHANNEL_ENABLED);
	}
	else if(eventType==BPS_INTER_CUTOFF_NOTIFY_ENABLED)
	{
		app_send_event(EVT_CHANNEL_ENABLED);
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

			if(p_app_conn != NULL)
			{
				p_app_conn->handle=p_event->eventField.evtConnect.connHandle;
			}
		}
		break;
		
		case BLE_GAP_EVT_DISCONNECTED:
		{
			//BLE link is terminated
			app_send_event(EVT_DISCONNECTED);	
			if(p_app_conn != NULL)
			{
				APP_HEAP_Free((u8 XDATA*)p_app_conn);
			}
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
	BLE_BPS_Catch_Event(p_event);	
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
					XDATA APP_GPIO_Port_Mask	gpio;
					gpio.maskPort0=0x00;
					gpio.maskPort1=(GPIO_PORT_MASK_BIT7);
					gpio.maskPort2=0x00;
					gpio.maskPort3=0x00;
					UTILITY_Shutdown(&gpio);
					//UTILITY_Shutdown(GPIO_P17);
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
			u8 XDATA *p_buffer;
				
			APP_UART_GetReceivedDataLength((u8 XDATA *)&data_len);
			p_buffer=APP_HEAP_Alloc(data_len);
            	
			if(p_buffer!=NULL)
			{
				APP_UART_GetReceivedData(data_len,p_buffer);
			}

			if(uart_cmd_wait)
			{
				APP_HEAP_Free(p_buffer);
				return;
			}
		
			while(data_len > 0)
			{
				tempBCH = p_buffer[index++];
				uart_cmd_buffer[uart_cmd_index]=tempBCH;

				if((uart_cmd_buffer[uart_cmd_index-1]==(UART_END_PATTERN>>8))&&(uart_cmd_buffer[uart_cmd_index]==(u8)(UART_END_PATTERN)))
				{
					uart_cmd_wait=UART_STATE_WAIT;
					APP_UART_Put(sizeof(uart_cmd_buffer), uart_cmd_buffer);	
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
	u8 XDATA newName[]="Blood Pressure";

	//BLE stack call back function initialize
	BLE_GAP_Init(gap_event_handler);
	BLE_L2CAP_Init(l2cap_event_handler);
	BLE_GATT_Init(gatt_event_handler);
	BLE_SMP_Init(smp_event_handler);
	
	//MBIoT call back function initialize
	APP_PERIPHERAL_Init(peripheral_event_handler);
	APP_COMMON_Init(common_event_handler);
	APP_HEAP_Init();
	
	//Application initialize
	BLE_BPS_Init(bps_event_handler);
	
	BLE_GATTS_StartCreatingServiceTable();
	BLE_BPS_Init_Service();
	BLE_GATTS_EndCreatingServiceTable();

	BLE_BPS_Body_Movement_Detection_Support(1);
	BLE_BPS_Cuff_Fit_Detection_Support(1);
	BLE_BPS_Irregular_Pulse_Detection_Support(1);
	BLE_BPS_Pulse_Rate_Range_Detection_Support(1);
	BLE_BPS_Measurement_Position_Detection_Support(1);
	BLE_BPS_Multiple_Bond_Support(1);

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
