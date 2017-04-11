/********************************************************************
  File Information:
    FileName:       ble_weight_scale_service.h
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
  @addtogroup BLE_Service BLE Service
  @{
 */
 
/**
  @}
 */
 

 
/**
  @addtogroup BLE_WSS Ble Weight Scale Service
  @ingroup BLE_Service BLE Service
  @{
  @brief Ble Weight Scale Serivce
  @note Definitions and prototypes for the Ble Weight Scale Serivce application programming interface.
 */
 
#ifndef BLE_WEIGHT_SCALE_SERVICE_H
#define BLE_WEIGHT_SCALE_SERVICE_H


/**
 * @defgroup WEIGHT_RESOLUTION WEIGHT_RESOLUTION
 * @brief Weight measure resolution.
 * @{ */
#define WEIGHT_RESOLUTION_05KG_OR_1IB			0x01		/**< Resolution of 0.5 kg or 1 lb. */
#define WEIGHT_RESOLUTION_02KG_OR_05IB			0x02		/**< Resolution of 0.2 kg or 0.5 lb. */
#define WEIGHT_RESOLUTION_01KG_OR_02IB			0x03		/**< Resolution of 0.1 kg or 0.2 lb. */
#define WEIGHT_RESOLUTION_005KG_OR_01IB			0x04		/**< Resolution of 0.05 kg or 0.1 lb. */
#define WEIGHT_RESOLUTION_002KG_OR_005IB		0x05		/**< Resolution of 0.02 kg or 0.05 lb. */
#define WEIGHT_RESOLUTION_001KG_OR_002IB		0x06		/**< Resolution of 0.01 kg or 0.02 lb. */
#define WEIGHT_RESOLUTION_0005KG_OR_001IB		0x07		/**< Resolution of 0.005 kg or 0.01 lb. */
/** @} */


/**
 * @defgroup HEIGHT_RESOLUTION HEIGHT_RESOLUTION
 * @brief Height measure resolution.
 * @{ */
#define HEIGHT_RESOLUTION_001METER_OR_1INCH		0x01		/**< Resolution of 0.01 meter or 1 inch. */
#define HEIGHT_RESOLUTION_005METER_OR_05INCH	0x02		/**< Resolution of 0.005 meter or 0.5 inch. */
#define HEIGHT_RESOLUTION_0001METER_OR_01INCH	0x03		/**< Resolution of 0.001 meter or 0.1 inch. */
/** @} */


/**
 * @defgroup MEASUREMENT_UNIT MEASUREMENT_UNIT
 * @brief Weight measurement unit.
 * @{ */
#define MEASUREMENT_UNIT_SI						0			/**< Weight and Mass in units of kilogram (kg) and Height in units of meter. */
#define MEASUREMENT_UNIT_IMPERIAL				1			/**< Weight and Mass in units of pound (lb) and Height in units of inch (in). */
/** @} */


/**
 * @defgroup WEIGHT_MEASUREMENT_CCCD_DEF WEIGHT_MEASUREMENT_CCCD_DEF
 * @brief The value of Weight Measurement Client Characterisitc Configuration Descriptor.
 * @{ */
#define WEIGHT_MEASUREMENT_INDICATE_DISABLED	0x0000			/**< Weight measurement indication is disabled. */
#define WEIGHT_MEASUREMENT_INDICATE_ENABLED		0x0200			/**< Weight measurement indication is enabled. */
/** @} */


#define UUID_WEIGHT_SCALE_SERVICE				0x181D			/**< Definition of Weight Scale service UUID. */
#define UUID_WEIGHT_SCALE_FEATURE				0x2A9E			/**< Definition of Weight Scale Feature characteristic UUID. */
#define UUID_WEIGHT_MEASUREMENT					0x2A9D			/**< Definition of Weight Measurement characteristic UUID. */


/**
 * @defgroup WEIGHT_SCALE_FEATURE_BIT WEIGHT_SCALE_FEATURE_BIT
 * @brief Definition of Weight Scale Feature bits.
 * @{ */
