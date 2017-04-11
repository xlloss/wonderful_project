/********************************************************************
  File Information:
    FileName:       homekit_accessory_information_service.h
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
  @addtogroup HAIS Homekit Accessory Information Service
  @ingroup BLE_Service BLE Service
  @{
  @brief Homekit Accessory Information Service
  @note Definitions and prototypes for the Ble Homekit Accessory Information Service application programming interface.
 */
 
#ifndef HOMEKIT_ACCESSORY_INFORMATION_SERVICE_H
#define HOMEKIT_ACCESSORY_INFORMATION_SERVICE_H

#define UUID_ACCESSORY_INFO_SERVICE									0x00, 0x00, 0xFE, 0xD3, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB		/**< Accessory Information Service 128-bit UUID. */
#define UUID_ACCESSORY_INFO_IDENTIFY								0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x26, 0xBB, 0x76, 0x52, 0x91		/**< Identify 128-bit UUID. */
#define UUID_ACCESSORY_INFO_MANUFACTURER							0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x26, 0xBB, 0x76, 0x52, 0x91		/**< Manufacturer 128-bit UUID. */
#define UUID_ACCESSORY_INFO_MODEL									0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x26, 0xBB, 0x76, 0x52, 0x91		/**< Model 128-bit UUID. */
#define UUID_ACCESSORY_INFO_NAME									0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x26, 0xBB, 0x76, 0x52, 0x91		/**< Name 128-bit UUID. */
#define UUID_ACCESSORY_INFO_SERIAL_NUMBER							0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x26, 0xBB, 0x76, 0x52, 0x91		/**< Serial Number 128-bit UUID. */ 
#define UUID_ACCESSORY_INFO_FIRMWARE_REV							0x00, 0x00, 0x00, 0x52, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x26, 0xBB, 0x76, 0x52, 0x91		/**< Firmware Revision 128-bit UUID. */
#define UUID_ACCESSORY_INFO_HARDWARE_REV							0x00, 0x00, 0x00, 0x53, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x26, 0xBB, 0x76, 0x52, 0x91		/**< Hardware Revision 128-bit UUID. */
#define UUID_ACCESSORY_INFO_SOFTWARE_REV							0x00, 0x00, 0x00, 0x54, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x26, 0xBB, 0x76, 0x52, 0x91		/**< Software Revision 128-bit UUID. */
#define UUID_ACCESSORY_INFO_SERVICE_INSTANCE_ID						0x00, 0x00, 0x00, 0x51, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x26, 0xBB, 0x76, 0x52, 0x91		/**< Service Instance 128-bit UUID. */
#define UUID_ACCESSORY_INFO_CHAR_INSTANCE_ID						0xDC, 0x46, 0xF0, 0xFE, 0x81, 0xD2, 0x46, 0x16, 0xB5, 0xD9, 0x6A, 0xBD, 0xD7, 0x96, 0x93, 0x9A		/**< Characteristic Instance 128-bit UUID. */


enum HAIS_AttributeIndex
{
	INDEX_HAIS_PRIMARY_SERVICE=0x00,							/**< Index of homekit accessory information primary service. */
	INDEX_ACCESSORY_INFO_SERVICE_INSTANCE_ID,					/**< Index of service instance ID characteristic. */
	INDEX_ACCESSORY_INFO_SERVICE_INSTANCE_ID_VALUE,				/**< Index of service instance ID characteristic value. */
	INDEX_ACCESSORY_INFO_CHAR_INSTANCE_ID1,						/**< Index of characteristic instance ID for SERVICE INSTANCE. */
	INDEX_ACCESSORY_INFO_ID,									/**< Index of identify characteristic. */
	INDEX_ACCESSORY_INFO_ID_VALUE,								/**< Index of identify characteristic value. */
	INDEX_ACCESSORY_INFO_CHAR_INSTANCE_ID2,						/**< Index of characteristic instance ID for ID(IDENTITY). */
	INDEX_ACCESSORY_INFO_MANFACTURER,							/**< Index of manufacturer characteristic. */
	INDEX_ACCESSORY_INFO_MANFACTURER_VALUE,						/**< Index of manufacturer characteristic value. */
	INDEX_ACCESSORY_INFO_CHAR_INSTANCE_ID3,						/**< Index of characteristic instance ID for MANUFACTURE. */
	INDEX_ACCESSORY_INFO_MODEL,									/**< Index of model characteristic. */
	INDEX_ACCESSORY_INFO_MODEL_VALUE,							/**< Index of model characteristic value. */
	INDEX_ACCESSORY_INFO_CHAR_INSTANCE_ID4,						/**< Index of characteristic instance ID for MODEL. */
	INDEX_ACCESSORY_INFO_NAME,									/**< Index of name characteristic. */
	INDEX_ACCESSORY_INFO_NAME_VALUE,							/**< Index of name characteristic value. */
	INDEX_ACCESSORY_INFO_CHAR_INSTANCE_ID5,						/**< Index of characteristic instance ID for NAME. */
	INDEX_ACCESSORY_INFO_SERIAL_NUM,							/**< Index of serial number characteristic. */
	INDEX_ACCESSORY_INFO_SERIAL_NUM_VALUE,						/**< Index of serial number characteristic value. */
	INDEX_ACCESSORY_INFO_CHAR_INSTANCE_ID6,						/**< Index of characteristic instance ID for SERIAL_NUM. */
	INDEX_ACCESSORY_INFO_FIRMWARE_REV,							/**< Index of firmware revision characteristic. */
	INDEX_ACCESSORY_INFO_FIRMWARE_REV_VALUE,					/**< Index of firmware revision characteristic value. */
	INDEX_ACCESSORY_INFO_CHAR_INSTANCE_ID7,						/**< Index of characteristic instance ID for FIRMWARE_REV. */
	INDEX_ACCESSORY_INFO_HARDWARE_REV,							/**< Index of hardmware revision characteristic. */
	INDEX_ACCESSORY_INFO_HARDWARE_REV_VALUE,					/**< Index of hardmware revision characteristic value. */
	INDEX_ACCESSORY_INFO_CHAR_INSTANCE_ID8,						/**< Index of characteristic instance ID for HARDWARE_REV. */
	INDEX_ACCESSORY_INFO_SOFTWARE_REV,							/**< Index of softmware revision characteristic. */
	INDEX_ACCESSORY_INFO_SOFTWARE_REV_VALUE,					/**< Index of softmware revision characteristic value. */
	INDEX_ACCESSORY_INFO_CHAR_INSTANCE_ID9,						/**< Index of characteristic instance ID for SOFTWARE_REV. */
	TOTAL_HAIS_ATTR_NUM											/**< Total attribure number of homekit accessory information service. */
};

typedef struct Homekit_Accessory_Information_Handle_List
{
	u16 identify;
	u16	serialNum;
}Homekit_Accessory_Information_Handle_List;

PUBLIC extern XDATA Homekit_Accessory_Information_Handle_List	homekitAccessoryInfoHandle;


/**@brief Initizlize Homekit Accessory Information service table.
 *
 * @return MBIOT_RES_SUCCESS	Successfully initialize Homekit Accessory Information service table.
 * @return MBIOT_RES_FAIL		Operation is not permitted.
 */
PUBLIC extern u16 HOMEKIT_AccessoryInformation_Init_Service() large;
#endif

/**
  @}
 */