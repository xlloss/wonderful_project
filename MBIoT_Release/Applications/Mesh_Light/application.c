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
#include "ble_mesh.h"

#ifdef APP_OPEN_DEBUG_MODE
PUBLIC XDATA u8 App_Debug_Tx_Buffer[APP_DEBUG_TX_BUFFER_LEN];
PUBLIC XDATA u8 App_Debug_Rx_Buffer[APP_DEBUG_RX_BUFFER_LEN];
#endif

/*******************************************************************/
/***                 Public Variable Declaration                 ***/
/*******************************************************************/

/**@brief Definition of dynamic memory. */
PUBLIC  XDATA APP_HEAP_STRUCT         App_Heap[APP_HEAP_BLOCK_NUMBER];
PUBLIC  XDATA APP_HEAP_INDEX          App_Heap_Index[APP_HEAP_BLOCK_NUMBER];

#define APP_RX_BUFFER_LEN	256		/**< APP RX Buffer Length. */
#define APP_TX_BUFFER_LEN	768		/**< APP TX Buffer Length. */

static XDATA u8 AppRxBuffer[APP_RX_BUFFER_LEN];
static XDATA u8 AppTxBuffer[APP_TX_BUFFER_LEN];

static XDATA MESH_Generic_OnOff gGenericOnOff;
static XDATA u8 gGpioStatus[4];
static XDATA u8 button1_state;
static XDATA u8 button_reset_state;
static XDATA MESH_LocalNode localNode;


/*******************************************************************/
/***                  Public MCODE Declaration                   ***/
/*******************************************************************/
PUBLIC  MCODE    u8   APP_VERSION[]={0x00,0x00};



/*******************************************************************/
/***                    Application Functions                    ***/
/*******************************************************************/

/*******************************************************************/
/***                     Callback Functions                      ***/
/*******************************************************************/


static void app_group_led_on() large reentrant
{
	APP_GPIO_Pin_Set(GROUP_LED);
}

static void app_group_led_off() large reentrant
{
	APP_GPIO_Pin_Clear(GROUP_LED);
}

static void app_node_led_on() large reentrant
{
	APP_GPIO_Pin_Set(NODE_LED);
}

static void app_node_led_off() large reentrant
{
	APP_GPIO_Pin_Clear(NODE_LED);
}

static void app_flash_led(u8 onInterval, u8 offInterval, u8 count, u8 flashInterval)large reentrant
{
	XDATA APP_LED_ConfParas ledParas;
		
	ledParas.flashType=LED_TYPE_SINGLE_FLASH;
	ledParas.onInterval=onInterval;
	ledParas.offInterval=offInterval;
	ledParas.count=count;
	ledParas.flashInterval=flashInterval;
	APP_LED_LED0_Flash(&ledParas);
}
static void app_start_button_polling() large reentrant
{
	XDATA APP_GPIO_Port_Mask 	gpioMask;

	gpioMask.maskPort0=0x00;
	gpioMask.maskPort1=RESET_MASK;
	gpioMask.maskPort2=0x00;
	gpioMask.maskPort3=BUTTON_1_MASK;
	APP_GPIO_Polling_Start(&gpioMask);
	APP_GPIO_Port_Read(GPIO_PORT1, &gGpioStatus[1]);	
	APP_GPIO_Port_Read(GPIO_PORT3, &gGpioStatus[3]);	
}

