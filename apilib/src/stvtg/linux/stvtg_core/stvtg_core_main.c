/*****************************************************************************
 *
 *  Module      : stvtg_core
 *  Date        : 15-10-2005
 *  Author      : ATROUSWA
 *  Description :
 *  Copyright   : STMicroelectronics (c)2005
 *****************************************************************************/

/* Requires   MODULE         defined on the command line */
/* Requires __KERNEL__       defined on the command line */
/* Requires __SMP__          defined on the command line for SMP systems   */
/* Requires EXPORT_SYMTAB    defined on the command line to export symbols */

#include <linux/init.h>    /* Initiliasation support */
#include <linux/module.h>  /* Module support */
#include <linux/kernel.h>  /* Kernel support */
#include <linux/version.h> /* Kernel version */


/*** MODULE INFO *************************************************************/
MODULE_AUTHOR("WALID ATROUS");
MODULE_DESCRIPTION("");
MODULE_SUPPORTED_DEVICE("");
MODULE_LICENSE("ST Microelectronics");

/*** PROTOTYPES **************************************************************/



/*** MODULE PARAMETERS *******************************************************/



/*** GLOBAL VARIABLES *********************************************************/


/*** EXPORTED SYMBOLS ********************************************************/


/*** LOCAL TYPES *********************************************************/


/*** LOCAL CONSTANTS ********************************************************/


/*** LOCAL VARIABLES *********************************************************/



/*** METHODS ****************************************************************/

/*=============================================================================

   stvtg_core_init_module

   Initialise the module (initialise the access routines (fops) and set the
   major number. Aquire any resources needed and setup internal structures.

  ===========================================================================*/
static int __init stvtg_core_init_module(void)
{
   return(0);
}


/*=============================================================================

   stvtg_core_cleanup_module

   Realease any resources allocaed to this module before the module is
   unloaded.

  ===========================================================================*/
static void __exit stvtg_core_cleanup_module(void)
{
}


/*** MODULE LOADING ******************************************************/


/* Tell the module system these are our entry points. */

module_init(stvtg_core_init_module);
module_exit(stvtg_core_cleanup_module);
