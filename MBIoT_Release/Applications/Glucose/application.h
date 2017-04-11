/********************************************************************
  File Information:
    FileName:       application.h
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
#ifndef APPLICATION_H
#define APPLICATION_H

#define APP_DEBUG_TX_BUFFER_LEN			512
#define APP_DEBUG_RX_BUFFER_LEN			256

#define APP_HEAP_SIZE            		(1024)			/**< The size of app heap. And it must be divisible by 32. */


#define EVT_LEN_ADVERTISING					13
#define EVT_LEN_UNSPECIFIC_ERROR			18
#define EVT_LEN_UNKNOWN_COMMAND				16
#define EVT_LEN_CONNECTED					11
#define EVT_LEN_DISCONNECTED				14
#define EVT_LEN_CHANNEL_ENABLED				22
#define EVT_LEN_CHANNEL_DISABLED			23

enum EVT_INDEX
{
	EVT_ADVERTISING,
	EVT_UNSPECIFIC_ERROR,
	EVT_UNKNOWN_COMMAND,
	EVT_CONNECTED,
	EVT_DISCONNECTED,
	EVT_CHANNEL_ENABLED,
	EVT_CHANNEL_DISABLED,
	EVT_END	
};

#define CMD_LEN_GLUCOSE_MEASUREMENT		8
#define CMD_LEN_GLUCOSE_MEASUREMENT_CONTEXT 11
#define CMD_LEN_WEIGHT 7
#define CMD_LEN_DISCONNECT				10

enum CMD_INDEX
{
	CMD_SEND_GLUCOSE_MEASUREMENT,
	CMD_SEND_GLUCOSE_MEASUREMENT_CONTEXT,
	CMD_DISCONNECT,
	CMD_END	
};

#define MAX_UART_CMD_BUFFER		30
#define MAX_UART_EVT_BUFFER		30

#define UART_END_PATTERN			0x0d0a				/**< Definition of end pattern. */

#define UART_STATE_IDLE				0					/**< Definition of UART idle state. */
#define UART_STATE_WAIT				1					/**< Definition of UART Wait state. */

#define APP_RX_BUFFER_LEN			256					/**< Length of UART RX buffer. */
#define APP_TX_BUFFER_LEN			256					/**< Length of UART TX buffer. */


/**@brief The structure of connection list. */
typedef struct APP_CONN_STRUCT
{
	u8					handle;					/**< Connection handle. */
}APP_CONN_STRUCT;

typedef struct APP_CMD_STRUCT
{
	u8					length;
	u8					content[MAX_UART_CMD_BUFFER];
}APP_CMD_STRUCT;

typedef struct APP_EVENT_STRUCT
{
	u8					length;
	u8					content[MAX_UART_EVT_BUFFER];
}APP_EVENT_STRUCT;

#endif