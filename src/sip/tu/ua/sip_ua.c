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

#include "..\..\include\syntax\sip_syntax.h"
#include "..\..\include\transport\sip_transport.h"
#include "..\..\include\transaction\sip_transaction.h"
#include "..\..\include\tu\sip_tu.h"

#include "..\include\locate\sip_locate.h"

/* 本模块对外提供的常量和结构头文件 */


/* 本模块内部使用的常量和结构头文件 */


/* 本模块内部函数声明头文件 */
#include "sip_uac.inc"
#include "sip_uas.inc"

/* 本模块全局变量声明头文件 */
ULONG SIP_UA_Init()
{
    ULONG ulRet;

    ulRet  = SIP_UAC_Init();
    ulRet |= SIP_UAS_Init();

    return ulRet;
}

/* 本模块全局变量声明头文件 */
ULONG SIP_UA_RecvUpMsg(ULONG ulTxnID,
                       SIP_LOCATION_S *pstPeerAddr,
                       UBUF_HEADER_S *pstUbufSipMsg)
{
    SIP_MSG_S *pstSipMsg = NULL_PTR;
    ULONG ulRet;

    pstSipMsg = UBUF_UBufPtr2Ptr(pstUbufSipMsg, 0);
    switch (pstSipMsg->eMsgType)
    {
        case SIP_MSG_TYPE_REQUEST:
            ulRet = SIP_UAS_ProcessingRequest(ulTxnID, pstPeerAddr, pstUbufSipMsg);
            break;

        case SIP_MSG_TYPE_RESPONSE:
            //ulRet = SIP_UAC_ProcessingResponse(ulTxnID, pstUbufSipMsg);
            break;

        default:
            ulRet = FAIL;
            break;
    }

    return ulRet;
}

