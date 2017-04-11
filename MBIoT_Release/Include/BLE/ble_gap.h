/********************************************************************
  File Information:
    FileName:       ble_gap.h
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
  @addtogroup BLE_GAP BLE GAP
  @{
  @brief BLE GAP Module
  @note Definitions and prototypes for the BLE GAP stack layer application programming interface.
 */

#ifndef BLE_GAP_H
#define BLE_GAP_H


/**@defgroup BLE_GAP_ADV_INTERVAL BLE_GAP_ADV_INTERVAL
 * @brief The range of advertising interval.
 * @{ */
#define BLE_GAP_ADV_INTERVAL_MIN				0x0020			/**< Minimum advertising interval. */
#define BLE_GAP_ADV_INTERVAL_MAX				0x4000			/**< Maximum advertising interval. */
/** @} */


/**@defgroup BLE_GAP_ADV_TYPE BLE_GAP_ADV_TYPE
 * @brief Advertising types.
 * @{ */
#define BLE_GAP_ADV_TYPE_ADV_IND				0x00			/**< Connectable undirected advertising (ADV_IND) (default). */
#define BLE_GAP_ADV_TYPE_ADV_DIRECT_IND			0x01			/**< Connectable high duty cycle directed advertising (ADV_DIRECT_IND, high duty cycle). */
#define BLE_GAP_ADV_TYPE_ADV_SCAN_IND			0x02			/**< Scannable undirected advertising (ADV_SCAN_IND). */
#define BLE_GAP_ADV_TYPE_ADV_NONCONN_IND		0x03			/**< Non connectable undirected advertising (ADV_NONCONN_IND). */
#define BLE_GAP_ADV_TYPE_ADV_DIRECT_IND_LOW		0x04			/**< Connectable low duty cycle directed advertising (ADV_DIRECT_IND, low duty cycle). */
/** @} */


/**@defgroup BLE_GAP_SCAN_INTERVAL BLE_GAP_SCAN_INTERVAL
 * @brief The range of scan interval.
 * @{ */
#define BLE_GAP_SCAN_INTERVAL_MIN					0x0004			/**< Minimum LE scan interval. */
#define BLE_GAP_SCAN_INTERVAL_MAX					0x4000			/**< Maximum LE scan interval. */
/** @} */


/**@defgroup BLE_GAP_SCAN_WINDOW BLE_GAP_SCAN_WINDOW
 * @brief The range of scan window.
 * @{ */
#define BLE_GAP_SCAN_WINDOW_MIN						0x0004			/**< Minimum LE scan window. */
#define BLE_GAP_SCAN_WINDOW_MAX						0x4000			/**< Maximum LE scan window. */
/** @} */


/**@defgroup BLE_GAP_SCAN_TYPE BLE_GAP_SCAN_TYPE
 * @brief Scan types.
 * @{ */
#define BLE_GAP_SCAN_TYPE_PASSIVE_SCAN				0x00			/**< Passive Scanning. No SCAN_REQ packets shall be sent (default). */
#define BLE_GAP_SCAN_TYPE_ACTIVE_SCAN				0x01			/**< Active scanning. SCAN_REQ packets may be sent. */
/** @} */


/**@defgroup BLE_GAP_ADDR_TYPE BLE_GAP_ADDR_TYPE
 * @brief Address types
 * @{ */
#define BLE_GAP_ADDR_TYPE_PUBLIC								0x00			/**< Public device address (default). */
#define BLE_GAP_ADDR_TYPE_RANDOM_STATIC							0x01			/**< Static random device address. */
#define BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_RESOLVABLE				0x02			/**< Private resolvable random device address. */
#define BLE_GAP_ADDR_TYPE_RANDOM_PRIVATE_NON_RESOLVABLE			0x03			/**< Private non-resolvable random device address. */
/** @} */


/**@defgroup BLE_GAP_SCAN_FILTER_POLICY BLE_GAP_SCAN_FILTER_POLICY
 * @brief Scan filter policy.
 * @{ */
#define BLE_GAP_SCAN_FILTER_DUPLICATES_DISABLE								0x00			/**< Duplicate filtering disabled. */
#define BLE_GAP_SCAN_FILTER_DUPLICATES_ENABLE								0x01			/**< Duplicate filtering enabled. */
/** @} */


