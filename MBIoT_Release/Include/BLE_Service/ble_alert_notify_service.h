/********************************************************************
  File Information:
    FileName:       ble_alert_notify_service.h
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
/**
  @addtogroup BLE_Service BLE Service
  @{
 */
 
/**
  @}
 */
 
/**
 @addtogroup BLE_ANS Ble Alert Notify Service
 @ingroup BLE_Service BLE Service
 @{
 @brief Ble Alert Notify Service
 @note Definitions and prototypes for the Ble Alert Notify Service application programming interface.
*/


#ifndef BLE_ALERT_NOTIFY_SERVICE_H
#define BLE_ALERT_NOTIFY_SERVICE_H

/**@brief Alert Notification Service UUID definations */
#define UUID_ALERT_NOTIFICATION								0x1811			/**< Definition of Alert Notification Service UUID. */
#define UUID_SUPPORTED_NEW_ALERT_CATEGORY 		0x2A47			/**< Definition of Supported New Alert Category UUID. */
#define UUID_NEW_ALERT				  							0x2A46			/**< Definition of New Alert UUID. */
#define UUID_ALERT_CATAGORY_ID 								0x2A43			/**< Definition of Alert Category ID UUID. */
#define UUID_CLIENT_CHARACTERISTIC_CONFIG 		0x2902			/**< Definition of Client Characteristic Configuration UUID. */
#define UUID_SUPPORTED_UNREAD_ALERT_CATEGORY 	0x2A48			/**< Definition of Supported Unread Alert Category UUID. */
#define UUID_UNREAD_ALERT_STATUS				  		0x2A45			/**< Definition of Unread Alert Status UUID. */
#define UUID_ALERT_NOTIFICATION_CTRL_POINT 		0x2A44			/**< Definition of Alert Notification Control Point UUID. */ 
#define UUID_ALERT_CATAGORY_ID_BIT_MASK 			0x2A42			/**< Definition of Alert Category ID Bit Mask UUID. */

/**@brief Attribute number in the app defined Service Table */
enum BLE_ANS_AttributeIndex
{
	INDEX_ANS_PRIMARY_SERVICE=0,																/**< Index of alert notification primary service. */
	INDEX_ANS_SUPPORTED_NEW_ALERT_CATEGORY,											/**< Index of supported new alert category characteristic. */
	INDEX_ANS_SUPPORTED_NEW_ALERT_CATAGORY_ID_BIT_MASK,					/**< Index of supported new alert category id bit mask. */
	INDEX_ANS_NEW_ALERT,																				/**< Index of new alert characteristic. */
	INDEX_ANS_NEW_ALERT_CATAGORY_ID,														/**< Index of new alert category id. */
	INDEX_ANS_NEW_ALERT_CCCD,																		/**< Index of new alert client characteristic configuration descriptor. */
	INDEX_ANS_SUPPORTED_UNREAD_ALERT_CATEGORY,									/**< Index of supported unread alert category characteristic. */
	INDEX_ANS_SUPPORTED_UNREAD_ALERT_CATAGORY_ID_BIT_MASK,			/**< Index of supported unread alert category id bit mask. */
	INDEX_ANS_UNREAD_ALERT_STATUS,															/**< Index of unread alert status characteristic. */
	INDEX_ANS_UNREAD_ALERT_STATUS_CATAGORY_ID,									/**< Index of unread alert status category id. */
	INDEX_ANS_UNREAD_ALERT_STATUS_CCCD,													/**< Index of unread alert status client characteristic configuration descriptor. */
	INDEX_ANS_ALERT_NOTIFICATION_CTRL_POINT,										/**< Index of alert notification control point. */
	INDEX_ANS_ALERT_NOTIFICATION_CTRL_POINT_VALUE,							/**< Index of alert notification control point value. */
	TOTAL_ANS_ATTR_NUM																					/**< Total attribure number of alert notification service. */	
};

