#ifndef __WINCE_PRIORITIES_H
#define __WINCE_PRIORITIES_H

#define MIN_USER_PRIORITY				0
#define MAX_USER_PRIORITY				99	/* ?? */


/* Sorted in increasing priority. Where unknown the numbers are derived from
 * the Linux priority.
 */

#define STBLAST_TASK_PRIORITY				MIN_USER_PRIORITY
#define STCC_TASK_PRIORITY				MIN_USER_PRIORITY
#define STDVM_SERVICE_TASK_PRIORITY			MIN_USER_PRIORITY
#define STEVT_USER_READ_TASK_PRIORITY			MIN_USER_PRIORITY
#define STINJ_TASK_PRIORITY				MIN_USER_PRIORITY
#define STPTI_INDEX_TASK_PRIORITY			MIN_USER_PRIORITY
#define STPTI_CAROUSEL_TASK_PRIORTY			MIN_USER_PRIORITY

#define STVIN_TASK_PRIORITY_INPUT			2

#define STREAMING_AUDIO_DECODER_TASK_PRIORITY		6
#define AUDIO_PP_TASK_PRIORITY				6

#define STLAYER_VIDEO_HAL_TASK_PRIORITY			7

#define PCMPLAYER_TASK_PRIORITY				10
#define SPDIFPLAYER_TASK_PRIORITY			10
#define PES_ES_PARSER_TASK_PRIORITY			10
#define STTTX_HANDLER_TASK_PRIORITY			10

#define STHDMI_TASK_CEC_PRIORITY			18
#define STPTI_EVENT_TASK_PRIORITY			18
#define STVID_TASK_PRIORITY_DISPLAY			18
#define STVOUT_TASK_CEC_PRIORITY			18
#define STVTG_TASK_PRIORITY_IT_PROCESSING		18

#define STVID_TASK_PRIORITY_TRICKMODE			20
#define STVID_TASK_PRIORITY_SPEED			20

#define STTTX_TIMEOUT_TASK_PRIORITY			22
#define STTTX_POOLING_TASK_PRIORITY			22
#define NET_TASK_PRIORITY				22
#define STVID_TASK_PRIORITY_DECODE			22

#define STVID_TASK_PRIORITY_PARSE			24

#define STVID_TASK_PRIORITY_INJECTERS			25

#define STVOUT_TASK_INTERRUPT_PROCESS_PRIORITY		26
#define STLAYER_GAMMAGFX_TASK_PRIORITY			26

#define STBLIT_INTERRUPT_PROCESS_TASK_STACK_PRIORITY	27

#define STBLIT_MASTER_TASK_STACK_PRIORITY		28
#define STBLIT_SLAVE_TASK_STACK_PRIORITY		28
#define STVOUT_TASK_HDCP_PRIORITY			28

#define STVID_TASK_PRIORITY_ERROR_RECOVERY		31

#define STBLAST_TIMER_TASK_PRIORITY			MAX_USER_PRIORITY
#define STCLKRV_FILTER_TASK_PRIORITY			MAX_USER_PRIORITY
#define STFDMA_CALLBACK_TASK_PRIORITY			MAX_USER_PRIORITY
#define STSMART_EVENTMGR_TASK_PRIORITY			MAX_USER_PRIORITY
#define STTUNER_SCAN_CAB_TASK_PRIORITY			MAX_USER_PRIORITY
#define STTUNER_SCAN_SAT_TASK_PRIORITY			MAX_USER_PRIORITY
#define STTUNER_SCAN_TER_TASK_PRIORITY			MAX_USER_PRIORITY
#define STPTI_INTERRUPT_TASK_PRIORITY			MAX_USER_PRIORITY
#define STUART_TIMER_TASK_PRIORIT			MAX_USER_PRIORITY

#endif