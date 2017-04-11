/********************************************************************
  File Information:
    FileName:       ble_gatt.h
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
 * @addtogroup BLE_GATT BLE GATT
 * @{
 * @brief Definitions and prototypes for the BLE GATT stack layer application programming interface.
 */
#ifndef BLE_GATT_H
#define BLE_GATT_H

/**@defgroup BLE_GATT_READ_TYPES BLE_GATT_READ_TYPES
 * @brief GATT read type definitions
 * @{ */
#define BLE_GATT_TYPE_READ_REQ				0x01	/**< Read Request.*/
#define BLE_GATT_TYPE_READ_BLOB_REQ			0x02	/**< Read Blob Request.*/
/** @} */

/**@defgroup BLE_GATT_WRITE_TYPES BLE_GATT_WRITE_TYPES
 * @brief GATT write type definitions
 * @{ */
#define BLE_GATT_TYPE_WRITE_REQ				0x01	/**< Write Request.*/
#define BLE_GATT_TYPE_WRITE_CMD				0x02	/**< Write Command.*/
#define BLE_GATT_TYPE_SIGN_WRITE_CMD		0x03	/**< Signed Write Command.*/
#define BLE_GATT_TYPE_PREP_WRITE_REQ		0x04	/**< Prepare Write Request.*/
#define BLE_GATT_TYPE_EXEC_WRITE_REQ		0x05	/**< Execute Write Request.*/
/** @} */

/**@defgroup BLE_GATT_EXEC_WRITE_FLAGS BLE_GATT_EXEC_WRITE_FLAGS
 * @brief GATT flags for Execute Write Request
 * @{ */
#define BLE_GATT_EXEC_WRITE_FLAG_CANCEL_ALL		0x00	/**< Cancel all prepared writes.*/
#define BLE_GATT_EXEC_WRITE_FLAG_WRITE			0x01	/**< Immediately write all pending prepared values.*/
/** @} */

/**@defgroup BLE_GATT_READ_RESP_TYPES BLE_GATT_READ_RESP_TYPES
 * @brief GATT write response type definitions
 * @{ */
#define BLE_GATT_TYPE_READ_RESP				0x01	/**< Read Response.*/
#define BLE_GATT_TYPE_READ_BLOB_RESP		0x02	/**< Read Blob Response.*/
/** @} */

/**@defgroup BLE_GATT_WRITE_RESP_TYPES BLE_GATT_WRITE_RESP_TYPES
 * @brief GATT write response type definitions
 * @{ */
#define BLE_GATT_TYPE_WRITE_RESP			0x01	/**< Write Response.*/
#define BLE_GATT_TYPE_PREP_WRITE_RESP		0x02	/**< Prepare Write Response.*/
#define BLE_GATT_TYPE_EXEC_WRITE_RESP		0x03	/**< Execute Write Response.*/
/** @} */


/**@defgroup BLE_GATT_SEND_HV_TYPES BLE_GATT_SEND_HV_TYPES
 * @brief GATT Server Handle Value sending types
 * @{ */
#define BLE_GATT_HV_NOTIFICATION			0x01	/**< Notification.*/
#define BLE_GATT_HV_INDICATION				0x02	/**< Indication.*/
/** @} */

#define BLE_ATT_ATTRIBUTE_VALUE_LEN			23		/**< The Maximum Length of Attribute Value. Refer to ATT default MTU size. */
#define BLE_ATT_DEFAULT_MTU_LENGTH			23		/**< ATT default MTU size. */
#define BLE_ATT_MAX_MTU_LENGTH				160		/**< The Maximum supported MTU size of ATT stack. */
#define BLE_ATT_MAX_UUID_SIZE				16		/**< The Maximum UUID Length. 128-bit. */
#define BLE_ATT_HANDLE_VALUE_HEADER_SIZE	3		/**< The BLE ATT Handle Value Notification/Indication Header Size. */
#define BLE_ATT_WRITE_HEADER_SIZE			3		/**< The BLE ATT Write Request/Command Header Size. */
#define BLE_ATT_READ_RESP_HEADER_SIZE		1		/**< The BLE ATT Read Response Header Size. */
#define BLE_ATT_WRITE_RESP_HEADER_SIZE		5		/**< The BLE ATT Prepare Write Response Header Size. */


/**@defgroup BLE_ATT_CONFIGURATION_BITS BLE_ATT_CONFIGURATION_BITS
 * @brief BLE ATT configuration bits definitions
 * @{ */
#define BLE_ATT_CONFIG_MANUAL_CONFIRMATION	0x0001	/**< bit 0: Configure ATT stack auto/manual responds confirmation when receiving indication. */
/** @} */

/**@defgroup BLE_ATT_ERROR_CODES  BLE_ATT_ERROR_CODES
 * @brief ATT Error Codes
 * @{ */
