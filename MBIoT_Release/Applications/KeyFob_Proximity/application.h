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

//#define APP_OPEN_DEBUG_MODE

#define APP_DEBUG_TX_BUFFER_LEN			512
#define APP_DEBUG_RX_BUFFER_LEN			256

#define APP_HEAP_SIZE            		(1024)		/**< The size of app heap. And it must be divisible by 32. */

/**@brief Definition of event length. */
#define EVT_LEN_ADVERTISING						13			/**< Length of advertising event. */
#define EVT_LEN_UNSPECIFIC_ERROR			18			/**< Length of unspecific error event. */
#define EVT_LEN_UNKNOWN_COMMAND				16			/**< Length of unknow command event. */
#define EVT_LEN_CONNECTED							11			/**< Length of connected event. */
#define EVT_LEN_DISCONNECTED					14			/**< Length of disconnected event. */
#define EVT_LEN_CHANNEL_ENABLED				22			/**< Length of channel enabled event. */
#define EVT_LEN_CHANNEL_DISABLED			23			/**< Length of channel disabled event. */
#define EVT_LEN_NEW_ALERT							11			/**< Lenght of new alert event.*/
#define EVT_LEN_UNREAD_ALERT_STATUS		21			/**< Length of the unread alert status event.*/

/**@brief Definition of event index. */
enum EVT_INDEX
{
	EVT_ADVERTISING,										/**< Index of advertising event. */
	EVT_UNSPECIFIC_ERROR,								/**< Index of unspecific error event. */
	EVT_UNKNOWN_COMMAND,								/**< Index of unknow command event. */
	EVT_CONNECTED,											/**< Index of connected event. */
	EVT_DISCONNECTED,										/**< Index of disconnected event. */
	EVT_CHANNEL_ENABLED,								/**< Index of channel enabled event. */
	EVT_CHANNEL_DISABLED,								/**< Index of channel disabled event. */
	EVT_NEW_ALERT,											/**< Index of new alert event. */
	EVT_UNREAD_ALERT_STATUS,						/**< Index of unread alert status event. */
	EVT_END	
};

/**@brief Definition of alerts command length. */
#define CMD_LEN_NEW_ALERT							8			/**< Length of new alert command. */
#define CMD_LEN_UNREAD_ALERT_STATUS		17		/**< Lenght of unread alert status command. */
#define CMD_LEN_CONNECT								7			/**< Lenght of connect command. */
#define CMD_LEN_DISCONNECT						10		/**< Length of disconnect command. */ 

/**@brief Definition of alerts command index. */
enum CMD_INDEX
{
	CMD_NEW_ALERT,
	CMD_UNREAD_ALERT_STATUS,
	CMD_CONNECT,
	CMD_DISCONNECT,
	CMD_END
};

/**@brief Definition of alert categories event length. */
#define EVT_LEN_SIMPLEALERT 14			/**< Length of simple alert event. */
#define EVT_LEN_EMAIL				7				/**< Length of email event. */
#define EVT_LEN_NEWS        6				/**< Length of news event. */ 
#define EVT_LEN_CALL        6				/**< Length of call event. */  
#define EVT_LEN_MISSEDCALL  13			/**< Length of missed call event. */  
#define EVT_LEN_SMSMMS      9				/**< Length of sms/mms event. */
#define EVT_LEN_VOICEMAIL   12			/**< Length of voice mail event .*/ 
#define EVT_LEN_SCHEDULE    10			/**< Length of schedule event. */
#define EVT_LEN_HIGHPRIORITIZED_ALERT		22		/**< Length of high prioritized alert event. */
#define EVT_LEN_INSTANT_MESSAGE					17		/**< Length of instant message event. */

/**@brief Definition of alert categories event index. */
enum ALERT_EVT_INDEX
{
	EVT_SIMPLEALERT,								/**< Index of simple alert event. */
	EVT_EMAIL,											/**< Index of email event. */
	EVT_NEWS,												/**< Index of news event. */ 
	EVT_CALL,												/**< Index of call event. */
	EVT_MISSEDCALL,									/**< Index of missed call event. */
	EVT_SMSMMS,											/**< Index of sms/mms event. */
	EVT_VOICEMAIL,									/**< Index of voice mail event .*/
	EVT_SCHEDULE,										/**< Index of schedule event. */
	EVT_HIGHPRIORITIZED_ALERT,			/**< Index of high prioritized alert event. */
	EVT_INSTANT_MESSAGE,						/**< Index of instant message event. */
	EVT_ALERT_END	
};

