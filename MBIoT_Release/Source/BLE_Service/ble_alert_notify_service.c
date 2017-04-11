/********************************************************************
  File Information:
    FileName:       ble_alert_notify_service.c
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
#include "ble_gatt.h"
#include "ble_alert_notify_service.h"

PUBLIC  MCODE  u8 APP_Defined_Alert_Notification_Services[]={	
	/* Alert Notification Service */
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_SERVICE_2 */
	0x0A,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_PRIMARY_SERVICE>>8,UUID_PRIMARY_SERVICE,					0x00,0x00,	UUID_ALERT_NOTIFICATION>>8,UUID_ALERT_NOTIFICATION,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */
	0x0B,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						READ,	0x00,0x00,	UUID_SUPPORTED_NEW_ALERT_CATEGORY>>8,UUID_SUPPORTED_NEW_ALERT_CATEGORY,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */
	0x09,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_SUPPORTED_NEW_ALERT_CATEGORY>>8,UUID_SUPPORTED_NEW_ALERT_CATEGORY,			0x02,	0x00, 0x00,
	
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */
	0x0B,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						NOTIFY,	0x00,0x00,	UUID_NEW_ALERT>>8,UUID_NEW_ALERT,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */
	0x08,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_NEW_ALERT>>8,UUID_NEW_ALERT,			0x01,	0x00,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_DESCRIPTOR_CCCD */
	0x08,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_CLIENT_CHARACTERISTIC_CONFIG>>8,UUID_CLIENT_CHARACTERISTIC_CONFIG,	0x00,0x00,	
	
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */
	0x0B,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						READ,	0x00,0x00,	UUID_SUPPORTED_UNREAD_ALERT_CATEGORY>>8,UUID_SUPPORTED_UNREAD_ALERT_CATEGORY,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */
	0x09,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_SUPPORTED_UNREAD_ALERT_CATEGORY>>8,UUID_SUPPORTED_UNREAD_ALERT_CATEGORY,			0x02,	0x00, 0x00,
	
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */
	0x0B,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						NOTIFY,	0x00,0x00,	UUID_UNREAD_ALERT_STATUS>>8,UUID_UNREAD_ALERT_STATUS,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */
	0x08,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_UNREAD_ALERT_STATUS>>8,UUID_UNREAD_ALERT_STATUS,			0x01,	0x00,
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_DESCRIPTOR_CCCD */
	0x08,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_CLIENT_CHARACTERISTIC_CONFIG>>8,UUID_CLIENT_CHARACTERISTIC_CONFIG,	0x00,0x00,		
	
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */
	0x0B,	0x00,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_CHARACTERISTIC>>8,UUID_CHARACTERISTIC,						WRITE,	0x00,0x00,	UUID_ALERT_NOTIFICATION_CTRL_POINT>>8,UUID_ALERT_NOTIFICATION_CTRL_POINT,	
	/* ATT_ATTRUBUTE_HEADER_2 */																		/* ATT_ATTRIBUTE_VALUE_CHARACTERISTIC_2 */
	0x08,	PM_MANUAL_WRITE_RESP,	0x00,0x00,	BLE_ATT_UUID_LENGTH_2,	UUID_ALERT_NOTIFICATION_CTRL_POINT>>8,UUID_ALERT_NOTIFICATION_CTRL_POINT,			0x01,	0x00,
};

PUBLIC XDATA ANSProcessRoutine	ANSProcess;

static u16 XDATA BLE_ANS_supportedNewAlertCategories;
static u16 XDATA BLE_ANS_supportedUnreadAlertCategories;

PUBLIC u16 XDATA supportedNewAlertCategoryHandle;
PUBLIC u16 XDATA newAlertHandle;
PUBLIC u16 XDATA supportedUnreadAlertCategoryHandle;
PUBLIC u16 XDATA unreadAlertStatusHandle;
PUBLIC u16 XDATA controlPointHandle;