#define BLE_ATT_ERRCODE_INVALID_HANDLE						0x01		/**< ATT Error Code: Invalid Handle.*/
#define BLE_ATT_ERRCODE_READ_NOT_PERMITTED					0x02		/**< ATT Error Code: Read Not Permitted.*/
#define BLE_ATT_ERRCODE_WRITE_NOT_PERMITTED					0x03		/**< ATT Error Code: Write Not Permitted.*/
#define BLE_ATT_ERRCODE_INVALID_PDU							0x04		/**< ATT Error Code: Invalid PDU.*/
#define BLE_ATT_ERRCODE_INSUFFICIENT_AUTHENTICATION			0x05		/**< ATT Error Code: Insufficient Authentication.*/
#define BLE_ATT_ERRCODE_REQUEST_NOT_SUPPORT					0x06		/**< ATT Error Code: Request Not Support.*/
#define BLE_ATT_ERRCODE_INVALID_OFFSET						0x07		/**< ATT Error Code: Invalid Offset.*/
#define BLE_ATT_ERRCODE_INSUFFICIENT_AUTHORIZATION			0x08		/**< ATT Error Code: Insufficient Authorization.*/
#define BLE_ATT_ERRCODE_PREPARE_QUEUE_FULL					0x09		/**< ATT Error Code: Prepare Queue Full.*/
#define BLE_ATT_ERRCODE_ATTRIBUTE_NOT_FOUND					0x0a		/**< ATT Error Code: Attribute Not Found.*/
#define BLE_ATT_ERRCODE_ATTRIBUTE_NOT_LONG					0x0b		/**< ATT Error Code: Attribute Not Long.*/
#define BLE_ATT_ERRCODE_INSUFFICIENT_ENCRYPTION_KEY_SIZE	0x0c		/**< ATT Error Code: Insufficient Encryption Key Size.*/
#define BLE_ATT_ERRCODE_INVALID_ATTRIBUTE_VALUE_LENGTH		0x0d		/**< ATT Error Code: Invalid Attribute Value Length.*/
#define BLE_ATT_ERRCODE_UNLIKELY_ERROR						0x0e		/**< ATT Error Code: Unlikely Error.*/
#define BLE_ATT_ERRCODE_INSUFFICIENT_ENCRYPTION				0x0f		/**< ATT Error Code: Insufficient Encryption.*/
#define BLE_ATT_ERRCODE_UNSUPPORTED_GROUP_TYPE				0x10		/**< ATT Error Code: Unsupported Group Type.*/
#define BLE_ATT_ERRCODE_INSUFFICIENT_RESOURCE				0x11		/**< ATT Error Code: Insufficient Resource.*/
#define BLE_ATT_ERRCODE_APPLICATION_ERROR					0x80		/**< ATT Error Code: Application Error.*/
/** @} */

/**@defgroup BLE_ATT_OPCODES	BLE_ATT_OPCODES
 * @brief ATT Attribute Opcodes
 * @{ */
#define ATT_ERROR_RESPONSE							0x01	/**< Attribute Opcode: Error Response.*/
#define ATT_EXCHANGE_MTU_REQUEST					0x02	/**< Attribute Opcode: Exchange MTU Request.*/
#define ATT_EXCHANGE_MTU_RESPONSE					0x03	/**< Attribute Opcode: Exchange MTU Response.*/
#define ATT_FIND_INFORMATION_REQUEST				0x04	/**< Attribute Opcode: Find Information Request.*/
#define ATT_FIND_INFORMATION_RESPONSE				0x05	/**< Attribute Opcode: Find Information Response.*/
#define ATT_FIND_BY_TYPE_VALUE_REQUEST				0x06	/**< Attribute Opcode: Find By Type Value Request.*/
#define ATT_FIND_BY_TYPE_VALUE_RESPONSE				0x07	/**< Attribute Opcode: Find By Type Value Response.*/
#define ATT_READ_BY_TYPE_REQUEST					0x08	/**< Attribute Opcode: Read By Type Request.*/
#define ATT_READ_BY_TYPE_RESPONSE					0x09	/**< Attribute Opcode: Read By Type Response.*/
#define ATT_READ_REQUEST							0x0a	/**< Attribute Opcode: Read Request.*/
#define ATT_READ_RESPONSE							0x0b	/**< Attribute Opcode: Read Response.*/
#define ATT_READ_BLOB_REQUEST						0x0c	/**< Attribute Opcode: Read Blob Request.*/
#define ATT_READ_BLOB_RESPONSE						0x0d	/**< Attribute Opcode: Read Blob Response.*/
#define ATT_READ_MULTIPLE_REQUEST					0x0e	/**< Attribute Opcode: Read Multiple Request.*/
#define ATT_READ_MULTIPLE_RESPONSE					0x0f	/**< Attribute Opcode: Read Multiple Response.*/
#define ATT_READ_BY_GROUP_TYPE_REQUEST				0x10	/**< Attribute Opcode: Read By Group Type Request.*/
#define ATT_READ_BY_GROUP_TYPE_RESPONSE				0x11	/**< Attribute Opcode: Read By Group Type Response.*/
#define ATT_WRITE_REQUEST							0x12	/**< Attribute Opcode: Write Request.*/
#define ATT_WRITE_RESPONSE							0x13	/**< Attribute Opcode: Write Response.*/
#define ATT_WRITE_COMMAND							0x52	/**< Attribute Opcode: Write Command.*/
#define ATT_PREPARE_WRITE_REQUEST					0x16	/**< Attribute Opcode: Prepare Write Request.*/
#define ATT_PREPARE_WRITE_RESPONSE					0x17	/**< Attribute Opcode: Prepare Write Response.*/
#define ATT_EXECUTE_WRITE_REQUEST					0x18	/**< Attribute Opcode: Execute Write Request.*/
#define ATT_EXECUTE_WRITE_RESPONSE					0x19	/**< Attribute Opcode: Execute Write Response.*/
#define ATT_HANDLE_VALUE_NOTIFICATION				0x1b	/**< Attribute Opcode: Handle Value Notification.*/
#define ATT_HANDLE_VALUE_INDICATION					0x1d	/**< Attribute Opcode: Handle Value Indication.*/
#define ATT_HANDLE_VALUE_CONFIRMATION				0x1e	/**< Attribute Opcode: Handle Value Confirmation.*/
#define ATT_SIGNED_WRITE_COMMAND					0xd2	/**< Attribute Opcode: Signed Write Command.*/
/** @} */

