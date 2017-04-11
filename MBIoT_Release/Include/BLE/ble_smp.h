/********************************************************************
  File Information:
    FileName:       ble_smp.h
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
  @addtogroup BLE_SMP BLE SMP
  @{
  @brief BLE Security Manager Module
  @note Definitions and prototypes for the BLE SM stack layer application programming interface.
 */

#ifndef BLE_SMP_H
#define BLE_SMP_H

 
/**@defgroup BLE_SMP_IO_CAPABILITY_DEF BLE_SMP_IO_CAPABILITY_DEF
 * @brief SMP IO Capabilities.
 * @{ */
#define BLE_SMP_IO_DISPLAYONLY				0x00				/**< Device has the ability to display or communicate a 6 digit decimal number but does not have the ability to indicate yes or no. */
#define BLE_SMP_IO_DISPLAYYESNO  			0x01				/**< Device has at least two buttons that can be easily mapped to yes and no and also has the ability to display or communicate a 6 digit decimal number. */
#define BLE_SMP_IO_KEYBOARDONLY 			0x02				/**< Device has a numeric keyboard that can input the numbers 0 through 9 and a confirmation but does not have the ability to display or communicate a 6 digit decimal number. */
#define BLE_SMP_IO_NOINPUTNOOUTPUT	 		0x03				/**< Device does not have the abilities to indicate yes or no and display or communicate a 6 digit decimal number. */
#define BLE_SMP_IO_KEYBOARDDISPLAY	 		0x04				/**< Device has a numeric keyboard that can input the numbers 0 through 9 and a confirmation and has the ability to display or communicate a 6 digit decimal number. */
/** @} */


/**@defgroup BLE_SMP_OPTION BLE_SMP_OPTION
 * @ SMP Authentication Requirements Flag.
 * @{ */
#define BLE_SMP_OPTION_NO_BONDING							0x00		/**< Do not allow a bond to be created with a peer device. */
#define BLE_SMP_OPTION_BONDING								0x01		/**< Allows two connected devices to exchange and store security and identity information to create a trusted relationship . */
#define BLE_SMP_OPTION_SECURE_CONNECTION					0x08		/**< Request LE Secure Connections pairing. */
/** @} */


/**@defgroup BLE_SMP_CONFIRM BLE_SMP_CONFIRM
 * @brief Definition of user confirm result.
 * @{ */
#define BLE_SMP_CONFIRM_YES								0x00			/**< User confirms "OK". */
#define BLE_SMP_CONFIRM_NO								0x01			/**< User confirms "NO". */
/** @} */


/**@defgroup BLE_SMP_PAIRING_RESULT BLE_SMP_PAIRING_RESULT
 * @brief The result of pairing process.
 * @{ */
#define BLE_SMP_PAIRING_SUCCESS								0x00		/**< Pairing process is successful. */
#define BLE_SMP_PAIRING_FAIL								0x01		/**< Pairing process is fail. */
/** @} */


/**@defgroup BLE_SMP_SC_ONLY_OPTION BLE_SMP_SC_ONLY_OPTION
 * @brief Definition of Secure Connections Only Mode option
 * @{ */
#define BLE_SMP_SC_ONLY_DISABLE								0x00		/**< Enable LE Secure Connections pairing only mode. */
#define BLE_SMP_SC_ONLY_ENABLE								0x01		/**< Disable LE Secure Connections pairing only mode. */
/** @} */


/** @brief BLE SMP callback events.*/
enum BLE_SMP_EventID
{
	BLE_SMP_EVT_PAIRING_COMPLETE,						/**< Pairing process is finished. See the @ref BLE_SMP_Evt_PairingComplete structure for eventField. */
	BLE_SMP_EVT_SECURITY_REQUEST,						/**< Receives Pairing request from remote device. See the @ref BLE_SMP_Evt_SecurityReq structure for eventField. */
	BLE_SMP_EVT_NUMERIC_COMPARISON_CONFIRM_REQUEST,		/**< Passkey is needed to be confirmed for pairing process. See the @ref BLE_SMP_Evt_DisplayCompareValue structure for eventField. */
	BLE_SMP_EVT_INPUT_PASSKEY,							/**< Passkey is requested to be inputted for pairing process. See the @ref BLE_SMP_Evt_InputPasskey structure for eventField. */
	BLE_SMP_EVT_DISPLAY_PASSKEY_REQUEST					/**< Passkey is needed to be displayed for pairing process. See the @ref BLE_SMP_Evt_DisplayPasskeyRequest structure for eventField. */
};


/**@brief The structure of pairing complete event parameters. */
typedef struct	BLE_SMP_Evt_PairingComplete
{
	u8 connHandle;												/**< Connection handle. */
	u8 status;													/**< Sstatus of pairing procedure. See @ref BLE_SMP_PAIRING_RESULT. */
}BLE_SMP_Evt_PairingComplete;


/**@brief The structure of display compare value event parameters. */
typedef struct	BLE_SMP_Evt_DisplayCompareValue
{
	u8 connHandle;												/**< Connection handle. */		
	u8 value[6];												/**< The passkey needs to be confirmed and the format is using ASCII. */
}BLE_SMP_Evt_DisplayCompareValue;