static u16 app_start_provisioning() large reentrant
{
	XDATA MESH_Provision_UUID uuid;
	XDATA BLE_GAP_Addr localAddr;
	XDATA u16 result = 0;
	
	BLE_GAP_Get_Addr(&localAddr);
	
	uuid.manufacture[0] = 0x62;
	uuid.manufacture[1] = 0x8f;
	uuid.manufacture[2] = 0xf8;
	uuid.manufacture[3] = 0xe3;
	#ifdef MESH_CHARACTERISTIC_OF_LIGHT
	uuid.productModel = MODEL_ID_GENERIC_ONOFF_SERVER;
	#else
	uuid.productModel = MODEL_ID_GENERIC_ONOFF_CLIENT;
	#endif
	uuid.characteristic[0] = 0x4f;
	uuid.characteristic[1] = 0x38;
	uuid.characteristic[2] = 0x8f;
	uuid.characteristic[3] = 0xbc;
	xmemcpy(uuid.bdAddr, localAddr.addr, 6);	
	
	//broadcast provisioning advertising
	result = BLE_MESH_DeviceProvisioningByGatt(MESH_OPERATOR_START, (u8 XDATA *)&uuid);	
	return result;
}

static void app_send_generif_onoff_set(u8 appInstance,u8 XDATA *payload) large reentrant
{
	XDATA MESH_Transmit_Data transmitData;
	
	transmitData.appPayloadType=APP_PAYLOAD_TYPE_2;
	transmitData.appInstance=appInstance;
	transmitData.ttl=localNode.defaultTtl;
	transmitData.dstAddr = localNode.publishAddress;
	transmitData.appPayload.twoOpcodeData.opcode=GENERIC_ON_OFF_SET_RELIABLE;
	transmitData.appPayload.twoOpcodeData.paraLen=2;
	xmemcpy(transmitData.appPayload.twoOpcodeData.parameters, payload, 2);
	BLE_MESH_TransmitData(localNode.nodeAddress,&transmitData);
}

static void app_transition_time_up() large reentrant
{
	if(gGenericOnOff.multiLedSelect == GROUP_LED_ACTIVE)
	{
		if(gGenericOnOff.targetOnOff==GENERIC_ONOFF_STATE_ON)
		{
			gGenericOnOff.onOff=GENERIC_ONOFF_STATE_ON;
			app_group_led_on();
		}
		else
		{
			gGenericOnOff.onOff=GENERIC_ONOFF_STATE_OFF;
			app_group_led_off();
		}	
	}
	else
	{	
		if(gGenericOnOff.targetOnOff==GENERIC_ONOFF_STATE_ON)
		{
			gGenericOnOff.onOff=GENERIC_ONOFF_STATE_ON;
			app_node_led_on();
		}
		else
		{
			gGenericOnOff.onOff=GENERIC_ONOFF_STATE_OFF;
			app_node_led_off();
		}
	}
	gGenericOnOff.targetOnOff = 0;
}

static u16 app_get_remaining_time() large reentrant
{
	XDATA u16 tempRemainingTime;
	if(gGenericOnOff.remainingTime != 0)
	{
		if(gGenericOnOff.timeResolution == TRANSITION_STEP_RESOLUTION_100MS)
		{
			tempRemainingTime = gGenericOnOff.remainingTime;
			tempRemainingTime &= 0x3F;
			tempRemainingTime |= (TRANSITION_STEP_RESOLUTION_100MS<<6);
		}
		else if(gGenericOnOff.timeResolution == TRANSITION_STEP_RESOLUTION_1S)
		{
			tempRemainingTime = divide_u16(gGenericOnOff.remainingTime, 10);
			tempRemainingTime &= 0x3F;
			tempRemainingTime |= (TRANSITION_STEP_RESOLUTION_1S<<6);
		}
		else if(gGenericOnOff.timeResolution == TRANSITION_STEP_RESOLUTION_10S)
		{
			tempRemainingTime = gGenericOnOff.remainingTime;
			tempRemainingTime &= 0x3F;
			tempRemainingTime |= (TRANSITION_STEP_RESOLUTION_10S<<6);
		}
		else if(gGenericOnOff.timeResolution == TRANSITION_STEP_RESOLUTION_600S)
		{
			tempRemainingTime = divide_u16(gGenericOnOff.remainingTime, 60);
			tempRemainingTime &= 0x3F;
			tempRemainingTime |= (TRANSITION_STEP_RESOLUTION_600S<<6);
		}
	}
	else
		tempRemainingTime = 0;	
	
	return tempRemainingTime;
}

