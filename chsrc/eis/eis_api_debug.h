/*
  * ===================================================================================
  * CopyRight By CHANGHONG NET L.T.D.
  * 文件: 	eis_api_debug.h
  * 描述: 	定义debug输出的相关接口
  * 作者:	蒋庆洲
  * 时间:	2008-10-22
  * ===================================================================================
  */

#ifndef __EIS_API_DEBUG__
#define __EIS_API_DEBUG__

//#define __EIS_API_DEBUG_ENABLE__
#define EIS_TEST

/*
功能说明：
	eis 接口debug信息调用
参数说明：
	输入参数：
		fmt：格式化字符串。参数说明参照标准的printf 函数。
	输出参数：无
	返 回：
		>0: 实际输出的字符数；
		IPANEL_ERR: 函数异常返回。
  */
extern INT32_T eis_report( const CHAR_T *fmt , ...);

#endif

/*--eof-----------------------------------------------------------------------------------------------------*/

