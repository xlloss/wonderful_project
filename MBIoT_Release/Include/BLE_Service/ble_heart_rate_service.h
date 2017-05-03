/********************************************************************
  File Information:
    FileName:       ble_heart_rate_service.h
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
  @addtogroup BLE_HRS Ble Heart Rate Service
  @ingroup BLE_Service BLE Service
  @{
  @brief Ble Heart Rate Service
  @note Definitions and prototypes for the Ble Heart Rate Service application programming interface.
 */
 


#ifndef BLE_HEART_RATE_SERVICE_H
#define BLE_HEART_RATE_SERVICE_H


/**@brief heart rate service UUID definition */
#define UUID_HEART_RATE_SERVICE				      0x180D
#define UUID_HEART_RATE_MEASUREMENT				  0x2A37
#define UUID_BODY_SENSOR_LOCATION           0x2A38
#define UUID_HEART_RATE_CONTROL_POINT       0x2A39

#define MAX_HEART_RATE_MEASUREMENT_LEN				 15

#define HEART_RATE_NOTIFICATION_DISABLED	      0x0000			/**< Heart Rate value notification disabled event. */
#define HEART_RATE_NOTIFICATION_ENABLED		      0x0100			/**< Heart Rate value notification enabled event. */
#define HEART_RATE_CONTROL_POINT_WRITE			  0x0001			/**< Heart Rate Control point write event. */

/**@brief heart rate meaurement value format*/
#define HR_Value_8BIT                 0
#define HR_Value_16BIT						   	1

/**@brief Body Sensor Location values*/
enum BLE_HRS_Body_Sensor_Location
{
	BLE_HRS_BODY_SENSOR_LOCATION_OTHER=0x00,
	BLE_HRS_BODY_SENSOR_LOCATION_CHEST,
	BLE_HRS_BODY_SENSOR_LOCATION_WRIST,
	BLE_HRS_BODY_SENSOR_LOCATION_FINGER,
	BLE_HRS_BODY_SENSOR_LOCATION_HAND,
	BLE_HRS_BODY_SENSOR_LOCATION_EAR_LOBE,
	BLE_HRS_BODY_SENSOR_LOCATION_FOOT
};

#define BLE_HRS_MAX_BUFFERED_RR_INTERVALS       5      /**< Size of RR Interval buffer inside service. More than this exceeds the MTU */

/**@brief Heart Rate Measurement flags  */
#define HEART_RATE_MEASUREMENT_FLAG_VALUE_FORMAT			(1<<0)
#define HEART_RATE_MEASUREMENT_FLAG_SENSOR_SUPPORTED	(1<<1)
#define HEART_RATE_MEASUREMENT_FLAG_SENSOR_DETECTED		(1<<2)
#define HEART_RATE_MEASUREMENT_FLAG_ENERGY_EXPENDED		(1<<3)
#define HEART_RATE_MEASUREMENT_FLAG_RR_INTERVAL				(1<<4)

/**@brief Attribute number in the app defined Service Table */
enum BLE_HRS_AttributeIndex
{
	INDEX_HRS_PRIMARY_SERVICE=0x00,
	INDEX_HEART_RATE_MEASUREMENT,
	INDEX_HEART_RATE_MEASUREMENT_VALUE,
	INDEX_HEART_RATE_MEASUREMENT_CCCD,
	INDEX_BODY_SENSOR_LOCATION,
	INDEX_BODY_SENSOR_LOCATION_VALUE,
	INDEX_HEART_RATE_CONTROL_POINT,
	INDEX_HEART_RATE_CONTROL_POINT_VALUE,
	TOTAL_HRS_ATTR_NUM
};

typedef struct BLE_HRS_Body_Sensor_location
{
	bool	isSensorContactSupported;	/**< Determines if sensor contact detection is to be supported. */
  u8 		body_sensor_location;                           
}BLE_HRS_Body_Sensor_location;

/**@brief ANS Events */
typedef struct BLE_HRS_Event
{
	u16 eventType;					/**< HRS event type. */
	u16	write_value;		/**< Event type (CCCD). */
}BLE_HRS_Event;


/**@brief Heart Rate measurement parameters */
typedef struct BLE_HRS_Measurement
{
  bool isHeartRateValueFormatsupportedBit;
  bool isSensorContactSupportedBit;
  bool isSensorContactDetectedBit;
  bool isEnergyExpendedStatusBit;
  bool isRRIntervalBit;
  u16 heart_rate;
	u16 energy_expended;
//  u16 rr_interval;
  u16	rr_interval[BLE_HRS_MAX_BUFFERED_RR_INTERVALS];       /**< Set of RR Interval measurements since the last Heart Rate Measurement transmission. */
  u8	rr_interval_count;                                    /**< Number of RR Interval measurements since the last Heart Rate Measurement transmission. */
}BLE_HRS_Measurement;

typedef void(*HRSProcessRoutine)(BLE_HRS_Event XDATA *p_hrsEvent) large;

/**@brief Initizlize BLE heart rate service.
 *
 * @param[in] hrsHandler		MBIoT callback function for BLE Heart rate events. 
 *
 * @return @ref MBIOT_RES_SUCCESS		Initialize BLE Heart rate  successfully.
 */
PUBLIC extern u16 BLE_HRS_Init(HRSProcessRoutine hrsHandler) large;

/**@brief Initizlize BLE herat rate service table.
 *
 * @return @ref MBIOT_RES_SUCCESS		Initialize BLE heart rate service table successfully.
 */
PUBLIC extern u16 BLE_HRS_Init_Service() large;

/**@brief Catch BLE_GATT events to get the heart rate service events.
 *
 * @param[in] p_event		Pointer to BLE_GATT_Event structure.
 *
 * @return @ref MBIOT_RES_SUCCESS		Catch BLE GATT events configuration successfully.
 */
PUBLIC extern void BLE_HRS_Catch_Event(BLE_GATT_Event XDATA * p_event) large;

/**@brief Send write response.
 *
 * @param[in] connHandle Connection handle.
 * @param[in] respType write response type.
 *
 * @return @ref MBIOT_RES_SUCCESS sent error response successfully.
 */
PUBLIC u16 BLE_HRS_Send_Write_Response(u8 connHandle, u8 respType);

/**@brief Send error response.
 *
 * @param[in] connHandle Connection handle.
 * @param[in] errCode unsupported command.
 *
 * @return @ref MBIOT_RES_SUCCESS sent error response successfully.
 */
PUBLIC extern u16 BLE_HRS_Send_Error_Response(u8 connHandle, u8 errCode);


/**@brief Send weight measurement.
 *
 * @param[in] p_measurement		Pointer to BLE_HRS_Measurement structure. 
 *
 * @return @ref MBIOT_RES_SUCCESS		Send heart rate measurement successfully.
 */
//PUBLIC extern u16  BLE_HRS_Send_Measurement(u8 connHandle, BLE_HRS_Measurement XDATA * p_measurement) large;
PUBLIC extern u16  BLE_HRS_Send_Measurement(u8 connHandle) large;


/**@brief Set the Body Sensor Location.
 *
 * @details Sets a new value of the Body Sensor Location characteristic. The new value will be sent
 *          to the client the next time the client reads the Body Sensor Location characteristic.
 *
 * @param[in]   body_sensor_location  New Body Sensor Location.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
PUBLIC extern u16 BLE_HRS_Body_Sensor_Location_Set(u8 body_sensor_location) large;

#endif // BLE_HRS_H__

/**
  @}
 */