/**@brief Alerts types as defined in the alert category id; UUID: 0x2A43. */
#define  ANS_TYPE_SIMPLE_ALERT                 0x00        /**< General text alert or non-text alert. */
#define  ANS_TYPE_EMAIL                        0x01        /**< Alert when email messages arrives. */
#define  ANS_TYPE_NEWS                         0x02        /**< News feeds such as RSS, Atom. */
#define  ANS_TYPE_NOTIFICATION_CALL            0x03        /**< Incoming call. */
#define  ANS_TYPE_MISSED_CALL                  0x04        /**< Missed call. */
#define  ANS_TYPE_SMS_MMS                      0x05        /**< SMS/MMS message arrives. */
#define  ANS_TYPE_VOICE_MAIL                   0x06        /**< Voice mail. */
#define  ANS_TYPE_SCHEDULE                     0x07        /**< Alert occurred on calendar, planner. */
#define  ANS_TYPE_HIGH_PRIORITIZED_ALERT       0x08        /**< Alert that should be handled as high priority.*/
#define  ANS_TYPE_INSTANT_MESSAGE              0x09        /**< Alert for incoming instant messages.*/
#define  ANS_TYPE_ALL_ALERTS                   0xFF        /**< Identifies All Alerts. */

/**@brief For CCCD handle */
#define ANS_NOTIFY_DISABLED					0x0000 /**< Alert Notifications are disabled */
#define ANS_NOTIFY_ENAABLED					0x0100 /**< Alert Notifications are enabled */

/**@brief Alerts notification control point commands as defined in the Alert Notification Specification;
 * UUID: 0x2A44.
 */
#define ANS_ENABLE_NEW_INCOMING_ALERT_NOTIFICATION        0      /**< Enable New Incoming Alert Notification. */
#define ANS_ENABLE_UNREAD_CATEGORY_STATUS_NOTIFICATION    1      /**< Enable Unread Category Status Notification. */
#define ANS_DISABLE_NEW_INCOMING_ALERT_NOTIFICATION       2      /**< Disable New Incoming Alert Notification.*/
#define ANS_DISABLE_UNREAD_CATEGORY_STATUS_NOTIFICATION   3      /**< Disable Unread Category Status Notification. */
#define ANS_NOTIFY_NEW_INCOMING_ALERT_IMMEDIATELY         4      /**< Notify New Incoming Alert immediately. */
#define ANS_NOTIFY_UNREAD_CATEGORY_STATUS_IMMEDIATELY     5      /**< Notify Unread Category Status immediately. */

/**@brief Error code, command not supported */
#define ANS_COMMAND_ERROR							0xA0

/**@brief New Alert parameters */
typedef struct BLE_ANS_NewAlert_Message
{
	u8 category_id;			/**< Category of the new alert. */
	u8 no_alerts;				/**< Number of new alerts in the server. */
	u8 info_length;			/**< Length of text info. */
	u8 *info;						/**< Brief text information for the last alert. */
}BLE_ANS_NewAlert_Message;

/**@brief Unread Alert Status parameters */
typedef struct BLE_ANS_UnreadAlertStatus_Message
{
	u8 category_id;			/**< Category for unread alert count. */
	u8 unread_count;		/**< Number of unread alerts exist on the server. */
}BLE_ANS_UnreadAlertStatus_Message;

/**@brief Alert Notification Control Point */
typedef struct BLE_ANS_Alert_Notification_ControlPoint
{
	u8 command_id;			/**< Command. */
	u8 category_id;			/**< Target category that the command applies to. */
}BLE_ANS_Alert_Notification_ControlPoint;

/**@brief Alert Notify State (enable/disable) */
enum alertNotifyState
{
	NOTIFY_DISABLE,	/**< Alert Notify Disable */
	NOTIFY_ENABLE,	/**< Alert Notify Enable */
};

/**@brief Alert Notification Category Status */
typedef struct BLE_ANS_AertNotifyState
{
	u16 newAlertNotifyState;				/**< New alert notify state(Notify enabled/disabled). */
	u16 unreadAlertNotifyState;			/**< Unread alert notify state(Notify enabled/disabled). */
}BLE_ANS_AertNotifyState;

/**@brief ANS Events */
typedef struct BLE_ANS_Event
{
	u16 handle;					/**< CCCD Handle. */
	u16	write_value;		/**< Event type (CCCD). */
}BLE_ANS_Event;	

/**@brief Prototype for ans events handler */
typedef void(*ANSProcessRoutine)(BLE_ANS_Event XDATA *p_ansEvent) large;