#define WEIGHT_SCALE_FEATURE_TIMESTAMP_BIT				0			/**< Time Stamp supported. */
#define WEIGHT_SCALE_FEATURE_MULTIPLE_USER_BIT			1			/**< Multiple user supported. */
#define WEIGHT_SCALE_FEATURE_BMI_BIT					2			/**< BMI supported. */
#define WEIGHT_SCALE_FEATURE_WEIGHT_RESOLUTION_BIT		3			/**< Weight measurement resolution. */
#define WEIGHT_SCALE_FEATURE_HEIGHT_RESOLUTION_BIT		7			/**< Height measurement resolution. */
/** @} */


/**
 * @defgroup WEIGHT_SCALE_FEATURE_MASK WEIGHT_SCALE_FEATURE_MASK
 * @brief Mask of Weight Scale Feature.
 * @{ */
#define WEIGHT_SCALE_FEATURE_TIMESTAMP_MASK				(1<<0)				/**< Mask of time Stamp supported. */
#define WEIGHT_SCALE_FEATURE_MULTIPLE_USER_MASK			(1<<1)				/**< Mask of multiple user supported. */
#define WEIGHT_SCALE_FEATURE_BMI_MASK					(1<<2)				/**< Mask of BMI supported. */
#define WEIGHT_SCALE_FEATURE_WEIGHT_RESOLUTION_MASK		(0x0071)			/**< Mask of weight measurement resolution. */
#define WEIGHT_SCALE_FEATURE_HEIGHT_RESOLUTION_MASK		(0x0180)			/**< Mask of height measurement resolution. */
/** @} */


/**
 * @defgroup WEIGHT_MEASUREMENT_FLAG WEIGHT_MEASUREMENT_FLAG
 * @brief Definition of Weight Measurement Flag.
 * @{ */
#define WEIGHT_MEASUREMENT_FLAG_UNITS					(1<<0)			/**< Measurement units. */
#define WEIGHT_MEASUREMENT_FLAG_TIMESTAMP				(1<<1)			/**< Time Stamp present. */
#define WEIGHT_MEASUREMENT_FLAG_USER_ID					(1<<2)			/**< User ID present. */
#define WEIGHT_MEASUREMENT_FLAG_BMI_HEIGHT				(1<<3)			/**< BMI and height present. */
/** @} */


/**@brief Attribute index of the service table */
enum BLE_WSS_AttributeIndex
{
	INDEX_WSS_PRIMARY_SERVICE=0x00,							/**< Index of weight scale primary service. */
	INDEX_WEIGHT_SCALE_FEATURE,								/**< Index of weight scale feature characteristic. */
	INDEX_WEIGHT_SCALE_FEATURE_VALUE,						/**< Index of weight scale feature characteristic value. */
	INDEX_WEIGHT_MEASUREMENT,								/**< Index of weight measurement characteristic. */
	INDEX_WEIGHT_MEASUREMENT_VALUE,							/**< Index of weight measurement characteristic value. */
	INDEX_WEIGHT_MEASUREMENT_CCCD,							/**< Index of weight measurement client characterisitc configuration descriptor. */
	TOTAL_WSS_ATTR_NUM										/**< Total attribure number of weight scale service. */
};


/**@brief Time stamp parameters */
typedef struct Time_Field
{
	u16	year;								/**< Yead. */
	u8	month;								/**< Month. */
	u8	day;								/**< Day. */
	u8	hours;								/**< Hours. */
	u8	minutes;							/**< Minutes. */
	u8	seconds;							/**< Seconds. */
}Time_Field;


/**@brief Weight measurement parameters */
typedef struct BLE_WSS_Measurement
{
	bool	measurementUnits;							/**< Measurement units. */
	bool	isTimeStampSupported;						/**< Time Stamp present. */
	bool	isMultipleUserSupported;					/**< User ID present. */
	bool	isBmiSupported;								/**< BMI and height present. */
	u16	weight;											/**< Weight-SI/ Weight-Imperial. */
	Time_Field	timeStamp;								/**< Time Stamp. */
	u8	userID;											/**< User ID. */
	u16	bmi;											/**< BMI. */
	u16	height;											/**< Height-SI/ Height-Imperial. */
}BLE_WSS_Measurement;


