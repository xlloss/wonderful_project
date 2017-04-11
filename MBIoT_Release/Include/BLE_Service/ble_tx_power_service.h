/********************************************************************
  File Information:
    FileName:       ble_tx_power_level_service.h
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
  @addtogroup BLE_TPS Ble Tx Power Service
  @ingroup BLE_Service BLE Service
  @{
  @brief Ble Tx Power Service
  @note Definitions and prototypes for the Ble Tx Power Service application programming interface.
 */
 

#ifndef BLE_TX_POWER_SERVICE_H
#define BLE_TX_POWER_SERVICE_H

/**@brief Tx Power Service UUID definitions. */
#define UUID_TX_POWER				  0x1804		/**< Definition of Tx Power Service UUID. */
#define UUID_TX_POWER_LEVEL	  0x2A07		/**< Definition of Tx Power Level characteristic UUID. */

#define INDEX_TPS_TX_POWER_LEVEL		2		/**< Index of Tx Power Level Characteristic. */ 

#define TOTAL_TPS_ATTR_NUM			3			/**< Total attribure number of tx power service. */	

/**@brief Maximum transmit power level(-100 - 20). */
#define MAX_TX_POWER_LEVEL		0x00

/**@brief Initialize BLE Transmit Power Level service table.
 *
 * @return @ref MBIOT_RES_SUCCESS		Initialize BLE Tansmit Power Level service table successfully.
 */
PUBLIC extern u16 BLE_TPS_Init_Service() large;

/**@brief Catch BLE_GATT events to get the TPS events.
 *
 * @param[in] p_event		Pointer to BLE_GATT_Event structure.
 *
 * @return @ref MBIOT_RES_SUCCESS		Catch BLE GATT events configuration successfully.
 */
PUBLIC extern void BLE_TPS_Catch_Event(BLE_GATT_Event XDATA * p_event) large;

/**@brief Set transmit Power level.
 *
 * @param[in] txPowerLevel Maximum transmit power level.
 *
 * @return @ref MBIOT_RES_SUCCESS set tx power level successfully.
 */
PUBLIC extern u16 BLE_TPS_Set_Tx_Power_Level(s8 txPowerLevel) large;

#endif

/**
  @}
 */