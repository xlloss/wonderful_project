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


#define EVT_LEN_ADVERTISING					13			/**< Length of advertising event. */
#define EVT_LEN_UNSPECIFIC_ERROR			18			/**< Length of unspecific error event. */
#define EVT_LEN_UNKNOWN_COMMAND				16			/**< Length of unknow command event. */
#define EVT_LEN_CONNECTED					11			/**< Length of connected event. */
#define EVT_LEN_DISCONNECTED				14			/**< Length of disconnected event. */
#define EVT_LEN_CHANNEL_ENABLED				22			/**< Length of channel enabled event. */
#define EVT_LEN_CHANNEL_DISABLED			23			/**< Length of channel disabled event. */
#define EVT_LEN_SHUTDOWN                    10

/**@brief Definition of event index. */
enum EVT_INDEX
{
	EVT_ADVERTISING,									/**< Index of advertising event. */
	EVT_UNSPECIFIC_ERROR,								/**< Index of unspecific error event. */
	EVT_UNKNOWN_COMMAND,								/**< Index of unknow command event. */
	EVT_CONNECTED,										/**< Index of connected event. */
	EVT_DISCONNECTED,									/**< Index of disconnected event. */
	EVT_CHANNEL_ENABLED,								/**< Index of channel enabled event. */
	EVT_CHANNEL_DISABLED,								/**< Index of channel disabled event. */
	EVT_END	
};

#define CMD_LEN_BLOOD_PRESSURE				14
#define CMD_LEN_INTER_CUFF_PRESSURE		10
#define CMD_LEN_DISCONNECT				    10
#define CMD_LEN_SHUTDOWN                    8

/**@brief Definition of command index. */
enum CMD_INDEX
{
	CMD_SEND_BLOOD_PRESSURE,		/**< Length of send BP command. */
	CMD_SEND_INTER_CUFF_PRESSURE,		/**< Length of send INter cuff pressure command. */
	CMD_DISCONNECT,					/**< Length of disconnect command. */
	CMD_SHUTDOWN,					/**< Length of Shutdown command. */
	CMD_END	
};


#define MAX_UART_CMD_BUFFER			30					/**< Maximum command length. */
#define MAX_UART_EVT_BUFFER			30					/**< Maximum event length. */

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


/**@brief The structure of cmd parameters. */
typedef struct APP_CMD_STRUCT
{
	u8					length;									/**< Command length. */
	u8					content[MAX_UART_CMD_BUFFER];			/**< Command definition. */
}APP_CMD_STRUCT;


/**@brief The structure of event parameters. */
typedef struct APP_EVENT_STRUCT
{
	u8					length;									/**< Event length. */
	u8					content[MAX_UART_EVT_BUFFER];			/**< Event definition. */
}APP_EVENT_STRUCT;

#endif //APPLICATION_H