/**@defgroup ATT_PERMISSION_KEY_SIZE  ATT_PERMISSION_KEY_SIZE
 * @brief Attribute might requires permission(Encryption/Authentication Permission) to accessed.
 * 		  This key size defines the minimum length of the pairing key. Shorter than this length will be taken as un-safe authentication/encryption.
 *		  Length ranges from 7 bytes to 16 bytes. Set to zero to disable the key size checking.
 * @{ */
#define ATT_PERMISSION_KEY_SIZE						0x10
/** @} */

/**@defgroup GATT_PROCEDURE_STATUS  GATT_PROCEDURE_STATUS
 * @brief Some GATT procedures would not finish in one ATT request. Thus, more than one event will receive in one procedure.
 *		  Status in the event indicates that if this procedure will continue or end.
 * @{ */
#define GATT_PROCEDURE_STATUS_CONTINUE				0x00	/**< Querying procedure continues. Further events will come. */
#define GATT_PROCEDURE_STATUS_FINISH				0x01	/**< Querying procedure finishes. Last event of this procedure. */
/** @} */

/** 
 * @brief BLE GATT callback events.
 */
enum BLE_GATT_EventID
{
	BLE_GATTC_EVT_ERROR_RESP = 0x00,			/**< Error Response Event. Please see @ref BLE_ATT_EVT_Error_t. */
	BLE_GATTC_EVT_DISC_PRIM_SERV_RESP,			/**< Discover Primary Services Response Event. Please see @ref BLE_ATT_EVT_DiscPrimServResp_t. */
	BLE_GATTC_EVT_DISC_PRIM_SERV_BY_UUID_RESP,	/**< Discover Primary Services By UUID Response Event. Please see @ref BLE_ATT_EVT_DiscPrimServByUuidResp_t. */
	BLE_GATTC_EVT_DISC_CHAR_RESP,				/**< Discover All Characeristics Response Event. Please see @ref BLE_ATT_EVT_DiscCharResp_t. */
	BLE_GATTC_EVT_DISC_DESC_RESP,				/**< Discover All Descriptors Response Event. *Please see @ref BLE_ATT_EVT_DiscDescResp_t. */
	BLE_GATTC_EVT_READ_USING_UUID_RESP,			/**< Read Using UUID Response Event. Please see @ref BLE_ATT_EVT_ReadUsingUuidResp_t. */
	BLE_GATTC_EVT_READ_RESP,					/**< Read Response Event. Please see @ref BLE_ATT_EVT_ReadResp_t. */
	BLE_GATTC_EVT_WRITE_RESP,					/**< Write Response Event. Please see @ref BLE_ATT_EVT_WriteResp_t. */
	BLE_GATTC_EVT_HV_NOTIFY,					/**< Handle Value Notification Event. Please see @ref BLE_ATT_EVT_ReceiveHV_t. */
	BLE_GATTC_EVT_HV_INDICATE,					/**< Handle Value Indication Event. Please see @ref BLE_ATT_EVT_ReceiveHV_t. */
	BLE_GATTS_EVT_READ,							/**< Read Request Event. Please see @ref BLE_ATT_EVT_Read_t. */
	BLE_GATTS_EVT_WRITE,						/**< Write Request/Command Event. Please see @ref BLE_ATT_EVT_Write_t. */
	BLE_GATTS_EVT_HV_CONFIRM,					/**< Handle Value Confirmation Event. Please see @ref BLE_ATT_EVT_ReceiveConfirm_t. */
	BLE_GATTS_EVT_READ_BY_TYPE,					/**< Read By Type Request event. Please see @ref BLE_ATT_EVT_ReadByType_t. */
	BLE_ATT_EVT_TIMEOUT,						/**< ATT Stack Timeout Event. */
	BLE_ATT_EVT_UPDATE_MTU,						/**< Update MTU Event. Please see @ref BLE_ATT_EVT_UpdateMTU_t. */
};


/**@brief ATT Read By Type Request Parameters. */
typedef struct BLE_GATTC_ReadByTypeParams
{
	u16			startHandle;						/**< Starting handle for querying. */
	u16			endHandle;							/**< End handle for querying. */
	u8			attrTypeLength;						/**< Length of the attribute type. */
	u8			attrType[BLE_ATT_MAX_UUID_SIZE];	/**< Attribute Type */
}	BLE_GATTC_ReadByTypeParams;

