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
#include "mbiot.h"
#include "application.h"

PUBLIC  MCODE    u8   APP_VERSION[]={0x00,0x00};

#ifdef APP_OPEN_DEBUG_MODE
PUBLIC XDATA u8 App_Debug_Tx_Buffer[APP_DEBUG_TX_BUFFER_LEN];
PUBLIC XDATA u8 App_Debug_Rx_Buffer[APP_DEBUG_RX_BUFFER_LEN];
#endif

PUBLIC  XDATA APP_HEAP_STRUCT         App_Heap[APP_HEAP_BLOCK_NUMBER];
PUBLIC  XDATA APP_HEAP_INDEX          App_Heap_Index[APP_HEAP_BLOCK_NUMBER];

static XDATA u8 AppRxBuffer[APP_RX_BUFFER_LEN];
static XDATA u8 AppTxBuffer[APP_TX_BUFFER_LEN];

static XDATA u8 uart_RxState;
static XDATA u8 uart_rx_checksum;
static XDATA u16 uart_cmd_rIndex;
static XDATA u16 uart_cmd_len;
static XDATA u8  XDATA *p_uart_cmd_buff;

static XDATA u8 ioCapability;
static XDATA u8 cmdProcessFlag;
static XDATA u8 currentCmd;
/*Module status*/
static XDATA u8 app_module_status;
/*Connection Link Status*/
static XDATA APP_CONN_STRUCT app_conn_list[MAX_CONN_NBR];
static XDATA u8 app_passkey[APP_PASSKEY_LENGTH];

#ifdef UART_CMD_PERIPHERAL_API_TEST
/* SPI sample codec */
static XDATA u8 spi_working;
static XDATA u8 spi_masterId;
static XDATA u8 spi_sw_ncs;
static XDATA u8 spi_sw_gpio;

static XDATA u8 SPI_TX_Buf[APP_SPI_BUFFER_LEN];
static XDATA u8 SPI_RX_Buf[APP_SPI_BUFFER_LEN];
#endif /* UART_CMD_PERIPHERAL_API_TEST*/

