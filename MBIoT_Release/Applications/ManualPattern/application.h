/********************************************************************
  File Information:
    FileName:       application.h
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
 * @addtogroup BLE_MANUAL_PATTERN BLE BLE_MANUAL_PATTERN
 * @{
 * @brief Definitions and prototypes for the BLE Manual Pattern Application.
 */

#ifndef APPLICATION_H
#define APPLICATION_H

//#define APP_OPEN_DEBUG_MODE

#define APP_DEBUG_TX_BUFFER_LEN			512
#define APP_DEBUG_RX_BUFFER_LEN			256

/**@brief App Heap Size. */
#define APP_HEAP_SIZE            		(1024)

/**@brief Maximum allowing Conncetion Numbers for MBIoT. */
#define MAX_CONN_NBR		4

#define APP_RX_BUFFER_LEN	1024		/**< APP RX Buffer Length. */
#define APP_TX_BUFFER_LEN	1024		/**< APP TX Buffer Length. */

#define MAX_UART_CMD_BUFFER		200

#define APP_PROCESS_CMD_ALLOW				0
#define APP_PROCESS_CMD_FORBID				1

#define APP_PASSKEY_LENGTH		6

#define ADC_VALUE					0x00
#define ADC_VALUE_UNIT_0_1V			0x01
#define ADC_VALUE_UNIT_0_05V		0x02
#define ADC_VALUE_UNIT_0_025V		0x03

#define STATIC_RANDOM_ADDRESS_MASK			0xc0

#define UART_RX_STATE_IDLE					0x00
#define UART_RX_STATE_LENGTH_H				0x01
#define UART_RX_STATE_LENGTH_L				0x02
#define UART_RX_STATE_CMD_ID				0x03
#define UART_RX_STATE_PAYLOAD				0x04
#define UART_RX_STATE_CHECKSUM				0x05

#define UART_START_BYTE						0xAA

#define CHECK_NUM 3
#define CHECK_TRANS_TX 0
#define CHECK_TRANS_RX 1
#define CHECK_TRANS_CTRL 2
#define TRANS_RETRY 0
#define TRANS_REPLAY 1
#define TRANS_NOREPLAY 2
#define TRANS_RETRY_COUNT_MAX	13
#define MAX_PACKET_BUFFER_SIZE		8

#define APP_START_HANDLE 0x0001
#define APP_END_HANDLE 0xFFFF
#define GET_REMOTE_TABLE_STEP1	0x01
#define GET_REMOTE_TABLE_STEP2	0x02
#define DISCOVER_PRIM_SER_BY_UUID_4 0x4
#define DISCOVER_PRIM_SER_BY_UUID_12 0x12

/**@defgroup MANUAL_PATTERN_UART_COMMAND_OPCODES MANUAL_PATTERN_UART_COMMAND_OPCODES
 * @brief Opcodes for Manual Pattern Application UART Commands.
 * @{ */
