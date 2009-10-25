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

/* UAC模块初始化函数 */
ULONG SIP_UAC_Init()
{
    COMM_CB_INIT(SIP_TU_UAC_NUM, sizeof(SIP_UAC_CB_S), g_pstSipUacCB, g_stSipUacCBQueue);
    return SUCCESS;
}

/* 申请UAC控制块 */
ULONG SIP_UAC_AllocCB(ULONG *pulUacID)
{
    COMM_CB_ALLOC(g_pstSipUacCB, g_stSipUacCBQueue, *pulUacID);
    if(*pulUacID == NULL_USHORT)
    {
        return FAIL;
    }

    return SUCCESS;
}

/* 释放UAC控制块 */
VOID SIP_UAC_FreeCB(ULONG ulUacID)
{
    COMM_CB_FREE(g_pstSipUacCB, g_stSipUacCBQueue, ulUacID);
    return;
}

/* 应用发送请求消息
ULONG ulAppID 应用ID，用于应用层匹配响应
ULONG ulDlgID 对话ID，不为空标识对话内请求
UBUF_HEADER_S * pstUbufSipMsg 待发送的消息
*/
ULONG SIP_UAC_SendRequest(ULONG ulAppID,
                          ULONG ulDlgID,
                          UBUF_HEADER_S * pstUbufSipMsg)
{
    ULONG       ulRet;
    SIP_URI_S * pstSipUri = NULL_PTR;
    ULONG       ulUacID;
    SIP_LOCATION_RESULT_S stResult;
    URI_S       *pstUri = NULL_PTR;
    SIP_MSG_S   *pstSipMsg = NULL_PTR;

    pstSipMsg = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUbufSipMsg);

    if (ulDlgID == NULL_ULONG)
    {
        /* 对话外请求一些关键头域需要自动生成 */
        SIP_UAC_GenerateRequest(pstUbufSipMsg);
    }
    else
    {
        /* 对话内请求，通过对话构造相关头域 */
    }

    /* 申请一个UAC控制块 */
    SIP_UAC_AllocCB(&ulUacID);
    g_pstSipUacCB[ulUacID].ulAppID = ulAppID;
    g_pstSipUacCB[ulUacID].ulDlgID = ulDlgID;
    g_pstSipUacCB[ulUacID].pstSipMsgUbuf = pstUbufSipMsg;

    SIP_Locate_FindNextHop(pstUbufSipMsg, &pstUri);

    ulRet = SIP_Locate_Server(pstUri,
                             &stResult,
                              SIP_UAC_LocateResult,
                              ulUacID);
    if (ulRet == SUCCESS)
    {
        /* 当做解析成功处理 */
        SIP_UAC_LocateResult(&stResult, ulUacID);
    }
    else
    {
        /* 未成功等待解析成功处理 */
    }

    return SUCCESS;
}

/* 底层使用该函数上报错误 rfc3261 8.1.3 */
ULONG SIP_UAC_ProcessingResponse(ULONG ulAppRef1,
                                 ULONG ulAppRef2,
                                 SIP_MSG_S *pstSipMsg)
{
    /* 查看是否创建对话 */
    /* 如果创建了对话携带对话参数 */
    /* 上报给应用程序 */
    /* 如果是最终响应，删除该控制块 */
    return SUCCESS;
}

ULONG SIP_UAC_LocateResult(SIP_LOCATION_RESULT_S *pstResult,
                           ULONG ulPara)
{
    ULONG  ulTxnID;
    ULONG  ulUacID;
    SIP_UAC_CB_S *pstUacCB = NULL_PTR;
    SIP_MSG_S    *pstSipMsg = NULL_PTR;

    if (pstResult->ulNumber == 0)
    {
        /* 通知TU发送失败 */
    }

    ulUacID = ulPara;
    pstUacCB = &g_pstSipUacCB[ulUacID];

    /* 找到下一跳后就可以添加Via头域 */
    SIP_UAC_AddViaHeader(pstUacCB->pstSipMsgUbuf);

    /* 一个一个轮着发 */
    pstSipMsg = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUacCB->pstSipMsgUbuf);
    if (pstSipMsg->uStartLine.stRequstLine.eMethod != SIP_METHOD_ACK)
    {
        /* 不是ACK通过事务发送 */

        /*申请一个事务*/
        SIP_Txn_AllocTxn(&pstResult->astLocations[0], &ulTxnID);

        /*在事务上发送请求*/
        SIP_Txn_RecvDownMsg(ulTxnID,
                            pstUacCB->pstSipMsgUbuf);
    }
    else
    {
        /* ACK直接调用TXP发送 */
        SIP_Txp_RecvDownMsg(pstUacCB->pstSipMsgUbuf,
                           &pstResult->astLocations[0]);
    }

    return SUCCESS;
}