PUBLIC u16 XDATA newAlertCCCDHandle;
PUBLIC u16 XDATA unreadAlertStatusCCCDHandle;

PUBLIC u16 BLE_ANS_Init(ANSProcessRoutine ansHandler) large
{
	if(ansHandler==NULL)
	{
		return MBIOT_RES_INVALID_PARA;
	}
	ANSProcess=ansHandler;
	
	return MBIOT_RES_SUCCESS;
}

/* Alert Notification Service */
PUBLIC u16 BLE_ANS_Init_Service() large
{	
	XDATA u16 startANSHandle;
	XDATA BLE_GATT_ServicesInfo serviceInfo;
	
	serviceInfo.p_definedService = (u8 XDATA *)APP_Defined_Alert_Notification_Services;
	serviceInfo.attributeNum = TOTAL_ANS_ATTR_NUM;
	serviceInfo.serviceSize = sizeof(APP_Defined_Alert_Notification_Services);
	BLE_GATTS_InitServiceTable(&serviceInfo, &startANSHandle);
	
	supportedNewAlertCategoryHandle = startANSHandle+INDEX_ANS_SUPPORTED_NEW_ALERT_CATAGORY_ID_BIT_MASK;
	newAlertHandle = startANSHandle+INDEX_ANS_NEW_ALERT_CATAGORY_ID;
	newAlertCCCDHandle = startANSHandle+INDEX_ANS_NEW_ALERT_CCCD;
	supportedUnreadAlertCategoryHandle = startANSHandle+INDEX_ANS_SUPPORTED_UNREAD_ALERT_CATAGORY_ID_BIT_MASK;
	unreadAlertStatusHandle = startANSHandle+INDEX_ANS_UNREAD_ALERT_STATUS_CATAGORY_ID;
	unreadAlertStatusCCCDHandle = startANSHandle+INDEX_ANS_UNREAD_ALERT_STATUS_CCCD;
	controlPointHandle = startANSHandle+INDEX_ANS_ALERT_NOTIFICATION_CTRL_POINT_VALUE;

	return MBIOT_RES_SUCCESS;
}

PUBLIC u16 BLE_ANS_Get_supportedNewAlertCategories(u16 *p_supportedNewAlertCategories) large
{
	*p_supportedNewAlertCategories = BLE_ANS_supportedNewAlertCategories;
	return MBIOT_RES_SUCCESS;
}

PUBLIC u16 BLE_ANS_Get_supportedUnreadAlertCategories(u16 *p_supportedUnreadAlertCategories) large
{
	*p_supportedUnreadAlertCategories = BLE_ANS_supportedUnreadAlertCategories;
	return MBIOT_RES_SUCCESS;
}

static void update_supportedNewAlertCategory(void) large
{
	XDATA u8 feature[2];
	
	feature[0]=BLE_ANS_supportedNewAlertCategories;
	feature[1]=BLE_ANS_supportedNewAlertCategories>>8;
	
	BLE_GATTS_SetHandleValue(supportedNewAlertCategoryHandle,0x02,&feature);
}

static void update_supportedUnreadAlertCategory(void) large
{
	XDATA u8 feature[2];
	
	feature[0]=BLE_ANS_supportedUnreadAlertCategories;
	feature[1]=BLE_ANS_supportedUnreadAlertCategories>>8;

	BLE_GATTS_SetHandleValue(supportedUnreadAlertCategoryHandle,0x02,&feature);
}

