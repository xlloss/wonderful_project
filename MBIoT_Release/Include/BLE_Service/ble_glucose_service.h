/********************************************************************
  File Information:
    FileName:       ble_glucose_scale_service.h
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
  @addtogroup BLE_GS Ble Glucose Service
  @ingroup BLE_Service BLE Service
  @{
  @brief Ble Glucose Service
  @note Definitions and prototypes for the Ble Glucose Service application programming interface.
 */


#ifndef BLE_GLUCOSE_SCALE_SERVICE_H
#define BLE_GLUCOSE_SCALE_SERVICE_H



/**@brief Glucose service UUID definition */
#define UUID_GLUCOSE_SERVICE						0x1808
#define UUID_GLUCOSE_MEASUREMENT					0x2A18
#define UUID_GLUCOSE_MEASUREMENT_CONTEXT    		0x2A34
#define UUID_GLUCOSE_Feature                		0x2A51
#define UUID_GLUCOSE_RECORD_ACCESS_CONTROL_POINT 	0x2A52

///**@brief Glucose measure type */
//Type 
#define GLUCOSE_CAPILLARY_WHOLE_BLOOD 		0x01
#define GLUCOSE_Capillary_Plasma      		0x02
#define GLUCOSE_VENOUS_WHOLE_BLOOD    		0x03
#define GLUCOSE_VENOUS_PLASMA         		0x04
#define GLUCOSE_ARTERIAL_WHOLE_BLOOD  		0x05
#define GLUCOSE_ARTERIAL_PLASMA       		0x06
#define GLUCOSE_UNDETERMINED_WHOLE_BLOOD 	0x07
#define GLUCOSE_UNDETERMINED_PLASMA         0x08
#define GLUCOSE_INTERSTITIAL_FLUID       	0x09
#define GLUCOSE_CONTROL_SOLUTION         	0x0A


//Glucose Measurement
#define GLUCOSE_MEASUREMENT_NOTIFY_ENABLED  	0x0100
#define GLUCOSE_MEASUREMENT_NOTIFY_DISABLED 	0x0000
#define GLUCOSE_MEASUREMENT_INDICATE_ENABLED 	0x0200
#define GLUCOSE_MEASUREMENT_INDICATE_DISABLED 	0x0000

#define MAX_GLUCOSE_MEASUREMENT_LEN				30
#define MAX_GLUCOSE_MEASUREMENT_CONTEXT_LEN				30



/**@brief Attribute number in the app defined Service Table */
#define APP_DEFINED_GLUCOSE_ATTRIBUTE_NUMBER		0x0C

/**@brief glucose Concentration unit */
#define GLUCOSE_CONCENTRATION_UNIT_KGL			0			/**Glucose Concentration Units	 0	kg/L	 */
#define GLUCOSE_CONCENTRATION_UNIT_MOL			1			/**Glucose Concentration Units     1	mol/L*/


/**@brief GLUCOSE measurement flags  */
#define GLUCOSE_MEASUREMENT_FLAG_TIMEOFFSET					               (1<<0)
#define GLUCOSE_MEASUREMENT_FLAG_GLUCOSE_CONCENTRATION_TYPE_LOCATION       (1<<1)
#define GLUCOSE_MEASUREMENT_FLAG_GLUCOSE_CONCENTRATION_UNIT				   (1<<2)
#define GLUCOSE_MEASUREMENT_FLAG_SENSOR_STATUS_ANNUCIATION_PRESENT 		   (1<<3)
#define GLUCOSE_MEASUREMENT_FLAG_CONTEXT_INFORMATION			           (1<<4)

