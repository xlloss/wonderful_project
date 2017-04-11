/********************************************************************
  File Information:
    FileName:       ble_mesh.h
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
  @addtogroup MESH MESH
  @{
  @brief Definitions and prototypes for the MESH application programming interface.
 */
 
#ifndef BLE_MESH_H
#define BLE_MESH_H


/**
 * @defgroup MESH_PB_TYPE_DEF MESH_PB_TYPE_DEF
 * @brief The definition of Mesh provision type.
 * @{ */
#define PB_TYPE_ADV									0		/**< Provisioning by using ADV bearer */
#define PB_TYPE_GATT								1		/**< Provisioning by using GATT bearer */
/** @} */


/**
 * @defgroup MESH_PROVISIONING_STATUS_DEF MESH_PROVISIONING_STATUS_DEF
 * @brief The definition of Mesh provision status.
 * @{ */
#define MESH_PROVISIONING_SUCCESS					0		/**< Provisioning successfully */
#define MESH_PROVISIONING_FAIL						1		/**< Provisioning fail */
/** @} */


/**
 * @defgroup MESH_PROVISIONING_OPERATOR_DEF MESH_PROVISIONING_OPERATOR_DEF
 * @brief The definition of Mesh provision operator.
 * @{ */
#define MESH_OPERATOR_STOP							0		/**< Stop provisioning */
#define MESH_OPERATOR_START							1		/**< Start provisioning */
/** @} */


/**
 * @defgroup MESH_RELAY_STATE_DEF MESH_RELAY_STATE_DEF
 * @brief The definition of Mesh relay state.
 * @{ */
#define MESH_REALY_STATE_DISABLE					0		/**< The node is not relaying */
#define MESH_REALY_STATE_ENABLE						1		/**< The node is relaying */
/** @} */


/**@brief Mesh configuration ID. */
enum Mesh_Config_ID
{
	MESH_CONFIG_RESET_TO_DEFAULT,							/**< Reset node to default */
	MESH_CONFIG_STATIC_PROVISIOINED,						/**< Configure node keys manually*/
	MESH_CONFIG_PACKET_ADV_PARAMS,							/**< Configure node advertising parameters */
	MESH_CONFIG_PACKET_SCAN_PARAMS,							/**< Configure node scan parameters */
	MESH_CONFIG_NODE_PARAMS									/**< Configure node parameters*/
};


/**
 * @defgroup MESH_APP_KEY_NUM MESH_APP_KEY_NUM
 * @{ */
#define MAX_APPLICATION_KEY_NUM						2		/**< The maximum set of application key*/
/** @} */


/**
 * @defgroup MESH_GROUP_NUM MESH_GROUP_NUM
 * @{ */
#define MAX_GROUP_NUM								8		/**< The maximum set of group*/
/** @} */


/**@brief Application payload type. */
enum Mesh_App_Payload_Type
{
	APP_PAYLOAD_TYPE_1,										/**< 1-octet Opcode payload type */
	APP_PAYLOAD_TYPE_2,										/**< 2-octet Opcode payload type*/
	APP_PAYLOAD_TYPE_3										/**< 3-octet Opcode payload type*/
};


/**
 * @defgroup MESH_PARAMS_LEN_DEF MESH_PARAMS_LEN_DEF
 * @{ */
#define MAX_PARAMETERS_LEN_OPCODE_1					7		/**< Maximum length of 1-octet Opcode payload  */
#define MAX_PARAMETERS_LEN_OPCODE_2					6		/**< Maximum length of 2-octet Opcode payload  */
#define MAX_PARAMETERS_LEN_OPCODE_3					5		/**< Maximum length of 3-octet Opcode payload  */
/** @} */