/**@brief BLE_WEIGHT_SCALE_SERVICE callback function prototype. */
typedef void(*WSSProcessRoutine)(u8 eventType) large;


/**@brief Initizlize BLE Weight Scale callback function.
 *
 * @param[in] wssHandler		MBIoT callback function for BLE Weight Scale events. 
 */
PUBLIC extern void BLE_WSS_Init(WSSProcessRoutine wssHandler) large; 


/**@brief Initizlize BLE Weight Scale service table.
 *
 * @return MBIOT_RES_SUCCESS	Successfully initialize BLE Weight Scale service table.
 * @return MBIOT_RES_FAIL		Operation is not permitted.
 */
PUBLIC extern u16 BLE_WSS_Init_Service() large;


/**@brief Catch BLE_GATT events to parse the weight scale events.
 *
 * @param[in] p_event		Pointer to BLE_GATT_Event structure.
 */
PUBLIC extern void BLE_WSS_Catch_Event(BLE_GATT_Event XDATA * p_event) large;


/**@brief Set time stamp option.
 *
 * @param[in] isTimeStampSupported		If time stamp field is supported, set this value to be true. Default time stamp field is unsupported.
 *
 * @return MBIOT_RES_SUCCESS		Successfully set time stamp option.
 * @return MBIOT_RES_FAIL			Operation is not permitted.
 * @return MBIOT_RES_INVALID_PARA	Invalid parameters.
 */
PUBLIC extern u16 BLE_WSS_Set_Time_Stamp(bool isTimeStampSupported) large;


/**@brief Set multiple user option.
 *
 * @param[in] isMultipleUserSupported		If multiple user field is supported, set this value to be true. Default multiple user field is unsupported.
 *
 * @return MBIOT_RES_SUCCESS		Successfully set multiple user option.
 * @return MBIOT_RES_FAIL			Operation is not permitted.
 * @return MBIOT_RES_INVALID_PARA	Invalid parameters.
 */
PUBLIC extern u16 BLE_WSS_Set_Users(bool isMultipleUserSupported) large;


/**@brief Set BMI option.
 *
 * @param[in] isBmiSupported	If BMI field is supported, set this value to be true. Default BMI field is unsupported
 *
 * @return MBIOT_RES_SUCCESS		Successfully set BMI option.
 * @return MBIOT_RES_FAIL			Operation is not permitted.
 * @return MBIOT_RES_INVALID_PARA	Invalid parameters.
 */
PUBLIC extern u16 BLE_WSS_Set_BMI(bool isBmiSupported)large;


/**@brief Set weight measurement resolution.
 *
 * @param[in] weightResolution		Weight measurement resolution. 
 *
 * @return MBIOT_RES_SUCCESS		Successfully set weight measurement resolution.
 * @return MBIOT_RES_FAIL			Operation is not permitted.
 * @return MBIOT_RES_INVALID_PARA	Invalid parameters.
 */
PUBLIC extern u16 BLE_WSS_Set_Weight_Resolution(u8 weightResolution) large;


/**@brief Set height measurement resolution.
 *
 * @param[in] heightResolution		Height measurement resolution. 
 *
 * @return MBIOT_RES_SUCCESS		Successfully set height measurement resolution.
 * @return MBIOT_RES_FAIL			Operation is not permitted.
 * @return MBIOT_RES_INVALID_PARA	Invalid parameters.
 */
PUBLIC extern u16 BLE_WSS_Set_Height_Resolution(u8 heightResolution) large;


/**@brief Send weight measurement.
 *
 * @param[in] connHandle 		Connection handle.
 * @param[in] p_measurement		Pointer to BLE_WSS_Measurement structure. 
 *
 * @return MBIOT_RES_SUCCESS		Send weight measurement successfully.
 * @return MBIOT_RES_FAIL			Operation is not permitted.
 * @return MBIOT_RES_INVALID_PARA	Invalid parameters.
 */
PUBLIC extern u16 BLE_WSS_Send_Weight_Measurement(u8 connHandle, BLE_WSS_Measurement XDATA *p_measurement) large;


#endif

/**
  @}
 */