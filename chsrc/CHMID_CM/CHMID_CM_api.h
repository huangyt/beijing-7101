/*******************************************************************************
* �ļ����� : CHMID_CM_api.h											
* �������� : CHMID_CMģ���ṩ���ϲ�Ӧ�õ��õĹ��ܽӿں������壬����һЩ���ݽṹ
* ���꣬�����ȶ��塣��Ӧ�ò��̵���
*
* ����:flex
* �޸���ʷ:
* ����					����						����
* 2009/6/23				 flex							create
*
********************************************************************************/
#ifndef _CHMID_CM_API_H__ 
#define _CHMID_CM_API_H__
#include "CHDRV_NET.h"

typedef enum
{
	CHMID_CM_CHECK_FAIL = 0,			/*	���ʧ��*/
	CHMID_CM_INITED,					/*��ʼ״̬*/
	CHMID_CM_DOWN_FREQ_LOCKED,		/*����������*/
	CHMID_CM_UP_FREQ_LOCKED,		/*����������*/
	CHMID_CM_OFFLINE,					/*	CM������*/
	CHMID_CM_ONLINE 					/*	CM����*/
}CHMID_CM_ONLINE_STATUS_e;

typedef enum
{
	CHMID_CM_OK = 0,				/*	�����ɹ�*/
	CHMID_CM_FAIL = -1,				/*	����ʧ��*/
	CHMID_CM_NOT_INIT = -2,		/*	ģ��û�г�ʼ��*/
	CHMID_CM_ALREADY_INIT = -3,	/*    ģ���Ѿ���ʼ��*/
	CHMID_CM_TIMEOUT = -4,			/*	��ʱ*/
	CHMID_CM_INVALID_PARAM = -5	/*	��������*/
}CHMID_CM_RESULT_e;

typedef struct
{
	s32_t i_InterfaceMainVer;/*Ϊ�ӿ��ĵ������汾��*/
	s32_t i_InterfaceSubVer;	/*Ϊ�ӿ��ĵ��Ĵΰ汾��*/
	s32_t i_ModuleMainVer;	/*Ϊģ��ʵ�ֵ����汾*/
	s32_t i_ModuleSubVer;	/*Ϊģ��ʵ�ֵĴΰ汾*/
}CHMID_CM_Version_t;

/*--------------------------------------------------------------------------*/
/* �������� : CHMID_CM_Init								*/
/* �������� : CHMID_CMģ���ʼ������					*/
/* ��ڲ��� :��				*/
/* ���ڲ��� : ��		*/
/* ����ֵ˵����	*/
/* ע��:	��������һ��,Ĭ�������,��ع���Ϊ��			*/
/*--------------------------------------------------------------------------*/
CHMID_CM_RESULT_e CHMID_CM_Init(void);
/*--------------------------------------------------------------------------*/
/* �������� : CHMID_CM_Open								*/
/* �������� : ��ģ����CM����״̬����				*/
/* ��ڲ��� :��				*/
/* ���ڲ��� : ��		*/
/* ����ֵ˵������CHMID_CM_RESULT_e����*/
/* ע��:				*/
/*--------------------------------------------------------------------------*/
CHMID_CM_RESULT_e CHMID_CM_Open(void);
/*--------------------------------------------------------------------------*/
/* �������� : CHMID_CM_Close								*/
/* �������� : �ر�ģ����CM����״̬����				*/
/* ��ڲ��� :��				*/
/* ���ڲ��� : ��		*/
/* ����ֵ˵������CHMID_CM_RESULT_e����*/
/* ע��:	�ڹرռ�غ���Ҫ���´򿪹��������CHMID_CM_Open()*/
/*--------------------------------------------------------------------------*/
CHMID_CM_RESULT_e CHMID_CM_Close(void);
/*--------------------------------------------------------------------------*/
/* �������� : CHMID_CM_GetVersion								*/
/* �������� :�õ� CHMID_CMģ��汾��Ϣ				*/
/* ��ڲ��� :��				*/
/* ���ڲ��� : ��		*/
/* ����ֵ˵����	*/
/* ע��:					*/
/*--------------------------------------------------------------------------*/
CHMID_CM_RESULT_e CHMID_CM_GetVersion(CHMID_CM_Version_t * pVersion);
/*--------------------------------------------------------------------------*/
/* �������� : CHMID_CM_CheckCMStatus								*/
/* �������� : �õ���ǰCM����״̬					*/
/* ��ڲ��� :��				*/
/* ���ڲ��� : ��		*/
/* ����ֵ˵������CHMID_CM_ONLINE_STATUS_e˵��	*/
/* ע��:				*/
/*--------------------------------------------------------------------------*/
CHMID_CM_ONLINE_STATUS_e CHMID_CM_CheckCMStatus(void);