/**@defgroup BLE_GAP_CP_RANGE  BLE_GAP_CP_RANGE
 * @brief The rnage of connection parameters.
 * @{ */
#define BLE_GAP_CP_MIN_CONN_INTVAL_MIN					0x0006  /**< Mimimum of interval_min, units :1.25 ms. */
#define BLE_GAP_CP_MIN_CONN_INTVAL_MAX					0x0C80  /**< Maximum of interval_min, units :1.25 ms. */
#define BLE_GAP_CP_MAX_CONN_INTVAL_MIN					0x0006  /**< Mimimum of interval_max, units :1.25 ms. */
#define BLE_GAP_CP_MAX_CONN_INTVAL_MAX					0x0C80  /**< Maximum of interval_max, units :1.25 ms. */
#define BLE_GAP_CP_LATENCY_MIN							0x0000  /**< Mimimum of latency. */
#define BLE_GAP_CP_LATENCY_MAX      		     		0x03E8  /**< Maximum of latency. */
#define BLE_GAP_CP_CONN_SUPERVISION_TIMEOUT_MIN			0x000A  /**< Mimimum of supervision_tmieout, units :10 ms. */
#define BLE_GAP_CP_CONN_SUPERVISION_TIMEOUT_MAX			0x0C80  /**< Maximum of supervision_tmieout, units :10 ms. */
/** @} */


/**@defgroup BLE_GAP_BONDED_FLAG BLE_GAP_BONDED_FLAG
 * @brief Bonded flag.
 * @{ */
#define BLE_GAP_DEVICE_IS_BONDED					0x00			/**< Remote device is bonded . */
#define BLE_GAP_DEVICET_IS_NO_BONDED				0x01			/**< Remote device is not bonded. */
/** @} */


/**@defgroup BLE_GAP_ROLE BLE_GAP_ROLE
 * @brief GAP role in connection state.
 * @{ */
#define BLE_GAP_ROLE_MASTER                  		0x00			/**< MBIoT plays as Master role in connection state . */
#define BLE_GAP_ROLE_SLAVE                   		0x01			/**< MBIoT plays as Slave role in connection state . */
/** @} */


/**@defgroup BLE_GAP_ENCRYPT_STATUS BLE_GAP_ENCRYPT_STATUS
 * @brief Encryptioin status
 * @{ */
#define BLE_GAP_ENCRYPT_SUCCESS                  		0x00		/**< The lnk is encrypted successful. */
#define BLE_GAP_ENCRYPT_FAIL	                   		0x01		/**< The lnk is encrypted fail. */
/** @} */


/**@defgroup BLE_GAP_ADREPORT_EVENT_TYPE BLE_GAP_ADREPORT_EVENT_TYPE
 * @brief Advertising report event type.
 * @{ */
#define BLE_GAP_ADREPORT_EVENT_TYPE_ADV_IND				0x00			/**< Connectable undirected advertising (ADV_IND). */
#define BLE_GAP_ADREPORT_EVENT_TYPE_ADV_DIRECT_IND		0x01			/**< Connectable directed advertising (ADV_DIRECT_IND). */
#define BLE_GAP_ADREPORT_EVENT_TYPE_ADV_SCAN_IND		0x02			/**< Scannable undirected advertising (ADV_SCAN_IND). */
#define BLE_GAP_ADREPORT_EVENT_TYPE_ADV_NONCONN_IND		0x03			/**< Non connectable undirected advertising (ADV_NONCONN_IND). */
#define BLE_GAP_ADREPORT_EVENT_TYPE_SCAN_RSP			0x04			/**< Scan Response (SCAN_RSP). */
/** @} */


/**@defgroup BLE_GAP_DISCONNECT_REASON BLE_GAP_DISCONNECT_REASON
 * @brief Reason of disconnection.
 * @{ */
