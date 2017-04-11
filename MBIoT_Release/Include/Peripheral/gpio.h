/********************************************************************
  File Information:
    FileName:       gpio.h
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
 * @addtogroup GPIO GPIO
 * @{
 * @brief Definitions and prototypes for the GPIO application programming interface.
 */
#ifndef GPIO_H
#define GPIO_H


/**
 * @brief  Definition of GPIO ports.
 */
enum GPIO_PORT_DEF
{
	GPIO_PORT0 = 0,		/**< P00-P07 */
	GPIO_PORT1,			/**< P10-P17 */
	GPIO_PORT2,			/**< P20-P27 */
	GPIO_PORT3			/**< P30-P37 */
};


/**
 * @brief  Definition of GPIO pin number.
 */
enum GPIO_PIN_NUM_DEF
{
	GPIO_P00 = 0,		/**< P00 */
	GPIO_P01,			/**< P01 */
	GPIO_P02,			/**< P02 */
	GPIO_P03,			/**< P03 */
	GPIO_P04,			/**< P04 */
	GPIO_P05,			/**< P05 */
	GPIO_P06,			/**< P06 */
	GPIO_P07,			/**< P07 */
	GPIO_P10,			/**< P10 */
	GPIO_P11,			/**< P11 */
	GPIO_P12,			/**< P12 */
	GPIO_P13,			/**< P13 */
	GPIO_P14,			/**< P14 */
	GPIO_P15,			/**< P15 */
	GPIO_P16,			/**< P16 */
	GPIO_P17,			/**< P17 */
	GPIO_P20,			/**< P20 */
	GPIO_P21,			/**< P21 */
	GPIO_P22,			/**< P22 */
	GPIO_P23,			/**< P23 */
	GPIO_P24,			/**< P24 */
	GPIO_P25,			/**< P25 */
	GPIO_P26,			/**< P26 */
	GPIO_P27,			/**< P27 */
	GPIO_P30,			/**< P30 */
	GPIO_P31,			/**< P31 */
	GPIO_P32,			/**< P32 */
	GPIO_P33,			/**< P33 */
	GPIO_P34,			/**< P34 */
	GPIO_P35,			/**< P35 */
	GPIO_P36,			/**< P36 */
	GPIO_P37,			/**< P37 */
	GPIO_NULL
};


/**
 * @defgroup GPIO_DIR_DEF GPIO_DIR_DEF
 * @brief Direction of GPIO pin/port.
 * @{ */
#define GPIO_DIR_INPUT			0		/**< Input*/
#define GPIO_DIR_OUTPUT			1		/**< Output. */
/** @} */


/**
 * @defgroup GPIO_VALUE_DEF GPIO_VALUE_DEF
 * @brief The value of a GPIO.
 * @{ */
#define GPIO_VALUE_LOW			0		/**< LOW*/
#define GPIO_VALUE_HIGH			1		/**< HIGH. */
/** @} */


/**
 * @defgroup GPIO_PORT_BIT_MASK GPIO_PORT_BIT_MASK
 * @brief Bit mask of the GPIO port.
 * @{ */
#define GPIO_PORT_MASK_BIT0			(1<<0)		/**< Bit 0*/
#define GPIO_PORT_MASK_BIT1			(1<<1)		/**< Bit 1*/
#define GPIO_PORT_MASK_BIT2			(1<<2)		/**< Bit 2*/
#define GPIO_PORT_MASK_BIT3			(1<<3)		/**< Bit 3*/
#define GPIO_PORT_MASK_BIT4			(1<<4)		/**< Bit 4*/
#define GPIO_PORT_MASK_BIT5			(1<<5)		/**< Bit 5*/
#define GPIO_PORT_MASK_BIT6			(1<<6)		/**< Bit 6*/
#define GPIO_PORT_MASK_BIT7			(1<<7)		/**< Bit 7*/
/** @} */


typedef struct APP_GPIO_Port_Mask{
    u8	maskPort0;								/**< The mask of Port0, see @ref GPIO_PORT_BIT_MASK. */
    u8	maskPort1;								/**< The mask of Port1, see @ref GPIO_PORT_BIT_MASK. */
    u8	maskPort2;								/**< The mask of Port2, see @ref GPIO_PORT_BIT_MASK. */
    u8	maskPort3;								/**< The mask of Port3, see @ref GPIO_PORT_BIT_MASK. */
}APP_GPIO_Port_Mask;