static u16 BLE_ANS_Set_SimpleAlert(bool isSimpleAlertSupported, u16 uuid) large
{
	if(uuid==UUID_SUPPORTED_NEW_ALERT_CATEGORY) {
		(isSimpleAlertSupported)?(BLE_ANS_supportedNewAlertCategories|=0x0001):(BLE_ANS_supportedNewAlertCategories&=(~0x0001));
		update_supportedNewAlertCategory();
	}	
	else
		if(uuid==UUID_SUPPORTED_UNREAD_ALERT_CATEGORY) {
			(isSimpleAlertSupported)?(BLE_ANS_supportedUnreadAlertCategories|=0x00001):(BLE_ANS_supportedUnreadAlertCategories&=(~0x0001));
			update_supportedUnreadAlertCategory();
		}
	return MBIOT_RES_SUCCESS;
}
static u16 BLE_ANS_Set_Email(bool isEmailSupported, u16 uuid) large
{
	if(uuid==UUID_SUPPORTED_NEW_ALERT_CATEGORY) {
		(isEmailSupported)?(BLE_ANS_supportedNewAlertCategories|=0x0002):(BLE_ANS_supportedNewAlertCategories&=(~0x0002));
		update_supportedNewAlertCategory();
	}
	else
		if(uuid==UUID_SUPPORTED_UNREAD_ALERT_CATEGORY) {
			(isEmailSupported)?(BLE_ANS_supportedUnreadAlertCategories|=0x0002):(BLE_ANS_supportedUnreadAlertCategories&=(~0x0002));
			update_supportedUnreadAlertCategory();
		}	
	return MBIOT_RES_SUCCESS;
}
static u16 BLE_ANS_Set_News(bool isNewsSupported, u16 uuid) large
{
	if(uuid==UUID_SUPPORTED_NEW_ALERT_CATEGORY) {
		(isNewsSupported)?(BLE_ANS_supportedNewAlertCategories|=0x0004):(BLE_ANS_supportedNewAlertCategories&=(~0x0004));
		update_supportedNewAlertCategory();
	}
	else 
		if(uuid==UUID_SUPPORTED_UNREAD_ALERT_CATEGORY) {
			(isNewsSupported)?(BLE_ANS_supportedUnreadAlertCategories|=0x0004):(BLE_ANS_supportedUnreadAlertCategories&=(~0x0004));
			update_supportedUnreadAlertCategory();
		}
	return MBIOT_RES_SUCCESS;
}
static u16 BLE_ANS_Set_Call(bool isCallSupported, u16 uuid) large
{
	if(uuid==UUID_SUPPORTED_NEW_ALERT_CATEGORY) {
		(isCallSupported)?(BLE_ANS_supportedNewAlertCategories|=0x0008):(BLE_ANS_supportedNewAlertCategories&=(~0x0008));
		update_supportedNewAlertCategory();
	}
	else 
		if(uuid==UUID_SUPPORTED_UNREAD_ALERT_CATEGORY) {
			(isCallSupported)?(BLE_ANS_supportedUnreadAlertCategories|=0x0008):(BLE_ANS_supportedUnreadAlertCategories&=(~0x0008));
			update_supportedUnreadAlertCategory();
		}
	return MBIOT_RES_SUCCESS;
}
static u16 BLE_ANS_Set_MissedCall(bool isMissedCallSupported, u16 uuid) large
{
	if(uuid==UUID_SUPPORTED_NEW_ALERT_CATEGORY) {
		(isMissedCallSupported)?(BLE_ANS_supportedNewAlertCategories|=0x0010):(BLE_ANS_supportedNewAlertCategories&=(~0x0010));
		update_supportedNewAlertCategory();
	}
	else 
		if(uuid==UUID_SUPPORTED_UNREAD_ALERT_CATEGORY) {
			(isMissedCallSupported)?(BLE_ANS_supportedUnreadAlertCategories|=0x0010):(BLE_ANS_supportedUnreadAlertCategories&=(~0x0010));
			update_supportedUnreadAlertCategory();
		}
	return MBIOT_RES_SUCCESS;
}
static u16 BLE_ANS_Set_MMS_SMS(bool isMMSSMSSUpported, u16 uuid) large
{
	if(uuid==UUID_SUPPORTED_NEW_ALERT_CATEGORY) {
		(isMMSSMSSUpported)?(BLE_ANS_supportedNewAlertCategories|=0x0020):(BLE_ANS_supportedNewAlertCategories&=(~0x0020));
		update_supportedNewAlertCategory();
	}
	else 
		if(uuid==UUID_SUPPORTED_UNREAD_ALERT_CATEGORY) {
			(isMMSSMSSUpported)?(BLE_ANS_supportedUnreadAlertCategories|=0x0020):(BLE_ANS_supportedUnreadAlertCategories&=(~0x0020));
			update_supportedUnreadAlertCategory();
		}	
	return MBIOT_RES_SUCCESS;
}
static u16 BLE_ANS_Set_VoiceMail(bool isVoiceMailSupported, u16 uuid) large
{
	if(uuid==UUID_SUPPORTED_NEW_ALERT_CATEGORY) {
		(isVoiceMailSupported)?(BLE_ANS_supportedNewAlertCategories|=0x0040):(BLE_ANS_supportedNewAlertCategories&=(~0x0040));
		update_supportedNewAlertCategory();
	}
	else 
		if(uuid==UUID_SUPPORTED_UNREAD_ALERT_CATEGORY) {
			(isVoiceMailSupported)?(BLE_ANS_supportedUnreadAlertCategories|=0x0040):(BLE_ANS_supportedUnreadAlertCategories&=(~0x0040));
			update_supportedUnreadAlertCategory();
		}
	return MBIOT_RES_SUCCESS;
}
static u16 BLE_ANS_Set_Schedule(bool isScheduleSupported, u16 uuid) large
{
	if(uuid==UUID_SUPPORTED_NEW_ALERT_CATEGORY) {
		(isScheduleSupported)?(BLE_ANS_supportedNewAlertCategories|=0x0080):(BLE_ANS_supportedNewAlertCategories&=(~0x0080));
		update_supportedNewAlertCategory();
	}
	else
		if(uuid==UUID_SUPPORTED_UNREAD_ALERT_CATEGORY) {
			(isScheduleSupported)?(BLE_ANS_supportedUnreadAlertCategories|=0x0080):(BLE_ANS_supportedUnreadAlertCategories&=(~0x0080));
			update_supportedUnreadAlertCategory();
		}
	return MBIOT_RES_SUCCESS;
}

