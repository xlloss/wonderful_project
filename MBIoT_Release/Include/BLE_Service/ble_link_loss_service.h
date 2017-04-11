/********************************************************************
  File Information:
    FileName:       ble_link_loss_service.h
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
  @addtogroup BLE_LLS Ble Link Loss Service
  @ingroup BLE_Service BLE Service
  @{
  @brief Ble Link Loss Service
  @note Definitions and prototypes for the Ble Link Loss Service application programming interface.
 */


#ifndef BLE_LINK_LOSS_SERVICE_H
#define BLE_LINK_LOSS_SERVICE_H

/**@brief Link Loss Service UUID definitions. */
#define UUID_LINK_LOSS				0x1803
#define UUID_ALERT_LEVEL		  0x2A06

#define INDEX_LLS_ALERT_LEVEL		2		/**< Index of Link Loss Alert Level Characteristic. */

/**@brief Alert Levels */
#define ALERT_LEVEL_NO_ALERT				0x00	/**< No Alert.   */
#define ALERT_LEVEL_MILD_ALERT			0x01	/**< Mild Alert. */
#define ALERT_LEVEL_HIGH_ALERT			0x02	/**< High Alert. */

#define TOTAL_LLS_ATTR_NUM		3	/**< Total attribure number of link loss service. */

/**@brief Prototype for lls events handler */
typedef void(*LLSProcessRoutine)(u8 alertLevel) large;

/**@brief Initialize BLE LLS.
 *
 * @param[in] llsHandler		MBIoT callback function for BLE LLS events.
 *
 * @return @ref MBIOT_RES_SUCCESS		Initialize BLE LLS successfully.
 */
PUBLIC u16 BLE_LLS_Init(LLSProcessRoutine llsHandler) large;

/**@brief Initialize BLE Link Loss service table.
 *
 * @return @ref MBIOT_RES_SUCCESS		Initialize BLE Link Loss service table successfully.
 */
PUBLIC extern u16 BLE_LLS_Init_Service() large;

/**@brief Catch BLE_GATT events to get the LLS events.
 *
 * @param[in] p_event		Pointer to BLE_GATT_Event structure.
 *
 * @return @ref MBIOT_RES_SUCCESS		Catch BLE GATT events configuration successfully.
 */
PUBLIC extern void BLE_LLS_Catch_Event(BLE_GATT_Event XDATA * p_event) large;

/**@brief Set alert level.
 *
 * @param[in] alertLevel Alert level.
 *
 * @return @ref MBIOT_RES_SUCCESS set alert level successfully.
 */
PUBLIC extern u16 BLE_LLS_Set_AlertLevel(u8 alertLevel) large;

#endif

/**
  @}
 */