/**@brief GLUCOSE measurement Context flags  */
#define GLUCOSE_MEASUREMENTCONTEXT_FLAG_CARBOHYDRATE_ID_PRESENT			      (1<<0)
#define GLUCOSE_MEASUREMENTCONTEXT_FLAG_MEAL_PRESENT                          (1<<1)
#define GLUCOSE_MEASUREMENTCONTEXT_FLAG_TESTER_HEALTH_PRESENT			      (1<<2)
#define GLUCOSE_MEASUREMENTCONTEXT_FLAG_EXERCISE_DURATION_INTENSITY_PRESENT   (1<<3)
#define GLUCOSE_MEASUREMENTCONTEXT_FLAG_MDICATION_ID_PRESENT			      (1<<4)
#define GLUCOSE_MEASUREMENTCONTEXT_FLAG_MDICATION_VALUE_PRESENT			      (1<<5)
#define GLUCOSE_MEASUREMENTCONTEXT_FLAG_HBA1CPRESENT			              (1<<6)
#define GLUCOSE_MEASUREMENTCONTEXT_FLAG_EXTENDED_FLAG_PRESENT	              (1<<7)

 
/**@brief GLUCOSE Feature flags  */
#define GLUCOSE_LOW_BATTERY_DETECTION_DURING_MEASUREMENT_SUPPORTED	  (1<<0)
#define GLUCOSE_SENSOR_MALFUNCTION_DETECTION_SUPPORTED                (1<<1)
#define GLUCOSE_SENSOR_SAMPLE_SIZE_SUPPORTED                          (1<<2)
#define GLUCOSE_SENSOR_STRIP_INSERTION_ERROR_DETECTION_SUPPORTED      (1<<3)
#define GLUCOSE_SENSOR_STRIP_TYPE_ERROR_DETECTION_SUPPORTED           (1<<4)
#define GLUCOSE_SENSOR_RESULT_HIGH_LOW_DETECTION_SUPPORTED            (1<<5)
#define GLUCOSE_SENSOR_TEMPERATURE_HIGH_LOW_DETECTION_SUPPORTED       (1<<6)
#define GLUCOSE_SENSOR_READ_INTERRUPT_DETECTION_SUPPORTED             (1<<7)
#define GLUCOSE_GENERAL_DEVICE_FAULT_SUPPORTED                        (1<<8)
#define GLUCOSE_TIME_FAULT_SUPPORTED                                  (1<<9)
#define GLUCOSE_MULTIPLE_BOND_SUPPORTED                               (1<<10)
#define GLUCOSE_ALL_FEATURE_SUPPORTED                                  0x03FF


/**@brief GLUCOSE Sensor AStatus Annunciation flags  */
#define DEVICE_BATTERY_LOW_AT_TIME_OF_MEASUREMENT											(1<<0)
#define SENSOR_MALFUNCTION_OR_FAULTING_AT_TIME_OF_MEASUREMENT								(1<<1)
#define SAMPLE_SIZE_FOR_BLOOD_OR_CONTROL_SOLUTION_INSUFFICIENT_AT_TIME_OF_MEASUREMENT		(1<<2)	
#define STRIP_INSERTION_ERROR																(1<<3)
#define STRIP_TYPE_INCORRECT_FOR_DEVICE														(1<<4)	
#define SENSOR_RESULT_HIGHER_THAN_THE_DEVICE_CAN_PROCESS									(1<<5)
#define SENSOR_RESULT_LOWER_THAN_THE_DEVICE_CAN_PROCESS										(1<<6)
#define SENSOR_TEMPERATURE_TOO_HIGH_FOR_VALID_TEST_RESULT_AT_TIME_OF_MEASUREMENT			(1<<7)	
#define SENSOR_TEMPERATURE_TOO_LOW_FOR_VALID_TEST_RESULT_AT_TIME_OF_MEASUREMENT				(1<<8)
#define SENSOR_READ_INTERRUPTED_BECAUSE_STRIP_WAS_PULLED_TOO_SOON_AT_TIME_OF_MEASUREMENT    (1<<9)	
#define GENERAL_DEVICE_FAULT_HAS_OCCURRED_IN_THE_SENSOR										(1<<10)
#define TIME_FAULT_HAS_OCCURRED_IN_THE_SENSOR_AND_TIME_MAY_BE_INACCURATE					(1<<11)