/**
 *@brief Configure the direction of GPIO pins within a specified range.
 *
 *@param[in] pinStart		The starting pin number of the range, see @ref GPIO_PIN_NUM_DEF.
 *@param[in] pinEnd			The end pin number of the range,  see @ref GPIO_PIN_NUM_DEF.
 *@param[in] direction		Input/Output, see @ref GPIO_DIR_DEF.
 *
 *@return MBIOT_RES_SUCCESS Successfully configure a range of GPIO pins.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 *
 *@note 	Example:\n
 *			Configure P00-P05 to be output.\n
 *			APP_GPIO_Range_Config_Direction(GPIO_P00, GPIO_P05, GPIO_DIR_OUTPUT);\n
 */
PUBLIC extern u16 APP_GPIO_Range_Config_Direction(u8 pinStart, u8 pinEnd, u8 direction) large;


/**
 *@brief Configure the direction of a specified GPIO pin.
 *
 *@param[in] pinNum			Pin number, see @ref GPIO_PIN_NUM_DEF.
 *@param[in] direction		Input/Output, see @ref GPIO_DIR_DEF.
 *
 *@return MBIOT_RES_SUCCESS Successfully configure the direction of a specified GPIO pin.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 *
 *@note 	Example:\n
 *			Configure P05 to be output.\n
 *			APP_GPIO_Pin_Config_Direction(GPIO_P05, GPIO_DIR_DEF_OUTPUT);\n
 */
PUBLIC extern u16 APP_GPIO_Pin_Config_Direction(u8 pinNum, u8 direction) large;


/**
 *@brief Configure the direction of a specified GPIO port.
 *
 *@param[in] portNum		Port number, see @ref GPIO_PORT_DEF.
 *@param[in] direction		Input/Output, see @ref GPIO_DIR_DEF.
 *
 *@return MBIOT_RES_SUCCESS Successfully configure direction of a GPIO port.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 *
 *@note 	Example:
 *			Configure port1 to be output.
 *			APP_GPIO_Port_Config_Direction(GPIO_PORT1, GPIO_DIR_OUTPUT)
 */
PUBLIC extern u16 APP_GPIO_Port_Config_Direction(u8 portNum, u8 direction) large;


/**
 *@brief Set a specified GPIO pin to HIGH.
 *
 *@param[in] pinNum		Pin number, @ref GPIO_PIN_NUM_DEF.
 *
 *@return MBIOT_RES_SUCCESS Successfully set a specified GPIO pin to HIGH.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 *
 *@note 	The direction of the specified GPIO pin must be configured to output before this API is executed.\n
 *			Example:\n
 *			Set P01 to HIGH.\n
 *			APP_GPIO_Pin_Set(GPIO_P01);\n
 */
PUBLIC extern u16 APP_GPIO_Pin_Set(u8 pinNum) large;


/**
 *@brief Set a specified GPIO pin to LOW.
 *
 *@param[in] pinNum		Pin number, @ref GPIO_PIN_NUM_DEF.
 *
 *@return MBIOT_RES_SUCCESS Successfully clear a GPIO pin.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 *
 *@note 	The direction of the specified GPIO pin must be configured to output before this API is executed.\n
 *			Example:\n
 *			Set P0_1 to LOW.\n
 *			APP_GPIO_Pin_Clear(GPIO_P01);\n
 */
PUBLIC extern u16 APP_GPIO_Pin_Clear(u8 pinNum) large;


/**
 *@brief Get the value of a specified GPIO pin.
 *
 *@param[in] pinNum			Pin number, @ref GPIO_PIN_NUM_DEF.
 *@param[out] p_readValue	Pointer to the value got via specified pin, see @ref GPIO_VALUE_DEF.
 *
 *@return MBIOT_RES_SUCCESS Successfully gets the value of a specified GPIO pin.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 *
 *@note 	The direction of the specified GPIO pin must be configured to output before this API is executed.\n
 *			Example:\n
 *			XDATA u8 readValue;\n
 *			Get the value of P01.\n
 *			APP_GPIO_Pin_Read(GPIO_P01, &readValue);\n
 */
