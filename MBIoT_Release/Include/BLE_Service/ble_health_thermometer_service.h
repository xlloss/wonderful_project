/********************************************************************
  File Information:
    FileName:       ble_health_thermometer_service.h
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
  @addtogroup BLE_HTS Ble Health Thermometer Service
  @ingroup BLE_Service BLE Service
  @{
  @brief Ble Health Thermometer Service
  @note Definitions and prototypes for the Ble Health Thermometer Service application programming interface.
 */
 


#ifndef BLE_HEALTH_THERMOMETER_SERVICE_H
#define BLE_HEALTH_THERMOMETER_SERVICE_H

/**@brief Height measure resolution */
#define HEIGHT_RESOLUTION_001METER_OR_1INCH		0x01		/**< Resolution of 0.01 meter or 1 inch. */
#define HEIGHT_RESOLUTION_005METER_OR_05INCH	0x02		/**< Resolution of 0.005 meter or 0.5 inch. */
#define HEIGHT_RESOLUTION_0001METER_OR_01INCH	0x03		/**< Resolution of 0.001 meter or 0.1 inch. */


/**@brief For CCCD handle */

#define HEALTH_THERMOMETER_INDICATE_DISABLED	0x0000			/**< HEALTH_THERMOMETER temperature measurement indication is disabled. */
#define HEALTH_THERMOMETER_INDICATE_ENABLED		0x0200			/**< HEALTH_THERMOMETER temperature  measurement indication is enabled. */

#define HEALTH_THERMOMETER_NOTIFY_DISABLED					0x0000 /**< Notifications are disabled */
#define HEALTH_THERMOMETER_NOTIFY_ENABLED					0x0100 /**<  Notifications are enabled */

#define MAX_HEALTH_THERMOMETER_MEASUREMENT_LEN 15

/** @} */

/**@brief HEALTH THERMOMETER  The Measurement Interval characteristic defines the time between measurements*/

#define HEALTH_THERMOMETER_MAX_MEASUREMENT_INTERVAL     41000 //**measurement Interval unit is in Seconds*/
#define  HEALTH_THERMOMETER_MEASUREMENT_NO_INTERVAL     0

/**@brief HEALTH THERMOMETER  valid range minimum and maximum  in seconds*/
#define HEALTH_THERMOMETER_MAX_RANGE  0xBB80  //Defined by Bluetooth Spec in 5.2 PARAMETER DEFINITIONS section
#define HEALTH_THERMOMETER_MIN_RANGE  0x0640   //Defined by Bluetooth Spec
#define OUT_OF_RANGE 0x80




/**@brief HEALTH THERMOMETER SERVICE UUID definition */
#define UUID_HEALTH_THERMOMETER_SERVICE				                 0x1809         //Definition of health thermometer service UUID
#define UUID_HEALTH_THERMOMETER_TEMPERATURE_MEASUREMENT			     0x2A1C			//Definition of  temperature measurement UUID
#define UUID_HEALTH_THERMOMETER_TEMPERATURE_TYPE                     0x2A1D			//Definition of temperature type UUID
#define UUID_HEALTH_THERMOMETER_INTERMMEDIATE_TEMPERATURE		     0x2A1E			//Definition of Intermediate temperature UUID
#define UUID_HEALTH_THERMOMETER_TEMPERATURE_INTERVAL                 0x2A21			//Definition of temperature interval
#define UUID_DESCRIPTOR_VALID_RANGE									 0x2906			//Definition of Characteristic Valid Range descriptor UUID 


/**@brief HEALTH THERMOMETER Temperature measurement flags  */
#define HEALTH_THERMOMETER_TEMPERATURE_MEASUREMENT_FLAG_TEMPERATURE_UNIT					        (1<<0)
#define HEALTH_THERMOMETER_TEMPERATURE_MEASUREMENT_FLAG_TIMESTAMP       							(1<<1)
#define HEALTH_THERMOMETER_TEMPERATURE_MEASUREMENT_FLAG_TEMPERATURE_TYPE				   			(1<<2)

// Temperature Type measurement, Temperature Text Description

