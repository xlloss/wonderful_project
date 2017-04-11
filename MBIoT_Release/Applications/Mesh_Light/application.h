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


#define APP_DEBUG_TX_BUFFER_LEN					512
#define APP_DEBUG_RX_BUFFER_LEN					256

/**@brief App heap size. */
#define APP_HEAP_SIZE            				(4*1024)

#define BUTTON_PRESSED							0
#define BUTTON_RELEASED							1

#define IDLE_STATE								0
#define ACTIVE_STATE							1

#define BUTTON_1								GPIO_P31
#define BUTTON_1_MASK							GPIO_PORT_MASK_BIT1

#define RESET_BUTTON							GPIO_P11
#define RESET_MASK								GPIO_PORT_MASK_BIT1
#define GROUP_LED								GPIO_P07
#define NODE_LED								GPIO_P10

#define GENERIC_ONOFF_STATE_OFF					0
#define GENERIC_ONOFF_STATE_ON					1


//generic on/off 
#define TRANSITION_STEP_RESOLUTION_100MS		0x00	
#define TRANSITION_STEP_RESOLUTION_1S			0x01	
#define TRANSITION_STEP_RESOLUTION_10S			0x02	
#define TRANSITION_STEP_RESOLUTION_600S			0x03

#define NODE_LED_ACTIVE							0	
#define GROUP_LED_ACTIVE						1

#define FAST_TIMER_100MS						160
#define FAST_TIMER_1S							1600
#define SLOW_TIMER_10S							16
#define RESET_BUTTON_PRESS_DURATION				256 //160ms

#define BUTTON_STATE_NONE						0
#define BUTTON_STATE_HIGH						1
#define BUTTON_STATE_LOW						2

//event opcode
#define GENERIC_ON_OFF_GET						0x8212
#define GENERIC_ON_OFF_SET_RELIABLE				0x8213
#define GENERIC_ON_OFF_SET_UNRELIABLE			0x8214
#define GENERIC_ON_OFF_STATUS					0x8215

#define MODEL_ID_GENERIC_ONOFF_SERVER			0x1001
#define MODEL_ID_GENERIC_ONOFF_CLIENT			0x9001


//#define MESH_CHARACTERISTIC_OF_LIGHT	
#define MESH_CHARACTERISTIC_OF_SWITCH	


typedef struct MESH_Generic_OnOff{
	u8									onOff;
	u8 									targetOnOff;
	u8									timeResolution;
	u16									transitionTime;
    u16									remainingTime;
	u8 									multiLedSelect;
}MESH_Generic_OnOff;

typedef struct MESH_Provision_UUID{
	u8									manufacture[4];
	u16									productModel;
	u8									characteristic[4];
	u8									bdAddr[6];
}MESH_Provision_UUID;

#endif