static void app_send_generic_onoff_status() large reentrant
{
	XDATA MESH_Transmit_Data transmitData;
	XDATA u16 tempRemainingTime;
	
	tempRemainingTime = app_get_remaining_time();
	xmemset((u8 XDATA *)&transmitData, 0, sizeof(MESH_Transmit_Data));
	transmitData.appPayloadType=APP_PAYLOAD_TYPE_2;//use payload type 2
	transmitData.appInstance=0;
	transmitData.ttl=localNode.defaultTtl;
	transmitData.dstAddr = localNode.publishAddress;
	transmitData.appPayload.twoOpcodeData.opcode=GENERIC_ON_OFF_STATUS;
	transmitData.appPayload.twoOpcodeData.paraLen=3;
	
	transmitData.appPayload.twoOpcodeData.parameters[0] = gGenericOnOff.onOff;
	transmitData.appPayload.twoOpcodeData.parameters[1] = gGenericOnOff.targetOnOff;
	transmitData.appPayload.twoOpcodeData.parameters[2] = tempRemainingTime;
	BLE_MESH_TransmitData(localNode.nodeAddress,&transmitData);							
}

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
		}
		break;
		
		case BLE_GAP_EVT_DISCONNECTED:
		{
			//BLE link is terminated
		}
		break;
		
		case BLE_GAP_EVT_CONN_PARA_UPDATE:
		{
			//The connection parameters of BLE link are updated
		}
		break;
		
		case BLE_GAP_EVT_ADVERTISING_REPORT:
		{
			//Received advertising report

		}
		break;
		
		case BLE_GAP_EVT_ENCRYPT_STATUS:
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
			//Pairing process is finished
		}
		break;
		
		case BLE_SMP_EVT_SECURITY_REQUEST:
		{
			//Receives Pairing request from remote device
		}
		break;
		
		case BLE_SMP_EVT_NUMERIC_COMPARISON_CONFIRM_REQUEST:
		{
			//Passkey is needed to be confirmed for pairing process
		}
		break;
		
		case BLE_SMP_EVT_INPUT_PASSKEY:
		{
			//Passkey is requested to be inputted for pairing process
		}
		break;
		
		case BLE_SMP_EVT_DISPLAY_PASSKEY_REQUEST:
		{
			//Passkey is needed to be displaied for pairing process
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
			//Error Response Event
		}
		break;
		
		case BLE_GATTC_EVT_DISC_PRIM_SERV_RESP:
		{
			//Discover Primary Services Response Event
		}
		break;
		
		case BLE_GATTC_EVT_DISC_CHAR_RESP:
		{
			//Discover All Characeristics Response Event
		}
		break;
		
		case BLE_GATTC_EVT_DISC_DESC_RESP:
		{
			//Discover All Descriptors Response Event
		}
		break;
		
		case BLE_GATTC_EVT_READ_USING_UUID_RESP:
		{
			//Read Using UUID Response Event
		}
		break;
		
		case BLE_GATTC_EVT_READ_RESP:
		{
			//Read Response Event
		}
		break;
		
		case BLE_GATTC_EVT_WRITE_RESP:
		{
			//Write Response Event
		}
		break;
		
		case BLE_GATTC_EVT_HV_NOTIFY:
		{
			//Handle Value Notification Event
		}
		break;
		
		case BLE_GATTC_EVT_HV_INDICATE:
		{
			//Handle Value Indication Event
		}
		break;
		
		case BLE_GATTS_EVT_READ:
		{
			// Read Request Event
		}
		break;
		
		case BLE_GATTS_EVT_WRITE:
		{
			//Write Request/Command Event
		}
		break;
		
		case BLE_GATTS_EVT_HV_CONFIRM:
		{
			// Handle Value Confirmation Event
		}
		break;
		
		case BLE_ATT_EVT_TIMEOUT:
		{
			// ATT Stack Timeout Event
		}
		break;
		
		case BLE_ATT_EVT_UPDATE_MTU:
		{
			//Update MTU Event
		}
		break;
		
		default:
		break;
	}
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
			//Receives L2CAP connection parameters update response from remote device
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
			 //Time up event
			switch(p_event->eventField.evtTimer.timerID)
			{
				case APP_TIMER_ID_0:
				{
					gGenericOnOff.remainingTime--;//fast
					if(gGenericOnOff.remainingTime == 0)
					{
						app_transition_time_up();
					}
					else
					{
						APP_TIMER_FastTimer_Start(APP_TIMER_ID_0, FAST_TIMER_100MS);
					}
				}
				break;
					
				case APP_TIMER_ID_1:
				{
					gGenericOnOff.remainingTime--;//slow
					if(gGenericOnOff.remainingTime == 0)
					{
						app_transition_time_up();
					}
					else
					{
						APP_TIMER_SlowTimer_Start(APP_TIMER_ID_1, SLOW_TIMER_10S);
					}
				
				}
				break;				
				
				case APP_TIMER_ID_2:
				{
					if(button_reset_state==BUTTON_STATE_LOW)
					{
						//RESET Button pressed	
						XDATA u16 result = 0;
						XDATA MESH_ConfParas	confParas;
						BLE_MESH_Config(MESH_CONFIG_RESET_TO_DEFAULT,&confParas);
						app_flash_led(1,3,1,60);
						result = app_start_provisioning();
						if(result != MBIOT_RES_SUCCESS)
						{
							APP_TIMER_FastTimer_Start(APP_TIMER_ID_4, FAST_TIMER_1S);
						}						
					}
					button_reset_state=BUTTON_STATE_NONE;
				}
				break;		
				
				case APP_TIMER_ID_3:
				{
					if(button1_state==BUTTON_STATE_LOW)
					{
						//Button 1 pressed					
						#ifdef MESH_CHARACTERISTIC_OF_LIGHT
						app_node_led_off();
						gGenericOnOff.onOff = GENERIC_ONOFF_STATE_OFF;
						app_send_generic_onoff_status();
						#else
						XDATA u8 payload[2];
						payload[0] = GENERIC_ONOFF_STATE_OFF;//turn off the light
						payload[1] = 0x00;//time = 0, immediately switch
						app_send_generif_onoff_set(0,payload);						
						#endif
					}
					else
					{
						//Button 1 released
						#ifdef MESH_CHARACTERISTIC_OF_LIGHT
						app_node_led_on();
						gGenericOnOff.onOff = GENERIC_ONOFF_STATE_ON;
						app_send_generic_onoff_status();
						#else
						XDATA u8 payload[2];
						payload[0] = GENERIC_ONOFF_STATE_ON;//turn on the light
						payload[1] = 0x00;//time = 0, immediately switch
						app_send_generif_onoff_set(0,payload);						
						#endif
					}
					button1_state=BUTTON_STATE_NONE;
				}
				break;
				
				case APP_TIMER_ID_4:
				{
					XDATA u16 result = 0;
					
					result = app_start_provisioning();
					if(result != MBIOT_RES_SUCCESS)
					{
						APP_TIMER_FastTimer_Start(APP_TIMER_ID_4, FAST_TIMER_1S);
					}					
				}
				break;
				//.....
			}
		}
		break;
		case APP_OTA_EVT:
		{
			//OTA request event
		}
		break;
		default:
		break;

	}
}