#define BLE_HTMS_TEMPERATURE_TYPE_ARMPIT      1
#define BLE_HTMS_TEMPERATURE_TYPE_BODY        2
#define BLE_HTMS_TEMPERATURE_TYPE_EAR         3
#define BLE_HTMS_TEMPERATURE_TYPE_FINGER      4
#define BLE_HTMS_TEMPERATURE_TYPE_GI_TRACT    5
#define BLE_HTMS_TEMPERATURE_TYPE_MOUTH       6
#define BLE_HTMS_TEMPERATURE_TYPE_RECTUM      7
#define BLE_HTMS_TEMPERATURE_TYPE_TOE         8
#define BLE_HTMS_TEMPERATURE_TYPE_EAR_DRUM    9

/**@brief Attribute index of the service table */
enum BLE_HTMS_AttributeIndex
{
	INDEX_HTM_PRIMARY_SERVICE=0x00,							      /**< Index of health thermometer  primary service. */
	INDEX_HEALTH_THERMOMETER_TEMPERATURE_MEASUREMENT,								
	INDEX_HEALTH_THERMOMETER_TEMPERATURE_MEASUREMENT_VALUE,						
	INDEX_HEALTH_THERMOMETER_TEMPERATURE_MEASUREMENT_CCCD,
	INDEX_HEALTH_THERMOMETER_TEMPERATURE_TYPE,								
	INDEX_HEALTH_THERMOMETER_TEMPERATURE_TYPE_VALUE,							
	INDEX_HEALTH_THERMOMETER_INTERMMEDIATE_TEMPERATURE,							
	INDEX_HEALTH_THERMOMETER_INTERMMEDIATE_TEMPERATURE_VALUE,	
	INDEX_HEALTH_THERMOMETER_INTERMMEDIATE_TEMPERATURE_CCCD,	
	INDEX_HEALTH_THERMOMETER_TEMPERATURE_INTERVAL,	
	INDEX_HEALTH_THERMOMETER_TEMPERATURE_INTERVAL_VALUE,	
	INDEX_HEALTH_THERMOMETER_TEMPERATURE_INTERVAL_CCCD,	
	INDEX_HEALTH_THERMOMETER_TEMPERATURE_INTERVAL_VALID_RANGE,	
	TOTAL_HTMS_ATTR_NUM										/**< Total attribure number of health thermometer  service. */
};


PUBLIC extern u16 XDATA  temperatureMeasurementHandle;
PUBLIC extern u16 XDATA  temperatureMeasurementHandleCCCDHandle;
PUBLIC extern u16 XDATA  temperatureTypeHandle;
PUBLIC extern u16 XDATA  intermediateTemperatureHandle;
PUBLIC extern u16 XDATA  intermediateTemperatureCCCDHandle;
PUBLIC extern u16 XDATA  measurementIntervalHandle;
PUBLIC extern u16 XDATA  measurementIntervalCCDHandle;
PUBLIC extern u16 XDATA  measurementIntervalValidRangeHandle;
PUBLIC extern XDATA u16 MeasurementInterval;                      /**< Temperature Measurement Interval. */


/**@brief Time stamp parameters */

typedef struct Time_Field
{
	u16	year;
	u8	month;
	u8	day;
	u8	hours;	
	u8	minutes;
	u8	seconds;
}Time_Field;

/**@brief HTS Events parameter  */

typedef struct BLE_HTMS_Event
{
	u16 handle;					/**< CCCD Handle. */
	u16	write_value;		/**< Event type (CCCD). */
}BLE_HTMS_Event;	

/**@brief Temperature measurement parameters */
typedef struct BLE_HTMS_Measurement
{
    bool IsTemperatureUnitAvailable;
	bool IsTimestampAvailable;
	bool IsTemperatureTypeAvailable;
	float temperatureMeasurementValue;
	Time_Field timeStamp;
	u8 temperatureType;
}BLE_HTMS_Measurement;

//Intermediate Temperature
typedef struct BLE_HTMS_Temp_Interval
{
    bool IsTemperatureUnitAvailable;
	bool IsTimestampAvailable;
	bool IsTemperatureTypeAvailable;
	float temperatureMeasurementValue;
	Time_Field timeStamp;
	u8 temperatureType;
}BLE_HTMS_Temp_Interval;


typedef void(*HTMSProcessRoutine)(BLE_HTMS_Event XDATA *p_htmsEvent) large;


