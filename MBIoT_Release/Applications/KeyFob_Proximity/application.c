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
#include "ble_alert_notify_service.h"
#include "ble_immediate_alert_service.h"
#include "ble_link_loss_service.h"
#include "ble_tx_power_service.h"

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

/**@biref ANS, supported alert categories. */
static XDATA u16 supportedNewAlertCategories;
static XDATA u16 supportedUnreadAlertCategories;

/**@biref ANS, control point data structures. */
static XDATA BLE_ANS_AertNotifyState alertNotifyCs;
static XDATA u16 newAlertCategories;
static XDATA u16 unreadAlertCategories;

/**@biref LLS alert level. */
static XDATA u8 LLS_alertLevel;

/**@brief Definition of events. */
static MCODE APP_EVENT_STRUCT app_event[]=
{
	{{EVT_LEN_ADVERTISING},			{'A','d','v','e','r','t','i','s','i','n','g',UART_END_PATTERN>>8,UART_END_PATTERN}},
	{{EVT_LEN_UNSPECIFIC_ERROR},	{'U','n','s','p','e','c','i','f','i','c',' ','E','r','r','o','r',UART_END_PATTERN>>8,UART_END_PATTERN}},
	{{EVT_LEN_UNKNOWN_COMMAND},		{'U','n','k','n','o','w',' ','C','o','m','m','a','n','d',UART_END_PATTERN>>8,UART_END_PATTERN}},
	{{EVT_LEN_CONNECTED},			{'C','o','n','n','e','c','t','e','d',UART_END_PATTERN>>8,UART_END_PATTERN}},
	{{EVT_LEN_DISCONNECTED},		{'D','i','s','c','o','n','n','e','c','t','e','d',UART_END_PATTERN>>8,UART_END_PATTERN}},
	{{EVT_LEN_CHANNEL_ENABLED},		{'D','a','t','a',' ','C','h','a','n','n','e','l',' ','E','n','a','b','l','e','d',UART_END_PATTERN>>8,UART_END_PATTERN}},
	{{EVT_LEN_CHANNEL_DISABLED},		{'D','a','t','a',' ','C','h','a','n','n','e','l',' ','D','i','s','a','b','l','e','d',UART_END_PATTERN>>8,UART_END_PATTERN}},
	{{EVT_LEN_NEW_ALERT},		{'N','e','w',' ','A','l','e','r','t',UART_END_PATTERN>>8,UART_END_PATTERN}},
	{{EVT_LEN_UNREAD_ALERT_STATUS}, {'U','n','r','e','a','d',' ', 'A','l','e','r','t',' ','S','t','a','t','u','s',UART_END_PATTERN>>8,UART_END_PATTERN}},		
};

/**@brief Definition of alert events. */
static MCODE APP_EVENT_STRUCT alert_event[]=
{
	{{EVT_LEN_SIMPLEALERT},		{'S','i','m','p','l','e',' ','A','l','e','r','t',UART_END_PATTERN>>8,UART_END_PATTERN}},
	{{EVT_LEN_EMAIL},					{'E','m','a','i','l',UART_END_PATTERN>>8,UART_END_PATTERN}},
	{{EVT_LEN_NEWS},					{'N','e','w','s',UART_END_PATTERN>>8,UART_END_PATTERN}},
	{{EVT_LEN_CALL},					{'C','a','l','l',UART_END_PATTERN>>8,UART_END_PATTERN}},
	{{EVT_LEN_MISSEDCALL},		{'M','i','s','s','e','d',' ','C','a','l','l',UART_END_PATTERN>>8,UART_END_PATTERN}},
	{{EVT_LEN_SMSMMS},				{'S','M','S','/','M','M','S',UART_END_PATTERN>>8,UART_END_PATTERN}},
	{{EVT_LEN_VOICEMAIL},			{'V','o','i','c','e',' ','M','a','i','l',UART_END_PATTERN>>8,UART_END_PATTERN}},
	{{EVT_LEN_SCHEDULE},			{'S','c','h','e','d','u','l','e',UART_END_PATTERN>>8,UART_END_PATTERN}},
	{{EVT_LEN_HIGHPRIORITIZED_ALERT},		{'H','i','g','h',' ','P','r','i','o','r','i','z','e','d',' ','A','l','e','r','t',UART_END_PATTERN>>8,UART_END_PATTERN}},
	{{EVT_LEN_INSTANT_MESSAGE},		{'I','n','s','t','a','n','t',' ','M','e','s','s','a','g','e',UART_END_PATTERN>>8,UART_END_PATTERN}},
};