/**@brief Function for handle PERIPHERAL events.
 *
 * @param[in] p_event	BLE PERIPHERAL events	
 */
PUBLIC void peripheral_event_handler(APP_Peripheral_Event XDATA * p_event) large reentrant
{
	switch(p_event->eventID)
	{
		case APP_GPIO_STATE_CHANGED:
		{
			if((gGpioStatus[3] ^ p_event->eventField.gpioStateEvent.port3State) & BUTTON_1_MASK)
			{
				if(button1_state == BUTTON_STATE_NONE)
				{
					if(p_event->eventField.gpioStateEvent.port3State & BUTTON_1_MASK)
					{
						button1_state = BUTTON_STATE_HIGH;
					}
					else
					{
						button1_state = BUTTON_STATE_LOW;
					}
					APP_TIMER_FastTimer_Start(APP_TIMER_ID_3, 120);//75ms	
				}
				else
				{
					button1_state = BUTTON_STATE_NONE;
					APP_TIMER_FastTimer_Stop(APP_TIMER_ID_3);
				}		
			}		
			
			if((gGpioStatus[1] ^ p_event->eventField.gpioStateEvent.port1State) & RESET_MASK)
			{
				if(button_reset_state == BUTTON_STATE_NONE)
				{
					if(p_event->eventField.gpioStateEvent.port1State & RESET_MASK)
					{
						button_reset_state = BUTTON_STATE_HIGH;
					}
					else
					{
						button_reset_state = BUTTON_STATE_LOW;
					}
					APP_TIMER_FastTimer_Start(APP_TIMER_ID_2, RESET_BUTTON_PRESS_DURATION);//160ms	
				}
				else
				{
					button_reset_state = BUTTON_STATE_NONE;
					APP_TIMER_FastTimer_Stop(APP_TIMER_ID_2);
				}					
			}
			
	
			APP_GPIO_Port_Read(GPIO_PORT1, &gGpioStatus[1]);	
			APP_GPIO_Port_Read(GPIO_PORT3, &gGpioStatus[3]);	
		}
		break;
		
		default:
		break;
	}
}

