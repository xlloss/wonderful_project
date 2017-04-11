/********************************************************************
  File Information:
    FileName:       spi_sflash.h
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
 * @addtogroup SPI_SERIAL_FLASH SPI Serial Flash
 * @{
 * @brief Definitions and prototypes for the SPI_SERIAL_FLASH interface
 */
#ifndef SPI_SFLASH_H
#define SPI_SFLASH_H

/**
 * @defgroup SPI_SFLASH_MAX SPI_SFLASH_MAX_SIZE_DEF
 * @brief Maximum length of tx and rx bus
 * @{ */
#define SPI_SFLASH_MAX_CONTENT_SIZE 7	/**< Maximum length of instruction content. */
#define SPI_SFLASH_MAX_READ_SIZE 7		/**< Maximum length of read buffer. */
/** @} */


/**
 * @brief  The struct of APP_SPI_SFlashRParas parameters.
 */
typedef struct APP_SPI_SFlashRParas
{
	u8 opcode;											/**< Instruction command. */
	u8 contentLen;										/**< Total length of Instruction addresses, dummy cycle and data. */
	u8 contentEntry[SPI_SFLASH_MAX_CONTENT_SIZE];		/**< Instruction addresses, dummy and data. */
	u8 readLen;											/**< Length of read instruction, maximun length @ref SPI_SFLASH_MAX_SIZE_DEF. */
	u8 readBuffer[SPI_SFLASH_MAX_READ_SIZE];			/**< Read data buffer. */
}APP_SPI_SFlashRParas;

/**
 * @brief  The struct of APP_SPI_SFlashWParas parameters.
 */
typedef struct APP_SPI_SFlashWParas
{
	u8 writeEnable;										/**< The Write Enable instruction.*/
	u8 opcode;											/**< Instructions command. */
	u8 contentLen;										/**< Total length of Instruction addresses, dummy cycle and data. */
	u8 contentEntry[SPI_SFLASH_MAX_CONTENT_SIZE];		/**< Length of read instruction, maximun length @ref SPI_SFLASH_MAX_SIZE_DEF . */
}APP_SPI_SFlashWParas;


/**
 *@brief Get data from SPI serial flash.
 *
 *@param[in] p_readCmd	Pointer to APP_SPI_SFlashRParas structure.
 *
 *@return MBIOT_RES_SUCCESS Successfully read data from Serial Flash memory.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 *
 *@note	Supporting eight-clocks command in Standard SPI mode for Serial Flash memory.\n
 *		SPI mode 0 and 16MHz clock frequency are used as default settings. \n
 *		To drive Read Instructions of Serial Flash.
 *		Example:\n
 *			Read 7 bytes data from seiral flash address 0x000006. \n
 *			Total length of instruction content is 0x03.\n
 *			Read Instruction is 0x03. \n
 *
 *			XDATA APP_SPI_SFlashRParas  readCmd; \n
 *			readCmd.contentEntry[0]=0x00; \n
 *			readCmd.contentEntry[1]=0x00; \n
 *			readCmd.contentEntry[2]=0x06; \n
 *			readCmd.contentLen = 0x03; \n
 *			readCmd.opcode = 0x03; \n 
 *			readCmd.readLen = 7; \n
 *			APP_SPI_SerialFlash_Read(&readCmd);
 */
PUBLIC extern u16 APP_SPI_SerialFlash_Read(APP_SPI_SFlashRParas XDATA * p_readCmd) large;  

/**
 *@brief Write data into SPI serial flash.
 *
 *@param[in] p_writeCmd		Pointer to APP_SPI_SFlashWParas structure.
 *
 *@return MBIOT_RES_SUCCESS Successfully read data from Serial Flash memory.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 *
 *@note	Supporting eight-clocks command in Standard SPI mode for Serial Flash memory.\n
 *		SPI mode 0 and 16MHz clock frequency are used as default settings. \n 
 *		To drive Register/Setting Instructions of Serial Flash.\n
 *		Example:\n
 *			Write 4 bytes length of data (0x08,0x09,0x0a,0x0b) in seiral flash address 0x000006. \n
 *			And the Write Enable instruction is 0x06.\n
 *			Page Program Instructions is 0x02. \n
 *			Total length of instruction content is 0x07. \n
 *
 *			XDATA APP_SPI_SFlashWParas writeCmd;		
 *			writeCmd.writeEnable = 0x06;
 *			writeCmd.contentLen = 0x07;
 *			writeCmd.opcode = 0x02;
 *			writeCmd.contentEntry[0]=0x00; \n
 *			writeCmd.contentEntry[1]=0x00; \n
 *			writeCmd.contentEntry[2]=0x06; \n
 *			writeCmd.contentEntry[3]=0x08; \n
 *			writeCmd.contentEntry[4]=0x09; \n
 *			writeCmd.contentEntry[5]=0x0a; \n
 *			writeCmd.contentEntry[6]=0x0b; \n
 *			APP_SPI_SerialFlash_Write(&writeCmd);
 */
PUBLIC extern u16 APP_SPI_SerialFlash_Write(APP_SPI_SFlashWParas XDATA * p_writeCmd) large;

#endif
/**
  @}
 */