/*******************************************************************************
功能    : SIP协议栈传输层功能函数
创建人  : 唐春平
创建日期: 2009.01.10
修改记录:
*******************************************************************************/
/* 外部依赖模块头文件 */
#include "common\common.h"
#include "ubuf\ubuf.h"
#include "uri\uri.h"
#include "sip\sip.h"
#include "system\system.h"
#include "connect\connect.h"
#include "timer\timer.h"

/* 本模块对外提供的常量和结构头文件 */

/* 本模块内部使用的常量和结构头文件 */
#include "sip_adapt_const.inc"
#include "sip_adapt_type.inc"

/* 本模块内部函数声明头文件 */
#include "sip_adapt.inc"

/* 本模块全局变量声明头文件 */
#include "sip_adapt_var.inc"

FUNC_SIP_ADAPT_PARSE g_apfnSipAdaptCfgParse[]=
{
    SIP_ADPT_BuildTuConfig,
    SIP_ADPT_BuildTransactionConfig,
    SIP_ADPT_BuildTransportConfig,
    SIP_ADPT_BuildSyntaxtConfig,
    SIP_ADPT_BuildConnConfig
};

FUNC_SIP_ADAPT_FREE g_apfnSipAdaptCfgFree[]=
{
    SIP_ADPT_FreeTuConfig,
    SIP_ADPT_FreeTransactionConfig,
    SIP_ADPT_FreeTransportConfig,
    SIP_ADPT_FreeSyntaxConfig,
    SIP_ADPT_FreeConnConfig
};

UCHAR *g_pucSipAdptBuffer;