PUBLIC void mesh_event_handler(Ble_Mesh_Event XDATA * p_event) large reentrant
{
	switch(p_event->eventID)
	{
		case BLE_MESH_EVT_RCV_DATA:
		{
			#ifdef MESH_CHARACTERISTIC_OF_LIGHT
		 	if(p_event->eventField.onMeshRcvData.appPayloadType == APP_PAYLOAD_TYPE_2)
			{
				XDATA u16 cmd_op;
				cmd_op = p_event->eventField.onMeshRcvData.appPayload.twoOpcodeData.opcode;
				switch(cmd_op)
				{
					case GENERIC_ON_OFF_SET_RELIABLE:
					case GENERIC_ON_OFF_SET_UNRELIABLE:
					case GENERIC_ON_OFF_GET:
					{
						XDATA MESH_Transmit_Data transmitData;
						XDATA u16 tempRemainingTime;
            	    	
						if((cmd_op == GENERIC_ON_OFF_SET_UNRELIABLE) || (cmd_op == GENERIC_ON_OFF_SET_RELIABLE))
						{
							XDATA u8 switchOnOff = p_event->eventField.onMeshRcvData.appPayload.twoOpcodeData.parameters[0];
							XDATA u16 onOffTime = (u16) (p_event->eventField.onMeshRcvData.appPayload.twoOpcodeData.parameters[1]&0x3F);
							XDATA u16 setTime = 0;
            			
							gGenericOnOff.timeResolution =  p_event->eventField.onMeshRcvData.appPayload.twoOpcodeData.parameters[1]>>6;

							if(onOffTime == 0)
							{
								if(switchOnOff == GENERIC_ONOFF_STATE_ON)
								{
									if((p_event->eventField.onMeshRcvData.dstAddr == localNode.nodeAddress) || (p_event->eventField.onMeshRcvData.dstAddr == 0xFFFF))
									{
										gGenericOnOff.onOff |= GENERIC_ONOFF_STATE_ON;
										app_node_led_on();
									}
									else
									{
										app_group_led_on();
									}
								}	
								else if(switchOnOff == GENERIC_ONOFF_STATE_OFF)
								{
									if((p_event->eventField.onMeshRcvData.dstAddr == localNode.nodeAddress) || (p_event->eventField.onMeshRcvData.dstAddr == 0xFFFF))
									{
										gGenericOnOff.onOff &= (~GENERIC_ONOFF_STATE_ON);
										app_node_led_off();
									}
									else
									{
										app_group_led_off();
									}
								}						
							}
							else
							{
								if(switchOnOff == GENERIC_ONOFF_STATE_ON)
								{
									gGenericOnOff.targetOnOff = GENERIC_ONOFF_STATE_ON;
									if((p_event->eventField.onMeshRcvData.dstAddr == localNode.nodeAddress) || (p_event->eventField.onMeshRcvData.dstAddr == 0xFFFF))
									{
										gGenericOnOff.multiLedSelect = NODE_LED_ACTIVE;										
									}
									else
									{
										gGenericOnOff.multiLedSelect = GROUP_LED_ACTIVE;
									}
								}
								else if(switchOnOff == GENERIC_ONOFF_STATE_OFF)
								{
									gGenericOnOff.targetOnOff =GENERIC_ONOFF_STATE_OFF;
									if((p_event->eventField.onMeshRcvData.dstAddr == localNode.nodeAddress) || (p_event->eventField.onMeshRcvData.dstAddr == 0xFFFF))
									{
										gGenericOnOff.multiLedSelect = NODE_LED_ACTIVE;										
									}
									else
									{
										gGenericOnOff.multiLedSelect = GROUP_LED_ACTIVE;
									}
								}
								
								if(gGenericOnOff.timeResolution == TRANSITION_STEP_RESOLUTION_100MS)
								{
									setTime = onOffTime;//How many numbers of 100ms
									APP_TIMER_FastTimer_Stop(APP_TIMER_ID_0);
									APP_TIMER_FastTimer_Start(APP_TIMER_ID_0, FAST_TIMER_100MS);
								}
								else if(gGenericOnOff.timeResolution == TRANSITION_STEP_RESOLUTION_1S)
								{
									setTime = multiply_u16(onOffTime, 10);
									APP_TIMER_FastTimer_Stop(APP_TIMER_ID_0);
									APP_TIMER_FastTimer_Start(APP_TIMER_ID_0, FAST_TIMER_100MS);
								}
								else if(gGenericOnOff.timeResolution == TRANSITION_STEP_RESOLUTION_10S)
								{
									setTime = onOffTime;
									APP_TIMER_FastTimer_Stop(APP_TIMER_ID_1);
									APP_TIMER_SlowTimer_Start(APP_TIMER_ID_1, SLOW_TIMER_10S);
								}
								else if(gGenericOnOff.timeResolution == TRANSITION_STEP_RESOLUTION_600S)
								{
									setTime = multiply_u16(onOffTime, 60);
									APP_TIMER_FastTimer_Stop(APP_TIMER_ID_1);
									APP_TIMER_SlowTimer_Start(APP_TIMER_ID_1, SLOW_TIMER_10S);
								}
								gGenericOnOff.transitionTime = gGenericOnOff.remainingTime = setTime;
							}
						}

						xmemset((u8 XDATA *)&transmitData, 0, sizeof(MESH_Transmit_Data));
						transmitData.appPayloadType=APP_PAYLOAD_TYPE_2;//use payload type 2
						transmitData.appInstance=p_event->eventField.onMeshRcvData.appInstance;
						transmitData.ttl=localNode.defaultTtl;
						transmitData.appPayload.twoOpcodeData.opcode=GENERIC_ON_OFF_STATUS;
						transmitData.appPayload.twoOpcodeData.paraLen=3;
						
						tempRemainingTime = app_get_remaining_time();
						
						transmitData.appPayload.twoOpcodeData.parameters[0] = gGenericOnOff.onOff;
						transmitData.appPayload.twoOpcodeData.parameters[1] = gGenericOnOff.targetOnOff;
						transmitData.appPayload.twoOpcodeData.parameters[2] = tempRemainingTime;
						
						if((cmd_op==GENERIC_ON_OFF_SET_RELIABLE) || (cmd_op==GENERIC_ON_OFF_SET_UNRELIABLE))
						{
							transmitData.dstAddr = localNode.publishAddress;
							BLE_MESH_TransmitData(localNode.nodeAddress,&transmitData);
						}						
						
						if((cmd_op == GENERIC_ON_OFF_SET_RELIABLE) || (cmd_op==GENERIC_ON_OFF_GET))
						{
							transmitData.dstAddr = p_event->eventField.onMeshRcvData.srcAddr;
							BLE_MESH_TransmitData(localNode.nodeAddress,&transmitData);
						}						
					}
					break;
					
					default:
					break;						
				}		
			}
			#endif //#ifdef MESH_CHARACTERISTIC_OF_LIGHT
		}
		break;
		
		case BLE_MESH_EVT_PROVISIONING_COMPLETE:
		{
			if(p_event->eventField.onMeshProvisioningComplete.status == MESH_PROVISIONING_SUCCESS)
			{
				BLE_MESH_GetLocalNodeInfo(&localNode);
				app_start_button_polling();
				app_flash_led(1,3,2,30);
			}
			else
			{
				XDATA u16 result = 0;
				result = app_start_provisioning();
				if(result != MBIOT_RES_SUCCESS)
				{
					APP_TIMER_FastTimer_Start(APP_TIMER_ID_4, FAST_TIMER_1S);
				}					
			}
		}
		break;

		case BLE_MESH_EVT_UPDATE_PUBLICATION:
		{
			localNode.defaultTtl=p_event->eventField.onMeshUpdatePublication.defaultTtl;
			localNode.publishAddress=p_event->eventField.onMeshUpdatePublication.publishAddress;
		}
		break;	
		
		case BLE_MESH_EVT_UPDATE_DEFAULT_TTL:
		{
			localNode.defaultTtl=p_event->eventField.onMeshUpdateDefaultTTL.defaultTtl;
		}
		break;
		
		case BLE_MESH_EVT_NODE_RESET:
		{
			XDATA APP_GPIO_Port_Mask 	gpioMask;
			XDATA u16 result = 0;
	
			gpioMask.maskPort0=0x00;
			gpioMask.maskPort1=0x00;
			gpioMask.maskPort2=0x00;
			gpioMask.maskPort3=BUTTON_1_MASK;
			APP_GPIO_Polling_Stop(&gpioMask);
			result = app_start_provisioning();
			if(result != MBIOT_RES_SUCCESS)
			{
				APP_TIMER_FastTimer_Start(APP_TIMER_ID_4, FAST_TIMER_1S);
			}
			app_flash_led(1,3,1,60);
		}
		break;
				
		default:
		break;
	}
}