/**@brief GATT Discover Primary Service By UUID Parameters. */
typedef struct BLE_GATTC_DiscoverPrimaryServiceByUuidParams
{
	u16			startHandle;
	u16			endHandle;
	u8			valueLength;
	u8			value[BLE_ATT_MAX_UUID_SIZE];
}	BLE_GATTC_DiscoverPrimaryServiceByUuidParams;

/**@brief GATT Write Request Parameters. */
typedef struct BLE_GATTC_WriteParams
{
	u16		charHandle;															/**< Handle of the characteristic to be written. */
	u16		charLength;															/**< Length of the characteristic to be written. */
	u8		charValue[BLE_ATT_MAX_MTU_LENGTH-BLE_ATT_WRITE_HEADER_SIZE];		/**< The written value array. */
	u8		writeType;															/**< Type of the write operation. See @ref BLE_GATT_WRITE_TYPES. */
	u16		valueOffset;														/**< The offset of the first octet to be written. */
	u8		flags;																/**< Flags. See @ref BLE_GATT_EXEC_WRITE_FLAGS. */
}	BLE_GATTC_WriteParams;

/**@brief GATT Handle Value Notification/Indication Parameters. */
typedef struct BLE_GATTS_HandleValueParams
{
	u16		charHandle;															/**< Handle of the characteristic to be notified/indicated. */
	u16		charLength;															/**< Length of the characteristic to be notified/indicated. */
	u8		charValue[BLE_ATT_MAX_MTU_LENGTH-BLE_ATT_HANDLE_VALUE_HEADER_SIZE];	/**< Pointer to the characteristic value to be notified/indicated. */
	u8		sendType;															/**< Notification of Indication. See @ref BLE_GATT_SEND_HV_TYPES. */
}	BLE_GATTS_HandleValueParams;

/**@brief GATT Send Error Response. */
typedef struct BLE_GATTS_SendErrResponseParams
{
	u8 reqOpcode;		/**< The request that generated this error response. */
	u16 charHandle;		/**< The attribute handle that generated this error response. */
	u8 errorCode;		/**< The reason why the request has generated an error response. See @ref BLE_ATT_ERROR_CODES. */
}	BLE_GATTS_SendErrResponseParams;

/**@brief GATT Send Read Response. */
typedef struct BLE_GATTS_SendReadResponseParams
{
	u8 responseType;														/**< The read response type. See @ref BLE_GATT_READ_RESP_TYPES. */
	u8 charValue[BLE_ATT_MAX_MTU_LENGTH-BLE_ATT_READ_RESP_HEADER_SIZE];		/**< The read value. */
	u16 charLength;															/**< Length of the read value. */
}	BLE_GATTS_SendReadResponseParams;

/**@brief GATT Send Write Response. */
typedef struct BLE_GATTS_SendWriteResponseParams
{
	u8 responseType;														/**< The write response type. See @ref BLE_GATT_WRITE_RESP_TYPES. */
	u16 charHandle;															/**< The handle of the attribute to be written. Use for Prepare Write Response. */
	u16 valueOffset;														/**< The offset of the first octet to be written. Use for Prepare Write Response. */
	u16 charLength;															/**< The length of the written value. Use for Prepare Write Response. */
	u8 charValue[BLE_ATT_MAX_MTU_LENGTH-BLE_ATT_WRITE_RESP_HEADER_SIZE];	/**< The value of the attribute to be written. Use for Prepare Write Response. */
}	BLE_GATTS_SendWriteResponseParams;

/**@brief GATT Send Read By Type Response. */
typedef struct BLE_GATTS_SendReadByTypeResponseParams
{
	u8 pairLength;															/**< Length of one handle-value pair. Read By Type Response is made by several handle-value pair. */
	u16 allPairsLength;														/**< Length of all handle-value pairs data. */
	u8 charValue[BLE_ATT_MAX_MTU_LENGTH-BLE_ATT_WRITE_RESP_HEADER_SIZE];	/**< The value of handle-value pairs to be read. If the attribute length is longer than ATT MTU, only the first MTU size bytes can be sent. */
}	BLE_GATTS_SendReadByTypeResponseParams;

/**@brief The services information of the Application definded services */
typedef struct BLE_GATT_ServicesInfo
{
	u8	XDATA	*p_definedService;	/**< The address of the Application defined Service. */
	u8			attributeNum;		/**< Total attribute number of the Service. */
	u16			serviceSize;		/**< The Service size in bytes */
}	BLE_GATT_ServicesInfo;

/**@brief GATT EVENT Error Response Parameters. */
typedef struct BLE_ATT_EVT_Error_t{
	u8			connHandle;	/**< Connection Handle. */
	u8			reqOpcode;	/**< The request that generated this error response. */
	u16			attrHandle;	/**< The attribute handle that generated this error response. */
	u8			errCode;	/**< The reason why the request has generated an error response. */
}	BLE_ATT_EVT_Error_t;

/**@brief GATT EVENT Read Request Parameters. */
typedef struct BLE_ATT_EVT_Read_t{
	u8			connHandle;	/**< Connection Handle. */
	u16			charHandle;	/**< The handle of the attribute to be read. */
	u8			readType;	/**< Read Type. See @ref BLE_GATT_READ_TYPES. */
	u16			readOffset;	/**< The offset of the first octet to be read. */
}	BLE_ATT_EVT_Read_t;