/**@brief Definition of alert level events. */
static MCODE APP_EVENT_STRUCT alert_level_event[]=
{
	{{EVT_LEN_NO_ALERT}, {'N','o',' ','A','l','e','r','t',UART_END_PATTERN>>8,UART_END_PATTERN}},
	{{EVT_LEN_MILD_ALERT}, {'M','i','l','d',' ','A','l','e','r','t',UART_END_PATTERN>>8,UART_END_PATTERN}},
	{{EVT_LEN_HIGH_ALERT}, {'H','i','g','h',' ','A','l','e','r','t',UART_END_PATTERN>>8,UART_END_PATTERN}},		
};

/**@brief Definition of alert level signals. */
static MCODE APP_SIGNAL_STRUCT alert_level_signal[]=
{
	{{SIG_LEN_NO_ALERT}, {'S','I','G',' ','N','o',' ','A','l','e','r','t',UART_END_PATTERN>>8,UART_END_PATTERN}},
	{{SIG_LEN_MILD_ALERT}, {'S','I','G',' ','M','i','l','d',' ','A','l','e','r','t',UART_END_PATTERN>>8,UART_END_PATTERN}},
	{{SIG_LEN_HIGH_ALERT}, {'S','I','G',' ','H','i','g','h',' ','A','l','e','r','t',UART_END_PATTERN>>8,UART_END_PATTERN}},		
};

/**@brief Definition of commands. */
static MCODE APP_CMD_STRUCT app_cmd[]=
{
	{{CMD_LEN_NEW_ALERT},						{"NewAlert"}},
	{{CMD_LEN_UNREAD_ALERT_STATUS}, {"UnreadAlertStatus"}},
	{{CMD_LEN_CONNECT},							{"Connect"}},
	{{CMD_LEN_DISCONNECT},					{"Disconnect"}},
};

static MCODE APP_CMD_STRUCT alert_cmd[]=
{
	{{CMD_LEN_SIMPLEALERT},		{"Simple Alert"}},
	{{CMD_LEN_EMAIL},					{"Email"}},
	{{CMD_LEN_NEWS},					{"News"}},
	{{CMD_LEN_CALL},					{"Call"}},
	{{CMD_LEN_MISSEDCALL},		{"Missed Call"}},
	{{CMD_LEN_SMSMMS},				{"SMS/MMS"}},
	{{CMD_LEN_VOICEMAIL},			{"Voice Mail"}},
	{{CMD_LEN_SCHEDULE},			{"Schedule"}},
	{{CMD_LEN_HIGHPRIORITIZED_ALERT},		{"High Priorized Alert"}},
	{{CMD_LEN_INSTANT_MESSAGE},		{"Instant Message"}},
};

