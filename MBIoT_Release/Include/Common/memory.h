/********************************************************************
  File Information:
    FileName:       memory_adapter.h
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
  @addtogroup HEAP Heap
  @{
  @brief Definitions and prototypes for the Heap application programming interface.
 */

#ifndef MEMORY_H
#define MEMORY_H
#include "application.h"

typedef u8              APP_HEAP_INDEX;       			/**< Type definition of APP_HEAP_INDEX. */
typedef u8              APP_ALLOC_BLOCK_TYPE; 			/**< Type definition of APP_ALLOC_BLOCK_TYPE. */
typedef u8 XDATA *      APP_HEAP_POINT_TYPE;			/**< Type definition of APP_HEAP_POINT_TYPE. */


/**@brief Definition of Memory module.*/
#define APP_HEAP_BLOCK_SIZE              (32)										/**< Heap block size. */
#define APP_HEAP_BLOCK_SIZE_SHIFT        (5)										/**< Bit mask of heap block size . */
#define APP_HEAP_BLOCK_SIZE_SHIFT_MASK   (APP_HEAP_BLOCK_SIZE-1)					/**< Mask of heap block size. */
#define APP_HEAP_BLOCK_NUMBER            (APP_HEAP_SIZE/APP_HEAP_BLOCK_SIZE)		/**< Total block heap number. */


/**@brief The structure of APP HEAP.*/
typedef struct APP_HEAP_STRUCT
{
    u8   heap[APP_HEAP_BLOCK_SIZE];			/**< APP Heap. */
}APP_HEAP_STRUCT;


PUBLIC  extern  APP_HEAP_STRUCT      XDATA   App_Heap[APP_HEAP_BLOCK_NUMBER];
PUBLIC  extern  APP_HEAP_INDEX       XDATA   App_Heap_Index[APP_HEAP_BLOCK_NUMBER];


/**
 *@brief Initialize APP heap list.
 */
PUBLIC extern void APP_HEAP_Init(void) large;


/**
 * @brief Alloc the request memory
 *
 * @param[in] size 	The size of required memory.
 *
 * @return  Returns a pointer to the allocated memory, or NULL if the request fails.
 */
PUBLIC  extern  APP_HEAP_POINT_TYPE    APP_HEAP_Alloc(u16 size);


/**
 * @brief Deallocates the memory previously allocated by APP_HEAP_Alloc API.
 *
 * @param[in] p_heap 	Pointer to a memory previously allocated.
 */
PUBLIC  extern  void	APP_HEAP_Free(APP_HEAP_POINT_TYPE p_heap) large;

#endif
 /**
  @}
*/