/**@brief GATT EVENT Write Request Parameters. */
typedef struct BLE_ATT_EVT_Write_t{
	u8 			connHandle;			/**< Connection Handle. */
	u16			charHandle;			/**< The handle of the attribute to be written. */
	u8			writeType;			/**< Write Type. */
	u16			valueOffset;		/**< Value offset for Prepare Write Request. */
	u8			flags;				/**< Flags for Execute Write Request. */
	u16			writeDataLength;	/**< Length of the written data. */
	u8 	XDATA 	*p_charValue;		/**< The value to be written to the attribute. */
}	BLE_ATT_EVT_Write_t;

/**@brief GATT EVENT Discover All Primary Services Response Parameters. */
typedef struct BLE_ATT_EVT_DiscPrimServResp_t{
	u8 			connHandle;			/**< Connection Handle. */
	u8			attrPairLength;		/**< Length of each attribute data in the list. */
	u16			attrDataLength;		/**< Length of all attributes data. */
	u8	XDATA	*p_attrData;		/**< The attribute data list. */
	u8			procedureStatus;	/**< Discover All Primary Services might not finish in one ATT request. Status notes that if further events will come. Please see @ref GATT_PROCEDURE_STATUS. */
}	BLE_ATT_EVT_DiscPrimServResp_t;

/**@brief GATT EVENT Discover Primary Service By UUID Response Parameters. */
typedef struct BLE_ATT_EVT_DiscPrimServByUuidResp_t{
	u8 			connHandle;			/**< Connection Handle. */
	u16			handleInfoLength;	/**< Length of the handles information list. */
	u8	XDATA	*p_handleInfo;		/**< The handles information list. */
	u8			procedureStatus;	/**< Discover Primary Service By UUID might not finish in one ATT request. Status notes that if further events will come. Please see @ref GATT_PROCEDURE_STATUS. */
}	BLE_ATT_EVT_DiscPrimServByUuidResp_t;

/**@brief GATT EVENT Discover All Characteristics Response Parameters. */
typedef struct BLE_ATT_EVT_DiscCharResp_t{
	u8			connHandle;			/**< Connection Handle. */
	u8			attrPairLength;		/**< Length of each attribute data in the list. */
	u16			attrDataLength;		/**< Length of all attributes data. */
	u8	XDATA	*p_attrData;		/**< The attribute data list. Each data consists of handle-value pair. */
	u8			procedureStatus;	/**< Discover All Characteristics might not finish in one ATT request. Status notes that if further events will come. Please see @ref GATT_PROCEDURE_STATUS. */
}	BLE_ATT_EVT_DiscCharResp_t;

/**@brief GATT EVENT Discover All Descriptors Response Parameters. */
typedef struct BLE_ATT_EVT_DiscDescResp_t{
	u8			connHandle;			/**< Connection Handle. */
	u8			infoDataFormat;		/**< Format of the received information Data. */
	u16			infoDataLength;		/**< Length of the all received information Data. */
	u8	XDATA	*p_infoData;		/**< Information Date. */
	u8			procedureStatus;	/**< Discover All Descriptors might not finish in one ATT request. Status notes that if further events will come. Please see @ref GATT_PROCEDURE_STATUS. */
}	BLE_ATT_EVT_DiscDescResp_t;

/**@brief GATT EVENT Read Using UUID Response Parameters. */
typedef struct BLE_ATT_EVT_ReadUsingUuidResp_t{
	u8			connHandle;			/**< Connection Handle. */
	u8			attrPairLength;		/**< Length of each attribute data in the list. */
	u16			attrDataLength;		/**< Length of all attributes data. */
	u8	XDATA	*p_attrData;		/**< The attribute data list. */
	u8			procedureStatus;	/**< Read Using UUID might not finish in one ATT request. Status notes that if further events will come. Please see @ref GATT_PROCEDURE_STATUS. */
}	BLE_ATT_EVT_ReadUsingUuidResp_t;

/**@brief GATT EVENT Read Response Parameters. */
typedef struct BLE_ATT_EVT_ReadResp_t{
	u8			connHandle;			/**< Connection Handle. */
	u8			responseType;		/**< The read response type. See @ref BLE_GATT_READ_RESP_TYPES. */
	u16			attrDataLength;		/**< Length of the read data. */
	u8	XDATA	*p_readValue;		/**< The read attribute value. */
}	BLE_ATT_EVT_ReadResp_t;

/**@brief GATT EVENT Write Response Parameters. */
typedef struct BLE_ATT_EVT_WriteResp_t{
	u8			connHandle;			/**< Connection Handle. */
	u8			responseType;		/**< The write response type. See @ref BLE_GATT_WRITE_RESP_TYPES. */
	u16			charHandle;			/**< The write handle in the prepare write response. */
	u16			valueOffset;		/**< The write offset in the prepare write response. */
	u16			writeLength;		/**< The length of written value in the prepare write response. */
	u8	XDATA	*p_writeValue;		/**< The value to be written in the prepare write response. */
}	BLE_ATT_EVT_WriteResp_t;

/**@brief GATT EVENT Updated MTU Parameters. */
typedef struct BLE_ATT_EVT_UpdateMTU_t{
	u8			connHandle;			/**< Connection Handle. */
	u16			exchangedMTU;		/**< The exchanged MTU after connection. */
}	BLE_ATT_EVT_UpdateMTU_t;