#define UART_CMD_READ_INFO								0x01		/**< Common Command: Read Device Information. */
#define UART_CMD_RESET									0x02		/**< Common Command: Reset. */
#define UART_CMD_READ_STATUS							0x03		/**< Common Command: Read Device Status. */
#define UART_CMD_READ_ADC_VALUE							0x04		/**< Common Command: Read ADC Value. */
#define UART_CMD_INTO_SHUTDOWN_MODE						0x05		/**< Common Command: Force Device Into Shutdown Mode. */
#define UART_CMD_DEBUG_COMMAND							0x06		/**< Common Command: Debug Command. */
#define UART_CMD_READ_DEVICE_NAME						0x07		/**< Common Command: Read Bluetooth Device Name. */
#define UART_CMD_WRITE_DEVICE_NAME						0x08		/**< Common Command: Write Bluetooth Device Name. */
#define UART_CMD_ERASE_ALL_PAIRED_DEVICE				0x09		/**< Common Command: Erase All Paired Devices Information. */
#define UART_CMD_READ_PAIRING_MODE						0x0A		/**< Common Command: Read Pairing Mode. */
#define UART_CMD_WRITE_PAIRING_MODE						0x0B		/**< Common Command: Write Pairing Mode. */
#define UART_CMD_READ_ALL_PAIRED_DEVICE					0x0C		/**< Common Command: Read All Paired Devices Information. */
#define UART_CMD_DELETE_PAIRED_DEVICE					0x0D		/**< Common Command: Delete Selected Paired Device. */
#define UART_CMD_DIO_CONTROL							0x0E		/**< Common Command: IO Control. */
#define UART_CMD_PWM_CONTROL							0x0F		/**< Common Command: PWM Control. */
#define UART_CMD_READ_RSSI								0x10		/**< GAP Command: Read RSSI Value. */
#define UART_CMD_WRITE_ADV_DATA							0x11		/**< GAP Command: Write Advertising Data. */
#define UART_CMD_WRITE_SCAN_RES_DATA					0x12		/**< GAP Command: Write Scan Response Data. */
#define UART_CMD_SET_ADV_PARAMETER						0x13		/**< GAP Command: Set Advertising Parameter. */
#define UART_CMD_SET_SCAN_PARAMETER						0x15		/**< GAP Command: Set Scan Parameter. */
#define UART_CMD_SET_SCAN_ENABLE						0x16		/**< GAP Command: Set Scan Enable. */
#define UART_CMD_LE_CREATE_CONNECTION					0x17		/**< GAP Command: Create LE Connection. */
#define UART_CMD_LE_CREATE_CONNECTION_CANCEL			0x18		/**< GAP Command: Cancel Creating LE connection. */
#define UART_CMD_CONNECTION_PARA_UPDATE_REQ				0x19		/**< GAP Command: Connection Parameter Update Request. */
#define UART_CMD_DISCONNECT								0x1B		/**< GAP Command: Disconnect connection. */
#define UART_CMD_SET_ADV_ENABLE							0x1C		/**< GAP Command: Set Advertising Enable. */
#define UART_CMD_SHOW_ALL_LINK_STATUS					0x1D		/**< GAP Command: Show All Link Status. */
#define UART_CMD_SEND_READ_BY_TYPE_RESPONSE				0x2C		/**< GATT Server Command: Send Read By Type Response. */
#define UART_CMD_HANDLE_VALUE_CONFIRMATION				0x2D		/**< GATT Client Command: Send Handle Value Confirmation. */
#define UART_CMD_DISCOVER_ALL_CHARACTERISTICS			0x2E		/**< GATT Client Command: Discover All Characteristics. */
#define UART_CMD_DISCOVER_ALL_DESCRIPTORS				0x2F		/**< GATT Client Command: Discover All Descriptors. */
#define UART_CMD_DISCOVER_ALL_PRIMARY_SERVICES			0x30		/**< GATT Client Command: Discover All Primary Services. */
#define UART_CMD_DISCOVER_PRIMARY_SERVICE_BY_UUID		0x31		/**< GATT CLient Command: Discover Primary Service by Service UUID. */
#define UART_CMD_READ_CHARACTERISTIC_VALUE				0x32		/**< GATT Client Command: Read Characteristic Value. */
#define UART_CMD_READ_USING_CHARACTERISTIC_UUID			0x33		/**< GATT Client Command: Read Using UUID. */
#define UART_CMD_WRITE_CHARACTERISTIC_VALUE				0x34		/**< GATT Client Command: Write Characteristic. */
#define UART_CMD_ENABLE_TRANSPARENT					0x35		/**< GATT Client Enable Transparent Service */
#define UART_CMD_SEND_ERROR_RESPONSE					0x37		/**< GATT Server Command: Send Error Response. */
#define UART_CMD_SEND_HANDLE_VALUE						0x38		/**< GATT Server Command: Send Handle Value Notification/Indication. */
#define UART_CMD_SET_HANDLE_VALUE						0x39		/**< GATT Server Command: Set Handle Value. */
#define UART_CMD_GET_HANDLE_VALUE						0x3A		/**< GATT Server Command: Get Handle Value. */
#define UART_CMD_RESERVE_ID_3B							0x3B		/**< Reserved. For compatibility with BLEDK3. */
#define UART_CMD_RESERVE_ID_3C							0x3C		/**< Reserved. For compatibility with BLEDK3. */
#define UART_CMD_SEND_WRITE_RESPONSE					0x3D		/**< GATT Server Command: Send Write Response. */
#define UART_CMD_SEND_READ_RESPONSE						0x3E		/**< GATT Server Command: Send Read Response. */
#define UART_CMD_SEND_TRANSPARENT_DATA					0x3F		/**< GATT Server/Client Command: Send transparent data. */
#define	UART_CMD_PASSKEY_ENTRY_RES						0x40		/**< SMP Command: Send Passkey Entry Response. */
#define	UART_CMD_DISPLAY_YESNO_RES						0x41		/**< SMP Command: Send Display Yes/NO Response. */
#define UART_CMD_PAIRING_REQUEST						0x42		/**< SMP Command: Send Pairing Request. */
#define UART_CMD_READ_PIN_CODE							0x50		/**< Common Command: Read PIN Code. */
#define UART_CMD_WRITE_PIN_CODE							0x51		/**< Common Command: Write PIN Code. */
#define UART_CMD_LEAVE_CONFIGURE_MODE					0x52		/**< Common Command: Leave Configure Mode. */
#define UART_CMD_PERIPHERAL_API_TEST					0xF0		/**< Peripheral Command: Test Command for Verifying Peripheral APIs. */
#define UART_CMD_PERIPHERAL_API_TEST2					0xF1 	/**< Peripheral Command: Test2 Command for Verifying Peripheral APIs. */
#define UART_CMD_MULTI_LINK_CONTROL					0xF2 	/**< Common Command: Command for Verifying Multi-link send data simultaneously. */
/** @} */

