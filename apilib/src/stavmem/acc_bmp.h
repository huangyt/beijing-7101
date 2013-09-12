/*******************************************************************************

File name : acc_bmp.h

Description : AVMEM memory access header file for block move DMA peripheral

COPYRIGHT (C) STMicroelectronics 2000.

Date               Modification                                     Name
----               ------------                                     ----
 8 Dec 1999         Created                                          HG
01 Jun 2001         Add stavmem_ before non API exported symbols     HSdLM
*******************************************************************************/

/* Define to prevent recursive inclusion */
#ifndef __AVMEM_ACC_BMPERIPH_H
#define __AVMEM_ACC_BMPERIPH_H

/* Includes ----------------------------------------------------------------- */

#include "stddefs.h"

#include "avm_acc.h"


/* C++ support */
#ifdef __cplusplus
extern "C" {
#endif

/* Exported Constants ------------------------------------------------------- */


/* Exported Types ----------------------------------------------------------- */


/* Exported Variables ------------------------------------------------------- */

extern stavmem_MethodOperations_t stavmem_BlockMovePeripheral;


/* Exported Macros ---------------------------------------------------------- */


/* Exported Functions ------------------------------------------------------- */



/* C++ support */
#ifdef __cplusplus
}
#endif

#endif /* #ifndef __AVMEM_ACC_BMPERIPH_H */

/* End of acc_bmp.h */