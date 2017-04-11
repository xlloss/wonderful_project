/********************************************************************
  File Information:
    FileName:       mbiot_xmemlib.h
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

#ifndef MBIOT_XMEMLIB_H
#define MBIOT_XMEMLIB_H


#define memset(d,v,n)                   xmemset((unsigned char XDATA *)(d),(unsigned char)(v),(unsigned char)(n))
#define memset_u16(d,v,n)               xmemset_u16((unsigned char XDATA *)(d),(unsigned char)(v),(unsigned int)(n))
#define memcmp(d,s,n)                   xmemcmp((unsigned char XDATA *)(d),(unsigned char XDATA *)(s),(unsigned char)(n))
#define c2xmemcpy(d,s,n)                comemcpy((unsigned char XDATA *)(d),(unsigned char code *)(s),(unsigned char)(n));
#define memcpy(d,s,n)                   xmemcpy((unsigned char XDATA *)(d),(unsigned char XDATA *)(s),(unsigned char)(n));
#define memcpy_u16(d,s,n)               xmemcpy_u16((unsigned char XDATA *)(d),(unsigned char XDATA *)(s),(unsigned int)(n));


/**@brief Copy block of XDATA memory.
 *
 * @param[out] dst 			Pointer to the destination XDATA memroy.
 * @param[in] src 			Pointer to the source of XDATA to be copied.
 * @param[in] n				Number of bytes to copy.
 */
PUBLIC  extern  void    xmemcpy(unsigned char XDATA *dst, unsigned char XDATA *src, unsigned char n);


/**@brief Copy block of XDATA memory. For length over 255 bytes.
 *
 * @param[out] dst 			Pointer to the destination XDATA memroy.
 * @param[in] src 			Pointer to the source of XDATA to be copied.
 * @param[in] n				Number of bytes to copy.
 */
PUBLIC  extern  void    xmemcpy_u16(unsigned char XDATA *dst, unsigned char XDATA *src, unsigned int n);


/**@brief Fill block of XDATA memory.
 *
 * @param[out] dst 			Pointer to the block of memory to fill.
 * @param[in] val 			Value to be set.
 * @param[in] n				Number of bytes to be set to the value.
 */
PUBLIC  extern  void    xmemset(unsigned char XDATA *dst, unsigned char val, unsigned char n);


/**@brief Fill block of XDATA memory. For length over 255 bytes.
 *
 * @param[out] dst 			Pointer to the block of memory to fill.
 * @param[in] val 			Value to be set.
 * @param[in] n				Number of bytes to be set to the value.
 */
PUBLIC  extern  void    xmemset_u16(unsigned char XDATA *dst, unsigned char val, unsigned int n);


/**@brief Compare two block of XDATA memroy.
 *
 * @param[in] dst 			Pointer to the block of XDATA memory.
 * @param[in] src 			Pointer to the block of XDATA memory.
 * @param[in] n				Number of bytes to compare.
 *
  * @return TRUE            If the two block content are all matched.
 */
PUBLIC  extern  bit     xmemcmp(unsigned char XDATA *dst, unsigned char XDATA *src, unsigned char n);


/**@brief Compare block of XDATA memroy and block of MCODE.
 *
 * @param[in] dst 			Pointer to the block of XDATA memory.
 * @param[in] src 			Pointer to the block of MCODE.
 * @param[in] n				Number of bytes to compare.
 *
  * @return TRUE	        If the two block content are all matched.
 */
PUBLIC  extern  bit     comemcmp(unsigned char XDATA *dst, unsigned char MCODE *src, unsigned char n);


/**@brief Move block of XDATA memory.
 *
 * @param[out] dst 			Pointer to the destination XDATA memroy where the content is to be copied.
 * @param[in] src 			Pointer to the source of data to be copied.
 * @param[in] n				Number of bytes to copy.
 */
PUBLIC  extern  void    xmemmove(unsigned char XDATA *dst, unsigned char XDATA *src, unsigned char n);


/**@brief Copy content from MCODE to  XDATA memory.
 *
 * @param[out] dst 			Pointer to the destination XDATA memroy.
 * @param[in] src 			Pointer to the source of MCODE to be copied.
 * @param[in] n				Number of bytes to copy.
 */
PUBLIC  extern  void    comemcpy(unsigned char XDATA *d, unsigned char code *s, unsigned char n);


/**@brief Copy content from MCODE to XDATA memory. For length over 255 bytes.
 *
 * @param[out] dst 			Pointer to the destination XDATA memroy.
 * @param[in] src 			Pointer to the source of MCODE to be copied.
 * @param[in] n				Number of bytes to copy.
 */
PUBLIC  extern  void    comemcpy_u16(unsigned char XDATA *dst, unsigned char code *src, unsigned int n);


/**@brief Swap contect for each four bytes and copy to block of XDATA memory.
 *
 * @param[out] dst 			Pointer to the destination XDATA memroy.
 * @param[in] src 			Pointer to the source of XDATA to be copied.
 * @param[in] n				Number of bytes to copy.
 */
PUBLIC  extern  void    xmemcpy_32_swap(unsigned char XDATA *dst, unsigned char XDATA *src, unsigned char n_DW);


#endif