//RECORD ACCESS CONTROL POINT OPCODE
#define GLUCOSE_REPORT_STORED_RECORDS						0x01        //REPORT STORED RECORDS (OPERATOR: VALUE FROM OPERATOR TABLE)
#define GLUCOSE_DELETE_STORED_RECORDS						0x02        //DELETE STORED RECORDS (OPERATOR: VALUE FROM OPERATOR TABLE)
#define GLUCOSE_ABORT_OPERATION								0x03        //ABORT OPERATION (OPERATOR: NULL 'VALUE OF 0X00 FROM OPERATOR TABLE')
#define GLUCOSE_REPORT_NUMBER_OF_STORED_RECORDS 			0x04 		//NUMBER OF STORED RECORDS RESPONSE (OPERATOR: NULL 'VALUE OF 0X00 FROM OPERATOR TABLE')
#define GLUCOSE_NUMBER_OF_STORED_RECORDS_RESPONSE			0x05 		//NUMBER OF STORED RECORDS RESPONSE (OPERATOR: NULL 'VALUE OF 0X00 FROM OPERATOR TABLE')
#define GLUCOSE_RESPONSE_CODE                               0x06     	//RESPONSE CODE (OPERATOR: NULL 'VALUE OF 0X00 FROM OPERATOR TABLE')


//RECORD ACCESS CONTROL POINT  OPERATOR
#define GLUCOSE_NULL						0x00
#define GLUCOSE_ALL_RECORDS                 0x01
#define GLUCOSE_LESS_THAN_OR_EQUAL_TO		0x02
#define GLUCOSE_GREATER_THAN_OR_EQUAL_TO	0x03
#define GLUCOSE_WITHIN_RRANGE_OF 			0x04                 //WITHIN RANGE OF (INCLUSIVE)
#define GLUCOSE_FIRST_RECORD				0x05                 //FIRST RECORD(I.E. OLDEST RECORD)
#define GLUCOSE_LAST_RECORD                 0x06                 //LAST RECORD (I.E. MOST RECENT RECORD)


//OP CODE / OPERAND VALUE CORRESPONDENCE

#define GLUCOSE_NA						0x00		//N/A
#define GLUCOSE_FILTER_PARAMETERS		0x01		//FILTER PARAMETERS (AS APPROPRIATE TO OPERATOR AND SERVICE)
#define GLUCOSE_FILTER_PARAMETERS2		0x02		//	FILTER PARAMETERS (AS APPROPRIATE TO OPERATOR AND SERVICE)
#define GLUCOSE_NOT_INCLUDED 			0x03		//	NOT INCLUDED
#define GLUCOSE_FILTER_PARAMETERS3		0x04		//	FILTER PARAMETERS (AS APPROPRIATE TO OPERATOR AND SERVICE)
#define GLUCOSE_NUMBER_OF_RECORDS_FIELDSIZE       0x05		//NUMBER OF RECORDS (FIELD SIZE DEFINED PER SERVICE)
#define GLUCOSE_REQUEST_OP_CODE         0x06		//	REQUEST OP CODE, RESPONSE CODE VALUE


//OPERAND INFORMATION

#define GLUCOSE_SUCCESS_NORMAL_RESPONSE					0x01	//SUCCESS	NORMAL RESPONSE FOR SUCCESSFUL OPERATION
#define GLUCOSE_OP_CODE_NOT_SUPPORTED					0x02	//OP CODE NOT SUPPORTED	NORMAL RESPONSE IF UNSUPPORTED OP CODE IS RECEIVED
#define GLUCOSE_INVALID_OPERATOR						0x03	//INVALID OPERATOR	NORMAL RESPONSE IF OPERATOR RECEIVED DOES NOT MEET THE REQUIREMENTS OF THE SERVICE (E.G. NULL WAS EXPECTED)
#define GLUCOSE_OPERATOR_NOT_SUPPORTED					0x04	//OPERATOR NOT SUPPORTED	NORMAL RESPONSE IF UNSUPPORTED OPERATOR IS RECEIVED
#define GLUCOSE_INVALID_OPERAND_NORMAL_RESPONSE			0x05	//INVALID OPERAND	NORMAL RESPONSE IF OPERAND RECEIVED DOES NOT MEET THE REQUIREMENTS OF THE SERVICE
#define GLUCOSE_NO_RCORDS_FOUND							0x06	//NO RECORDS FOUND	NORMAL RESPONSE IF REQUEST TO REPORT STORED RECORDS OR REQUEST TO DELETE STORED RECORDS RESULTED IN NO RECORDS MEETING CRITERIA.
#define GLUCOSE_ABORT_UNSUCCESSFUL						0x07	//ABORT UNSUCCESSFUL	NORMAL RESPONSE IF REQUEST FOR ABORT CANNOT BE COMPLETED
#define GLUCOSE_PROCEDURE_NOT_COMPLETED					0x08	//PROCEDURE NOT COMPLETED	NORMAL RESPONSE IF UNABLE TO COMPLETE A PROCEDURE FOR ANY REASON
#define GLUCOSE_OPERAND_NOT_SUPPORTED					0x09	//OPERAND NOT SUPPORTED	NORMAL RESPONSE IF UNSUPPORTED OPERAND IS RECEIVED