static u16 BLE_ANS_Set_HighPrioritizedAlert(bool isHighPrioritizedAlertSupported, u16 uuid) large
{
	if(uuid==UUID_SUPPORTED_NEW_ALERT_CATEGORY) {
		(isHighPrioritizedAlertSupported)?(BLE_ANS_supportedNewAlertCategories|=0x0100):(BLE_ANS_supportedNewAlertCategories&=(~0x0100));
		update_supportedNewAlertCategory();
	}
	else
		if(uuid==UUID_SUPPORTED_UNREAD_ALERT_CATEGORY) {
			(isHighPrioritizedAlertSupported)?(BLE_ANS_supportedUnreadAlertCategories|=0x0100):(BLE_ANS_supportedUnreadAlertCategories&=(~0x0100));
			update_supportedUnreadAlertCategory();
		}
	return MBIOT_RES_SUCCESS;
}

static u16 BLE_ANS_Set_InstantMessage(bool isInstantMessageSupported, u16 uuid) large
{
	if(uuid==UUID_SUPPORTED_NEW_ALERT_CATEGORY) {
		(isInstantMessageSupported)?(BLE_ANS_supportedNewAlertCategories|=0x0200):(BLE_ANS_supportedNewAlertCategories&=(~0x0200));
		update_supportedNewAlertCategory();
	}
	else
		if(uuid==UUID_SUPPORTED_UNREAD_ALERT_CATEGORY) {
			(isInstantMessageSupported)?(BLE_ANS_supportedUnreadAlertCategories|=0x0200):(BLE_ANS_supportedUnreadAlertCategories&=(~0x0200));
			update_supportedUnreadAlertCategory();
		}
	return MBIOT_RES_SUCCESS;
}

