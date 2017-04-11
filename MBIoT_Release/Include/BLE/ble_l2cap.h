/********************************************************************
  File Information:
    FileName:       ble_l2cap.h
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
  @addtogroup BLE_L2CAP BLE L2CAP
  @{
  @brief BLE L2CAP Module
  @note Definitions and prototypes for the BLE L2CAP stack layer application programming interface.
 */

#ifndef BLE_L2CAP_H
#define BLE_L2CAP_H
 
/**@brief BLE L2CAP call back events. */
enum BLE_L2CAP_EventID
{
	BLE_L2CAP_EVT_CONN_PARA_UPDATE_RESULT,		/**< Receives L2CAP connection parameters update response from remote device. See the @ref BLE_L2CAP_Evt_ConnParaUpdateResult for the event content. */
	BLE_L2CAP_EVT_COC_RCV_DATA,					/**< Receives L2CAP credit based data from remote device. See the @ref BLE_L2CAP_Evt_Coc_Rcv_Data for the event content. */
	BLE_L2CAP_EVT_COC_FLOW_CONTROL_CREDIT,		/**< Receives L2CAP credit based flow control from remote device. See the @ref BLE_L2CAP_Evt_Coc_Flow_Control_Credit for the event content. */
	BLE_L2CAP_EVT_COC_CONN_RSP,					/**< Receives L2CAP credit based connection respone from remote device. See the @ref BLE_L2CAP_Evt_Coc_Conn_Rsp for the event content. */
	BLE_L2CAP_EVT_COC_CONN_REQ,					/**< Receives L2CAP credit based connection request from remote device. See the @ref BLE_L2CAP_Evt_Coc_Conn_Req for the event content. */
	BLE_L2CAP_EVT_COC_DISCONN_REQ,				/**< Receives L2CAP credit based disconnection request from remote device. See the @ref BLE_L2CAP_Evt_Coc_Disconn_Req for the event content. */
	BLE_L2CAP_EVT_COC_DISCONN_RES				/**< Receives L2CAP credit based disconnection response from remote device. See the @ref BLE_L2CAP_Evt_Coc_Disconn_Req for the event content. */
};


/**@defgroup BLE_L2CAP_CPU_RESULT BLE_L2CAP_CPU_RESULT
 * @brief The result of connection Parameter Update Request.
 * @{ */
#define BLE_L2CAP_CONNECTION_PARAS_ACCEPTED			0x0000		/**< Connection Parameter Update Request is accepted. */		
#define BLE_L2CAP_CONNECTION_PARAS_REJECTED			0x0001		/**< Connection Parameter Update Request is rejected */
/** @} */


/**@defgroup BLE_L2CAP_COC_OPTION BLE_L2CAP_COC_OPTION
 * @brief Permission of credit based connection establishment.
 * @{ */
#define BLE_L2CAP_CREDIT_BASED_CONNECTION_PERMISSION_DISALLOW			0x0000		/**< Credit based connection is disallowed. */		
#define BLE_L2CAP_CREDIT_BASED_CONNECTION_PERMISSION_ALLOW				0x0001		/**< Credit based connection is allowed */
/** @} */


/**@brief The structure of onnection parameters update request parameters.*/
typedef struct BLE_L2CAP_ConnParaUpdateReqPara{
    u16      intervalMin;					/**< Minimum value for the connection event interval. (Unit: 1.25ms)*/
    u16      intervalMax;					/**< Maximum value for the connection event interval. (Unit: 1.25ms)*/
	u16      latency;						/**< Slave latency for the connection in number of connection events. */
	u16      timeout;						/**< Supervision timeout for the LE Link. (Unit: 10ms)*/
}BLE_L2CAP_ConnParaUpdateReqPara;


/**@brief The structure of credit based connection request parameters.*/
typedef struct BLE_L2CAP_CocConnReqPara{
    u16      lePSM;							/**< LE Protocol/Service Multiplexer.*/
    u16      mps;							/**< Maximum payload size*/
	u16      initialCredits;				/**< Initial credits.*/
}BLE_L2CAP_CocConnReqPara;


/**@brief The structure of credit based connection send data parameters.*/
typedef struct BLE_L2CAP_CocSendDataPara{
    u16      length;						/**< Length of payload size.*/
	u8 XDATA *p_payload;					/**< Pointer to payload buffer.*/
}BLE_L2CAP_CocSendDataPara;


