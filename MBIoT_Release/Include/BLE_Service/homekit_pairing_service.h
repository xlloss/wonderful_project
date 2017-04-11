/********************************************************************
  File Information:
    FileName:       homekit_pairing_service.h
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
  @addtogroup HPS Homekit Pairing Service
  @ingroup BLE_Service BLE Service
  @{
  @brief Homekit Pairing Service
  @note Definitions and prototypes for the Ble Homekit Pairing Service application programming interface.
 */
 
#ifndef HOMEKIT_PAIRING_SERVICE_H
#define HOMEKIT_PAIRING_SERVICE_H

#define UUID_HOMEKIT_PAIRING_SERVICE						0x00, 0x00, 0xFE, 0xD4, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB		/**< Pairing Service UUID. */
#define UUID_HOMEKIT_PAIRING_PAIR_SETUP						0x00, 0x00, 0x00, 0x4C, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x26, 0xBB, 0x76, 0x52, 0x91		/**< Pair Setup 128-bit UUID. */
#define UUID_HOMEKIT_PAIRING_PAIR_VERIFY					0x00, 0x00, 0x00, 0x4E, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x26, 0xBB, 0x76, 0x52, 0x91		/**< Pair Verify 128-bit UUID. */
#define UUID_HOMEKIT_PAIRING_FEATURES						0x00, 0x00, 0x00, 0x4F, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x26, 0xBB, 0x76, 0x52, 0x91		/**< Features 128-bit UUID. */
#define UUID_HOMEKIT_PAIRING_PAIRINGS						0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x26, 0xBB, 0x76, 0x52, 0x91		/**< Pairings 128-bit UUID. */
#define UUID_HOMEKIT_PAIRING_SERVICE_INSTANCE_ID			0x00, 0x00, 0x00, 0x51, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x26, 0xBB, 0x76, 0x52, 0x91		/**< Service Instance 128-bit UUID. */
#define UUID_HOMEKIT_PAIRING_CHAR_INSTANCE_ID				0xDC, 0x46, 0xF0, 0xFE, 0x81, 0xD2, 0x46, 0x16, 0xB5, 0xD9, 0x6A, 0xBD, 0xD7, 0x96, 0x93, 0x9A		/**< Characteristic Instance 128-bit UUID. */


enum HPS_AttributeIndex
{
	INDEX_HPS_PRIMARY_SERVICE=0x00,							/**< Index of homekit pairing primary service. */
	INDEX_PAIRING_SERVICE_INSTANCE_ID,						/**< Index of service instance ID characteristic. */
	INDEX_PAIRING_SERVICE_INSTANCE_ID_VALUE,				/**< Index of service instance ID characteristic value. */
	INDEX_PAIRING_CHAR_INSTANCE_ID1,						/**< Index of characteristic instance ID for SERVICE INSTANCE. */
	INDEX_PAIRING_PAIR_SETUP,								/**< Index of pair-setup characteristic. */
	INDEX_PAIRING_PAIR_SETUP_VALUE,							/**< Index of pair-setup characteristic value. */
	INDEX_PAIRING_CHAR_INSTANCE_ID2,						/**< Index of characteristic instance ID for PAIR_SETUP. */
	INDEX_PAIRING_PAIR_VERIFY,								/**< Index of pair-verify characteristic. */
	INDEX_PAIRING_PAIR_VERIFY_VALUE,						/**< Index of pair-verify characteristic value. */
	INDEX_PAIRING_CHAR_INSTANCE_ID3,						/**< Index of characteristic instance ID for VERIFY. */
	INDEX_PAIRING_PAIR_FEATURE,								/**< Index of pair-feature characteristic. */
	INDEX_PAIRING_PAIR_FEATURE_VALUE,						/**< Index of pair-feature characteristic value. */
	INDEX_PAIRING_CHAR_INSTANCE_ID4,						/**< Index of characteristic instance ID for FEATURE. */
	INDEX_PAIRING_PAIRINGS,									/**< Index of pairings characteristic. */
	INDEX_PAIRING_PAIRINGS_VALUE,							/**< Index of pairings characteristic value. */
	INDEX_PAIRING_CHAR_INSTANCE_ID5,						/**< Index of characteristic instance ID for PAIRING. */
	TOTAL_HPS_ATTR_NUM										/**< Total attribure number of homekit pairing service. */
};

typedef struct Homekit_Pairing_Handle_List
{
	u16 pairSetup;
	u16 pairVerify;
	u16 pairPairings;	
}Homekit_Pairing_Handle_List;


/**@brief Initizlize Homekit Pairing service table.
 *
 * @return MBIOT_RES_SUCCESS	Successfully initialize Homekit Pairing service table.
 * @return MBIOT_RES_FAIL		Operation is not permitted.
 */
PUBLIC extern u16 HOMEKIT_Pairing_Init_Service() large;
#endif

/**
  @}
 */