/**@brief Mesh call back events. */
enum Mesh_Event_ID
{
	BLE_MESH_EVT_RCV_DATA = 0x00,							/**< Mesh data was received, see @ref MESH_EVT_Rcv_Data*/
	BLE_MESH_EVT_PROVISIONING_COMPLETE,						/**< Provisioning procedure was complete, see @ref MESH_EVT_ProvisioningComplete */
	BLE_MESH_EVT_NEW_NODES,									/**< Unprovisioned device, see @ref MESH_EVT_New_Nodes */
	BLE_MESH_EVT_UPDATE_PUBLICATION,						/**< Public address was updated, see @ref MESH_EVT_Update_Publication */
	BLE_MESH_EVT_UPDATE_SUBSCRIPTION,						/**< Subscription list was updated, see @ref MESH_EVT_Update_Subscription */
	BLE_MESH_EVT_UPDATE_DEFAULT_TTL,						/**< Default TTL was updated, see @ref MESH_EVT_Update_Default_TTL */
	BLE_MESH_EVT_UPDATE_RELAY_STATE,						/**< Relay state was updated, see @ref MESH_EVT_Update_Relay */
	BLE_MESH_EVT_NODE_RESET									/**< Node reset to default */
};

/**@brief Mesh Node States Type. */
enum Mesh_Node_States_Type
{
	BLE_MESH_STATE_UNPROVISIOINED = 0x00,					/**< Mesh Node is Unprovisioned */
	BLE_MESH_STATE_PROVISIONING,							/**< Mesh Node is Provisioning */
	BLE_MESH_STATE_OPERATIONAL,								/**< Mesh Node is Operational */
};

/**@brief The structure of application payload with 1-octect Opcodes*/
typedef struct MESH_OneOpcode_Data
{
    u8	opcode;												/**< 1-octet Opcodes */
    u8	paraLen;											/**< Length of application parameters */
    u8	parameters[MAX_PARAMETERS_LEN_OPCODE_1];			/**< Application parameters */
} MESH_OneOpcode_Data;


/**@brief The structure of application payload with 2-octect Opcodes*/
typedef struct MESH_TwoOpcode_Data
{
    u16	opcode;												/**< 2-octet Opcodes */
    u8	paraLen;											/**< Length of application parameters */
    u8	parameters[MAX_PARAMETERS_LEN_OPCODE_2];			/**< Application parameters */
} MESH_TwoOpcode_Data;


/**@brief The structure of application payload with 3-octect Opcodes*/
typedef struct MESH_ThreeOpcode_Data
{
    u8	opcode[3];											/**< 3-octet Opcodes */
    u8	paraLen;											/**< Length of application parameters */
    u8	parameters[MAX_PARAMETERS_LEN_OPCODE_3];			/**< Application parameters */
} MESH_ThreeOpcode_Data;


/**@brief Structure of APP PERIPHERAL events. */
typedef union
{
    MESH_OneOpcode_Data			oneOpcodeData;				/**< Structure of application payload with 1-octect Opcodes */
    MESH_TwoOpcode_Data			twoOpcodeData;				/**< Structure of application payload with 2-octect Opcodes */
    MESH_ThreeOpcode_Data		threeOpcodeData;			/**< Structure of application payload with 3-octect Opcodes */	
} MESH_ApplicationPayload;


/**@brief The structure of Mesh data was received event parameters.*/
typedef struct MESH_EVT_Rcv_Data{
	u16			dstAddr;									/**< Destination address */
	u16			srcAddr;									/**< Source address */
	u8			appInstance;								/**< Instance of application */
    u8			appPayloadType;								/**< Application payload type */
    MESH_ApplicationPayload		appPayload;					/**< Application payload*/

}MESH_EVT_Rcv_Data;


/**@brief The structure of Provision procedure was complete event parameters.*/
typedef struct MESH_EVT_ProvisioningComplete{
	u8 status;												/**< Procision status, see @ref MESH_PROVISIONING_STATUS_DEF */
	u16 nodeAddr;											/**< Assigned node address */
}MESH_EVT_ProvisioningComplete;