/*--------------------------------------------------------------------------*/
/* �������� : CHMID_CM_GetCMMac								*/
/* �������� : �õ�CM��MAC��ַ				*/
/* ��ڲ��� :��			*/
/* ���ڲ��� : mac,�õ���mac��ַ			*/
/* ����ֵ˵�����ɹ�����0*/
/* ע��:	����Ӧ��Ϊһ���������ָ�룬��������Ϊ6BYTE			*/
/*--------------------------------------------------------------------------*/
CHMID_CM_RESULT_e CHMID_CM_GetCMMac(u8_t * mac);
/*--------------------------------------------------------------------------*/
/* �������� : CHMID_CM_GetCMSNNumber								*/
/* �������� : �õ�CM�����к�(S/N)				*/
/* ��ڲ��� :��			*/
/* ���ڲ��� : SN_NO,�õ������к�(��:)		*/
/* ����ֵ˵�����ɹ�����0*/
/* ע��:	����Ӧ��Ϊһ���������ָ�룬��������Ϊ20BYTE			*/
/*--------------------------------------------------------------------------*/
CHMID_CM_RESULT_e CHMID_CM_GetCMSNNumber(u8_t * SN_NO);
/*--------------------------------------------------------------------------*/
/* �������� : CHMID_CM_Reboot								*/
/* �������� : ����CM				*/
/* ��ڲ��� :��			*/
/* ���ڲ��� : ��			*/
/* ����ֵ˵�����ɹ�����0*/
/* ע��:				*/
/*--------------------------------------------------------------------------*/
CHMID_CM_RESULT_e CHMID_CM_Reboot(void);
/*--------------------------------------------------------------------------*/
/* �������� : CHMID_CM_SetDownFreq								*/
/* �������� : ����CM��Ĭ������Ƶ��				*/
/* ��ڲ��� :	 Freq,�ַ�����ʽ��Ƶ������(��:612000000)	*/
/* ���ڲ��� :	��			*/
/* ����ֵ˵�����ɹ�����0*/
/* ע��:	*/
/*--------------------------------------------------------------------------*/
CHMID_CM_RESULT_e CHMID_CM_SetDownFreq(u8_t * Freq);
/*--------------------------------------------------------------------------*/
/* �������� : CHMID_CM_GetUPFreq								*/
/* �������� : ��ȡCM������Ƶ��				*/
/* ��ڲ��� :��			*/
/* ���ڲ��� : uc_UPFreq,�ַ�����ʽ��Ƶ������(��:612000000)			*/
/* ����ֵ˵�����ɹ�����0*/
/* ע��:	����Ӧ��Ϊһ���������ָ�룬��������Ϊ20BYTE			*/
/*--------------------------------------------------------------------------*/
CHMID_CM_RESULT_e CHMID_CM_GetUPFreq(u8_t * uc_UPFreq);
/*--------------------------------------------------------------------------*/
/* �������� : CHMID_CM_GetDownFreq								*/
/* �������� : ��ȡCM������Ƶ��				*/
/* ��ڲ��� :��			*/
/* ���ڲ��� : uc_DownFreq,�ַ�����ʽ��Ƶ������(��:612000000)			*/
/* ����ֵ˵�����ɹ�����0		*/
/* ע��:	����Ӧ��Ϊһ���������ָ�룬��������Ϊ20BYTE			*/
/*--------------------------------------------------------------------------*/
CHMID_CM_RESULT_e CHMID_CM_GetDownFreq(u8_t * uc_DownFreq);
/*--------------------------------------------------------------------------*/
/* �������� : CHMID_CM_SetCheckCycleTime								*/
/* �������� :����CM״̬��ѯ����ʱ��(s)				*/
/* ��ڲ��� :	ui_time_s,�µĲ�ѯʱ������(s)				*/
/* ���ڲ��� : ��					*/
/* ����ֵ˵�����ɹ�����0				*/
/* ע��:				*/
/*--------------------------------------------------------------------------*/
CHMID_CM_RESULT_e CHMID_CM_SetCheckCycleTime(u32_t ui_time_s);
/*--------------------------------------------------------------------------*/
/* �������� : CHMID_CM_GetCheckCycleTime								*/
/* �������� : ��ȡCM״̬��ѯ����ʱ��(s)						*/
/* ��ڲ��� :��			*/
/* ���ڲ��� : ��			*/
/* ����ֵ˵����	CM״̬��ѯ����ʱ��(s)	*/
/* ע��:				*/
/*--------------------------------------------------------------------------*/
u32_t CHMID_CM_GetCheckCycleTime(void);

#endif
/*eof*/