#ifdef UART_CMD_PERIPHERAL_API_TEST
#define APP_NORMAL_MODE 0
#define APP_SPI_MASTER 			1
#define APP_SPI_SLAVE 			2
#define APP_SPI_CONFIG 			0
#define APP_SPI_CLOSE 			1
#define APP_SPI_SEND_DATA 		2
#define APP_SPI_BUFFER_LEN		255
#define APP_SPI_MASTER_CLK_RATE_4MHZ 0x00
#define APP_SPI_MASTER_CLK_RATE_2MHZ 0x01
#define APP_SPI_MASTER_CLK_RATE_1MHZ 0x02
#define APP_SPI_MASTER_CLK_RATE_500HZ 0x03
#define APP_SPI_SLAVE_CLK_RATE_16MHZ 0x00
#define APP_SPI_SLAVE_CLK_RATE_8MHZ 0x01
#define APP_SPI_SLAVE_CLK_RATE_4MHZ 0x02
#define APP_SPI_SLAVE_CLK_RATE_2MHZ 0x03

#define PERIPHERAL_API_TEST_ADC						0x01
#define PERIPHERAL_API_TEST_PWM						0x02
#define PERIPHERAL_API_TEST_LED						0x03
#define PERIPHERAL_API_TEST_GPIO					0x04
#define PERIPHERAL_API_TEST_SPI_Master				0x05
#define PERIPHERAL_API_TEST_SPI_Slave				0x06
#define PERIPHERAL_API_TEST_I2C						0x07
#define PERIPHERAL_API_TEST_SPI_Serial_Flash		0x08
#define PERIPHERAL_API_TEST_LOWPOWER_OPERATION			0x09


#define TEST_ADC_GET_1_ADC_CHANNEL					0x01
#define TEST_ADC_GET_MULTI_ADC_CHANNEL 				0x02
#define TEST_ADC_GET_BATTERY    					0x03
#define TEST_ADC_GET_REFERENCE_CALIBRATION_VALUE    0x04

#define TEST_PWM_CONFIGURATION						0x01
#define TEST_PWM_START								0x02
#define TEST_PWM_STOP								0x03
#define TEST_PWM_CLOSE								0x04

#define TEST_LED_CONFIGURATION						0x01
#define TEST_LED0_FLASH_CONTROL						0x02
#define TEST_LED1_FLASH_CONTROL						0x03
#define TEST_LED_COMBO_FLASH_CONTROL				0x04
#define TEST_LED_CLOSE								0x05

#define TEST_GPIO_RANGE_CONFIG_DIRECTION			0x01
#define TEST_GPIO_PIN_CONFIG_DIRECTIO				0x02
#define TEST_GPIO_PORT_CONFIG_DIRECTION				0x03
#define TEST_GPIO_PIN_SET							0x04
#define TEST_GPIO_PORT_SET							0x05
#define TEST_GPIO_PORT_WRITE						0x06
#define TEST_GPIO_PIN_CLEAR							0x07
#define TEST_GPIO_PORT_CLEAR						0x08
#define TEST_GPIO_PIN_READ							0x09
#define TEST_GPIO_PORT_READ							0x0a
#define TEST_GPIO_POLLING_START						0x0b
#define TEST_GPIO_POLLING_STOP						0x0c