#define BLE_GAP_DISCONNECT_REASON_AUTH_FAIL						0x05			/**< Authentication failure */
#define BLE_GAP_DISCONNECT_REASON_REMOTE_TERMINATE				0x13			/**< Remote User Terminated Connection */
#define BLE_GAP_DISCONNECT_REASON_LOW_RESOURCES					0x14			/**< Remote Device Terminated Connection due to Low Resources */
#define BLE_GAP_DISCONNECT_REASON_POWER_OFF						0x15			/**< Remote Device Terminated Connection due to Power Off */
#define BLE_GAP_DISCONNECT_REASON_UNSUPPORTED					0x1A			/**< Unsupported Remote Feature / Unsupported LMP Feature */
#define BLE_GAP_DISCONNECT_REASON_KEY_LEN_UNSUPPORTED			0x29			/**< Pairing with Unit Key Not Supported */
#define BLE_GAP_DISCONNECT_REASON_UNACCEPTABLE_CP				0x3B			/**< Unacceptable Connection Parameters */
/** @} */


/**@brief APP GAP call back events. */
enum BLE_GAP_EventID
{
	BLE_GAP_EVT_CONNECTED,						/**< BLE link is established. See @ref BLE_GAP_Evt_Connect for the event content. */
	BLE_GAP_EVT_DISCONNECTED,					/**< BLE link is terminated. See @ref BLE_GAP_Evt_Disconnect for the event content. */
	BLE_GAP_EVT_CONN_PARA_UPDATE,				/**< The connection parameters of BLE link are updated. See @ref BLE_GAP_Evt_ConnParaUpdatePara for the event content. */
	BLE_GAP_EVT_ADVERTISING_REPORT,				/**< Received advertising report. See @ref BLE_GAP_Evt_AdvertisingReport for the event content. */
	BLE_GAP_EVT_ENCRYPT_STATUS					/**< The encryption status of BLE link. See @ref BLE_GAP_Evt_encryptStatus for the event content. */
};


/**@defgroup BLE_GAP_MAX_ADV_LEN BLE_GAP_MAX_ADV_LEN
 * @{ */
#define BLE_GAP_ADV_MAX_LENGTH						0x1F			/**< Maximum length of advertising data. */
/** @} */


#define BLE_GAP_ADDR_LENGTH							6				/**< Length of bluetooth address. */
#define BLE_GAP_DEVICE_MAX_LENGTH					32				/**< Maximum length of device name. */


/**@brief The structure of Bluetooth address. */
typedef struct BLE_GAP_Addr{
    u8		addr_type;							/**< See @ref BLE_GAP_ADDR_TYPE. */
    u8		addr[BLE_GAP_ADDR_LENGTH];			/**< Bluetooth address, LSB format. */
}BLE_GAP_Addr;


/**@brief The structure of advertising parameters. */
typedef struct BLE_GAP_AdvertisingParas{
    u8				type;						/**< Advertising type, see @ref BLE_GAP_ADV_TYPE. */
    BLE_GAP_Addr	peer_addr;					/**< Remote bluetooth address and address type, see @ref BLE_GAP_Addr.*/
    u16				interval;					/**< Advertising interval, see @ref BLE_GAP_ADV_INTERVAL. Unit: 0.625ms */
}BLE_GAP_AdvertisingParas;


/**@brief The structure of scan parameters. */
typedef struct BLE_GAP_ScanningParas{
    u8				type;						/**< Scanning type, see @ref BLE_GAP_SCAN_TYPE. */
    u16				interval;					/**< Scanning interval, see @ref BLE_GAP_SCAN_INTERVAL. Unit: 0.625ms*/
    u16				window;						/**< Scanning window, see @ref BLE_GAP_SCAN_WINDOW. Unit: 0.625ms*/
    u8				filterDuplicate;			/**< Filter duplicate policy, see @ref BLE_GAP_SCAN_FILTER_POLICY. */
}BLE_GAP_ScanningParas;


/**@brief The structure of connection parameters.*/
typedef struct BLE_GAP_ConnParas{
    u16				interval_min;				/**< Minimum value for the connection event interval, see @ref BLE_GAP_CP_RANGE. */
    u16				interval_max;				/**< Maximum value for the connection event interval, see @ref BLE_GAP_CP_RANGE. */
    u16				latency;					/**< Slave latency for the connection in number of connection events, see @ref BLE_GAP_CP_RANGE. */
    u16				supervision_tmieout;		/**< Supervision timeout for the LE Link, see @ref BLE_GAP_CP_RANGE. */
}BLE_GAP_ConnParas;