PUBLIC extern u16 APP_GPIO_Pin_Read(u8 pinNum, u8 XDATA *p_readValue) large;


/**
 *@brief Set multiple GPIO pins within a PORT to HIGH.
 *
 *@param[in] portNum	Port number, see @ref GPIO_PORT_DEF.
 *@param[in] setMask	Bit mask of the specified GPIO port, see @ref GPIO_PORT_BIT_MASK. The GPIO pin(s) can be set to HIGH only when the bit mask is configred. And the other GPIO pin(s) will not be influenced.
 *
 *@return MBIOT_RES_SUCCESS Successfully set mutilple GPIO pins within a PORT to HIGH.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 *
 *@note 	The direction of the specified GPIO port must be configured to output before this API is executed.\n
 *			Example:\n
 *			Set GPIO port1 bit 1(P11) and bit2(P12) to HIGH.\n
 *			APP_GPIO_Port_Set(GPIO_PORT1, GPIO_PORT_MASK_BIT1|GPIO_PORT_MASK_BIT2);\n
 */
PUBLIC extern u16 APP_GPIO_Port_Set(u8 portNum, u8 setMask) large;


/**
 *@brief Set multiple GPIO pins within a PORT to LOW.
 *
 *@param[in] portNum	Port number, see @ref GPIO_PORT_DEF.
 *@param[in] clearMask	Bit mask of the specified GPIO port, see @ref GPIO_PORT_BIT_MASK. The GPIO pin(s) can be set to LOW only when the bit mask is configred. And the other GPIO pin(s) will not be influenced.
 *
 *@return MBIOT_RES_SUCCESS Successfully set mutilple GPIO pins within a PORT to LOW.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 *
 * @note 	The direction of the specified GPIO port must be configured to output before this API is executed.\n
 *			Example:\n
 *			Set GPIO port3 bit 5(P35) and bit7(P37) to LOW.\n
 *			APP_GPIO_Port_Clear(GPIO_PORT3, GPIO_PORT_MASK_BIT5|GPIO_PORT_MASK_BIT7);\n
 */
PUBLIC extern u16 APP_GPIO_Port_Clear(u8 portNum, u8 clearMask) large;


/**
 *@brief Assign a value as a specified GPIO port output value.
 *
 *@param[in] portNum		Port number, see @ref GPIO_PORT_DEF.
 *@param[in] writeValue		The value assigned as the specified GPIO port output value.
 *
 *@return MBIOT_RES_SUCCESS Successfully asign a value as a specified GPIO port output value.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 *
 *@note 	The direction of the specified GPIO port must be configured to output before this API is executed.\n
 *			Example:\n
 *			Asign 0x35 as GPIO port1 output value.\n
 *			APP_GPIO_Port_Write(GPIO_PORT1, 0x35);\n
 */
PUBLIC extern u16 APP_GPIO_Port_Write(u8 portNum, u8 writeValue) large;


/**
 *@brief Get a value via a specified GPIO port.
 *
 *@param[in] portNum		Port number, see @ref GPIO_PORT_DEF.
 *@param[out] p_readValue	Pointer to the value got via specified GPIO port.
 *
 *@return MBIOT_RES_SUCCESS Successfully get a value via a specified GPIO port.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 *
 *@note 	The direction of the specified GPIO port must be configured to input before this API is executed.\n
 *			Example:
 *			XDATA u8 readValue;
 *			Get a value via GPIO port1.
 *			APP_GPIO_Port_Write(GPIO_PORT1, &readValue);
 */
PUBLIC extern u16 APP_GPIO_Port_Read(u8 portNum, u8 XDATA *p_readValue) large;


/**
 *@brief Start the specified GPIOs polling.
 *
 *@param[in] p_startMask	Pointer to APP_GPIO_Port_Mask structure.
 */
PUBLIC  extern void APP_GPIO_Polling_Start(APP_GPIO_Port_Mask XDATA *p_startMask) large;


/**
 *@brief Stop the specified GPIOs polling.
 *
 *@param[in] p_stopMask	Pointer to APP_GPIO_Port_Mask structure.
 */
PUBLIC  extern void APP_GPIO_Polling_Stop(APP_GPIO_Port_Mask XDATA *p_stopMask) large;
#endif
/**
  @}
 */