#define TEST_I2C_CONFIGURATION						0x01
#define TEST_I2C_WRITE								0x02
#define TEST_I2C_READ								0x03
#define TEST_I2C_COMBINEWR							0x04
#define TEST_I2C_CLOSE								0x05

#define TEST_SPI_SFLASH_READ						0x01 
#define TEST_SPI_SFLASH_WRITE						0x02
#define TEST_SPI_SFLASH_ERASE						0x03
#define TEST_SPI_SFLASH_READID						0x04
#define  FLASH_WE_COM 0x06
#define  ERASE_CHIP	0x00
#define  ERASE_SECTOR	0x01
#define  ERASE_BLOCK 	0x02

#define  FLASH_PP_COM 0x02
#define  FLASH_CHIP_ERASE_COM 0xc7
#define  FLASH_SECTOR_ERASE_COM 0x20
#define  FLASH_BLOCK_ERASE 0xd8
#define  FLASH_FR_COM	0x03
#define  FLASH_DEVICE_ID	0x90

#define TEST_LOW_POWER_CTRL	0x00
#define TEST_LOW_POWER_ULPC_CTRL	0x01
#endif

/**@defgroup MANUAL_PATTERN_UART_EVENT_OPCODES MANUAL_PATTERN_UART_EVENT_OPCODES
 * @brief Opcodes for Manual Pattern Application UART Events.
 * @{ */
#define UART_EVENT_PASSKEY_ENTRY_REQ						0x60		/**< SMP Event: Passkey Entry Request. */
#define UART_EVENT_PAIRING_COMPLETE							0x61		/**< SMP Event: Pairing Complete. */
#define UART_EVENT_DISPLAY_YESNO_REQ						0x62		/**< SMP Event: Display Yes/No Request. */
#define UART_EVENT_ADVERTISING_REPORT						0x70		/**< GAP Event: Advertising Report. */
#define UART_EVENT_LE_CONNECTION_COMPLETE					0x71		/**< GAP Event: LE Connection Complete. */
#define UART_EVENT_DISCONNECT_COMPLETE						0x72		/**< GAP Event: Disconnection Complete. */
#define UART_EVENT_CONNECTION_PARA_UPDATE_NOTIFY			0x73		/**< GAP Event: Connection Parameter Update Notify. */
#define UART_EVENT_COMMAND_COMPLETE							0x80		/**< Common Event: Command Complete. */
#define UART_EVENT_STATUS_REPORT							0x81		/**< Common Event: Device Status and Connection Link Status Report. */
#define UART_EVENT_CONFIGURE_MODE_STATUS					0x8f		/**< Common Event: Configure Mode Status. */
#define UART_EVENT_DISCOVER_ALL_PRIMARY_SERVICES_RESP		0x90		/**< GATT Client Event: Discover All Primary Services Response. */
#define UART_EVENT_DISCOVER_ALL_CHARACTERISTICS_RESP		0x91		/**< GATT Client Event: Discover All Characteristics Response. */
#define UART_EVENT_DISCOVER_ALL_DESCRIPTORS_RESP			0x92		/**< GATT Client Event: Discover All Descriptors Response. */
#define UART_EVENT_CHARACTERISTIC_VALUE_RECEIVED			0x93		/**< GATT Client Event: Receive Handle Value Notification/Indication. */
#define UART_EVENT_READ_RESP								0x94		/**< GATT Client Event: Receive Read Response. */
#define UART_EVENT_READ_USING_UUID_RESP						0x95		/**< GATT Client Event: Receive Read Using UUID Response. */
#define UART_EVENT_ERROR_RESP								0x96		/**< GATT Client Event: Receive Error Response. */
#define UART_EVENT_WRITE_RESP								0x97		/**< GATT CLient Event: Receive Write Response. */
#define UART_EVENT_CLIENT_WRITE_CHAR_VALUE					0x98		/**< GATT Server Event: Receive Client Write Requests/Command. */
#define UART_EVENT_READ_REQ									0x99		/**< GATT Server Event: Receive Client Read Request. */
#define UART_EVENT_RECEIVED_TRANSPARENT_DATA				0x9A		/**< GAP Event: Received Transparent Data. */
#define UART_EVENT_HANDLE_VALUE_CONFIRMATION_RECEIVED		0x9B		/**< GATT Server Event: Receive Handle Value Confirmation. */
#define UART_EVENT_DISCOVER_PRIMARY_SERVICE_BY_UUID_RESP	0x9C		/**< GATT Client Event: Discover Primary Service By UUID Response. */
#define UART_EVENT_READ_BY_TYPE_REQ							0x9D		/**< GATT Server Event: Read By Type Request. */
/** @} */