/* UAC参数一个对话外请求 */
ULONG SIP_UAC_GenerateRequest(UBUF_HEADER_S * pstUbufSipMsg)
{
    SIP_MSG_S       *pstSipMsg = NULL_PTR;
    SIP_HEADER_TO_S *pstTo     = NULL_PTR;
    SIP_HEADER_CALL_ID_S *pstCallID = NULL_PTR;
    SIP_HEADER_CSEQ_S    *pstCseq   = NULL_PTR;
    SIP_HEADER_MAX_FORWARDS_S *pstMaxForwards   = NULL_PTR;
    SIP_HEADER_VIA_S          *pstVia   = NULL_PTR;
    CHAR            acString[100];
    UCHAR           *pucString = NULL_PTR;
    ULONG            ulRuleIndex;

    pstSipMsg = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUbufSipMsg);

    /* From 头域不能为空 */
    if (pstSipMsg->apstHeaders[SIP_HEADER_FROM] == NULL_PTR)
    {
        return FAIL;
    }

    /* To 头域不能为空 */
    if (pstSipMsg->apstHeaders[SIP_HEADER_TO] == NULL_PTR)
    {
        return FAIL;
    }

    /* 生成 Request URI，从To头域中赋值一份就可以了 */
    if (pstSipMsg->uStartLine.stRequstLine.pstRequestURI == NULL_PTR)
    {
        pstTo = (SIP_HEADER_TO_S *)pstSipMsg->apstHeaders[SIP_HEADER_TO];
        SIP_Syntax_GetRuleIndex("addr-spec", &ulRuleIndex);
        SIP_Syntax_Clone(ulRuleIndex,
                         pstTo->stNameAddr.pstUri,
                         pstUbufSipMsg,
                         &pstSipMsg->uStartLine.stRequstLine.pstRequestURI);
    }

    /* 生成Call-ID头域 */
    if (pstSipMsg->apstHeaders[SIP_HEADER_CALL_ID] == NULL_PTR)
    {
        pstCallID = UBUF_AddComponent(pstUbufSipMsg, sizeof(SIP_HEADER_CALL_ID_S));
        pstSipMsg->apstHeaders[SIP_HEADER_CALL_ID] = (SIP_HEADER_S *)pstCallID;
        pstCallID->stHeader.pstNext = NULL_PTR;

        SIP_GenerateRandomString(acString, 100);
        pucString = UBUF_AddComponent(pstUbufSipMsg,
                                     (ULONG)strlen(acString)+1);
        pstCallID->pucCallID = pucString;
        memcpy(pucString, acString, strlen(acString)+1);
    }

    /* 生成Cseq头域 */
    if (pstSipMsg->apstHeaders[SIP_HEADER_CSEQ] == NULL_PTR)
    {
        pstCseq = UBUF_AddComponent(pstUbufSipMsg, sizeof(SIP_HEADER_CSEQ_S));
        pstSipMsg->apstHeaders[SIP_HEADER_CSEQ] = (SIP_HEADER_S *)pstCseq;
        pstCseq->stHeader.pstNext = NULL_PTR;
        pstCseq->eMethod = pstSipMsg->uStartLine.stRequstLine.eMethod;
        pstCseq->ulSeq = 0;
    }

    /* 生成Max-Forwards头域 */
    if (pstSipMsg->apstHeaders[SIP_HEADER_MAX_FORWARDS] == NULL_PTR)
    {
        pstMaxForwards = UBUF_AddComponent(pstUbufSipMsg, sizeof(SIP_HEADER_MAX_FORWARDS_S));
        pstSipMsg->apstHeaders[SIP_HEADER_MAX_FORWARDS] = (SIP_HEADER_S *)pstMaxForwards;
        pstMaxForwards->stHeader.pstNext = NULL_PTR;
        pstMaxForwards->ulMaxForwards = 70;
    }

    return SUCCESS;
}

ULONG SIP_UAC_AddViaHeader(UBUF_HEADER_S *pstUbufSipMsg)
{
    SIP_MSG_S        *pstSipMsg = NULL_PTR;
    SIP_HEADER_VIA_S *pstVia    = NULL_PTR;
    SIP_VIA_PARM_S   *pstViaPara =  NULL_PTR;
    CHAR              acBranch[100];
    CHAR             *pcMagic = "z9hG4bK";
    UCHAR            *pucString = NULL_PTR;

    pstSipMsg = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUbufSipMsg);

    /* 生成Via头域 */
    pstVia = UBUF_AddComponent(pstUbufSipMsg, sizeof(SIP_HEADER_VIA_S));
    pstSipMsg->apstHeaders[SIP_HEADER_VIA] = (SIP_HEADER_S *)pstVia;
    pstVia->stHeader.pstNext = NULL_PTR;
    pstViaPara = UBUF_AddComponent(pstUbufSipMsg, sizeof(SIP_VIA_PARM_S));
    pstVia->pstViaParm = pstViaPara;
    memset(pstViaPara, 0, sizeof(SIP_VIA_PARM_S));
    strcpy(acBranch, pcMagic);
    SIP_GenerateRandomString(acBranch + strlen(pcMagic),
                             100 - (ULONG)strlen(pcMagic));
    pucString = UBUF_AddComponent(pstUbufSipMsg,
                                  (ULONG)strlen(acBranch) + 1);
    pstViaPara->pucBranch = pucString;
    memcpy(pucString, acBranch, strlen(acBranch)+1);
    return SUCCESS;
}