/**@brief The structure of create connection parameters.*/
typedef struct BLE_GAP_CreateConnParas{
    u16						scan_interval;		/**< Scan interval, see @ref BLE_GAP_SCAN_TYPE. */
    u16						scan_window;		/**< Scan window, see @ref BLE_GAP_SCAN_WINDOW. */
	BLE_GAP_Addr			peer_addr;			/**< Remote bluetooth address and address type, see @ref BLE_GAP_Addr. */
	BLE_GAP_ConnParas		conn_paras;			/**< GAP connection parameters. see @ref BLE_GAP_ConnParas. */
}BLE_GAP_CreateConnParas;


/**@brief The structure of set advertising data configuration.*/
typedef struct BLE_GAP_SetAdvertisingDataParas{
	u8 adLen;										/**< Length of advertising data. */
	u8 adData[BLE_GAP_ADV_MAX_LENGTH];				/**< Advertising data, see @ref BLE_GAP_MAX_ADV_LEN for the limitioin.*/
	u8 adScanResLen;								/**< Length of scan response advertising data. */		
	u8 adScanResData[BLE_GAP_ADV_MAX_LENGTH];		/**< scan response advertising data, see @ref BLE_GAP_MAX_ADV_LEN for the limitioin. */
}BLE_GAP_SetAdvertisingDataParas;


/**@brief The structure of disconnect event parameters. */
typedef struct BLE_GAP_Evt_Disconnect
{
	u8 connHandle;											/**< Connection handle. */
	u8 reason;												/**< The reason of disconnection, see @ref BLE_GAP_DISCONNECT_REASON. */
}BLE_GAP_Evt_Disconnect;


/**@brief The structure of connect event parameters. */
typedef struct BLE_GAP_Evt_Connect
{
	u8 connHandle;											/**< Connection handle. */
	u8 role;												/**< GAP role, see @ref BLE_GAP_ROLE. */
	u8 bondedDevice;										/**< Bonded flag, see @ref BLE_GAP_BONDED_FLAG. */
	BLE_GAP_Addr remoteAddr;								/**< Remote bluetooth address, see @ref BLE_GAP_Addr. */
	BLE_GAP_ConnParas	connPara;							/**< Connection parameters, see @ref BLE_GAP_ConnParas. */
}BLE_GAP_Evt_Connect;


/**@brief The structure of connection parameter update event parameters. */
typedef struct BLE_GAP_Evt_ConnParaUpdatePara
{
	u8 connHandle;											/**< Connection handle. */
	u8 status;												/**< Status of connection parameter update. 0: Connection parameters update success. Others: Fail. */
	BLE_GAP_ConnParas	connPara;							/**< Connection parameters, see @ref BLE_GAP_ConnParas. */
}BLE_GAP_Evt_ConnParaUpdatePara;


/**@brief The structure of advertising report event parameters. */
typedef struct BLE_GAP_Evt_AdvertisingReport
{
	u8 eventType;											/**< Event type, see @ref BLE_GAP_ADREPORT_EVENT_TYPE. */
	u8 addrType;											/**< Address type, see @ref BLE_GAP_ADDR_TYPE.  */
	u8 addr[BLE_GAP_ADDR_LENGTH];							/**< Bluetooth device address. */
	u8 length;												/**< Length of advertising data. */
	u8 advData[BLE_GAP_ADV_MAX_LENGTH];						/**< Advertising data. */
	s8 rssi;												/**< RSSI value. */
	BLE_GAP_ConnParas	connPara;							/**< Connection parameters, see @ref BLE_GAP_ConnParas.  */
}BLE_GAP_Evt_AdvertisingReport;


/**@brief The structure of encryption status parameters. */
typedef struct BLE_GAP_Evt_encryptStatus
{
	u8 connHandle;											/**< Connection handle. */
	u8 status;												/**< Encryption status. */
}BLE_GAP_Evt_encryptStatus;


/**@brief BLE GAP event types */
typedef union 
{ 
	BLE_GAP_Evt_Disconnect				evtDisconnect;			/**< Data field of disconnection event. */
	BLE_GAP_Evt_Connect					evtConnect;				/**< Data field of connect event. */
	BLE_GAP_Evt_ConnParaUpdatePara		evtConnParaUpdate;		/**< Data field of connection parameter update event. */
	BLE_GAP_Evt_AdvertisingReport		evtAdvReport;			/**< Data field of advertising report event. */
	BLE_GAP_Evt_encryptStatus			evtEncryptStatus;		/**< Data field of encryption status event. */
}BLE_GPPEventField;