/**@brief The structure of Un-provisioned device event parameters.*/
typedef struct MESH_EVT_New_Nodes{
	u8 deviceUuid[16];										/**< Device UUID */
	u8 pbTypeSupported;										/**< Supported provision type, see @ref MESH_PB_TYPE_DEF*/
	BLE_GAP_Addr	gapAddr;								/**< Device Bluetooth address */
}MESH_EVT_New_Nodes;


/**@brief The structure of Public address was updated event parameters.*/
typedef struct MESH_EVT_Update_Publication{
	u8	defaultTtl;											/**< Default TTL */
	u16	publishAddress;										/**< Publish address */
}MESH_EVT_Update_Publication;


/**@brief The structure of Subscription list was updated event parameters.*/
typedef struct MESH_EVT_Update_Subscription{
	u16 groupAddr[MAX_GROUP_NUM];							/**< List of group address, only non-zero address is valid */
}MESH_EVT_Update_Subscription;


/**@brief The structure of Default TTL was updated was updated event parameters.*/
typedef struct MESH_EVT_Update_Default_TTL{
	u8	defaultTtl;											/**< Default TTL */
}MESH_EVT_Update_Default_TTL;


/**@brief The structure of Relay state was updated event parameters.*/
typedef struct MESH_EVT_Update_Relay{
	u8  relayState;											/**< Relay state, see @ref MESH_RELAY_STATE_DEF*/
}MESH_EVT_Update_Relay;


/**@brief MESH event types */
typedef union 
{
	MESH_EVT_Rcv_Data				onMeshRcvData;						/**< Data field of Mesh data was received event. */
	MESH_EVT_ProvisioningComplete	onMeshProvisioningComplete;			/**< Data field of Provision procedure was complete event. */
	MESH_EVT_New_Nodes				onMeshNewNodes;						/**< Data field of Un-provisioned device event. */
	MESH_EVT_Update_Publication		onMeshUpdatePublication;			/**< Data field of Public address was updated event. */
	MESH_EVT_Update_Subscription	onMeshUpdateSubscription;			/**< Data field of Subscription list was updated event. */
	MESH_EVT_Update_Default_TTL		onMeshUpdateDefaultTTL;				/**< Data field of Default TTL was updated event. */
	MESH_EVT_Update_Relay			onMeshUpdateRelayStatus;			/**< Data field of Relay state was updated event. */

}MESHEventField;


/**@brief Structure of MESH events. */
typedef struct	Ble_Mesh_Event
{
	u8	eventID;											/**< Event ID. See @ref Mesh_Event_ID. */
	MESHEventField		eventField;							/**< Data field of this event. */	
}Ble_Mesh_Event;


/**@brief MESH callback function prototype. */
typedef void(*MESHProcessRoutine)( Ble_Mesh_Event XDATA *p_event) large;


/**@brief The structrue of transmit data */
typedef struct MESH_Transmit_Data
{
    u8	appInstance;										/**< Instance of application */
    u8	ttl;												/**< TTL */
    u16	dstAddr;											/**< Destination address */
    u8	appPayloadType;										/**< Payload type */
    MESH_ApplicationPayload		appPayload;					/**< Application payload */
} MESH_Transmit_Data;


/**@brief The structrue of application key */
typedef struct MESH_App_Key
{
	u8 key[16];												/**< Application key */
}MESH_App_Key;


/**@brief The structrue of static provisioned */
typedef struct	MESH_CFG_StaticProvisioned
{
	u8 netKey[16];											/**< Network key */
	u8 devKey[16];											/**< Device key */
	u32 ivIndex;											/**< IV index */
	u16 nodeAddr;											/**< Node address */
	u8 groupAddrNum;										/**< Valid group number */
	u16 groupAddr[MAX_GROUP_NUM];							/**< Group address */
	u8 appKeyNum;											/**< Valid application key number*/
	MESH_App_Key appKey[MAX_APPLICATION_KEY_NUM];			/**< Application key */
}MESH_CFG_StaticProvisioned;


