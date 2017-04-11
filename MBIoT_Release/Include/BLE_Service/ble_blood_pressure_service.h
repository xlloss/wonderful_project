/********************************************************************
  File Information:
    FileName:       ble_blood_pressure_service.h
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
  @addtogroup BLE_BPS Ble Blood Pressure Service
  @ingroup BLE_Service BLE Service
  @{
  @brief Ble Blood Pressure Service
  @note Definitions and prototypes for the Ble Blood Pressure Service application programming interface.
 */


#ifndef BLE_BLOOD_PRESSURE_SERVICE_H
#define BLE_BLOOD_PRESSURE_SERVICE_H

/*
// Blood Pressure Feature bits
 #define BLE_BPS_FEATURE_BODY_MOVEMENT_BIT               (0x01 << 0)         
 #define BLE_BPS_FEATURE_CUFF_FIT_BIT                    (0x01 << 1)         
 #define BLE_BPS_FEATURE_IRREGULAR_PULSE_BIT             (0x01 << 2)         
 #define BLE_BPS_FEATURE_PULSE_RATE_RANGE_BIT            (0x01 << 3)         
 #define BLE_BPS_FEATURE_MEASUREMENT_POSITION_BIT        (0x01 << 4)         
 #define BLE_BPS_FEATURE_MULTIPLE_BOND_BIT               (0x01 << 5) */

#define BLOOD_PRESSURE_UNIT_MMHG                      0
#define BLOOD_PRESSURE_UNIT_KPA                       1


#define BPS_MEASUREMENT_INDICATE_DISABLED							0x0000			/**< BPS measurement indication is disabled. */
#define BPS_MEASUREMENT_INDICATE_ENABLED							0x0200			/**< BPS measurement indication is enabled. */
#define BPS_INTER_CUTOFF_NOTIFY_ENABLED								0x0100			/**< BPS intermediate cutoff notification is enabled. */

#define BPS_MEASUREMENT_LEN											15

/**@brief Blood Pressure service UUID definition */
#define UUID_BLOOD_PRESSURE_SERVICE				0x1810
#define UUID_BLOOD_PRESSURE_MEASUREMENT		    0x2A35
#define UUID_INTERMEDIATE_CUFF_PRESSURE			0x2A36
#define UUID_BLOOD_PRESSURE_FEATURE             0x2A49

/**@brief Bit position of blood pressure feature  */
#define BP_FEATURE_BODY_MOVEMENT_DETECT_BIT				0
#define BP_FEATURE_COFF_FIT_DETECT_BIT					1
#define BP_FEATURE_IRREGULAR_PULSE_DETECT_BIT			2
#define BP_FEATURE_PULSE_RATE_RANGE_DETECT_BIT			3
#define BP_FEATURE_MEASUREMENT_POSITION_DETECT_BIT		4
#define BP_FEATURE_MULTIPLE_BOND_BIT					5

/**@brief Mask of blood pressure feature  */
#define BP_FEATURE_BODY_MOVEMENT_DETECT_MASK				(1<<0)
#define BP_FEATURE_COFF_FIT_DETECT_MASK							(1<<1)
#define BP_FEATURE_IRREGULAR_PULSE_DETECT_MASK			(1<<2)
#define BP_FEATURE_PULSE_RATE_RANGE_DETECT_MASK			(1<<3)
#define BP_FEATURE_MEASUREMENT_POSITION_DETECT_MASK	(1<<4)
#define BP_FEATURE_MULTIPLE_BOND_MASK								(1<<5)

/**@brief Mask of Blood Pressure measurement flags  */
#define BP_MEASUREMENT_FLAG_UNITS					(1<<0)
#define BP_MEASUREMENT_FLAG_TIMESTAMP			(1<<1)
#define BP_MEASUREMENT_FLAG_PULSE_RATE		(1<<2)
#define BP_MEASUREMENT_FLAG_USER_ID				(1<<3)
#define BP_MEASUREMENT_FLAG_STATUS				(1<<4)

/**@brief Attribute number in the app defined Service Table */
enum BLE_BPS_AttributeIndex
{
	INDEX_BPS_PRIMARY_SERVICE=0x00,
	INDEX_BLOOD_PRESSURE_MEASUREMENT,
	INDEX_BLOOD_PRESSURE_MEASUREMENT_VALUE,
	INDEX_BLOOD_PRESSURE_MEASUREMENT_CCCD,
	INDEX_INTERMEDIATE_CUFF_PRESSURE,
	INDEX_INTERMEDIATE_CUFF_PRESSURE_VALUE,
	INDEX_INTERMEDIATE_CUFF_PRESSURE_CCCD,
	INDEX_BLOOD_PRESSURE_FEATURE,
	INDEX_BLOOD_PRESSURE_FEATURE_VALUE,
	TOTAL_BPS_ATTR_NUM
};

typedef struct ieee_float
{
  u8 exponent;
  u16 mantissa; 
}ieee_float;

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
 
typedef struct BLE_BPS_Measurement
{
	bool            blood_pressure_units_in_kpa;               
	bool            time_stamp_present;                        
	bool            pulse_rate_present;                        
	bool            user_id_present;                           
	bool            measurement_status_present;
	ieee_float			blood_pressure_systolic;                   
	ieee_float      blood_pressure_diastolic;                  
	ieee_float      mean_arterial_pressure;                    
	Time_Field		  time_stamp;                                
	ieee_float			pulse_rate;                                
	u8              user_id;                                   
	u16							measurement_status; 
}BLE_BPS_Measurement;