/**@brief Function for Application init entry. */
PUBLIC void Application_Init() large reentrant
{
	APP_HEAP_Init();
	
	//BLE stack call back function initialize
	BLE_GAP_Init(gap_event_handler);
	BLE_L2CAP_Init(l2cap_event_handler);
	BLE_GATT_Init(gatt_event_handler);
	BLE_SMP_Init(smp_event_handler);
	
	//MBIoT call back function initialize
	APP_PERIPHERAL_Init(peripheral_event_handler);
	APP_COMMON_Init(common_event_handler);
	
	UTILITY_LowPowerModeControl(LOW_POWER_MODE_DISALLOW);
	
	BLE_MESH_Init(mesh_event_handler);	
	BLE_MESH_Start();
	
	BLE_MESH_GetLocalNodeInfo(&localNode);
	
	APP_GPIO_Pin_Config_Direction(NODE_LED,GPIO_DIR_OUTPUT);
	APP_GPIO_Pin_Clear(NODE_LED);
	APP_GPIO_Pin_Config_Direction(GROUP_LED,GPIO_DIR_OUTPUT);
	APP_GPIO_Pin_Clear(GROUP_LED);
	APP_LED_Init(LED0_MASK);
	
	if(localNode.nodeAddress == 0x0000)
	{
		app_start_provisioning();
		app_flash_led(1,3,1,60);
	}
	else
	{
		//provisioned
		app_start_button_polling();
		app_flash_led(1,3,2,30);
	}
}