#define BLE_GLUCOSE_Database_MAX_RECORDS      20

#define BLE_GLUCOSE_COMMAND_ERROR  0x81


//SFLOAT
typedef struct S_float
{
	u8 exponent;
	u16 mantissa;
}S_float;


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

/**@brief Attribute index number in the app defined Service Table */

enum BLE_GLUCOSE_AttributeIndex
{
	INDEX_GLUCOSE_PRIMARY_SERVICE=0x00,
	INDEX_GLUCOSE_MEASUREMENT,
	INDEX_GLUCOSE_MEASUREMENT_VALUE,
	INDEX_GLUCOSE_MEASUREMENT_CCCD,
	INDEX_GLUCOSE_MEASUREMENT_CONTEXT,
	INDEX_GLUCOSE_MEASUREMENT_CONTEXT_VALUE,
	INDEX_GLUCOSE_MEASUREMENT_CONTEXT_CCCD,
	INDEX_GLUCOSE_FEATURE,
	INDEX_GLUCOSE_FEATURE_VALUE,
	INDEX_GLUCOSE_RECORD_ACCESS_CONTROL_POINT,
	INDEX_GLUCOSE_RECORD_ACCESS_CONTROL_POINT_VALUE,
	INDEX_GLUCOSE_RECORD_ACCESS_CONTROL_POINT_CCCD,
	TOTAL_GLUCOSE_ATTR_NUM
};

typedef struct BLE_GLUCOSE_Event
{
	u16 handle;
	u8 XDATA *write_value;
	u16	receivedDataLength;
}BLE_GLUCOSE_Event;

/**@brief Glucose RACP  Service communication state. */

typedef enum
{
    STATE_NO_COMM,                                                     /**< The service is not in a communicating state. */
    STATE_RACP_PROC_ACTIVE,                                            /**< Processing requested data. */
    STATE_RACP_RESPONSE_PENDING,                                       /**< There is a RACP indication waiting to be sent. */
    STATE_RACP_RESPONSE_IND_VERIF                                      /**< Waiting for a verification of a RACP indication. */
} BLE_Glucose_RACPstate;

/**brief Glucose Record Access Point */
typedef struct BLE_Glucose_RACP{
    u8 opCode;
    u8 oPerator;
	u16 operand;
}BLE_Glucose_RACP;
/**@brief Time stamp parameters */

/**@brief Glucose measurement parameters */
typedef struct BLE_GLUCOSE_Measurement
{
   	bool IsTimeOffsetEnabled;
	bool IsGlucoseConcentrationTypeSampleLocationPresent;
	bool IsGlucoseConcentrationUnits;
	bool IsSensorStatusAnnunciationPresent;
 	bool IsContextInformationFollows;
	u16 SequenceNumber;
	Time_Field basetime;
	s16 timeoffset;
	S_float glucoseConcentrationUnitsPerLitre;
	u8 typeFieldN; 
	u8 sampleLocationN;
	u16	sensorStatus;
}BLE_GLUCOSE_Measurement;


//*** Glucose Measurement Context structure 
typedef struct BLE_GLUCOSE_Measurement_Context
{
	bool IsCarbohydrateIDPresent;
	bool IsMealPresent;
	bool IsTesterHealthEnabled;
	bool IsExerciseDurationIntensityPresent;
	bool IsMedicationIDPresent;
	bool IsMedicationValueUnit;
	bool IsHbA1CprecisePercentagePresent;
	bool IsExtendedFlagPresent;
	u16  SequenceNumber;
	u8 ExtendedFlags;
	u8 CarbohydrateID;
	S_float CarbohydrateUnit;
	u8 Meal;
	u8 TesterN;
	u8 HealthN;
	u16 ExerciseDuration;
    u8 ExerciseIntensity;
	u8 MedicationID;
	S_float MedicationPresent; 
	S_float MedicationUnit;//kg/liter
    S_float HbA1cS;
}BLE_GLUCOSE_Measurement_Context;


