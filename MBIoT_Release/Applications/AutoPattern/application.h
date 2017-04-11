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
#ifndef APPLICATION_H
#define APPLICATION_H

//#define APP_OPEN_DEBUG_MODE

#define APP_DEBUG_TX_BUFFER_LEN			512
#define APP_DEBUG_RX_BUFFER_LEN			256

#define APP_HEAP_SIZE            		(3*1024)			/**< The size of app heap. And it must be divisible by 32. */

/**@brief UUID Definitions */
#define UUID_DEVICE_INFORMATION						0x180A			/**< Definition of Device Information service UUID. */
#define UUID_SYSTEM_ID								0x2A23			/**< Definition of Sysem ID characteristic UUID. */
#define UUID_MODEL_NUMBER_STRING					0x2A24			/**< Definition of Model Number String characteristic UUID. */
#define UUID_SERIAL_NUMBER_STRING					0x2A25			/**< Definition of Serial Number String characteristic UUID. */
#define UUID_FIRMWARE_REVISION_STRING				0x2A26			/**< Definition of Firmware Revision String characteristic UUID. */
#define UUID_HARDWARE_REVISION_STRING				0x2A27			/**< Definition of Hardware Revision String characteristic UUID. */
#define UUID_SOFTWARE_REVISION_STRING				0x2A28			/**< Definition of Firmware Revision String characteristic UUID. */
#define UUID_MANUFACTURE_NAME_STRING				0x2A29			/**< Definition of Manufacturer Name String characteristic UUID. */
#define UUID_IEEE_11073_20601						0x2A2A			/**< Definition of IEEE 11073-20601 Regulatory Certification Data Lis characteristic UUID. */

#define UUID_MCHP_PROPRIETARY_SERVICE_16			0x49,0x53,0x53,0x43,0xFE,0x7D,0x4A,0xE5,0x8F,0xA9,0x9F,0xAF,0xD2,0x05,0xE4,0x55			/**< Definition of MCHP proprietary service UUID. */
#define UUID_MCHP_TRANS_TX_16						0x49,0x53,0x53,0x43,0x1E,0x4D,0x4B,0xD9,0xBA,0x61,0x23,0xC6,0x47,0x24,0x96,0x16			/**< Definition of MCHP Transparent TX characteristic UUID. */
#define UUID_MCHP_TRANS_RX_16						0x49,0x53,0x53,0x43,0x88,0x41,0x43,0xF4,0xA8,0xD4,0xEC,0xBE,0x34,0x72,0x9B,0xB3			/**< Definition of MCHP Transparent RX characteristic UUID. */
#define UUID_MCHP_TRANS_CTRL_16						0x49,0x53,0x53,0x43,0x4c,0x8a,0x39,0xb3,0x2f,0x49,0x51,0x1c,0xff,0x07,0x3b,0x7e			/**< Definition of MCHP Transparent Control characteristic UUID. */
#define UUID_MCHP_TRANS_TX_INVERSE_16  0x16,0x96, 0x24,0x47,0xC6,0x23,0x61,0xBA,0xD9,0x4B,0x4D,0x1E,0x43,0x53,0x53,0x49			/**< Definition Inverse of MCHP Transparent TX characteristic UUID. */
#define UUID_MCHP_TRANS_RX_INVERSE_16  0xB3,0x9B,0x72,0x34,0xBE,0xEC,0xD4,0xA8,0xF4,0x43,0x41,0x88,0x43,0x53,0x53,0x49   			/**< Definition Inverse of MCHP Transparent RX characteristic UUID. */
#define UUID_MCHP_TRANS_CTRL_INVERSE_16  0x7e,0x3b,0x07,0xff,0x1c,0x51,0x49,0x2f,0xb3,0x39,0x8a,0x4c,0x43,0x53,0x53,0x49   		/**< Definition Inverse of MCHP Transparent Control characteristic UUID. */

#define AD_TYPE_MANUFACTURE						0xff

#define CLIENT_CHAR_CONFIG_DESCRIPTOR_DISABLE	0x0000
#define CLIENT_CHAR_CONFIG_DESCRIPTOR_ENABLE	0x0100

//Transparent Vendor Command ID
#define TR_READ_MTU_SIZE_AND_ENABLE_CREDIT_CTRL	0x14

#define CREDIT_CTRL_STATUS_SUCCESS					0

#define CHECK_NUM 3
#define CHECK_TRANS_TX 0
#define CHECK_TRANS_RX 1
#define CHECK_TRANS_CTRL 2

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


enum BLE_DIS_AttributeIndex
{
	INDEX_DIS_PRIMARY_SERVICE=0x00,
	INDEX_MANUFACTURE_NAME,
	INDEX_MANUFACTURE_NAME_VALUE,
	INDEX_MODEL_NUMBER,
	INDEX_MODEL_NUMBER_VALUE,
	INDEX_SERIAL_NUMBER,
	INDEX_SERIAL_NUMBER_VALUE,
	INDEX_HARDWARE_REVISION,
	INDEX_HARDWARE_REVISION_VALUE,
	INDEX_FIRMWARE_REVISION,
	INDEX_FIRMWARE_REVISION_VALUE,
	INDEX_SOFTWARE_REVISION,
	INDEX_SOFTWARE_REVISION_VALUE,
	INDEX_SYSTEM_ID,
	INDEX_SYSTEM_ID_VALUE,
	INDEX_IEEE_11073_20601,
	INDEX_IEEE_11073_20601_VALUE,

	TOTAL_DIS_ATTR_NUM
};

#define APP_RX_BUFFER_LEN	512
#define APP_TX_BUFFER_LEN	1024

#define MAX_PACKET_BUFFER_LIST		8
#define RESERVED_PACKET_BUFFER		4


#define CREDIT_BASED_FLOW_CONTROL_DISABLED				0
#define CREDIT_BASED_FLOW_CONTROL_ENABLED				1

typedef struct Packet_Buffer
{
	u8 length;
	u8 XDATA *packet;
	u8 write_type;
}Packet_Buffer;

typedef enum
{
	QUERY_IDLE,
	QUERY_TX_CHAR,
	QUERY_RX_CHAR,
	QUERY_CTRL_CHAR	
}app_query_index;

typedef enum
{
	WRITE_IDLE,
	WRITE_DATA_RECEIVE_CTRL_ENABLE,
	WRITE_DATA_RECEIVE_CTRL_DISABLE,
	WRITE_ENABLE_CREDIT_FLOW_CTRL,
	WRITE_INITIATE_CREDIT_FLOW_CTRL,
	WRITE_START_CREDIT_FLOW_CTRL
} app_write_flag;

typedef enum
{
	APP_STATE_OFF,
	APP_STATE_STANDBY,
	APP_STATE_LE_CONNECTED,
	APP_STATE_LE_OPEN,
	SPP_STATE_TOTAL
}app_state_type;

typedef struct App_List
{
	u8 outReadIndex;
	u8 outWriteIndex;
	u8 outUsedNum;
	u8 inReadIndex;
	u8 inWriteIndex;
	u8 inUsedNum;
	u8 appState;
	u16 attMtu;
	u8 connHandle;
	u8 role;
	u8 creditBasedEnable;
	u8 credit;
	u8 queryIndex;
	u8 writeFlag;
	u8 disableReceive;
	u8 updateConnPara;
}App_List;


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

#endif