/********************************************************************
  File Information:
    FileName:       pwm.h
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
 * @addtogroup PWM PWM
 * @{
 * @brief Definitions and prototypes for the PWM application programming interface, MBIoT PWM is Edge-aligned mode and up-count.
 */
#ifndef PWM_H
#define PWM_H


/**
 * @defgroup PWM_CHANNEL_MASK PWM_CHANNEL_MASK
 * @brief Mask of PWM channel.
 * @{ */
#define PWM0_MASK			(1<<0)	/**< Mask of PWM channel 0. The pin is P21*/
#define PWM1_MASK			(1<<1)	/**< Mask of PWM channel 1. The pin is P22*/
#define PWM2_MASK			(1<<2)	/**< Mask of PWM channel 2. The pin is P23*/
#define PWM3_MASK			(1<<3)	/**< Mask of PWM channel 3. The pin is P25*/
/** @} */


/**
 * @brief  Definition of PWM clock source.
 */
enum PWM_CLOCK_SOURCE
{ 
	PWM_CLOCK_32K,			/**< 32KHz. */
	PWM_CLOCK_1024K,		/**< 1024KHz. */
	PWM_CLOCK_16M			/**< 16MHz. */
};


/**
 * @brief  Definition of PWM output polarity.
 */
enum PWM_OUTPUT_POLARITY
{ 
	POSITIVE_OUTPUT,			/**< Output of positive. */
	INVERSE_OUTPUT				/**< Inverse PWM waveforms. */
};


/**
 * @brief  Structure of PWM configuration parameters.
 */
typedef struct APP_PWM_ConfParas
{
	u8 clockSource;			/**< PWM clock source, see @ref PWM_CLOCK_SOURCE. */
	u16 autoReloadValue;	/**< The auto-reload value. And autoReloadValue must larger than compareValue.*/
	u16 compareValue;		/**< The compare value.*/
	u8 outputPolarity;		/**< PWM output polarity, see @ref PWM_OUTPUT_POLARITY. */
}APP_PWM_ConfParas;


/**
 *@brief Initialize PWM Channel 
 *
 *@param[in] pwmMask	The mask of PWM channel.\n
 *						If PWM0_MASK is set, the P21 is occupied. If PWM1_MASK is set, the P22 is occupied.If PWM2_MASK is set, the P23 is occupied. If PWM3_MASK is set, the GPIO P25 is occupied.
 *@param[in] p_pwmParas	Pointer to the APP_PWM_ConfParas structure.
 *
 *@return MBIOT_RES_SUCCESS Successfully configuring PWM Channel.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 *							
 *@note		Example:\n
 *			XDATA APP_PWM_ConfParas pwm = {PWM_CLOCK_32K, 319, 159, POSITIVE_OUTPUT};\n
 * 			APP_PWM_Config((PWM0_MASK), &pwm); \n
 *			The result of PWM0 is :\n
 *			Frequency=clockSource/(autoReloadValue+1)=32000/(319+1) = 100 Hz \n
 *			Duty cycle=((autoReloadValue-compareValue)/(autoReloadValue+1))* 100% =  (319-159)/(319+1)=50%\n
 */
PUBLIC extern u16 APP_PWM_Config(u8 pwmMask, APP_PWM_ConfParas XDATA *p_pwmParas) large;


/**
 *@brief  Free PWM channel bus, the occupied GPIO pins return to input and internal pull high. 
 *
 *@param[in] pwmMask		The mask of PWM channel.
 *
 *@return MBIOT_RES_SUCCESS Successfully free PWM channel bus.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 *
 *@note		Example:\n
 *			Free PWM channel 1 and 2 bus.\n
 *			APP_PWM_Close(PWM1_MASK|PWM2_MASK);\n
 */
PUBLIC extern u16 APP_PWM_Close(u8 pwmMask) large;


/**
 *@brief Start PWM signal by specified channel(s).
 *
 *@param[in] pwmMask		The mask of PWM channel.
 *
 *@return MBIOT_RES_SUCCESS Successfully start the PWM.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 *
 *@note		PWM must be initialized befor start PWM signal. And the parameters of PWM can't be changed during PWM signal is actived.\n
 *			PWM signal must be stoped before changing the parameters of PWM. \n
 *			Example:\n
 *			Start PWM couner for PWM channel 1.\n
 *			APP_PWM_Start(PWM1_MASK);\n
 */
PUBLIC extern u16 APP_PWM_Start(u8 pwmMask)large;


/**
 *@brief Stop specified PWM channel signal.
 *
 *@param[in] pwmMask	The mask of PWM channel.
 *
 *@return MBIOT_RES_SUCCESS Successfully stop the specified PWM channel signal.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 *
 *note		Example:\n
 *			Stop PWM couner for PWM channel 2.\n
 *			APP_PWM_Stop(PWM2_MASK);\n
 */
PUBLIC extern u16 APP_PWM_Stop(u8 pwmMask)large;


#endif

/**
  @}
 */