/**@brief Initialize BLE health thermometer .
 *
 * @param[in] smpHandler		MBIoT callback function for HTS  events. 
 *
 * @return @ref MBIOT_RES_SUCCESS		Initialize BLE health thermometer  successfully.
 */
PUBLIC extern void BLE_HTMS_Init(HTMSProcessRoutine htmsHandler) large; 


/**@brief Initialize BLE health thermometer  service table.
 *
 * @return @ref MBIOT_RES_SUCCESS		Initialize BLE health thermometer  service table successfully.
 */
PUBLIC extern u16 BLE_HTMS_Init_Service() large;


/**@brief Catch BLE_GATT events to get the health thermometer  events.
 *
 * @param[in] p_event		Pointer to BLE_GATT_Event structure.
 *
 * @return @ref MBIOT_RES_SUCCESS		Catch BLE GATT events configuration successfully.
 */
PUBLIC extern void BLE_HTMS_Catch_Event(BLE_GATT_Event XDATA * p_event) large;


/**@brief Set TemperatureType user option.
 *
 * @param[in] typeDescription	 Temperature Text Description 
 *
 * @return @ref MBIOT_RES_SUCCESS		Set typeDescription  option successfully.
 */
PUBLIC extern u16 BLE_HTMS_Set_TemperatureType(u8 typeDescription) large;

/**@brief Set Minimum Range user option.
 *
 * @param[in] MinRange		set MinRange Lower inclusive value . 
 *
 * @return @ref MBIOT_RES_SUCCESS		Set MinRange  option successfully.
 */
PUBLIC extern u16 BLE_HTMS_Set_Min_Valid_Range(u16 MinRange) large;
/**@brief Set Maximum Range user option.
 *
 * @param[in] MaxRange		set MaxRange Upper inclusive value . 
 *
 * @return @ref MBIOT_RES_SUCCESS		Set MaxRange  option successfully.
 */

PUBLIC extern u16 BLE_HTMS_Set_Max_Valid_Range(u16 MaxRange) large;


/**@brief  Temperature Measurement Interval user option.
 *
 * @param[in] measurementInterval		set Measurement Interval. 
 *
 * @return @ref MBIOT_RES_SUCCESS		Set measurementInterval  option successfully.
 */
PUBLIC extern u16 BLE_HTMS_Set_Temperature_Interval(u16 measurementInterval) large;

/**@brief Send Temperature measurement.
 *
 * @param[in] connHandle		Connection handle.
 * @param[in] p_intermediateTemperature 	Pointer to BLE_HTMS_Measurement structure. 
 *
 * @return MBIOT_RES_SUCCESS		Send Temperature measurement successfully.
 * @return MBIOT_RES_FAIL			Operation is not permitted.
 * @return MBIOT_RES_INVALID_PARA	Invalid parameters.
 */
PUBLIC extern u16 BLE_HTMS_Send_Temperature_Measurement(u8 connHandle, BLE_HTMS_Measurement XDATA *p_measurement) large;

/**@brief Send Intermediate Temperature.
 *
 * @param[in] connHandle		Connection handle.
 * @param[in] p_intermediateTemperature 	Pointer to BLE_HTMS_Measurement structure. 
 *
 * @return MBIOT_RES_SUCCESS		Send Intermediate Temperature successfully.
 * @return MBIOT_RES_FAIL			Operation is not permitted.
 * @return MBIOT_RES_INVALID_PARA	Invalid parameters.
 */


PUBLIC extern u16 BLE_HTMS_Send_Intermediate_Temperature(u8 connHandle, BLE_HTMS_Measurement XDATA *p_intermediateTemperature) large;


/**@brief Send Temperature Meaurement Interval.
 *
 * @param[in] connHandle		Connection handle.
 * @param[in] measurementInterval 	Measurement Interval . 
 *
 * @return MBIOT_RES_SUCCESS		Send Intermediate Temperature successfully.
 * @return MBIOT_RES_FAIL			Operation is not permitted.
 * @return MBIOT_RES_INVALID_PARA	Invalid parameters.
 */
PUBLIC extern u16 BLE_HTMS_Send_Temperature_Interval(u8 connHandle,u16 measurementInterval) large;

#endif

/**
  @}
 */