/********************************************************************
  File Information:
    FileName:       ble_ancs_service.h
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
  @addtogroup BLE_ANCS Ble Apple Notification Center Service
  @ingroup BLE_Service BLE Service
  @{
  @brief Ble Apple Notification Center Service
  @note Definitions and prototypes for the Ble Apple Notification Center Service application programming interface.
 */

typedef struct BLE_ANCS_Characteristic
{
	u16 attributeHandle;
	u8 properties;
	u16 handleValue;
	u16 handleCCD;
	u16 uuid;
	u8 uuidType;
}BLE_ANCS_Characteristic;

/**@brief Event IDs for iOS notifications. */
typedef enum
{
    BLE_ANCS_EVENT_ID_NOTIFICATION_ADDED,     /**< The iOS notification was added. */
    BLE_ANCS_EVENT_ID_NOTIFICATION_MODIFIED,  /**< The iOS notification was modified. */
    BLE_ANCS_EVENT_ID_NOTIFICATION_REMOVED    /**< The iOS notification was removed. */
} BLE_ANCS_EvtId_Values;

/**@brief Flags for iOS notifications. */
typedef struct
{
    u8 silent          : 1;  /**< If this flag is set, the notification has a low priority. */
    u8 important       : 1;  /**< If this flag is set, the notification has a high priority. */
    u8 pre_existing    : 1;  /**< If this flag is set, the notification is pre-existing. */
    u8 positive_action : 1;  /**< If this flag is set, the notification has a positive action that can be taken. */
    u8 negative_action : 1;  /**< If this flag is set, the notification has a negative action that can be taken. */
} BLE_ANCS_Notify_Flags;

/**@brief Category IDs for iOS notifications. */
typedef enum
{
    BLE_ANCS_CATEGORY_ID_OTHER,                /**< The iOS notification belongs to the "other" category.  */
    BLE_ANCS_CATEGORY_ID_INCOMING_CALL,        /**< The iOS notification belongs to the "Incoming Call" category. */
    BLE_ANCS_CATEGORY_ID_MISSED_CALL,          /**< The iOS notification belongs to the "Missed Call" category. */
    BLE_ANCS_CATEGORY_ID_VOICE_MAIL,           /**< The iOS notification belongs to the "Voice Mail" category. */
    BLE_ANCS_CATEGORY_ID_SOCIAL,               /**< The iOS notification belongs to the "Social" category. */
    BLE_ANCS_CATEGORY_ID_SCHEDULE,             /**< The iOS notification belongs to the "Schedule" category. */
    BLE_ANCS_CATEGORY_ID_EMAIL,                /**< The iOS notification belongs to the "E-mail" category. */
    BLE_ANCS_CATEGORY_ID_NEWS,                 /**< The iOS notification belongs to the "News" category. */
    BLE_ANCS_CATEGORY_ID_HEALTH_AND_FITNESS,   /**< The iOS notification belongs to the "Health and Fitness" category. */
    BLE_ANCS_CATEGORY_ID_BUSINESS_AND_FINANCE, /**< The iOS notification belongs to the "Buisness and Finance" category. */
    BLE_ANCS_CATEGORY_ID_LOCATION,             /**< The iOS notification belongs to the "Location" category. */
    BLE_ANCS_CATEGORY_ID_ENTERTAINMENT         /**< The iOS notification belongs to the "Entertainment" category. */
}BLE_ANCS_CategoryId_Values;

/**@brief iOS notification structure. */
typedef struct BLE_ANCS_Evt_Notify
{
    BLE_ANCS_EvtId_Values           evtId;          /**< Whether the notification was added, removed, or modified. */
    BLE_ANCS_Notify_Flags           evtFlags;       /**< Bitmask to signal if a special condition applies to the notification, for example, "Silent" or "Important". */
    BLE_ANCS_CategoryId_Values      categoryId;     /**< Classification of the notification type, for example, email or location. */
    u8                         categoryCount;  /**< Current number of active notifications for this category ID. */
    u32                        notif_UUID;     /**< Notification UID. */
} BLE_ANCS_Evt_Notify;


/**@brief Prototype for ancs events handler */
typedef void(*ANCSProcessRoutine)(BLE_ATT_EVT_ReceiveHV_t XDATA *p_onReceiveHV) large;

PUBLIC u16 BLE_ANCS_Init(ANCSProcessRoutine ancsHandler) large;

/**@brief Initialize BLE ANCS service table.
 *
 * @return @ref MBIOT_RES_SUCCESS		Initialize BLE ANCS service table successfully.
 */
PUBLIC extern u16 BLE_ANCS_Init_Service() large;

PUBLIC extern void BLE_ANCS_Catch_Event(BLE_GATT_Event XDATA * p_event) large;

PUBLIC extern u16 BLE_ANCS_Subscribe_Notification_Source(u8 connHandle, BLE_ANCS_Characteristic *p_ANCS_Source_Notify) large;

PUBLIC extern u16 BLE_ANCS_Subscribe_Data_Source(u8 connHandle, BLE_ANCS_Characteristic *p_ANCS_Data_Source) large;

/**
  @}
 */