/**@brief Glucose measurement record */
typedef struct BLE_GLUCOSE_RecordsDatabase
{
  BLE_GLUCOSE_Measurement          meas;                                /**< Glucose measurement */
  BLE_GLUCOSE_Measurement_Context  context;                             /**< Glucose measurement context */
} BLE_GLUCOSE_RecordsDatabase;

/**@brief structure for Glucose measurement record */
typedef struct
{
    bool          in_use_flag;
    BLE_GLUCOSE_RecordsDatabase record;
} BLE_GLUCOSE_RecordsDatabase_Entry;

//typedef void(*GlucoseProcessRoutine)(u8 eventType) large;

//typedef void(*GlucoseProcessRoutine)(u16 eventType) large;
typedef void(*GlucoseProcessRoutine)(BLE_GLUCOSE_Event XDATA *p_glucoseEvent) large;


PUBLIC extern u16 XDATA  BLE_Glucose_Feature;
PUBLIC extern u16 XDATA  glucoseMeasurementHandle; 
PUBLIC extern u16 XDATA  glucoseMeasurementCCDHandle;
PUBLIC extern u16 XDATA  glucoseMeasurementContextHandle;
PUBLIC extern u16 XDATA  glucoseMeasurementContextCCDHandle;
PUBLIC extern u16 XDATA  glucoseFeatureHandle;
PUBLIC extern u16 XDATA  glucoseRecordAccessControlPointHandle;
PUBLIC extern u16 XDATA  glucoseRecordAccessControlPointCCDHandle;

/**@brief Initizlize BLE glucose.
 *
 * @param[in] smpHandler		MBIoT callback function for BLE glucose events. 
 *
 * @return @ref MBIOT_RES_SUCCESS		Initialize BLE Glucose Scale successfully.
 */
PUBLIC extern u16 BLE_GLUCOSE_Init(GlucoseProcessRoutine glucoseHandler)large; 


/**@brief Initizlize BLE Glucose service table.
 *
 * @return @ref MBIOT_RES_SUCCESS		Initialize BLE Glucose Scale service table successfully.
 */
PUBLIC extern u16 BLE_GLUCOSE_Init_Service()large;


/**@brief Function for initializing the glucose record database.
 *
 * @details This call initializes the database holding glucose records.
 *
 * @return      NRF_SUCCESS on success. 
 */
PUBLIC extern u16 BLE_GLUCOSE_Database_Init(void) large;

/**@brief Add Glucose measurement database.
 *
 * @param[in] p_recorddatabase		Pointer to Nested structure of BLE_GLUCOSE_Measurement and BLE_GLUCOSE_Measurement_Context structure. 
 *
 * @return @ref MBIOT_RES_SUCCESS		add glucose measurement  database successfully.
 */
PUBLIC extern u16 BLE_Add_Glucose_MeasurementDatabase(BLE_GLUCOSE_RecordsDatabase XDATA * p_recorddatabase)large;


/**@brief Delete specific Glucose measurement record.
 *
 * @param[in] record_num   Index of record to delete.
 * @return @ref MBIOT_RES_SUCCESS		add glucose measurement  record deleted  successfully.
 */
PUBLIC extern u16 BLE_Delete_Glucose_MeasurementRecord(u8 XDATA record_index)large;

/**@brief Function for getting the number of records in the database.
 *
 * @details This call returns the number of records in the database.
 *
 * @return      Number of records in the database.
 */

PUBLIC extern u16 BLE_Get_NumberOfRecords_Glucose_MeasurementRecord(void)large;

/**@brief Function for getting a record from the database.
 *
 * @details This call returns a specified record from the database.
 *
 * @param[in]   record_index    Index of the record to retrieve.
 * @param[out]  p_recorddatabase         Pointer to record structure where retrieved record is copied to.
 * 
 * @return      MBIOT_RES_SUCCESS on success.
 */
PUBLIC extern u16 BLE_Get_Glucose_MeasurementRecord(u8 record_index,BLE_GLUCOSE_RecordsDatabase XDATA * p_recorddatabase )large;


