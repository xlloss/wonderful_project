/********************************************************************
  File Information:
    FileName:       i2c.h
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
 * @addtogroup I2C I2C
 * @{
 * @brief Definitions and prototypes for the I2C master application programming interface.
 */
#ifndef I2C_H
#define I2C_H
#include "mbiot_error.h"


/**
 * @defgroup I2C_MAX_SIZE_DEF I2C_MAX_SIZE_DEF
 * @brief Maximum length of tx and rx bus
 * @{ */
#define I2C_MAX_TX_SIZE 32	/**< Maximum data length of tx bus. */
#define I2C_MAX_RX_SIZE 32	/**< Maximum data length of rx bus. */
/** @} */


/**
 * @brief  Definition of I2C bus speed.
 */
enum I2C_BUS_SPEED
{
	DIV_TO_400_KHz = 0, /**< 400KHz,fast mode. */
	DIV_TO_200_KHz,	 	/**< 200KHz. */
	DIV_TO_100_KHz,	 	/**< 100KHz. */
	DIV_TO_50_KHz,		/**< 50KHz. */
	DIV_TO_25_KHz,		/**< 25KHz. */
	DIV_TO_12_5_KHz	 	/**< 12.5KHz. */
};


/**
 * @brief  Definition of I2C error code.
 */
enum I2C_NACK_STATUS_DEF
{
	I2C_ERR_ADDR_NACK=MTIOT_RES_PERIPHERAL_I2C_BASE,	/**< Receives a NACK on address. */
	I2C_ERR_DATA_NACK,									/**< Receives a NACK on data. */
	I2C_ERR_NO_CONFIG,									/**< I2C bus is not initialized. */
	I2C_ERR_LENGTH_EXCEEDED								/**< Data length is exceeded. */
};


/**
 * @brief  The struct of APP_I2C_CombineWR parameters.
 */
typedef struct APP_I2C_CombineWRParas
{
	u8 lenWrite;						/**< The data length of tx, maximun length sees @ref I2C_MAX_SIZE_DEF. */
	u8 writeBuffer[I2C_MAX_TX_SIZE];  	/**< The data buffer of tx. */
	u8 lenRead;							/**< The data length of rx, maximun length @ref I2C_MAX_SIZE_DEF . */
	u8 readBuffer[I2C_MAX_RX_SIZE];		/**< The data buffer of rx. */
}APP_I2C_CombineWRParas;


/**
 *@brief I2C configuration
 *
 *@param[in] address	7-bit address of the Slave.
 *@param[in] speed		I2C bus speed, see @ref I2C_BUS_SPEED.
 *
 *@return MBIOT_RES_SUCCESS Configuring I2C successfully.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 *
 * @note 	Two GPIO pins are occupied if I2C bus are enabled.\n
 *			I2C SCL is pin P12, I2C SDA is pin P13.\n
 *			Example:\n
 *			Access E2prom by I2C bus.\n
 *			EEPROM(I2C slave) address is 0xA0. I2C bus speed is 200KHz.\n
 *			APP_I2C_Config(0xA0, DIV_TO_200_KHz);
 */
PUBLIC extern u16 APP_I2C_Config(u8 address, u8 speed) large;


/**
 *@brief  Free I2C bus, the occupied GPIO pins return to input and internal pull high.
 */
PUBLIC extern void APP_I2C_Close(void) large;


/**
 *@brief Send data to I2C slave side.
 *
 *@param[in] len			The data length of tx, maximun length sees @ref I2C_MAX_SIZE_DEF.
 *@param[in] p_write		Pointer to the tx data buffer.
 *
 *@return MBIOT_RES_SUCCESS Successfully send data to I2C slave side.
 *@return @ref I2C_ERR_NO_CONFIG I2C bus is not initialized.
 *@return @ref I2C_ERR_ADDR_NACK Receives a NACK on address.
 *@return @ref I2C_ERR_LENGTH_EXCEEDED Data length is exceeded.
 *
 * @note 	Example:\n
 *			Send 3 bytes length of data to I2C slave. \n
 *			XDATA u8 test[3];\n
 *			test[0]=0xC0;\n
 *			test[1]=0xFF;\n
 *			test[2]=0xEE;\n
 *			APP_I2C_Write(3, test);\n
 */
PUBLIC extern u16 APP_I2C_Write(u8 len, u8 XDATA * p_write) large;


/**
 *@brief Get data from I2C Slave side.
 *
 *@param[in] len			The data length of rx, maximun length sees @ref I2C_MAX_SIZE_DEF.
 *@param[out] p_read		Pointer to the rx data buffer.
 *
 *@return MBIOT_RES_SUCCESS Successfully send data to I2C slave side.
 *@return @ref I2C_ERR_NO_CONFIG I2C bus is not initialized.
 *@return @ref I2C_ERR_ADDR_NACK Receives a NACK on address.
 *@return @ref I2C_ERR_LENGTH_EXCEEDED Data length is exceeded.
 *
 * @note 	Example:\n
 *			Get 2 bytes length of data from I2C slave side. \n
 *			XDATA u8 test[2];\n
 *			APP_I2C_Read(2, test);\n
 */
PUBLIC extern u16 APP_I2C_Read(u8 len, u8 XDATA * p_read) large;


/**
 *@brief Send data to I2C slave side and then get data from I2C slave side.
 *
 *@param[in] p_combineWRParas		Pointer to @ref APP_I2C_CombineWRParas structure.
 *
 *@return MBIOT_RES_SUCCESS Successfully send data to I2C slave side.
 *@return @ref I2C_ERR_NO_CONFIG I2C bus is not initialized.
 *@return @ref I2C_ERR_ADDR_NACK Receives a NACK on address.
 *@return @ref I2C_ERR_LENGTH_EXCEEDED Data length is exceeded.
 *
 *@note	Example:\n
 *		Send 20 bytes length of data to I2C slave side, and then get 10 bytes length of data form I2C slave.\n
 *		XDATA APP_I2C_CombineWRParas p_combineWR;\n
 *		p_combineWR.lenWrite = 20;\n
 *		xmemset(&p_combineWR.writeBuffer, 0x11, p_combineWR.lenWrite);\n
 *		p_combineWR.lenRead = 10;\n
 *		xmemset(&p_combineWR.readBuffer, 0, p_combineWR.lenRead);\n
 *		APP_I2C_CombineWR(&p_combineWR);\n
 */
PUBLIC extern u16 APP_I2C_CombineWR(APP_I2C_CombineWRParas	XDATA * p_combineWRParas) large;

#endif

/**
  @}
 */