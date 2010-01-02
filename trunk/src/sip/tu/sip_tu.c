/*******************************************************************************
功能    : 传输层连接管理(WIN32专用)
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/
/* 外部依赖模块头文件 */
#include "common\common.h"
#include "abnf\abnf.h"
#include "ubuf\ubuf.h"
#include "sip\uri.h"
#include "sip\sip.h"

#include "include\ua\sip_ua.h"
/* 本模块对外提供的常量和结构头文件 */

/* 本模块内部使用的常量和结构头文件 */

/* 本模块内部函数声明头文件 */


/* 本模块全局变量声明头文件 */

/* 用于定位请求的目的位置 */
#include "sip_tu_var.inc"

ULONG SIP_TU_Init(SIP_TU_CFG_S *pstCfg)
{
    ULONG ulRet;

    g_eSipTuCore = pstCfg->eType;
    switch (g_eSipTuCore)
    {
    	case SIP_CORE_UA:
            ulRet = SIP_UA_Init();
            break;

        default:
            break;
    }

    return SUCCESS;
}

ULONG SIP_TU_RecvUpMsg(ULONG ulCoreID,
                       SIP_LOCATION_S *pstPeerAddr,
                       UBUF_HEADER_S  *pstUbufSipMsg)
{
    ULONG ulRet;

    switch (g_eSipTuCore)
    {
        case SIP_CORE_UA:
            ulRet = SIP_UA_RecvUpMsg(ulCoreID, pstPeerAddr, pstUbufSipMsg);
            break;

        default:
            break;
    }

    return SUCCESS;
}

ULONG SIP_TU_RecvDownMsg(ULONG ulAppRef1,
                         ULONG ulAppRef2,
                         ULONG ulStackRef1,
                         ULONG ulStackRef2,
                         UBUF_HEADER_S  *pstUbufSipMsg)
{
    ULONG ulRet;

    switch (g_eSipTuCore)
    {
        case SIP_CORE_UA:
            ulRet = SIP_UA_RecvDownMsg(ulAppRef1,
                                       ulAppRef2,
                                       ulStackRef1,
                                       ulStackRef2,
                                       pstUbufSipMsg);
            break;

        default:
            break;
    }

    return SUCCESS;
}