/**@defgroup BLE_SMP_NOTIFY_TYPE BLE_SMP_NOTIFY_TYPE
 * @brief Notification Type for UART Passkey Response Command.
 * @{ */
#define NOTIFICATION_TYPE_PASSKEY_STARTED		0x00			/**< Notification Type: Passkey Started. */
#define NOTIFICATION_TYPE_PASSKEY_ENTERED		0x01			/**< Notification Type: Passkey Entered. */
#define NOTIFICATION_TYPE_PASSKEY_ERASED		0x02			/**< Notification Type: Passkey Erased. */
#define NOTIFICATION_TYPE_PASSKEY_CLEARED		0x03			/**< Notification Type: Passkey Cleared. */
#define NOTIFICATION_TYPE_PASSKEY_COMPLETED		0x04			/**< Notification Type: Passkey Completed. */
/** @} */

/**@defgroup BLE_DIS_UUID	BLE_DIS_UUID
 * @brief UUID Definitions for Device Information Service.
 * @{ */
#define UUID_DEVICE_INFORMATION				0x180A		/**< Device Information Service UUID. */
#define UUID_SYSTEM_ID						0x2A23		/**< System ID UUID. */
#define UUID_MODEL_NUMBER_STRING			0x2A24		/**< Model Number String UUID. */
#define UUID_SERIAL_NUMBER_STRING			0x2A25		/**< Serial Number String UUID. */
#define UUID_FIRMWARE_REVISION_STRING		0x2A26		/**< Firmware Revision String UUID. */
#define UUID_HARDWARE_REVISION_STRING		0x2A27		/**< Hardware Revision String UUID. */
#define UUID_SOFTWARE_REVISION_STRING		0x2A28		/**< Software Revision String UUID. */
#define UUID_MANUFACTURE_NAME_STRING		0x2A29		/**< Manufacture Name String UUID. */
#define UUID_IEEE_11073_20601				0x2A2A		/**< IEEE 11073 20601 Format UUID. */

#define UUID_MCHP_PROPRIETARY_SERVICE_16	0x49,0x53,0x53,0x43,0xFE,0x7D,0x4A,0xE5,0x8F,0xA9,0x9F,0xAF,0xD2,0x05,0xE4,0x55		/**< Definition of MCHP proprietary service UUID. */
#define UUID_MCHP_TRANS_TX_16	0x49,0x53,0x53,0x43,0x1E,0x4D,0x4B,0xD9,0xBA,0x61,0x23,0xC6,0x47,0x24,0x96,0x16			/**< Definition of MCHP Transparent TX characteristic UUID. */
#define UUID_MCHP_TRANS_RX_16	0x49,0x53,0x53,0x43,0x88,0x41,0x43,0xF4,0xA8,0xD4,0xEC,0xBE,0x34,0x72,0x9B,0xB3			/**< Definition of MCHP Transparent RX characteristic UUID. */
#define UUID_MCHP_TRANS_CTRL_16 0x49,0x53,0x53,0x43,0x4c,0x8a,0x39,0xb3,0x2f,0x49,0x51,0x1c,0xff,0x07,0x3b,0x7e			/**< Definition of MCHP Transparent Control characteristic UUID. */
#define UUID_MCHP_TRANS_TX_INVERSE_16  0x16,0x96, 0x24,0x47,0xC6,0x23,0x61,0xBA,0xD9,0x4B,0x4D,0x1E,0x43,0x53,0x53,0x49			/**< Definition Inverse of MCHP Transparent TX characteristic UUID. */
#define UUID_MCHP_TRANS_RX_INVERSE_16  0xB3,0x9B,0x72,0x34,0xBE,0xEC,0xD4,0xA8,0xF4,0x43,0x41,0x88,0x43,0x53,0x53,0x49   			/**< Definition Inverse of MCHP Transparent RX characteristic UUID. */
#define UUID_MCHP_TRANS_CTRL_INVERSE_16  0x7e,0x3b,0x07,0xff,0x1c,0x51,0x49,0x2f,0xb3,0x39,0x8a,0x4c,0x43,0x53,0x53,0x49   		/**< Definition Inverse of MCHP Transparent Control characteristic UUID. */
/** @} */