/**@brief GATT EVENT Receive Handle Value Notification or Indication Parameters. */
typedef struct BLE_ATT_EVT_ReceiveHV_t{
	u8			connHandle;			/**< Connection Handle. */
	u16			charHandle;			/**< The handle of the attribute which is notified/indicated from. */
	u16			receivedLength;		/**< The length of received notification/indication. */
	u8	XDATA	*p_receivedValue;	/**< The received value from Notification/Indication. */
}	BLE_ATT_EVT_ReceiveHV_t;

/**@brief GATT EVENT Receive Handle Value Confirmation Parameters. */
typedef struct BLE_ATT_EVT_ReceiveConfirm_t{
	u8			connHandle;			/**< Connection Handle. */
}	BLE_ATT_EVT_ReceiveConfirm_t;

/**@brief GATT EVENT Receive Read By Type Request Parameters. */
typedef struct BLE_ATT_EVT_ReadByType_t{
	u8 			connHandle;			/**< Connection Handle. */
	u16			charHandle;			/**< Characteristic Handle of the request attribute. */
}	BLE_ATT_EVT_ReadByType_t;

/**
 * @brief The Union Structure of All Events Structures.
 */
typedef union 
{
	BLE_ATT_EVT_Error_t						onError;					/**< Pointer to BLE_ATT_EVT_Error_t structure. */
	BLE_ATT_EVT_Read_t						onRead;						/**< Pointer to BLE_ATT_EVT_Read_t structure. */
	BLE_ATT_EVT_Write_t						onWrite;					/**< Pointer to BLE_ATT_EVT_Write_t structure. */
	BLE_ATT_EVT_DiscPrimServResp_t			onDiscPrimServResp;			/**< Pointer to BLE_ATT_EVT_DiscPrimServResp_t structure. */
	BLE_ATT_EVT_DiscPrimServByUuidResp_t	onDiscPrimServByUuidResp;	/**< Pointer to BLE_ATT_EVT_DiscPrimServByUuidResp_t structure. */
	BLE_ATT_EVT_DiscCharResp_t				onDiscCharResp;				/**< Pointer to BLE_ATT_EVT_DiscCharResp_t structure. */
	BLE_ATT_EVT_DiscDescResp_t				onDiscDescResp;				/**< Pointer to BLE_ATT_EVT_DiscDescResp_t structure. */
	BLE_ATT_EVT_ReadUsingUuidResp_t			onReadUsingUuidResp;		/**< Pointer to BLE_ATT_EVT_ReadUsingUuidResp_t structure. */
	BLE_ATT_EVT_ReadResp_t					onReadResp;					/**< Pointer to BLE_ATT_EVT_ReadResp_t structure. */
	BLE_ATT_EVT_WriteResp_t					onWriteResp;				/**< Pointer to BLE_ATT_EVT_WriteResp_t structure. */
	BLE_ATT_EVT_UpdateMTU_t					onUpdateMTU;				/**< Pointer to BLE_ATT_EVT_UpdateMTU_t structure. */
	BLE_ATT_EVT_ReceiveHV_t					onNotification;				/**< Pointer to BLE_ATT_EVT_ReceiveHV_t structure. */
	BLE_ATT_EVT_ReceiveHV_t					onIndication;				/**< Pointer to BLE_ATT_EVT_ReceiveHV_t structure. */
	BLE_ATT_EVT_ReceiveConfirm_t			onConfirmation;				/**< Pointer to BLE_ATT_EVT_ReceiveConfirm_t structure. */
	BLE_ATT_EVT_ReadByType_t				onReadByType;				/**< Pointer to BLE_ATT_EVT_ReadByType_t structure. */
}BLE_GATTEventField;

/**
 * @brief GATT callback function parameters.
 */
typedef struct	BLE_GATT_Event
{
	u8	eventID;											/**< Event ID. See @ref BLE_GATT_EventID */
	BLE_GATTEventField		eventField;						/**< Data field of this event. */
}BLE_GATT_Event;

/**
 * @brief BLE GATT callback function prototype.
 */
typedef void(*GATTProcessRoutine)(BLE_GATT_Event XDATA *p_event) large;
 
/**
 *@brief Initialize GATT
 *
 *@param[in] gattHandler		Function to called if there's gatt event.
 *
 *@return MBIOT_RES_SUCCESS GATT initialize successfully.
 */
PUBLIC extern u16 BLE_GATT_Init(GATTProcessRoutine gattHandler) large;

/**
 *@brief Configure ATT stack behavior.
 *
 *@param[in] configuration		Bitfield representing each configuration. Bit definitions refer to @ref BLE_ATT_CONFIGURATION_BITS
 *
 *@return MBIOT_RES_SUCCESS Configuration successfully.
 *@return MBIOT_RES_FAIL Operation is not permitted.
 */
PUBLIC extern u16 BLE_GATT_Configure(u16 configuration) large;

/**
 *@brief GATT Server starts the procedure to init Service Table
 *
 *@return MBIOT_RES_SUCCESS Successfully start the creating Service Table Procedure.
 *@return MBIOT_RES_OOM Out Of Memory.
 */
PUBLIC extern u16 BLE_GATTS_StartCreatingServiceTable() large;