/**@brief The structrue of change scan parameters */
typedef struct	MESH_CFG_packetScanParams
{
	u16	interval;											/**< Scan interval, default is 0x0060 */
	u16	window;												/**< Scan window, default is 0x0060 */
}MESH_CFG_ScanParams;


/**@brief The structrue of change node information */
typedef struct	MESH_CFG_nodeParams
{
	u8	defaultTtl;											/**< Default TTL, default is 10*/
	u16 publishAddress;										/**< Publish address*/
	u8	relayState;											/**< Relay state, see @ref MESH_RELAY_STATE_DEF, default is MESH_REALY_STATE_ENABLE*/
}MESH_CFG_nodeParams;


/**@brief The structrue of change advertising parameters. */
typedef struct	MESH_CFG_advParams
{
	u16	advInterval;										/**< Advertising interval, default is 0x0032 */
	u16	advTimeout;											/**< Advertising timeout, default is 0x0320*/
}MESH_CFG_advParams;


/**@brief Data field of configuration*/
typedef union
{
    MESH_CFG_advParams				cfgAdvParams;			/**< The structure of change advertising parameters */
    MESH_CFG_ScanParams				cfgScanParams;			/**< The structure of change scan parameters */
    MESH_CFG_StaticProvisioned		cfgStaticProvisioned;	/**< The structure of static provisioned */
    MESH_CFG_nodeParams				cfgNodeParams;			/**< The structure of change node information */
} MESH_ConfParas;


/**@brief The structure of node information parameters.*/
typedef struct	MESH_LocalNode
{
	u16	nodeAddress;										/**< Node address */
	u8	defaultTtl;											/**< Default TTL */
	u16	publishAddress;										/**< Publish address */
	u8 netKey[16];											/**< Network key */
	u8 devKey[16];											/**< Device key */
	u32 ivIndex;											/**< IV index */
	MESH_App_Key appKey[MAX_APPLICATION_KEY_NUM];			/**< Application key */
	u16 groupAddr[MAX_GROUP_NUM];							/**< Group address */
}MESH_LocalNode;


/**
 *@brief Initialize Mesh module
 *
 *@param[in] meshHandler		MBIoT callback function for Mesh events.
 */
PUBLIC extern void BLE_MESH_Init(MESHProcessRoutine meshHandler)large;


/**@brief Start Mesh scheme.
 *
 * @return MBIOT_RES_SUCCESS Successfully start advertising.
 * @return MBIOT_RES_FAIL Unspecific error.
 */
PUBLIC extern u16 BLE_MESH_Start() large;


/**@brief Stop Mesh scheme. */
PUBLIC extern void BLE_MESH_Stop() large;


/**@brief Cofigure node parameters.
 *
 * @param[in] option			Configure id, See @ref Mesh_Config_ID
 * @param[in] p_confParas		Pointer to @ref MESH_ConfParas structure
 *
 * @return MBIOT_RES_SUCCESS Successfully configure node.
 * @return MBIOT_RES_FAIL Unspecific error.
 * @return MBIOT_RES_INVALID_PARA Invalid parameters.
 */
PUBLIC extern u16 BLE_MESH_Config(u8 option, MESH_ConfParas XDATA *p_confParas) large;


/**@brief Trsnsmit data.
 *
 * @param[in] componentAddress		Component address
 * @param[in] p_transmitData		Pointer to @ref MESH_Transmit_Data structure
 *
 * @return MBIOT_RES_SUCCESS Successfully configure node.
 * @return MBIOT_RES_FAIL Unspecific error.
 * @return MBIOT_RES_INVALID_PARA Invalid parameters.
 */
PUBLIC extern u16 BLE_MESH_TransmitData(u16 componentAddress,MESH_Transmit_Data XDATA *p_transmitData)large;


/**@brief Get node information.
 *
 * @param[in] p_localNode		Pointer to @ref MESH_LocalNode structure
 */