PUBLIC MCODE u8 APP_Defined_Services[] = {
	/*Device Information Service*/
	/* ATT_ATTRUBUTE_HEADER_2 */																						/* ATT_ATTRIBUTE_VALUE_SERVICE_2 */
	0x0A,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_PRIMARY_SERVICE>>8,UUID_PRIMARY_SERVICE,					0x00,0x00,	UUID_DEVICE_INFORMATION>>8,UUID_DEVICE_INFORMATION,
	/* ATT_ATTRUBUTE_HEADER_2 */																						/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */
	0x0B,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						READ,	0x00,0x00,	UUID_MANUFACTURE_NAME_STRING>>8,UUID_MANUFACTURE_NAME_STRING,
	/* ATT_ATTRUBUTE_HEADER_2 */																						/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE */
	0x1E,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_MANUFACTURE_NAME_STRING>>8,UUID_MANUFACTURE_NAME_STRING,	0x04,	'M','C','H','P',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	/* ATT_ATTRUBUTE_HEADER_2 */																						/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */
	0x0B,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						READ,	0x00,0x00,	UUID_MODEL_NUMBER_STRING>>8,UUID_MODEL_NUMBER_STRING,
	/* ATT_ATTRUBUTE_HEADER_2 */																						/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE */
	0x1E,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_MODEL_NUMBER_STRING>>8,UUID_MODEL_NUMBER_STRING,			0x04,	'B','M','7','0',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	/* ATT_ATTRUBUTE_HEADER_2 */																						/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */
	0x0B,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						READ,	0x00,0x00,	UUID_SERIAL_NUMBER_STRING>>8,UUID_SERIAL_NUMBER_STRING,
	/* ATT_ATTRUBUTE_HEADER_2 */																						/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE */
	0x1E,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_SERIAL_NUMBER_STRING>>8,UUID_SERIAL_NUMBER_STRING,			0x04,	0x30,0x30,0x30,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	/* ATT_ATTRUBUTE_HEADER_2 */																						/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */
	0x0B,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						READ,	0x00,0x00,	UUID_HARDWARE_REVISION_STRING>>8,UUID_HARDWARE_REVISION_STRING,
	/* ATT_ATTRUBUTE_HEADER_2 */																						/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE */
	0x1E,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_HARDWARE_REVISION_STRING>>8,UUID_HARDWARE_REVISION_STRING,	0x0E,	'5','5','0','5',' ','1','0','2','_','L','E','S','D','K',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	/* ATT_ATTRUBUTE_HEADER_2 */																						/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */
	0x0B,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						READ,	0x00,0x00,	UUID_FIRMWARE_REVISION_STRING>>8,UUID_FIRMWARE_REVISION_STRING,
	/* ATT_ATTRUBUTE_HEADER_2 */																						/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE */
	0x1E,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_FIRMWARE_REVISION_STRING>>8,UUID_FIRMWARE_REVISION_STRING,	0x06,	0x30,0x30,0x39,0x30,0x31,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	/* ATT_ATTRUBUTE_HEADER_2 */																						/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */
	0x0B,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						READ,	0x00,0x00,	UUID_SOFTWARE_REVISION_STRING>>8,UUID_SOFTWARE_REVISION_STRING,
	/* ATT_ATTRUBUTE_HEADER_2 */																						/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE */
	0x1E,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_SOFTWARE_REVISION_STRING>>8,UUID_SOFTWARE_REVISION_STRING,	0x04,	0x30,0x30,0x30,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	/* ATT_ATTRUBUTE_HEADER_2 */																						/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */
	0x0B,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						READ,	0x00,0x00,	UUID_SYSTEM_ID>>8,UUID_SYSTEM_ID,
	/* ATT_ATTRUBUTE_HEADER_2 */																						/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE */
	0x1E,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_SYSTEM_ID>>8,UUID_SYSTEM_ID,								0x08,	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	/* ATT_ATTRUBUTE_HEADER_2 */																						/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */
	0x0B,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						READ,	0x00,0x00,	UUID_IEEE_11073_20601>>8,UUID_IEEE_11073_20601,
	/* ATT_ATTRUBUTE_HEADER_2 */																						/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE */
	0x1E,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_IEEE_11073_20601>>8,UUID_IEEE_11073_20601,					0x08,	0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

/*Enable Milti-Transparent*/
static XDATA u8 TransPolling;
static XDATA Handle_List	localHandleList;
static XDATA u8 indexreTrans;
static XDATA Retry_Buffer reTrans[MAX_PACKET_BUFFER_SIZE];
static XDATA u8 uarbaudrate;
static XDATA u8 retryTrCount;

PUBLIC  MCODE  u8 APP_Defined_Services_TRS[]={
	/*Transparent Service*/
	/* ATT_ATTRUBUTE_HEADER_2 */																											/* ATT_ATTRIBUTE_VALUE_SERVICE_16 */
	0x18,	0x00,	0x00,0x00,		BLE_ATT_UUID_LENGTH_2,	UUID_PRIMARY_SERVICE>>8,UUID_PRIMARY_SERVICE,									0x00,0x00,	UUID_MCHP_PROPRIETARY_SERVICE_16,
	/* ATT_ATTRUBUTE_HEADER_2 */																											/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_16 */
	0x19,	0x00,	0x00,0x00,		BLE_ATT_UUID_LENGTH_2,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,										WRITE|NOTIFY,	0x00,0x00,	UUID_MCHP_TRANS_TX_16,
	/* ATT_ATTRUBUTE_HEADER_16 */																											/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE */
	0x16,	0x00,	0x00,0x00,		BLE_ATT_UUID_LENGTH_16,	UUID_MCHP_TRANS_TX_16,															0x01,	0x00,
	/* ATT_ATTRUBUTE_HEADER_2 */																											/* ATT_ATTRIBUTE_DESCRIPTOR_CCCD */
	0x08,	PM_WRITABLE,	0x00,0x00,		BLE_ATT_UUID_LENGTH_2,	UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG>>8,UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG,		0x00,0x00,
	/* ATT_ATTRUBUTE_HEADER_2 */																											/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_16 */
	0x19,	0x00,	0x00,0x00,		BLE_ATT_UUID_LENGTH_2,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,										WRITE|WRITE_WITHOUT_RESPONSE,	0x00,0x00,	UUID_MCHP_TRANS_RX_16,
	/* ATT_ATTRUBUTE_HEADER_16 */																											/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE */
	0x16,	0x00,	0x00,0x00,		BLE_ATT_UUID_LENGTH_16,	UUID_MCHP_TRANS_RX_16,															0x01,	0x00,
	/* ATT_ATTRUBUTE_HEADER_2 */																											/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_16 */
	0x19,	0x00,	0x00,0x00,		BLE_ATT_UUID_LENGTH_2,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,										WRITE|NOTIFY,	0x00,0x00,	UUID_MCHP_TRANS_CTRL_16,
	/* ATT_ATTRUBUTE_HEADER_16 */																											/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE */
	0x16,	0x00,	0x00,0x00,		BLE_ATT_UUID_LENGTH_16,	UUID_MCHP_TRANS_CTRL_16,														0x01,	0x00,
	/* ATT_ATTRUBUTE_HEADER_2 */																											/* ATT_ATTRIBUTE_DESCRIPTOR_CCCD */
	0x08,	PM_WRITABLE,	0x00,0x00,		BLE_ATT_UUID_LENGTH_2,	UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG>>8,UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG,		0x00,0x00
};

static XDATA u8 manualpattern_procedure;
static XDATA u16 cHandle,sHandle,eHandle;

static void app_master_enable_credit_flow_control(APP_CONN_STRUCT XDATA * connInfo) large reentrant
{
	XDATA BLE_GATTC_WriteParams  wParams;
	XDATA u16 result;

	wParams.charHandle=connInfo->remoteHandleList.ctrlCCCD;
	wParams.charLength=0x02;
	wParams.charValue[0]=(u8)NOTIFICATION;
	wParams.charValue[1]=NOTIFICATION>>8;
	wParams.writeType=BLE_GATT_TYPE_WRITE_REQ;
	wParams.valueOffset=0x0000;
	wParams.flags=0;
	
	result=BLE_GATTC_Write(connInfo->handle, &wParams);
	if(result==MBIOT_RES_SUCCESS)
	{
		connInfo->serviceState=QUERY_IDLE;
		connInfo->txCapacityState =WRITE_INITIATE_CREDIT_FLOW_CTRL;
	}	
}

static void app_master_initiate_credit_flow_control(APP_CONN_STRUCT XDATA * connInfo) large reentrant
{
	XDATA BLE_GATTC_WriteParams  wParams;
	XDATA u8 result;
	
	wParams.charHandle=connInfo->remoteHandleList.transCtrl;
	wParams.charLength=0x01;
	wParams.charValue[0]=TR_READ_MTU_SIZE_AND_ENABLE_CREDIT_CTRL;
	wParams.writeType=BLE_GATT_TYPE_WRITE_REQ;
	wParams.valueOffset=0x0000;
	wParams.flags=0;
	
	result=BLE_GATTC_Write(connInfo->handle, &wParams);
	if(result==MBIOT_RES_SUCCESS)
	{
		connInfo->txCapacityState = WRITE_ENABLE_CREDIT_FLOW_CTRL;
	}
}

static u16 app_master_ctrl_data_receive(u16 option,APP_CONN_STRUCT XDATA * connInfo) large reentrant
{
	XDATA BLE_GATTC_WriteParams  wParams;
	XDATA u16 payload;
	XDATA u16 result;

	payload=option;
	wParams.charHandle=connInfo->remoteHandleList.txCCCD;
	wParams.charLength=sizeof(payload);
	wParams.charValue[0]=payload>>8;
	wParams.charValue[1]=(u8)payload;
	wParams.writeType=BLE_GATT_TYPE_WRITE_REQ;
	wParams.valueOffset=0x0000;
	wParams.flags=0;
	
	result=BLE_GATTC_Write(connInfo->handle, &wParams);	
	if(result==MBIOT_RES_SUCCESS)
	{
		connInfo->role_att = BLE_GATT_ROLE_CLIENT;
		if(option==CLIENT_CHAR_CONFIG_DESCRIPTOR_ENABLE)
		{
			connInfo->txCapacityState = WRITE_DATA_RECEIVE_CTRL_ENABLE;
		}
		else
		{
			connInfo->txCapacityState = WRITE_DATA_RECEIVE_CTRL_DISABLE;
		}
	}
	
	return result;
}

static u8 appGetConnList(u8 handle) large reentrant
{
	XDATA u8 i;
	
	for(i=0;i<MAX_CONN_NBR;i++)
	{
		if(app_conn_list[i].handle==handle)
			break;
	}
	return i;
}

static u8 appGetSpecificList(u8 state) large reentrant
{
	XDATA u8 i;
	
	for(i=0;i<MAX_CONN_NBR;i++)
	{
		if(app_conn_list[i].appState==state)
			break;
	}
	return i;
}

static APP_CONN_STRUCT XDATA * appGetConnInfo(u8 handle) large reentrant
{
	XDATA u8 i = 0;
	do {
		if(app_conn_list[i].handle == handle) 
			return &app_conn_list[i];	
		i++;
	} while (i < MAX_CONN_NBR);

	return NULL;
}

static APP_CONN_STRUCT XDATA * appGetstateConnInfo(u8 state) large reentrant
{
	XDATA u8 i = 0;
	do {
		if(app_conn_list[i].appState == state) 
			return &app_conn_list[i];	
		i++;
	} while (i < MAX_CONN_NBR);

	return NULL;
}

static u8 appGetIdleCount(void) large reentrant
{
	XDATA u8 i = 0, count = 0;
	for(i=0; i<MAX_CONN_NBR; i++)
	{
		if(app_conn_list[i].appState == APP_STATE_IDLE)
			count++;
	}
	return count;
}

static void package_event(u16 tx_len, u8 XDATA *p_payload) large reentrant
{
	XDATA u16	i;
	XDATA u8	check_sum;
	XDATA u8 XDATA 	*p_event, *p_temp;

	p_event = APP_HEAP_Alloc(tx_len+4);
	if(p_event!=NULL)
	{
		p_temp=p_event;
		check_sum = 0;
		*p_temp++ = UART_START_BYTE;
		*p_temp++ = (u8)(tx_len>>8);
		check_sum += (u8)(tx_len>>8);
		*p_temp++ = (u8)tx_len;			
		check_sum += (u8)tx_len;
		
		for (i=0; i < tx_len; i++)
		{
			*p_temp++ =	*(p_payload+i);						
			check_sum += *(p_payload+i);
		}
	
		check_sum = 0xff - check_sum + 1;
		*p_temp = check_sum;

		APP_UART_Put(tx_len+4,p_event);
		APP_HEAP_Free(p_event);
	}
}

static void process_package_event(u8 handle, u8 uartOpc, u16 tx_len, u8 XDATA *p_payload) large reentrant
{
	XDATA u16	i;
	XDATA u8	check_sum;
	XDATA u8 XDATA 	*p_event, *p_temp;

	p_event = APP_HEAP_Alloc(tx_len+4);
	if(p_event!=NULL)
	{
		p_temp=p_event;
		check_sum = 0;
		*p_temp++ = UART_START_BYTE;
		*p_temp++ = (u8)(tx_len>>8);
		check_sum += (u8)(tx_len>>8);
		*p_temp++ = (u8)tx_len;			
		check_sum += (u8)tx_len;
		
		*p_temp++ =uartOpc;
		check_sum += uartOpc;
		*p_temp++ = handle;
		check_sum += handle;
		for (i=0; i < tx_len-2; i++)
		{
			*p_temp++ =	*(p_payload+i);						
			check_sum += *(p_payload+i);
		}
	
		check_sum = 0xff - check_sum + 1;
		*p_temp = check_sum;

		APP_UART_Put(tx_len+4,p_event);
		APP_HEAP_Free(p_event);
	}
}

static void send_event_command_complete(u8 data_len, u8 opcode, u8 status, u8 XDATA *p_buffer) large reentrant
{
	XDATA u8 XDATA * p_commandComplere;
	XDATA u8 total_len=0;
	total_len=data_len+3;
	p_commandComplere=APP_HEAP_Alloc(total_len);
	if(p_commandComplere!=NULL)
	{
		*(p_commandComplere+0)=UART_EVENT_COMMAND_COMPLETE;
		*(p_commandComplere+1)=opcode;
		*(p_commandComplere+2)=status;
		if(data_len)
		{
			xmemcpy((u8 XDATA *)(p_commandComplere+3),(u8 XDATA *)p_buffer,data_len);
		}
		cmdProcessFlag=APP_PROCESS_CMD_ALLOW;
		package_event(total_len, p_commandComplere);
		APP_HEAP_Free(p_commandComplere);
	}
}
static void send_event_connection_complete(u8 status, APP_CONN_STRUCT XDATA *p_conn) large reentrant
{
	XDATA u8 XDATA *p_conn_complete;
	XDATA u8 total_len=0;
	
	if(status==0)
	{
		total_len=17;
		p_conn_complete=APP_HEAP_Alloc(total_len);
		if(p_conn_complete!=NULL)
		{
			*(p_conn_complete+0)=UART_EVENT_LE_CONNECTION_COMPLETE;
			*(p_conn_complete+1)=status;
			*(p_conn_complete+2)=p_conn->handle;
			*(p_conn_complete+3)=p_conn->role;
			
			xmemcpy((u8 XDATA *)(p_conn_complete+4),(byte XDATA *)&(p_conn->remoteAddr),7);
			xmemcpy((u8 XDATA *)(p_conn_complete+11),(byte XDATA *)&(p_conn->connPara.interval_min),2);
			xmemcpy((u8 XDATA *)(p_conn_complete+13),(byte XDATA *)&(p_conn->connPara.latency),2);
			xmemcpy((u8 XDATA *)(p_conn_complete+15),(byte XDATA *)&(p_conn->connPara.supervision_tmieout),2);
		}
	}
	else
	{
		total_len=2;
		p_conn_complete=APP_HEAP_Alloc(total_len);
		if(p_conn_complete!=NULL)
		{
			*(p_conn_complete+0)=UART_EVENT_LE_CONNECTION_COMPLETE;
			*(p_conn_complete+1)=status;
		}
	}
	cmdProcessFlag=APP_PROCESS_CMD_ALLOW;
	if(p_conn_complete!=NULL)
	{
		package_event(total_len, p_conn_complete);
		APP_HEAP_Free(p_conn_complete);
	}
}

static void send_event_disconnection_complete(u8 handle, u8 reason) large reentrant
{
	XDATA u8 XDATA * pt;
	XDATA u8 total_len = 0;
	total_len = 3;
	pt = APP_HEAP_Alloc(total_len);
	if(pt != NULL)
	{
		*(pt+0)=UART_EVENT_DISCONNECT_COMPLETE;
		*(pt+1)=handle;
		*(pt+2)=reason;
		package_event(total_len, pt);
		APP_HEAP_Free(pt);
	}
	cmdProcessFlag = APP_PROCESS_CMD_ALLOW;
}


static void send_event_status(u8 appState) large reentrant
{
	XDATA u8 XDATA *p_status;
	XDATA u8 i;
	p_status = APP_HEAP_Alloc(6);
	if(p_status != NULL)
	{
		*p_status = UART_EVENT_STATUS_REPORT;
		*(p_status+1) = appState;
		for(i=0;i<MAX_CONN_NBR;i++)
		{
			*(p_status+2+i) = app_conn_list[i].appState;
		}
		package_event(6, p_status);
		APP_HEAP_Free(p_status);
	}
}

static void send_event_pairing_complete(u8 handle, u8 status) large reentrant
{
	XDATA u8 XDATA *p_pairing_complete;
	p_pairing_complete = APP_HEAP_Alloc(3);
	if(p_pairing_complete!=NULL)
	{
		*p_pairing_complete = UART_EVENT_PAIRING_COMPLETE;
		*(p_pairing_complete+1) = handle;
		*(p_pairing_complete+2) = status;
		package_event(3, p_pairing_complete);
		APP_HEAP_Free(p_pairing_complete);
	}
	cmdProcessFlag=APP_PROCESS_CMD_ALLOW;
}

static void send_event_display_number(u8 handle, u8 XDATA *p_value) large reentrant
{	
	XDATA u8 XDATA *p_display_number;
	p_display_number = APP_HEAP_Alloc(8);
	if(p_display_number!=NULL)
	{
		*p_display_number = UART_EVENT_DISPLAY_YESNO_REQ;
		*(p_display_number+1) = handle;
		xmemcpy((u8 XDATA *)(p_display_number+2), (u8 XDATA *)p_value,6);
		package_event(8, p_display_number);
		APP_HEAP_Free(p_display_number);
	}
}
static void send_event_input_passkey(u8 handle) large reentrant
{
	XDATA u8 XDATA *p_request;
	p_request = APP_HEAP_Alloc(2);
	if(p_request!=NULL)
	{
		*p_request = UART_EVENT_PASSKEY_ENTRY_REQ;
		*(p_request+1) = handle;
		package_event(2, p_request);
		APP_HEAP_Free(p_request);
	}
}

static void Transparent_clear(u8 handle) large reentrant
{
/*Clear all data of reTransparented*/
	if(indexreTrans)
	{
		XDATA u8 i=0;
		XDATA u8 indexreTrans_temp;
		indexreTrans_temp = indexreTrans;
		
		if(handle == 0x00)/*Clear all reTrans_datas of links */
		{
			APP_TIMER_FastTimer_Stop(APP_TIMER_ID_3);
			for(; i < indexreTrans_temp; i++)
			{
				APP_HEAP_Free(reTrans[i].p_reTransData);
			}
			indexreTrans=0;
			retryTrCount=0;
			send_event_command_complete(0, UART_CMD_SEND_TRANSPARENT_DATA, 0, NULL);
		}
		else
		{
			
			for(; i < indexreTrans_temp; i++)
			{
				if(handle == reTrans[i].transConnHandle){
					APP_HEAP_Free(reTrans[i].p_reTransData);
					indexreTrans--;
					retryTrCount--;
				}
			}

			if(indexreTrans<=0x00)
			{
				APP_TIMER_FastTimer_Stop(APP_TIMER_ID_3);
				indexreTrans=0;
				retryTrCount=0;
				send_event_command_complete(0, UART_CMD_SEND_TRANSPARENT_DATA, 0, NULL);
			}
		}
	}
}

static u16 app_start_advertising() large reentrant
{
	BLE_GAP_AdvertisingParas XDATA advpara;
	BLE_GAP_SetAdvertisingDataParas	XDATA advData;
	XDATA u16 result;
	//u8 XDATA temp[]={0x02,0x01,0x05,0x0b,0x09,0x45,0x6c,0x6c,0x61,0x5f,0x42,0x4c,0x45,0x54,0x52};
	XDATA u8 temp[]={0x02,0x01,0x06};

	advpara.type=BLE_GAP_ADV_TYPE_ADV_IND;
	//advpara.interval=0x0020;
	advpara.interval=0x00c9;
	advData.adLen=sizeof(temp);
	xmemcpy(advData.adData,temp,advData.adLen );
	advData.adScanResLen=0;
	BLE_GAP_Set_AdvertisingData(&advData);
	result=BLE_GAP_Advertising_Start(&advpara);

	return result;
}

static void app_start_beacon() large reentrant
{
	XDATA u8 randnum[6]={0x9A,0xD2,0xF3,0x8D,0x21,0x68},i;
	BLE_GAP_Addr XDATA beaconAddr, localAddr;
	XDATA u8 beaconData[]={0x05,0xff,0x54,0x65,0x73,0x74};
	
	//BLE_GAP_Addr XDATA addr={{0x05},{0x05,0xff,0x54,0x65,0x73,0x74}};
	BLE_GAP_Get_Addr(&localAddr);
	for(i=0;i<sizeof(beaconAddr.addr);i++)
	{
		beaconAddr.addr[i]=((localAddr.addr[i])^randnum[i]);
	}

	beaconAddr.addr[5]|=STATIC_RANDOM_ADDRESS_MASK;
	BLE_GAP_Set_BeaconAddr(&beaconAddr.addr);	
	BLE_GAP_Set_BeaconData(sizeof(beaconData),(u8 XDATA *)beaconData);
	BLE_GAP_BeaconStart(0x0030);
}

#ifdef UART_CMD_PERIPHERAL_API_TEST
static void delay(u8 count)large reentrant
{
	XDATA u8 i;
	for(i=0; i<count ;i++){	
		_nop_();
		_nop_();
	}
}

static void app_ADC_Verification(u8 XDATA *p_cmdPayload) large reentrant
{
	XDATA u8 api_type;
	XDATA u8 XDATA *return_pt = NULL;
	XDATA u8 len = 0;
	XDATA u16 result = MBIOT_RES_SUCCESS;
	
	api_type = *(p_cmdPayload+2);

	if(api_type == TEST_ADC_GET_1_ADC_CHANNEL)
	{
	/* configure 1 ADC channel  Id */
		XDATA u16 adcValue = 0;
		APP_ADC_Init((1 << (*(p_cmdPayload+3))));
		result = APP_ADC_GetValue((*(p_cmdPayload+3)), &adcValue);
		APP_ADC_Close();
		len = 4;
		return_pt = APP_HEAP_Alloc(len);
		if(return_pt == NULL)
			return;
		
		xmemset(return_pt, 0, len);
		(*return_pt) = api_type;
		(*(return_pt+1)) = *(p_cmdPayload+3);
		(*(return_pt+2)) = (adcValue >> 8);
		(*(return_pt+3)) = adcValue;
		

	}
	else if(api_type == TEST_ADC_GET_MULTI_ADC_CHANNEL)
	{
	/* Configure multi-ADC channel */
		XDATA u16 maskValue = 0; 
		XDATA u16 adcValue = 0;
		XDATA u8 i = 0;
		len = 33;
		return_pt = APP_HEAP_Alloc(len);
		if(return_pt == NULL)
			return;
		
		xmemset(return_pt, 0, len);
		(*return_pt) = api_type;
		xmemcpy((u8 XDATA *)(&maskValue), (u8 XDATA*)(p_cmdPayload+3), 2);
		APP_ADC_Init(maskValue);
		for(; i<16; i++) { /* Read 16 ADC sar value */
			if((1<<i) && maskValue)
			{
				result = APP_ADC_GetValue(i, &adcValue);    
				if(result != MBIOT_RES_SUCCESS)
					break;
			}
			xmemcpy((u8 XDATA *)(return_pt+1+multiply_u16(i,2)), (u8 XDATA*)(&adcValue), 2);
			adcValue = 0;
		}
		APP_ADC_Close();
	}
	else if(api_type == TEST_ADC_GET_BATTERY)
	{
		len = 4;
		return_pt = APP_HEAP_Alloc(len);
		if(return_pt == NULL)
			return;
		
		(*return_pt) = api_type;
		(*(return_pt+1)) = ADC_VALUE_UNIT_0_025V;
		APP_ADC_GetBattery((u8 XDATA*)return_pt+2);
	}
	else if(api_type == TEST_ADC_GET_REFERENCE_CALIBRATION_VALUE)
	{
		len = 5;
		return_pt = APP_HEAP_Alloc(len);
		if(return_pt == NULL)
			return;

		(*return_pt) = api_type;
    	APP_ADC_GetCalibrationValue((u8 XDATA*)return_pt+1, (u8 XDATA*)return_pt+3);
	}

	if(return_pt != NULL)
	{
		send_event_command_complete(len, UART_CMD_PERIPHERAL_API_TEST, result, (u8 XDATA*)return_pt);
		APP_HEAP_Free(return_pt);
	}
	else
	{
		send_event_command_complete(0, UART_CMD_PERIPHERAL_API_TEST, APP_ErrCode_Unknown_Command, 0);
	}
}

static void app_PWM_Verification(u8 XDATA *p_cmdPayload) large reentrant
{
	XDATA u8 api_type;
	XDATA u8 XDATA *return_pt = NULL;
	XDATA u8 len = 0;
	XDATA u16 result = MBIOT_RES_SUCCESS;
	
	api_type = *(p_cmdPayload+2);

	if(api_type == TEST_PWM_CONFIGURATION)
	{
	/* Configuration */
		APP_PWM_ConfParas pwmPara;
		len = 8;
		return_pt = APP_HEAP_Alloc(len);
		if(return_pt == NULL)
			return;
		
		
		(*return_pt) = api_type;
		(*(return_pt+1)) = *(p_cmdPayload+3);/* PWM ID */
		(*(return_pt+2)) = *(p_cmdPayload+4);/* Clock */
		(*(return_pt+3)) = *(p_cmdPayload+5);/* autoReloadValue */
		(*(return_pt+4)) = *(p_cmdPayload+6);
		(*(return_pt+5)) = *(p_cmdPayload+7);/* prescalerValue */
		(*(return_pt+6)) = *(p_cmdPayload+8);
		(*(return_pt+7)) = *(p_cmdPayload+9);/* output */
		pwmPara.clockSource = *(p_cmdPayload+4);
		xmemcpy((u8 XDATA *)(&pwmPara.autoReloadValue), (u8 XDATA*)(p_cmdPayload+5), 2);
		xmemcpy((u8 XDATA *)(&pwmPara.compareValue), (u8 XDATA*)(p_cmdPayload+7), 2);
		pwmPara.outputPolarity = *(p_cmdPayload+9);
		result=APP_PWM_Config(*(p_cmdPayload+3), &pwmPara);
		
	}
	else if(api_type == TEST_PWM_START)
	{
	/* Start */
		result=APP_PWM_Start(*(p_cmdPayload+3));
	}
	else if(api_type == TEST_PWM_STOP)
	{
	/* Stop */
		result=APP_PWM_Stop(*(p_cmdPayload+3));
	}
	else if(api_type == TEST_PWM_CLOSE)
	{
	/* Close */
		result=APP_PWM_Close(*(p_cmdPayload+3));
	} 

	if((api_type >= TEST_PWM_START) && (api_type <= TEST_PWM_CLOSE))
	{
		len = 2;
		return_pt = APP_HEAP_Alloc(len);
		if(return_pt == NULL)
			return;
		
		(*return_pt) = api_type;
		(*(return_pt+1)) = *(p_cmdPayload+3);/* PWM ID */
	}
	
	if(return_pt != NULL)
	{
		send_event_command_complete(len, UART_CMD_PERIPHERAL_API_TEST, result, (u8 XDATA*)return_pt);
		APP_HEAP_Free(return_pt);
	}
	else
	{
		send_event_command_complete(0, UART_CMD_PERIPHERAL_API_TEST,APP_ErrCode_Unknown_Command, 0);
	}
}

static void app_LED_Verification(u8 XDATA *p_cmdPayload) large reentrant
{
	XDATA u8 api_type;
	XDATA u8 XDATA *return_pt = NULL;
	XDATA u8 len = 0;
	XDATA u16 result = MBIOT_RES_SUCCESS;
	api_type = *(p_cmdPayload+2);

	if(api_type == TEST_LED_CONFIGURATION)
	{
	/* LED Configuration */	
		len = 2;
		return_pt = APP_HEAP_Alloc(len);
		if(return_pt == NULL)
			return;
		
		*(return_pt+1) = *(p_cmdPayload+3);
		APP_LED_Init(*(p_cmdPayload+3));

	}
	else if((api_type >= TEST_LED0_FLASH_CONTROL) && (api_type <= TEST_LED_COMBO_FLASH_CONTROL))
	{
	/* 0x02:LED0 Flash, 0x03:LED1 Flash, 0x04:LED Combo Flash */
		APP_LED_ConfParas XDATA profile;
		len = 6;
		return_pt = APP_HEAP_Alloc(len);
		if(return_pt == NULL)
			return;
		
		*(return_pt+1) = *(p_cmdPayload+3);
		*(return_pt+2) = *(p_cmdPayload+4);
		*(return_pt+3) = *(p_cmdPayload+5);
		*(return_pt+4) = *(p_cmdPayload+6);
		*(return_pt+5) = *(p_cmdPayload+7);
		profile.flashType = *(p_cmdPayload+3);
		profile.onInterval = *(p_cmdPayload+4);
		profile.offInterval = *(p_cmdPayload+5);
		profile.count = *(p_cmdPayload+6);
		profile.flashInterval = *(p_cmdPayload+7);
		if(api_type == TEST_LED0_FLASH_CONTROL) 
			result=APP_LED_LED0_Flash(&profile);
		else if(api_type == TEST_LED1_FLASH_CONTROL) 
			result=APP_LED_LED1_Flash(&profile);
		 else if(api_type == TEST_LED_COMBO_FLASH_CONTROL) 
			result=APP_LED_Combo_Flash(&profile);
			

	}
	else if(api_type == TEST_LED_CLOSE)
	{
	/* LED Close */
		len = 2;
		return_pt = APP_HEAP_Alloc(len);
		if(return_pt == NULL)
			return;
		
		*(return_pt+1) = *(p_cmdPayload+3);
		APP_LED_Close(*(p_cmdPayload+3));
	}
	
	if(return_pt != NULL)
	{
		*return_pt = api_type;
		send_event_command_complete(len, UART_CMD_PERIPHERAL_API_TEST, result, (u8 XDATA*)return_pt);
		APP_HEAP_Free(return_pt);
	}
	else
	{
		send_event_command_complete(0, UART_CMD_PERIPHERAL_API_TEST,
										APP_ErrCode_Unknown_Command, 0);
	}
}


static void app_GPIO_Verification(u8 XDATA *p_cmdPayload) large reentrant
{	
	XDATA u8 api_type;
	XDATA u8 XDATA *return_pt = NULL;
	XDATA u8 len = 0;
	XDATA u16 result = MBIOT_RES_SUCCESS;
	
	xmemcpy((u8 XDATA *)(&api_type), (u8 XDATA*)(p_cmdPayload+2), 1);
	return_pt = APP_HEAP_Alloc(4);
	if(return_pt == NULL)
		return;
	
	
	xmemset(return_pt, 0, 4);
	switch(api_type)
	{
		case TEST_GPIO_RANGE_CONFIG_DIRECTION:
		{/* Range Config Direction */
			len = 4;
			*(return_pt+1) = *(p_cmdPayload+3);/* pinStart */
			*(return_pt+2) = *(p_cmdPayload+4);/* pinEnd */
			*(return_pt+3) = *(p_cmdPayload+5);/* direction */
			result = APP_GPIO_Range_Config_Direction(*(p_cmdPayload+3), *(p_cmdPayload+4), 
												*(p_cmdPayload+5)); 
		}	
		break;	
		case TEST_GPIO_PIN_CONFIG_DIRECTIO:
		{/* Pin Config Direction */
			len = 3;
			*(return_pt+1) = *(p_cmdPayload+3);/* pinNum */
			*(return_pt+2) = *(p_cmdPayload+4);/* direction */
			result = APP_GPIO_Pin_Config_Direction(*(p_cmdPayload+3), *(p_cmdPayload+4));
		}
		break;
		case TEST_GPIO_PORT_CONFIG_DIRECTION:
		{/* Port Config Direction */
			len = 3;
			*(return_pt+1) = *(p_cmdPayload+3);/* portNum */
			*(return_pt+2) = *(p_cmdPayload+4);/* direction */
			result = APP_GPIO_Port_Config_Direction(*(p_cmdPayload+3), *(p_cmdPayload+4));
		}	
		break;
		case TEST_GPIO_PIN_SET:
		{/* Pin Set */
			len = 2;
			*(return_pt+1) = *(p_cmdPayload+3);/* pinNum */
			result = APP_GPIO_Pin_Set(*(p_cmdPayload+3));
		}
		break;
		case TEST_GPIO_PORT_SET: 
		{/* Port Set */
			len = 3;
			*(return_pt+1) = *(p_cmdPayload+3);/* portNum */
			*(return_pt+2) = *(p_cmdPayload+4);/* setMask */
			result = APP_GPIO_Port_Set(*(p_cmdPayload+3), *(p_cmdPayload+4));
		}
		break;
		case TEST_GPIO_PORT_WRITE:
		{/* Port Write */
			len = 3;
			*(return_pt+1) = *(p_cmdPayload+3);/* portNum */
			*(return_pt+2) = *(p_cmdPayload+4);/* writeValue */
			result = APP_GPIO_Port_Write(*(p_cmdPayload+3), *(p_cmdPayload+4));
		}
		break;
		case TEST_GPIO_PIN_CLEAR: 
		{/* Pin Clear */
			len = 2;
			*(return_pt+1) = *(p_cmdPayload+3);/* nNum*/
			result = APP_GPIO_Pin_Clear(*(p_cmdPayload+3));
		}
		break;
		case TEST_GPIO_PORT_CLEAR:
		{/* Port Clear */
			len = 3;
			*(return_pt+1) = *(p_cmdPayload+3);/* portNum */
			*(return_pt+2) = *(p_cmdPayload+4);/* clearMask */
			result = APP_GPIO_Port_Clear(*(p_cmdPayload+3), *(p_cmdPayload+4));
		}
		break;	
		case TEST_GPIO_PIN_READ:
		{/* Pin Read */
			XDATA u8 read = 0;
			len = 3;
			*(return_pt+1) = *(p_cmdPayload+3);/* pinNum */
			result = APP_GPIO_Pin_Read(*(p_cmdPayload+3), &read);
			*(return_pt+2) = read;
		}
		break;
		case TEST_GPIO_PORT_READ:
		{/* Port Read */
			XDATA u8 read = 0;
			len = 3;
			*(return_pt+1) = *(p_cmdPayload+3);/* portNum */
			result=APP_GPIO_Port_Read(*(p_cmdPayload+3), &read);
			*(return_pt+2) = read;
		}
		break;
		case TEST_GPIO_POLLING_START:
		{
			XDATA APP_GPIO_Port_Mask gpioMask;
			len = 3;
			*(return_pt+1) = *(p_cmdPayload+3);/* portNum */
			*(return_pt+2) = *(p_cmdPayload+4);/* pinMask */

			if(*(p_cmdPayload+3) == 0x00)
			{
				gpioMask.maskPort0=*(p_cmdPayload+4);
				gpioMask.maskPort1=0x00;
				gpioMask.maskPort2=0x00;
				gpioMask.maskPort3=0x00;
			}
			else if(*(p_cmdPayload+3) == 0x01)
			{
				gpioMask.maskPort0=0x00;
				gpioMask.maskPort1=*(p_cmdPayload+4);
				gpioMask.maskPort2=0x00;
				gpioMask.maskPort3=0x00;
			}
			else if(*(p_cmdPayload+3) == 0x02)
			{
				gpioMask.maskPort0=0x00;
				gpioMask.maskPort1=0x00;
				gpioMask.maskPort2=*(p_cmdPayload+4);
				gpioMask.maskPort3=0x00;
			}
			else if(*(p_cmdPayload+3) == 0x03)
			{
				gpioMask.maskPort0=0x00;
				gpioMask.maskPort1=0x00;
				gpioMask.maskPort2=0x00;
				gpioMask.maskPort3=*(p_cmdPayload+4);
			}
			APP_GPIO_Polling_Start(&gpioMask);
		}
		break;
		case TEST_GPIO_POLLING_STOP:
		{
			XDATA APP_GPIO_Port_Mask gpioMask;
			len = 3;
			*(return_pt+1) = *(p_cmdPayload+3);/* portNum */
			*(return_pt+2) = *(p_cmdPayload+4);/* pinMask */

			if(*(p_cmdPayload+3) == 0x00)
			{
				gpioMask.maskPort0=*(p_cmdPayload+4);
				gpioMask.maskPort1=0x00;
				gpioMask.maskPort2=0x00;
				gpioMask.maskPort3=0x00;
			}
			else if(*(p_cmdPayload+3) == 0x01)
			{
				gpioMask.maskPort0=0x00;
				gpioMask.maskPort1=*(p_cmdPayload+4);
				gpioMask.maskPort2=0x00;
				gpioMask.maskPort3=0x00;
			}
			else if(*(p_cmdPayload+3) == 0x02)
			{
				gpioMask.maskPort0=0x00;
				gpioMask.maskPort1=0x00;
				gpioMask.maskPort2=*(p_cmdPayload+4);
				gpioMask.maskPort3=0x00;
			}
			else if(*(p_cmdPayload+3) == 0x03)
			{
				gpioMask.maskPort0=0x00;
				gpioMask.maskPort1=0x00;
				gpioMask.maskPort2=0x00;
				gpioMask.maskPort3=*(p_cmdPayload+4);
			}
			APP_GPIO_Polling_Stop(&gpioMask);		
		}
		break;	
		default:/* unknow command */
			APP_HEAP_Free(return_pt);
		break;
		
	}
	if(return_pt != NULL)
	{
		xmemcpy((u8 XDATA *)(return_pt), (u8 XDATA*)(&api_type), 1);
		send_event_command_complete(len, UART_CMD_PERIPHERAL_API_TEST, result, (u8 XDATA*)return_pt);
		APP_HEAP_Free(return_pt);
	}
	else
	{
		send_event_command_complete(0, UART_CMD_PERIPHERAL_API_TEST,
										APP_ErrCode_Unknown_Command, 0);
	}
}

static u16 app_SPI_Master_Verification(u8 opt, u8 len, u8 XDATA *p_cmdPayload) large reentrant
{
	XDATA u16 result;
	if(opt == APP_SPI_SEND_DATA)
	{/* send data */
		XDATA APP_SPI_Master_SendReceParas  spiMasterSendRece;
		spiMasterSendRece.spiMasterId=spi_masterId;
		spiMasterSendRece.txLen=(len-1);
		spiMasterSendRece.p_txBuffer=(u8 XDATA*)(p_cmdPayload+1);
		spiMasterSendRece.rxBufferLen=0;
		spiMasterSendRece.p_rxBuffer=NULL;
		result = APP_SPI_MASTER_SendReceive(&spiMasterSendRece);
	}
	else if(opt == APP_SPI_CONFIG)
	{/* configure */ 
		XDATA APP_SPI_Master_ConfParas spi_test;
		
		if(*(p_cmdPayload+2) == APP_SPI_MASTER_CLK_RATE_4MHZ) 
			spi_test.clockRate = SPI_MASTER_CLK_RATE_4MHZ;
		else if(*(p_cmdPayload+2) == APP_SPI_MASTER_CLK_RATE_2MHZ) 
			spi_test.clockRate = SPI_MASTER_CLK_RATE_2MHZ;
		else if(*(p_cmdPayload+2) == APP_SPI_MASTER_CLK_RATE_1MHZ) 
			spi_test.clockRate = SPI_MASTER_CLK_RATE_1MHZ;
	       else if(*(p_cmdPayload+2) == APP_SPI_MASTER_CLK_RATE_500HZ) 
			spi_test.clockRate = SPI_MASTER_CLK_RATE_500KHZ;
		else 
			spi_test.clockRate = SPI_MASTER_CLK_RATE_250KHZ;
		
		spi_test.mode = *(p_cmdPayload+3);
		spi_test.txBufferLen = sizeof(SPI_TX_Buf);
		spi_test.p_txBuffer = SPI_TX_Buf;
		spi_test.rxBufferLen = sizeof(SPI_RX_Buf);
		spi_test.p_rxBuffer = SPI_RX_Buf;
		spi_masterId = SPI_MASTER_HW_NCS0;/*default*/
		if(len>=0x03)
			spi_masterId  = *(p_cmdPayload+4);/*Use user setting*/

		spi_sw_gpio = *(p_cmdPayload+5);
		spi_sw_ncs = *(p_cmdPayload+6);
		
		if(spi_sw_gpio == 0x00)
		{
			APP_GPIO_Pin_Config_Direction(GPIO_P11, GPIO_DIR_INPUT);
		}
		else if(spi_sw_gpio == 0x01)
		{
			APP_GPIO_Pin_Config_Direction(GPIO_P12, GPIO_DIR_INPUT);
		}
		
		result = APP_SPI_MASTER_Init(&spi_test);
		if((spi_masterId==SPI_MASTER_HW_NCS0)||(spi_masterId==SPI_MASTER_MANUAL_NCS))
		{
			if(spi_sw_ncs == 0x00)
			{
				APP_GPIO_Pin_Config_Direction(GPIO_P31, GPIO_DIR_OUTPUT);
				APP_GPIO_Pin_Set(GPIO_P31);
			}
			if(spi_sw_ncs == 0x01)
			{
				APP_GPIO_Pin_Config_Direction(GPIO_P30, GPIO_DIR_OUTPUT);
				APP_GPIO_Pin_Set(GPIO_P30);
			}
		}
		else if((spi_masterId==SPI_MASTER_HW_NCS1))
		{
			APP_GPIO_Pin_Config_Direction(GPIO_P27, GPIO_DIR_OUTPUT);
			APP_GPIO_Pin_Set(GPIO_P27);
		}
		APP_TIMER_FastTimer_Start(APP_TIMER_ID_13, 640);	
		spi_working = APP_SPI_MASTER;
	}
	else if(opt == APP_SPI_CLOSE)
	{/* close */ 
		APP_SPI_MASTER_Close();
		spi_working = APP_NORMAL_MODE;
		result = MBIOT_RES_SUCCESS;
	}
	return result;
}

static void app_SPI_Master_RX()large reentrant
{
	XDATA u8 read = 0;

	if(spi_sw_gpio == 0x00)
	{
		APP_GPIO_Pin_Read(GPIO_P11, &read);
	}
	else if(spi_sw_gpio == 0x01)
	{
		APP_GPIO_Pin_Read(GPIO_P12, &read);
	} 
	
	if(read)
	{
		XDATA u8 XDATA *p_spi = NULL;
		XDATA APP_SPI_Master_SendReceParas  spiMasterSendRece;
		p_spi = APP_HEAP_Alloc(APP_SPI_BUFFER_LEN);
		if(p_spi == NULL)
			return; 
		if(spi_masterId == SPI_MASTER_MANUAL_NCS)
		{
			/*Application user must define NCS pin, and then pull down NCS*/
			if(spi_sw_ncs == 0x00)
			{
				APP_GPIO_Pin_Config_Direction(GPIO_P31, GPIO_DIR_OUTPUT);	
				APP_GPIO_Pin_Clear(GPIO_P31);
			}
			else if(spi_sw_ncs == 0x01)
			{
				APP_GPIO_Pin_Config_Direction(GPIO_P30, GPIO_DIR_OUTPUT);	
				APP_GPIO_Pin_Clear(GPIO_P30);
			}
			/*Delay duration for NCS pull down3.1 ms*/
			delay(250);
			delay(250);
			delay(25);
			
		}
		xmemset(p_spi, 0, APP_SPI_BUFFER_LEN);

		spiMasterSendRece.spiMasterId=spi_masterId;
		spiMasterSendRece.txLen=APP_SPI_BUFFER_LEN;
		spiMasterSendRece.p_txBuffer=NULL;/*No data to send*/
		spiMasterSendRece.rxBufferLen=0x32;/*Copy Rx length from DMA buffer*/
		spiMasterSendRece.p_rxBuffer=p_spi;
		APP_SPI_MASTER_SendReceive(&spiMasterSendRece);
		package_event(APP_SPI_BUFFER_LEN, p_spi);/* Send data to UART */
		APP_HEAP_Free(p_spi);

		if(spi_masterId == SPI_MASTER_MANUAL_NCS)
		{	/*After send/receive data, application user pull up NCS*/
			if(spi_sw_ncs == 0x00)
			{
				APP_GPIO_Pin_Set(GPIO_P31);
				APP_GPIO_Pin_Config_Direction(GPIO_P31, GPIO_DIR_INPUT);
			}
			else if(spi_sw_ncs == 0x01)
			{
				APP_GPIO_Pin_Set(GPIO_P30);
				APP_GPIO_Pin_Config_Direction(GPIO_P30, GPIO_DIR_INPUT);
			}
		}
	}
}

static u16 app_SPI_Slave_Verification(u8 opt, u8 len, u8 XDATA *p_cmdPayload) large reentrant
{
	XDATA u16 result = MBIOT_RES_SUCCESS;
	if(opt == APP_SPI_SEND_DATA)
	{/* send data */
		XDATA APP_SPI_Slave_Send  spiSlaveSend;
		spiSlaveSend.txBufferLen=(len-1);
		spiSlaveSend.p_txBuffer=(u8 XDATA*)(p_cmdPayload+1);
		result = APP_SPI_SLAVE_Send(&spiSlaveSend);

		if(spi_sw_gpio == 0x00)
		{
			APP_GPIO_Pin_Set(GPIO_P11);
		}
		else if(spi_sw_gpio == 0x01)
		{
			APP_GPIO_Pin_Set(GPIO_P12);
		}
	}
	else if(opt == APP_SPI_CONFIG)
	{/* configure */
		XDATA APP_SPI_Slave_Config spiSlaveConf;

		if(*(p_cmdPayload+2) == APP_SPI_SLAVE_CLK_RATE_16MHZ) 
			spiSlaveConf.frequency = SPI_SLAVE_CLK_RATE_16MHZ;
		else if(*(p_cmdPayload+2) == APP_SPI_SLAVE_CLK_RATE_8MHZ) 
			spiSlaveConf.frequency = SPI_SLAVE_CLK_RATE_8MHZ;
		else if(*(p_cmdPayload+2) == APP_SPI_SLAVE_CLK_RATE_4MHZ) 
			spiSlaveConf.frequency = SPI_SLAVE_CLK_RATE_4MHZ;
		else if(*(p_cmdPayload+2) == APP_SPI_SLAVE_CLK_RATE_2MHZ) 
			spiSlaveConf.frequency = SPI_SLAVE_CLK_RATE_2MHZ;
		else 
			spiSlaveConf.frequency = SPI_SLAVE_CLK_RATE_1MHZ;
	
		spiSlaveConf.mode = *(p_cmdPayload+3);
		spiSlaveConf.respMode = *(p_cmdPayload+4);
		spiSlaveConf.txBufferLen = sizeof(SPI_TX_Buf);
		spiSlaveConf.p_txBuffer = SPI_TX_Buf;
		spiSlaveConf.rxBufferLen = sizeof(SPI_RX_Buf);
		spiSlaveConf.p_rxBuffer = SPI_RX_Buf;
		result = APP_SPI_SLAVE_Init(&spiSlaveConf);

		spi_sw_gpio = *(p_cmdPayload+5);
		if(spi_sw_gpio == 0x00)
		{
			APP_GPIO_Pin_Config_Direction(GPIO_P11, GPIO_DIR_OUTPUT);
			APP_GPIO_Pin_Clear(GPIO_P11);
		}
		else if(spi_sw_gpio == 0x01)
		{
			APP_GPIO_Pin_Config_Direction(GPIO_P12, GPIO_DIR_OUTPUT);
			APP_GPIO_Pin_Clear(GPIO_P12);
		}
		
		spi_working = APP_SPI_SLAVE;
	}
	else if(opt == APP_SPI_CLOSE)
	{/* close */
		APP_SPI_SLAVE_Close();
		spi_working = APP_NORMAL_MODE;
	}
	return result;
}


static void app_SPI_Slave_RX(u8 len, u8 XDATA *p_rePayload)large reentrant
{
	package_event(len, p_rePayload);/* Send data To Uart */
	if(spi_sw_gpio == 0x00)
	{
		APP_GPIO_Pin_Clear(GPIO_P11);
	}
	else if(spi_sw_gpio == 0x01)
	{
		APP_GPIO_Pin_Clear(GPIO_P12);
	}
}

static void app_I2C_Verification(u8 XDATA *p_cmdPayload) large reentrant
{
	XDATA u8 api_type;
	XDATA u8 XDATA *return_pt = NULL;
	XDATA u8 len = 0;		
	XDATA u16 result = MBIOT_RES_SUCCESS;
	api_type = *(p_cmdPayload+2);

	
	if(api_type == TEST_I2C_CONFIGURATION)
	{
	/* configure */
		XDATA u8 addr, clk;
		addr = *(p_cmdPayload+3);
		clk = *(p_cmdPayload+4);
		result = APP_I2C_Config(addr, clk);
		len = 3;
		return_pt = APP_HEAP_Alloc(len);
		if(return_pt == NULL)
			return;
		(*return_pt) = api_type;
		(*(return_pt+1)) = addr;
		(*(return_pt+2)) = clk;
	}
	else if(api_type == TEST_I2C_WRITE)
	{
	/* Write */
		XDATA u8 senLen = *(p_cmdPayload+3);
		result = APP_I2C_Write(senLen, (p_cmdPayload+4));
		len = (senLen+1);
		return_pt = APP_HEAP_Alloc(senLen+1);
		if(return_pt == NULL)
			return;
		
		(*return_pt) = api_type;
		xmemcpy((u8 XDATA *)(return_pt+1), (u8 XDATA*)(p_cmdPayload+4), senLen);
		
	}
	else if(api_type == TEST_I2C_READ)
	{
	/* Read */
		XDATA u8 writeLen = *(p_cmdPayload+3);
		XDATA u8 XDATA *p_write = APP_HEAP_Alloc(writeLen);
		XDATA u8 readLen = *(p_cmdPayload+3+1+writeLen);
		XDATA u8 XDATA *p_read = APP_HEAP_Alloc(readLen);
		if((p_write == NULL) ||(p_read == NULL))
			return;
		
		
		xmemset(p_write, 0, writeLen);
		xmemcpy((u8 XDATA *)(p_write), (u8 XDATA*)(p_cmdPayload+4), writeLen);
		result = APP_I2C_Write(writeLen, p_write);

		len = (readLen+1);
		return_pt = APP_HEAP_Alloc(readLen+1);
		if(return_pt == NULL)
			return;
		
		xmemset(p_read, 0, readLen);
		(*return_pt) = api_type;
		if(result == MBIOT_RES_SUCCESS)
			result = APP_I2C_Read(readLen, p_read);
		xmemcpy((u8 XDATA *)(return_pt+1), (u8 XDATA*)p_read, readLen);
	
		APP_HEAP_Free(p_read);
		APP_HEAP_Free(p_write);
	}
	else if(api_type == TEST_I2C_COMBINEWR)
	{
	/* CombineWR */
		XDATA APP_I2C_CombineWRParas p_combineWR;
		p_combineWR.lenWrite = *(p_cmdPayload+3);
		xmemset(&p_combineWR.writeBuffer, 0, I2C_MAX_TX_SIZE);
		xmemcpy((u8 XDATA *)&p_combineWR.writeBuffer, (u8 XDATA*)(p_cmdPayload+4), p_combineWR.lenWrite);
		
		p_combineWR.lenRead = *(p_cmdPayload+1+p_combineWR.lenWrite+3);
		xmemset(&p_combineWR.readBuffer, 0, I2C_MAX_RX_SIZE);
		
		result = APP_I2C_CombineWR(&p_combineWR);

		len = (p_combineWR.lenRead+1);
		return_pt=APP_HEAP_Alloc(p_combineWR.lenRead+1);
		if(return_pt == NULL)
			return;
		
		(*return_pt) = api_type;
		
		xmemcpy((u8 XDATA *)(return_pt+1), (u8 XDATA*)(&p_combineWR.readBuffer), p_combineWR.lenRead);
	}
	else if(api_type == TEST_I2C_CLOSE)
	{
	/* close */
		APP_I2C_Close();
		len = 1;
		return_pt = APP_HEAP_Alloc(len);
		if(return_pt == NULL)
			return;
	
		(*return_pt) = api_type;
	}
		
	if(return_pt != NULL)
	{
		send_event_command_complete(len, UART_CMD_PERIPHERAL_API_TEST, result, (u8 XDATA*)return_pt);
		APP_HEAP_Free(return_pt);
	}
	else
	{
		send_event_command_complete(0, UART_CMD_PERIPHERAL_API_TEST,
										APP_ErrCode_Unknown_Command, 0);
	}
}

static void app_SPI_Serial_Flash_Verification(u8 XDATA *p_cmdPayload) large reentrant
{
	XDATA u16 result = MBIOT_RES_SUCCESS;
	XDATA u8 XDATA *return_pt = NULL;
	XDATA u8 len = 0;
	XDATA u8 api_type = *(p_cmdPayload + 2);
	
	if(api_type == TEST_SPI_SFLASH_READ) 
	{/*read*/	
		XDATA APP_SPI_SFlashRParas  readCmd;

		len = *(p_cmdPayload + 3);
		xmemset(readCmd.readBuffer, 0, SPI_SFLASH_MAX_READ_SIZE);
		
		/*flash address*/
		readCmd.contentEntry[0]=*(p_cmdPayload + 4);
		readCmd.contentEntry[1]=*(p_cmdPayload + 5);
		readCmd.contentEntry[2]=*(p_cmdPayload + 6);
		return_pt = APP_HEAP_Alloc(len + 1);
		if(return_pt == NULL)
			return;
		readCmd.contentLen = 0x03;
		readCmd.opcode = FLASH_FR_COM; 
		readCmd.readLen = len;//<=7
		result=APP_SPI_SerialFlash_Read(&readCmd);
		xmemcpy((u8 XDATA *)(return_pt + 1), (u8 XDATA*)(readCmd.readBuffer), len);
		
	}
	else if (api_type == TEST_SPI_SFLASH_WRITE) 
	{/*write*/
		XDATA APP_SPI_SFlashWParas writeCmd;

		len = *(p_cmdPayload + 3);
		writeCmd.contentEntry[0] = *(p_cmdPayload + 4);
		writeCmd.contentEntry[1] = *(p_cmdPayload + 5);
		writeCmd.contentEntry[2] = *(p_cmdPayload + 6);
		/*write data into flash */
		xmemcpy((u8 XDATA *)(&writeCmd.contentEntry[3]), (u8 XDATA*)(p_cmdPayload + 7), len);
		writeCmd.writeEnable = FLASH_WE_COM;
		writeCmd.contentLen = 0x07;
		writeCmd.opcode = FLASH_PP_COM;
		
		return_pt = APP_HEAP_Alloc(len + 1);
		if(return_pt == NULL)
			return;
		
		result=APP_SPI_SerialFlash_Write(&writeCmd);
		xmemcpy((u8 XDATA *)(return_pt + 1), (u8 XDATA*)(p_cmdPayload + 7), len);
		
	}
	else if (api_type == TEST_SPI_SFLASH_ERASE)
	{/*erase*/
		XDATA APP_SPI_SFlashWParas writeCmd;
		return_pt = APP_HEAP_Alloc(len + 1);
		if(return_pt == NULL)
			return;
		writeCmd.writeEnable = FLASH_WE_COM;
		if(*(p_cmdPayload + 3) == ERASE_CHIP)//Chip Erase
		{
			writeCmd.contentLen = 0;
			writeCmd.opcode = FLASH_CHIP_ERASE_COM;
		}
		else if(*(p_cmdPayload + 3) == ERASE_SECTOR)//Sector Erase
		{//*(p_cmdPayload + 4)
			writeCmd.contentLen = 0x03;
			writeCmd.contentEntry[0] = *(p_cmdPayload + 4);
			writeCmd.contentEntry[1] = *(p_cmdPayload + 5);
			writeCmd.contentEntry[2] = *(p_cmdPayload + 6);
			writeCmd.opcode = FLASH_SECTOR_ERASE_COM;
		}
		else if(*(p_cmdPayload + 3) == ERASE_BLOCK)//Block Erase
		{
			writeCmd.contentLen = 0x03;
			writeCmd.contentEntry[0] = *(p_cmdPayload + 4);
			writeCmd.contentEntry[1] = *(p_cmdPayload + 5);
			writeCmd.contentEntry[2] = *(p_cmdPayload + 6);
			writeCmd.opcode = FLASH_BLOCK_ERASE;
		}
		result=APP_SPI_SerialFlash_Write(&writeCmd);
	}
	else if (api_type == TEST_SPI_SFLASH_READID) 
	{/*read ID*/
		XDATA APP_SPI_SFlashRParas  readCmd;
		len = 2;
		return_pt = APP_HEAP_Alloc(len + 1);
		if(return_pt == NULL)
			return;
		xmemset(return_pt, 0, len);	
		xmemset(readCmd.contentEntry, 0, SPI_SFLASH_MAX_READ_SIZE);
		xmemset(readCmd.readBuffer, 0, SPI_SFLASH_MAX_READ_SIZE);
		readCmd.contentLen = 0x03;
		readCmd.opcode = FLASH_DEVICE_ID;
		readCmd.readLen = 0x02;//<=7
		result=APP_SPI_SerialFlash_Read(&readCmd);
		xmemcpy((u8 XDATA *)(return_pt + 1), (u8 XDATA*)(readCmd.readBuffer), len);
		
	}

	if(return_pt != NULL)
	{	
		*return_pt = api_type;
		send_event_command_complete(len + 1, UART_CMD_PERIPHERAL_API_TEST, result, (u8 XDATA*)return_pt);
		APP_HEAP_Free(return_pt);
	}
	else
	{
		send_event_command_complete(0, UART_CMD_PERIPHERAL_API_TEST,
										APP_ErrCode_Unknown_Command, 0);
	}
				
}

static void app_LowPower_Verification(u8 XDATA *p_cmdPayload) large reentrant
{
	XDATA u8 XDATA *return_pt = NULL;
	
	if(*(p_cmdPayload+2) == TEST_LOW_POWER_CTRL)
	{
		UTILITY_LowPowerModeControl(*(p_cmdPayload+3));
	}
	
	return_pt = APP_HEAP_Alloc(0x01);
	if(return_pt == NULL)
	{
		send_event_command_complete(0, UART_CMD_PERIPHERAL_API_TEST,
										APP_ErrCode_Unknown_Command, 0);
		return;
	}
	*return_pt =*(p_cmdPayload+1);
	send_event_command_complete(0x01, UART_CMD_PERIPHERAL_API_TEST, MBIOT_RES_SUCCESS, (u8 XDATA*)return_pt);
	APP_HEAP_Free(return_pt);	
}


static u16 spi_processdata(u16 len, u8 XDATA *p_cmdPayload, u8 XDATA  *revalue) large reentrant
{
	/* Process SPI Master or slave data */
	XDATA u16 result = MBIOT_RES_NO_RESOURCE;

	if(spi_working > APP_NORMAL_MODE)
	{
		if((len-1))
		{
		/* Send data from uart */	
			if(spi_working == APP_SPI_MASTER)
			{
			
				if(spi_masterId == SPI_MASTER_MANUAL_NCS)
				{
					/*Application user must define NCS pin, and then pull down NCS*/
					if(spi_sw_ncs == 0x00)
					{
						APP_GPIO_Pin_Config_Direction(GPIO_P31, GPIO_DIR_OUTPUT);	
						APP_GPIO_Pin_Clear(GPIO_P31);
					}
					else if(spi_sw_ncs == 0x01)
					{
						APP_GPIO_Pin_Config_Direction(GPIO_P30, GPIO_DIR_OUTPUT);	
						APP_GPIO_Pin_Clear(GPIO_P30);
					}
					/*Delay duration for NCS pull down3.1 ms*/
					delay(250);
					delay(250);
					delay(25);
					
				}
				result = app_SPI_Master_Verification(APP_SPI_SEND_DATA, len, p_cmdPayload);
				if(spi_masterId == SPI_MASTER_MANUAL_NCS)
				{	/*After send/receive data, application user pull up NCS*/
					if(spi_sw_ncs == 0x00)
					{
						APP_GPIO_Pin_Set(GPIO_P31);
						APP_GPIO_Pin_Config_Direction(GPIO_P31, GPIO_DIR_INPUT);
					}
					else if(spi_sw_ncs == 0x01)
					{
						APP_GPIO_Pin_Set(GPIO_P30);
						APP_GPIO_Pin_Config_Direction(GPIO_P30, GPIO_DIR_INPUT);
					}
				}
				(*revalue) = PERIPHERAL_API_TEST_SPI_Master;
			
			}
			else if(spi_working == APP_SPI_SLAVE)
			{
				result = app_SPI_Slave_Verification(APP_SPI_SEND_DATA, len, p_cmdPayload);
				(*revalue) = PERIPHERAL_API_TEST_SPI_Slave;
			}
		}
		else
		{
		/* Close SPI  */
			if(spi_working == APP_SPI_MASTER)
			{
				result = app_SPI_Master_Verification(APP_SPI_CLOSE, 0, NULL);
				(*revalue) = PERIPHERAL_API_TEST_SPI_Master;
			}
			else if(spi_working == APP_SPI_SLAVE)
			{
				result = app_SPI_Slave_Verification(APP_SPI_CLOSE, 0, NULL);
				(*revalue) = PERIPHERAL_API_TEST_SPI_Slave;
			}
		}

	}/* end if(spi_working > 0) */
	return result;
}

static void PeripheralVerifyEnd(u8  revalue, u8 result) large reentrant
{
	XDATA u8 XDATA *return_pt = NULL;
	return_pt = APP_HEAP_Alloc(1);
	if(return_pt == NULL)
		return;
	
	xmemcpy((u8 XDATA *)(return_pt), (u8 XDATA*)(&revalue), 1);
	send_event_command_complete(1, UART_CMD_PERIPHERAL_API_TEST, 
										result, (u8 XDATA*)return_pt);
	APP_HEAP_Free(return_pt);
}

#endif /* UART_CMD_PERIPHERAL_API_TEST */

static void app_process_uart_cmd(u16 len, u8 XDATA *p_cmdPayload) large reentrant
{
	XDATA u8 RX_OPCode;

	RX_OPCode = *p_cmdPayload;
	if((RX_OPCode == UART_CMD_LE_CREATE_CONNECTION_CANCEL) /*&& (p_app_conn->appState == APP_STATE_CONNECTING)*/)
	{
		cmdProcessFlag = APP_PROCESS_CMD_ALLOW;
	}

	if(cmdProcessFlag==APP_PROCESS_CMD_FORBID)
	{
		if((RX_OPCode!=UART_CMD_RESET) && (RX_OPCode!=UART_CMD_DISCONNECT) && (RX_OPCode!=UART_CMD_PASSKEY_ENTRY_RES) &&(RX_OPCode!=UART_CMD_DISPLAY_YESNO_RES) && (RX_OPCode!=UART_CMD_LE_CREATE_CONNECTION_CANCEL) && (RX_OPCode!=UART_CMD_SEND_TRANSPARENT_DATA))
		{
			send_event_command_complete(0,RX_OPCode,APP_ErrCode_Command_Disallowed,0);
			return;
		}
	}
	cmdProcessFlag=APP_PROCESS_CMD_FORBID;
	switch(RX_OPCode)
	{
		case UART_CMD_READ_INFO:	//0x01
		{
			XDATA u8 version[10];
			BLE_GAP_Addr XDATA localAddr;
			XDATA APP_UTILITY_MbiotInfo	info;
			xmemset(version,0,10);	
			UTILITY_GetMbiotInformation(&info);
			BLE_GAP_Get_Addr(&localAddr);
			version[0] = info.flashMainVersion;
			version[1] = info.flashSubVersion;
			xmemcpy((u8 XDATA*)(version+4), (u8 XDATA*)localAddr.addr, 6);
			send_event_command_complete(10,UART_CMD_READ_INFO,0,version);
			
		}
		break;

		case UART_CMD_RESET:	//0x02
		{
			UTILITY_Reset();
		}
		break;
		
		case UART_CMD_READ_STATUS: /* 0x03 */
		{	
			send_event_status(app_module_status);

			cmdProcessFlag=APP_PROCESS_CMD_ALLOW;
		}
		break;
		
		case UART_CMD_READ_ADC_VALUE: /* 0x04*/
		{
			XDATA u8 len = 3, type;
			XDATA u8 XDATA *return_pt;
			type = *(p_cmdPayload+1);

			return_pt = APP_HEAP_Alloc(len);
			if(return_pt == NULL)
				break;
			
			if(type == 0x10)
			{
				*return_pt=ADC_VALUE_UNIT_0_025V;
				APP_ADC_GetBattery((u8 XDATA*)return_pt+1);
				send_event_command_complete(len,UART_CMD_READ_ADC_VALUE,0,(u8 XDATA*)return_pt);
			}
			else
			{
				send_event_command_complete(0,UART_CMD_READ_ADC_VALUE,APP_ErrCode_Invalid_Parameters,NULL);
			}

			APP_HEAP_Free(return_pt);
		}
		break;
		
		case UART_CMD_INTO_SHUTDOWN_MODE:
		{	
			XDATA APP_GPIO_Port_Mask	gpio;
			gpio.maskPort0=0x00;
			gpio.maskPort1=0x00;
			gpio.maskPort2=0x00;
			gpio.maskPort3=(GPIO_PORT_MASK_BIT0|GPIO_PORT_MASK_BIT1);
			UTILITY_Shutdown(&gpio);
		}
		break;

		
		case UART_CMD_DEBUG_COMMAND: /* 0x06 */
		{
			XDATA u8 len, type;
			XDATA u16 address=0;
			XDATA u8 XDATA *return_pt;
			XDATA u16 result;
			type = *(p_cmdPayload+1);
			len = *(p_cmdPayload+2);
			address = (u16)((*(u8 XDATA*)(p_cmdPayload+3))<<8) + *(u8 XDATA*)(p_cmdPayload+4);
			
			if(len == 0)
			{
				send_event_command_complete(0, UART_CMD_DEBUG_COMMAND, APP_ErrCode_Invalid_Parameters, NULL);
			}
			else
			{
				if(type == 0x01)/*Read xmemory*/
				{
					return_pt = APP_HEAP_Alloc(len+1);
					if(return_pt != NULL)
					{
						*(return_pt)=type;
						xmemcpy((u8 XDATA *)(return_pt+1),(u8 XDATA*)address,len);
						send_event_command_complete(len+1, UART_CMD_DEBUG_COMMAND, 0, (u8 XDATA*)return_pt);
						APP_HEAP_Free(return_pt);
					}
				}
				else if(type == 0x02)/*write xmemory*/
				{
					xmemcpy((u8 XDATA *)address, (u8 XDATA*)(p_cmdPayload+5), len);
					send_event_command_complete(1, UART_CMD_DEBUG_COMMAND, 0, (u8 XDATA*)(p_cmdPayload+1));
				}
				else if(type == 0x03)/*read eflash*/
				{
					return_pt = APP_HEAP_Alloc(len+1);
					if(return_pt != NULL)
					{
						xmemset(return_pt, 0, (len+1));
						*(return_pt)=type;
						
						result = UTILITY_ReadEflash(address, len, (u8 XDATA *)(return_pt+1));
						if(result == MBIOT_RES_SUCCESS)
							send_event_command_complete(len+1, UART_CMD_DEBUG_COMMAND, 0, (u8 XDATA*)return_pt);
						else
							send_event_command_complete(0, UART_CMD_DEBUG_COMMAND, result, NULL);
						APP_HEAP_Free(return_pt);
					}
				}
				else if(type == 0x04)/*write eflash*/
				{
					result = UTILITY_WriteEflash(address, len, (u8 XDATA*)(p_cmdPayload+5));
					if(result == MBIOT_RES_SUCCESS) 
						send_event_command_complete(1, UART_CMD_DEBUG_COMMAND, result, (u8 XDATA*)(p_cmdPayload+1));
					else
						send_event_command_complete(0, UART_CMD_DEBUG_COMMAND, result, NULL);
				}
				else if(type == 0x05)/*erase eflash*/
				{
					result = UTILITY_EraseEflash(address, len);
					
					send_event_command_complete(0, UART_CMD_DEBUG_COMMAND, result, NULL);
				}
				else
				{
					send_event_command_complete(0, UART_CMD_DEBUG_COMMAND, APP_ErrCode_Command_Disallowed, NULL);
				} 
			}
		}
		break;
		
		case UART_CMD_READ_DEVICE_NAME: //0x07
		{
			XDATA u8 name[BLE_GAP_DEVICE_MAX_LENGTH],nameLen;
			BLE_GAP_Get_DeviceName(&nameLen,name);
			send_event_command_complete(nameLen,UART_CMD_READ_DEVICE_NAME,0,name);
		}
		break;
		
	
		case UART_CMD_ERASE_ALL_PAIRED_DEVICE: //0x09
		{
			if(appGetIdleCount() == MAX_CONN_NBR)
			{
				UTILITY_EraseAllPairedDevice();
      			send_event_command_complete(0, UART_CMD_ERASE_ALL_PAIRED_DEVICE, 0, NULL);
			}
			else
			{
				send_event_command_complete(0,UART_CMD_ERASE_ALL_PAIRED_DEVICE,APP_ErrCode_Command_Disallowed,NULL);
			}
		}
		break;
		
		case UART_CMD_READ_PAIRING_MODE: //0x0A
		{
			send_event_command_complete(1,UART_CMD_READ_PAIRING_MODE,0,&ioCapability);
		}
		break;
		
		case UART_CMD_WRITE_PAIRING_MODE: //0x0B
		{
			if(appGetIdleCount() == MAX_CONN_NBR)
			{
				XDATA u16 result;
				ioCapability=*(p_cmdPayload+2);

				result=BLE_SMP_Config(ioCapability,BLE_SMP_OPTION_BONDING|BLE_SMP_OPTION_SECURE_CONNECTION,BLE_SMP_SC_ONLY_DISABLE);
				send_event_command_complete(0,UART_CMD_WRITE_PAIRING_MODE,result,NULL);
			}
			else
			{
				send_event_command_complete(0,UART_CMD_WRITE_PAIRING_MODE,APP_ErrCode_Command_Disallowed,NULL);
			}
		}
		break;
		
		case UART_CMD_READ_ALL_PAIRED_DEVICE: //0x0C
		{
			XDATA u8 XDATA *p_list;

			p_list=APP_HEAP_Alloc((multiply_u16(MAX_DEVICE_NO, sizeof(APP_UTILITY_PairedInfo))+1));
			if(p_list!=NULL)
			{
				UTILITY_ReadAllPairedDevice(p_list, p_list+1);
				send_event_command_complete((multiply_u16((*(p_list)),sizeof(APP_UTILITY_PairedInfo))+1),UART_CMD_READ_ALL_PAIRED_DEVICE,0,p_list);
				APP_HEAP_Free(p_list);
			}
		}
		break;
		
		case UART_CMD_DELETE_PAIRED_DEVICE: //0x0D
			if(appGetIdleCount() == MAX_CONN_NBR)
			{
				XDATA u16 result;
  				result=UTILITY_DeleteSpecificPairedDevice(*(p_cmdPayload+1));	
   				send_event_command_complete(0,UART_CMD_DELETE_PAIRED_DEVICE,result,NULL);
			}
			else
			{
				send_event_command_complete(0,UART_CMD_DELETE_PAIRED_DEVICE,APP_ErrCode_Command_Disallowed,NULL);
			}
		break;
		
		case UART_CMD_READ_RSSI: /* 0x10 */
		{
			XDATA u8 connHandle;
			XDATA s8 rssi;
			connHandle = *(p_cmdPayload+1);			
				
			if(BLE_GAP_GetRSSI(connHandle, &rssi) == MBIOT_RES_SUCCESS)
				send_event_command_complete(1, UART_CMD_READ_RSSI, 0, &rssi);
			else
				send_event_command_complete(1, UART_CMD_READ_RSSI, APP_ErrCode_Command_Disallowed, NULL);
			
		}
		break;		

		case UART_CMD_WRITE_ADV_DATA: //0x11
		{
			if(appGetIdleCount() == MAX_CONN_NBR)
			{
				if((len-2)<=BLE_GAP_ADV_MAX_LENGTH)
				{
					XDATA u16 result;
					BLE_GAP_SetAdvertisingDataParas	XDATA advData;
					advData.adLen=len-2;
					xmemcpy(advData.adData,p_cmdPayload+2,advData.adLen);
					advData.adScanResLen=0;
					
					result=BLE_GAP_Set_AdvertisingData(&advData);				
					send_event_command_complete(0,UART_CMD_WRITE_ADV_DATA,result,NULL);	
				}
				else
				{
					send_event_command_complete(0,UART_CMD_WRITE_ADV_DATA,APP_ErrCode_Invalid_Parameters,NULL);
				}
			}
			else
			{
				send_event_command_complete(0,UART_CMD_DELETE_PAIRED_DEVICE,APP_ErrCode_Command_Disallowed,NULL);
			}	
		}
		break;
		
		case UART_CMD_SET_SCAN_ENABLE: /* 0x16 */
		{
			if((*(p_cmdPayload+1))==0)/* Scan Disable */
			{
				XDATA u16 result;
				result = BLE_GAP_Scanning_Stop();
				app_module_status = APP_STATE_IDLE;
				send_event_status(APP_STATE_IDLE);
				send_event_command_complete(0, UART_CMD_SET_SCAN_ENABLE, result, NULL);
			}
			else if((*(p_cmdPayload+1))==1)/* Scan Enable */
			{
				XDATA BLE_GAP_ScanningParas	scanParas;
				XDATA u16 result;
				
				scanParas.filterDuplicate = *(p_cmdPayload+2);
				scanParas.type = BLE_GAP_SCAN_TYPE_PASSIVE_SCAN;
				scanParas.interval = 0x0020;
				scanParas.window = 0x0020;
				result = BLE_GAP_Scanning_Start(&scanParas);
				
				send_event_command_complete(0, UART_CMD_SET_SCAN_ENABLE, result, NULL);
				if(result == MBIOT_RES_SUCCESS)
				{
					app_module_status=APP_STATE_SCANNING;
					send_event_status(APP_STATE_SCANNING);
				}			
			}
			else
			{
				send_event_command_complete(0,UART_CMD_SET_SCAN_ENABLE,APP_ErrCode_Invalid_Parameters,NULL);
			}
			
		}
		break;

		case UART_CMD_LE_CREATE_CONNECTION: /* 0x17 */
		{
			XDATA APP_CONN_STRUCT XDATA *p_conn = appGetstateConnInfo(APP_STATE_IDLE);
			if(p_conn != NULL)
			{
				XDATA BLE_GAP_CreateConnParas	paras;
				XDATA u16 result;
				
				paras.scan_interval = 0x0020;
				paras.scan_window = 0x0020;				
				paras.peer_addr.addr_type = *(p_cmdPayload+2);				
   				xmemcpy((u8 XDATA *)paras.peer_addr.addr, (u8 XDATA *)(p_cmdPayload+3), 6);
   				paras.conn_paras.interval_min = 0x0020;/*For multilink link establish  offset in time*/
   				paras.conn_paras.interval_max = 0x0020;
   				paras.conn_paras.latency = 0x0000;
   				paras.conn_paras.supervision_tmieout = 0x0100;

				result = BLE_GAP_CreateConnection(&paras);
				if(result == MBIOT_RES_SUCCESS) {
					p_conn->appState = APP_STATE_CONNECTING;
					send_event_status(app_module_status);/*Update Link status*/
				}
			}
			else
			{
				/*Connecttion link is more than MAX(4)*/
				send_event_command_complete(0,UART_CMD_LE_CREATE_CONNECTION,APP_ErrCode_Connection_Limit_Exceeded,NULL);
			}
		}
		break;

		case UART_CMD_LE_CREATE_CONNECTION_CANCEL: /* 0x18 */
		{
			XDATA APP_CONN_STRUCT XDATA *p_conn = appGetstateConnInfo(APP_STATE_CONNECTING);
			if(p_conn != NULL)
			{
				XDATA u16 result;
				result = BLE_GAP_CancelConnection();
					p_conn->appState = APP_STATE_IDLE;
				send_event_command_complete(0, UART_CMD_LE_CREATE_CONNECTION_CANCEL, result, NULL);
				send_event_status(app_module_status);/*Update Link status*/
			}
			else
			{
				send_event_command_complete(0,UART_CMD_LE_CREATE_CONNECTION_CANCEL,APP_ErrCode_Unknown_Connection_Identifier,NULL);
			}
		}
		break;
		
		case UART_CMD_DISCONNECT: /* 0x1B */
		{
			
			XDATA u8 connHandle = *(p_cmdPayload+1);			
			XDATA APP_CONN_STRUCT XDATA *p_conn = appGetConnInfo(connHandle);
			
			if(p_conn != NULL)
			{
				if((p_conn->appState == APP_STATE_LE_CONNECTED) || (p_conn->appState == APP_STATE_LE_OPEN)){
					XDATA u16 result;
					result=BLE_GAP_TerminateConnection(connHandle, BLE_GAP_DISCONNECT_REASON_REMOTE_TERMINATE);
					if(result == MBIOT_RES_SUCCESS){
						xmemset((u8 XDATA*)p_conn, 0, sizeof(APP_CONN_STRUCT));
						p_conn->appState = APP_STATE_IDLE;
						p_conn->attMTU = BLE_ATT_DEFAULT_MTU_LENGTH;	// BLE ATT Default MTU size
						p_conn->serviceState = QUERY_IDLE;
						p_conn->txCapacityState =WRITE_IDLE;
						Transparent_clear(connHandle);
						send_event_status(app_module_status);/*Update Link status*/
					}
					send_event_command_complete(0, UART_CMD_DISCONNECT, result, NULL);
				}
				else
				{
					send_event_command_complete(0,UART_CMD_DISCONNECT,APP_ErrCode_Command_Disallowed,NULL);
				}
				
			}
			else
			{
				send_event_command_complete(0,UART_CMD_DISCONNECT,APP_ErrCode_Unknown_Connection_Identifier,NULL);
			}
			
		}
		break;
		
		
		case UART_CMD_CONNECTION_PARA_UPDATE_REQ: //0x19
		{
			XDATA u8	connHandle;
			XDATA APP_CONN_STRUCT XDATA *p_conn = NULL;
			connHandle = *(p_cmdPayload+1);		
			p_conn = appGetConnInfo(connHandle);
			
			if((p_conn != NULL)&&((p_conn->appState == APP_STATE_LE_CONNECTED) || (p_conn->appState == APP_STATE_LE_OPEN)))
			{
				if(p_conn->role == BLE_GAP_ROLE_MASTER)
				{
					XDATA BLE_GAP_ConnParas paras;
					XDATA u16 result;
					
					paras.interval_min = (u16)((*(u8 XDATA*)(p_cmdPayload+2))<<8)+*(u8 XDATA*)(p_cmdPayload+3);
					paras.interval_max = paras.interval_min+0x08;
					paras.latency = (u16)((*(u8 XDATA*)(p_cmdPayload+4))<<8)+*(u8 XDATA*)(p_cmdPayload+5);
					paras.supervision_tmieout = (u16)((*(u8 XDATA*)(p_cmdPayload+6))<<8)+*(u8 XDATA*)(p_cmdPayload+7);
					result=BLE_GAP_UpdateConnPara(connHandle, &paras);
					if(result!=MBIOT_RES_SUCCESS)
					{
						send_event_command_complete(0, UART_CMD_CONNECTION_PARA_UPDATE_REQ, result, NULL);
					}
				}
				else
				{
					XDATA BLE_L2CAP_ConnParaUpdateReqPara para;
					XDATA u16 result;
									
					para.intervalMin = (u16)((*(u8 XDATA*)(p_cmdPayload+2))<<8)+*(u8 XDATA*)(p_cmdPayload+3);
					para.intervalMax = para.intervalMin+0x08;
					para.latency = (u16)((*(u8 XDATA*)(p_cmdPayload+4))<<8)+*(u8 XDATA*)(p_cmdPayload+5);
					para.timeout = (u16)((*(u8 XDATA*)(p_cmdPayload+6))<<8)+*(u8 XDATA*)(p_cmdPayload+7);					
					result=BLE_L2CAP_ConnParaUpdateReq(p_conn->handle, &para);
					if(result!=MBIOT_RES_SUCCESS)
					{
						send_event_command_complete(0, UART_CMD_CONNECTION_PARA_UPDATE_REQ, result, NULL);
					}
				}
			}
			else
			{
				send_event_command_complete(0, UART_CMD_CONNECTION_PARA_UPDATE_REQ, APP_ErrCode_Command_Disallowed, 0);
			}
		}
		break;

		case UART_CMD_SET_ADV_ENABLE: /* 0x1C */
		{
			XDATA u8 option;
			option = *(p_cmdPayload+1);
			
			/* 00: Leave Standby mode */
			if(option == 0x00)
			{
				XDATA APP_CONN_STRUCT XDATA *p_conn = appGetstateConnInfo(APP_STATE_STANDBY);
				if(p_conn != NULL)
				{
					XDATA u16 result;
					p_conn->appState = APP_STATE_IDLE;
					BLE_GAP_BeaconStop();
					result=BLE_GAP_Advertising_Stop();
					send_event_command_complete(0, UART_CMD_SET_ADV_ENABLE, result, NULL);
					send_event_status(app_module_status);/*Update Link status*/
				}
				else
				{
					send_event_command_complete(0, UART_CMD_SET_ADV_ENABLE, APP_ErrCode_Invalid_Parameters, NULL);
				}

				
			}
			else
			{

				/* 01: Enter Standby mode
				 81: Enter Standby mode with Beacon */
				if(option&0x01)
				{
					XDATA u16 result;
					XDATA APP_CONN_STRUCT XDATA *p_conn = appGetstateConnInfo(APP_STATE_IDLE);
					if(p_conn != NULL)
					{
						/*Just be one  at once*/					
						if(appGetstateConnInfo(APP_STATE_STANDBY) == NULL)
						{
							if(option&0x80)
								app_start_beacon();
							
							result = app_start_advertising();
							send_event_command_complete(0, UART_CMD_SET_ADV_ENABLE, result, NULL);
							if(result == MBIOT_RES_SUCCESS)
							{
								p_conn->appState = APP_STATE_STANDBY;
								send_event_status(app_module_status);/*Update Link status*/
							}
						}
						else
						{
							send_event_command_complete(0,UART_CMD_SET_ADV_ENABLE,APP_ErrCode_Command_Disallowed,NULL);
						}
					}
					else
					{
						send_event_command_complete(0,UART_CMD_SET_ADV_ENABLE,APP_ErrCode_Connection_Limit_Exceeded,NULL);
					}
					
				}
			}
		}		
		break;
		
		case UART_CMD_SHOW_ALL_LINK_STATUS: /* 0x1D */
		{
				XDATA u8 XDATA *return_pt = NULL;
				XDATA u8 i;
				return_pt = APP_HEAP_Alloc(4);
				if(return_pt == NULL)
					break;
				
				if(return_pt!=NULL)
				{				
					for(i=0;i<MAX_CONN_NBR;i++)
					{
						*(return_pt+i) = app_conn_list[i].appState;
					}
					send_event_command_complete(4, UART_CMD_SHOW_ALL_LINK_STATUS, 0, return_pt);
					APP_HEAP_Free(return_pt);
				}
				else
				{
					send_event_command_complete(0,UART_CMD_SHOW_ALL_LINK_STATUS,MBIOT_RES_OOM,NULL);
				}
			cmdProcessFlag=APP_PROCESS_CMD_ALLOW;
		}break;
		
		//GATT Server
		case UART_CMD_SEND_READ_BY_TYPE_RESPONSE: //0x2C
		{
			XDATA u16 errCode = MBIOT_RES_SUCCESS;
			XDATA u8 connHandle = *(p_cmdPayload+1);
			XDATA BLE_GATTS_SendReadByTypeResponseParams respParams;
			
			respParams.pairLength = *(p_cmdPayload+2);
			respParams.allPairsLength = (respParams.pairLength)*1;
			xmemcpy_u16(respParams.charValue, (u8 XDATA *)(p_cmdPayload+3), (len-3));
			
			errCode = BLE_GATTS_SendReadByTypeResponse(connHandle, &respParams);
			send_event_command_complete(0, UART_CMD_SEND_READ_BY_TYPE_RESPONSE, (u8)errCode, NULL);
		}
		break;
		
		//GATT Client
		case UART_CMD_HANDLE_VALUE_CONFIRMATION:	//0x2D
		{
			XDATA u16 errCode = MBIOT_RES_SUCCESS;
			XDATA u8 connHandle = *(p_cmdPayload+1);
			
			errCode = BLE_GATTC_HandleValueConfirm(connHandle);
			send_event_command_complete(0, UART_CMD_HANDLE_VALUE_CONFIRMATION, (u8)errCode, NULL);
		}
		break;
		
		case UART_CMD_DISCOVER_ALL_CHARACTERISTICS: //0x2E
		{
			XDATA u8	connHandle, index;
			connHandle = *(p_cmdPayload+1);			
			index=appGetConnList(connHandle);
			
			if(index<MAX_CONN_NBR)
			{
				XDATA u16 errCode = MBIOT_RES_SUCCESS;
				XDATA u16 startHandle = (u16)((*(u8 XDATA*)(p_cmdPayload+2))<<8)+*(u8 XDATA*)(p_cmdPayload+3);
				XDATA u16 endHandle = (u16)((*(u8 XDATA*)(p_cmdPayload+4))<<8)+*(u8 XDATA*)(p_cmdPayload+5);
				
				errCode = BLE_GATTC_DiscoverAllCharacteristics(connHandle, startHandle, endHandle);
				currentCmd = UART_CMD_DISCOVER_ALL_CHARACTERISTICS;
				//Sync with BLEDK3. Don't send command complete here.
				//send_event_command_complete(0, UART_CMD_DISCOVER_ALL_CHARACTERISTICS, (u8)errCode, NULL);
			}
			else
			{
				send_event_command_complete(0, UART_CMD_DISCOVER_ALL_CHARACTERISTICS, APP_ErrCode_Command_Disallowed, 0);
			}
		}
		break;
		
		case UART_CMD_DISCOVER_ALL_DESCRIPTORS:	//0x2F
		{
			XDATA u8	connHandle, index;
			connHandle = *(p_cmdPayload+1);			
			index=appGetConnList(connHandle);
			
			if(index<MAX_CONN_NBR)
			{
				XDATA u16 errCode = MBIOT_RES_SUCCESS;
				XDATA u16 startHandle = (u16)((*(u8 XDATA*)(p_cmdPayload+2))<<8)+*(u8 XDATA*)(p_cmdPayload+3);
				XDATA u16 endHandle = (u16)((*(u8 XDATA*)(p_cmdPayload+4))<<8)+*(u8 XDATA*)(p_cmdPayload+5);
				
				errCode = BLE_GATTC_DiscoverAllDescriptors(connHandle, startHandle, endHandle);
				currentCmd = UART_CMD_DISCOVER_ALL_DESCRIPTORS;
				//Sync with BLEDK3. Don't send command complete here.
				//send_event_command_complete(0, UART_CMD_DISCOVER_ALL_DESCRIPTORS, (u8)errCode, NULL);
			}
			else
			{
				send_event_command_complete(0, UART_CMD_DISCOVER_ALL_DESCRIPTORS, APP_ErrCode_Command_Disallowed, 0);
			}
		}
		break;
		
		case UART_CMD_DISCOVER_ALL_PRIMARY_SERVICES: //0x30
		{
			XDATA u8	connHandle, index;
			connHandle = *(p_cmdPayload+1);			
			index=appGetConnList(connHandle);
			
			if(index<MAX_CONN_NBR)
			{
				XDATA u16 errCode = MBIOT_RES_SUCCESS;
				
				errCode = BLE_GATTC_DiscoverAllPrimaryServices(connHandle);
				if(errCode != MBIOT_RES_SUCCESS)
				{
					send_event_command_complete(0, UART_CMD_DISCOVER_ALL_PRIMARY_SERVICES, (u8)errCode, NULL);
				}
				else
				{
					currentCmd = UART_CMD_DISCOVER_ALL_PRIMARY_SERVICES;
					//Sync with BLEDK3. Don't send command complete here.
					//send_event_command_complete(0, UART_CMD_DISCOVER_ALL_PRIMARY_SERVICES, (u8)errCode, NULL);
				}
			}
			else
			{
				send_event_command_complete(0, UART_CMD_DISCOVER_ALL_PRIMARY_SERVICES, APP_ErrCode_Command_Disallowed, 0);
			}
		}
		break;
		
		case UART_CMD_DISCOVER_PRIMARY_SERVICE_BY_UUID:	//0x31
		{
			XDATA u8	connHandle, index;
			connHandle = *(p_cmdPayload+1);
			index=appGetConnList(connHandle);
			
			if(index<MAX_CONN_NBR)
			{
				XDATA u16 errCode = MBIOT_RES_SUCCESS;
				XDATA BLE_GATTC_DiscoverPrimaryServiceByUuidParams discParams;
				/*Get remote gatt table*/
				if((len==DISCOVER_PRIM_SER_BY_UUID_4)||(len==DISCOVER_PRIM_SER_BY_UUID_12))
				{
					XDATA u8 i=0;
					manualpattern_procedure|=GET_REMOTE_TABLE_STEP1;
					if(len==DISCOVER_PRIM_SER_BY_UUID_12)
						manualpattern_procedure|=DISCOVER_PRIM_SER_BY_UUID_12;
					discParams.startHandle = APP_START_HANDLE;
					discParams.endHandle = APP_END_HANDLE;
					discParams.valueLength = (len-2);
					
					for(i=0;i<len-2;i++)
					{
						discParams.value[i] = *(u8 XDATA *)(p_cmdPayload+len-1-i);
					}
					errCode = BLE_GATTC_DiscoverPrimaryServiceByUUID(connHandle, &discParams);
					currentCmd = UART_CMD_DISCOVER_PRIMARY_SERVICE_BY_UUID;
				}
				else
				{	/*Normal Case*/
					discParams.startHandle = (u16)((*(p_cmdPayload+2))<<8)+*(p_cmdPayload+3);
					discParams.endHandle = (u16)((*(p_cmdPayload+4))<<8)+*(p_cmdPayload+5);
					discParams.valueLength = (len-6);
					xmemcpy((u8 XDATA *)&discParams.value[0], (u8 XDATA*)(p_cmdPayload+6), len-6);
					errCode = BLE_GATTC_DiscoverPrimaryServiceByUUID(connHandle, &discParams);
					currentCmd = UART_CMD_DISCOVER_PRIMARY_SERVICE_BY_UUID;
					//Sync with BLEDK3. Don't send command complete here.
					//send_event_command_complete(0, UART_CMD_DISCOVER_PRIMARY_SERVICE_BY_UUID, (u8)errCode, NULL);
				}
			}
			else
			{
				send_event_command_complete(0, UART_CMD_DISCOVER_PRIMARY_SERVICE_BY_UUID, APP_ErrCode_Command_Disallowed, 0);
			}
		}
		break;
		
		case UART_CMD_READ_CHARACTERISTIC_VALUE: //0x32
		{
			XDATA u8	connHandle, index;
			connHandle = *(p_cmdPayload+1);
			index=appGetConnList(connHandle);
			
			if(index<MAX_CONN_NBR)
			{				
				XDATA u16	char_value_handle, errCode, valueOffset;

				if(len==DISCOVER_PRIM_SER_BY_UUID_4)
				{/*Get remote GATT table*/
					manualpattern_procedure|=GET_REMOTE_TABLE_STEP2;
					char_value_handle = (u16)((*(p_cmdPayload+2))<<8)+*(p_cmdPayload+3);
					valueOffset = 0x00;
					errCode = BLE_GATTC_Read(connHandle, char_value_handle, valueOffset);
					currentCmd = UART_CMD_READ_CHARACTERISTIC_VALUE;
				}
				else
				{
					char_value_handle = (u16)((*(p_cmdPayload+2))<<8)+*(p_cmdPayload+3);
					valueOffset = (u16)((*(p_cmdPayload+4))<<8)+*(p_cmdPayload+5);
					errCode = BLE_GATTC_Read(connHandle, char_value_handle, valueOffset);
					send_event_command_complete(0, UART_CMD_READ_CHARACTERISTIC_VALUE, (u8)errCode, NULL);
				}
			}
			else
			{
				send_event_command_complete(0, UART_CMD_READ_CHARACTERISTIC_VALUE, APP_ErrCode_Command_Disallowed, 0);
			}
		}
		break;

		case UART_CMD_READ_USING_CHARACTERISTIC_UUID: //0x33
		{
			XDATA u8	connHandle, index;
			connHandle = *(p_cmdPayload+1);			
			index=appGetConnList(connHandle);
			
			if(index<MAX_CONN_NBR)
			{
				XDATA BLE_GATTC_ReadByTypeParams readParams;
				XDATA u16 errCode = MBIOT_RES_SUCCESS;
				
				readParams.startHandle = (u16)((*(p_cmdPayload+2))<<8)+*(p_cmdPayload+3);
				readParams.endHandle = (u16)((*(p_cmdPayload+4))<<8)+*(p_cmdPayload+5);
				readParams.attrTypeLength = (len-6);
				xmemcpy((u8 XDATA *)&readParams.attrType[0], (u8 XDATA*)(p_cmdPayload+6), len-6);
				errCode = BLE_GATTC_ReadUsingUUID(connHandle, &readParams);
				currentCmd = UART_CMD_READ_USING_CHARACTERISTIC_UUID;
				//Sync with BLEDK3. Don't send command complete here.
				//send_event_command_complete(0, UART_CMD_READ_USING_CHARACTERISTIC_UUID, (u8)errCode, NULL);
			}
			else
			{
				send_event_command_complete(0, UART_CMD_READ_USING_CHARACTERISTIC_UUID, APP_ErrCode_Command_Disallowed, 0);
			}
		}
		break;
		
		case UART_CMD_WRITE_CHARACTERISTIC_VALUE: //0x34
		{
			XDATA u8	connHandle, index;
			connHandle = *(p_cmdPayload+1);			
			index=appGetConnList(connHandle);
			
			if(index<MAX_CONN_NBR)
			{
				XDATA u8 type = *(p_cmdPayload+2);
				XDATA u16 errCode = MBIOT_RES_SUCCESS;
				XDATA u16 charHandle = (u16)((*(u8 XDATA*)(p_cmdPayload+3))<<8)+*(u8 XDATA*)(p_cmdPayload+4);
				XDATA u16 valueOffset = (u16)((*(u8 XDATA*)(p_cmdPayload+5))<<8)+*(u8 XDATA*)(p_cmdPayload+6);
				XDATA u8 flags = *(u8 XDATA*)(p_cmdPayload+7);
				
				if((type!=BLE_GATT_TYPE_WRITE_REQ)&&(type!=BLE_GATT_TYPE_WRITE_CMD)&&(type!=BLE_GATT_TYPE_PREP_WRITE_REQ)&&(type!=BLE_GATT_TYPE_EXEC_WRITE_REQ))
				{
					send_event_command_complete(0, UART_CMD_WRITE_CHARACTERISTIC_VALUE, MBIOT_RES_INVALID_PARA, NULL);
				}
				else
				{
					XDATA BLE_GATTC_WriteParams XDATA *p_params;
					p_params = (BLE_GATTC_WriteParams XDATA *)APP_HEAP_Alloc(sizeof(BLE_GATTC_WriteParams));
					if(p_params!=NULL)
					{
						p_params->charHandle = charHandle;
						p_params->charLength = (len-8);
						p_params->writeType = type;
						p_params->valueOffset = valueOffset;
						p_params->flags = flags;
						xmemcpy_u16(p_params->charValue, (u8 XDATA *)(p_cmdPayload+8), (len-8));
						errCode = BLE_GATTC_Write(connHandle, p_params);
						send_event_command_complete(0, UART_CMD_WRITE_CHARACTERISTIC_VALUE, (u8)errCode, NULL);
						APP_HEAP_Free((u8 XDATA *)p_params);
					}
					else
					{
						send_event_command_complete(0, UART_CMD_WRITE_CHARACTERISTIC_VALUE, MBIOT_RES_OOM, 0);
					}
				}
			}
			else
			{
				send_event_command_complete(0, UART_CMD_WRITE_CHARACTERISTIC_VALUE, APP_ErrCode_Command_Disallowed, 0);
			}
		}
		break;

		case UART_CMD_ENABLE_TRANSPARENT:/*0x35*/
		{
			XDATA APP_CONN_STRUCT XDATA *p_conn = NULL;
			XDATA u8 connHandle = *(p_cmdPayload+1);
			p_conn = appGetConnInfo(connHandle);
			if(p_conn != NULL)
			{
				XDATA u16 errCode = MBIOT_RES_SUCCESS;
				
				if(*(p_cmdPayload+2)) /*enable transparent*/
				{
					if(p_conn->appState == APP_STATE_LE_CONNECTED)
					{
						XDATA u16 startHandle = APP_START_HANDLE;
						XDATA u16 endHandle = APP_END_HANDLE;
						
						errCode = BLE_GATTC_DiscoverAllCharacteristics(p_conn->handle, startHandle, endHandle);
						if(errCode == MBIOT_RES_SUCCESS)
						p_conn->serviceState = QUERY_TX_CHAR;
						p_conn->Transparent_Rule = *(p_cmdPayload+3);
					}
					else
					{
						send_event_command_complete(0, UART_CMD_ENABLE_TRANSPARENT, APP_ErrCode_Command_Disallowed, NULL);
						break;
					}
				}
				else/*disable transparent*/
				{
					XDATA u16 result = MBIOT_RES_SUCCESS;
					if(p_conn->appState == APP_STATE_LE_OPEN)
						result = app_master_ctrl_data_receive(CLIENT_CHAR_CONFIG_DESCRIPTOR_DISABLE, p_conn);	

					if(result == MBIOT_RES_SUCCESS)
					{
						p_conn->serviceState = QUERY_IDLE;
						p_conn->appState = APP_STATE_LE_CONNECTED;
						p_conn->txCapacityState =WRITE_IDLE;
						send_event_status(app_module_status);/*Update Link status*/
					}
				}
				send_event_command_complete(0, UART_CMD_ENABLE_TRANSPARENT, errCode, NULL);
			}
			else
			{
				send_event_command_complete(0, UART_CMD_ENABLE_TRANSPARENT, APP_ErrCode_Unknown_Connection_Identifier, NULL);
			}
		}	
		break;
		
		case UART_CMD_MULTI_LINK_CONTROL:/*For Multi-link send data simultaneously*/
		{
			TransPolling  = *(p_cmdPayload+1);
			send_event_command_complete(0, UART_CMD_MULTI_LINK_CONTROL, 0, NULL);
		}
		break;
		
		case UART_CMD_SEND_TRANSPARENT_DATA:/*0x3F*/
		{
			XDATA APP_CONN_STRUCT XDATA *p_conn = NULL;
			XDATA u8 connHandle = *(p_cmdPayload+1);
			XDATA u8 i = 0, nonTS=0;
			XDATA u8 response = TRANS_REPLAY;
			indexreTrans=0;
			
			do{
				if(TransPolling)
				{
					i++;
					if(app_conn_list[i-1].appState == APP_STATE_LE_OPEN)
					{
						connHandle = app_conn_list[i-1].handle;
					}
					else
					{
						nonTS++;
						if(nonTS >= MAX_CONN_NBR)
						{
							send_event_command_complete(0, UART_CMD_SEND_TRANSPARENT_DATA, APP_ErrCode_Command_Disallowed, NULL);
							break;
						}
						else
						{
							continue;
						}
					}
				}
			p_conn = appGetConnInfo(connHandle);
			if(p_conn != NULL)
			{
				if(p_conn->appState == APP_STATE_LE_OPEN)
				{
					XDATA u16 errCode = MBIOT_RES_SUCCESS;
					if(p_conn->role_att == BLE_GATT_ROLE_CLIENT)
					{
						XDATA BLE_GATTC_WriteParams  wParams;
						
						wParams.charHandle=p_conn->remoteHandleList.transRx;
						wParams.charLength=len-2;
						xmemcpy_u16(wParams.charValue, (p_cmdPayload+2), wParams.charLength);
						if(p_conn->Transparent_Rule)
							wParams.writeType=BLE_GATT_TYPE_WRITE_CMD;
						else
							wParams.writeType=BLE_GATT_TYPE_WRITE_REQ;
						
						wParams.valueOffset=0x0000;
						wParams.flags=0;
						
						errCode = BLE_GATTC_Write(p_conn->handle, &wParams);
						
						if(p_conn->Transparent_Rule)
						{/*Write Cmd*/
							if( errCode != MBIOT_RES_SUCCESS)
							{
								reTrans[indexreTrans].p_reTransData = (u8 XDATA*)APP_HEAP_Alloc(sizeof(BLE_GATTC_WriteParams));
								if(reTrans[indexreTrans].p_reTransData != NULL)
								{
									/*Resend Transparent Data.*/
									reTrans[indexreTrans].transConnHandle=p_conn->handle;
									reTrans[indexreTrans].transTxMode=BLE_GATT_TYPE_WRITE_CMD;
									xmemcpy_u16(reTrans[indexreTrans].p_reTransData, (u8 XDATA*)&wParams, sizeof(BLE_GATTC_WriteParams));	
									response = TRANS_RETRY;
									indexreTrans++;
								}
							}
							
						}
						else
						{	/*Write Req*/
							response = TRANS_NOREPLAY;
							if( errCode != MBIOT_RES_SUCCESS)
								send_event_command_complete(0, UART_CMD_SEND_TRANSPARENT_DATA, errCode, NULL);
							/*Wait for Write Response*/
						}
					}
					else
					{
						XDATA BLE_GATTS_HandleValueParams  hvParams;

						hvParams.charHandle = localHandleList.transTx;
						hvParams.charLength = len-2;
						xmemcpy_u16(hvParams.charValue, (p_cmdPayload+2), hvParams.charLength);
						hvParams.sendType = BLE_GATT_HV_NOTIFICATION;
						errCode = BLE_GATTS_SendHandleValue(p_conn->handle, &hvParams);
						if(errCode != MBIOT_RES_SUCCESS)
						{
							
							reTrans[indexreTrans].p_reTransData = (u8 XDATA*)APP_HEAP_Alloc(sizeof(BLE_GATTS_HandleValueParams));
							if(reTrans[indexreTrans].p_reTransData != NULL)
							{
								/*Resend Transparent Data.*/
								reTrans[indexreTrans].transConnHandle=p_conn->handle;
								reTrans[indexreTrans].transTxMode=BLE_GATT_HV_NOTIFICATION;
								xmemcpy_u16(reTrans[indexreTrans].p_reTransData,  (u8 XDATA*)&hvParams, sizeof(BLE_GATTS_HandleValueParams));
								response = TRANS_RETRY;
								indexreTrans++;
							}
						}
						
					}
				}
				else
				{
					send_event_command_complete(0, UART_CMD_SEND_TRANSPARENT_DATA, APP_ErrCode_Command_Disallowed, NULL);
				}
			}
			else
			{
				send_event_command_complete(0, UART_CMD_SEND_TRANSPARENT_DATA, APP_ErrCode_Unknown_Connection_Identifier, NULL);
				break;
			}

			}while(TransPolling&&(i<MAX_CONN_NBR));/*do-while*/

			if(response==TRANS_REPLAY)
				send_event_command_complete(0, UART_CMD_SEND_TRANSPARENT_DATA, 0, NULL);
			else if(response==TRANS_RETRY)
				APP_TIMER_FastTimer_Start(APP_TIMER_ID_3, 100);
		}
		break;
		
		//GATT Server
		case UART_CMD_SEND_ERROR_RESPONSE:	//0x37
		{
			XDATA u16 errCode = MBIOT_RES_SUCCESS;
			XDATA u8 connHandle = *(p_cmdPayload+1);
			XDATA BLE_GATTS_SendErrResponseParams errParams;
			
			errParams.reqOpcode = *(p_cmdPayload+2);
			errParams.charHandle = (u16)((*(p_cmdPayload+3))<<8) + *(p_cmdPayload+4);
			errParams.errorCode = *(p_cmdPayload+5);
			errCode = BLE_GATTS_SendErrorResponse(connHandle, &errParams);
			send_event_command_complete(0, UART_CMD_SEND_ERROR_RESPONSE, (u8)errCode, NULL);
		}
		break;
		
		case UART_CMD_SEND_HANDLE_VALUE: //0x38
		{
			XDATA u8	connHandle, index;
			connHandle = *(p_cmdPayload+1);			
			index=appGetConnList(connHandle);
			
			if(index<MAX_CONN_NBR)
			{
				XDATA u16 propertyLength = 1, errCode;
				BLE_GATTS_HandleValueParams XDATA p_hvParams;
				
				// Service must know its property of attribute
				p_hvParams.sendType = *(p_cmdPayload+2);
				p_hvParams.charHandle = (u16)((*(p_cmdPayload+3))<<8)+*(p_cmdPayload+4);
				p_hvParams.charLength = (len-5);
				xmemcpy_u16(p_hvParams.charValue, (u8 XDATA*)(p_cmdPayload+5), p_hvParams.charLength);
				errCode = BLE_GATTS_SendHandleValue(connHandle, &p_hvParams);
				send_event_command_complete(0, UART_CMD_SEND_HANDLE_VALUE, (u8)errCode, NULL);
			}
			else
			{
				send_event_command_complete(0, UART_CMD_SEND_HANDLE_VALUE, APP_ErrCode_Command_Disallowed, 0);
			}
		}
		break;
		
		case UART_CMD_SET_HANDLE_VALUE:	//0x39
		{
			XDATA u16 errCode = MBIOT_RES_SUCCESS;
			if((len-3)<=BLE_ATT_ATTRIBUTE_VALUE_LEN)
			{
				XDATA u16 attrHandle = (u16)((*(p_cmdPayload+1))<<8) + *(p_cmdPayload+2);
				errCode = BLE_GATTS_SetHandleValue(attrHandle, (len-3), (u8 XDATA *)(p_cmdPayload+3));
				send_event_command_complete(0, UART_CMD_SET_HANDLE_VALUE, (u8)errCode, NULL);
			}
			else
			{
				send_event_command_complete(0, UART_CMD_SET_HANDLE_VALUE, APP_ErrCode_Invalid_Parameters, NULL);
			}
		}
		break;
		
		case UART_CMD_GET_HANDLE_VALUE:	//0x3A
		{
			XDATA u16 errCode = MBIOT_RES_SUCCESS, attrHandle;
			XDATA u8 attrLength = BLE_ATT_ATTRIBUTE_VALUE_LEN;	// fixed length of default MTU size
			XDATA u8 XDATA  *readBuffer;
			
			readBuffer = (u8 XDATA *)APP_HEAP_Alloc(attrLength);
			if(readBuffer!=NULL)
			{
				if(len==3)
				{
					attrHandle = (u16)((*(p_cmdPayload+1))<<8) + *(p_cmdPayload+2);
					errCode = BLE_GATTS_GetHandleValue(attrHandle, &attrLength, readBuffer);
					if(errCode != MBIOT_RES_SUCCESS)
					{
						attrLength = 0;
					}
					send_event_command_complete(attrLength, UART_CMD_GET_HANDLE_VALUE, (u8)errCode, readBuffer);
				}
				else
				{
					send_event_command_complete(0, UART_CMD_GET_HANDLE_VALUE, APP_ErrCode_Invalid_Parameters, NULL);
				}
				APP_HEAP_Free(readBuffer);
			}
			else
			{
				send_event_command_complete(0, UART_CMD_GET_HANDLE_VALUE, MBIOT_RES_OOM, NULL);
			}
		}
		break;
		
		case UART_CMD_SEND_WRITE_RESPONSE:	//0x3D
		{
			XDATA u16 errCode = MBIOT_RES_SUCCESS;
			XDATA u8 connHandle = *(p_cmdPayload+1);
			XDATA u8 writeType = *(p_cmdPayload+2);
			
			if((writeType==BLE_GATT_TYPE_WRITE_RESP)||(writeType==BLE_GATT_TYPE_PREP_WRITE_RESP)||(writeType==BLE_GATT_TYPE_EXEC_WRITE_RESP))
			{
				XDATA BLE_GATTS_SendWriteResponseParams respParams;
				
				respParams.responseType = writeType;
				respParams.charHandle = (u16)((*(p_cmdPayload+3))<<8) + *(p_cmdPayload+4);
				respParams.valueOffset = (u16)((*(p_cmdPayload+5))<<8) + *(p_cmdPayload+6);
				respParams.charLength = (len-7);
				if((len-7)>0)
				{
					xmemcpy_u16(respParams.charValue, (p_cmdPayload+7), (len-7));
				}
				errCode = BLE_GATTS_SendWriteResponse(connHandle, &respParams);
			}
			else
			{
				errCode = MBIOT_RES_INVALID_PARA;
			}
			send_event_command_complete(0, UART_CMD_SEND_WRITE_RESPONSE, (u8)errCode, NULL);
		}
		break;
		
		case UART_CMD_SEND_READ_RESPONSE:	//0x3E
		{
			XDATA u16 errCode = MBIOT_RES_SUCCESS;
			XDATA u8 connHandle = *(p_cmdPayload+1);
			XDATA BLE_GATTS_SendReadResponseParams respParams;
				
			respParams.responseType = *(p_cmdPayload+2);
			xmemcpy_u16(respParams.charValue, (u8 XDATA *)(p_cmdPayload+3), (len-3));
			respParams.charLength = (len-3);
			errCode = BLE_GATTS_SendReadResponse(connHandle, &respParams);
			send_event_command_complete(0, UART_CMD_SEND_READ_RESPONSE, (u8)errCode, NULL);
		}
		break;
		
		case UART_CMD_PASSKEY_ENTRY_RES: //0x40
		{
			XDATA u8	connHandle;
			XDATA APP_CONN_STRUCT XDATA *p_conn;
			connHandle = *(p_cmdPayload+1);	
			p_conn = appGetConnInfo(connHandle);
			
			if(p_conn != NULL)	
			{
				XDATA u8 notification_type, entered_passkey;
				notification_type = *(p_cmdPayload+2);
				entered_passkey = *(p_cmdPayload+3);			
				
 				if(notification_type==NOTIFICATION_TYPE_PASSKEY_ENTERED)
				{
					if(p_conn->passkeyIndex<6)
					{
						p_conn->passkey[p_conn->passkeyIndex]=entered_passkey;
						p_conn->passkeyIndex++;
					}
				}
				else if(notification_type==NOTIFICATION_TYPE_PASSKEY_ERASED)
				{
					if(p_conn->passkeyIndex)
					{
						p_conn->passkeyIndex--;
					}
				}
				else if(notification_type==NOTIFICATION_TYPE_PASSKEY_CLEARED)
				{
					p_conn->passkeyIndex=0;
				}
				else if(notification_type==NOTIFICATION_TYPE_PASSKEY_COMPLETED)
				{
					BLE_SMP_PasskeyReply(connHandle,p_conn->passkey);
				}
			}
			else
			{
				send_event_command_complete(0, UART_CMD_PASSKEY_ENTRY_RES, APP_ErrCode_Command_Disallowed, 0);
			}
		}
		break;
		
		case UART_CMD_DISPLAY_YESNO_RES://0x41
		{
			XDATA APP_CONN_STRUCT XDATA *p_conn = NULL;
			XDATA u8	connHandle;
			connHandle = *(p_cmdPayload+1);	
			p_conn = appGetConnInfo(connHandle);
			if(p_conn != NULL)
			{
				XDATA u16 result;
				if(p_conn->numericComparisonConfirmEnable)
				{
					if((*(p_cmdPayload+2))==0x00)
					{
						result=BLE_SMP_NumericComparisonConfirmReply(connHandle,BLE_SMP_CONFIRM_YES);
					}
					else
					{
						result=BLE_SMP_NumericComparisonConfirmReply(connHandle,BLE_SMP_CONFIRM_NO);
					}
				}
				else
				{
					if(ioCapability==BLE_SMP_IO_DISPLAYYESNO)
					{
						if((*(p_cmdPayload+2))==0x00)
						{
							result=BLE_SMP_PasskeyReply(connHandle,app_passkey);
						}
						else
						{
							result=BLE_SMP_PasskeyNegativeReply(connHandle);
						}
					}
				}
				send_event_command_complete(0,UART_CMD_DISPLAY_YESNO_RES,result,NULL);
			}
			else
			{
				send_event_command_complete(0, UART_CMD_DISPLAY_YESNO_RES, APP_ErrCode_Command_Disallowed, 0);
			}
		}
		break;
		
		case UART_CMD_PAIRING_REQUEST:
		{			
			XDATA u8	connHandle;
			connHandle = *(p_cmdPayload+1);			

			if(appGetConnInfo(connHandle) != NULL)
			{
				XDATA u16 result;
				result = BLE_SMP_InitiatePairing(connHandle);
				send_event_command_complete(0, UART_CMD_PAIRING_REQUEST, result, NULL);
			}
			else
			{
				send_event_command_complete(0, UART_CMD_PAIRING_REQUEST, APP_ErrCode_Command_Disallowed, 0);
			}

		}
		break;
		
#ifdef UART_CMD_PERIPHERAL_API_TEST
		case UART_CMD_PERIPHERAL_API_TEST:
		{
			XDATA u8 api_type;
			XDATA u8 revalue;
			
			/* Enter SPI Master or slave mode */ 
			if(spi_working > 0)
			{
				XDATA u16 result;
				//goto SPI_TEST_VERIFICATION;
				result = spi_processdata(len, p_cmdPayload, &revalue);
				PeripheralVerifyEnd(revalue, result);
				break;
			}
			if(len < 1)
			{
				send_event_command_complete(0, UART_CMD_PERIPHERAL_API_TEST,
												APP_ErrCode_Unknown_Command, 0);
				break; 
			}
				
			api_type = *(p_cmdPayload+1);
			/* ADC Verification */
			if(api_type == PERIPHERAL_API_TEST_ADC)
			{
				app_ADC_Verification(p_cmdPayload);
			}
			else if(api_type == PERIPHERAL_API_TEST_PWM)
			{
			/* PWM Verification */
				app_PWM_Verification(p_cmdPayload);
				
			}
			else if(api_type == PERIPHERAL_API_TEST_LED)
			{
			/* LED Verification */
				app_LED_Verification(p_cmdPayload);
			}
			else if(api_type == PERIPHERAL_API_TEST_GPIO)
			{
			/* GPIO Verification */
				if(len < 3)
				{
					send_event_command_complete(0, UART_CMD_PERIPHERAL_API_TEST, 
													APP_ErrCode_Unknown_Command, 0);
					break;
				}
				app_GPIO_Verification(p_cmdPayload);
			}
			else if(api_type == PERIPHERAL_API_TEST_SPI_Master)
			{
				/* SPI Master Verification */
				XDATA u16 result = 0;
				/* configure SPI Master */
				result = app_SPI_Master_Verification(APP_SPI_CONFIG, len, p_cmdPayload);
				revalue = api_type;
				PeripheralVerifyEnd(revalue, result);
				break;
			}
			else if(api_type == PERIPHERAL_API_TEST_SPI_Slave)
			{
			/* SPI Slave Verification */
				XDATA u16 result = 0;
				/* configure SPI Slave */
				result = app_SPI_Slave_Verification(APP_SPI_CONFIG, 0, p_cmdPayload);
				revalue = api_type;
				PeripheralVerifyEnd(revalue, result);
				break;
			}
			else if(api_type == PERIPHERAL_API_TEST_LOWPOWER_OPERATION)
			{/*Low power Operation for SPI Master or Slave Verification*/
				app_LowPower_Verification(p_cmdPayload);
			}
			else if(api_type == PERIPHERAL_API_TEST_I2C)
			{
			/* I2C Verification */
				app_I2C_Verification(p_cmdPayload);
			} 
			else if(api_type == PERIPHERAL_API_TEST_SPI_Serial_Flash)
			{	
			/*SPI Serial Flash Verification */
				 app_SPI_Serial_Flash_Verification(p_cmdPayload);
			}
			
			if((api_type >= PERIPHERAL_API_TEST_ADC) && (api_type <= PERIPHERAL_API_TEST_LOWPOWER_OPERATION))
				break;
			else
				send_event_command_complete(0, UART_CMD_PERIPHERAL_API_TEST, 
													APP_ErrCode_Unknown_Command, 0);
			
		}
		break;
		
		case UART_CMD_PERIPHERAL_API_TEST2:
		{
			spi_masterId = *(p_cmdPayload+1);
			send_event_command_complete(0, UART_CMD_PERIPHERAL_API_TEST2, 0, 0);
		}break;
#endif /* UART_CMD_PERIPHERAL_API_TEST */
		
		default:
		{
			send_event_command_complete(0,RX_OPCode,APP_ErrCode_Unknown_Command,0);
		}
		break;
	}

	APP_HEAP_Free(p_cmdPayload);
}

static void app_init(void) large reentrant
{
	XDATA u8 i;
	/*Record Connection Link Status*/
	for(i=0;i<MAX_CONN_NBR;i++)
	{
		xmemset((u8 XDATA*)&app_conn_list[i],0,sizeof(APP_CONN_STRUCT));
		app_conn_list[i].appState = APP_STATE_IDLE;
		app_conn_list[i].attMTU = BLE_ATT_DEFAULT_MTU_LENGTH;	// BLE ATT Default MTU size
		app_conn_list[i].serviceState = QUERY_IDLE;
		app_conn_list[i].txCapacityState =WRITE_IDLE;
	}
	/*Record Module Status*/
	app_module_status = APP_STATE_IDLE;
	
	currentCmd=0;
	cmdProcessFlag=APP_PROCESS_CMD_ALLOW;
	ioCapability=BLE_SMP_IO_NOINPUTNOOUTPUT;

	send_event_status(APP_STATE_IDLE);
	//app_start_advertising();
}


PUBLIC void gap_event_handler(BLE_GAP_Event XDATA * p_event) large reentrant
{
	switch(p_event->eventID)
	{
		case BLE_GAP_EVT_CONNECTED:
		{
			XDATA APP_CONN_STRUCT XDATA *p_conn = NULL;
			if(p_event->eventField.evtConnect.role == BLE_GAP_ROLE_SLAVE)
				p_conn = appGetstateConnInfo(APP_STATE_STANDBY);
			else	
				p_conn = appGetstateConnInfo(APP_STATE_CONNECTING);
			if(p_conn != NULL) {
				p_conn->handle = p_event->eventField.evtConnect.connHandle;
				p_conn->bondedDevice = p_event->eventField.evtConnect.bondedDevice;
				p_conn->remoteAddr = p_event->eventField.evtConnect.remoteAddr;
				p_conn->connPara = p_event->eventField.evtConnect.connPara;
				p_conn->role = p_event->eventField.evtConnect.role;
				p_conn->appState = APP_STATE_LE_CONNECTED;

				send_event_connection_complete(0,  p_conn);
				send_event_status(app_module_status);/*Update Link status*/		
			}		
		}
		break;
		
		case BLE_GAP_EVT_DISCONNECTED:
		{
			XDATA APP_CONN_STRUCT XDATA *p_conn = appGetConnInfo(p_event->eventField.evtDisconnect.connHandle);
			
			if(p_conn != NULL)
			{
				xmemset((u8 XDATA*)p_conn, 0, sizeof(APP_CONN_STRUCT));
				p_conn->appState = APP_STATE_IDLE;
				p_conn->attMTU = BLE_ATT_DEFAULT_MTU_LENGTH;	// BLE ATT Default MTU size
				p_conn->serviceState = QUERY_IDLE;
				p_conn->txCapacityState =WRITE_IDLE;
				Transparent_clear(p_event->eventField.evtDisconnect.connHandle);
				send_event_status(app_module_status);/*Update Link status*/
			}
			send_event_disconnection_complete(p_event->eventField.evtDisconnect.connHandle, p_event->eventField.evtDisconnect.reason);
		}
		break;
		
		case BLE_GAP_EVT_CONN_PARA_UPDATE:
		{
			if(p_event->eventField.evtConnParaUpdate.status == 0)
			{
				XDATA APP_CONN_STRUCT XDATA *p_conn = NULL;	
				p_conn = appGetConnInfo(p_event->eventField.evtConnParaUpdate.connHandle);
				
				if(p_conn != NULL)
				{
					XDATA u8 XDATA *return_pt = NULL;
					return_pt = APP_HEAP_Alloc((sizeof(p_conn->connPara)+2));
					
					if(return_pt!=NULL)
					{
						xmemset(return_pt, 0, (sizeof(p_conn->connPara)+2));
						*(return_pt) = UART_EVENT_CONNECTION_PARA_UPDATE_NOTIFY;
						*(return_pt+1) = p_conn->handle;
						
						p_conn->connPara.interval_max = p_event->eventField.evtConnParaUpdate.connPara.interval_max;
						p_conn->connPara.interval_min = p_event->eventField.evtConnParaUpdate.connPara.interval_min;
						p_conn->connPara.latency = p_event->eventField.evtConnParaUpdate.connPara.latency;
						p_conn->connPara.supervision_tmieout = p_event->eventField.evtConnParaUpdate.connPara.supervision_tmieout;
						
						xmemcpy((u8 XDATA *)(return_pt+2), (u8 XDATA*)&(p_conn->connPara), sizeof(p_conn->connPara));
						
						send_event_command_complete((sizeof(p_conn->connPara)+2), UART_CMD_CONNECTION_PARA_UPDATE_REQ, 0, (u8 XDATA*)return_pt);
						APP_HEAP_Free(return_pt);
					}
				}
			}
			else
			{
				send_event_command_complete(0, UART_CMD_CONNECTION_PARA_UPDATE_REQ, APP_ErrCode_Command_Disallowed, NULL);
			}
		}
		break;
		
		case BLE_GAP_EVT_ENCRYPT_STATUS:
		{
			cmdProcessFlag=APP_PROCESS_CMD_ALLOW;
		}
		break;
		
		case BLE_GAP_EVT_ADVERTISING_REPORT:
		{
			XDATA u8 XDATA *p_adv_report;
			p_adv_report = APP_HEAP_Alloc(p_event->eventField.evtAdvReport.length+11+1);
			if(p_adv_report!=NULL)
			{
				*p_adv_report = UART_EVENT_ADVERTISING_REPORT;
				*(p_adv_report+1) = p_event->eventField.evtAdvReport.eventType;
				*(p_adv_report+2) = p_event->eventField.evtAdvReport.addrType;				
				xmemcpy((u8 XDATA *)(p_adv_report+3), (u8 XDATA *)p_event->eventField.evtAdvReport.addr,6);
				*(p_adv_report+9)=p_event->eventField.evtAdvReport.length;

				xmemcpy((u8 XDATA *)(p_adv_report+10),(u8 XDATA *)p_event->eventField.evtAdvReport.advData,p_event->eventField.evtAdvReport.length);
				if(p_event->eventField.evtAdvReport.length<=31)
				{
					*(p_adv_report+10+p_event->eventField.evtAdvReport.length)= p_event->eventField.evtAdvReport.advData[p_event->eventField.evtAdvReport.length];
				}
				else
				{
					*(p_adv_report+10+p_event->eventField.evtAdvReport.length)= p_event->eventField.evtAdvReport.rssi;
				}
				
				package_event(p_event->eventField.evtAdvReport.length+11+1, p_adv_report);
				APP_HEAP_Free(p_adv_report);
			}
		}
		break;
		
		default:
		break;
	}
}

PUBLIC void smp_event_handler(BLE_SMP_Event XDATA * p_event) large reentrant
{
	switch(p_event->eventID)
	{
		case BLE_SMP_EVT_PAIRING_COMPLETE:
		{		
			send_event_pairing_complete(p_event->eventField.evtPairingComplete.connHandle,p_event->eventField.evtPairingComplete.status);
		}
		break;
		
		case BLE_SMP_EVT_SECURITY_REQUEST:
		{
			BLE_SMP_InitiatePairing(p_event->eventField.evtSecurityReq.connHandle);
		}
		break;
		
		case BLE_SMP_EVT_NUMERIC_COMPARISON_CONFIRM_REQUEST:
		{
			XDATA APP_CONN_STRUCT XDATA *p_conn = NULL;
			p_conn = appGetConnInfo(p_event->eventField.evtDisplayCompareValue.connHandle);
			if(p_conn!=NULL)
			{
				p_conn->numericComparisonConfirmEnable=1;
				send_event_display_number(p_event->eventField.evtDisplayCompareValue.connHandle,p_event->eventField.evtDisplayCompareValue.value);
			}
		}
		break;
		
		case BLE_SMP_EVT_INPUT_PASSKEY:
		{
			send_event_input_passkey(p_event->eventField.evtInputPasskey.connHandle);
		}
		break;
		
		case BLE_SMP_EVT_DISPLAY_PASSKEY_REQUEST:	
		{
			XDATA APP_CONN_STRUCT XDATA *p_temp = NULL;
			p_temp=appGetConnInfo(p_event->eventField.evtDisplayPasskeyReq.connHandle);
	
			if(p_temp==NULL)
			{
				return;
			}
			BLE_SMP_GeneratePasskey(app_passkey);

			send_event_display_number(p_event->eventField.evtDisplayPasskeyReq.connHandle, app_passkey);
			if((ioCapability==BLE_SMP_IO_DISPLAYONLY)|| (ioCapability==BLE_SMP_IO_KEYBOARDDISPLAY))
			{
				BLE_SMP_PasskeyReply(p_event->eventField.evtDisplayPasskeyReq.connHandle,app_passkey);
			}
		}
		break;

		default:
		break;
	}
	
}

PUBLIC void gatt_event_handler(BLE_GATT_Event XDATA * p_event) large reentrant
{
	switch(p_event->eventID)
	{
		case BLE_GATTC_EVT_ERROR_RESP:
		{
			// Receive error response
			if(currentCmd!=0)
			{
				if(manualpattern_procedure&GET_REMOTE_TABLE_STEP1)
				{/*Get remote GATT table*/
					if(currentCmd==UART_CMD_DISCOVER_PRIMARY_SERVICE_BY_UUID)
					{
						APP_TIMER_FastTimer_Start(APP_TIMER_ID_0, 40);
						
					}
					else if(currentCmd == UART_CMD_DISCOVER_ALL_CHARACTERISTICS)
					{
						if(manualpattern_procedure&DISCOVER_PRIM_SER_BY_UUID_12)
						{
							APP_TIMER_FastTimer_Start(APP_TIMER_ID_0, 40);
						}
						else
						{
							manualpattern_procedure=0;
							send_event_command_complete(0, UART_CMD_DISCOVER_PRIMARY_SERVICE_BY_UUID,MBIOT_RES_SUCCESS, NULL);
							currentCmd = 0;
						}
					}
				}
				else
				{
				
					// Discover procedure ends. Send command complete.
					send_event_command_complete(0, currentCmd,MBIOT_RES_SUCCESS, NULL);
					currentCmd = 0;
					
				}
			}
			else
			{
				// Send error response
				XDATA u8 XDATA *p_resp;
				
				p_resp = APP_HEAP_Alloc(sizeof(BLE_ATT_EVT_Error_t)+1);	// sizeof(BLE_ATT_EVT_Error_t) + UART EVENT ID
				if(p_resp!=NULL)
				{
					*(p_resp+0) = UART_EVENT_ERROR_RESP;
					*(p_resp+1) = p_event->eventField.onError.connHandle;
					*(p_resp+2) = p_event->eventField.onError.reqOpcode;
					*(p_resp+3) = (p_event->eventField.onError.attrHandle>>8);
					*(p_resp+4) = p_event->eventField.onError.attrHandle;
					*(p_resp+5) = p_event->eventField.onError.errCode;
					package_event(sizeof(BLE_ATT_EVT_Error_t)+1, p_resp);
					APP_HEAP_Free(p_resp);
				}
			}
		}
		break;
		
		case BLE_GATTS_EVT_READ:
		{
			// Receive Read Request. Send read response by connection handle
			
			/* PM_MANUAL_READ_RESP is set:
			 *	1. Directly Send Read Response or Error Response when receiving event
			 *		- BLE_GATTS_SendReadResponse() or BLE_GATTS_SendErrorResponse()
			 *	2. Send this event to MCU by UART and then let MCU send Read Response or Error Response
			 */
			XDATA u8 XDATA *p_resp;
			
			p_resp = APP_HEAP_Alloc(7);
			if(p_resp!=NULL)
			{
				*(p_resp+0) = UART_EVENT_READ_REQ;
				*(p_resp+1) = p_event->eventField.onRead.connHandle;
				*(p_resp+2) = (p_event->eventField.onRead.charHandle>>8);
				*(p_resp+3) = (u8)p_event->eventField.onRead.charHandle;
				*(p_resp+4) = p_event->eventField.onRead.readType;
				*(p_resp+5) = (p_event->eventField.onRead.readOffset>>8);
				*(p_resp+6) = (u8)p_event->eventField.onRead.readOffset;
				package_event(7, p_resp);
				APP_HEAP_Free(p_resp);
			}
		}
		break;
		
		case BLE_GATTS_EVT_WRITE:
		{
			// Receive Write Request/Command
			XDATA APP_CONN_STRUCT XDATA *p_conn = NULL;
			
			p_conn = appGetConnInfo(p_event->eventField.onWrite.connHandle);
			if(p_conn == NULL)
				break;
			/*Be Queried Enable Transparent*/
			if(p_event->eventField.onWrite.charHandle==localHandleList.txCCCD)
			{
				XDATA u16 cccd;
				xmemcpy((u8 XDATA *)&cccd, p_event->eventField.onWrite.p_charValue, 2);
				if(cccd==CLIENT_CHAR_CONFIG_DESCRIPTOR_ENABLE)
				{
					p_conn->appState=APP_STATE_LE_OPEN;
					p_conn->role_att = BLE_GATT_ROLE_SERVER;
					send_event_status(app_module_status);/*Update Link status*/
				}
				else
				{
					p_conn->appState=APP_STATE_LE_CONNECTED;
					send_event_status(app_module_status);/*Update Link status*/
				}
			}
			else if(p_event->eventField.onWrite.charHandle==localHandleList.transRx)
			{
				process_package_event(p_conn->handle, UART_EVENT_RECEIVED_TRANSPARENT_DATA, p_event->eventField.onWrite.writeDataLength+2, p_event->eventField.onWrite.p_charValue);
			}
			else
			{
				/* If PM_MANUAL_WRITE_RESP is set and p_event->eventField.onWrite.writeType is Request rather than Command:
				 *	1. Directly Send Write Response or Error Response when receiving Write Request event
				 *		- BLE_GATTS_SendWriteResponse() or BLE_GATTS_SendErrorResponse()
				 *	2. Send this event to MCU by UART and then let MCU send Write Response or Error Response 
				 */
				XDATA u8 XDATA *p_resp;
				XDATA u16 eventLength = 1+1+2+1+2+1+p_event->eventField.onWrite.writeDataLength;	// event ID(1) + connHandle(1) + charHandle(2) + write type(1)
																									// value offset(2) + flags(1) + writeDataLength
				p_resp = APP_HEAP_Alloc(eventLength);
				if(p_resp!=NULL)
				{
					*(p_resp+0) = UART_EVENT_CLIENT_WRITE_CHAR_VALUE;
					*(p_resp+1) = p_event->eventField.onWrite.connHandle;
					*(p_resp+2) = (p_event->eventField.onWrite.charHandle>>8);
					*(p_resp+3) = (u8)p_event->eventField.onWrite.charHandle;
					*(p_resp+4) = p_event->eventField.onWrite.writeType;
					*(p_resp+5) = (p_event->eventField.onWrite.valueOffset>>8);
					*(p_resp+6) = (u8)p_event->eventField.onWrite.valueOffset;
					*(p_resp+7) = p_event->eventField.onWrite.flags;
					if(p_event->eventField.onWrite.writeDataLength)
					{
						xmemcpy_u16((p_resp+8), p_event->eventField.onWrite.p_charValue, p_event->eventField.onWrite.writeDataLength);
					}
					package_event(eventLength, p_resp);
					APP_HEAP_Free(p_resp);
				}
			}
		}
		break;
		
		case BLE_GATTS_EVT_HV_CONFIRM:
		{
			// Indication ACK
			XDATA u8 XDATA *p_resp;
			
			p_resp = APP_HEAP_Alloc(2);
			if(p_resp!=NULL)
			{
				*(p_resp+0) = UART_EVENT_HANDLE_VALUE_CONFIRMATION_RECEIVED;
				*(p_resp+1) = p_event->eventField.onConfirmation.connHandle;
				package_event(2, p_resp);
				APP_HEAP_Free(p_resp);
			}
		}
		break;
		
		case BLE_GATTC_EVT_HV_NOTIFY:
		{
			// Receive Handle Value Notification
			XDATA APP_CONN_STRUCT XDATA *p_conn = NULL;
			
			p_conn = appGetConnInfo(p_event->eventField.onNotification.connHandle);
			if(p_conn == NULL)
				break;
			if((p_conn->appState==APP_STATE_LE_OPEN)&&(p_event->eventField.onNotification.charHandle==p_conn->remoteHandleList.transTx))
			{
					process_package_event(p_conn->handle, UART_EVENT_RECEIVED_TRANSPARENT_DATA, p_event->eventField.onNotification.receivedLength+2, p_event->eventField.onNotification.p_receivedValue);
			}
			else
			{
				XDATA u8 XDATA *p_resp;
				
				p_resp = APP_HEAP_Alloc(p_event->eventField.onNotification.receivedLength+4);
				if(p_resp!=NULL)
				{
					*(p_resp+0) = UART_EVENT_CHARACTERISTIC_VALUE_RECEIVED;
					*(p_resp+1) = p_event->eventField.onNotification.connHandle;
					*(p_resp+2) = (p_event->eventField.onNotification.charHandle>>8);
					*(p_resp+3) = p_event->eventField.onNotification.charHandle;
					xmemcpy((u8 XDATA *)(p_resp+4), (u8 XDATA *)p_event->eventField.onNotification.p_receivedValue, p_event->eventField.onNotification.receivedLength);
					package_event(p_event->eventField.onNotification.receivedLength+4, p_resp);
					APP_HEAP_Free(p_resp);
				}
			}
		}
		break;
		
		case BLE_GATTC_EVT_HV_INDICATE:
		{
			// Receive Handle Value Indication
			XDATA u8 XDATA *p_resp;
			
			p_resp = APP_HEAP_Alloc(p_event->eventField.onIndication.receivedLength+4);
			if(p_resp!=NULL)
			{
				*(p_resp+0) = UART_EVENT_CHARACTERISTIC_VALUE_RECEIVED;
				*(p_resp+1) = p_event->eventField.onIndication.connHandle;
				*(p_resp+2) = (p_event->eventField.onIndication.charHandle>>8);
				*(p_resp+3) = p_event->eventField.onIndication.charHandle;
				xmemcpy((u8 XDATA *)(p_resp+4), (u8 XDATA *)p_event->eventField.onIndication.p_receivedValue, p_event->eventField.onIndication.receivedLength);
				package_event(p_event->eventField.onIndication.receivedLength+4, p_resp);
				APP_HEAP_Free(p_resp);
			}
		}
		break;
		
		case BLE_GATTC_EVT_DISC_PRIM_SERV_RESP:
		{
			XDATA u8 XDATA *p_resp;
			
			p_resp = APP_HEAP_Alloc(p_event->eventField.onDiscPrimServResp.attrDataLength+3);
			if(p_resp!=NULL)
			{
				*(p_resp+0) = UART_EVENT_DISCOVER_ALL_PRIMARY_SERVICES_RESP;
				*(p_resp+1) = p_event->eventField.onDiscPrimServResp.connHandle;
				*(p_resp+2) = p_event->eventField.onDiscPrimServResp.attrPairLength;
				// Data format in Air is little endian. Adjust data format in p_event->eventField.onDiscPrimServResp.p_attrData if needed.
				xmemcpy((u8 XDATA *)(p_resp+3),(u8 XDATA *)p_event->eventField.onDiscPrimServResp.p_attrData, p_event->eventField.onDiscPrimServResp.attrDataLength);
				package_event(p_event->eventField.onDiscPrimServResp.attrDataLength+3, p_resp);
				APP_HEAP_Free(p_resp);
			}
			
			if((p_event->eventField.onDiscPrimServResp.procedureStatus==GATT_PROCEDURE_STATUS_FINISH)&&(currentCmd==UART_CMD_DISCOVER_ALL_PRIMARY_SERVICES))
			{
				send_event_command_complete(0, UART_CMD_DISCOVER_ALL_PRIMARY_SERVICES, 0, NULL);
				currentCmd = 0;
			}
		}
		break;
		
		case BLE_GATTC_EVT_DISC_PRIM_SERV_BY_UUID_RESP:
		{
			XDATA u8 XDATA *p_resp;
			if(manualpattern_procedure&GET_REMOTE_TABLE_STEP1)
			{/*Get remote GATT table*/
				cHandle = p_event->eventField.onDiscPrimServByUuidResp.connHandle;
				sHandle = (u16)((*(u8 XDATA*)(p_event->eventField.onDiscPrimServByUuidResp.p_handleInfo+1))<<8)+*(u8 XDATA*)(p_event->eventField.onDiscPrimServByUuidResp.p_handleInfo);
				eHandle = (u16)((*(u8 XDATA*)(p_event->eventField.onDiscPrimServByUuidResp.p_handleInfo+3))<<8)+*(u8 XDATA*)(p_event->eventField.onDiscPrimServByUuidResp.p_handleInfo+2);
				
			}
			else
			{
				p_resp = APP_HEAP_Alloc(p_event->eventField.onDiscPrimServByUuidResp.handleInfoLength+2);
				if(p_resp!=NULL)
				{
					*(p_resp+0) = UART_EVENT_DISCOVER_PRIMARY_SERVICE_BY_UUID_RESP;
					*(p_resp+1) = p_event->eventField.onDiscPrimServByUuidResp.connHandle;
					// Data format in Air is little endian. Adjust data format in p_event->eventField.onDiscPrimServByUuidResp.p_handleInfo if needed.
					xmemcpy((u8 XDATA *)(p_resp+2), (u8 XDATA *)p_event->eventField.onDiscPrimServByUuidResp.p_handleInfo, p_event->eventField.onDiscPrimServByUuidResp.handleInfoLength);
					package_event(p_event->eventField.onDiscPrimServByUuidResp.handleInfoLength+2, p_resp);
					APP_HEAP_Free(p_resp);
				}
				
				if((p_event->eventField.onDiscPrimServByUuidResp.procedureStatus==GATT_PROCEDURE_STATUS_FINISH)&&(currentCmd==UART_CMD_DISCOVER_PRIMARY_SERVICE_BY_UUID))
				{
					send_event_command_complete(0, UART_CMD_DISCOVER_PRIMARY_SERVICE_BY_UUID, 0, NULL);
					currentCmd = 0;
				}
			}
		}
		break;
		
		case BLE_GATTC_EVT_DISC_CHAR_RESP:
		{
			XDATA u8 XDATA *p_resp;
			XDATA APP_CONN_STRUCT XDATA *p_conn = NULL;
			
			p_conn = appGetConnInfo(p_event->eventField.onReadUsingUuidResp.connHandle);
			if(p_conn == NULL)
				break;

			if(p_conn->serviceState < QUERY_TX_CHAR)
			{	
				if(manualpattern_procedure&GET_REMOTE_TABLE_STEP1)
				{/*Get remote GATT table*/
						currentCmd = UART_CMD_DISCOVER_ALL_CHARACTERISTICS;
				}
				
				/*normal case*/
				p_resp = APP_HEAP_Alloc(p_event->eventField.onDiscCharResp.attrDataLength+3);
				if(p_resp!=NULL)
				{
					*(p_resp+0) = UART_EVENT_DISCOVER_ALL_CHARACTERISTICS_RESP;
					*(p_resp+1) = p_event->eventField.onDiscCharResp.connHandle;
					*(p_resp+2) = p_event->eventField.onDiscCharResp.attrPairLength;
					// Data format in Air is little endian. Adjust data format in p_event->eventField.onDiscCharResp.p_attrData if needed.
					xmemcpy((u8 XDATA *)(p_resp+3),(u8 XDATA *)p_event->eventField.onDiscCharResp.p_attrData, p_event->eventField.onDiscCharResp.attrDataLength);
					package_event(p_event->eventField.onDiscCharResp.attrDataLength+3, p_resp);
					APP_HEAP_Free(p_resp);
				}
				
				if((p_event->eventField.onDiscCharResp.procedureStatus==GATT_PROCEDURE_STATUS_FINISH)&&(currentCmd==UART_CMD_DISCOVER_ALL_CHARACTERISTICS))
				{
					send_event_command_complete(0, UART_CMD_DISCOVER_ALL_CHARACTERISTICS, 0, NULL);
					currentCmd = 0;
				}
				
				break;	
			}
			/*Get TX/RX CCCD*/
			{
				XDATA u8 tempUUID[48]={UUID_MCHP_TRANS_TX_INVERSE_16, UUID_MCHP_TRANS_RX_INVERSE_16, UUID_MCHP_TRANS_CTRL_INVERSE_16};
				XDATA u8 i = 0, basic=0x00;
				
				for(i = 0; i <CHECK_NUM; i++)
				{
					basic = 0x00;
					do
					{	
							if(xmemcmp((p_event->eventField.onDiscCharResp.p_attrData+basic+0x05),&tempUUID[(i*16)], p_event->eventField.onDiscCharResp.attrPairLength-0x05)== 0x00)
							{
								if(i == CHECK_TRANS_TX)
								{
									EQU_Swap_u16pt(&p_conn->remoteHandleList.transTx, (p_event->eventField.onDiscCharResp.p_attrData+basic+0x03));
									p_conn->remoteHandleList.txCCCD=p_conn->remoteHandleList.transTx+1;
									p_conn->serviceState=QUERY_RX_CHAR;
								}
								else if(i == CHECK_TRANS_RX)
								{
									EQU_Swap_u16pt(&p_conn->remoteHandleList.transRx, (p_event->eventField.onDiscCharResp.p_attrData+basic+0x03));
									p_conn->serviceState=QUERY_CTRL_CHAR;
								}
								else if(i == CHECK_TRANS_CTRL)
								{
									EQU_Swap_u16pt(&p_conn->remoteHandleList.transCtrl, (p_event->eventField.onDiscCharResp.p_attrData+basic+0x03));
									p_conn->remoteHandleList.ctrlCCCD=p_conn->remoteHandleList.transCtrl+1;
									//app_master_enable_credit_flow_control(p_conn);
									app_master_ctrl_data_receive(CLIENT_CHAR_CONFIG_DESCRIPTOR_ENABLE, p_conn);
								}
								break;
							}
							
							basic+=p_event->eventField.onDiscCharResp.attrPairLength;						
						
					}while(basic <= p_event->eventField.onDiscCharResp.attrDataLength);
				}
			}
		}
		break;
		
		case BLE_GATTC_EVT_DISC_DESC_RESP:
		{
			// Send discover response event if there is data.
			if(p_event->eventField.onDiscDescResp.infoDataLength!=0)
			{
				XDATA u8 XDATA *p_resp;
				
				p_resp = APP_HEAP_Alloc(p_event->eventField.onDiscDescResp.infoDataLength+3);
				if(p_resp!=NULL)
				{
					*(p_resp+0) = UART_EVENT_DISCOVER_ALL_DESCRIPTORS_RESP;
					*(p_resp+1) = p_event->eventField.onDiscDescResp.connHandle;
					*(p_resp+2) = p_event->eventField.onDiscDescResp.infoDataFormat;
					// Data format in Air is little endian. Adjust data format in p_event->eventField.onDiscDescResp.p_infoData if needed.
					xmemcpy((u8 XDATA *)(p_resp+3), (u8 XDATA *)p_event->eventField.onDiscDescResp.p_infoData, p_event->eventField.onDiscDescResp.infoDataLength);
					package_event(p_event->eventField.onDiscDescResp.infoDataLength+3, p_resp);
					APP_HEAP_Free(p_resp);
				}
			}
			
			// Send command complete if procedure end. No matter there is data in the event.
			if((p_event->eventField.onDiscDescResp.procedureStatus==GATT_PROCEDURE_STATUS_FINISH)&&(currentCmd==UART_CMD_DISCOVER_ALL_DESCRIPTORS))
			{
				send_event_command_complete(0, UART_CMD_DISCOVER_ALL_DESCRIPTORS, 0, NULL);
				currentCmd = 0;
			}

			/*Get remote GATT table*/
			if(manualpattern_procedure&GET_REMOTE_TABLE_STEP2)
			{
				/*Wait UUID_DESCRIPTOR_CLIENT_CHAR_CONFIG list*/
				manualpattern_procedure=0;
				APP_TIMER_FastTimer_Start(APP_TIMER_ID_0, 250);
			}
		}
		break;
		
		case BLE_GATTC_EVT_READ_USING_UUID_RESP:
		{
			XDATA u8 XDATA *p_resp;
			
			p_resp = APP_HEAP_Alloc(p_event->eventField.onReadUsingUuidResp.attrDataLength+3);
			if(p_resp!=NULL)
			{
				*(p_resp+0) = UART_EVENT_READ_USING_UUID_RESP;
				*(p_resp+1) = p_event->eventField.onReadUsingUuidResp.connHandle;
				*(p_resp+2) = p_event->eventField.onReadUsingUuidResp.attrPairLength;
				// Data format in Air is little endian. Adjust data format in p_event->eventField.onReadUsingUuidResp.p_attrData if needed.
				xmemcpy((u8 XDATA *)(p_resp+3),(u8 XDATA *)p_event->eventField.onReadUsingUuidResp.p_attrData, p_event->eventField.onReadUsingUuidResp.attrDataLength);
				package_event(p_event->eventField.onReadUsingUuidResp.attrDataLength+3, p_resp);
				APP_HEAP_Free(p_resp);
			}
			
			if((p_event->eventField.onReadUsingUuidResp.procedureStatus==GATT_PROCEDURE_STATUS_FINISH)&&(currentCmd==UART_CMD_READ_USING_CHARACTERISTIC_UUID))
			{
				send_event_command_complete(0, UART_CMD_READ_USING_CHARACTERISTIC_UUID, 0, NULL);
				currentCmd = 0;
			}
		}
		break;
		
		case BLE_GATTC_EVT_READ_RESP:
		{
			// Receive Read Response
			XDATA u8 XDATA *p_resp;
			if(manualpattern_procedure&GET_REMOTE_TABLE_STEP2)
			{/*Get remote GATT table*/
				p_resp = APP_HEAP_Alloc(p_event->eventField.onReadResp.attrDataLength+3);
				if(p_resp!=NULL)
				{
					*(p_resp+0) = p_event->eventField.onReadResp.connHandle;
					*(p_resp+1) = currentCmd;
					*(p_resp+2) = MBIOT_RES_SUCCESS;
					xmemcpy((u8 XDATA *)(p_resp+3), (u8 XDATA *)p_event->eventField.onReadResp.p_readValue, p_event->eventField.onReadResp.attrDataLength);
					package_event(p_event->eventField.onReadResp.attrDataLength+3, p_resp);
					currentCmd=0;
					manualpattern_procedure=0;
					APP_HEAP_Free(p_resp);
					cmdProcessFlag=APP_PROCESS_CMD_ALLOW;
				}
			}
			else
			{
				p_resp = APP_HEAP_Alloc(p_event->eventField.onReadResp.attrDataLength+3);
				if(p_resp!=NULL)
				{
					*(p_resp+0) = UART_EVENT_READ_RESP;
					*(p_resp+1) = p_event->eventField.onReadResp.connHandle;
					*(p_resp+2) = p_event->eventField.onReadResp.responseType;
					xmemcpy((u8 XDATA *)(p_resp+3), (u8 XDATA *)p_event->eventField.onReadResp.p_readValue, p_event->eventField.onReadResp.attrDataLength);
					package_event(p_event->eventField.onReadResp.attrDataLength+3, p_resp);
					APP_HEAP_Free(p_resp);
				}
			}
		}
		break;
		
		case BLE_GATTC_EVT_WRITE_RESP:
		{
			// Receive Write Response
			XDATA u8 XDATA *p_resp;
			XDATA APP_CONN_STRUCT XDATA *p_conn = NULL;
			
			p_conn = appGetConnInfo(p_event->eventField.onReadUsingUuidResp.connHandle);
			if(p_conn == NULL)
				break;
			
			if(p_event->eventField.onWriteResp.responseType==BLE_GATT_TYPE_PREP_WRITE_RESP)
			{
				XDATA u16 eventLength = 1+1+1+2+2+p_event->eventField.onWriteResp.writeLength;	// event ID(1) + connHandle(1) + response Type(1) +
																								// charHandle(2) + value Offset(2) + writeLength
				p_resp = APP_HEAP_Alloc(eventLength);
				if(p_resp!=NULL)
				{
					*(p_resp+0) = UART_EVENT_WRITE_RESP;
					*(p_resp+1) = p_event->eventField.onWriteResp.connHandle;
					*(p_resp+2) = p_event->eventField.onWriteResp.responseType;
					*(p_resp+3) = (p_event->eventField.onWriteResp.charHandle>>8);
					*(p_resp+4) = (u8)p_event->eventField.onWriteResp.charHandle;
					*(p_resp+5) = (p_event->eventField.onWriteResp.valueOffset>>8);
					*(p_resp+6) = (u8)p_event->eventField.onWriteResp.valueOffset;
					xmemcpy_u16((p_resp+7), p_event->eventField.onWriteResp.p_writeValue, p_event->eventField.onWriteResp.writeLength);
					package_event(eventLength, p_resp);
					APP_HEAP_Free(p_resp);
				}
			}
			else
			{

				switch(p_conn->txCapacityState)
				{
					case WRITE_INITIATE_CREDIT_FLOW_CTRL:
					{
						app_master_initiate_credit_flow_control(p_conn);
					}
					break;
					case WRITE_ENABLE_CREDIT_FLOW_CTRL:
					{
						app_master_ctrl_data_receive(CLIENT_CHAR_CONFIG_DESCRIPTOR_ENABLE, p_conn);					
					}
					break;
					case WRITE_DATA_RECEIVE_CTRL_ENABLE:
					{
						if(p_conn->appState==APP_STATE_LE_CONNECTED)
						{
							p_conn->appState=APP_STATE_LE_OPEN;
							send_event_status(app_module_status);/*Update Link status*/
						}
						else if(p_conn->appState==APP_STATE_LE_OPEN){
							send_event_command_complete(0, UART_CMD_SEND_TRANSPARENT_DATA, 0, NULL);
						}
					}
					break;
					case WRITE_DATA_RECEIVE_CTRL_DISABLE:
					{
						p_conn->appState=APP_STATE_LE_CONNECTED;
						send_event_status(app_module_status);/*Update Link status*/
					}
					break;
					
					default:/*normal case*/
					{
						XDATA u16 eventLength = 1+1+1;	// event ID(1) + connHandle(1) + response Type(1)
						p_resp = APP_HEAP_Alloc(eventLength);
						if(p_resp!=NULL)
						{
							*(p_resp+0) = UART_EVENT_WRITE_RESP;
							*(p_resp+1) = p_event->eventField.onWriteResp.connHandle;
							*(p_resp+2) = p_event->eventField.onWriteResp.responseType;
							package_event(eventLength, p_resp);
							APP_HEAP_Free(p_resp);
						}
					}
					break;
				}
			}
		}
		break;
		
		case BLE_GATTS_EVT_READ_BY_TYPE:
		{
			// This event is only received when Read By Type Request sent by Client is performed on PM_MANUAL_READ_RESP attribute.
			// Send Read By Type Response or Error Response with corresponding error code.
			XDATA u8 XDATA *p_resp;
			
			p_resp = APP_HEAP_Alloc(4);
			if(p_resp!=NULL)
			{
				*(p_resp+0) = UART_EVENT_READ_BY_TYPE_REQ;
				*(p_resp+1) = p_event->eventField.onReadByType.connHandle;
				*(p_resp+2) = (p_event->eventField.onReadByType.charHandle>>8);
				*(p_resp+3) = (u8)p_event->eventField.onReadByType.charHandle;
				package_event(4, p_resp);
				APP_HEAP_Free(p_resp);
			}
		}
		break;
		
		case BLE_ATT_EVT_TIMEOUT:
		{
			// Timeout handling
		}
		break;
		
		case BLE_ATT_EVT_UPDATE_MTU:
		{
			// Update ATT MTU event. Change the ATT MTU when receive exchange MTU Request of Response.
			XDATA APP_CONN_STRUCT XDATA *p_conn = appGetConnInfo(p_event->eventField.onUpdateMTU.connHandle);/*event must have conn handle*/
			p_conn->attMTU = p_event->eventField.onUpdateMTU.exchangedMTU;
		}
		break;
		
		default:
		break;
	}
}

PUBLIC void l2cap_event_handler(BLE_L2CAP_Event XDATA * p_event) large reentrant
{
	switch(p_event->eventID)
	{
		case BLE_L2CAP_EVT_CONN_PARA_UPDATE_RESULT:
		{
			if(p_event->eventField.evtConnParaUpdateResult.result==BLE_L2CAP_CONNECTION_PARAS_REJECTED)
			{
				send_event_command_complete(0, UART_CMD_CONNECTION_PARA_UPDATE_REQ, APP_ErrCode_Invalid_Parameters, NULL);
			}
		}
		break;

		default:
		break;
	}
}

PUBLIC void common_event_handler(APP_Common_Event XDATA * p_event) large reentrant
{
	switch(p_event->eventID)
	{
		case APP_TIMER_EVT:
		{
			switch(p_event->eventField.evtTimer.timerID)
			{
				case APP_TIMER_ID_0:	
				{/*Get remote GATT table*/
					if(currentCmd==UART_CMD_DISCOVER_PRIMARY_SERVICE_BY_UUID)
					{
						BLE_GATTC_DiscoverAllCharacteristics(cHandle, sHandle, eHandle);
						
					}
					else if(currentCmd == UART_CMD_DISCOVER_ALL_CHARACTERISTICS)
					{
						if(manualpattern_procedure&GET_REMOTE_TABLE_STEP2)
						BLE_GATTC_DiscoverAllDescriptors(cHandle, sHandle, eHandle);
						else
						{
							send_event_command_complete(0, UART_CMD_DISCOVER_PRIMARY_SERVICE_BY_UUID,MBIOT_RES_SUCCESS, NULL);
							currentCmd = 0;
						}
					}
					
				}
				break;
					
				case APP_TIMER_ID_1:
				break;

				case APP_TIMER_ID_3:
				{	
					if((indexreTrans>0)&&(indexreTrans<=MAX_PACKET_BUFFER_SIZE))
					{
						XDATA u16 errCode = MBIOT_RES_SUCCESS;
						if( reTrans[indexreTrans-1].transTxMode == BLE_GATT_TYPE_WRITE_CMD)
							errCode = BLE_GATTC_Write(reTrans[indexreTrans-1].transConnHandle, (BLE_GATTC_WriteParams XDATA *)reTrans[indexreTrans-1].p_reTransData);	
						else if( reTrans[indexreTrans-1].transTxMode == BLE_GATT_HV_NOTIFICATION)
							errCode = BLE_GATTS_SendHandleValue(reTrans[indexreTrans-1].transConnHandle, (BLE_GATTS_HandleValueParams XDATA*)reTrans[indexreTrans-1].p_reTransData);
						retryTrCount++;
						if(errCode == MBIOT_RES_SUCCESS)
						{
							APP_HEAP_Free(reTrans[indexreTrans-1].p_reTransData);
							indexreTrans--;
						}

						if(indexreTrans>0)
						{
							if(retryTrCount<TRANS_RETRY_COUNT_MAX)
							{
								APP_TIMER_FastTimer_Start(APP_TIMER_ID_3, multiply_u16(multiply_u16(retryTrCount,indexreTrans),multiply_u16(retryTrCount,indexreTrans))+120);
							}
							else
							{
								send_event_command_complete(0, UART_CMD_SEND_TRANSPARENT_DATA, APP_ErrCode_Insufficient_Resources, NULL);
								Transparent_clear(0x00);
								break;
							}
						}
						else
						{
							retryTrCount=0;
							send_event_command_complete(0, UART_CMD_SEND_TRANSPARENT_DATA, 0, NULL);
						}
					}
				}
				break;	
#ifdef UART_CMD_PERIPHERAL_API_TEST
				case APP_TIMER_ID_13:
				{	
					if(spi_working==APP_SPI_MASTER) 
						app_SPI_Master_RX();
					
					APP_TIMER_FastTimer_Start(APP_TIMER_ID_13, 80);	
					
				}break;	
#endif				
			}
		}
		break;
		
		case APP_OTA_EVT:
		{
		
		}
		break;
	}
}

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
						
			while(data_len > 0)
			{
				tempBCH = p_buffer[index++];
				switch(uart_RxState)
				{
					case UART_RX_STATE_IDLE:
						if(tempBCH==UART_START_BYTE)
						{
							p_uart_cmd_buff=APP_HEAP_Alloc(MAX_UART_CMD_BUFFER);
							if(p_uart_cmd_buff!=NULL)
							{
								uart_RxState = UART_RX_STATE_LENGTH_H;
							}
						}
						break;
    		
					case UART_RX_STATE_LENGTH_H:
						uart_rx_checksum = tempBCH;
						uart_cmd_len = (tempBCH<<8);
						uart_RxState = UART_RX_STATE_LENGTH_L;
						break;
    		
					case UART_RX_STATE_LENGTH_L:
						uart_cmd_len |= tempBCH;
						uart_rx_checksum += tempBCH;
						uart_RxState = UART_RX_STATE_CMD_ID;
						uart_cmd_rIndex=0;
						break;
    		
					case UART_RX_STATE_CMD_ID:
						if(p_uart_cmd_buff!=NULL)
						{
							p_uart_cmd_buff[uart_cmd_rIndex]=tempBCH;
						}
						uart_cmd_rIndex++;
						uart_rx_checksum += tempBCH;
						if((uart_cmd_len-1)>0)
						{
							uart_RxState = UART_RX_STATE_PAYLOAD;
						}
						else
						{
							uart_RxState = UART_RX_STATE_CHECKSUM;
						}
						break;
						
					case UART_RX_STATE_PAYLOAD:
						if(p_uart_cmd_buff!=NULL)
						{
							p_uart_cmd_buff[uart_cmd_rIndex]=tempBCH;
						}
						uart_cmd_rIndex++;
						uart_rx_checksum += tempBCH;
						if(uart_cmd_rIndex >= MAX_UART_CMD_BUFFER)
						{
							uart_RxState=UART_RX_STATE_IDLE;
						}
						else if(uart_cmd_rIndex == uart_cmd_len)
						{
							uart_RxState=UART_RX_STATE_CHECKSUM;
						}
						break;
    		
					case UART_RX_STATE_CHECKSUM:
					{
						uart_rx_checksum += tempBCH;
						if(uart_rx_checksum == 0)
						{
							if(p_uart_cmd_buff!=NULL)
							{
								app_process_uart_cmd(uart_cmd_rIndex,p_uart_cmd_buff);
							}
						}
						else
						{
							if(p_uart_cmd_buff!=NULL)
							{
								APP_HEAP_Free(p_uart_cmd_buff);
							}
							send_event_command_complete(0,0,0xff,NULL);//Checksum error								
						}
						uart_RxState=UART_RX_STATE_IDLE;
					}
					break;
						
					default:
						uart_RxState=UART_RX_STATE_IDLE;
						break;
				}
				data_len--;
			}

			APP_HEAP_Free(p_buffer);
		}
		break;

		case APP_SPI_SLAVE_EVT_TRANSFER_DONE:
		{
#ifdef UART_CMD_PERIPHERAL_API_TEST	
			/*SPI communication is finished*/
			XDATA u8 XDATA *p_status;
			p_status = APP_HEAP_Alloc(2);
			if(p_status != NULL)
			{
				*p_status=PERIPHERAL_API_TEST_SPI_Slave;
				*(p_status+1)=APP_SPI_SLAVE_EVT_TRANSFER_DONE;
				package_event(2, p_status);
				APP_HEAP_Free(p_status);
			}
			if(spi_sw_gpio == 0x00)
			{
				APP_GPIO_Pin_Clear(GPIO_P11);
			}
			else if(spi_sw_gpio == 0x01)
			{
				APP_GPIO_Pin_Clear(GPIO_P12);
			}
			
#endif
		}	
		break;
		
		case APP_SPI_SLAVE_EVT_RECEIVE_DATA:	
		{
#ifdef UART_CMD_PERIPHERAL_API_TEST	
			/*SPI Slave get data from master*/
			app_SPI_Slave_RX(p_event->eventField.spiSlaveEvent.dataLen, p_event->eventField.spiSlaveEvent.p_rxBuffer);
#endif			
		}
		break;

		case APP_GPIO_STATE_CHANGED:
		{
			XDATA u8 XDATA *p_status;
			
			p_status = APP_HEAP_Alloc(3);
			if(p_status != NULL)
			{
				*p_status=MBIOT_RES_SUCCESS;
				*(p_status+1)=PERIPHERAL_API_TEST_GPIO;
				*(p_status+2)=APP_GPIO_STATE_CHANGED;
			
				package_event(3, p_status);
				APP_HEAP_Free(p_status);
			}
		}
		break;
	}
}

