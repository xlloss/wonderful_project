/********************************************************************
  File Information:
    FileName:       utility.h
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
  @addtogroup UTILITY Utility
  @{
  @brief Definitions and prototypes for the Utility application programming interface.
 */
 
#ifndef UTILITY_H
#define UTILITY_H

#include "ble_gap.h"
#include "gpio.h"

/**
 * @defgroup UTILITY_LOW_POWER_MODE UTILITY_LOW_POWER_MODE
 * @brief Definition of low power mode option.
 * @{ */
#define LOW_POWER_MODE_ALLOW				0				/**< Low power mode is enabled. */
#define LOW_POWER_MODE_DISALLOW				1				/**< Low power mode is disabled. */
/** @} */

/**@brief Definition length of random number. */
#define APP_RAN_NUM_SIZE_MAX	0x10

/**@brief Structure of paired device information. */
typedef struct	APP_UTILITY_PairedInfo
{
	u8 index;									/**< Paired device index. */
	u8 priority;								/**< Paired device priority. (First priority is 1.) */
	BLE_GAP_Addr	addr;						/**< Paired device bluetooth address. */
	
}APP_UTILITY_PairedInfo;


/**@brief Structure of MBIoT information. */
typedef struct APP_UTILITY_MbiotInfo
{
	u8	icInformation[14];						/**< IC information. */
	u8	flashMainVersion;						/**< Flash main version. */
	u8	flashSubVersion;						/**< Flash sub version. */
	u8	deviceID[4];							/**< Device ID. */
}APP_UTILITY_MbiotInfo;


/**
 *@brief MBIoT low power mode configuration.
 *
 * @param[in] option		Low power mode parameter, see @ref UTILITY_LOW_POWER_MODE. (Default low power mode is enabled.)
 */
PUBLIC extern void UTILITY_LowPowerModeControl(u8 option) large;

/**
 *@brief Shutdown MBIoT.
 *
 *@param[in] p_startMask	Pointer to @ref APP_GPIO_polling structure.
 *
 */
PUBLIC extern void UTILITY_Shutdown(APP_GPIO_Port_Mask XDATA *p_startMask) large;


/**
 *@brief Read all paired device information.
 *
 *@param[out] p_deviceNum		Pointer to the number of paired device. 
 *@param[out] p_pairedList		Pointer to paired device information, see @ref APP_UTILITY_PairedInfo.
 */
PUBLIC extern void UTILITY_ReadAllPairedDevice(u8 XDATA *p_deviceNum, u8 XDATA *p_pairedList) large;


/**
 *@brief Erase all paired device information.
 */
PUBLIC extern void UTILITY_EraseAllPairedDevice() large;


/**
 *@brief Delete sepcified paired device information.
 *
 *@param[in] index			The index of paired information.
 *
 *@return MBIOT_RES_SUCCESS	Successfully delete sepcified paired device information.
 *@return MBIOT_RES_INVALID_PARA 	Invalid parameters.
 */
PUBLIC extern u16 UTILITY_DeleteSpecificPairedDevice(u8 index) large;


/**
 *@brief Reset MBIoT. 
 */
PUBLIC extern void UTILITY_Reset() large;

/**
 *@brief Read data from eflash.
 *
 *@param[in] address		Address
 *@param[in] length			The data length.
 *@param[out] p_payload		Pointer to the data buffer.
 *
 *@note Range:0x4000~0xFFFF 
 *
 *@return MBIOT_RES_SUCCESS	Successfully Read data from Eflash.
 *@return MBIOT_RES_INVALID_PARA 	Invalid parameters.
 */
PUBLIC extern u16 UTILITY_ReadEflash(u16 address, u16 length, u8 XDATA* p_payload) large;

/**
 *@brief Write data in eflash.
 *
 *@param[in] address		Address.
 *@param[in] length			The data length.
 *@param[in] p_payload		Pointer to the data buffer.
 *
 *@note Range:0x4000~0xFFFF 
 *
 *@return MBIOT_RES_SUCCESS	Successfully write data in Eflash.
 *@return MBIOT_RES_INVALID_PARA 	Invalid parameters.
 */
PUBLIC extern u16 UTILITY_WriteEflash(u16 address, u16 length, u8 XDATA* p_payload) large;

/**
 *@brief Erase eFlash.
 *
 *@param[in] address		Address for eFlash erase.
 *@param[in] length		The data length in byte for eFlash erase.
 *
 * @note Range:0x4000~0xFFFF\n
 *   Minumum Erase Unit is 2KB
 *
 *@return MBIOT_RES_SUCCESS	Successfully erase address for eFlash .
 */
PUBLIC extern u16 UTILITY_EraseEflash(u16 address, u16 length) large;

/**
 *@brief Register OTA authenticated key.
 *
 *@param[in] p_adminKey		Pointer to the 16 bytes authenticated key buffer.
 *							Key value with 16 bytes 0xFF will be taken as disabling OTA feature.
 */
PUBLIC extern void UTILITY_RegisterOTAKey(u8 XDATA* p_adminKey) large;



/**
 *@brief Get MBIot information.
 *
 *@param[in] p_info		Pointer to APP_UTILITY_MbiotInfo structure.
 */
PUBLIC extern void UTILITY_GetMbiotInformation(APP_UTILITY_MbiotInfo XDATA* p_info) large;

/**
 *@brief Write data in eflash.
 *
 *@param[in] length			The random number length.
 *@param[in] p_payload		Pointer to the random number data buffer.
 *@
  *@return MBIOT_RES_SUCCESS	Successfully generate random number.
 *@return MBIOT_RES_INVALID_PARA 	Invalid parameters.
 */
PUBLIC extern u16 UTILITY_GenerateRandomNumber(u8 length, u8 XDATA* random) large;
#endif 
 /**
  @}
*/