PUBLIC u16 BLE_ANS_Add_SupportedNewAlertCategories() large
{
	BLE_ANS_Set_SimpleAlert(1,UUID_SUPPORTED_NEW_ALERT_CATEGORY);
	BLE_ANS_Set_Email(1, UUID_SUPPORTED_NEW_ALERT_CATEGORY);
	BLE_ANS_Set_News(1, UUID_SUPPORTED_NEW_ALERT_CATEGORY);
	BLE_ANS_Set_Call(1, UUID_SUPPORTED_NEW_ALERT_CATEGORY);
	BLE_ANS_Set_MissedCall(1, UUID_SUPPORTED_NEW_ALERT_CATEGORY);
	BLE_ANS_Set_MMS_SMS(1, UUID_SUPPORTED_NEW_ALERT_CATEGORY);
	BLE_ANS_Set_VoiceMail(1, UUID_SUPPORTED_NEW_ALERT_CATEGORY);
	BLE_ANS_Set_Schedule(1, UUID_SUPPORTED_NEW_ALERT_CATEGORY);	
	BLE_ANS_Set_HighPrioritizedAlert(1, UUID_SUPPORTED_NEW_ALERT_CATEGORY);
	BLE_ANS_Set_InstantMessage(1, UUID_SUPPORTED_NEW_ALERT_CATEGORY);
	
	return MBIOT_RES_SUCCESS;
}

PUBLIC u16 BLE_ANS_Add_SupportedUnreadAlertCategories() large
{
	BLE_ANS_Set_SimpleAlert(1,UUID_SUPPORTED_UNREAD_ALERT_CATEGORY);
	BLE_ANS_Set_Email(1, UUID_SUPPORTED_UNREAD_ALERT_CATEGORY);
	BLE_ANS_Set_News(1, UUID_SUPPORTED_UNREAD_ALERT_CATEGORY);
	BLE_ANS_Set_Call(1, UUID_SUPPORTED_UNREAD_ALERT_CATEGORY);
	BLE_ANS_Set_MissedCall(1, UUID_SUPPORTED_UNREAD_ALERT_CATEGORY);
	BLE_ANS_Set_MMS_SMS(1, UUID_SUPPORTED_UNREAD_ALERT_CATEGORY);
	BLE_ANS_Set_VoiceMail(1, UUID_SUPPORTED_UNREAD_ALERT_CATEGORY);
	BLE_ANS_Set_Schedule(1, UUID_SUPPORTED_UNREAD_ALERT_CATEGORY);
	BLE_ANS_Set_HighPrioritizedAlert(1, UUID_SUPPORTED_UNREAD_ALERT_CATEGORY);
	BLE_ANS_Set_InstantMessage(1, UUID_SUPPORTED_UNREAD_ALERT_CATEGORY);
		
	return MBIOT_RES_SUCCESS;
}

static void BLE_ANS_Event_Write(BLE_GATT_Event XDATA * p_event) large
{
	XDATA u16 handle;

	handle = p_event->eventField.onWrite.charHandle;
	if((handle == newAlertCCCDHandle)|| (handle == unreadAlertStatusCCCDHandle)|| (handle == controlPointHandle))
	{
		XDATA u16 write_value;
		XDATA BLE_ANS_Event XDATA *p_ansEvent;
		xmemcpy((u8 XDATA *)&write_value, p_event->eventField.onWrite.p_charValue, p_event->eventField.onWrite.writeDataLength);
		p_ansEvent = (BLE_ANS_Event XDATA *)APP_HEAP_Alloc(sizeof(BLE_ANS_Event));
		if(p_ansEvent!= NULL)
		{
			p_ansEvent->handle = handle;
			p_ansEvent->write_value = write_value;
			ANSProcess(p_ansEvent);
			APP_HEAP_Free((u8 XDATA *)p_ansEvent);
		}
	}
}

PUBLIC void BLE_ANS_Catch_Event(BLE_GATT_Event XDATA * p_event) large
{	
	switch(p_event->eventID)
	{
		case BLE_GATTS_EVT_WRITE:
		{
				BLE_ANS_Event_Write(p_event);
		}	
		break;
		default:
		{					
		}
		break;
	}
}