/**@brief Function for starting advertising. */
static u16 app_start_advertising() large reentrant
{
	BLE_GAP_AdvertisingParas XDATA advpara;
	BLE_GAP_SetAdvertisingDataParas	XDATA advData;
	XDATA u16 result;
	XDATA u8 temp[]={0x02,0x01,0x05,0x18,0x09,'K','e','y','F','o','b',' ','P','r','o','x','i','m','i','t','y',' ','D','e','v','i','c','e',0x03,0x03,0x11,0x18};

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
static void app_send_event(u8 index) large reentrant
{
	XDATA u8 event[MAX_UART_EVT_BUFFER];			
	comemcpy(event, app_event[index].content,app_event[index].length);
	APP_UART_Put(app_event[index].length, event);				
}

/**@brief Function for sending alert events by UART.
 *
 * @param[in] index	  The index of app_alert_event	
 */
static void app_send_alert_event(u8 index) large reentrant
{
	XDATA u8 event[MAX_UART_EVT_BUFFER];
	comemcpy(event, alert_event[index].content,alert_event[index].length);
	APP_UART_Put(alert_event[index].length, event);
}

/**@brief Function for sending alert level events by UART.
 *
 * @param[in] index	  The index of app_alert_level_event	
 */
static void app_send_alert_level_event(u8 index) large reentrant
{
	XDATA u8 event[MAX_UART_EVT_BUFFER];
	comemcpy(event, alert_level_event[index].content,alert_level_event[index].length);
	APP_UART_Put(alert_level_event[index].length, event);
}

/**@brief Function for sending alert level signals by UART.
 *
 * @param[in] index	  The index of app_alert_level_signal	
 */
static void app_send_alert_level_signal(u8 index) large reentrant
{
	XDATA u8 signal[MAX_UART_EVT_BUFFER];
	comemcpy(signal, alert_level_signal[index].content,alert_level_signal[index].length);
	APP_UART_Put(alert_level_signal[index].length, signal);
}

/**@brief Function for initializing application. */
static void app_init() large reentrant
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

static void BLE_GATT_CreateConnection(u8 XDATA *mac_addr) large reentrant
{
	XDATA BLE_GAP_CreateConnParas XDATA *p_createConnPara;
	p_createConnPara=(BLE_GAP_CreateConnParas XDATA *)APP_HEAP_Alloc(sizeof(BLE_GAP_CreateConnParas));
	if(p_createConnPara!=NULL)
	{				
		p_createConnPara->scan_interval = BLE_GAP_SCAN_INTERVAL_MAX;
		p_createConnPara->scan_window = BLE_GAP_SCAN_WINDOW_MAX;

		p_createConnPara->peer_addr.addr_type = BLE_GAP_ADDR_TYPE_PUBLIC;
		p_createConnPara->peer_addr.addr[0] = *(mac_addr+5);
		p_createConnPara->peer_addr.addr[1] = *(mac_addr+4);
		p_createConnPara->peer_addr.addr[2] = *(mac_addr+3);
		p_createConnPara->peer_addr.addr[3] = *(mac_addr+2);
		p_createConnPara->peer_addr.addr[4] = *(mac_addr+1);
		p_createConnPara->peer_addr.addr[5] = *(mac_addr+0);

		p_createConnPara->conn_paras.interval_min = 0x80;
		p_createConnPara->conn_paras.interval_max = 0x80;
		p_createConnPara->conn_paras.latency = 0;
		p_createConnPara->conn_paras.supervision_tmieout = 0x0C00;
		BLE_GAP_CreateConnection(p_createConnPara);
		APP_HEAP_Free((u8 XDATA *)p_createConnPara);
	}
}

/**@brief Function for parsing received UART data.
 *
 * @param[in] p_payload	  Pointer to the payload buffer	
 */
static void app_uart_parsing(u8 XDATA *p_payload) large reentrant
{
	XDATA u8 i;
	XDATA u8 payloadIndex;
	XDATA u8 *p_alert_cmd_base;
		
	for(i=0;i<CMD_END;i++)
	{
		if(!comemcmp(p_payload, app_cmd[i].content,app_cmd[i].length))
		{
			break;
		}
	}
	
	switch(i)
	{
		case CMD_NEW_ALERT: 
			app_send_event(EVT_NEW_ALERT);
			payloadIndex = CMD_LEN_NEW_ALERT;
			p_alert_cmd_base = (p_payload+payloadIndex);
			for(i=0;i<CMD_ALERT_END;i++)
			{
				if(!comemcmp(p_alert_cmd_base, alert_cmd[i].content, alert_cmd[i].length))
				{
					if(alertNotifyCs.newAlertNotifyState == NOTIFY_ENABLE)
					{
						XDATA BLE_ANS_NewAlert_Message XDATA *p_ansNewAlertMessage;
						p_ansNewAlertMessage=(BLE_ANS_NewAlert_Message XDATA *)APP_HEAP_Alloc(sizeof(BLE_ANS_NewAlert_Message));
						if(p_ansNewAlertMessage!=NULL)
						{
							xmemset((u8 XDATA *)p_ansNewAlertMessage,0,sizeof(BLE_ANS_NewAlert_Message));
							payloadIndex += alert_cmd[i].length;
							p_ansNewAlertMessage->category_id = *(p_payload+payloadIndex);
							payloadIndex += 1;
							p_ansNewAlertMessage->no_alerts = *(p_payload+payloadIndex);
							payloadIndex += 1;
							p_ansNewAlertMessage->info_length = *(p_payload+payloadIndex);
							payloadIndex += 1;
							p_ansNewAlertMessage->info = (u8 XDATA *)APP_HEAP_Alloc(p_ansNewAlertMessage->info_length);
							xmemset(p_ansNewAlertMessage->info,0,p_ansNewAlertMessage->info_length);
							xmemcpy(p_ansNewAlertMessage->info,(p_payload+payloadIndex),p_ansNewAlertMessage->info_length);
							if((newAlertCategories&(1<<p_ansNewAlertMessage->category_id))) {							
								BLE_ANS_Send_New_Alert(p_app_conn->handle, p_ansNewAlertMessage);
								app_send_alert_event(i);
							}	
							APP_HEAP_Free((u8 XDATA *)p_ansNewAlertMessage->info);
							APP_HEAP_Free((u8 XDATA *)p_ansNewAlertMessage);
							break;
						}
					}
				}
			}				
		break;
			
		case CMD_UNREAD_ALERT_STATUS:
			app_send_event(EVT_UNREAD_ALERT_STATUS);
			payloadIndex = CMD_LEN_UNREAD_ALERT_STATUS;
			p_alert_cmd_base = (p_payload+payloadIndex);
			for(i=0;i<CMD_ALERT_END;i++)
			{
				if(!comemcmp(p_alert_cmd_base, alert_cmd[i].content, alert_cmd[i].length))
				{
					if(alertNotifyCs.unreadAlertNotifyState == NOTIFY_ENABLE)
					{
						XDATA BLE_ANS_UnreadAlertStatus_Message XDATA *p_ansUnreadAlertStatusMessage;
						p_ansUnreadAlertStatusMessage=(BLE_ANS_UnreadAlertStatus_Message XDATA *)APP_HEAP_Alloc(sizeof(BLE_ANS_UnreadAlertStatus_Message));
						if(p_ansUnreadAlertStatusMessage!=NULL)
						{
							xmemset((u8 XDATA *)p_ansUnreadAlertStatusMessage,0,sizeof(BLE_ANS_UnreadAlertStatus_Message));
							payloadIndex += alert_cmd[i].length;
							p_ansUnreadAlertStatusMessage->category_id = *(p_payload+payloadIndex);
							payloadIndex += 1;
							p_ansUnreadAlertStatusMessage->unread_count = *(p_payload+payloadIndex);
							if((unreadAlertCategories&(1<<p_ansUnreadAlertStatusMessage->category_id))) {
								BLE_ANS_Send_Unread_Alert_Status(p_app_conn->handle, p_ansUnreadAlertStatusMessage);
								app_send_alert_event(i);
							}
							APP_HEAP_Free((u8 XDATA *)p_ansUnreadAlertStatusMessage);
							break;
						}
					}
				}
			}							
		break;		
		
		case CMD_CONNECT:
			payloadIndex = CMD_LEN_CONNECT;
			BLE_GATT_CreateConnection((p_payload+payloadIndex));
		break;
				
		case CMD_DISCONNECT:
			BLE_GAP_TerminateConnection(p_app_conn->handle,BLE_GAP_DISCONNECT_REASON_REMOTE_TERMINATE);
		break;
		
		default:
			app_send_event(EVT_UNKNOWN_COMMAND);
		break;
	}
}

/**@brief Function for alerting signals.
 *
 * @param[in] alertLevel	alert levels(No alert, Mild alert and High alert).  
 */
void BLE_Alert_Signal(u8 alertLevel) large reentrant
{
	switch(alertLevel)
	{
		case ALERT_LEVEL_NO_ALERT:
			app_send_alert_level_signal(SIG_NO_ALERT);
			break;
		case ALERT_LEVEL_MILD_ALERT:
			app_send_alert_level_signal(SIG_MILD_ALERT);
			break;
		case ALERT_LEVEL_HIGH_ALERT:
			app_send_alert_level_signal(SIG_HIGH_ALERT);
			break;
	}
}

/**@brief Function for handling Link Loss Service event.
 *
 * @param[in] eventType	  Link Loss Service events	
 */
static void lls_event_handler(u8 alertLevel) large reentrant
{
	LLS_alertLevel = alertLevel;
	switch(alertLevel)
	{
		case ALERT_LEVEL_NO_ALERT:
			app_send_alert_level_event(EVT_NO_ALERT);
			break;
		case ALERT_LEVEL_MILD_ALERT:
			app_send_alert_level_event(EVT_MILD_ALERT);
			break;
		case ALERT_LEVEL_HIGH_ALERT:
			app_send_alert_level_event(EVT_HIGH_ALERT);
			break;
	}
}

/**@brief Function for handling Immeadiate Alert Service event.
 *
 * @param[in] eventType	  Alert Notification Service events	
 */
static void ias_event_handler(u8 alertLevel) large reentrant
{
	BLE_Alert_Signal(alertLevel);
}

/**@brief Function for handling Alert Notification Service event.
 *
 * @param[in] eventType	  Alert Notification Service events	
 */
static void ans_event_handler(BLE_ANS_Event XDATA *p_ansEvent) large reentrant
{
	XDATA u16 handle=p_ansEvent->handle;
	if(handle==newAlertCCCDHandle)
	{
		XDATA u16 eventType;
		eventType = p_ansEvent->write_value;
		
		if(eventType==ANS_NOTIFY_DISABLED)
		{
			app_send_event(EVT_CHANNEL_DISABLED);
			alertNotifyCs.newAlertNotifyState = NOTIFY_DISABLE;
		}
		else if(eventType==ANS_NOTIFY_ENAABLED)
		{
			app_send_event(EVT_CHANNEL_ENABLED);
			alertNotifyCs.newAlertNotifyState = NOTIFY_ENABLE;
		}
	}
	else if(handle==unreadAlertStatusCCCDHandle)
	{
		XDATA u16 eventType;
		eventType=p_ansEvent->write_value;
		
		if(eventType==ANS_NOTIFY_DISABLED)
		{
			app_send_event(EVT_CHANNEL_DISABLED);
			alertNotifyCs.unreadAlertNotifyState = NOTIFY_DISABLE;
		}
		else if(eventType==ANS_NOTIFY_ENAABLED)
		{
			app_send_event(EVT_CHANNEL_ENABLED);
			alertNotifyCs.unreadAlertNotifyState = NOTIFY_ENABLE;
		}
	}		
	else if(handle==controlPointHandle)
	{	
		XDATA u8 i;
		XDATA BLE_ANS_Alert_Notification_ControlPoint XDATA *p_alertNotifyCp;
		XDATA BLE_ANS_NewAlert_Message XDATA *p_ansNewAlertMessage;
		XDATA BLE_ANS_UnreadAlertStatus_Message XDATA *p_ansUnreadAlertStatusMessage;
		
		p_alertNotifyCp = (BLE_ANS_Alert_Notification_ControlPoint XDATA *)APP_HEAP_Alloc(sizeof(BLE_ANS_Alert_Notification_ControlPoint));
		if(p_alertNotifyCp != NULL) {
			p_alertNotifyCp->command_id = p_ansEvent->write_value>>8;
			p_alertNotifyCp->category_id = p_ansEvent->write_value;
		}
		
		switch(p_alertNotifyCp->command_id)
		{
			case ANS_ENABLE_NEW_INCOMING_ALERT_NOTIFICATION:
			{				
				if(p_alertNotifyCp->category_id == ANS_TYPE_ALL_ALERTS)
					newAlertCategories = supportedNewAlertCategories;
				else
					newAlertCategories = newAlertCategories|(1<<p_alertNotifyCp->category_id);
				
				BLE_ANS_Send_Write_Response(p_app_conn->handle, BLE_GATT_TYPE_WRITE_RESP);
			}	
			break;
			case ANS_ENABLE_UNREAD_CATEGORY_STATUS_NOTIFICATION:
			{
				if(p_alertNotifyCp->category_id == ANS_TYPE_ALL_ALERTS)
					unreadAlertCategories = supportedUnreadAlertCategories;
				else
					unreadAlertCategories = unreadAlertCategories|(1<<p_alertNotifyCp->category_id);
				
				BLE_ANS_Send_Write_Response(p_app_conn->handle, BLE_GATT_TYPE_WRITE_RESP);
			}
			break;				
			case ANS_DISABLE_NEW_INCOMING_ALERT_NOTIFICATION:
			{			
				if(p_alertNotifyCp->category_id == ANS_TYPE_ALL_ALERTS)
					newAlertCategories = 0;
				else
					newAlertCategories = newAlertCategories&(~(1<<p_alertNotifyCp->category_id));
				
				BLE_ANS_Send_Write_Response(p_app_conn->handle, BLE_GATT_TYPE_WRITE_RESP);
			}
			break;				
			case ANS_DISABLE_UNREAD_CATEGORY_STATUS_NOTIFICATION:
			{
				if(p_alertNotifyCp->category_id == ANS_TYPE_ALL_ALERTS)
					unreadAlertCategories = 0;
				else
					unreadAlertCategories = unreadAlertCategories&(~(1<<p_alertNotifyCp->category_id));
				
				BLE_ANS_Send_Write_Response(p_app_conn->handle, BLE_GATT_TYPE_WRITE_RESP);
			}
			break;				
			case ANS_NOTIFY_NEW_INCOMING_ALERT_IMMEDIATELY:
			{
				if(alertNotifyCs.newAlertNotifyState == NOTIFY_ENABLE)
				{
					p_ansNewAlertMessage=(BLE_ANS_NewAlert_Message XDATA *)APP_HEAP_Alloc(sizeof(BLE_ANS_NewAlert_Message));
					if(p_ansNewAlertMessage!=NULL)
					{
						XDATA u8 new_info[] = "new incoming alert";
						xmemset((u8 XDATA *)p_ansNewAlertMessage,0,sizeof(BLE_ANS_NewAlert_Message));
						p_ansNewAlertMessage->category_id = p_alertNotifyCp->category_id;
						p_ansNewAlertMessage->no_alerts = 1;
						p_ansNewAlertMessage->info_length = sizeof(new_info);
						p_ansNewAlertMessage->info = new_info;
						if(p_ansNewAlertMessage->category_id != ANS_TYPE_ALL_ALERTS)
						{
							if((newAlertCategories&(1<<p_ansNewAlertMessage->category_id))) {
								BLE_ANS_Send_New_Alert(p_app_conn->handle, p_ansNewAlertMessage);
								app_send_alert_event(p_ansNewAlertMessage->category_id);
							}
						}
						else
						{
							for(i=0;i<CMD_ALERT_END;i++)
							{
								if((newAlertCategories&(1<<i))) {
									p_ansNewAlertMessage->category_id = i;
									BLE_ANS_Send_New_Alert(p_app_conn->handle, p_ansNewAlertMessage);
									app_send_alert_event(p_ansNewAlertMessage->category_id);
								}
							}
						}
						APP_HEAP_Free((u8 XDATA *)p_ansNewAlertMessage);
					}
				}
				BLE_ANS_Send_Write_Response(p_app_conn->handle, BLE_GATT_TYPE_WRITE_RESP);
			}
			break;				
			case ANS_NOTIFY_UNREAD_CATEGORY_STATUS_IMMEDIATELY:
			{				
				if(alertNotifyCs.unreadAlertNotifyState == NOTIFY_ENABLE)
				{	
					p_ansUnreadAlertStatusMessage=(BLE_ANS_UnreadAlertStatus_Message XDATA *)APP_HEAP_Alloc(sizeof(BLE_ANS_UnreadAlertStatus_Message));
					if(p_ansUnreadAlertStatusMessage!=NULL)
					{
						xmemset((u8 XDATA *)p_ansUnreadAlertStatusMessage,0,sizeof(BLE_ANS_UnreadAlertStatus_Message));
						p_ansUnreadAlertStatusMessage->category_id = p_alertNotifyCp->category_id;
						p_ansUnreadAlertStatusMessage->unread_count = 1;
						if(p_ansUnreadAlertStatusMessage->category_id != ANS_TYPE_ALL_ALERTS)
						{
							if((unreadAlertCategories&(1<<p_ansUnreadAlertStatusMessage->category_id))) {
								BLE_ANS_Send_Unread_Alert_Status(p_app_conn->handle, p_ansUnreadAlertStatusMessage);
								app_send_alert_event(p_ansUnreadAlertStatusMessage->category_id);
							}
						}
						else
						{	
							for(i=0;i<CMD_ALERT_END;i++)
							{
								if((unreadAlertCategories&(1<<i))) {
									p_ansUnreadAlertStatusMessage->category_id = i;
									BLE_ANS_Send_Unread_Alert_Status(p_app_conn->handle, p_ansUnreadAlertStatusMessage);
									app_send_alert_event(p_ansUnreadAlertStatusMessage->category_id);
								}	
							}
						}
						APP_HEAP_Free((u8 XDATA *)p_ansUnreadAlertStatusMessage);
					}
				}
				BLE_ANS_Send_Write_Response(p_app_conn->handle, BLE_GATT_TYPE_WRITE_RESP);
			}
			break;
			default:
			{
				BLE_ANS_Send_Error_Response(p_app_conn->handle, ANS_COMMAND_ERROR);
			}
			break;
		}
		APP_HEAP_Free((u8 XDATA *)p_alertNotifyCp);
	}
}

/**@brief Function for handling alert notify event.
 *
 * @param[in] eventType	  alert notify service events	
 */
static void BLE_Dispach_Service_Events(BLE_GATT_Event XDATA * p_event) large reentrant
{
	BLE_ANS_Catch_Event(p_event);
	BLE_IAS_Catch_Event(p_event);
	BLE_LLS_Catch_Event(p_event);
	BLE_TPS_Catch_Event(p_event);
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
			
			LLS_alertLevel = ALERT_LEVEL_NO_ALERT;
			BLE_Alert_Signal(LLS_alertLevel);
		}
		break;
		
		case BLE_GAP_EVT_DISCONNECTED:
		{
			//BLE link is terminated
			app_send_event(EVT_DISCONNECTED);			
			APP_HEAP_Free((u8 XDATA*)p_app_conn);
			APP_TIMER_FastTimer_Start(APP_TIMER_ID_0,4);
			
			BLE_Alert_Signal(LLS_alertLevel);
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

	BLE_Dispach_Service_Events(p_event);
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
	XDATA u8 newName[] = "KeyFob Proximity Device";
	BLE_GAP_Addr XDATA maddr;
	
	maddr.addr_type = 0x00;
	maddr.addr[0] = 0x00;
	maddr.addr[1] = 0x12;
	maddr.addr[2] = 0xec;
	maddr.addr[3] = 0x5e;
	maddr.addr[4] = 0xbe;
	maddr.addr[5] = 0xcf;
		
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
	
	//Application initialize	
	BLE_ANS_Init(ans_event_handler);
	BLE_IAS_Init(ias_event_handler);
	BLE_LLS_Init(lls_event_handler);
	
	//Initialize service tables
	BLE_GATTS_StartCreatingServiceTable();
	BLE_ANS_Init_Service();
	BLE_IAS_Init_Service();
	BLE_LLS_Init_Service();
	BLE_TPS_Init_Service();
	BLE_GATTS_EndCreatingServiceTable();
	
	//Set service features
	BLE_ANS_Add_SupportedNewAlertCategories();
	BLE_ANS_Add_SupportedUnreadAlertCategories();
	
	BLE_ANS_Get_supportedNewAlertCategories(&supportedNewAlertCategories);
	BLE_ANS_Get_supportedUnreadAlertCategories(&supportedUnreadAlertCategories);
	
	BLE_LLS_Set_AlertLevel(ALERT_LEVEL_NO_ALERT);
	BLE_TPS_Set_Tx_Power_Level(MAX_TX_POWER_LEVEL);
	
	//MBIoT initalize
	UTILITY_LowPowerModeControl(LOW_POWER_MODE_DISALLOW);
	BLE_GAP_Set_DeviceName(sizeof(newName),newName);
	
	//Application initial
	app_init();	
}
