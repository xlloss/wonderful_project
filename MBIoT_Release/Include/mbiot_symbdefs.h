/********************************************************************
  File Information:
    FileName:       mbiot_mbiot_symbdefs.h
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

#ifndef MBIOT_SYMBDEFS_H
#define MBIOT_SYMBDEFS_H


/**@brief Definition of common used types. */
typedef unsigned char   bool;
typedef unsigned char   byte;
typedef unsigned short  word;
typedef unsigned long   dword;


/**@brief Definition of global symbdefs. */
typedef unsigned int 	u16;
typedef int 			s16;
typedef unsigned long	u32;
typedef long			s32;
typedef unsigned char	u8;
typedef signed char		s8; 

/**@brief Definition of Keil 8051 C compiler. */
#ifdef __C51__
#define OPTBYTE byte
#define OPTBOOL bit
#define VOLATILE volatile
#define packed
#define MDATA data
#define XDATA xdata
#define IDATA idata
#define REENTRANT reentrant
#define MCODE code
#define PDATA pdata
#define LARGE large
#define BDATA bdata
#define BIT   bit
#endif


/**@brief Definition of common used constant values. */
#define TRUE            (!0)
#define FALSE           (0)

#define SUCCESS         TRUE
#define FAIL            FALSE


#ifndef NULL
#define NULL    ((void *) 0)
#endif


#ifndef PUBLIC
#define PUBLIC
#endif
   
#ifndef STATIC
#define STATIC static
#endif

#ifndef PRIVATE
#define PRIVATE static
#endif


/**@brief Swap two bytes length of variable.
 *
 * @param[out] x 				Result.
 * @param[in] y 				Variable value.
 */
#define EQU_Swap_u16(x,y)       *(((u8 *)(&(x))))=*(((u8 *)(&(y)))+1);      \
                                *(((u8 *)(&(x)))+1)=*(((u8 *)(&(y))));



/**@brief Swap four bytes length of variable.
 *
 * @param[out] x 				Result.
 * @param[in] y 				Variable value.
 */
#define EQU_Swap_u32(x,y)       *(((u8 *)(&(x))))=*(((u8 *)(&(y)))+3);      \
                                *(((u8 *)(&(x)))+1)=*(((u8 *)(&(y)))+2);    \
                                *(((u8 *)(&(x)))+2)=*(((u8 *)(&(y)))+1);    \
                                *(((u8 *)(&(x)))+3)=*(((u8 *)(&(y))));
                                

/**@brief Swap two bytes length of specified data buffer.
 *
 * @param[out] x 				Pointer to the result buffer.
 * @param[in] y 				Pointer to the variable value.
 */
#define EQU_Swap_u16pt(x,y)     *(((u8 *)(x)))=*(((u8 *)(y))+1);      \
                                *(((u8 *)(x))+1)=*(((u8 *)(y)));


/**@brief Swap four bytes length of specified data buffer.
 *
 * @param[out] x 				Pointer to the result buffer.
 * @param[in] y 				Pointer to the variable value.
 */
#define EQU_Swap_u32pt(x,y)     *(((u8 *)(x)))=*(((u8 *)(y))+3);      \
                                *(((u8 *)(x))+1)=*(((u8 *)(y))+2);    \
                                *(((u8 *)(x))+2)=*(((u8 *)(y))+1);    \
                                *(((u8 *)(x))+3)=*(((u8 *)(y)));


/**@brief Swap two bytes length of constant.
 *
 * @param[out] x 				Pointer to the result buffer.
 * @param[in] y 				Constant value.
 */
#define EQU_Swap_u16C(x,y)      *(((u8 *)(&(x))))=(((u16)(y))>>8);    \
                                *(((u8 *)(&(x)))+1)=(((u16)(y)));


/**@brief Swap four bytes length of constant.
 *
 * @param[out] x 				Pointer to the result buffer.
 * @param[in] y 				Constant value.
 */
#define EQU_Swap_u32C(x,y)      *(((u8 *)(&(x))))=(((u16)(y))>>24);   \
                                *(((u8 *)(&(x)))+1)=(((u16)(y))>>16); \
                                *(((u8 *)(&(x)))+2)=(((u16)(y))>>8);  \
                                *(((u8 *)(&(x)))+3)=(((u16)(y)));

#define struct_offset(parastruct,para)  ( (u16)( &( ((parastruct XDATA *)0) -> para) ) )

#endif