/**@brief Catch BLE_GATT events to get the Glucose scale events.
 *
 * @param[in] p_event		Pointer to BLE_GATT_Event structure.
 *
 * @return @ref MBIOT_RES_SUCCESS		Catch BLE GATT events configuration successfully.
 */
PUBLIC extern void BLE_Glucose_Catch_Event(BLE_GATT_Event XDATA * p_event)large;

/**@brief Set Low_Battery_Detection option.
 *
 * @param[in] isLowBatterySupported		If Low_Battery_Detection field is supported, set this value to be true. Default Low_Battery_Detection field is unsupported.
 *
 * @return MBIOT_RES_SUCCESS		Successfully set Low_Battery_Detection option.
 * @return MBIOT_RES_FAIL			Operation is not permitted.
 * @return MBIOT_RES_INVALID_PARA	Invalid parameters.
 */
 
PUBLIC u16 BLE_Glucose_Low_Battery_Detection(bool isLowBatterySupported) large;

/**@brief Set Set_Sensor_Manfunction_Detection option.
 *
 * @param[in] isMalfunctionSupported		If Set_Sensor_Manfunction_Detection field is supported, set this value to be true. Default Set_Sensor_Manfunction_Detection field is unsupported.
 *
 * @return MBIOT_RES_SUCCESS		Successfully set Set_Sensor_Manfunction_Detection option.
 * @return MBIOT_RES_FAIL			Operation is not permitted.
 * @return MBIOT_RES_INVALID_PARA	Invalid parameters.
 */

PUBLIC u16 BLE_Glucose_Set_Sensor_Manfunction_Detection(bool isMalfunctionSupported) large;

/**@brief Set Set_SampleSize option.
 *
 * @param[in] isSamplesizeSupported		If Set_SampleSize field is supported, set this value to be true. Default Set_SampleSize field is unsupported.
 *
 * @return MBIOT_RES_SUCCESS		Successfully set Set_SampleSize option.
 * @return MBIOT_RES_FAIL			Operation is not permitted.
 * @return MBIOT_RES_INVALID_PARA	Invalid parameters.
 */

PUBLIC u16 BLE_Glucose_Set_SampleSize(bool isSamplesizeSupported) large;

/**@brief Set Set_Strip_Insertion_Error_Detection option.
 *
 * @param[in] isStripInsertionSupported		If Set_Strip_Insertion_Error_Detection field is supported, set this value to be true. Default Set_Strip_Insertion_Error_Detection field is unsupported.
 *
 * @return MBIOT_RES_SUCCESS		Successfully set Set_Strip_Insertion_Error_Detection option.
 * @return MBIOT_RES_FAIL			Operation is not permitted.
 * @return MBIOT_RES_INVALID_PARA	Invalid parameters.
 */

PUBLIC u16 BLE_Glucose_Set_Strip_Insertion_Error_Detection(bool isStripInsertionSupported) large;


/**@brief Set Set_Strip_Type option.
 *
 * @param[in] isStripTypeSupported		If Set_Strip_Type field is supported, set this value to be true. Default Set_Strip_Type field is unsupported.
 *
 * @return MBIOT_RES_SUCCESS		Successfully set Set_Strip_Type option.
 * @return MBIOT_RES_FAIL			Operation is not permitted.
 * @return MBIOT_RES_INVALID_PARA	Invalid parameters.
 */

PUBLIC u16 BLE_Glucose_Set_Strip_Type(bool isStripTypeSupported) large;

/**@brief Set Read_Interrupt_Detection option.
 *
 * @param[in] isReadInterruptSupported		If Read_Interrupt_Detection field is supported, set this value to be true. Default Read_Interrupt_Detection field is unsupported.
 *
 * @return MBIOT_RES_SUCCESS		Successfully set Read_Interrupt_Detection option.
 * @return MBIOT_RES_FAIL			Operation is not permitted.
 * @return MBIOT_RES_INVALID_PARA	Invalid parameters.
 */


PUBLIC u16 BLE_Glucose_Set_Read_Interrupt_Detection(bool isReadInterruptSupported) large;

