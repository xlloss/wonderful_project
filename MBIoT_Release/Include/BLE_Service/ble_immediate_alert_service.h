/********************************************************************
  File Information:
    FileName:       ble_immediate_alert_service.h
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
  @addtogroup BLE_IAS Ble Immediate Alert Service
  @ingroup BLE_Service BLE Service
  @{
  @brief Ble Immediate Alert Service
  @note Definitions and prototypes for the Ble Immediate Alert Service application programming interface.
 */


#ifndef BLE_IMMEDIATE_ALERT_SERVICE_H
#define BLE_IMMEDIATE_ALERT_SERVICE_H


/**@brief Immediate Alert service UUID definitions. */
#define UUID_IMMEDIATE_ALERT  0x1802
#define UUID_ALERT_LEVEL		  0x2A06

#define INDEX_IAS_ALERT_LEVEL		2		/**< Index of Immediate Alert Level Characteristic. */

/**@brief Alert levels. */
#define ALERT_LEVEL_NO_ALERT			0x00	/**< No Alert.   */
#define ALERT_LEVEL_MILD_ALERT		0x01	/**< Mild Alert. */
#define ALERT_LEVEL_HIGH_ALERT		0x02	/**< High Alert. */

#define TOTAL_IAS_ATTR_NUM		3	/**< Total attribure number of Immediate Alert service. */

/**@brief Prototype for ias events handler */
typedef void(*IASProcessRoutine)(u8 alertLevel) large;

/**@brief Initialize BLE IAS.
 *
 * @param[in] iasHandler		MBIoT callback function for BLE IAS events.
 *
 * @return @ref MBIOT_RES_SUCCESS		Initialize BLE IAS successfully.
 */
PUBLIC u16 BLE_IAS_Init(IASProcessRoutine iasHandler) large;

/**@brief Initialize BLE Immediate Alert service table.
 *
 * @return @ref MBIOT_RES_SUCCESS		Initialize BLE Immediate ALert service table successfully.
 */
PUBLIC extern u16 BLE_IAS_Init_Service() large;

/**@brief Catch BLE_GATT events to get the IAS events.
 *
 * @param[in] p_event		Pointer to BLE_GATT_Event structure.
 *
 * @return @ref MBIOT_RES_SUCCESS		Catch BLE GATT events configuration successfully.
 */
PUBLIC extern void BLE_IAS_Catch_Event(BLE_GATT_Event XDATA * p_event) large;

#endif

/**
  @}
 */