/**
 *@brief GATT Server ends the procedure to init Service Table
 *
 *@return MBIOT_RES_SUCCESS Successfully create the whole Service Table.
 *@return MBIOT_RES_FAIL Operation is not permitted.
 */
PUBLIC extern u16 BLE_GATTS_EndCreatingServiceTable() large;

/**
 *@brief GATT Server initialize the GATT Service Table
 *
 *@note This function must be called between BLE_GATTS_StartCreatingServiceTable() and BLE_GATTS_EndCreatingServiceTable().
 *
 *@param[in] p_serviceInfo			Pointer to the information of registered Service.
 *@param[out] p_startHandle			Pointer to the starting handle of the registered Service.
 *
 *@return MBIOT_RES_SUCCESS Successfully initialize all information of the registered Service.
 *@return MBIOT_RES_FAIL Operation is not permitted.
 */
PUBLIC extern u16 BLE_GATTS_InitServiceTable(BLE_GATT_ServicesInfo XDATA *p_serviceInfo, u16 XDATA *p_startHandle) large;

/**
 *@brief GATT Client initiates a read operation to GATT Server
 *
 *@param[in] connHandle		Handle of the connection to send read request.
 *@param[in] charHandle		Handle of the characteristic to be read.
 *@param[in] valueOffset	None-zero offset value to initiate a read blob request.
 *
 *@return MBIOT_RES_SUCCESS Successfully starts the read operation.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 */
PUBLIC extern u16 BLE_GATTC_Read(u8 connHandle, u16 charHandle, u16 valueOffset) large;

/**
 *@brief GATT Client initiates a write operation to GATT Server
 *
 *@param[in] connHandle		Handle of the connection to send write operation.
 *@param[in] p_writeParams	Pointer to the Write Request parameters.
 *
 *@return MBIOT_RES_SUCCESS Successfully starts the write operation.
 *@return MBIOT_RES_FAIL Operation is not permitted.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 */
PUBLIC extern u16 BLE_GATTC_Write(u8 connHandle, BLE_GATTC_WriteParams XDATA *p_writeParams) large;

/**
 *@brief GATT Client sends Handle Value Confirmation for receiving a Handle Value Indication
 *
 *@param[in] connHandle		Handle of the connection to send Handle Value Confirmation.
 *
 *@return MBIOT_RES_SUCCESS Successfully starts the Handle Value Confirmation.
 *@return MBIOT_RES_FAIL Operation is not permitted.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 */
PUBLIC extern u16 BLE_GATTC_HandleValueConfirm(u8 connHandle) large;

/**
 *@brief GATT Client initialize the discovering all primary services procedure. Two possible response events will come for this procedure.
 *		 Receive BLE_GATTC_EVT_DISC_PRIM_SERV_RESP if service is discovered. Status flag indicates if more events are coming or procedure end.
 *		 Receive BLE_GATTC_EVT_ERROR_RESP when no primary service is found. The procedure also ends.
 *
 *@param[in] connHandle		Handle of the connection to discover the services.
 *
 *@return MBIOT_RES_SUCCESS Successfully starts the discovering.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 */
PUBLIC extern u16 BLE_GATTC_DiscoverAllPrimaryServices(u8 connHandle) large;

/**
 *@brief GATT Client initialize the discovering primary service by uuid procedure with the given handle range. Two possible response events will come for this procedure.
 *		 Receive BLE_GATTC_EVT_DISC_PRIM_SERV_BY_UUID_RESP if service is discovered. Status flag indicates if more events are coming or procedure end.
 *		 Receive BLE_GATTC_EVT_ERROR_RESP when no matched service is found. The procedure also ends.
 *
 *@param[in] connHandle		Handle of the connection to discover the services.
 *@param[in] discParams		Pointer to the Discover Primary Service by UUID parameter structure.
 *
 *@return MBIOT_RES_SUCCESS Successfully starts the discovering.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 */
PUBLIC extern u16 BLE_GATTC_DiscoverPrimaryServiceByUUID(u8 connHandle, BLE_GATTC_DiscoverPrimaryServiceByUuidParams XDATA *p_discParams) large;

/**
 *@brief GATT Client initialize the discovering all characteristics procedure with the given handle range. Two possible response events will come for this procedure.
 *		 Receive BLE_GATTC_EVT_DISC_CHAR_RESP if characteristics are discovered. Status flag indicates if more events are coming or procedure end.
 *		 Receive BLE_GATTC_EVT_ERROR_RESP when no characteristic is found. The procedure also ends.
 *
 *@param[in] connHandle		Handle of the connection to discover the services.
 *@param[in] startHandle	Start of the querying handle range.
 *@param[in] endHandle		End of the querying handle range.
 *
 *@return MBIOT_RES_SUCCESS Successfully starts the discovering.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 */
PUBLIC extern u16 BLE_GATTC_DiscoverAllCharacteristics(u8 connHandle, u16 startHandle, u16 endHandle) large;

/**
 *@brief GATT Client initialize the discovering all descriptors procedure with the given handle range. Two possible response events will come for this procedure.
 *		 Receive BLE_GATTC_EVT_DISC_DESC_RESP if matched descriptors are discovered. Status flag indicates if more events are coming or procedure end.
 *		 Receive BLE_GATTC_EVT_ERROR_RESP when no descriptors is found. The procedure also ends.
 *
 *@param[in] connHandle		Handle of the connection to discover the services.
 *@param[in] startHandle	Start of the querying handle range.
 *@param[in] endHandle		End of the querying handle range.
 *
 *@return MBIOT_RES_SUCCESS Successfully starts the discovering.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 */