/**@brief Set Device_Fault_Supported option.
 *
 * @param[in] isDeviceFaultSupported		If Device_Fault_Supported field is supported, set this value to be true. Default Device_Fault_Supported field is unsupported.
 *
 * @return MBIOT_RES_SUCCESS		Successfully set Device_Fault_Supported option.
 * @return MBIOT_RES_FAIL			Operation is not permitted.
 * @return MBIOT_RES_INVALID_PARA	Invalid parameters.
 */


PUBLIC u16 BLE_Glucose_Set_Device_Fault_Supported(bool isDeviceFaultSupported) large;

/**@brief Set Time_Fault_Supported option.
 *
 * @param[in] isTimeFaultSupported		If Time_Fault_Supported field is supported, set this value to be true. Default Time_Fault_Supported field is unsupported.
 *
 * @return MBIOT_RES_SUCCESS		Successfully set Time_Fault_Supported option.
 * @return MBIOT_RES_FAIL			Operation is not permitted.
 * @return MBIOT_RES_INVALID_PARA	Invalid parameters.
 */

PUBLIC u16 BLE_Glucose_Set_Time_Fault_Supported(bool isTimeFaultSupported) large;

/**@brief Set Result_High_Low_Detection option.
 *
 * @param[in] isHighLowResultSupported		If Result_High_Low_Detection field is supported, set this value to be true. Default Result_High_Low_Detection field is unsupported.
 *
 * @return MBIOT_RES_SUCCESS		Successfully set Result_High_Low_Detection option.
 * @return MBIOT_RES_FAIL			Operation is not permitted.
 * @return MBIOT_RES_INVALID_PARA	Invalid parameters.
 */

PUBLIC u16 BLE_Glucose_Set_Result_High_Low_Detection(bool isHighLowResultSupported) large;

/**@brief Set Temperature_High_Low_Detection option.
 *
 * @param[in] isHighLowTempSupported		If Temperature_High_Low_Detection field is supported, set this value to be true. Default Temperature_High_Low_Detection field is unsupported.
 *
 * @return MBIOT_RES_SUCCESS		Successfully set Temperature_High_Low_Detection option.
 * @return MBIOT_RES_FAIL			Operation is not permitted.
 * @return MBIOT_RES_INVALID_PARA	Invalid parameters.
 */

PUBLIC u16 BLE_Glucose_Set_Temperature_High_Low_Detection(bool isHighLowTempSupported) large;

/**@brief Set Glucose_MultiBond option.
 *
 * @param[in] isMultiBondSupported		If Glucose_MultiBond field is supported, set this value to be true. Default Glucose_MultiBond field is unsupported.
 *
 * @return MBIOT_RES_SUCCESS		Successfully set Glucose_MultiBond option.
 * @return MBIOT_RES_FAIL			Operation is not permitted.
 * @return MBIOT_RES_INVALID_PARA	Invalid parameters.
 */


PUBLIC u16 BLE_Glucose_Set_Glucose_MultiBond(bool isMultiBondSupported)large;

	
/**@brief Send error response.
 *
 * @param[in] respType, write response type.
 *
 * @return @ref MBIOT_RES_SUCCESS sent error response successfully.
 */
PUBLIC u16 BLE_Send_Glucose_Error_Response(u8 connHandle, u8 respType);


/**@brief Send write response.
 *
 * @param[in] respType, write response type.
 *
 * @return @ref MBIOT_RES_SUCCESS sent error response successfully.
 */
PUBLIC u16 BLE_Send_Glucose_Write_Response(u8 connHandle, u8 respType);


/**@brief Send Glucose measurement.
 *
 * @param[in] p_measurement		Pointer to BLE_GLUCOSE_Measurement structure. 
 *
 * @return @ref MBIOT_RES_SUCCESS		Send glucose measurement successfully.
 */
PUBLIC extern u16 BLE_Send_Glucose_Measurement(u8 connHandle,BLE_GLUCOSE_Measurement XDATA * p_measurement)large;
/**@brief Send Glucose measurement Context.
 *
 * @param[in] p_measurement		Pointer to BLE_GLUCOSE_Measurement_Context structure. 
 *
 * @return @ref MBIOT_RES_SUCCESS		Send g lucose measurement context successfully.
 */

PUBLIC extern u16 BLE_Send_Glucose_Measurement_Context(u8 connHandle,BLE_GLUCOSE_Measurement_Context XDATA * p_measurementContext)large;

#endif

/**
  @}
 */