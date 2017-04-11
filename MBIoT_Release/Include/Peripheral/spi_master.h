/********************************************************************
  File Information:
    FileName:       spi_master.h
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
 * @addtogroup SPI_MASTER SPI Master
 * @{
 * @brief Definitions and prototypes for the SPI Master application programming interface.
 */
#ifndef SPI_MASTER_H
#define SPI_MASTER_H


 /**
 * @defgroup SPI_MASTER_ID SPI_MASTER_ID
 * @brief The mode of SPI Master
 * @{ */

#define SPI_MASTER_MANUAL_NCS		0	/**<  Application Manual Control NCS. */
#define SPI_MASTER_HW_NCS0			1	/**<  Hardware SPI Master 0. */
#define SPI_MASTER_HW_NCS1			2	/**<  Hardware SPI Master 1. */

/** @} */


 /**
 * @defgroup SPI_MASTER_MAX_BUF_SIZE SPI_MASTER_MAX_BUF_SIZE
 * @{ */
#define SPI_MASTER_MAX_BUFFER_SIZE 255				/**< The maximum length of data buffer */
/** @} */


/**
 * @brief  Definitions for SPI Master mode.
 */
enum SPI_MASTER_MODE_DEF
{
	SPI_MASTER_MODE0 = 0,		/**< SPI Mode 0: CHOL is 0, CPHA is 0. */
	SPI_MASTER_MODE1,			/**< SPI Mode 1: CHOL is 0, CPHA is 1. */
	SPI_MASTER_MODE2,			/**< SPI Mode 2: CHOL is 1, CPHA is 0. */
	SPI_MASTER_MODE3     		/**< SPI Mode 3: CHOL is 1, CPHA is 1. */
};


/**
 * @brief  Definition of master clock rate.
 */
enum SPI_MASTER_CLK_RATE_DEF
{
	SPI_MASTER_CLK_RATE_4MHZ = 3,			/**< 4MHz. */
	SPI_MASTER_CLK_RATE_2MHZ = 7,			/**< 2MHz. */
	SPI_MASTER_CLK_RATE_1MHZ = 15,			/**< 1MHz. */
	SPI_MASTER_CLK_RATE_500KHZ = 31,		/**< 500KHz. */
	SPI_MASTER_CLK_RATE_250KHZ =63			/**< 250KHz. */
};


/**
 * @brief  Struct of SPI master configuration parameters.
 */
typedef struct APP_SPI_Master_ConfParas
{
	u8 clockRate;			/**< Clock rate, see @ref SPI_MASTER_CLK_RATE_DEF. */
	u8 mode;				/**< SPI mode, see @ref SPI_MASTER_MODE_DEF. */
	u8 txBufferLen;			/**< The size of tx buffer, see @ref SPI_MASTER_MAX_BUF_SIZE for the limition. */
	u8 XDATA *p_txBuffer;	/**< Pointer to the tx buffer */
	u8 rxBufferLen;			/**< The size of rx buffer, see @ref SPI_MASTER_MAX_BUF_SIZE for the limition. */
	u8 XDATA *p_rxBuffer;	/**< Pointer to the rx buffer */
}APP_SPI_Master_ConfParas;

/**
 * @brief  Struct of SPI master Sending or Receiving parameters.
 */
typedef struct APP_SPI_Master_SendReceParas
{
	u8 spiMasterId;			/**< The mode of SPI Master, see @ref SPI_MASTER_ID. */
	u8 txLen;			/**< The size of tx buffer, see @ref SPI_MASTER_MAX_BUF_SIZE for the limition. */
	u8 XDATA *p_txBuffer;	/**< Pointer to the tx buffer */
	u8 rxBufferLen;			/**< The size of rx buffer, see @ref SPI_MASTER_MAX_BUF_SIZE for the limition. */
	u8 XDATA *p_rxBuffer;	/**< Pointer to the rx buffer */
}APP_SPI_Master_SendReceParas;

/**
 *@brief Initialize SPI master bus.
 *
 *@param[in] p_spiMasterConf	Pointer to the @ref APP_SPI_Master_ConfParas structure.
 *							
 *@return MBIOT_RES_SUCCESS Successfully initialize SPI master bus.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 *
 *@note Default NCS is pulled low.
 */
PUBLIC extern u16 APP_SPI_MASTER_Init(APP_SPI_Master_ConfParas XDATA * p_spiMasterConf)  large;


/**
 *@brief Start SPI communication
 *
 *@param[in] p_spiMasterSendRece	 	 		Pointer to the @ref APP_SPI_Master_SendReceParas structure.
 *
 *@return MBIOT_RES_SUCCESS SPI bus communicate successfully.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 *
 *@note  Hardware SPI master mode can connect Two SPI slaves by spiMasterId setting.\n 
 * If SPI_MASTER_HW_NCS0 is set, below GPIO pins are occupied for SPI bus.\n
 *		- P31: NCS\n
 *		- P32: MISO\n
 *		- P33: MOSI\n
 *		- P34: SCLK\n
 *		If SPI_MASTER_HW_NCS1 is set, below GPIO pins are occupied for SPI bus.\n
 *		- P27: NCS\n
 *		- P32: MISO\n
 *		- P33: MOSI\n
 *		- P34: SCLK\n
 *		If SPI_MASTER_MANUAL_NCS is set, below GPIO pins are occupied for SPI bus.\n
 *		- User define PXX for NCS\n
 *		- P32: MISO\n
 *		- P33: MOSI\n
 *		- P34: SCLK\n
 */
PUBLIC extern u16 APP_SPI_MASTER_SendReceive(APP_SPI_Master_SendReceParas XDATA * p_spiMasterSendRece) large;


/**
 *@brief Free SPI bus, the occupied GPIO pins return to digital input and internal pull high.
 *
 */
PUBLIC extern void APP_SPI_MASTER_Close() large;

#endif


/**
  @}
 */