/**@brief THe structure of connection parameters update response event parameters.*/
typedef struct	BLE_L2CAP_Evt_ConnParaUpdateResult
{
	u8 connHandle;												/**< Connection handle. */	
	u16 result;													/**< The field indicates the response to the Connection Parameter Update Request. See @ref BLE_L2CAP_CPU_RESULT.*/
}BLE_L2CAP_Evt_ConnParaUpdateResult;


/**@brief THe structure of credit based data receive event parameters.*/
typedef struct	BLE_L2CAP_Evt_Coc_Rcv_Data
{
	u8 		connHandle;											/**< Connection handle.*/
	u16		lePSM;												/**< LE Protocol/Service Multiplexer.*/
	u16 	length;												/**< Length of received data.*/
	u8 XDATA *dataPayload;										/**< Pointer to received data.*/
}BLE_L2CAP_Evt_Coc_Rcv_Data;


/**@brief THe structure of credit based flow control event parameters.*/
typedef struct	BLE_L2CAP_Evt_Coc_Flow_Control_Credit
{
	u8 		connHandle;											/**< Connection handle.*/
	u16		lePSM;												/**< LE Protocol/Service Multiplexer.*/
	u16 	creditNum;											/**< Credit number.*/
}BLE_L2CAP_Evt_Coc_Flow_Control_Credit;


/**@brief THe structure of credit based connection respone event parameters.*/
typedef struct	BLE_L2CAP_Evt_Coc_Conn_Rsp
{
	u8		connHandle;											/**< Connection handle.*/
	u16		initialCredits;										/**< Initial credits.*/
	u16		result;												/**< The outcome of the connection request.*/
}BLE_L2CAP_Evt_Coc_Conn_Rsp;


/**@brief THe structure of credit based connection request event parameters.*/
typedef struct	BLE_L2CAP_Evt_Coc_Conn_Req
{
	u8 		connHandle;											/**< Connection handle.*/
    u16		lePSM;												/**< LE Protocol/Service Multiplexer.*/
	u16		initialCredits;										/**< Initial credits.*/
}BLE_L2CAP_Evt_Coc_Conn_Req;


/**@brief THe structure of credit based disconnection request event parameters.*/
typedef struct	BLE_L2CAP_Evt_Coc_Disconn_Req
{
	u8		connHandle;											/**< Connection handle.*/
    u16     lePSM;												/**< LE Protocol/Service Multiplexer.*/
}BLE_L2CAP_Evt_Coc_Disconn_Req;


/**@brief THe structure of credit based disconnection response event parameters.*/
typedef struct	BLE_L2CAP_Evt_Coc_Disconn_Res
{
	u8		connHandle;											/**< Connection handle.*/
    u16     lePSM;												/**< LE Protocol/Service Multiplexer.*/
}BLE_L2CAP_Evt_Coc_Disconn_Res;

/**@brief BLE L2CAP event types */
typedef union 
{ 
	BLE_L2CAP_Evt_ConnParaUpdateResult		evtConnParaUpdateResult;		/**< Data field of receiving L2CAP connection parameters update response event. */
	BLE_L2CAP_Evt_Coc_Rcv_Data				evtCocRcvData;					/**< Data field of receiving L2CAP connection parameters update response event. */
	BLE_L2CAP_Evt_Coc_Flow_Control_Credit	evtCocFlowControl;				/**< Data field of receiving L2CAP connection parameters update response event. */
	BLE_L2CAP_Evt_Coc_Conn_Rsp				evtCocConnRsp;					/**< Data field of receiving L2CAP connection parameters update response event. */
	BLE_L2CAP_Evt_Coc_Conn_Req				evtCocConnReq;					/**< Data field of receiving L2CAP credit based connection request event. */
	BLE_L2CAP_Evt_Coc_Disconn_Req			evtCocDisConnReq;				/**< Data field of receiving L2CAP credit based disconnection request event. */
	BLE_L2CAP_Evt_Coc_Disconn_Res			evtCocDisConnRes;				/**< Data field of receiving L2CAP credit based disconnection response event. */
}BLE_L2CAPEventField;