/**@brief The sructure of display passkey event parameters. */
typedef struct	BLE_SMP_Evt_DisplayPasskeyRequest
{
	u8 connHandle;												/**< Connection handle. */	
}BLE_SMP_Evt_DisplayPasskeyRequest;


/**@brief The structure of input passkey event parameter. */
typedef struct	BLE_SMP_Evt_InputPasskey
{
	u8 connHandle;												/**< Connection handle. */	
}BLE_SMP_Evt_InputPasskey;


/**@brief The structrue of security request event parameters. */
typedef struct	BLE_SMP_Evt_SecurityReq
{
	u8 connHandle;												/**< Connection handle. */	
	u8 authReq;													/**< Authentication requirement flag. See @ref BLE_SMP_OPTION. */
}BLE_SMP_Evt_SecurityReq;


/**@brief BLE SMP event types */
typedef union 
{ 
	BLE_SMP_Evt_PairingComplete			evtPairingComplete;			/**< Data field of pairing complete event. */
	BLE_SMP_Evt_InputPasskey			evtInputPasskey;			/**< Data field of input passkey event. */
	BLE_SMP_Evt_DisplayCompareValue		evtDisplayCompareValue;		/**< Data field of display compare value event. */
	BLE_SMP_Evt_DisplayPasskeyRequest	evtDisplayPasskeyReq;		/**< Data field of display passkey event. */
	BLE_SMP_Evt_SecurityReq				evtSecurityReq;				/**< Data field of receive security request event. */
}BLE_SMPEventField;


/**@brief BLE SMP callback function parameters. */
typedef struct	BLE_SMP_Event
{
	u8	eventID;											/**< Event ID. See @ref BLE_SMP_EventID. */
	BLE_SMPEventField		eventField;						/**< Data field of this event. */
}BLE_SMP_Event;


/**@brief BLE SMP callback function prototype. */
typedef void(*SMPProcessRoutine)(BLE_SMP_Event *p_event) large;


/**@brief Initialize BLE SMP callback function.
 *
 * @param[in] smpHandler		MBIoT callback function for smp events. 
 */
PUBLIC extern void BLE_SMP_Init(SMPProcessRoutine smpHandler) large;


/**@brief Configure SMP pairing parameters.
 *
 * @param[in] ioCapability 	See @ref BLE_SMP_IO_CAPABILITY_DEF. Input and output capabilities of a device handle.
 * @param[in] authReqFlag 	See @ref BLE_SMP_OPTION. Authentication Requirement Flag.
 * @param[in] secureConnectionsOnly	 See @ref BLE_SMP_SC_ONLY_OPTION. Secure Connections only mode.
 *
 * @return MBIOT_RES_SUCCESS Configure SMP pairing parameters successfully.
 * @return MBIOT_RES_INVALID_PARA The configured parameter(s) is/are invalid.
 */
PUBLIC extern u16 BLE_SMP_Config(u8 ioCapability, u8 authReqFlag, u8 secureConnectionsOnly) large;


/**@brief Reply passkey for pairing process.
 *
 * @param[in] connHandle 		Connection handle.
 * @param[in] p_passkey 	    Pointer to the passkey structure. The length of passkey should be 6 bytes in ASCII format.
 *
 * @return MBIOT_RES_SUCCESS Successfully reply passkey for pairing process.
 * @return MBIOT_RES_FAIL The specified connection handle is invalid.
 */
PUBLIC extern u16 BLE_SMP_PasskeyReply(u8 connHandle, u8 XDATA *p_passkey) large;


/**@brief Reply passkey is not available.
 *
 * @param[in] connHandle 		Connection handle.
 *
 * @return MBIOT_RES_SUCCESS Successfully reply passkey is not available.
 * @return MBIOT_RES_FAIL Unspecific error.
 */
PUBLIC extern u16 BLE_SMP_PasskeyNegativeReply(u8 connHandle) large;


/**@brief Reply the result of user confirm value.
 *
 * @param[in] connHandle 	Connection handle.
 * @param[in] outcome 	  See @ref BLE_SMP_CONFIRM. The result of user confirm.
 *
 * @return MBIOT_RES_SUCCESS Successcully reply user confirm result.
 * @return MBIOT_RES_FAIL The specified connection handle is invalid.
 */
PUBLIC extern u16 BLE_SMP_NumericComparisonConfirmReply(u8 connHandle, u8 outcome) large;


/**@brief Initiate pairing procedure.
 *
 * @note  For master role, BLE SMP starts the Pairing Feature Exchange by sending a Pairing Request.
 * command to the responding device.
 * For slave role, BLE SMP request that the master initiates security with the requested security properties.
 *
 * @param[in] connHandle 	Connection handle.
 *
 * @return MBIOT_RES_SUCCESS Initiate pairing procedure successfully.
 * @return MBIOT_RES_FAIL The specified connection handle is invalid.
 */
PUBLIC extern u16 BLE_SMP_InitiatePairing(u8 connHandle) large;


/**@brief Generate passkey.
 *
 * @param[out] p_passkey 	Pointer to passkey buffer.
 */
PUBLIC extern void BLE_SMP_GeneratePasskey(u8 XDATA *p_passkey) large;
#endif

/**
  @}
*/