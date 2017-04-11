/********************************************************************
  File Information:
    FileName:       peripheral.h
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
  @addtogroup APP_PERIPHERAL Peripheral
  @{
  @brief APP Peripheral
  @note Definitions and prototypes for the Peripheral application programming interface.
 */
 
#ifndef PERIPHERAL_H
#define PERIPHERAL_H

#include "uart.h"
#include "gpio.h"
#include "led.h"
#include "i2c.h"
#include "adc.h"
#include "pwm.h"
#include "spi_master.h"
#include "spi_slave.h"
#include "spi_sflash.h"

/**@brief APP PERIPHERAL call back events. */
enum Peripheral_Event_ID
{
	APP_UART_EVT_RECEIVE_DATA=0x00,			/**< The length of UART queued data in RX buffer is larger than the APP_UART_ConfParas rxNotifyLength parameter. See @ref UART_Event for the event content. */
	APP_UART_EVT_SEND_DATA_FINISH,			/**< The specified length of data is transmitted. See @ref UART_Event for the event content. */
	APP_SPI_SLAVE_EVT_TRANSFER_DONE, 		/**< SPI communication is finished. See @ref SPI_SLAVE_Event for the event content. */
	APP_SPI_SLAVE_EVT_RECEIVE_DATA, 			/**< The length of receiving data from SPI SLAVE RX buffer. See @ref SPI_SLAVE_Event for the event content. */
	APP_GPIO_STATE_CHANGED					/**< The state of polled GPIOs are changed. See @ref GPIO_State_Event for the event content. */
};


/**@brief The structure of UART event. */
typedef struct UART_Event
{
	u16 dataLen;				/**< The length of UART receiving/transmission. */
}UART_Event;


/**@brief The structure of SPI Slave event. */ 
typedef struct SPI_SLAVE_Event
{
	u8 dataLen;					/**< The length of SPI communications. */
	u8 XDATA *p_rxBuffer;		/**< Pointer to the rx buffer */
}SPI_SLAVE_Event;


/**@brief The structure of APP GPIO state change event. */
typedef struct GPIO_State_Event
{
	u8 port0State;				/**< GPIO port 0 state.*/
	u8 port1State;				/**< GPIO port 1 state.*/
	u8 port2State;				/**< GPIO port 2 state.*/
	u8 port3State;				/**< GPIO port 3 state.*/
}GPIO_State_Event;

/**@brief Structure of APP PERIPHERAL events. */
typedef union 
{
	UART_Event			uartEvent; 			/**< Data field of UART event. */
	SPI_SLAVE_Event 	spiSlaveEvent;		/**< Data field of SPI Slave event. */
	GPIO_State_Event	gpioStateEvent;		/**< Data field of GPIO state event. */
}PeripheralEventField;


/**@brief APP PERIPHERAL callback function parameters. */
typedef struct	APP_Peripheral_Event
{
	u16	eventID;											/**< Event ID. See @ref Peripheral_Event_ID. */
	PeripheralEventField		eventField;					/**< Data field of this event. */
}APP_Peripheral_Event;


/**@brief APP PERIPHERAL callback function prototype. */
typedef void(*PeripheralProcessRoutine)(APP_Peripheral_Event XDATA *p_event) large;


/**
 *@brief Initialize APP PERIPHERAL callback function.
 *
 *@param[in] peripheralHandler		MBIoT callback function for APP PERIPHERAL events.
 */
PUBLIC extern void APP_PERIPHERAL_Init(PeripheralProcessRoutine peripheralHandler) large;

#endif 

/**
  @}
*/