/**@brief Structure of BLE GAP events. */
typedef struct	BLE_GAP_Event
{
	u8	eventID;											/**< Event ID. See @ref BLE_GAP_EventID.  */
	BLE_GPPEventField	eventField;							/**< Data field of this event, see @ref BLE_GAP_ENCRYPT_STATUS.  */
}BLE_GAP_Event;


/**@brief BLE GAP callback function prototype. */
typedef void(*GAPProcessRoutine)(BLE_GAP_Event XDATA *p_event) large;


/**
 *@brief Initialize GAP callback function.
 *
 *@param[in] gapHandler		MBIoT callback function for gap events.
 */
PUBLIC extern void BLE_GAP_Init(GAPProcessRoutine gapHandler) large;


/**@brief Get local bluetooth address.
 *
 *@param[out] p_addr	Pointer to @ref BLE_GAP_Addr address structure
 *
 */
PUBLIC extern void BLE_GAP_Get_Addr(BLE_GAP_Addr XDATA  *p_addr) large;


/**@brief Set local Bluetooth address.
 *
 *@param[in] p_addr Pointer to @ref BLE_GAP_Addr address structure.
 *
 *@return MBIOT_RES_SUCCESS Successfully set local bluetooth address.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 */
PUBLIC extern u16 BLE_GAP_Set_Addr(BLE_GAP_Addr XDATA *p_addr) large;


/**@brief Get local device name.
 *
 *@param[out] p_len   Pointer to the length buffer of the p_deviceName size.
 *@param[out] p_deviceName	pointer to an device name buffer.
 */
PUBLIC extern void BLE_GAP_Get_DeviceName(u8 XDATA  *p_len, u8 XDATA  *p_deviceName) large;


/**@brief Set device name.
 *
 *@param[in] len   Length of the device name.
 *@param[in] p_deviceName Pointer to a device name buffer (UTF-8 string).
 *
 *@return MBIOT_RES_SUCCESS Successfully set device name successfully.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 */
PUBLIC extern u16 BLE_GAP_Set_DeviceName(u8 len, u8 XDATA  *p_deviceName) large;


/**@brief Configure advertising data and scan response advertising data.
 *
 *@param[in] p_adData      Pointer to @ref BLE_GAP_SetAdvertisingDataParas structure.
 *
 *@return MBIOT_RES_SUCCESS Advertising data successfully configured.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 */
PUBLIC extern u16 BLE_GAP_Set_AdvertisingData(BLE_GAP_SetAdvertisingDataParas XDATA  *p_adData) large;


/**@brief Start advertising.
 *
 * @param[in] p_advParas 	Pointer to @ref BLE_GAP_AdvertisingParas structure.
 *
 * @return MBIOT_RES_SUCCESS Successfully start advertising.
 * @return MBIOT_RES_FAIL Unspecific error.
 * @return MBIOT_RES_INVALID_PARA Invalid parameters.
 */
PUBLIC extern u16 BLE_GAP_Advertising_Start(BLE_GAP_AdvertisingParas XDATA  *p_advParas) large;


/**@brief Stop advertising.
 *
 * @return MBIOT_RES_SUCCESS Successfully stop advertising.
 * @return MBIOT_RES_FAIL Unspecific error.
 */
PUBLIC extern u16 BLE_GAP_Advertising_Stop();


/**@brief Start scanning.
 *
 * @param[in] p_scanParas 	Pointer to @ref BLE_GAP_ScanningParas structure.
 *
 * @return MBIOT_RES_SUCCESS Successfully start scanning.
 * @return MBIOT_RES_FAIL Unspecific error.
 * @return MBIOT_RES_INVALID_PARA Invalid parameters.
 */
PUBLIC extern u16 BLE_GAP_Scanning_Start(BLE_GAP_ScanningParas XDATA  *p_scanParas) large;


/**@brief Stop scanning.
 *
 * @return MBIOT_RES_SUCCESS Successfully stop scanning.
 * @return MBIOT_RES_FAIL Unspecific error.
 */