/**@brief extern CCCD handle, required in application */
PUBLIC extern u16 XDATA supportedNewAlertCategoryHandle;
PUBLIC extern u16 XDATA newAlertHandle;
PUBLIC extern u16 XDATA supportedUnreadAlertCategoryHandle;
PUBLIC extern u16 XDATA unreadAlertStatusHandle;
PUBLIC extern u16 XDATA controlPointHandle;
PUBLIC extern u16 XDATA newAlertCCCDHandle;
PUBLIC extern u16 XDATA unreadAlertStatusCCCDHandle;

/**@brief Initialize BLE ANS.
 *
 * @param[in] ansHandler		MBIoT callback function for BLE ANS events.
 *
 * @return @ref MBIOT_RES_SUCCESS		Initialize BLE ANS successfully.
 */
PUBLIC u16 BLE_ANS_Init(ANSProcessRoutine ansHandler) large;

/**@brief Initialize BLE Alert Notification service table.
 *
 * @return @ref MBIOT_RES_SUCCESS		Initialize BLE Alert Notification service table successfully.
 */
PUBLIC extern u16 BLE_ANS_Init_Service() large;

/**@brief Catch BLE_GATT events to get the ANS events.
 *
 * @param[in] p_event		Pointer to BLE_GATT_Event structure.
 *
 * @return @ref MBIOT_RES_SUCCESS		Catch BLE GATT events configuration successfully.
 */
PUBLIC extern void BLE_ANS_Catch_Event(BLE_GATT_Event XDATA * p_event) large;

/**@brief Send new alert status.
 *
 * @param[in] connHandle Connection handle.
 * @param[in] p_ansNewAlertMessage pointer to BLE_ANS_NewAlert_Message structure.
 *
 * @return @ref MBIOT_RES_SUCCESS sent new alert status successfully.
 */
PUBLIC extern u16 BLE_ANS_Send_New_Alert(u8 connHandle, BLE_ANS_NewAlert_Message XDATA *p_ansNewAlertMessage) large;

/**@brief Send unread alert status.
 *
 * @param[in] connHandle Connection handle.
 * @param[in] p_ansUnreadAlertStatusMessage pointer to BLE_ANS_UnreadAlertStatus_Message structure.
 *
 * @return @ref MBIOT_RES_SUCCESS sent unread alert status successfully.
 */
PUBLIC extern u16 BLE_ANS_Send_Unread_Alert_Status(u8 connHandle, BLE_ANS_UnreadAlertStatus_Message XDATA *p_ansUnreadAlertStatusMessage) large;

/**@brief Send write response.
 *
 * @param[in] connHandle Connection handle.
 * @param[in] respType write response type.
 *
 * @return @ref MBIOT_RES_SUCCESS sent error response successfully.
 */
PUBLIC u16 BLE_ANS_Send_Write_Response(u8 connHandle, u8 respType) large;

/**@brief Send error response.
 *
 * @param[in] connHandle Connection handle.
 * @param[in] errCode unsupported command.
 *
 * @return @ref MBIOT_RES_SUCCESS sent error response successfully.
 */
PUBLIC extern u16 BLE_ANS_Send_Error_Response(u8 connHandle, u8 errCode) large;

/**@brief Add supported new alert categories.
 *
 * @return @ref MBIOT_RES_SUCCESS Added new alert categories successfully.
 */
PUBLIC extern u16 BLE_ANS_Add_SupportedNewAlertCategories() large;

/**@brief Add supported unread alert categories.
 *
 * @return @ref MBIOT_RES_SUCCESS Added unread alert categories successfully.
 */
PUBLIC extern u16 BLE_ANS_Add_SupportedUnreadAlertCategories() large;

/**@brief Get supported new alert categories.
 *
 * @param[in] p_supportedNewAlertCategories copy supported new alert categories to this pointer.
 * @return @ref MBIOT_RES_SUCCESS  copied supported new alert categories. 
 */
PUBLIC u16 BLE_ANS_Get_supportedNewAlertCategories(u16 *p_supportedNewAlertCategories) large;

/**@brief Get supported unread alert categories.
 *
 * @param[in] p_supportedUnreadAlertCategories copy supported unread alert categories to this pointer.
 * @return @ref MBIOT_RES_SUCCESS  copied supported unread alert categories.
 */
PUBLIC u16 BLE_ANS_Get_supportedUnreadAlertCategories(u16 *p_supportedUnreadAlertCategories) large;
#endif

/**
  @}
 */