/********************************************************************
  File Information:
    FileName:       common.h
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
  @addtogroup APP_COMMON Common
  @{
  @brief APP Common
  @note Definitions and prototypes for the Commmon application programming interface.
 */
 
 
#ifndef COMMON_H
#define COMMON_H

#include "utility.h"
#include "timer.h"
#include "debug.h"
#include "memory.h"

/**@brief APP COMMON call back events. */
enum Common_Event_ID
{
	APP_TIMER_EVT,				/**< Time up event. See @ref APP_TIMER_buffer for the event content. */
	APP_OTA_EVT					/**< OTA request event. */
};


/** @brief Parameter of timer event. */
typedef struct	APP_TIMER_buffer
{
	u8 timerID;					/**< Timer ID. */
}APP_TIMER_buffer;


/**@brief Structure of APP COMMON events. */
typedef union 
{ 
	APP_TIMER_buffer			evtTimer;			/**< Data field of Timer event. */
}CommonEventField;


/**@brief APP COMMON callback function parameters. */
typedef struct	APP_Common_Event
{
	u16	eventID;											/**< Event ID. See @ref Common_Event_ID. */
	CommonEventField		eventField;						/**< Data field of this event. */
}APP_Common_Event;


/**@brief APP COMMON callback function prototype. */
typedef void(*CommonProcessRoutine)(APP_Common_Event XDATA *p_event) large;

/**
 *@brief Initialize APP COMMON callback function.
 *
 *@param[in] commonHandler		MBIoT callback function for APP COMMON events.
 */
PUBLIC extern void APP_COMMON_Init(CommonProcessRoutine commonHandler) large;

#endif 

/**
  @}
*/