#define APP_ErrCode_Unknown_Command			0x01	/**< Application Error Code: Unknown Command. */
#define APP_ErrCode_Unknown_Connection_Identifier		0x02	/**< Application Error Code: Unknown Connection Identifier. */
#define APP_ErrCode_Connection_Timeout		0x08	/**< Application Error Code: Connection Timeout. */
#define APP_ErrCode_Connection_Limit_Exceeded		0x09	/**< Application Error Code: Connection Limit Exceeded */
#define APP_ErrCode_Command_Disallowed		0x0C	/**< Application Error Code: Command Disallowed. */
#define APP_ErrCode_Connection_Rejected_Limited_Resources		0x0D	/**< Application Error Code: Connection Rejected due to Limited Resources. */
#define APP_ErrCode_Connection_Rejected_Security_Reasons		0x0E	/**< Application Error Code: Connection Rejected Due To Security Reasons. */
#define APP_ErrCode_Connection_Rejected_Unacceptable_BD_ADDR		0x0F	/**< Application Error Code: Connection Rejected due to Unacceptable BD_ADDR. */
#define APP_ErrCode_Connection_Rejected_Accept_Timeout_Exceeded		0x10	/**< Application Error Code: Connection Accept Timeout Exceeded. */
#define APP_ErrCode_Invalid_Parameters		0x12	/**< Application Error Code: Invalid Parameters. */
#define APP_ErrCode_Remote_User_Terminated_Connection		0x13	/**< Application Error Code: Remote User Terminated Connection. */
#define APP_ErrCode_Remote_Device_Terminated_Connection_Low_Resources		0x14	/**< Application Error Code: Remote Device Terminated Connection due to Low Resources. */
#define APP_ErrCode_Remote_Device_Terminated_Connection_PowerOff		0x15	/**< Application Error Code: Remote Device Terminated Connection due to Power Off. */
#define APP_ErrCode_Connection_Terminated_ByLocalHost		0x16	/**< Application Error Code: Connection Terminated By Local Host. */
#define APP_ErrCode_Connection_Rejected_NoSuitableChannelFound	0x39	/**< Application Error Code: Connection Rejected due to No Suitable Channel Found. */
#define APP_ErrCode_Controller_Busy	0x3A	/**< Application Error Code: Controller Busy. */
#define APP_ErrCode_Unacceptable_Connection_Interval		0x3B	/**< Application Error Code: Unacceptable Connection Interval. */
#define APP_ErrCode_Directed_Advertising_Timeout		0x3C	/**< Application Error Code: Directed Advertising Timeout. */
#define APP_ErrCode_Connection_Terminated_MIC_Failure		0x3D	/**< Application Error Code: Connection Terminated due to MIC Failure. */
#define APP_ErrCode_Connection_Failed_Established		0x3E	/**< Application Error Code: Connection Failed to be Established. */
#define APP_ErrCode_Insufficient_Resources 0x91		/**< Application Error Code: Insufficient Resources. */

#define APP_DEFINED_ATTRIBUTE_NUMBER		0x11		/**< Number of Attributes in Application Defined Service. */
#define MAX_DEVICE_NO	8								/**< Maximum device number of pairing information of MBIOT. */

#define CLIENT_CHAR_CONFIG_DESCRIPTOR_DISABLE	0x0000
#define CLIENT_CHAR_CONFIG_DESCRIPTOR_ENABLE	0x0100

#define TR_READ_MTU_SIZE_AND_ENABLE_CREDIT_CTRL	0x14

#define BLE_GATT_ROLE_SERVER 0x00	/**< A server role in ATT protocol. */
#define BLE_GATT_ROLE_CLIENT 0x01   /**< A client role in ATT protocol. */

/** 
 * @brief Device State Definitions
 */
