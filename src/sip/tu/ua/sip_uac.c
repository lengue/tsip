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
#include "..\include\dialog\sip_dlg.h"

#include "..\..\include\transaction\sip_transaction.h"
#include "..\..\include\transport\sip_transport.h"
#include "..\..\include\syntax\sip_syntax.h"
#include "..\..\include\shell\sip_shell.h"

/* 本模块对外提供的常量和结构头文件 */

/* 本模块内部使用的常量和结构头文件 */
#include "sip_ua_const.inc"
#include "sip_ua_type.inc"

/* 本模块内部函数声明头文件 */
#include "sip_uac.inc"

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
ULONG SIP_UAC_SendRequest(ULONG ulAppDlgID,
                          ULONG ulAppTxnID,
                          ULONG ulDlgID,
                          ULONG *pulUacID,
                          UBUF_HEADER_S *pstUbufSipMsg)
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
        SIP_Dlg_GenerateRequest(ulDlgID, pstUbufSipMsg);
    }

    /* 申请一个UAC控制块 */
    SIP_UAC_AllocCB(&ulUacID);
    
    g_pstSipUacCB[ulUacID].ulAppDlgID = ulAppDlgID;
    g_pstSipUacCB[ulUacID].ulAppTxnID = ulAppTxnID;
    g_pstSipUacCB[ulUacID].pstSipMsgUbuf = pstUbufSipMsg;
    memset(g_pstSipUacCB[ulUacID].astFork, 0xff, SIP_LOCATION_MAX_NUMBER * 8);
    
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
ULONG SIP_UAC_ProcessingResponse(ULONG ulUacID,
                                 ULONG ulTxnID,
                                 UBUF_HEADER_S *pstUbufSipMsg)
{
    UCHAR ucTxnSeq;
    ULONG ulDlgID;
    ULONG ulRet;
    SIP_DLG_SESSION_STATE_E eSessionState;
    UBUF_HEADER_S        *pstUbufRequest   = NULL_PTR;
    SIP_MSG_S            *pstSipRequest    = NULL_PTR;
    SIP_MSG_S            *pstSipResponse   = NULL_PTR;
    SIP_HEADER_CALL_ID_S *pstHeaderCallID  = NULL_PTR;
    SIP_HEADER_FROM_S    *pstHeaderFrom    = NULL_PTR;
    SIP_HEADER_TO_S      *pstHeaderTo      = NULL_PTR;
    SIP_HEADER_CONTACT_S *pstHeaderContact = NULL_PTR;
    SIP_HEADER_CSEQ_S    *pstHeaderCseq    = NULL_PTR;

    ulRet = SIP_UAC_FindTxnSeq(ulUacID, ulTxnID, &ucTxnSeq);
    if (ulRet != SUCCESS)
    {
        return FAIL;
    }
    
    pstUbufRequest = g_pstSipUacCB[ulUacID].pstSipMsgUbuf;

    /* 会话和对话的处理 */
    pstSipRequest  = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUbufRequest);
    pstSipResponse = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUbufSipMsg);

    ulDlgID = g_pstSipUacCB[ulUacID].astFork[ucTxnSeq].ulDlgID;
    
    /*对话外的INVITE的创建会话*/
    if (pstSipRequest->uStartLine.stRequstLine.eMethod == SIP_METHOD_INVITE)
    {
        /* 初始INVITE的非失败响应会创建对话 */
        if ((ulDlgID == NULL_ULONG) 
          &&(pstSipResponse->uStartLine.stStatusLine.eStatusCode < SIP_STATUS_CODE_300))
        {
            pstHeaderCallID  = (SIP_HEADER_CALL_ID_S *)pstSipResponse->apstHeaders[SIP_HEADER_CALL_ID];
            pstHeaderFrom    = (SIP_HEADER_FROM_S *)pstSipResponse->apstHeaders[SIP_HEADER_FROM];
            pstHeaderTo      = (SIP_HEADER_TO_S *)pstSipResponse->apstHeaders[SIP_HEADER_TO];
            pstHeaderContact = (SIP_HEADER_CONTACT_S *)pstSipResponse->apstHeaders[SIP_HEADER_CONTACT];
            pstHeaderCseq    = (SIP_HEADER_CSEQ_S *)pstSipRequest->apstHeaders[SIP_HEADER_CSEQ];

            /* 创建对话 */
            SIP_Dlg_AllocDialog(pstHeaderCallID->pucCallID,
                                pstHeaderFrom->pucTag,
                                pstHeaderTo->pucTag, 
                               &ulDlgID);
            
            /* 创建对话 */
            SIP_Dlg_UpdateLocalURI(ulDlgID, pstHeaderFrom->stNameAddr.pstUri);
            SIP_Dlg_UpdateRemoteURI(ulDlgID, pstHeaderTo->stNameAddr.pstUri);
            SIP_Dlg_UpdateLocalSeq(ulDlgID, pstHeaderCseq->ulSeq);
            SIP_Dlg_UpdateRemoteTarget(ulDlgID, pstHeaderContact->pstParam->stAddr.pstUri);
            SIP_Dlg_UpdateSecureFlag(ulDlgID, FALSE);
            //SIP_Dlg_UpdateRouteSet(ulDlgID, pstRouteSet);
            
            g_pstSipUacCB[ulUacID].astFork[ucTxnSeq].ulDlgID = ulDlgID;

            /* 临时响应创建早会话，成功的响应创建确认对话*/
            if (pstSipResponse->uStartLine.stStatusLine.eStatusCode < SIP_STATUS_CODE_200)
            {
                SIP_Dlg_UpdateSessionState(ulDlgID, SIP_DLG_SESSION_STATE_EARLY);
            }
            else
            {
                SIP_Dlg_UpdateSessionState(ulDlgID, SIP_DLG_SESSION_STATE_CONFIRM);
            }
        }
        else if (ulDlgID != NULL_ULONG)
        {
            SIP_Dlg_GetSessionState(ulDlgID, &eSessionState);
            if (eSessionState == SIP_DLG_SESSION_STATE_EARLY)
            {
                /*成功响应确认对话，失败响应释放早对话，临时响应不处理*/
                if (pstSipResponse->uStartLine.stStatusLine.eStatusCode >= SIP_STATUS_CODE_300)
                {
                    SIP_Dlg_UpdateSessionState(ulDlgID, SIP_DLG_SESSION_STATE_BUTT);
                    ulRet = SIP_Dlg_CanDialogRelease(ulDlgID);
                    if (ulRet == SUCCESS)
                    {
                        SIP_Dlg_ReleaseDialog(ulDlgID);
                    }
                }
                else if (pstSipResponse->uStartLine.stStatusLine.eStatusCode >= SIP_STATUS_CODE_200)
                {
                    SIP_Dlg_UpdateSessionState(ulDlgID, SIP_DLG_SESSION_STATE_CONFIRM);
                }
            }
        }
    }
    
    /* 上报给应用程序 */
    SIP_SendUpMsg(ulDlgID, 
                  ulUacID, 
                  g_pstSipUacCB[ulUacID].ulAppDlgID, 
                  g_pstSipUacCB[ulUacID].ulAppTxnID, 
                  pstUbufSipMsg);

    /* 收到终结响应后UAS就完成了 */
    if (pstSipResponse->uStartLine.stStatusLine.eStatusCode >= SIP_STATUS_CODE_200)
    {
        g_pstSipUacCB[ulUacID].astFork[ucTxnSeq].ulTxnID = NULL_ULONG;
        g_pstSipUacCB[ulUacID].astFork[ucTxnSeq].ulDlgID = NULL_ULONG;
    }

    /*  如果所有事物都完成，则删除该控制块 */
    ulRet = SIP_UAC_IsRequestComplete(ulUacID);
    if (ulRet == SUCCESS)
    {
        SIP_UAC_FreeCB(ulUacID);
    }

    return SUCCESS;
}

