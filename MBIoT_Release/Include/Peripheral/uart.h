/********************************************************************
  File Information:
    FileName:       uart.h
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
  @addtogroup UART UART
  @{
  @brief Definitions and prototypes for the UART application programming interface.
 */
 
#ifndef UART_H
#define UART_H

/**
 * @defgroup UART_FLOW_CONTROL_DEF UART_FLOW_CONTROL_DEF
 * @brief The definition of UART flow control.
 * @{ */
#define APP_UART_FLOW_CONTROL_DISABLE			0x00			/**< UART flow control is disabled. */
#define APP_UART_FLOW_CONTROL_ENABLE			0x01			/**< UART flow control is enabled. CTS is P00 RTS is P36 */
/** @} */


/**
 * @defgroup UART_MINIMUM_BUFFER_LEN UART_MINIMUM_BUFFER_LEN
 * @{ */
#define APP_UART_BUFFER_MIN_LENGTH			0x32				/**< The minimum length of UART buffer. */
/** @} */


/**
 * @defgroup UART_BAUDRATE_DEF UART_BAUDRATE_DEF
 * @brief The definition of UART baud rate.
 * @{ */
#define APP_UART_BAUD_RATE_921600			0x00				/**< UART baud rate: 921600. */
#define APP_UART_BAUD_RATE_460800			0x01				/**< UART baud rate: 460800. */
#define APP_UART_BAUD_RATE_230400			0x02 				/**< UART baud rate: 230400. */
#define APP_UART_BAUD_RATE_115200			0x03 				/**< UART baud rate: 115200. */
#define APP_UART_BAUD_RATE_57600			0x04 				/**< UART baud rate: 57600. */
#define APP_UART_BAUD_RATE_38400			0x05 				/**< UART baud rate: 38400. */
#define APP_UART_BAUD_RATE_28800			0x06 				/**< UART baud rate: 28800. */
#define APP_UART_BAUD_RATE_19200			0x07 				/**< UART baud rate: 19200. */
#define APP_UART_BAUD_RATE_14400			0x08 				/**< UART baud rate: 14400. */
#define APP_UART_BAUD_RATE_9600				0x09 				/**< UART baud rate: 9600. */
#define APP_UART_BAUD_RATE_4800				0x0A				/**< UART baud rate: 4800. */
#define APP_UART_BAUD_RATE_2400				0x0B				/**< UART baud rate: 2400. */
/** @} */
	

/**@brief The structure of UART configuration parameters. */
typedef struct APP_UART_ConfParas{
    u8		 baudrate;								/**< UART baud rate, see @ref UART_BAUDRATE_DEF. */
    u8		 flowContol;							/**< UART flow control option, see @ref UART_FLOW_CONTROL_DEF.\n */
    												/**< If the parameter is set to APP_UART_FLOW_CONTROL_ENABLE, then the GPIO P00 is occupied for UART_CTS and the GPIO P36 is occupied for UART_RTS. */
    u16		 rxNotifyLength;						/**< The length of queued data in UART RX buffer needs to be notified by UART callback event APP_UART_EVT_RECEIVE_DATA. */
    u16		 rxBufferSize;							/**< UART RX buffer size, see the minimum limition @ref APP_UART_BUFFER_MIN_LENGTH. */
    u8 XDATA *rxBuffer;								/**< Pointer to the UART RX buffer. */
    u16		 txBufferSize;							/**< UART TX buffer size, see the minimum limition @ref APP_UART_BUFFER_MIN_LENGTH. */
    u8 XDATA *txBuffer;								/**< Pointer to the UART TX buffer. */
	u8		 rxWakeupPinNum;						/**< The pin number used to wake up MBIoT from low power mode to receive UART data, see @ref GPIO_PIN_NUM_DEF */
	u8		 txIndicateInterval;					/**< The timing setting of UART_Tx_Ind signal deasserted if txIndicateInterval is not zero. UART_Tx_Ind is low before UART_Tx packet is transmitted.\n */
													/**< The GPIO P02 is occupied as UART_Tx_Ind if the value is not set to 0. (Unit: 625us) */
}APP_UART_ConfParas;


/**
 *@brief Initialize UART module
 *
 *@param[in] p_uartParas		Pointer to @ref APP_UART_ConfParas structure.
 *
 *@return MBIOT_RES_SUCCESS Successfully initialize UART module.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 */
PUBLIC extern u16 APP_UART_Init(APP_UART_ConfParas XDATA *p_uartParas) large;


/**
 *@brief Get length of queued data in UART RX buffer.
 *
 *@param[out] p_length		Pointer to the length of queued data.
 */
PUBLIC extern void APP_UART_GetReceivedDataLength(u8 XDATA *p_length) large;


/**
 *@brief Get specified length of queued received UART data from UART RX buffer.
 *
 *@param[in] length			The length of acquired data.
 *@param[out] p_buffer		Pointer to the data buffer.
 *
 *@return MBIOT_RES_SUCCESS Successfully get specified length of queued data from UART RX buffer
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 */
PUBLIC extern u16 APP_UART_GetReceivedData(u16 length, u8 XDATA *p_buffer) large;


/**
 *@brief Transmit UART data by specified length.
 *
 *@param[in] length			The length of data needed to be transmitted.
 *@param[in] p_buffer		Pointer to the buffer of transmitted data.
 *
 *@return MBIOT_RES_SUCCESS Successfully transmit UART data by specified length.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 *@return MBIOT_RES_NO_RESOURCE  UART TX buffer is full.
 */
PUBLIC extern u16 APP_UART_Put(u16 length, u8 XDATA *p_buffer) large;


/**
 *@brief Close uart module. All occupied GPIO pins return to digital input and internal pull high.
 *
 *@return MBIOT_RES_SUCCESS Successfully close UART module.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 */
PUBLIC extern u16 APP_UART_Close()large;

#endif 
 /**
  @}
*/