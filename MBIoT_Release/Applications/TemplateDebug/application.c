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
				break;
					
				case APP_TIMER_ID_1:
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
		case APP_UART_EVT_RECEIVE_DATA:
		{
			//The length of UART queued data in RX buffer is larger than the APP_UART_ConfParas rxNotifyLength parameter
		}
		break;
		
		case APP_UART_EVT_SEND_DATA_FINISH:
		{
			//The specified length of data is transmitted
		}
		break;
		
		case APP_SPI_SLAVE_EVT_TRANSFER_DONE:
		{
			//SPI communication is finished
		}
		break;
		
		case APP_SPI_SLAVE_EVT_RECEIVE_DATA:
		{
			//SPI Slave get data from master 
		}
		break;
		
		case APP_GPIO_STATE_CHANGED:
		{
			//The polled GPIOs state are changed
		}
		break;
		
		default:
		break;
	}
}


/**@brief Function for Application init entry. */
PUBLIC void Application_Init() large reentrant
{
	//BLE stack call back function initialize
	BLE_GAP_Init(gap_event_handler);
	BLE_L2CAP_Init(l2cap_event_handler);
	BLE_GATT_Init(gatt_event_handler);
	BLE_SMP_Init(smp_event_handler);
	
	//MBIoT call back function initialize
	APP_PERIPHERAL_Init(peripheral_event_handler);
	APP_COMMON_Init(common_event_handler);
	APP_HEAP_Init();
}
