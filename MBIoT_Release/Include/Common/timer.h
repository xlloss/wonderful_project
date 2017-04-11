/********************************************************************
  File Information:
    FileName:       timer.h
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
  @addtogroup TIMER Timer
  @{
  @brief Definitions and prototypes for the Timer application programming interface.
 */
 
#ifndef TIMER_H
#define TIMER_H


/** @brief The definition of APP timer ID. */
enum APP_TIMER_ID
{
	APP_TIMER_ID_0,				/**< Timer ID 0 */
	APP_TIMER_ID_1,				/**< Timer ID 1 */
	APP_TIMER_ID_2,				/**< Timer ID 2 */
	APP_TIMER_ID_3,				/**< Timer ID 3 */
	APP_TIMER_ID_4,				/**< Timer ID 4 */
	APP_TIMER_ID_5,				/**< Timer ID 5 */
	APP_TIMER_ID_6,				/**< Timer ID 6 */
	APP_TIMER_ID_7,				/**< Timer ID 7 */
	APP_TIMER_ID_8,				/**< Timer ID 8 */
	APP_TIMER_ID_9,				/**< Timer ID 9 */
	APP_TIMER_ID_10,			/**< Timer ID 10 */
	APP_TIMER_ID_11,			/**< Timer ID 11 */
	APP_TIMER_ID_12,			/**< Timer ID 12 */
	APP_TIMER_ID_13,			/**< Timer ID 13 */
	APP_TIMER_ID_14,			/**< Timer ID 14 */
	APP_TIMER_ID_15				/**< Timer ID 15 */
};


/**
 *@brief Start a fast timer.
 *
 *@param[in] timerID		Timer ID ,see ref@ APP_TIMER_ID.
 *@param[in] timeoutTicks	Timeout ticks. (Unit: 0.625ms).
 *
 *@return MBIOT_RES_SUCCESS Successfully starts a fast timer.
 *@return MBIOT_RES_FAIL Timer operation is fail.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 */
PUBLIC extern u16 APP_TIMER_FastTimer_Start(u8 timerID, u16 timeoutTicks) large;


/**
 *@brief Stop a exist fast timer.
 *
 *@param[in] timerID		Timer ID ,see ref@ APP_TIMER_ID.
 *
 *@return MBIOT_RES_SUCCESS Successfully stops a exist fast timer.
 *@return MBIOT_RES_FAIL Timer operation is fail.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 */
PUBLIC extern u16 APP_TIMER_FastTimer_Stop(u8 timerID) large;


/**
 *@brief Start a slow timer
 *
 *@param[in] timerID		Timer ID ,see ref@ APP_TIMER_ID.
 *@param[in] timeoutTicks	Timeout ticks. (Unit: 640ms).
 *
 *@return MBIOT_RES_SUCCESS Successfully starts a slow timer.
 *@return MBIOT_RES_FAIL Timer operation is fail.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 */
PUBLIC extern u16 APP_TIMER_SlowTimer_Start(u8 timerID, u16 timeoutTicks) large;


/**
 *@brief Stop a exist slow timer.
 *
 *@param[in] timerID		Timer ID ,see ref@ APP_TIMER_ID.
 *
 *@return MBIOT_RES_SUCCESS Successfully stops a exist slow timer.
 *@return MBIOT_RES_FAIL Timer operation is fail.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 */
PUBLIC extern u16 APP_TIMER_SlowTimer_Stop(u8 timerID) large; 
#endif 
 /**
  @}
*/