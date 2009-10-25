/*******************************************************************************
功能    : 传输层消息管理(WIN32专用)
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/
/* 外部依赖模块头文件 */
#include "common\common.h"
#include "ubuf\ubuf.h"
#include "sip\uri.h"
#include "sip\sip.h"

#include "..\include\syntax\sip_syntax.h"
#include "..\include\transport\sip_transport.h"
#include "..\include\transaction\sip_transaction.h"
#include "..\include\tu\sip_tu.h"
#include "..\include\shell\sip_shell.h"

/* 本模块对外提供的常量和结构头文件 */


/* 本模块内部使用的常量和结构头文件 */
#include "sip_shell_const.inc"
#include "sip_shell_type.inc"

/* 本模块内部函数声明头文件 */
#include "sip_shell_cfg.inc"

/* 本模块全局变量声明头文件 */
#include "sip_shell_var.inc"

ULONG SIP_TimeoutProc(ULONG ulName, ULONG ulPara)
{
    /* 根据名字分发定时器 */
    SIP_Txn_TimerProc(ulPara, ulName);

    return SUCCESS;
}


ULONG SIP_GetRuleIndex(UCHAR *pucRuleName, ULONG *pulRuleIndex)
{
    return SIP_Syntax_GetRuleIndex(pucRuleName, pulRuleIndex);
}