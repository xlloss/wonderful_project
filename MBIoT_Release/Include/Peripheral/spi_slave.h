/********************************************************************
  File Information:
    FileName:       spi_slave.h
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
 * @addtogroup SPI_SLAVE SPI Slave
 * @{
 * @brief Definitions and prototypes for the SPI Slave application programming interface.
 */
#ifndef SPI_SLAVE_H
#define SPI_SLAVE_H

 /**
 * @defgroup SPI_SLAVE_MAX_BUF_SIZE SPI_SLAVE_MAX_BUF_SIZE
 * @{ */
#define SPI_SLAVE_MAX_BUFFER_SIZE 255					/**< The maximum length of data buffer. */
/** @} */


/**
 * @brief  Definitions for SPI Slave mode.
 */
enum SPI_SLAVE_MODE_DEF
{
	SPI_SLAVE_MODE0 = 0,		/**< SPI Mode 0: CHOL is 0, CPHA is 0. */
	SPI_SLAVE_MODE1,			/**< SPI Mode 1: CHOL is 0, CPHA is 1. */
	SPI_SLAVE_MODE2 ,			/**< SPI Mode 2: CHOL is 1, CPHA is 0. */
	SPI_SLAVE_MODE3     		/**< SPI Mode 3: CHOL is 1, CPHA is 1. */
};


/**
 * @brief  Definition of slave clock rate.
 */
enum SPI_SLAVE_OVERSAMPLING_CLK_RATE_DEF
{
	SPI_SLAVE_CLK_RATE_16MHZ = 0,		/**< 16MHz. SCLK Rate:4MHz~1.9MHz.  */
	SPI_SLAVE_CLK_RATE_8MHZ = 1,		/**< 8MHz. SCLK Rate:2MHz~900KHz. */
	SPI_SLAVE_CLK_RATE_4MHZ = 3,		/**< 4MHz. SCLK Rate:1MHz~400KHz. */
	SPI_SLAVE_CLK_RATE_2MHZ = 7,		/**< 2MHz. SCLK Rate:500KHz~200KHz. */
	SPI_SLAVE_CLK_RATE_1MHZ = 15,		/**< 1MHz. SCLK Rate:250KHz~100KHz. */
	SPI_SLAVE_CLK_RATE_500KHZ = 31,		/**< 500KHz. SCLK Rate:125KHz~50KHz. */
	SPI_SLAVE_CLK_RATE_250KHZ =63		/**< 250KHz. SCLK Rate:65KHz~31KHz. */	
};

/**
 * @defgroup SPI_SLAVE_RESPONSE_MODE SPI_SLAVE_RESPONSE_MODE
 * @brief Definition of spi response mode option.
 * @{ */
#define SPI_SLAVE_RESPONSE_QUICK				0				/**< The interval between SPI NCS pull to low and first signal of SCLK is smaller than 3ms. (Default)*/
#define SPI_SLAVE_RESPONSE_NORMAL				1				/**< The interval between SPI NCS pull to low and first signal of SCLK is larger than 3ms. */
/** @} */

/**
 * @brief  Struct of SPI slave configuration parameters.
 */
typedef struct APP_SPI_Slave_Config
{
	u8 frequency;			/**< Clock rate, see @ref SPI_SLAVE_OVERSAMPLING_CLK_RATE_DEF. */
	u8 mode;				/**< SPI mode, see @ref SPI_SLAVE_MODE_DEF. */
	u8 respMode;			/**< Response mode (the miminum interval between SPI NCS pull low and first signal of SCLK should not less than 2ms) see @ref SPI_SLAVE_RESPONSE_MODE. */
	u8 txBufferLen;			/**< The size of tx buffer, see @ref SPI_SLAVE_MAX_BUF_SIZE for the limition. */
	u8 XDATA *p_txBuffer;	/**< Pointer to the tx buffer */
	u8 rxBufferLen;			/**< The size of rx buffer, see @ref SPI_SLAVE_MAX_BUF_SIZE for the limition. */
	u8 XDATA *p_rxBuffer;	/**< Pointer to the rx buffer */
}APP_SPI_Slave_Config;

/**
 * @brief  Struct of SPI slave Data Sending parameters.
 */
typedef struct APP_SPI_Slave_Send
{
	u8 txBufferLen;			/**< The size of tx buffer, see @ref SPI_SLAVE_MAX_BUF_SIZE for the limition. */
	u8 XDATA *p_txBuffer;	/**< Pointer to the tx buffer */
}APP_SPI_Slave_Send;


/**
 *@brief Initialize SPI slave bus.
 *
 *@param[in] p_spiSlaveConf	 Pointer to the @ref APP_SPI_Slave_Config structure.
 *
 *@return MBIOT_RES_SUCCESS Successfully initialize SPI slave bus.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 *@note If SPI slave bus is initialized, below GPIO pins are occupied for SPI bus.\n
 *		- P31: NCS\n
 *		- P32: MISO\n
 *		- P33: MOSI\n
 *		- P34: SCLK\n
 *		If an interval of NCS pulled down until first sclok is less than 3 min second,\n user must use SPI_SLAVE_RESPONSE_QUICK.\n
 */
PUBLIC extern u16 APP_SPI_SLAVE_Init(APP_SPI_Slave_Config XDATA * p_spiSlaveConf) large;


/**
 *@brief Prepare data for SPI communication.
 *
  *@param[in] p_spiSlaveSend	 Pointer to the @ref APP_SPI_Slave_Send structure.
 *
 *@return MBIOT_RES_SUCCESS Successfully prepare data for SPI communication.
 *@return MBIOT_RES_NO_RESOURCE SPI bus is busy.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 *
 */
PUBLIC extern u16 APP_SPI_SLAVE_Send(APP_SPI_Slave_Send XDATA * p_spiSlaveSend) large;

/**
 *@brief Free specified SPI bus, the occupied GPIO pins return to digital input and internal pull high.
 */
PUBLIC extern void APP_SPI_SLAVE_Close(void) large;

#endif

/**
  @}
 */
