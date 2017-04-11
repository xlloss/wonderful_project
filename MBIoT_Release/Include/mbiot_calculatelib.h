/********************************************************************
  File Information:
    FileName:       mbiot_calculatelib.h
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

#ifndef MBIOT_CALCULATELID_H
#define MBIOT_CALCULATELID_H

/**@brief 16-bit Multiplication.
 *
 * @param[in] multiplicand 			16-bit multiplicand.
 * @param[in] multiplier				16-bit multiplier.
 * @return u32				Multiplication of two 16-bit value.
 */
PUBLIC	extern	u32	multiply_u16(u16 multiplicand, u16 multiplier);


/**@brief 16-bit Division.
 *
 * @param[in] dividend 			16-bit dividend.
 * @param[in] divisor				16-bit divisor.
 * @return u16				16-bit quotient.
 */
PUBLIC	extern	u16		divide_u16(u16 dividend, u16 divisor);

/**@brief 16-bit remainder.
 *
 * @param[in] dividend 			16-bit dividend.
 * @param[in] divisor				16-bit divisor.
 * @return u16				16-bit remainder.
 */
PUBLIC	extern	u16		mod_u16(u16  dividend, u16 divisor);


/**@brief 32-bit remainder.
 *
 * @param[in] dividend_h 			High byte of dividend.
 * @param[in] dividend_l 			Low byte of dividend.
 * @param[in] divisior				16-bit divisor.
 * @return u16				16-bit remainder.
 */
PUBLIC  extern  u16 	mod_u32(u16 dividend_h,u16 dividend_l, u16 divisior);

/**@brief The NOP instruction does nothing for 0.5 us.
 *
 */
PUBLIC extern void _nop_(void);

#endif
