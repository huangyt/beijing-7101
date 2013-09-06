/* ----------------------------------------------------------------------------

File Name: errors.c

Description: get error as string representation from error code.

Copyright (C) 2002 STMicroelectronics

---------------------------------------------------------------------------- */

/* from err.pl autogenerated file error.c */

#include "stddefs.h"
#include "staud.h"
#include "stavmem.h"
#include "stboot.h"
#include "stcc.h"
/*#include "stcfg.h"*/
#include "stclkrv.h"
#include "stddefs.h"
#include "stdenc.h"
#include "stevt.h"
#include "sti2c.h"
#include "stgfx.h"
#include "stlayer.h"
/*#include "stosd.h"*/

#ifdef DVD_TRANSPORT_STPTI
#include "stpti.h"
#endif

/*#include "stpwm.h"
#include "stscart.h"*/
/*#include "stsubt.h"/
#include "sttbx.h"
/*#include "sttsmux.h"*/
#include "stttx.h"
#include "sttuner.h"
#include "stuart.h"
#include "stvbi.h"
#include "stvid.h"
#include "stvmix.h"
#include "stvout.h"
#include "stvtg.h"

char *GetErrorText(ST_ErrorCode_t ST_ErrorCode)
{
    switch(ST_ErrorCode)
    {
        /* staud.h */
        case STAUD_ERROR_DECODER_RUNNING:
            return("STAUD_ERROR_DECODER_RUNNING");
        case STAUD_ERROR_DECODER_STOPPED:
            return("STAUD_ERROR_DECODER_STOPPED");
        case STAUD_ERROR_DECODER_PREPARING:
            return("STAUD_ERROR_DECODER_PREPARING");
        case STAUD_ERROR_DECODER_PREPARED:
            return("STAUD_ERROR_DECODER_PREPARED");
        case STAUD_ERROR_DECODER_PAUSING:
            return("STAUD_ERROR_DECODER_PAUSING");
        case STAUD_ERROR_DECODER_NOT_PAUSING:
            return("STAUD_ERROR_DECODER_NOT_PAUSING");
        case STAUD_ERROR_INVALID_STREAM_ID:
            return("STAUD_ERROR_INVALID_STREAM_ID");
        case STAUD_ERROR_INVALID_STREAM_TYPE:
            return("STAUD_ERROR_INVALID_STREAM_TYPE");
        case STAUD_ERROR_INVALID_DEVICE:
            return("STAUD_ERROR_INVALID_DEVICE");
        case STAUD_ERROR_EVT_REGISTER:
            return("STAUD_ERROR_EVT_REGISTER");
        case STAUD_ERROR_EVT_UNREGISTER:
            return("STAUD_ERROR_EVT_UNREGISTER");
        case STAUD_ERROR_CLKRV_OPEN:
            return("STAUD_ERROR_CLKRV_OPEN");
        case STAUD_ERROR_CLKRV_CLOSE:
            return("STAUD_ERROR_CLKRV_CLOSE");
        case STAUD_ERROR_AVSYNC_TASK:
            return("STAUD_ERROR_AVSYNC_TASK");


        /* stavmem.h */
        case STAVMEM_ERROR_FUNCTION_NOT_IMPLEMENTED:
            return("STAVMEM_ERROR_FUNCTION_NOT_IMPLEMENTED");
        case STAVMEM_ERROR_MAX_PARTITION:
            return("STAVMEM_ERROR_MAX_PARTITION");
        case STAVMEM_ERROR_INVALID_PARTITION_HANDLE:
            return("STAVMEM_ERROR_INVALID_PARTITION_HANDLE");
        case STAVMEM_ERROR_PARTITION_FULL:
            return("STAVMEM_ERROR_PARTITION_FULL");
        case STAVMEM_ERROR_CREATED_PARTITION:
            return("STAVMEM_ERROR_CREATED_PARTITION");
        case STAVMEM_ERROR_MAX_BLOCKS:
            return("STAVMEM_ERROR_MAX_BLOCKS");
        case STAVMEM_ERROR_INVALID_BLOCK_HANDLE:
            return("STAVMEM_ERROR_INVALID_BLOCK_HANDLE");
        case STAVMEM_ERROR_ALLOCATED_BLOCK:
            return("STAVMEM_ERROR_ALLOCATED_BLOCK");
        case STAVMEM_ERROR_BLOCK_IN_FORBIDDEN_ZONE:
            return("STAVMEM_ERROR_BLOCK_IN_FORBIDDEN_ZONE");
        case STAVMEM_ERROR_GPDMA_OPEN:
            return("STAVMEM_ERROR_GPDMA_OPEN");
        case STAVMEM_ERROR_GPDMA_CLOSE:
            return("STAVMEM_ERROR_GPDMA_CLOSE");


        /* stboot.h */
        case STBOOT_ERROR_KERNEL_INIT:
            return("STBOOT_ERROR_KERNEL_INIT");
        case STBOOT_ERROR_KERNEL_START:
            return("STBOOT_ERROR_KERNEL_START");
        case STBOOT_ERROR_INTERRUPT_INIT:
            return("STBOOT_ERROR_INTERRUPT_INIT");
        case STBOOT_ERROR_INTERRUPT_ENABLE:
            return("STBOOT_ERROR_INTERRUPT_ENABLE");
        case STBOOT_ERROR_UNKNOWN_BACKEND:
            return("STBOOT_ERROR_UNKNOWN_BACKEND");
        case STBOOT_ERROR_BACKEND_MISMATCH:
            return("STBOOT_ERROR_BACKEND_MISMATCH");
        case STBOOT_ERROR_INVALID_DCACHE:
            return("STBOOT_ERROR_INVALID_DCACHE");
        case STBOOT_ERROR_SDRAM_INIT:
            return("STBOOT_ERROR_SDRAM_INIT");


        /* stcc.h */
        case STCC_ERROR_DECODER_RUNNING:
            return("STCC_ERROR_DECODER_RUNNING");
        case STCC_ERROR_DECODER_STOPPED:
            return("STCC_ERROR_DECODER_STOPPED");
        case STCC_ERROR_VBI_UNKNOWN:
            return("STCC_ERROR_VBI_UNKNOWN");
        case STCC_ERROR_VBI_ACCESS:
            return("STCC_ERROR_VBI_ACCESS");
        case STCC_ERROR_EVT_REGISTER:
            return("STCC_ERROR_EVT_REGISTER");
        case STCC_ERROR_EVT_UNREGISTER:
            return("STCC_ERROR_EVT_UNREGISTER");
        case STCC_ERROR_EVT_SUBSCRIBE:
            return("STCC_ERROR_EVT_SUBSCRIBE");
        case STCC_ERROR_TASK_CALL:
            return("STCC_ERROR_TASK_CALL");


#if 0
        /* Out for the moment because STCC and STCFG have the same driver ID */
        /* stcfg.h */
        case STCFG_ERROR_INVALID_CMD:
            return("STCFG_ERROR_INVALID_CMD");
#endif


        /* stclkrv.h */
        case STCLKRV_ERROR_HANDLER_INSTALL:
            return("STCLKRV_ERROR_HANDLER_INSTALL");
        case STCLKRV_ERROR_PCR_UNAVAILABLE:
            return("STCLKRV_ERROR_PCR_UNAVAILABLE");
        /*case STCLKRV_ERROR_PWM:
            return("STCLKRV_ERROR_PWM");*/
        case STCLKRV_ERROR_EVT_REGISTER:
            return("STCLKRV_ERROR_EVT_REGISTER");
        /*case STCLKRV_ERROR_THRESHOLD:
            return("STCLKRV_ERROR_THRESHOLD");*/


        /* stddefs.h */
        case ST_ERROR_BAD_PARAMETER:
            return("ST_ERROR_BAD_PARAMETER");
        case ST_ERROR_NO_MEMORY:
            return("ST_ERROR_NO_MEMORY");
        case ST_ERROR_UNKNOWN_DEVICE:
            return("ST_ERROR_UNKNOWN_DEVICE");
        case ST_ERROR_ALREADY_INITIALIZED:
            return("ST_ERROR_ALREADY_INITIALIZED");
        case ST_ERROR_NO_FREE_HANDLES:
            return("ST_ERROR_NO_FREE_HANDLES");

        case ST_ERROR_OPEN_HANDLE:
            return("ST_ERROR_OPEN_HANDLE");
        case ST_ERROR_INVALID_HANDLE:
            return("ST_ERROR_INVALID_HANDLE");
        case ST_ERROR_FEATURE_NOT_SUPPORTED:
            return("ST_ERROR_FEATURE_NOT_SUPPORTED");
        case ST_ERROR_INTERRUPT_INSTALL:
            return("ST_ERROR_INTERRUPT_INSTALL");
        case ST_ERROR_INTERRUPT_UNINSTALL:
            return("ST_ERROR_INTERRUPT_UNINSTALL");
        case ST_ERROR_TIMEOUT:
            return("ST_ERROR_TIMEOUT");
        case ST_ERROR_DEVICE_BUSY:
            return("ST_ERROR_DEVICE_BUSY");


        /* stdenc.h */
        case STDENC_ERROR_I2C:
            return("STDENC_ERROR_I2C");
        case STDENC_ERROR_INVALID_ENCODING_MODE:
            return("STDENC_ERROR_INVALID_ENCODING_MODE");


        /* stevt.h */
        case STEVT_ERROR_INVALID_EVENT_ID:
            return("STEVT_ERROR_INVALID_EVENT_ID");
        case STEVT_ERROR_INVALID_SUBSCRIBER_ID:
            return("STEVT_ERROR_INVALID_SUBSCRIBER_ID");
        case STEVT_ERROR_ALREADY_SUBSCRIBED:
            return("STEVT_ERROR_ALREADY_SUBSCRIBED");
        case STEVT_ERROR_ALREADY_REGISTERED:
            return("STEVT_ERROR_ALREADY_REGISTERED");
        case STEVT_ERROR_NO_MORE_SPACE:
            return("STEVT_ERROR_NO_MORE_SPACE");
        case STEVT_ERROR_INVALID_EVENT_NAME:
            return("STEVT_ERROR_INVALID_EVENT_NAME");
        case STEVT_ERROR_ALREADY_UNREGISTERED:
            return("STEVT_ERROR_ALREADY_UNREGISTERED");
        case STEVT_ERROR_MISSING_NOTIFY_CALLBACK:
            return("STEVT_ERROR_MISSING_NOTIFY_CALLBACK");
        case STEVT_ERROR_NOT_SUBSCRIBED:
            return("STEVT_ERROR_NOT_SUBSCRIBED");


        /* stgfx.h */
        case STGFX_ERROR_INVALID_GC:
            return("STGFX_ERROR_INVALID_GC");
        case STGFX_ERROR_XYL_POOL_EXCEEDED:
            return("STGFX_ERROR_XYL_POOL_EXCEEDED");
        case STGFX_ERROR_INVALID_FONT:
            return("STGFX_ERROR_INVALID_FONT");
        case STGFX_ERROR_NOT_CHARACTER:
            return("STGFX_ERROR_NOT_CHARACTER");
        case STGFX_ERROR_STEVT_DRV:
            return("STGFX_ERROR_STEVT_DRV");
        case STGFX_ERROR_STBLIT_DRV:
            return("STGFX_ERROR_STBLIT_DRV");
        case STGFX_ERROR_STBLIT_BLIT:
            return("STGFX_ERROR_STBLIT_BLIT");
        case STGFX_ERROR_STGXOBJ:
            return("STGFX_ERROR_STGXOBJ");
        case STGFX_ERROR_STAVMEM:
            return("STGFX_ERROR_STAVMEM");


        /* sti2c.h */
        case STI2C_ERROR_LINE_STATE:
            return("STI2C_ERROR_LINE_STATE");
        case STI2C_ERROR_STATUS:
            return("STI2C_ERROR_STATUS");
        case STI2C_ERROR_ADDRESS_ACK:
            return("STI2C_ERROR_ADDRESS_ACK");
        case STI2C_ERROR_WRITE_ACK:
            return("STI2C_ERROR_WRITE_ACK");
        case STI2C_ERROR_NO_DATA:
            return("STI2C_ERROR_NO_DATA");
        case STI2C_ERROR_PIO:
            return("STI2C_ERROR_PIO");
        case STI2C_ERROR_BUS_IN_USE:
            return("STI2C_ERROR_BUS_IN_USE");
        case STI2C_ERROR_EVT_REGISTER:
            return("STI2C_ERROR_EVT_REGISTER");


        /* stlayer.h */
        case STLAYER_ERROR_INVALID_INPUT_RECTANGLE:
            return("STLAYER_ERROR_INVALID_INPUT_RECTANGLE");
        case STLAYER_ERROR_INVALID_OUTPUT_RECTANGLE:
            return("STLAYER_ERROR_INVALID_OUTPUT_RECTANGLE");
        case STLAYER_ERROR_NO_FREE_HANDLES:
            return("STLAYER_ERROR_NO_FREE_HANDLES");
        case STLAYER_ERROR_IORECTANGLES_NOT_ADJUSTABLE:
            return("STLAYER_ERROR_IORECTANGLES_NOT_ADJUSTABLE");
        case STLAYER_ERROR_INVALID_LAYER_TYPE:
            return("STLAYER_ERROR_INVALID_LAYER_TYPE");
        case STLAYER_ERROR_USER_ALLOCATION_NOT_ALLOWED:
            return("STLAYER_ERROR_USER_ALLOCATION_NOT_ALLOWED");
        case STLAYER_ERROR_OVERLAP_VIEWPORT:
            return("STLAYER_ERROR_OVERLAP_VIEWPORT");
        case STLAYER_ERROR_NO_AV_MEMORY:
            return("STLAYER_ERROR_NO_AV_MEMORY");
        case STLAYER_ERROR_OUT_OF_LAYER:
            return("STLAYER_ERROR_OUT_OF_LAYER");
        case STLAYER_ERROR_OUT_OF_BITMAP:
            return("STLAYER_ERROR_OUT_OF_BITMAP");
        case STLAYER_ERROR_INSIDE_LAYER:
            return("STLAYER_ERROR_INSIDE_LAYER");
        case STLAYER_ERROR_EVENT_REGISTRATION:
            return("STLAYER_ERROR_EVENT_REGISTRATION");

#if 0 /*yxl 2005-08-24 cancel this section*/

        /* stosd.h */
        case STOSD_ERROR_INVALID_REGION_HANDLE:
            return("STOSD_ERROR_INVALID_REGION_HANDLE");
        case STOSD_ERROR_INVALID_DST_REGION_HANDLE:
            return("STOSD_ERROR_INVALID_DST_REGION_HANDLE");
        case STOSD_ERROR_INVALID_SRC_REGION_HANDLE:
            return("STOSD_ERROR_INVALID_SRC_REGION_HANDLE");
        case STOSD_ERROR_MAX_REGION:
            return("STOSD_ERROR_MAX_REGION");
        case STOSD_ERROR_NO_AV_SPACE:
            return("STOSD_ERROR_NO_AV_SPACE");
        case STOSD_ERROR_OUT_OF_REGION:
            return("STOSD_ERROR_OUT_OF_REGION");
        case STOSD_ERROR_TRUE_COLOR_REGION:
            return("STOSD_ERROR_TRUE_COLOR_REGION");
        case STOSD_ERROR_EXCEED_ORIGINAL_REGION_MEMORY_SIZE:
            return("STOSD_ERROR_EXCEED_ORIGINAL_REGION_MEMORY_SIZE");
        case STOSD_ERROR_DEVICE_NOT_INITIALIZED:
            return("STOSD_ERROR_DEVICE_NOT_INITIALIZED");
#endif 

#ifdef DVD_TRANSPORT_STPTI
        /* stpti.h */
        case STPTI_ERROR_ALREADY_WAITING_ON_SLOT:
            return("STPTI_ERROR_ALREADY_WAITING_ON_SLOT");
        case STPTI_ERROR_ALT_OUT_ALREADY_IN_USE:
            return("STPTI_ERROR_ALT_OUT_ALREADY_IN_USE");
        case STPTI_ERROR_ALT_OUT_TYPE_NOT_SUPPORTED:
            return("STPTI_ERROR_ALT_OUT_TYPE_NOT_SUPPORTED");
        case STPTI_ERROR_BUFFER_NOT_LINKED:
            return("STPTI_ERROR_BUFFER_NOT_LINKED");
        case STPTI_ERROR_CAROUSEL_ALREADY_ALLOCATED:
            return("STPTI_ERROR_CAROUSEL_ALREADY_ALLOCATED");
        case STPTI_ERROR_CAROUSEL_ENTRY_INSERTED:
            return("STPTI_ERROR_CAROUSEL_ENTRY_INSERTED");
        case STPTI_ERROR_CAROUSEL_LOCKED_BY_DIFFERENT_SESSION:
            return("STPTI_ERROR_CAROUSEL_LOCKED_BY_DIFFERENT_SESSION");
        case STPTI_ERROR_CAROUSEL_NOT_LOCKED:
            return("STPTI_ERROR_CAROUSEL_NOT_LOCKED");
        case STPTI_ERROR_CAROUSEL_OUTPUT_ONLY_ON_NULL_SLOT:
            return("STPTI_ERROR_CAROUSEL_OUTPUT_ONLY_ON_NULL_SLOT");
        case STPTI_ERROR_COLLECT_FOR_ALT_OUT_ONLY_ON_NULL_SLOT:
            return("STPTI_ERROR_COLLECT_FOR_ALT_OUT_ONLY_ON_NULL_SLOT");
        case STPTI_ERROR_CORRUPT_DATA_IN_BUFFER:
            return("STPTI_ERROR_CORRUPT_DATA_IN_BUFFER");
        case STPTI_ERROR_DESCRAMBLER_ALREADY_ASSOCIATED:
            return("STPTI_ERROR_DESCRAMBLER_ALREADY_ASSOCIATED");
        case STPTI_ERROR_DESCRAMBLER_NOT_ASSOCIATED:
            return("STPTI_ERROR_DESCRAMBLER_NOT_ASSOCIATED");
        case STPTI_ERROR_DESCRAMBLER_TYPE_NOT_SUPPORTED:
            return("STPTI_ERROR_DESCRAMBLER_TYPE_NOT_SUPPORTED");
        case STPTI_ERROR_DMA_UNAVAILABLE:
            return("STPTI_ERROR_DMA_UNAVAILABLE");
        case STPTI_ERROR_ENTRY_ALREADY_INSERTED:
            return("STPTI_ERROR_ENTRY_ALREADY_INSERTED");
        case STPTI_ERROR_ENTRY_IN_USE:
            return("STPTI_ERROR_ENTRY_IN_USE");
        case STPTI_ERROR_ENTRY_NOT_IN_CAROUSEL:
            return("STPTI_ERROR_ENTRY_NOT_IN_CAROUSEL");
        case STPTI_ERROR_EVENT_QUEUE_EMPTY:
            return("STPTI_ERROR_EVENT_QUEUE_EMPTY");
        case STPTI_ERROR_EVENT_QUEUE_FULL:
            return("STPTI_ERROR_EVENT_QUEUE_FULL");
        case STPTI_ERROR_FILTER_ALREADY_ASSOCIATED:
            return("STPTI_ERROR_FILTER_ALREADY_ASSOCIATED");
        case STPTI_ERROR_FLUSH_FILTERS_NOT_SUPPORTED:
            return("STPTI_ERROR_FLUSH_FILTERS_NOT_SUPPORTED");
        case STPTI_ERROR_FUNCTION_NOT_SUPPORTED:
            return("STPTI_ERROR_FUNCTION_NOT_SUPPORTED");
        case STPTI_ERROR_INCOMPLETE_PES_IN_BUFFER:
            return("STPTI_ERROR_INCOMPLETE_PES_IN_BUFFER");
        case STPTI_ERROR_INCOMPLETE_SECTION_IN_BUFFER:
            return("STPTI_ERROR_INCOMPLETE_SECTION_IN_BUFFER");
        case STPTI_ERROR_INDEX_INVALID_ASSOCIATION:
            return("STPTI_ERROR_INDEX_INVALID_ASSOCIATION");
        case STPTI_ERROR_INDEX_INVALID_HANDLE:
            return("STPTI_ERROR_INDEX_INVALID_HANDLE");
        case STPTI_ERROR_INDEX_NONE_FREE:
            return("STPTI_ERROR_INDEX_NONE_FREE");
        case STPTI_ERROR_INDEX_NOT_ASSOCIATED:
            return("STPTI_ERROR_INDEX_NOT_ASSOCIATED");
        case STPTI_ERROR_INDEX_PID_ALREADY_ASSOCIATED:
            return("STPTI_ERROR_INDEX_PID_ALREADY_ASSOCIATED");
        case STPTI_ERROR_INDEX_SLOT_ALREADY_ASSOCIATED:
            return("STPTI_ERROR_INDEX_SLOT_ALREADY_ASSOCIATED");
        case STPTI_ERROR_INTERRUPT_QUEUE_EMPTY:
            return("STPTI_ERROR_INTERRUPT_QUEUE_EMPTY");
        case STPTI_ERROR_INTERRUPT_QUEUE_FULL:
            return("STPTI_ERROR_INTERRUPT_QUEUE_FULL");
        case STPTI_ERROR_INVALID_ALLOW_OUTPUT_TYPE:
            return("STPTI_ERROR_INVALID_ALLOW_OUTPUT_TYPE");
        case STPTI_ERROR_INVALID_ALTERNATE_OUTPUT_TYPE:
            return("STPTI_ERROR_INVALID_ALTERNATE_OUTPUT_TYPE");
        case STPTI_ERROR_INVALID_BUFFER_HANDLE:
            return("STPTI_ERROR_INVALID_BUFFER_HANDLE");
        case STPTI_ERROR_INVALID_CAROUSEL_ENTRY_HANDLE:
            return("STPTI_ERROR_INVALID_CAROUSEL_ENTRY_HANDLE");
        case STPTI_ERROR_INVALID_CAROUSEL_HANDLE:
            return("STPTI_ERROR_INVALID_CAROUSEL_HANDLE");
        case STPTI_ERROR_INVALID_CD_FIFO_ADDRESS:
            return("STPTI_ERROR_INVALID_CD_FIFO_ADDRESS");
        case STPTI_ERROR_INVALID_DESCRAMBLER_ASSOCIATION:
            return("STPTI_ERROR_INVALID_DESCRAMBLER_ASSOCIATION");
        case STPTI_ERROR_INVALID_DESCRAMBLER_HANDLE:
            return("STPTI_ERROR_INVALID_DESCRAMBLER_HANDLE");
        case STPTI_ERROR_INVALID_DEVICE:
            return("STPTI_ERROR_INVALID_DEVICE");
        case STPTI_ERROR_INVALID_FILTER_DATA:
            return("STPTI_ERROR_INVALID_FILTER_DATA");
        case STPTI_ERROR_INVALID_FILTER_HANDLE:
            return("STPTI_ERROR_INVALID_FILTER_HANDLE");
        case STPTI_ERROR_INVALID_FILTER_OPERATING_MODE:
            return("STPTI_ERROR_INVALID_FILTER_OPERATING_MODE");
        case STPTI_ERROR_INVALID_FILTER_REPEAT_MODE:
            return("STPTI_ERROR_INVALID_FILTER_REPEAT_MODE");
        case STPTI_ERROR_INVALID_FILTER_TYPE:
            return("STPTI_ERROR_INVALID_FILTER_TYPE");
        case STPTI_ERROR_INVALID_INJECTION_TYPE:
            return("STPTI_ERROR_INVALID_INJECTION_TYPE");
        case STPTI_ERROR_INVALID_KEY_USAGE:
            return("STPTI_ERROR_INVALID_KEY_USAGE");
        case STPTI_ERROR_INVALID_LOADER_OPTIONS:
            return("STPTI_ERROR_INVALID_LOADER_OPTIONS");
        case STPTI_ERROR_INVALID_PARITY:
            return("STPTI_ERROR_INVALID_PARITY");
        case STPTI_ERROR_INVALID_PES_START_CODE:
            return("STPTI_ERROR_INVALID_PES_START_CODE");
        case STPTI_ERROR_INVALID_PID:
            return("STPTI_ERROR_INVALID_PID");
        case STPTI_ERROR_INVALID_SESSION_HANDLE:
            return("STPTI_ERROR_INVALID_SESSION_HANDLE");
        case STPTI_ERROR_INVALID_SIGNAL_HANDLE:
            return("STPTI_ERROR_INVALID_SIGNAL_HANDLE");
        case STPTI_ERROR_INVALID_SLOT_HANDLE:
            return("STPTI_ERROR_INVALID_SLOT_HANDLE");
        case STPTI_ERROR_INVALID_SLOT_TYPE:
            return("STPTI_ERROR_INVALID_SLOT_TYPE");
        case STPTI_ERROR_INVALID_SUPPORTED_TC_CODE:
            return("STPTI_ERROR_INVALID_SUPPORTED_TC_CODE");
        case STPTI_ERROR_NOT_ALLOCATED_IN_SAME_SESSION:
            return("STPTI_ERROR_NOT_ALLOCATED_IN_SAME_SESSION");
        case STPTI_ERROR_NOT_INITIALISED:
            return("STPTI_ERROR_NOT_INITIALISED");
        case STPTI_ERROR_NOT_ON_SAME_DEVICE:
            return("STPTI_ERROR_NOT_ON_SAME_DEVICE");
        case STPTI_ERROR_NOT_SUPPORTED_FOR_DTV:
            return("STPTI_ERROR_NOT_SUPPORTED_FOR_DTV");
        case STPTI_ERROR_NO_FREE_DESCRAMBLERS:
            return("STPTI_ERROR_NO_FREE_DESCRAMBLERS");
        case STPTI_ERROR_NO_FREE_DMAS:
            return("STPTI_ERROR_NO_FREE_DMAS");
        case STPTI_ERROR_NO_FREE_FILTERS:
            return("STPTI_ERROR_NO_FREE_FILTERS");
        case STPTI_ERROR_NO_FREE_SLOTS:
            return("STPTI_ERROR_NO_FREE_SLOTS");
        case STPTI_ERROR_NO_PACKET:
            return("STPTI_ERROR_NO_PACKET");
        case STPTI_ERROR_OFFSET_EXCEEDS_PACKET_SIZE:
            return("STPTI_ERROR_OFFSET_EXCEEDS_PACKET_SIZE");
        case STPTI_ERROR_ONLY_ONE_SIGNAL_PER_BUFFER:
            return("STPTI_ERROR_ONLY_ONE_SIGNAL_PER_BUFFER");
        case STPTI_ERROR_ONLY_ONE_SIGNAL_PER_SLOT:
            return("STPTI_ERROR_ONLY_ONE_SIGNAL_PER_SLOT");
        case STPTI_ERROR_PID_ALREADY_COLLECTED:
            return("STPTI_ERROR_PID_ALREADY_COLLECTED");
        case STPTI_ERROR_SET_MATCH_ACTION_NOT_SUPPORTED:
            return("STPTI_ERROR_SET_MATCH_ACTION_NOT_SUPPORTED");
        case STPTI_ERROR_SIGNAL_ABORTED:
            return("STPTI_ERROR_SIGNAL_ABORTED");
        case STPTI_ERROR_SIGNAL_EVERY_PACKET_ONLY_ON_PES_SLOT:
            return("STPTI_ERROR_SIGNAL_EVERY_PACKET_ONLY_ON_PES_SLOT");
        case STPTI_ERROR_SLOT_FLAG_NOT_SUPPORTED:
            return("STPTI_ERROR_SLOT_FLAG_NOT_SUPPORTED");
        case STPTI_ERROR_SLOT_NOT_ASSOCIATED:
            return("STPTI_ERROR_SLOT_NOT_ASSOCIATED");
        case STPTI_ERROR_SLOT_NOT_RAW_MODE:
            return("STPTI_ERROR_SLOT_NOT_RAW_MODE");
        case STPTI_ERROR_SLOT_NOT_SIGNAL_EVERY_PACKET:
            return("STPTI_ERROR_SLOT_NOT_SIGNAL_EVERY_PACKET");
        case STPTI_ERROR_STORE_LAST_HEADER_ONLY_ON_RAW_SLOT:
            return("STPTI_ERROR_STORE_LAST_HEADER_ONLY_ON_RAW_SLOT");
        case STPTI_ERROR_UNABLE_TO_ENABLE_FILTERS:
            return("STPTI_ERROR_UNABLE_TO_ENABLE_FILTERS");
        case STPTI_ERROR_USER_BUFFER_NOT_ALIGNED:
            return("STPTI_ERROR_USER_BUFFER_NOT_ALIGNED");
        case STPTI_ERROR_WILDCARD_PID_ALREADY_SET:
            return("STPTI_ERROR_WILDCARD_PID_ALREADY_SET");
        case STPTI_ERROR_WILDCARD_PID_NOT_SUPPORTED:
            return("STPTI_ERROR_WILDCARD_PID_NOT_SUPPORTED");
#endif

#if 0 /*yxl 2005-08-24 cancel this section*/
        /* stpwm.h */
        case STPWM_ERROR_PIO:
            return("STPWM_ERROR_PIO");
#endif
#if 0 /*yxl 2006-11-23 cancel this section*/
        /* stscart.h */
        case STSCART_ERROR_CONFLICT_ROUTE:
            return("STSCART_ERROR_CONFLICT_ROUTE");
        case STSCART_ERROR_INVALID_GAIN:
            return("STSCART_ERROR_INVALID_GAIN");
        case STSCART_ERROR_I2C:
            return("STSCART_ERROR_I2C");
        case STSCART_ERROR_NOT_INITIALIZED:
            return("STSCART_ERROR_NOT_INITIALIZED");
        case STSCART_ERROR_EVENT_REGISTER:
            return("STSCART_ERROR_EVENT_REGISTER");


        /* stsubt.h */
        case STSUBT_ERROR_INVALID_HANDLE:
            return("STSUBT_ERROR_INVALID_HANDLE");
        case STSUBT_ERROR_ALREADY_INITIALIZED:
            return("STSUBT_ERROR_ALREADY_INITIALIZED");
        case STSUBT_ERROR_TOO_MANY_INSTANCES:
            return("STSUBT_ERROR_TOO_MANY_INSTANCES");
        case STSUBT_ERROR_ALLOCATING_MEMORY:
            return("STSUBT_ERROR_ALLOCATING_MEMORY");
        case STSUBT_ERROR_OPEN_HANDLE:
            return("STSUBT_ERROR_OPEN_HANDLE");
        case STSUBT_ERROR_OPENING_STEVT:
            return("STSUBT_ERROR_OPENING_STEVT");
        case STSUBT_ERROR_OPENING_STCLKRV:
            return("STSUBT_ERROR_OPENING_STCLKRV");
        case STSUBT_ERROR_OPENING_STPTI:
            return("STSUBT_ERROR_OPENING_STPTI");
        case STSUBT_ERROR_CLOSING_STPTI:
            return("STSUBT_ERROR_CLOSING_STPTI");
        case STSUBT_ERROR_CLOSING_STEVT:
            return("STSUBT_ERROR_CLOSING_STEVT");
        case STSUBT_ERROR_CLOSING_STCLKRV:
            return("STSUBT_ERROR_CLOSING_STCLKRV");
        case STSUBT_ERROR_EVENT_REGISTER:
            return("STSUBT_ERROR_EVENT_REGISTER");
        case STSUBT_ERROR_EVENT_SUBSCRIBE:
            return("STSUBT_ERROR_EVENT_SUBSCRIBE");
        case STSUBT_ERROR_EVENT_UNREGISTER:
            return("STSUBT_ERROR_EVENT_UNREGISTER");
        case STSUBT_ERROR_SETTING_SLOT:
            return("STSUBT_ERROR_SETTING_SLOT");
        case STSUBT_ERROR_DRIVER_RUNNING:
            return("STSUBT_ERROR_DRIVER_RUNNING");
        case STSUBT_ERROR_DRIVER_NOT_RUNNING:
            return("STSUBT_ERROR_DRIVER_NOT_RUNNING");
        case STSUBT_ERROR_DRIVER_NOT_SUSPENDED:
            return("STSUBT_ERROR_DRIVER_NOT_SUSPENDED");
        case STSUBT_ERROR_DRIVER_NOT_RESUMED:
            return("STSUBT_ERROR_DRIVER_NOT_RESUMED");
        case STSUBT_ERROR_DRIVER_DISPLAYING:
            return("STSUBT_ERROR_DRIVER_DISPLAYING");
        case STSUBT_ERROR_DRIVER_NOT_DISPLAYING:
            return("STSUBT_ERROR_DRIVER_NOT_DISPLAYING");
        case STSUBT_ERROR_INVALID_STANDARD:
            return("STSUBT_ERROR_INVALID_STANDARD");
        case STSUBT_ERROR_NO_STREAM_SET:
            return("STSUBT_ERROR_NO_STREAM_SET");
        case STSUBT_ERROR_CREATING_FILTER:
            return("STSUBT_ERROR_CREATING_FILTER");
        case STSUBT_ERROR_DELETING_FILTER:
            return("STSUBT_ERROR_DELETING_FILTER");
        case STSUBT_ERROR_STARTING_FILTER:
            return("STSUBT_ERROR_STARTING_FILTER");
        case STSUBT_ERROR_STOPPING_FILTER:
            return("STSUBT_ERROR_STOPPING_FILTER");
        case STSUBT_ERROR_CREATING_PROCESSOR:
            return("STSUBT_ERROR_CREATING_PROCESSOR");
        case STSUBT_ERROR_DELETING_PROCESSOR:
            return("STSUBT_ERROR_DELETING_PROCESSOR");
        case STSUBT_ERROR_STARTING_PROCESSOR:
            return("STSUBT_ERROR_STARTING_PROCESSOR");
        case STSUBT_ERROR_STOPPING_PROCESSOR:
            return("STSUBT_ERROR_STOPPING_PROCESSOR");
        case STSUBT_ERROR_INITIALIZING_DISPLAY:
            return("STSUBT_ERROR_INITIALIZING_DISPLAY");
        case STSUBT_ERROR_STARTING_ENGINE:
            return("STSUBT_ERROR_STARTING_ENGINE");
        case STSUBT_ERROR_STARTING_TIMER:
            return("STSUBT_ERROR_STARTING_TIMER");
        case STSUBT_ERROR_STOPPING_ENGINE:
            return("STSUBT_ERROR_STOPPING_ENGINE");
        case STSUBT_ERROR_STOPPING_TIMER:
            return("STSUBT_ERROR_STOPPING_TIMER");
        case STSUBT_ERROR_DELETING_ENGINE:
            return("STSUBT_ERROR_DELETING_ENGINE");
        case STSUBT_ERROR_DELETING_TIMER:
            return("STSUBT_ERROR_DELETING_TIMER");
        case STSUBT_ERROR_UNKNOWN_DISPLAY_SERVICE:
            return("STSUBT_ERROR_UNKNOWN_DISPLAY_SERVICE");
        case STSUBT_ERROR_INIT_DISPLAY_SERVICE:
            return("STSUBT_ERROR_INIT_DISPLAY_SERVICE");
        case STSUBT_ERROR_TERM_DISPLAY_SERVICE:
            return("STSUBT_ERROR_TERM_DISPLAY_SERVICE");
        case STSUBT_ERROR_INVALID_WORKING_MODE:
            return("STSUBT_ERROR_INVALID_WORKING_MODE");
        case STSUBT_ERROR_OVERLAY_MODE_NOT_ENABLED:
            return("STSUBT_ERROR_OVERLAY_MODE_NOT_ENABLED");
        case STSUBT_ERROR_OBJECT_CACHE_NOT_ENABLED:
            return("STSUBT_ERROR_OBJECT_CACHE_NOT_ENABLED");
        case STSUBT_ERROR_INVALID_CALL_MODE:
            return("STSUBT_ERROR_INVALID_CALL_MODE");
        case STSUBT_ERROR_NO_PACKET_AVAILABLE:
            return("STSUBT_ERROR_NO_PACKET_AVAILABLE");
        case STSUBT_ERROR_UNKNOWN_APDU:
            return("STSUBT_ERROR_UNKNOWN_APDU");
        case STSUBT_ERROR_INVALID_APDU:
            return("STSUBT_ERROR_INVALID_APDU");
        case STSUBT_ERROR_UNEXPECTED_APDU:
            return("STSUBT_ERROR_UNEXPECTED_APDU");
        case STSUBT_ERROR_LOST_APDU_PACKET:
            return("STSUBT_ERROR_LOST_APDU_PACKET");
        case STSUBT_ERROR_BAD_PARAMETER:
            return("STSUBT_ERROR_BAD_PARAMETER");
#endif

        /* sttbx.h */
        case STTBX_ERROR_FUNCTION_NOT_IMPLEMENTED:
            return("STTBX_ERROR_FUNCTION_NOT_IMPLEMENTED");

#if 0 /*yxl 2005-08-24 cancel this section*/
        /* sttsmux.h */
        case STTSMUX_ERROR_INVALID_SRC_OBJ:
            return("STTSMUX_ERROR_INVALID_SRC_OBJ");
        case STTSMUX_ERROR_INVALID_DST_OBJ:
            return("STTSMUX_ERROR_INVALID_DST_OBJ");
        case STTSMUX_ERROR_INVALID_OBJ:
            return("STTSMUX_ERROR_INVALID_OBJ");
        case STTSMUX_ERROR_INVALID_BYTECLK:
            return("STTSMUX_ERROR_INVALID_BYTECLK");
        case STTSMUX_ERROR_INVALID_STREAM_RATE:
            return("STTSMUX_ERROR_INVALID_STREAM_RATE");
        case STTSMUX_ERROR_INVALID_SYNC_MODE:
            return("STTSMUX_ERROR_INVALID_SYNC_MODE");
        case STTSMUX_ERROR_INVALID_TS_MODE:
            return("STTSMUX_ERROR_INVALID_TS_MODE");
#endif 

        /* stttx.h */
        case STTTX_ERROR_CANT_START:
            return("STTTX_ERROR_CANT_START");
        case STTTX_ERROR_CANT_STOP:
            return("STTTX_ERROR_CANT_STOP");
        case STTTX_ERROR_CANT_FILTER:
            return("STTTX_ERROR_CANT_FILTER");
        case STTTX_ERROR_CANT_CLEAR:
            return("STTTX_ERROR_CANT_CLEAR");
        case STTTX_ERROR_NO_MORE_REQUESTS:
            return("STTTX_ERROR_NO_MORE_REQUESTS");
        case STTTX_ERROR_TASK_CREATION:
            return("STTTX_ERROR_TASK_CREATION");
        case STTTX_ERROR_PID_NOT_SET:
            return("STTTX_ERROR_PID_NOT_SET");
        case STTTX_ERROR_EVT_REGISTER:
            return("STTTX_ERROR_EVT_REGISTER");
        case STTTX_ERROR_DECODER_RUNNING:
            return("STTTX_ERROR_DECODER_RUNNING");
        case STTTX_ERROR_STVBI_SETUP:
            return("STTTX_ERROR_STVBI_SETUP");


        /* sttuner.h */
        case STTUNER_ERROR_UNSPECIFIED:
            return("STTUNER_ERROR_UNSPECIFIED");
        case STTUNER_ERROR_HWFAIL:
            return("STTUNER_ERROR_HWFAIL");
        case STTUNER_ERROR_EVT_REGISTER:
            return("STTUNER_ERROR_EVT_REGISTER");
        case STTUNER_ERROR_ID:
            return("STTUNER_ERROR_ID");
        case STTUNER_ERROR_POINTER:
            return("STTUNER_ERROR_POINTER");
        case STTUNER_ERROR_INITSTATE:
            return("STTUNER_ERROR_INITSTATE");


        /* stuart.h */
        case STUART_ERROR_OVERRUN:
            return("STUART_ERROR_OVERRUN");
        case STUART_ERROR_PARITY:
            return("STUART_ERROR_PARITY");
        case STUART_ERROR_FRAMING:
            return("STUART_ERROR_FRAMING");
        case STUART_ERROR_ABORT:
            return("STUART_ERROR_ABORT");
        case STUART_ERROR_RETRIES:
            return("STUART_ERROR_RETRIES");
        case STUART_ERROR_PIO:
            return("STUART_ERROR_PIO");


        /* stvbi.h */
        case STVBI_ERROR_VBI_ALREADY_REGISTERED:
            return("STVBI_ERROR_VBI_ALREADY_REGISTERED");
        case STVBI_ERROR_VBI_NOT_ENABLE:
            return("STVBI_ERROR_VBI_NOT_ENABLE");
        case STVBI_ERROR_VBI_ENABLE:
            return("STVBI_ERROR_VBI_ENABLE");
        case STVBI_ERROR_UNSUPPORTED_VBI:
            return("STVBI_ERROR_UNSUPPORTED_VBI");
        case STVBI_ERROR_DENC_OPEN:
            return("STVBI_ERROR_DENC_OPEN");
        case STVBI_ERROR_DENC_ACCESS:
            return("STVBI_ERROR_DENC_ACCESS");


        /* stvid.h */
        case STVID_ERROR_DECODER_RUNNING:
            return("STVID_ERROR_DECODER_RUNNING");
        case STVID_ERROR_DECODER_RUNNING_IN_RT_MODE:
            return("STVID_ERROR_DECODER_RUNNING_IN_RT_MODE");
        case STVID_ERROR_DECODER_PAUSING:
            return("STVID_ERROR_DECODER_PAUSING");
        case STVID_ERROR_DECODER_STOPPED:
            return("STVID_ERROR_DECODER_STOPPED");
        case STVID_ERROR_DECODER_NOT_PAUSING:
            return("STVID_ERROR_DECODER_NOT_PAUSING");
        case STVID_ERROR_NOT_AVAILABLE:
            return("STVID_ERROR_NOT_AVAILABLE");
        case STVID_ERROR_DECODER_FREEZING:
            return("STVID_ERROR_DECODER_FREEZING");
        case STVID_ERROR_EVENT_REGISTRATION:
            return("STVID_ERROR_EVENT_REGISTRATION");
        case STVID_ERROR_SYSTEM_CLOCK:
            return("STVID_ERROR_SYSTEM_CLOCK");


        /* stvmix.h */
        case STVMIX_ERROR_LAYER_UNKNOWN:
            return("STVMIX_ERROR_LAYER_UNKNOWN");
        case STVMIX_ERROR_LAYER_UPDATE_PARAMETERS:
            return("STVMIX_ERROR_LAYER_UPDATE_PARAMETERS");
        case STVMIX_ERROR_LAYER_ALREADY_CONNECTED:
            return("STVMIX_ERROR_LAYER_ALREADY_CONNECTED");
        case STVMIX_ERROR_LAYER_ACCESS:
            return("STVMIX_ERROR_LAYER_ACCESS");
        case STVMIX_ERROR_VOUT_UNKNOWN:
            return("STVMIX_ERROR_VOUT_UNKNOWN");
        case STVMIX_ERROR_VOUT_ALREADY_CONNECTED:
            return("STVMIX_ERROR_VOUT_ALREADY_CONNECTED");
        case STVMIX_ERROR_NO_VSYNC:
            return("STVMIX_ERROR_NO_VSYNC");


        /* stvout.h */
        case STVOUT_ERROR_DENC_ACCESS:
            return("STVOUT_ERROR_DENC_ACCESS");
        case STVOUT_ERROR_VOUT_NOT_AVAILABLE:
            return("STVOUT_ERROR_VOUT_NOT_AVAILABLE");
        case STVOUT_ERROR_VOUT_NOT_ENABLE:
            return("STVOUT_ERROR_VOUT_NOT_ENABLE");
        case STVOUT_ERROR_VOUT_ENABLE:
            return("STVOUT_ERROR_VOUT_ENABLE");
        case STVOUT_ERROR_VOUT_INCOMPATIBLE:
            return("STVOUT_ERROR_VOUT_INCOMPATIBLE");


        /* stvtg.h */
        case STVTG_ERROR_INVALID_MODE:
            return("STVTG_ERROR_INVALID_MODE");
        case STVTG_ERROR_EVT_REGISTER:
            return("STVTG_ERROR_EVT_REGISTER");
        case STVTG_ERROR_EVT_UNREGISTER:
            return("STVTG_ERROR_EVT_UNREGISTER");
        case STVTG_ERROR_EVT_SUBSCRIBE:
            return("STVTG_ERROR_EVT_SUBSCRIBE");
        case STVTG_ERROR_DENC_OPEN:
            return("STVTG_ERROR_DENC_OPEN");
        case STVTG_ERROR_DENC_ACCESS:
            return("STVTG_ERROR_DENC_ACCESS");


        case ST_NO_ERROR:
            return("ST_NO_ERROR");

        default:
            break;
    }

    return("UNKNOWN ERROR");
}