PUBLIC u16 BLE_ANS_Send_New_Alert(u8 connHandle, BLE_ANS_NewAlert_Message XDATA *p_ansNewAlertMessage) large
{
	XDATA u16 result;
	XDATA u8 totalMessageLength;					
	
	XDATA BLE_GATTS_HandleValueParams XDATA *p_value;
	p_value = (BLE_GATTS_HandleValueParams*)APP_HEAP_Alloc(sizeof(BLE_GATTS_HandleValueParams));
	if(p_value!=NULL)
	{
		p_value->sendType=BLE_GATT_HV_NOTIFICATION;
		xmemcpy(&p_value->charValue,&p_ansNewAlertMessage->category_id,sizeof(p_ansNewAlertMessage->category_id));
		totalMessageLength = sizeof(p_ansNewAlertMessage->category_id);
		xmemcpy((&p_value->charValue+totalMessageLength),&p_ansNewAlertMessage->no_alerts,sizeof(p_ansNewAlertMessage->no_alerts));
		totalMessageLength += sizeof(p_ansNewAlertMessage->no_alerts);
		xmemcpy((&p_value->charValue+totalMessageLength),p_ansNewAlertMessage->info,p_ansNewAlertMessage->info_length);
		totalMessageLength += p_ansNewAlertMessage->info_length;
		p_value->charHandle=newAlertHandle;
		p_value->charLength=totalMessageLength;
		result = BLE_GATTS_SendHandleValue(connHandle, p_value);
		APP_HEAP_Free((u8 XDATA *)p_value);
		return result;
	}
	return MBIOT_RES_OOM;
}

PUBLIC u16 BLE_ANS_Send_Unread_Alert_Status(u8 connHandle, BLE_ANS_UnreadAlertStatus_Message XDATA *p_ansUnreadAlertStatusMessage) large
{
	XDATA u8 totalMessageLength;
	XDATA u16 result;
	
	XDATA BLE_GATTS_HandleValueParams XDATA *p_value;
	p_value = (BLE_GATTS_HandleValueParams*)APP_HEAP_Alloc(sizeof(BLE_GATTS_HandleValueParams));
	if(p_value!=NULL)
	{
		p_value->sendType=BLE_GATT_HV_NOTIFICATION;
		xmemcpy(&p_value->charValue,&p_ansUnreadAlertStatusMessage->category_id,sizeof(p_ansUnreadAlertStatusMessage->category_id));
		totalMessageLength = sizeof(p_ansUnreadAlertStatusMessage->category_id);
		xmemcpy((&p_value->charValue+totalMessageLength),&p_ansUnreadAlertStatusMessage->unread_count,sizeof(p_ansUnreadAlertStatusMessage->unread_count));
		totalMessageLength += sizeof(p_ansUnreadAlertStatusMessage->unread_count);
		p_value->charHandle=unreadAlertStatusHandle;
		p_value->charLength=totalMessageLength;
		result = BLE_GATTS_SendHandleValue(connHandle, p_value);
		APP_HEAP_Free((u8 XDATA *)p_value);
		return result;
	}		
	return MBIOT_RES_OOM;
}

PUBLIC u16 BLE_ANS_Send_Write_Response(u8 connHandle, u8 respType) large
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

PUBLIC u16 BLE_ANS_Send_Error_Response(u8 connHandle, u8 errCode) large
{
	XDATA BLE_GATTS_SendErrResponseParams XDATA * p_errParams;
	XDATA u16 result;
	
	p_errParams = (BLE_GATTS_SendErrResponseParams XDATA *)APP_HEAP_Alloc(sizeof(BLE_GATTS_SendErrResponseParams));
	if(p_errParams!=NULL) {		
		p_errParams->reqOpcode = ATT_WRITE_REQUEST;
		p_errParams->charHandle = controlPointHandle;
		p_errParams->errorCode = errCode;
		result = BLE_GATTS_SendErrorResponse(connHandle, p_errParams);
		APP_HEAP_Free((u8 XDATA *)p_errParams);
		return result;
	}
	return MBIOT_RES_OOM;
}