PUBLIC extern u16 BLE_GATTC_DiscoverAllDescriptors(u8 connHandle, u16 startHandle, u16 endHandle) large;

/**
 *@brief GATT Client Read Characteristic Value using UUID. Two possible response events will come for this procedure.
 *		 Receive BLE_GATTC_EVT_READ_USING_UUID_RESP if characteristic value is returned. Status flag indicates if more events are coming or procedure end.
 *		 Receive BLE_GATTC_EVT_ERROR_RESP when no matched characteristic is found. The procedure also ends.
 *
 *@param[in] connHandle		Handle of the connection to read characteristic value.
 *@param[in] p_readParams	Pointer to Read Using UUID Parameters.
 *
 *@return MBIOT_RES_SUCCESS Successfully starts the discovering.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 */
PUBLIC extern u16 BLE_GATTC_ReadUsingUUID(u8 connHandle, BLE_GATTC_ReadByTypeParams XDATA *p_readParams) large;

/**
 *@brief GATT Server sends Handle Value Notification or Indication to GATT Client
 *
 *@param[in] connHandle		Handle of the connection to send Handle Value Notification/Indication.
 *@param[in] p_hvParams		Pointer to the Handle Value Notification/Indication Parameters.
 *
 *@return MBIOT_RES_SUCCESS Successfully starts the Handle Value Notification/Indication
 *@return MBIOT_RES_FAIL Operation is not permitted.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 */
PUBLIC extern u16 BLE_GATTS_SendHandleValue(u8 connHandle, BLE_GATTS_HandleValueParams XDATA *p_hvParams) large;

/**
 *@brief GATT Server sets the value of the characteristic
 *
 *@param[in] charHandle		Handle of the characteristic to be set.
 *@param[in] charLength		Length of the characteristic to be set.
 *@param[in] p_charValue	Pointer to the value to be set to the characteristic.
 *
 *@return MBIOT_RES_SUCCESS Successfully sets the characteristic value.
 *@return MBIOT_RES_FAIL Operation is not permitted.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 */
PUBLIC extern u16 BLE_GATTS_SetHandleValue(u16 charHandle, u16 charLength, u8 XDATA *p_charValue) large;

/**
 *@brief GATT Server gets the value of the characteristic
 *
 *@param[in] charHandle			Handle of the characteristic to be get.
 *@param[in,out] p_charLength	Pointer to length of the characteristic value.
 *@param[in,out] p_charValue	Pointer to the buffer to store the read value.
 *
 *@return MBIOT_RES_SUCCESS Successfully gets the characteristic value.
 *@return MBIOT_RES_FAIL Operation is not permitted.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 */
PUBLIC extern u16 BLE_GATTS_GetHandleValue(u16 charHandle, u8 XDATA *p_charLength, u8 XDATA *p_charValue) large;

/**
 *@brief GATT Server sends the read response
 *
 *@param[in] connHandle		Handle of the connection to send the read response.
 *@param[in] p_respParams	Pointer to Read Response parameters.
 *
 *@return MBIOT_RES_SUCCESS Successfully starts to send the read response.
 *@return MBIOT_RES_FAIL Operation is not permitted.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 */
PUBLIC extern u16 BLE_GATTS_SendReadResponse(u8 connHandle, BLE_GATTS_SendReadResponseParams XDATA *p_respParams) large;

/**
 *@brief GATT Server sends the write response
 *
 *@param[in] connHandle		Handle of the connection to send the write response.
 *@param[in] p_respParams	Pointer to Write Response Parameters.
 *
 *@return MBIOT_RES_SUCCESS Successfully starts to send the write response.
 *@return MBIOT_RES_FAIL Operation is not permitted.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 */
PUBLIC extern u16 BLE_GATTS_SendWriteResponse(u8 connHandle, BLE_GATTS_SendWriteResponseParams XDATA *p_respParams) large;

/**
 *@brief GATT Server sends the error response. This API can be called after Application receives Write Request event.
 *
 *@param[in] connHandle		Handle of the connection to send the error response.
 *@param[in] p_errParams	Pointer to Error Response parameters.
 *
 *@return MBIOT_RES_SUCCESS Successfully starts to send the error response.
 *@return MBIOT_RES_FAIL Operation is not permitted.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 */
PUBLIC extern u16 BLE_GATTS_SendErrorResponse(u8 connHandle, BLE_GATTS_SendErrResponseParams XDATA *p_errParams) large;

/**
 *@brief GATT Server sends the Read By Type response. This API can be called after Application receives Read By Type Request event.
 *
 *@param[in] connHandle		Handle of the connection to send the read by type response.
 *@param[in] p_readParams	Pointer to Read By Type Response parameters.
 *
 *@return MBIOT_RES_SUCCESS Successfully starts to send the read by type response.
 *@return MBIOT_RES_FAIL Operation is not permitted.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 */
PUBLIC extern u16 BLE_GATTS_SendReadByTypeResponse(u8 connHandle, BLE_GATTS_SendReadByTypeResponseParams XDATA *p_readParams) large;

#endif

/**
  @}
 */