PUBLIC void Application_Init() large reentrant
{
	XDATA u16 startHandle;
	XDATA u8 otaAdminKey[16];
	XDATA BLE_GATT_ServicesInfo serviceInfo;
	
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
	BLE_GATTS_StartCreatingServiceTable();
	serviceInfo.p_definedService = (u8 XDATA *)APP_Defined_Services_TRS;
	serviceInfo.attributeNum = TOTAL_TRS_ATTR_NUM;
	serviceInfo.serviceSize = sizeof(APP_Defined_Services_TRS);
	BLE_GATTS_InitServiceTable(&serviceInfo, &startHandle);
	
	/*Create TX/RX CCCD*/
	localHandleList.transTx=startHandle+INDEX_MCHP_TRANS_TX_VALUE;
	localHandleList.txCCCD=startHandle+INDEX_MCHP_TRANS_TX_CCCD;
	localHandleList.transRx=startHandle+INDEX_MCHP_TRANS_RX_VALUE;
	localHandleList.transCtrl=startHandle+INDEX_MCHP_TRANS_CTRL_VALUE;
	localHandleList.ctrlCCCD=startHandle+INDEX_MCHP_TRANS_CTRL_CCCD;
	
	serviceInfo.p_definedService = (u8 XDATA *)APP_Defined_Services;
	serviceInfo.attributeNum = APP_DEFINED_ATTRIBUTE_NUMBER;
	serviceInfo.serviceSize = sizeof(APP_Defined_Services);
	BLE_GATTS_InitServiceTable(&serviceInfo, &startHandle);
	BLE_GATTS_EndCreatingServiceTable();
	
	//Register OTA admin key if OTA feature needed
	xmemset(otaAdminKey, 0xAA, 16);
	UTILITY_RegisterOTAKey(otaAdminKey);
	
	//ATT stack configuration
	BLE_GATT_Configure(0x0000);

	{
		XDATA APP_UART_ConfParas uartParas;
		
		uartParas.baudrate=APP_UART_BAUD_RATE_115200;
    	uartParas.flowContol=APP_UART_FLOW_CONTROL_DISABLE;
    	uartParas.rxNotifyLength=0x05;
    	uartParas.rxBufferSize=APP_RX_BUFFER_LEN;
    	uartParas.rxBuffer=AppRxBuffer;
    	uartParas.txBufferSize=APP_TX_BUFFER_LEN;
    	uartParas.txBuffer=AppTxBuffer;
    	uartParas.txIndicateInterval=5;
    	uartParas.rxWakeupPinNum=GPIO_P14;
    			
		APP_UART_Init(&uartParas);
		uarbaudrate =uartParas.baudrate;
	}	

	UTILITY_LowPowerModeControl(LOW_POWER_MODE_ALLOW);
	app_init();
}
