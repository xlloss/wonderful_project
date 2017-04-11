/********************************************************************
  File Information:
    FileName:       adc.h
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
 * @addtogroup ADC ADC
 * @{
 * @brief Definitions and prototypes for the ADC (Analog-to-Digital Converter) application programming interface.
 */
#ifndef ADC_H
#define ADC_H


/**
 * @defgroup ADC_CHANNEL_DEF ADC_CHANNEL_DEF
 * @brief Definition of ADC channel bus.
 * @note All ADC channel buses are shared with GPIO pins.
 * @{ */
#define ADC_CH0_BIT			(1<<0)		/**< ADC channel 0. The pin is P00.*/
#define ADC_CH1_BIT			(1<<1)		/**< ADC channel 1. The pin is P01. */
#define ADC_CH2_BIT			(1<<2)		/**< ADC channel 2. The pin is P02. */
#define ADC_CH3_BIT			(1<<3)		/**< ADC channel 3. The pin is P03. */
#define ADC_CH4_BIT			(1<<4)		/**< ADC channel 4. The pin is P04. */
#define ADC_CH5_BIT			(1<<5)		/**< ADC channel 5. The pin is P05. */
#define ADC_CH6_BIT			(1<<6)		/**< ADC channel 6. The pin is P06. */
#define ADC_CH7_BIT			(1<<7)		/**< ADC channel 7. The pin is P07. */
#define ADC_CH8_BIT			(1<<8)		/**< ADC channel 8. The pin is P10. */
#define ADC_CH9_BIT			(1<<9)		/**< ADC channel 9. The pin is P11. */
#define ADC_CH10_BIT		(1<<10)		/**< ADC channel 10. The pin is P12. */
#define ADC_CH11_BIT		(1<<11)		/**< ADC channel 11. The pin is P13. */
#define ADC_CH12_BIT		(1<<12)		/**< ADC channel 12. The pin is P14. */
#define ADC_CH13_BIT		(1<<13)		/**< ADC channel 13. The pin is P15. */
#define ADC_CH14_BIT		(1<<14)		/**< ADC channel 14. The pin is P27. */
#define ADC_CH15_BIT		(1<<15)		/**< ADC channel 15. The pin is P25. */
/** @} */


/**
 * @brief  Definition of ADC channel number.
 */
enum ADC_CHANNEL_NUM
{
	ADC_CHANNEL0 = 0,		 /**< ADC Channel 0. The pin is P00.*/
	ADC_CHANNEL1,			 /**< ADC Channel 1. The pin is P01.*/
	ADC_CHANNEL2,			 /**< ADC Channel 2. The pin is P02.*/
	ADC_CHANNEL3,			 /**< ADC Channel 3. The pin is P03.*/
	ADC_CHANNEL4,			 /**< ADC Channel 4. The pin is P04.*/
	ADC_CHANNEL5,			 /**< ADC Channel 5. The pin is P05.*/
	ADC_CHANNEL6,			 /**< ADC Channel 6. The pin is P06.*/
	ADC_CHANNEL7,			 /**< ADC Channel 7. The pin is P07.*/
	ADC_CHANNEL8,			 /**< ADC Channel 8. The pin is P10.*/
	ADC_CHANNEL9,			 /**< ADC Channel 9. The pin is P11.*/
	ADC_CHANNEL10,		 	 /**< ADC Channel 10. The pin is P12.*/
	ADC_CHANNEL11,		 	 /**< ADC Channel 11. The pin is P13.*/
	ADC_CHANNEL12,		 	 /**< ADC Channel 12. The pin is P14.*/
	ADC_CHANNEL13,		 	 /**< ADC Channel 13. The pin is P15.*/
	ADC_CHANNEL14,		 	 /**< ADC Channel 14. The pin is P25.*/
	ADC_CHANNEL15			 /**< ADC Channel 15. The pin is P27.*/
};


/**
 *@brief  Initialize ADC channel(s).
 *
 *@param[in] channel	Specify which channel(s) will be used, see @ref  ADC_CHANNEL_DEF. 
 *
 *@note 	Example:\n
 *			Initialize ADC Channe1 and Channel 13.\n
 *			APP_ADC_Init((ADC_CH1_BIT|ADC_CH13_BIT));\n
 */
PUBLIC extern  void  APP_ADC_Init(u16 channel) large;


/**
 *@brief Free all ADC channel bus, all occupied GPIOs return to digital input with internal pull high.
 */
PUBLIC extern  void  APP_ADC_Close(void) large;


/**
 *@brief Get current ADC value(12 bits resolution, and 10 bits ENOB) of specified ADC channel.
 *
 *@param[in] channel		Specify which ADC channel needs to be readed, see @ref ADC_CHANNEL_NUM.
 *@param[out] p_readValue	Pointer to the value got via specified channel.
 *
 *@return MBIOT_RES_SUCCESS Successfully get a value from specified ADC channel.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 *
 *@note	Exampe:\n
 *			Get ADC value via channel 1.\n
 *			XDATA u16 read;\n
 *			APP_ADC_GetValue(ADC_CHANNEL1, &read);\n
 */
PUBLIC extern  u16  APP_ADC_GetValue(u8 channel, u16 XDATA *p_readValue) large;


/**
 *@brief Get battery voltage of Vbat channel. 
 *
 *@param[out] p_readValue	Pointer to value got via BAT. (Unit: 0.025 voltage)
 */
PUBLIC extern void APP_ADC_GetBattery(u8 XDATA *p_readValue) large;

/**
 *@brief Get Reference Calibration Value. 
 *
 *@param[out] p_v18SarValue	Pointer to value got Reference SAR Value of 1.8 voltage. 
 *@param[out] p_v36SarValue	Pointer to value got Reference SAR Value of 3.6 voltage. 
 *
 *@note	Exampe:\n
 *			Get Reference Calibration Value.\n
 *			XDATA u16 v18,v36;\n
 *			APP_ADC_GetCalibrationValue((u8 XDATA *)&v18, (u8 XDATA *)&v36);\n
 */
PUBLIC extern void APP_ADC_GetCalibrationValue(u8 XDATA *p_v18SarValue, u8 XDATA *p_v36SarValue) large;

#endif	/*ADC_H*/

/**
  @}
 */
