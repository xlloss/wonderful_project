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

PUBLIC  MCODE    u8   APP_VERSION[]={0x05,0x00};
PUBLIC  MCODE    u8   SPECIFIC_PATTERN[]={"MCHP"};

#ifdef APP_OPEN_DEBUG_MODE
PUBLIC XDATA u8 App_Debug_Tx_Buffer[APP_DEBUG_TX_BUFFER_LEN];
PUBLIC XDATA u8 App_Debug_Rx_Buffer[APP_DEBUG_RX_BUFFER_LEN];
#endif

PUBLIC  XDATA APP_HEAP_STRUCT         App_Heap[APP_HEAP_BLOCK_NUMBER];
PUBLIC  XDATA APP_HEAP_INDEX          App_Heap_Index[APP_HEAP_BLOCK_NUMBER];


static XDATA u8 AppRxBuffer[APP_RX_BUFFER_LEN];
static XDATA u8 AppTxBuffer[APP_TX_BUFFER_LEN];

static XDATA Packet_Buffer 	PacketOut[MAX_PACKET_BUFFER_LIST];
static XDATA Packet_Buffer 	PacketIn[MAX_PACKET_BUFFER_LIST];
static XDATA App_List		appList;
static XDATA Handle_List	localHandleList;
static XDATA Handle_List	remoteHandleList;

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