/**@brief Definition of alert categories command length. */
#define CMD_LEN_SIMPLEALERT 12		/**< Length of simple alert command. */
#define CMD_LEN_EMAIL				5			/**< Length of email command. */
#define CMD_LEN_NEWS        4 		/**< Length of news command. */
#define CMD_LEN_CALL        4			/**< Length of call command. */  
#define CMD_LEN_MISSEDCALL  11		/**< Length of missed call command. */  
#define CMD_LEN_SMSMMS      7			/**< Length of sms/mms command. */
#define CMD_LEN_VOICEMAIL   10		/**< Length of voice mail command .*/ 
#define CMD_LEN_SCHEDULE    8			/**< Length of schedule command. */
#define CMD_LEN_HIGHPRIORITIZED_ALERT		20		/**< Length of high prioritized alert command. */ 
#define CMD_LEN_INSTANT_MESSAGE					15		/**< Length of instant message command. */

/**@brief Definition of alert categories command index. */
enum ALERT_CMD_INDEX
{
	CMD_SIMPLEALERT,							/**< Index of simple alert command. */
	CMD_EMAIL,										/**< Index of email command. */
	CMD_NEWS,											/**< Index of news command. */
	CMD_CALL,											/**< Index of call command. */
	CMD_MISSEDCALL,								/**< Index of missed call command. */
	CMD_SMSMMS,										/**< Index of sms/mms command. */
	CMD_VOICEMAIL,								/**< Index of voice mail command .*/
	CMD_SCHEDULE,									/**< Index of schedule command. */
	CMD_HIGHPRIORITIZED_ALERT,		/**< Index of high prioritized alert command. */
	CMD_INSTANT_MESSAGE,					/**< Index of instant message command. */
	CMD_ALERT_END	
};

/**@brief Definition of alert level event length. */
#define EVT_LEN_NO_ALERT			10		/**< Lenght of No Alert event. */
#define EVT_LEN_MILD_ALERT		12		/**< Lenght of Mild Alert event. */
#define EVT_LEN_HIGH_ALERT		12		/**< Lenght of High Alert event. */

/**@brief Definition of alert level event index. */
enum ALERT_LEVEL_EVT_INDEX
{
	EVT_NO_ALERT,
	EVT_MILD_ALERT,
	EVT_HIGH_ALERT,
	EVT_ALERT_LEVEL_END
};

/**@brief Definition of alert level signal length. */
#define SIG_LEN_NO_ALERT			14		/**< Lenght of No Alert signal. */
#define SIG_LEN_MILD_ALERT		16		/**< Lenght of Mild Alert signal. */
#define SIG_LEN_HIGH_ALERT		16		/**< Lenght of High Alert signal. */

/**@brief Definition of alert level signal index. */
enum ALERT_LEVEL_SIG_INDEX
{
	SIG_NO_ALERT,
	SIG_MILD_ALERT,
	SIG_HIGH_ALERT,
	SIG_ALERT_LEVEL_END
};

#define MAX_UART_CMD_BUFFER		30
#define MAX_UART_EVT_BUFFER		30

#define UART_END_PATTERN			0x0d0a			/**< Definition of end pattern. */

#define UART_STATE_IDLE				0						/**< Definition of UART idle state. */
#define UART_STATE_WAIT				1						/**< Definition of UART Wait state. */

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
	u8					length;														/**< Command length. */
	u8					content[MAX_UART_CMD_BUFFER];			/**< Command definition. */
}APP_CMD_STRUCT;


/**@brief The structure of event parameters. */
typedef struct APP_EVENT_STRUCT
{
	u8					length;														/**< Event length. */
	u8					content[MAX_UART_EVT_BUFFER];			/**< Event definition. */
}APP_EVENT_STRUCT;

/**@brief The structure of signal parameters. */
typedef struct APP_SIGNAL_STRUCT
{
	u8					length;														/**< Event length. */
	u8					content[MAX_UART_EVT_BUFFER];			/**< Event definition. */
}APP_SIGNAL_STRUCT;

#endif