typedef struct BLE_BPS_Cuff_Pressure
{
	bool            blood_pressure_units_in_kpa;               
	bool            time_stamp_present;                        
	bool            pulse_rate_present;                        
	bool            user_id_present;                           
	bool            measurement_status_present;
	ieee_float			blood_pressure_systolic;                   
	ieee_float      blood_pressure_diastolic;                  
	ieee_float      mean_arterial_pressure;                    
	Time_Field		  time_stamp;                                
	ieee_float			pulse_rate;                                
	u8              user_id;                                   
	u16							measurement_status; 
}BLE_BPS_Cuff_Pressure;
 
typedef void(*BPSProcessRoutine)(u16 eventType) large;

 /**@brief Initizlize BLE Blood Pressure service.
 *
 * @param[in] bpsHandler		MBIoT callback function for BLE Blood Pressure events. 
 *
 * @return @ref MBIOT_RES_SUCCESS		Initialize BLE Blood Pressurev successfully.
 */
PUBLIC extern void BLE_BPS_Init(BPSProcessRoutine bpsHandler) large;

/**@brief Initizlize BLE Blood Pressure service table.
 *
 * @return @ref MBIOT_RES_SUCCESS		Initialize BLE Blood Pressure service table successfully.
 */
PUBLIC extern u16 BLE_BPS_Init_Service() large;


/**@brief Catch BLE_GATT events to get the Blood Pressure events.
 *
 * @param[in] p_event		Pointer to BLE_GATT_Event structure.
 *
 * @return @ref MBIOT_RES_SUCCESS		Catch BLE GATT events configuration successfully.
 */
PUBLIC extern void BLE_BPS_Catch_Event(BLE_GATT_Event XDATA * p_event) large;

 /**@brief Set BPS Body Movement Detection Support option.
 *
 * @param[in] isBodyMovementDetectionSupportbit		If Body Movement Detection field is supported, set this value to be true. Default Body Movement Detection field is unsupported.
 *
 * @return @ref MBIOT_RES_SUCCESS		Set Body Movement Detection option successfully.
 */
PUBLIC extern u16 BLE_BPS_Body_Movement_Detection_Support(bool isBodyMovementDetectionSupportbit) large;

/**@brief Set BPS Cuff Fit Detection Support option.
 *
 * @param[in] isCuffFitDetectionSupportbit		If Cuff Fit Detection Support field is supported, set this value to be true. Default Cuff Fit Detection Support field is unsupported.
 *
 * @return @ref MBIOT_RES_SUCCESS		Set Cuff Fit Detection Support option successfully.
 */
PUBLIC extern u16 BLE_BPS_Cuff_Fit_Detection_Support(bool isCuffFitDetectionSupportbit) large;

/**@brief Set BPS Irregular Pulse Detection Support Option.
 *
 * @param[in] isIrregularPulseDetectionSupportbit		If BPS Irregular Pulse Detection is supported, set this value to be true. Default BPS Irregular Pulse Detection Support field is unsupported.
 *
 * @return @ref MBIOT_RES_SUCCESS		Set BPS Irregular Pulse Detection option successfully.
 */
PUBLIC extern u16 BLE_BPS_Irregular_Pulse_Detection_Support(bool isIrregularPulseDetectionSupportbit) large;


/**@brief Set BLE_BPS_Pulse_Rate_Range_Detection_Support option.
 *
 * @param[in] isPulseRateRangeDetectionSupportbit		If BLE_BPS_Pulse_Rate_Range_Detection field is supported, set this value to be true. Default BLE_BPS_Pulse_Rate_Range_Detection field is unsupported.
 *
 * @return @ref MBIOT_RES_SUCCESS		Set BLE_BPS_Pulse_Rate_Range_Detection option successfully.
 */
PUBLIC extern u16 BLE_BPS_Pulse_Rate_Range_Detection_Support(bool isPulseRateRangeDetectionSupportbit) large;

/**@brief Set BLE_BPS_Measurement_Position_Detection_Support option.
 *
 * @param[in] isMeasurementPositionDetectionSupportbit		If BLE_BPS_Measurement_Position_Detection field is supported, set this value to be true. Default BLE_BPS_Measurement_Position_Detection field is unsupported.
 *
 * @return @ref MBIOT_RES_SUCCESS		Set BLE BPS Measurement Position Detection option successfully.
 */
PUBLIC extern u16 BLE_BPS_Measurement_Position_Detection_Support(bool isMeasurementPositionDetectionSupportbit) large;


/**@brief Set BPS_Multiple_Bond_Support option.
 *
 * @param[in] isMultipleBondSupportbit		If BPS_Multiple_Bond field is supported, set this value to be true. Default BPS_Multiple_Bond field is unsupported
 *
 * @return @ref MBIOT_RES_SUCCESS		Set BPS_Multiple_Bond option successfully.
 */
PUBLIC u16 BLE_BPS_Multiple_Bond_Support(bool isMultipleBondSupportbit) large;


/**@brief Send BPS measurement.
 *
 * @param[in] p_measurement		Pointer to BLE_BPS_Measurement structure. 
 *
 * @return @ref MBIOT_RES_SUCCESS		Send BPS measurement successfully.
 */
PUBLIC extern u16 BLE_BPS_Send_Measurement(u8 connHandle, BLE_BPS_Measurement XDATA *p_measurement) large;

PUBLIC extern u16 BLE_BPS_Send_Inter_cuff(u8 connHandle, BLE_BPS_Cuff_Pressure XDATA *p_CuffPressure) large;


#endif

/**
  @}
 */