PUBLIC extern void BLE_MESH_GetLocalNodeInfo(MESH_LocalNode XDATA *p_localNode) large;


/**@brief Get node status.
 *
 * @return BLE_MESH_STATE_UNPROVISIOINED Node is under unprovisioned state.
 * @return BLE_MESH_STATE_PROVISIONING 	 Node is under provisioning state.
 * @return BLE_MESH_STATE_OPERATIONAL 	 Node is under operational state.
 */
PUBLIC extern u8 BLE_MESH_GetLocalNodeStates(void) large;


/**@brief Device provisioning by advertising.
 *
 * @param[in] meshOperator			Provisioning operator, see @ref MESH_PROVISIONING_OPERATOR_DEF
 * @param[in] p_deviceUUID		    Pointer to device uuid
 *
 * @return MBIOT_RES_SUCCESS Device wait for provisioning.
 * @return MBIOT_RES_FAIL Unspecific error.
 * @return MBIOT_RES_INVALID_PARA Invalid parameters.
 */	
PUBLIC extern u16 BLE_MESH_DeviceProvisioningByAdv(u8 meshOperator, u8 XDATA *p_deviceUUID) large;


/**@brief Device provisioning by gatt.
 *
 * @param[in] meshOperator			Provisioning operator, see @ref MESH_PROVISIONING_OPERATOR_DEF
 * @param[in] p_deviceUUID		    Pointer to device uuid
 *
 * @return MBIOT_RES_SUCCESS Device wait for provisioning.
 * @return MBIOT_RES_FAIL Unspecific error.
 * @return MBIOT_RES_INVALID_PARA Invalid parameters.
 */	
PUBLIC extern u16 BLE_MESH_DeviceProvisioningByGatt(u8 meshOperator, u8 XDATA *p_deviceUUID) large;


/**@brief Provisioner scans nearby unprovisioned device.
 *
 * @param[in] meshOperator			Provisioning operator, see @ref MESH_PROVISIONING_OPERATOR_DEF
 *
 * @return MBIOT_RES_SUCCESS Provisioner starts to scan nearby unprovisioned device.
 * @return MBIOT_RES_FAIL Unspecific error.
 * @return MBIOT_RES_INVALID_PARA Invalid parameters.
 */	
PUBLIC extern u16 BLE_MESH_ProvisionerScanning(u8 meshOperator) large;


/**@brief Provisioner provisioning nearby unprovisioned device by advertising.
 *
 * @param[in] meshOperator			Provisioning operator, see @ref MESH_PROVISIONING_OPERATOR_DEF
 * @param[in] p_deviceUUID		    Pointer to device uuid
 * @param[in] deviceAddress 		Assigned device address
 *
 * @return MBIOT_RES_SUCCESS Provisioner starts to provision nearby unprovisioned device.
 * @return MBIOT_RES_FAIL Unspecific error.
 * @return MBIOT_RES_INVALID_PARA Invalid parameters.
 */	
PUBLIC extern u16 BLE_MESH_ProvisionerProvisioningByAdv(u8 meshOperator, u8 XDATA *p_deviceUUID, u16 deviceAddress) large;


/**@brief Provisioner provisioning nearby unprovisioned device by gatt.
 *
 * @param[in] meshOperator			Provisioning operator, see @ref MESH_PROVISIONING_OPERATOR_DEF
 * @param[in] p_deviceUUID		    Pointer to @ref BLE_GAP_Addr structure
 * @param[in] deviceAddress 		Assigned device address
 *
 * @return MBIOT_RES_SUCCESS Provisioner starts to provision nearby unprovisioned device.
 * @return MBIOT_RES_FAIL Unspecific error.
 * @return MBIOT_RES_INVALID_PARA Invalid parameters.
 */		
PUBLIC extern u16 BLE_MESH_ProvisionerProvisioningByGatt(u8 meshOperator, BLE_GAP_Addr XDATA *p_deviceAddr, u16 deviceAddress) large;


#endif
 /**
  @}
*/