PUBLIC MCODE u8 APP_Defined_Services_DIS[] = {
	/*Device Information Service*/
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_SERVICE_2 */
	0x0A,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_PRIMARY_SERVICE>>8,UUID_PRIMARY_SERVICE,					0x00,0x00,	UUID_DEVICE_INFORMATION>>8,UUID_DEVICE_INFORMATION,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */
	0x0B,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						READ,	0x00,0x00,	UUID_MANUFACTURE_NAME_STRING>>8,UUID_MANUFACTURE_NAME_STRING,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE */
	0x1E,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_MANUFACTURE_NAME_STRING>>8,UUID_MANUFACTURE_NAME_STRING,	0x04,	'M','C','H','P',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */
	0x0B,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						READ,	0x00,0x00,	UUID_MODEL_NUMBER_STRING>>8,UUID_MODEL_NUMBER_STRING,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE */
	0x1E,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_MODEL_NUMBER_STRING>>8,UUID_MODEL_NUMBER_STRING,			0x04,	'B','M','7','0',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */
	0x0B,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						READ,	0x00,0x00,	UUID_SERIAL_NUMBER_STRING>>8,UUID_SERIAL_NUMBER_STRING,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE */
	0x1E,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_SERIAL_NUMBER_STRING>>8,UUID_SERIAL_NUMBER_STRING,			0x04,	0x30,0x30,0x30,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */
	0x0B,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						READ,	0x00,0x00,	UUID_HARDWARE_REVISION_STRING>>8,UUID_HARDWARE_REVISION_STRING,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE */
	0x1E,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_HARDWARE_REVISION_STRING>>8,UUID_HARDWARE_REVISION_STRING,	0x0E,	'5','5','0','5',' ','1','0','2','_','L','E','S','D','K',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */
	0x0B,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						READ,	0x00,0x00,	UUID_FIRMWARE_REVISION_STRING>>8,UUID_FIRMWARE_REVISION_STRING,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE */
	0x1E,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_FIRMWARE_REVISION_STRING>>8,UUID_FIRMWARE_REVISION_STRING,	0x06,	0x30,0x30,0x39,0x30,0x31,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */
	0x0B,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						READ,	0x00,0x00,	UUID_SOFTWARE_REVISION_STRING>>8,UUID_SOFTWARE_REVISION_STRING,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE */
	0x1E,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_SOFTWARE_REVISION_STRING>>8,UUID_SOFTWARE_REVISION_STRING,	0x04,	0x30,0x30,0x30,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */
	0x0B,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						READ,	0x00,0x00,	UUID_SYSTEM_ID>>8,UUID_SYSTEM_ID,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE */
	0x1E,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_SYSTEM_ID>>8,UUID_SYSTEM_ID,								0x08,	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */
	0x0B,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						READ,	0x00,0x00,	UUID_IEEE_11073_20601>>8,UUID_IEEE_11073_20601,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_VALUE */
	0x1E,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_IEEE_11073_20601>>8,UUID_IEEE_11073_20601,					0x08,	0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

static u16 app_create_connection(u8 addrType, u8 XDATA * p_addr)large reentrant
{
	XDATA BLE_GAP_CreateConnParas	paras;
	
	paras.scan_interval=0x0020;
	paras.scan_window=0x0020;				
	paras.peer_addr.addr_type=addrType;				
   	xmemcpy((u8 XDATA *)paras.peer_addr.addr,(u8 XDATA *)(p_addr),sizeof(paras.peer_addr.addr));
   	paras.conn_paras.interval_min=0x0008;
   	paras.conn_paras.interval_max=0x0008;
   	paras.conn_paras.latency=0x0000;
   	paras.conn_paras.supervision_tmieout=0x0100;

	return BLE_GAP_CreateConnection(&paras);
}

static void app_server_return_credit(u8 credit)large reentrant
{
	XDATA BLE_GATTS_HandleValueParams p_hvParams;
	XDATA u8 payload[5];
	
	p_hvParams.charHandle = localHandleList.transCtrl;
	p_hvParams.charLength = 0x05;
	payload[0]=CREDIT_CTRL_STATUS_SUCCESS;
	payload[1]=TR_READ_MTU_SIZE_AND_ENABLE_CREDIT_CTRL;
	payload[2]=appList.attMtu>>8;
	payload[3]=appList.attMtu;
	payload[4]=credit;
	xmemcpy(p_hvParams.charValue, payload, p_hvParams.charLength);
	p_hvParams.sendType = BLE_GATT_HV_NOTIFICATION;
						
	if(BLE_GATTS_SendHandleValue(appList.connHandle, &p_hvParams)==MBIOT_RES_SUCCESS)
	{
		appList.credit=0;
		APP_TIMER_FastTimer_Stop(APP_TIMER_ID_3);
	}
	else
	{
		APP_TIMER_FastTimer_Start(APP_TIMER_ID_3, 20);
	}
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

static u16 app_start_scanning()large reentrant
{
	XDATA BLE_GAP_ScanningParas	scanParas;
	XDATA u16 result;
	scanParas.filterDuplicate=BLE_GAP_SCAN_FILTER_DUPLICATES_ENABLE;
	scanParas.type=BLE_GAP_SCAN_TYPE_PASSIVE_SCAN;
	scanParas.interval=0x0020;
	scanParas.window=0x0010;
	APP_TIMER_SlowTimer_Start(APP_TIMER_ID_2, 8);
	result= BLE_GAP_Scanning_Start(&scanParas);
	app_flash_led(1,3,3,30);
	
	return result;
}

static u16 app_start_advertising()large reentrant
{
	XDATA BLE_GAP_AdvertisingParas advpara;
	XDATA BLE_GAP_SetAdvertisingDataParas	advData;
	XDATA u16 result;
	XDATA u8 temp[]={0x02,0x01,0x06,0x09,0x09,'B','L','E','S','D','K','0','1',0x05,0xff,'B','B','0','1'};

	advpara.type=BLE_GAP_ADV_TYPE_ADV_IND;
	advpara.interval=0x0020;		
	advData.adLen=sizeof(temp);
	advData.adScanResLen=0;
	xmemcpy(advData.adData,temp,advData.adLen);
	BLE_GAP_Set_AdvertisingData(&advData);
	result=BLE_GAP_Advertising_Start(&advpara);
	app_flash_led(1,3,1,60);	// LED parameter default: (1,3,1,60)
								// LED parameter OTA test: (30,1,1,100)
	return result;
}
static u16 process_data()large reentrant
{	
#ifdef APP_OPEN_DEBUG_MODE
	if(appList.role==BLE_GAP_ROLE_SLAVE)
	{
		XDATA BLE_GATTS_HandleValueParams  hvParams;

		hvParams.charHandle = localHandleList.transTx;
		hvParams.charLength = PacketIn[appList.inReadIndex].length;
		xmemcpy_u16(hvParams.charValue, PacketIn[appList.inReadIndex].packet, hvParams.charLength);
		hvParams.sendType = BLE_GATT_HV_NOTIFICATION;
		return BLE_GATTS_SendHandleValue(appList.connHandle, &hvParams);
	}
	else if(appList.role==BLE_GAP_ROLE_MASTER)
	{
		XDATA BLE_GATTC_WriteParams  wParams;

		wParams.charHandle=remoteHandleList.transRx;
		wParams.charLength=PacketIn[appList.inReadIndex].length;
		xmemcpy_u16(wParams.charValue, PacketIn[appList.inReadIndex].packet, wParams.charLength);
		wParams.writeType=BLE_GATT_TYPE_WRITE_CMD;
		wParams.valueOffset=0x0000;
		wParams.flags=0;
	
		return BLE_GATTC_Write(appList.connHandle, &wParams);
	}
#else
	return APP_UART_Put(PacketIn[appList.inReadIndex].length,PacketIn[appList.inReadIndex].packet);	
#endif
}

static u16 app_master_ctrl_data_receive(u16 option) large reentrant
{
	XDATA BLE_GATTC_WriteParams  wParams;
	XDATA u16 payload;
	XDATA u16 result;

	payload=option;
	wParams.charHandle=remoteHandleList.txCCCD;
	wParams.charLength=sizeof(payload);
	wParams.charValue[0]=payload>>8;
	wParams.charValue[1]=(u8)payload;
	wParams.writeType=BLE_GATT_TYPE_WRITE_REQ;
	wParams.valueOffset=0x0000;
	wParams.flags=0;
	
	result=BLE_GATTC_Write(appList.connHandle, &wParams);	
	if(result==MBIOT_RES_SUCCESS)
	{
		if(option==CLIENT_CHAR_CONFIG_DESCRIPTOR_ENABLE)
		{
			appList.writeFlag=WRITE_DATA_RECEIVE_CTRL_ENABLE;
		}
		else
		{
			appList.writeFlag=WRITE_DATA_RECEIVE_CTRL_DISABLE;
		}
	}
	
	return result;
}

static void app_master_restart_data_receive() large reentrant
{
	XDATA u16 result;
	if(appList.inUsedNum<(MAX_PACKET_BUFFER_LIST-RESERVED_PACKET_BUFFER))
	{
		if(appList.writeFlag)
		{
			APP_TIMER_FastTimer_Start(APP_TIMER_ID_4, 48);
		}
		else
		{
			result=app_master_ctrl_data_receive(CLIENT_CHAR_CONFIG_DESCRIPTOR_ENABLE);
			if(result==MBIOT_RES_SUCCESS)
			{
				appList.disableReceive=0;
			}
			else
			{	
				APP_TIMER_FastTimer_Start(APP_TIMER_ID_4, 48);
			}
		}
	}	
}

static void app_master_enable_credit_flow_control() large reentrant
{
	XDATA BLE_GATTC_WriteParams  wParams;
	XDATA u16 result;

	wParams.charHandle=remoteHandleList.ctrlCCCD;
	wParams.charLength=0x02;
	wParams.charValue[0]=(u8)NOTIFICATION;
	wParams.charValue[1]=NOTIFICATION>>8;
	wParams.writeType=BLE_GATT_TYPE_WRITE_REQ;
	wParams.valueOffset=0x0000;
	wParams.flags=0;
	
	result=BLE_GATTC_Write(appList.connHandle, &wParams);
	if(result==MBIOT_RES_SUCCESS)
	{
		appList.writeFlag=WRITE_ENABLE_CREDIT_FLOW_CTRL;
	}	
}

static void app_master_initiate_credit_flow_control() large reentrant
{
	XDATA BLE_GATTC_WriteParams  wParams;
	XDATA u8 result;
	
	wParams.charHandle=remoteHandleList.transCtrl;
	wParams.charLength=0x01;
	wParams.charValue[0]=TR_READ_MTU_SIZE_AND_ENABLE_CREDIT_CTRL;
	wParams.writeType=BLE_GATT_TYPE_WRITE_REQ;
	wParams.valueOffset=0x0000;
	wParams.flags=0;
	
	result=BLE_GATTC_Write(appList.connHandle, &wParams);
	if(result==MBIOT_RES_SUCCESS)
	{
		appList.writeFlag=WRITE_INITIATE_CREDIT_FLOW_CTRL;
	}
}

static void app_master_check_credit(u8 XDATA *payload) large reentrant
{
	if((payload[0]==CREDIT_CTRL_STATUS_SUCCESS)&& (payload[1]==TR_READ_MTU_SIZE_AND_ENABLE_CREDIT_CTRL))
	{
		appList.creditBasedEnable=CREDIT_BASED_FLOW_CONTROL_ENABLED;
		appList.attMtu=((u16)payload[2]<<8)|payload[3];
		appList.credit+=payload[4];
	}
}

static void app_process_incomming_packet() large reentrant
{
	XDATA u16 result;	
	do
	{
		result=process_data();
		if(result==MBIOT_RES_SUCCESS)
		{
			APP_HEAP_Free(PacketIn[appList.inReadIndex].packet);
			if(appList.role==BLE_GAP_ROLE_SLAVE)
			{
				if((appList.creditBasedEnable==CREDIT_BASED_FLOW_CONTROL_ENABLED) 
					&& (PacketIn[appList.inReadIndex].write_type == BLE_GATT_TYPE_WRITE_CMD))
				{
					appList.credit++;
					app_server_return_credit(appList.credit);
				}
			}

			appList.inUsedNum--;
			appList.inReadIndex++;
			if(appList.inReadIndex==MAX_PACKET_BUFFER_LIST)
			{
				appList.inReadIndex=0;
			}
		}
	}while((appList.inUsedNum)&&(result==MBIOT_RES_SUCCESS));
	
	if(result!=MBIOT_RES_SUCCESS)
	{
		APP_TIMER_FastTimer_Start(APP_TIMER_ID_1, 10);
	}
	
	if((appList.role==BLE_GAP_ROLE_MASTER)&& appList.disableReceive)
	{
		app_master_restart_data_receive();
	}		
}


static void push_into_receive_buffer(u8 len, u8 XDATA * buf ,u8 writeType)large reentrant
{
	appList.inUsedNum++;
	PacketIn[appList.inWriteIndex].length=len;
	PacketIn[appList.inWriteIndex].packet=buf;
	PacketIn[appList.inWriteIndex].write_type = writeType;
	appList.inWriteIndex++;
	if(appList.inWriteIndex==MAX_PACKET_BUFFER_LIST)
	{
		appList.inWriteIndex=0;
	}
	app_process_incomming_packet();
}

static void app_initialize(void) large reentrant
{
	appList.attMtu=BLE_ATT_ATTRIBUTE_VALUE_LEN;
	appList.appState=APP_STATE_STANDBY;
	APP_GPIO_Pin_Set(GPIO_P11);	// Status Ind. 1
	APP_GPIO_Pin_Clear(GPIO_P10);	// Status Ind. 2
	app_start_scanning();
}

static u16 send_data(void) large reentrant
{
	XDATA u16 result;
	if(appList.role==BLE_GAP_ROLE_MASTER)
	{
		XDATA BLE_GATTC_WriteParams  wParams;

		wParams.charHandle=remoteHandleList.transRx;
		wParams.charLength=PacketOut[appList.outReadIndex].length;
		xmemcpy_u16(wParams.charValue, PacketOut[appList.outReadIndex].packet, wParams.charLength);
		wParams.writeType=BLE_GATT_TYPE_WRITE_CMD;
		wParams.valueOffset=0x0000;
		wParams.flags=0;
	
		result=BLE_GATTC_Write(appList.connHandle, &wParams);
	}
	else
	{
		XDATA BLE_GATTS_HandleValueParams  hvParams;

		hvParams.charHandle = localHandleList.transTx;
		hvParams.charLength = PacketOut[appList.outReadIndex].length;
		xmemcpy_u16(hvParams.charValue, PacketOut[appList.outReadIndex].packet, hvParams.charLength);
		hvParams.sendType = BLE_GATT_HV_NOTIFICATION;
		result= BLE_GATTS_SendHandleValue(appList.connHandle, &hvParams);
		//result= APP_UART_Put(PacketOut[appList.outReadIndex].length, PacketOut[appList.outReadIndex].packet);
	}
	return result;
}

static void app_process_incomming_uart() large reentrant
{
	XDATA u16 result;
	
	if((appList.role==BLE_GAP_ROLE_MASTER)
		&& (appList.creditBasedEnable==CREDIT_BASED_FLOW_CONTROL_ENABLED)
		&& (appList.credit==0))
	{
		return;		
	}
	
	do
	{
		result=send_data();
		if(result==MBIOT_RES_SUCCESS)
		{
			APP_HEAP_Free(PacketOut[appList.outReadIndex].packet);

			if((appList.role==BLE_GAP_ROLE_MASTER)&& (appList.creditBasedEnable==CREDIT_BASED_FLOW_CONTROL_ENABLED))
				appList.credit--;

			appList.outUsedNum--;
			appList.outReadIndex++;
			if(appList.outReadIndex==MAX_PACKET_BUFFER_LIST)
			{
				appList.outReadIndex=0;
			}
		}

		if((appList.role==BLE_GAP_ROLE_MASTER)
			&& (appList.creditBasedEnable==CREDIT_BASED_FLOW_CONTROL_ENABLED)
			&& (appList.credit==0))
		{
			if((appList.outUsedNum)&&(result==MBIOT_RES_SUCCESS))
				APP_TIMER_FastTimer_Start(APP_TIMER_ID_0, 48); //30ms
				
			break;
		}
			
	}while((appList.outUsedNum)&&(result==MBIOT_RES_SUCCESS));

	if(result!=MBIOT_RES_SUCCESS)
	{
		APP_TIMER_FastTimer_Start(APP_TIMER_ID_0, 48); //30ms
	}	
}

static void push_into_transmit_buffer(u8 len, u8 XDATA * buf)large reentrant
{
	appList.outUsedNum++;
	PacketOut[appList.outWriteIndex].length=len;
	PacketOut[appList.outWriteIndex].packet=buf;
	appList.outWriteIndex++;

	if(appList.outWriteIndex==MAX_PACKET_BUFFER_LIST)
	{
		appList.outWriteIndex=0;
	}

	if(appList.appState==APP_STATE_LE_OPEN)
	{
		app_process_incomming_uart();
	}
}

PUBLIC void gap_event_handler(BLE_GAP_Event XDATA * p_event) large reentrant
{
	switch(p_event->eventID)
	{
		case BLE_GAP_EVT_CONNECTED:
		{
			APP_TIMER_SlowTimer_Stop(APP_TIMER_ID_5);
			
			appList.appState=APP_STATE_LE_CONNECTED;
			appList.connHandle=p_event->eventField.evtConnect.connHandle;
			appList.role=p_event->eventField.evtConnect.role;
			BLE_GAP_Scanning_Stop();
			BLE_GAP_Advertising_Stop();
			APP_TIMER_SlowTimer_Stop(APP_TIMER_ID_2);
			app_flash_led(1,3,2,30);

			APP_GPIO_Pin_Clear(GPIO_P11);	// Status Ind. 1
			APP_GPIO_Pin_Clear(GPIO_P10);	// Status Ind. 2
			
			if(appList.role==BLE_GAP_ROLE_MASTER)
			{
				BLE_GATTC_DiscoverAllCharacteristics(appList.connHandle, 0x0001,0xFFFF);
			}
		}
		break;
		
		case BLE_GAP_EVT_DISCONNECTED:
		{
			xmemset_u16((u8 XDATA *)&appList,0,sizeof(App_List));
			appList.attMtu=BLE_ATT_ATTRIBUTE_VALUE_LEN;
			appList.appState=APP_STATE_STANDBY;
			APP_GPIO_Pin_Set(GPIO_P11);	// Status Ind. 1
			APP_GPIO_Pin_Clear(GPIO_P10);	// Status Ind. 2
			app_start_advertising();
			APP_TIMER_SlowTimer_Start(APP_TIMER_ID_5, 96);
		}
		break;
		
		case BLE_GAP_EVT_CONN_PARA_UPDATE:
		{
		}
		break;
		
		case BLE_GAP_EVT_ENCRYPT_STATUS:
		{
		}
		break;
		
		case BLE_GAP_EVT_ADVERTISING_REPORT:
		{
			XDATA u8 index=0;

			do
			{
				if(p_event->eventField.evtAdvReport.advData[index+1]==AD_TYPE_MANUFACTURE)
				{
					if(!comemcmp(&p_event->eventField.evtAdvReport.advData[index+2],SPECIFIC_PATTERN,sizeof(SPECIFIC_PATTERN)))
					{
						APP_TIMER_SlowTimer_Stop(APP_TIMER_ID_2);
						app_create_connection(p_event->eventField.evtAdvReport.addrType, p_event->eventField.evtAdvReport.addr);
					}
					break;
				}
				index+=p_event->eventField.evtAdvReport.advData[index]+1;
				
			}while(index<p_event->eventField.evtAdvReport.length);
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
		}
		break;
		
		case BLE_SMP_EVT_SECURITY_REQUEST:
		{
		}
		break;
		
		case BLE_SMP_EVT_NUMERIC_COMPARISON_CONFIRM_REQUEST:
		{
		}
		break;
		
		case BLE_SMP_EVT_INPUT_PASSKEY:
		{
		}
		break;
		
		case BLE_SMP_EVT_DISPLAY_PASSKEY_REQUEST:	
		{
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
			switch(appList.writeFlag)
			{
				case WRITE_ENABLE_CREDIT_FLOW_CTRL:
				{
					appList.writeFlag=WRITE_IDLE;
					app_master_ctrl_data_receive(CLIENT_CHAR_CONFIG_DESCRIPTOR_ENABLE);
				}
				break;
				case WRITE_INITIATE_CREDIT_FLOW_CTRL:
				{
					appList.writeFlag=WRITE_IDLE;
				}
				break;
				case WRITE_DATA_RECEIVE_CTRL_ENABLE:
				{
					appList.writeFlag=WRITE_IDLE;
				}
				break;
				case WRITE_DATA_RECEIVE_CTRL_DISABLE:
				{
					appList.writeFlag=WRITE_IDLE;
				}
				break;
				case WRITE_START_CREDIT_FLOW_CTRL:
				{
					appList.writeFlag=WRITE_IDLE;
					app_master_enable_credit_flow_control();
				}
				break;
			}			
		}
		break;
		
		case BLE_GATTS_EVT_READ:
		{
			// Receive Read Request. Send read response by connection handle
		}
		break;
		
		case BLE_GATTS_EVT_WRITE:
		{
			// Receive Write Request/Command
			if(p_event->eventField.onWrite.charHandle==localHandleList.txCCCD)
			{
				XDATA u16 cccd;
				xmemcpy((u8 XDATA *)&cccd, p_event->eventField.onWrite.p_charValue, 2);
				if(cccd==CLIENT_CHAR_CONFIG_DESCRIPTOR_ENABLE)
				{
					if(!appList.updateConnPara)
					{
						XDATA BLE_L2CAP_ConnParaUpdateReqPara para;
						para.intervalMin = 0x08;
						para.intervalMax = 0x10;
						para.latency = 0;
						para.timeout = 0x0200;
						BLE_L2CAP_ConnParaUpdateReq(appList.connHandle, &para);
						appList.updateConnPara=1;
					}
										
					appList.appState=APP_STATE_LE_OPEN;
					APP_GPIO_Pin_Clear(GPIO_P11);		// Status Ind. 1
					APP_GPIO_Pin_Set(GPIO_P10);		// Status Ind. 2
					if(appList.outUsedNum)
					{
						app_process_incomming_uart();
					}
				}
				else
				{
					appList.appState=APP_STATE_LE_CONNECTED;
					APP_GPIO_Pin_Clear(GPIO_P11);		// Status Ind. 1
					APP_GPIO_Pin_Clear(GPIO_P10);		// Status Ind. 2
				}
			}
			else if(p_event->eventField.onWrite.charHandle==localHandleList.transRx)
			{
				if(appList.inUsedNum<MAX_PACKET_BUFFER_LIST)
				{
					XDATA u8 XDATA *p_buffer;					
					p_buffer=APP_HEAP_Alloc(p_event->eventField.onWrite.writeDataLength);
					if(p_buffer!=NULL)
					{
						xmemcpy(p_buffer, p_event->eventField.onWrite.p_charValue, p_event->eventField.onWrite.writeDataLength);
						push_into_receive_buffer(p_event->eventField.onWrite.writeDataLength,p_buffer,p_event->eventField.onWrite.writeType);
					}
				}
			}
			else if(p_event->eventField.onWrite.charHandle==localHandleList.transCtrl)
			{
				if(p_event->eventField.onWrite.p_charValue[0]==TR_READ_MTU_SIZE_AND_ENABLE_CREDIT_CTRL)
				{
					appList.creditBasedEnable=CREDIT_BASED_FLOW_CONTROL_ENABLED;
					app_server_return_credit(MAX_PACKET_BUFFER_LIST);
				}
			}
		}
		break;
		
		case BLE_GATTS_EVT_HV_CONFIRM:
		{
			// Indication ACK
		}
		break;
		
		case BLE_GATTC_EVT_DISC_PRIM_SERV_RESP:
		{
		}
		break;
		
		case BLE_GATTC_EVT_DISC_CHAR_RESP:
		{
		/*Get TX/RX CCCD*/
		
			{
				XDATA u8 tempUUID[48]={UUID_MCHP_TRANS_TX_INVERSE_16, UUID_MCHP_TRANS_RX_INVERSE_16, UUID_MCHP_TRANS_CTRL_INVERSE_16};
				XDATA u8 i = 0, basic=0x00;
				
				for(i = 0; i <CHECK_NUM; i++)
				{
					basic = 0x00;
					do
					{
						XDATA u8 XDATA *p_compare;						
						
						p_compare=(u8 XDATA *)(p_event->eventField.onDiscCharResp.p_attrData+basic+0x05);

						if(xmemcmp(p_compare,&tempUUID[(i*16)], p_event->eventField.onDiscCharResp.attrPairLength-0x05)== 0x00)
						{
							if(i == CHECK_TRANS_TX)
							{
								EQU_Swap_u16pt(&remoteHandleList.transTx, (p_event->eventField.onDiscCharResp.p_attrData+basic+0x03));
								remoteHandleList.txCCCD=remoteHandleList.transTx+1;	
							}
							else if(i == CHECK_TRANS_RX)
							{
								EQU_Swap_u16pt(&remoteHandleList.transRx, (p_event->eventField.onDiscCharResp.p_attrData+basic+0x03));
							}
							else if(i == CHECK_TRANS_CTRL)
							{
								EQU_Swap_u16pt(&remoteHandleList.transCtrl, (p_event->eventField.onDiscCharResp.p_attrData+basic+0x03));
								appList.writeFlag=WRITE_START_CREDIT_FLOW_CTRL;
								remoteHandleList.ctrlCCCD=remoteHandleList.transCtrl+1;
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
		}
		break;
		
		case BLE_GATTC_EVT_READ_USING_UUID_RESP:
		{			
		}
		break;
		
		case BLE_GATTC_EVT_READ_RESP:
		{
		}
		break;
		
		case BLE_GATTC_EVT_WRITE_RESP:
		{
			// Receive Write Response, Application might do something
			switch(appList.writeFlag)
			{
				case WRITE_ENABLE_CREDIT_FLOW_CTRL:
				{
					appList.writeFlag=WRITE_IDLE;
					app_master_initiate_credit_flow_control();
				}
				break;
				case WRITE_INITIATE_CREDIT_FLOW_CTRL:
					appList.writeFlag=WRITE_IDLE;
					app_master_ctrl_data_receive(CLIENT_CHAR_CONFIG_DESCRIPTOR_ENABLE);					
				break;
				case WRITE_DATA_RECEIVE_CTRL_ENABLE:
				{
					appList.writeFlag=WRITE_IDLE;
					appList.appState=APP_STATE_LE_OPEN;
					APP_GPIO_Pin_Clear(GPIO_P11);		// Status Ind. 1
					APP_GPIO_Pin_Set(GPIO_P10);		// Status Ind. 2
				}
				break;
				case WRITE_DATA_RECEIVE_CTRL_DISABLE:
				{
					appList.writeFlag=WRITE_IDLE;
					appList.appState=APP_STATE_LE_CONNECTED;
					APP_GPIO_Pin_Clear(GPIO_P11);		// Status Ind. 1
					APP_GPIO_Pin_Clear(GPIO_P10);		// Status Ind. 2
				}
				break;
			}
		}
		break;
		
		case BLE_ATT_EVT_TIMEOUT:
		{
			// Timeout handling
		}
		break;
		
		case BLE_ATT_EVT_UPDATE_MTU:
			appList.attMtu=p_event->eventField.onUpdateMTU.exchangedMTU;
		break;
		
		case BLE_GATTC_EVT_HV_NOTIFY:
		{
			if(p_event->eventField.onNotification.charHandle==remoteHandleList.transTx)
			{
				if(appList.inUsedNum<MAX_PACKET_BUFFER_LIST)
				{
					XDATA u8 XDATA *p_buffer;
					
					p_buffer=APP_HEAP_Alloc(p_event->eventField.onNotification.receivedLength);
					if(p_buffer!=NULL)
					{
						xmemcpy(p_buffer, p_event->eventField.onNotification.p_receivedValue, p_event->eventField.onNotification.receivedLength);
						push_into_receive_buffer(p_event->eventField.onNotification.receivedLength,p_buffer,0);
					}
					else
					{
						//Drop packet
					}
				}
				else
				{
					//Drop packet
				}
				
				if((appList.inUsedNum>=(MAX_PACKET_BUFFER_LIST-3))&&(!appList.writeFlag))
				{
					appList.disableReceive=1;
					app_master_ctrl_data_receive(CLIENT_CHAR_CONFIG_DESCRIPTOR_DISABLE);
				}
			}
			else if(p_event->eventField.onNotification.charHandle==remoteHandleList.transCtrl)
			{
				app_master_check_credit( p_event->eventField.onNotification.p_receivedValue);
				if(appList.outUsedNum)
				{
					app_process_incomming_uart();
				}
			}
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
				{
					if((appList.appState==APP_STATE_LE_OPEN)&&appList.outUsedNum)
					{
						app_process_incomming_uart();
					}					
				}					
				break;
					
				case APP_TIMER_ID_1:
				{
					if(appList.inUsedNum)
					{
						app_process_incomming_packet();
					}
				}
				break;
				
				case APP_TIMER_ID_2:
					BLE_GAP_Scanning_Stop();
					app_start_advertising();
					APP_TIMER_SlowTimer_Start(APP_TIMER_ID_5, 96);
				break;
				
				case APP_TIMER_ID_3:
				{
					app_server_return_credit(appList.credit);
				}	
				break;
				
				case APP_TIMER_ID_4:
				{
					if((appList.role==BLE_GAP_ROLE_MASTER)&& appList.disableReceive)
					{
						app_master_restart_data_receive();
					}
				}
				break;

				case APP_TIMER_ID_5:
				{	
					XDATA APP_GPIO_Port_Mask	gpio;
					XDATA APP_LED_ConfParas ledParas;

					BLE_GAP_Advertising_Stop();
					APP_GPIO_Pin_Set(GPIO_P11);		// Status Ind. 1
					APP_GPIO_Pin_Set(GPIO_P10);		// Status Ind. 2
		
					ledParas.flashType=LED_TYPE_SINGLE_OFF;
					ledParas.onInterval=0;
					ledParas.offInterval=0;
					ledParas.count=0;
					ledParas.flashInterval=0;
					APP_LED_LED0_Flash(&ledParas);
					
					APP_GPIO_Pin_Config_Direction(GPIO_P10,GPIO_DIR_INPUT);// Status Ind. 2
					APP_GPIO_Pin_Config_Direction(GPIO_P11,GPIO_DIR_INPUT);// Status Ind. 1
					APP_GPIO_Pin_Config_Direction(GPIO_P36,GPIO_DIR_INPUT);//RTS
					APP_LED_Close(LED0_MASK);	
			
					gpio.maskPort0=0x00;
					gpio.maskPort1=0x00;
					gpio.maskPort2=0x00;
					gpio.maskPort3=(GPIO_PORT_MASK_BIT0|GPIO_PORT_MASK_BIT1);
					UTILITY_Shutdown(&gpio);		
				}
				break;
			}
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
			if(appList.outUsedNum<(MAX_PACKET_BUFFER_LIST-1))
			{
				XDATA u16 data_len, header_size;
				XDATA u8 XDATA *p_buffer;
				
				if(appList.role==BLE_GAP_ROLE_MASTER)
				{
					header_size = BLE_ATT_WRITE_HEADER_SIZE;
				}
				else
				{
					header_size = BLE_ATT_HANDLE_VALUE_HEADER_SIZE;
				}
				
				APP_UART_GetReceivedDataLength((u8 XDATA *)&data_len);
				if(data_len>=(appList.attMtu-header_size))
				{
					data_len=appList.attMtu-header_size;
				}
				
				p_buffer=APP_HEAP_Alloc(data_len);
				if(p_buffer!=NULL)
				{
					APP_UART_GetReceivedData(data_len,p_buffer);			
					push_into_transmit_buffer(data_len,(u8 XDATA*)p_buffer);
				}
			}
		}
		break;
		
		case APP_UART_EVT_SEND_DATA_FINISH:
			//p_event->uartEvent.dataLen;
		break;
	}
}

PUBLIC void Application_Init() large reentrant
{
	XDATA u16 startHandle;
	XDATA BLE_GATT_ServicesInfo serviceInfo;
	XDATA u8 otaAdminKey[16];
	
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
 
	localHandleList.transTx=startHandle+INDEX_MCHP_TRANS_TX_VALUE;
	localHandleList.txCCCD=startHandle+INDEX_MCHP_TRANS_TX_CCCD;
	localHandleList.transRx=startHandle+INDEX_MCHP_TRANS_RX_VALUE;
	localHandleList.transCtrl=startHandle+INDEX_MCHP_TRANS_CTRL_VALUE;
	localHandleList.ctrlCCCD=startHandle+INDEX_MCHP_TRANS_CTRL_CCCD;
	
	serviceInfo.p_definedService = (u8 XDATA *)APP_Defined_Services_DIS;
	serviceInfo.attributeNum = TOTAL_DIS_ATTR_NUM;
	serviceInfo.serviceSize = sizeof(APP_Defined_Services_DIS);
	BLE_GATTS_InitServiceTable(&serviceInfo, &startHandle);
	BLE_GATTS_EndCreatingServiceTable();
	
	//Register OTA admin key if OTA feature needed
	xmemset(otaAdminKey, 0xAA, 16);
	UTILITY_RegisterOTAKey(otaAdminKey);
	
	{
		XDATA APP_UART_ConfParas uartParas;
		
		uartParas.baudrate=APP_UART_BAUD_RATE_115200;
    	uartParas.flowContol=APP_UART_FLOW_CONTROL_ENABLE;
    	uartParas.rxNotifyLength=BLE_ATT_MAX_MTU_LENGTH;
    	uartParas.rxBufferSize=APP_RX_BUFFER_LEN;
    	uartParas.rxBuffer=AppRxBuffer;
    	uartParas.txBufferSize=APP_TX_BUFFER_LEN;
    	uartParas.txBuffer=AppTxBuffer;
    	uartParas.txIndicateInterval=5;
    	uartParas.rxWakeupPinNum=GPIO_P33;
		APP_UART_Init(&uartParas);
	}
	UTILITY_LowPowerModeControl(LOW_POWER_MODE_ALLOW);
	APP_LED_Init(LED0_MASK);	
	APP_GPIO_Pin_Config_Direction(GPIO_P10,GPIO_DIR_OUTPUT);
	APP_GPIO_Pin_Config_Direction(GPIO_P11,GPIO_DIR_OUTPUT);
	APP_GPIO_Pin_Set(GPIO_P10);
	APP_GPIO_Pin_Set(GPIO_P11);

	app_initialize();
}