ULONG SIP_UAC_LocateResult(SIP_LOCATION_RESULT_S *pstResult,
                           ULONG ulPara)
{
    UCHAR  ucSeq;
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
        SIP_Txn_AllocTxn(ulUacID, &ulTxnID);
        
        SIP_UAC_AllocTxnSeq(ulUacID, &ucSeq);
        pstUacCB->astFork[ucSeq].ulTxnID = ulTxnID;

        /*在事务上发送请求*/
        SIP_Txn_RecvDownMsg(ulTxnID,
                            pstUacCB->pstSipMsgUbuf,
                           &pstResult->astLocations[0]);
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
    SIP_MSG_S         *pstSipMsg = NULL_PTR;
    SIP_HEADER_FROM_S *pstFrom   = NULL_PTR;
    SIP_HEADER_TO_S   *pstTo     = NULL_PTR;
    SIP_HEADER_CALL_ID_S *pstCallID = NULL_PTR;
    SIP_HEADER_CSEQ_S    *pstCseq   = NULL_PTR;
    SIP_HEADER_MAX_FORWARDS_S *pstMaxForwards   = NULL_PTR;
    SIP_HEADER_VIA_S          *pstVia       = NULL_PTR;
    SIP_HEADER_CONTACT_S      *pstContact   = NULL_PTR;
    CHAR            acString[100];
    UCHAR           *pucString = NULL_PTR;
    ULONG            ulRuleIndex;

    pstSipMsg = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUbufSipMsg);
    SIP_Syntax_GetRuleIndex("addr-spec", &ulRuleIndex);

    /* From 头域不能为空 */
    if (pstSipMsg->apstHeaders[SIP_HEADER_FROM] == NULL_PTR)
    {
        return FAIL;
    }

    /*增加From tag */
    pstFrom = (SIP_HEADER_FROM_S *)pstSipMsg->apstHeaders[SIP_HEADER_FROM];
    if (pstFrom->pucTag == NULL_PTR)
    {
        SIP_GenerateRandomString(acString, 100);
        UBUF_CLONE_STRING(acString, pstUbufSipMsg, pstFrom->pucTag);
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
        UBUF_CLONE_STRING(acString, pstUbufSipMsg, pstCallID->pucCallID);
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

    /*对话外的INVITE要创建对话，因此要加Contact头域*/
    if (pstSipMsg->uStartLine.stRequstLine.eMethod == SIP_METHOD_INVITE)
    {
        pstContact = (SIP_HEADER_CONTACT_S *)UBUF_AddComponent(pstUbufSipMsg, sizeof(SIP_HEADER_CONTACT_S));
        pstContact->stHeader.pstNext = NULL_PTR;
        pstSipMsg->apstHeaders[SIP_HEADER_CONTACT] = (SIP_HEADER_S *)pstContact;
        pstContact->ucIsStar = FALSE;
        pstContact->pstParam = (SIP_CONTACT_PARAM_S *)UBUF_AddComponent(pstUbufSipMsg, sizeof(SIP_CONTACT_PARAM_S));
        pstContact->pstParam->pstNext = NULL_PTR;
        pstContact->pstParam->ulExpires = NULL_ULONG;
        pstContact->pstParam->stAddr.bName = FALSE;
        pstContact->pstParam->stAddr.pucName = NULL_PTR;
        pstContact->pstParam->stAddr.pstUri  = NULL_PTR;
        SIP_Syntax_Decode(ulRuleIndex,
                          g_pucSipUaContact,
                          (ULONG)strlen(g_pucSipUaContact),
                          pstUbufSipMsg,
                         &pstContact->pstParam->stAddr.pstUri);
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
    UBUF_CLONE_STRING(acBranch, pstUbufSipMsg, pstViaPara->pucBranch);
    return SUCCESS;
}

/* 获取事物在UAC中的序号，也可以用于判断事物的合法性*/
ULONG SIP_UAC_FindTxnSeq(ULONG ulUacID, ULONG ulTxnID, UCHAR *pucSeq)
{
    UCHAR ucLoop;
    SIP_UAC_CB_S *pstSipUacCB = NULL_PTR;

    pstSipUacCB = &g_pstSipUacCB[ulUacID];
    for (ucLoop = 0; ucLoop < SIP_LOCATION_MAX_NUMBER; ucLoop++)
    {
        if (pstSipUacCB->astFork[ucLoop].ulTxnID == ulTxnID)
        {
            *pucSeq = ucLoop;
            return SUCCESS;
        }
    }

    return FAIL;
}

/* 申请一个事物序号 */
ULONG SIP_UAC_AllocTxnSeq(ULONG ulUacID, UCHAR *pucSeq)
{
    UCHAR ucLoop;
    SIP_UAC_CB_S *pstSipUacCB = NULL_PTR;

    pstSipUacCB = &g_pstSipUacCB[ulUacID];
    for (ucLoop = 0; ucLoop < SIP_LOCATION_MAX_NUMBER; ucLoop++)
    {
        if (pstSipUacCB->astFork[ucLoop].ulTxnID == NULL_ULONG)
        {
            *pucSeq = ucLoop;
            return SUCCESS;
        }
    }

    return FAIL;    
}

/* 判断请求是否已经完成，条件是所有关联的事物的事物都完成*/
ULONG SIP_UAC_IsRequestComplete(ULONG ulUacID)
{
    ULONG ulLoop;
    SIP_UAC_CB_S *pstSipUacCB = NULL_PTR;

    pstSipUacCB = &g_pstSipUacCB[ulUacID];
    for (ulLoop = 0; ulLoop < SIP_LOCATION_MAX_NUMBER; ulLoop++)
    {
        if (pstSipUacCB->astFork[ulLoop].ulTxnID != NULL_ULONG)
        {
            return FAIL;
        }
    }

    return SUCCESS;    
}