PUBLIC extern u16 BLE_GAP_Scanning_Stop();


/**@brief Get beacon address.
 *
 *@param[out] p_addr	Pointer to six bytes address buffer.
 *
 */
PUBLIC extern void BLE_GAP_Set_BeaconAddr(u8 XDATA  *p_addr) large;


/**@brief Set beacon address.
 *
 *@param[in] p_addr 	Pointer to six bytes address buffer.
 *
 */
PUBLIC extern void BLE_GAP_Get_BeaconAddr(u8 XDATA  *p_addr) large;


/**@brief Configure beacon advertising data.
 *
 *@param[in] bLen       Length of beacon advertising data. See @ref BLE_GAP_MAX_ADV_LEN for the limition. If p_bData is NULL, the bLen should be 0.
 *@param[in] p_bData   Pointer to the data buffer used in beacon advertising packets.
 *
 *@return MBIOT_RES_SUCCESS Successfully configure beacon advertising data.
 *@return MBIOT_RES_INVALID_PARA Invalid parameters.
 */
PUBLIC extern u16 BLE_GAP_Set_BeaconData(u8 bLen, u8 XDATA *p_bData) large;


/**@brief Start beacon advertising.
 *
 * @param[in] interval 	Beacon advertising interval.
 *
 * @return MBIOT_RES_SUCCESS Successfully start beacon advertising.
 * @return MBIOT_RES_FAIL Unspecific error.
 * @return MBIOT_RES_INVALID_PARA Invalid parameters.
 */
PUBLIC extern u16 BLE_GAP_BeaconStart(u16 interval) large;


/**@brief Stop beacon advertising.
 *
 * @return MBIOT_RES_SUCCESS Successfully stop beacon advertising.
 * @return MBIOT_RES_FAIL Unspecific error.
 */
PUBLIC extern u16 BLE_GAP_BeaconStop();


/**@brief Create a link layer connection to a connectable advertiser.
 *
 * @param[in] p_reateConnPara 	Pointer to the @ref BLE_GAP_CreateConnParas structure.
 *
 * @return MBIOT_RES_SUCCESS	Successfully create a BLE link with specified device.
 * @return MBIOT_RES_OOM		Out of memory.
 * @return MBIOT_RES_INVALID_PARA Invalid parameters. 
 * 
 */
PUBLIC extern u16 BLE_GAP_CreateConnection(BLE_GAP_CreateConnParas XDATA *p_reateConnPara) large;


/**@brief Cancel the current connecting procedure.
 *
 * @return MBIOT_RES_SUCCESS	Successfully cancel the current connecting procedure.
 * @return MBIOT_RES_FAIL		Unspecific error.
 */
PUBLIC extern u16 BLE_GAP_CancelConnection();


/**@brief Terminate specified BLE link.
 *
 * @param[in] connHandle 	Connection handle.
 * @param[in] reason 		Reason of the disconnection. 
 *
 * @return MBIOT_RES_SUCCESS	Successfully terminate the specified BLE link.
 * @return MBIOT_RES_OOM		Out of memory.
 */
PUBLIC extern u16 BLE_GAP_TerminateConnection(u8 connHandle, u8 reason) large;


/**@brief Update connection parameters (Used by central role).
 *
 * @param[in] connHandle 	Connection handle.
 * @param[in] p_connParas 	Pointer to the @ref BLE_GAP_ConnParas structure. 
 *
 * @return MBIOT_RES_SUCCESS	Successfully update the connection parameters.
 * @return MBIOT_RES_OOM		Out of memory.
 * @return MBIOT_RES_INVALID_PARA Invalid parameters.
 */
PUBLIC extern u16 BLE_GAP_UpdateConnPara(u8 connHandle, BLE_GAP_ConnParas XDATA * p_connParas) large;


/**@brief Get signal strength at antenna measured in dBm.
 *
 * @param[in] connHandle 	Connection handle.
 * @param[out] p_rssi 		Pointer to rssi buffer.
 *
 * @return MBIOT_RES_SUCCESS Successfully get RSSI value.
 * @return MBIOT_RES_INVALID_PARA Invalid parameters.
 */
PUBLIC extern u16 BLE_GAP_GetRSSI(u8 connHandle, u8 XDATA *p_rssi) large;

#endif

/**
  @}
*/