/**@brief BLE L2CAP callback function parameters. */
typedef struct	BLE_L2CAP_Event
{
	u8	eventID;											/**< Event ID. See @ref BLE_L2CAP_EventID.*/
	BLE_L2CAPEventField		eventField;						/**< Data field of this event. */
}BLE_L2CAP_Event;


/**@brief BLE L2CAP callback function prototype. */
typedef void(*L2CAPProcessRoutine)(BLE_L2CAP_Event XDATA *p_event) large;


/**@brief Initialize BLE L2CAP callback function.
 *
 *@param[in] l2capHandler		MBIoT callback function for l2cap events.
 */
PUBLIC extern void BLE_L2CAP_Init(L2CAPProcessRoutine l2capHandler) large;


/**@brief Request connection parameters update.
 *
 * @param[in] connHandle 	Connection handle.
 * @param[in] p_connParas 	Pointer to the @ref BLE_L2CAP_ConnParaUpdateReqPara structure. 
 *
 * @return MBIOT_RES_SUCCESS - Successfully request connection parameters update.
 * @return MBIOT_RES_OOM  Out of memory.
 * @return MBIOT_RES_INVALID_PARA Invalid connection parameters.
 *
 * @note
 * This API allows the LE slave Host to request a set of new connection parameters and shall only be used by the LE slave device.
 */
PUBLIC extern u16 BLE_L2CAP_ConnParaUpdateReq(u8 connHandle, BLE_L2CAP_ConnParaUpdateReqPara XDATA *p_connParas) large;


/**@brief Credit based connection request.
 *
 * @param[in] connHandle 	Connection handle.
 * @param[in] p_connReqParas 	Pointer to the @ref BLE_L2CAP_CocConnReqPara structure. 
 *
 * @return MBIOT_RES_SUCCESS - Successfully request connection parameters update.
 * @return MBIOT_RES_OOM  Out of memory.
 * @return MBIOT_RES_INVALID_PARA Invalid connection parameters.
 */
PUBLIC extern u16 BLE_L2CAP_CreditBasedConnectionRequest(u8 connHandle, BLE_L2CAP_CocConnReqPara XDATA *p_connReqParas) large;


/**@brief Flow control credit.
 *
 * @param[in] connHandle 	Connection handle.
 * @param[in] credit 		Credit number. 
 *
 * @return MBIOT_RES_SUCCESS - Successfully request connection parameters update.
 * @return MBIOT_RES_OOM  Out of memory.
 * @return MBIOT_RES_INVALID_PARA Invalid connection parameters.
 */
PUBLIC extern u16 BLE_L2CAP_CreditBasedFlowControlCredit(u8 handle, u16 credit) large;


/**@brief Send data through credit based connection channel.
 *
 * @param[in] connHandle 	Connection handle.
 * @param[in] p_sendDataParas 	Pointer to the @ref BLE_L2CAP_CocSendDataPara structure. 
 *
 * @return MBIOT_RES_SUCCESS - Successfully request connection parameters update.
 * @return MBIOT_RES_OOM  Out of memory.
 * @return MBIOT_RES_INVALID_PARA Invalid connection parameters.
 */
PUBLIC extern u16 BLE_L2CAP_CreditBasedSendData(u8 connHandle, BLE_L2CAP_CocSendDataPara XDATA *p_sendDataParas) large;


/**@brief Send credit based disconnection.
 *
 * @param[in] connHandle 	Connection handle.
 *
 * @return MBIOT_RES_SUCCESS - Successfully request connection parameters update.
 * @return MBIOT_RES_OOM  Out of memory.
 * @return MBIOT_RES_INVALID_PARA Invalid connection parameters.
 */
PUBLIC extern u16 BLE_L2CAP_SendDisconnectRequest(u8 connHandle) large;


/**@brief Credit based connection permission.
 *
 * @param[in] option 	Permission of credit based connection @ref BLE_L2CAP_COC_OPTION.
 *
 * @return MBIOT_RES_SUCCESS - Successfully request connection parameters update.
 * @return MBIOT_RES_INVALID_PARA Invalid connection parameters.
 */
PUBLIC extern u16 BLE_L2CAP_CreditBasedConnectionPermission(u8 option) large;

#endif

/**
  @}
*/