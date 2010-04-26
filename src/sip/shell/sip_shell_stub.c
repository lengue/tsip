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

/* SIP协议栈向业务层发送消息 */
ULONG SIP_SendUpMsg(ULONG ulStackRef1,
                    ULONG ulStackRef2,
                    ULONG ulAppRef1,
                    ULONG ulAppRef2,
                    UBUF_HEADER_S *pstUbufSipMsg)
{
    /* 调用注册的钩子函数 */
    if (g_stSiphellCfg.pfnSendUpMsg == NULL_PTR)
    {
        return FAIL;
    }

    return g_stSiphellCfg.pfnSendUpMsg(ulStackRef1,
                                       ulStackRef2,
                                       ulAppRef1,
                                       ulAppRef2,
                                       pstUbufSipMsg);
}

/* SIP协议栈向网络层发送消息 */
ULONG SIP_SendDownMsg(UBUF_HEADER_S  *pstSipMsgUbuf,
                      SIP_LOCATION_S *pstPeerLocation)
{
    /* 调用注册的钩子函数 */
    if (g_stSiphellCfg.pfnSendDownMsg == NULL_PTR)
    {
        return FAIL;
    }

    return g_stSiphellCfg.pfnSendDownMsg(pstSipMsgUbuf,
                                         pstPeerLocation);
}

ULONG SIP_StartTimer(ULONG ulName,
                     ULONG ulLen,
                     ULONG ulPara,
                     SIP_TIMER_TYPE_E eType,
                     ULONG *pulHandle)
{
    /* 调用注册的钩子函数 */
    if (g_stSiphellCfg.pfnStartTimer == NULL_PTR)
    {
        return FAIL;
    }

    return g_stSiphellCfg.pfnStartTimer(ulName,
                                        ulLen,
                                        ulPara,
                                        eType,
                                        pulHandle);
}

ULONG SIP_StopTimer(ULONG ulHandle)
{
    /* 调用注册的钩子函数 */
    if (g_stSiphellCfg.pfnStopTimer == NULL_PTR)
    {
        return FAIL;
    }

    g_stSiphellCfg.pfnStopTimer(ulHandle);
    return SUCCESS;
}

ULONG SIP_GenerateRandomString(CHAR *pcBuffer, ULONG ulBufferLen)
{
    /* 调用注册的钩子函数 */
    if (g_stSiphellCfg.pfnGenerateRandom == NULL_PTR)
    {
        return FAIL;
    }

    g_stSiphellCfg.pfnGenerateRandom(pcBuffer, ulBufferLen);
    return SUCCESS;
}

