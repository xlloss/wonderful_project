/********************************************************************
  File Information:
    FileName:       led.h
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
 * @addtogroup LED LED
 * @{
 * @brief Definitions and prototypes for the LED application programming interface.
 */
 
#ifndef LED_H
#define LED_H
 

/**
 * @defgroup LED_BIT_MASK LED_BIT_MASK
 * @brief Mask of LEDs.
 * @{ */
#define LED0_MASK									(1<<0)	/**< The mask of LED0. The pin is P02.*/
#define LED1_MASK									(1<<1)	/**< The mask of LED1. The pin is P35.*/
/** @} */


/**
 * @defgroup LED_LIGHTING_TYPE_SINGLE LED_LIGHTING_TYPE_SINGLE
 * @brief Type of single LED lighting.
 * @{ */
#define LED_TYPE_SINGLE_OFF							0	/**< LED is always off. */
#define LED_TYPE_SINGLE_ON							1	/**< LED is always on. */
#define LED_TYPE_SINGLE_FLASH						2	/**< LED flashes. */
/** @} */


/**
 * @defgroup LED_LIGHTING_TYPE_COMBO LED_LIGHTING_TYPE_COMBO
 * @brief Type of two LEDs lighting.
 * @{ */
#define LED_TYPE_COMBO_OFF						0		/**< LED0 and LED1 are always off. */
#define LED_TYPE_COMBO_ON						1		/**< LED0 and LED1 are always on. */
#define LED_TYPE_COMBO_FLASH_SIMULTANEOUSLY		2		/**< LED0 and LED1 flash simultaneously. */
#define LED_TYPE_COMBO_FLASH_INTERLEAVE			3		/**< LED0 and LED1 flash interleave. */
/** @} */


/**
 * @defgroup LED_LIGHTING_TIMES LED_LIGHTING_TIMES
 * @brief The lighting times of LED flashes.
 * @{ */
#define LED_TIMES_1				1	/**< LED(s) flash one time within a specified interval. */
#define LED_TIMES_2				2	/**< LED(s) flash two times within a specified interval. */
#define LED_TIMES_3				3	/**< LED(s) flash three times within a specified interval. */
/** @} */

/**
 * @defgroup LED_DURATION LED_DURATION
 * @brief The duration of LED turn on or off.
 * @{ */
#define LED_DURATION_50_MS		1	/**< LED(s) 50 ms duration. */
#define LED_DURATION_100_MS		2	/**< LED(s) 100 ms duration. */
#define LED_DURATION_1_SEC		20	/**< LED(s) 1 sec duration. */
/** @} */


/**
 * @brief  Structure of LED configuration parameters.
 */
typedef struct
{
	u8 flashType;		/**< Type of LED(s) lighting, see @ref LED_LIGHTING_TYPE_SINGLE or @ref LED_LIGHTING_TYPE_COMBO.*/			
	u8 onInterval;		/**< The interval of LED stes to on. (Unit: 50ms)*/
	u8 offInterval;		/**< The interval of LED sets to off. (Unit: 50ms)**/
	u8 count;			/**< The lighting times of LED flashes, see @ref LED_LIGHTING_TIMES. */
	u8 flashInterval; 	/**< The interval of LED flashes. (Unit: 50ms) */	
} APP_LED_ConfParas;


/**
 *@brief LED initialization
 *
 *@param[in] ledMask		Mask of LEDs. If LED0_MASK is set, the P02 is occupied. If LED1_MASK is set, the P35 is occupied. See @ref LED_BIT_MASK.
 *
 *@return MBIOT_RES_SUCCESS Successfully initialz LED.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 *
 *@note		Example:\n
 *			Initial LED0 and LED1.\n
 *			APP_LED_Init(LED0_MASK|LED1_MASK);\n
 */
PUBLIC extern u16 APP_LED_Init(u8 ledMask) large;


/**
 *@brief Free LED control, occupied GPIO returns to digital input with internal pull high.
 *
 *@param[in] ledMask
 *
 *@return MBIOT_RES_SUCCESS Successfully free LED bus.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 *
 *@note		Example:\n
 *			Close LED1.\n
 *			APP_LED_Close(LED1_MASK);\n
 */
PUBLIC extern u16 APP_LED_Close(u8 ledMask) large;


 /**
 *@brief Configure lighting parameters of LED0 and start to flash.
 *
 *@param[in] p_profile	Pointer to @ref APP_LED_ConfParas structure.
 *
 *@return MBIOT_RES_SUCCESS Successfully configure lighting parameters of LED0 and starts to flash.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 *							
 *@note		Example:\n
 *			LED0 flashes 2 tiimes within 1 minute, each time turn on 50ms and turn off 50ms .\n
 *			XDATA APP_LED_ConfParas led;\n
 *			led.flashType=LED_TYPE_SINGLE_FLASH;\n
 *			led.onInterval=LED_DURATION_50_MS;\n
 *			led.offInterval=LED_DURATION_100_MS;\n
 *			led.count=LED_TIMES_2;\n
 *			led.flashInterval=LED_DURATION_1_SEC;\n
 * 			APP_LED_LED0_Flash(&led);\n
 */
 PUBLIC extern u16 APP_LED_LED0_Flash(APP_LED_ConfParas XDATA * p_profile) large;


 /**
 *@brief Configure lighting parameters of LED1 and start to flash.
 *
 *@param[in] p_profile	Pointer to @ref APP_LED_ConfParas structure.
 *
 *@return MBIOT_RES_SUCCESS configure lighting parameters of LED1 and starts to flash.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 *							
 *@note		Example:\n
 *			LED1 always turns on.\n
 *			XDATA APP_LED_ConfParas led;\n
 *			led.flashType=LED_TYPE_SINGLE_ON;\n
 * 			APP_LED_LED0_Flash(&led);\n
 */
 PUBLIC extern u16 APP_LED_LED1_Flash(APP_LED_ConfParas XDATA * p_profile) large;


  /**
  *@brief Configure lighting parameters of LED1 and LED2, starts to flash both LEDs.
 *
 * @param[in] p_profile	Pointer to @ref APP_LED_ConfParas structure.
 *
 *@return MBIOT_RES_SUCCESS Successfully configure lighting parameters of LED1 and LED2, starts to flash both LEDs.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 *							
 *@note		Example:\n
 *			Interleave both LEDs flashes 3 tiimes within 2 minutes, each time turn on 100ms and turn off 50ms .\n
 *			XDATA APP_LED_ConfParas led;\n
 *			led.flashType=LED_TYPE_COMBO_FLASH_INTERLEAVE;\n
 *			led.onInterval=LED_DURATION_100_MS;\n
 *			led.offInterval=LED_DURATION_50_MS;\n
 *			led.count=LED_TIMES_3;\n
 *			led.flashInterval=(LED_DURATION_1_SEC*2);\n
 * 			APP_LED_Combo_Flash(&led);\n
 */
 PUBLIC extern u16 APP_LED_Combo_Flash(APP_LED_ConfParas XDATA * p_profile) large;


#endif

/**
  @}
 */