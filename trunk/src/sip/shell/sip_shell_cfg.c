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

ULONG SIP_Config(SIP_CFG_S *pstCfg)
{
    ULONG ulRet;

    /* 配置全局层 */
    memcpy(&g_stSiphellCfg, &pstCfg->stShellCfg, sizeof(SIP_SHELL_CFG_S));

    /* 配置TU层 */
    ulRet = SIP_TU_Init(&pstCfg->stTuCfg);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }

    /* 配置TXN层 */
    ulRet = SIP_Txn_Init(&pstCfg->stTxnCfg);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }

    /* 配置TXP层 */
    ulRet = SIP_Txp_Init(&pstCfg->stTxpCfg);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }

    /* 配置SYNTAX */
    ulRet = SIP_Syntax_Init(&pstCfg->stSyntaxCfg);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }

    return SUCCESS;
}