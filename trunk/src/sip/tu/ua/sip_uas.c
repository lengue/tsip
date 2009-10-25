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

#include "..\include\locate\sip_locate.h"

#include "..\..\include\transaction\sip_transaction.h"
#include "..\..\include\transport\sip_transport.h"
#include "..\..\include\syntax\sip_syntax.h"
#include "..\..\include\shell\sip_shell.h"

/* 本模块对外提供的常量和结构头文件 */
#include "sip_ua_const.inc"
#include "sip_ua_type.inc"

/* 本模块内部使用的常量和结构头文件 */


/* 本模块内部函数声明头文件 */
#include "sip_uac.inc"
#include "sip_uas.inc"

/* 本模块全局变量声明头文件 */
#include "sip_ua_var.inc"

/*UAS初始化*/
ULONG SIP_UAS_Init()
{
    COMM_CB_INIT(SIP_TU_UAS_NUM, sizeof(SIP_UAS_CB_S), g_pstSipUasCB, g_stSipUasCBQueue);
    return SUCCESS;
}

ULONG SIP_UAS_AllocCB(ULONG *pulUasID)
{
    COMM_CB_ALLOC(g_pstSipUasCB, g_stSipUasCBQueue, *pulUasID);
    if(*pulUasID == NULL_USHORT)
    {
        return FAIL;
    }

    return SUCCESS;
}

VOID SIP_UAS_FreeCB(ULONG ulUasID)
{
    COMM_CB_FREE(g_pstSipUasCB, g_stSipUasCBQueue, ulUasID);
    return;
}

/* 底层使用该函数上报错误 rfc3261 8.2.5 */
ULONG SIP_UAS_ProcessingRequest(ULONG ulCoreID,
                                SIP_LOCATION_S *pstPeerAddr,
                                UBUF_HEADER_S  *pstUbufSipMsg)
{
    ULONG ulDlgID = NULL_ULONG;
    ULONG ulRet;
    ULONG ulUasID;
    ULONG ulTxnID;
    ULONG ulAppRef1;
    ULONG ulAppRef2;

    ulUasID = ulCoreID;

    /* 处理事务 */
    if (ulUasID == NULL_ULONG)
    {
        /* 初始化环境 */
        SIP_UAS_AllocCB(&ulUasID);
        SIP_Txn_AllocTxn(ulUasID, &ulTxnID);
        g_pstSipUasCB[ulUasID].ulTxnID = ulTxnID;
        g_pstSipUasCB[ulUasID].pstSipMsgUbuf = pstUbufSipMsg;

        /* 将消息下发给TXN层 */
        SIP_Txn_RecvUpMsg(ulTxnID, pstUbufSipMsg, pstPeerAddr);
        return SUCCESS;
    }

    /* 匹配对话 */
    if(ulDlgID == NULL_ULONG)
    {
        /* 对话外请求 */
        ulRet = SIP_UAS_MethodInspection(pstUbufSipMsg);
        if(ulRet != SUCCESS)
        {
            /* 产生405响应，增加Allow头域 */
        }

        ulRet = SIP_UAS_HeaderInspection(pstUbufSipMsg);
        if(ulRet != SUCCESS)
        {
            /*  */
        }

        ulRet = SIP_UAS_ContentProcessing(pstUbufSipMsg);
        if(ulRet != SUCCESS)
        {
            /*  */
        }

        ulRet = SIP_UAS_ApplyingExtensions(pstUbufSipMsg);
        if(ulRet != SUCCESS)
        {
            /*  */
        }

        /* 与方法相关的处理 */
    }
    else
    {

    }

    /* 上报用户 */
    SIP_SendUpMsg(0, ulUasID, &ulAppRef1, &ulAppRef2, pstUbufSipMsg);
    return SUCCESS;
}

/*
应用层发送相应消息
ULONG ulTxnID 事务ID，用于匹配事务
ULONG *pulDlgID  如果不为空，标识创建或者存在一个对话中
UBUF_HEADER_S * pstUbufSipMsg 待发送的消息
*/
ULONG SIP_UAS_SendResponse(IN  ULONG ulUasID,
                           OUT ULONG *pulDlgID,
                           UBUF_HEADER_S * pstUbufSipMsg)
{
    /* 填充相关头域 */
    SIP_UAS_GenerateResponse(ulUasID, pstUbufSipMsg);

    /* 向TXN发送消息 */
    SIP_Txn_RecvDownMsg(g_pstSipUasCB[ulUasID].ulTxnID,
                        pstUbufSipMsg,
                        NULL_PTR);

    return SUCCESS;
}

/*rfc3261 8.2.1*/
ULONG SIP_UAS_MethodInspection(UBUF_HEADER_S *pstSipMsgUbuf)
{
    return SUCCESS;
}

/*rfc3261 8.2.2*/
ULONG SIP_UAS_HeaderInspection(UBUF_HEADER_S *pstSipMsgUbuf)
{
    return SUCCESS;
}

/*rfc3261 8.2.3*/
ULONG SIP_UAS_ContentProcessing(UBUF_HEADER_S *pstSipMsgUbuf)
{
    return SUCCESS;
}

/*rfc3261 8.2.4*/
ULONG SIP_UAS_ApplyingExtensions(UBUF_HEADER_S *pstSipMsgUbuf)
{
    return SUCCESS;
}

ULONG SIP_UAS_GenerateResponse(ULONG ulUasID, UBUF_HEADER_S * pstUbufSipMsg)
{
    UBUF_HEADER_S * pstUbufRequestMsg = NULL_PTR;

    pstUbufRequestMsg = g_pstSipUasCB[ulUasID].pstSipMsgUbuf;

    /* 克隆From头域 */
    /* 克隆Call-ID头域 */
    /* 克隆CSeq头域 */
    /* 克隆Via头域 */
    /* 克隆To头域 */
    /* 如果To头域没有tag，添加一个tag */

    return SUCCESS;
}