typedef enum
{
	APP_STATE_OFF,				/**< Application State: OFF. */
	APP_STATE_SCANNING,			/**< Application State: Scanning. */
	APP_STATE_CONNECTING,		/**< Application State: Connecting. */
	APP_STATE_STANDBY,			/**< Application State: Standby. */
	APP_STATE_BROADCASTING,		/**< Application State: Broadcasting. */
	RESERVED_STATE_1,			/**< Application State: Reserved State. */
	RESERVED_STATE_2,			/**< Application State: Reserved State. */
	RESERVED_STATE_3,			/**< Application State: Reserved State. */
	APP_STATE_LE_OPEN,			/**< Application State: Transparent Data Session Open. */
	APP_STATE_IDLE,				/**< Application State: Idle. */
	APP_STATE_SHUTDOWN,			/**< Application State: Shutdown. */
	RESERVED_STATE_4,			/**< Application State: Reserved State. */
	APP_STATE_LE_CONNECTED,		/**< Application State: LE Connected. */
	APP_STATE_TOTAL				/**< Application State: End of the State Definition. */
}APP_STATE_TYPE;

typedef enum
{
	WRITE_IDLE,
	WRITE_DATA_RECEIVE_CTRL_ENABLE,
	WRITE_DATA_RECEIVE_CTRL_DISABLE,
	WRITE_ENABLE_CREDIT_FLOW_CTRL,
	WRITE_INITIATE_CREDIT_FLOW_CTRL
} app_write_flag;

typedef enum
{
	QUERY_IDLE,
	QUERY_TX_CHAR,
	QUERY_RX_CHAR,
	QUERY_CTRL_CHAR
}app_query_status;

/**
 * @brief  The struct of handle list.
 */
typedef struct Handle_List
{
	u16	transTx;			/**< Handle of transparent tx. */
	u16 txCCCD;				/**< Handle of transparent tx cccd. */
	u16	transRx;			/**< Handle of transparent rx. */
	u16 transCtrl;			/**< Handle of transparent control. */
	u16 ctrlCCCD;			/**< Handle of transparent control cccd. */
}Handle_List;

enum BLE_TRS_AttributeIndex
{
	INDEX_MCHP_PRIMARY_SERVICE=0x00,
	INDEX_MCHP_TRANS_TX,
	INDEX_MCHP_TRANS_TX_VALUE,
	INDEX_MCHP_TRANS_TX_CCCD,
	INDEX_MCHP_TRANS_RX,
	INDEX_MCHP_TRANS_RX_VALUE,
	INDEX_MCHP_TRANS_CTRL,
	INDEX_MCHP_TRANS_CTRL_VALUE,
	INDEX_MCHP_TRANS_CTRL_CCCD,
	TOTAL_TRS_ATTR_NUM
};

/**@brief Enable Service for Testing of TX/RX performance of Multi-Link */
typedef struct Retry_Buffer
{
	u8 transConnHandle;		/**< The Connection Handle. */
	u8 transTxMode;			/**< Transparent Mode. */
	u8 XDATA *p_reTransData;	/**< Data Payload. */
}Retry_Buffer;

/**@brief Application Connection Parameters for Recording Connection Information. */
typedef struct APP_CONN_STRUCT
{
	u8					appState;							/**< Application Current State. */
	u8					role;								/**< BLE Role in the connection. */
	u8					role_att;							/**< SERVER or CLIENT Role in the ATT protocol. */
	u8					handle;								/**< The Connection Handle. */
	u8 					bondedDevice;						/**< Flag to Record the Connection Is Bonded or Not. */
	BLE_GAP_Addr 		remoteAddr;							/**< Pointer to Address Type Structure. */
	BLE_GAP_ConnParas	connPara;							/**< Pointer to the Connection Parameters Structure. */
	u8					passkeyIndex;						/**< Record Passkey Input Index in UART_CMD_PASSKEY_ENTRY_RES Command. */
	u8					passkey[6];							/**< Record Input Passkey. */
	u16					attMTU;								/**< Record the Current Connection MTU size. */
	u8					numericComparisonConfirmEnable;		/**< Flag to Record Receiving BLE_SMP_EVT_NUMERIC_COMPARISON_CONFIRM_REQUEST. */
	u8					Transparent_Rule;
	u8 					serviceState;
	u8 					txCapacityState;
	Handle_List			remoteHandleList;
}APP_CONN_STRUCT;

#endif

/**
  @}
 */
