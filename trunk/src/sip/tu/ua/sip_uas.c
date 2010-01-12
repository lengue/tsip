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
    ULONG ulAppRef1 = NULL_ULONG;
    ULONG ulAppRef2 = NULL_ULONG;
    SIP_MSG_S *pstSipMsg = NULL_PTR;
    SIP_HEADER_TO_S *pstHeaderTo = NULL_PTR;

    ulUasID = ulCoreID;
    
    /* 处理事务 */
    if (ulUasID == NULL_ULONG)
    {
        /* 初始化环境 */
        SIP_UAS_AllocCB(&ulUasID);
        SIP_Txn_AllocTxn(ulUasID, &ulTxnID);
        g_pstSipUasCB[ulUasID].ulTxnID = ulTxnID;

        /* 将消息下发给TXN层 */
        SIP_Txn_RecvUpMsg(ulTxnID, pstUbufSipMsg, pstPeerAddr);
        return SUCCESS;
    }

    pstSipMsg = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUbufSipMsg);
    pstHeaderTo = (SIP_HEADER_TO_S *)pstSipMsg->apstHeaders[SIP_HEADER_TO];
    if (pstHeaderTo->pucTag != NULL_PTR)
    {
        /* 匹配对话 */
    }

    if (ulDlgID == NULL_ULONG)
    {
        /* 认证 */
        
        /* 对话外请求 */
        ulRet = SIP_UAS_MethodInspection(pstUbufSipMsg);
        if(ulRet != SUCCESS)
        {
            /* 产生405响应，增加Allow头域 */
        }

        /* 头域检查*/
        ulRet = SIP_UAS_HeaderInspection(pstUbufSipMsg);
        if(ulRet != SUCCESS)
        {
            /* 头域不正确继续处理 */
        }


        /* 内容检查 */
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
    SIP_SendUpMsg(0, ulUasID, ulAppRef1, ulAppRef2, pstUbufSipMsg);
    return SUCCESS;
}

/*
应用层发送相应消息
ULONG ulTxnID 事务ID，用于匹配事务
ULONG *pulDlgID  如果不为空，标识创建或者存在一个对话中
UBUF_HEADER_S * pstUbufSipMsg 待发送的消息
*/
ULONG SIP_UAS_SendResponse(ULONG ulAppDlgID,
                           ULONG ulAppTxnID,
                           ULONG *pulDlgID,
                           ULONG ulUasID,
                           UBUF_HEADER_S * pstUbufSipMsg)
{
    ULONG ulDlgID;
    ULONG ulRet;
    ULONG ulTxnID;
    SIP_DLG_SESSION_STATE_E eSessionState;
    UBUF_HEADER_S        *pstUbufRequest   = NULL_PTR;
    SIP_MSG_S            *pstSipRequest    = NULL_PTR;
    SIP_MSG_S            *pstSipResponse   = NULL_PTR;
    SIP_HEADER_CALL_ID_S *pstHeaderCallID  = NULL_PTR;
    SIP_HEADER_FROM_S    *pstHeaderFrom    = NULL_PTR;
    SIP_HEADER_TO_S      *pstHeaderTo      = NULL_PTR;
    SIP_HEADER_CONTACT_S *pstHeaderContact = NULL_PTR;
    SIP_HEADER_CSEQ_S    *pstHeaderCseq    = NULL_PTR;

    pstUbufRequest = SIP_Txn_GetInitMsg(g_pstSipUasCB[ulUasID].ulTxnID);
    
    /* 填充通用头域 */
    SIP_UAS_GenerateResponse(pstUbufRequest, pstUbufSipMsg);

    /* 会话和对话的处理 */
    pstSipRequest  = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUbufSipMsg);
    pstSipResponse = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUbufSipMsg);

    ulDlgID = g_pstSipUasCB[ulUasID].ulDlgID;

    /*对话外的INVITE的创建会话*/
    if (pstSipRequest->uStartLine.stRequstLine.eMethod == SIP_METHOD_INVITE)
    {
        /* 初始INVITE的非失败响应会创建对话 */
        if ((ulDlgID == NULL_ULONG) 
          &&(pstSipResponse->uStartLine.stStatusLine.eStatusCode < SIP_STATUS_CODE_300))
        {
            pstHeaderCallID = (SIP_HEADER_CALL_ID_S *)pstSipResponse->apstHeaders[SIP_HEADER_CALL_ID];
            pstHeaderFrom   = (SIP_HEADER_FROM_S *)pstSipResponse->apstHeaders[SIP_HEADER_FROM];
            pstHeaderTo     = (SIP_HEADER_TO_S *)pstSipResponse->apstHeaders[SIP_HEADER_TO];
            pstHeaderContact = (SIP_HEADER_CONTACT_S *)pstSipRequest->apstHeaders[SIP_HEADER_CONTACT];
            pstHeaderCseq    = (SIP_HEADER_CSEQ_S *)pstSipRequest->apstHeaders[SIP_HEADER_CSEQ];

            /* 创建对话 */
            SIP_Dlg_AllocDialog(pstHeaderCallID->pucCallID,
                                pstHeaderTo->pucTag, 
                                pstHeaderFrom->pucTag, 
                               &ulDlgID);
            
            /* 创建对话 */
            SIP_Dlg_UpdateLocalURI(ulDlgID, pstHeaderTo->stNameAddr.pstUri);
            SIP_Dlg_UpdateRemoteSeq(ulDlgID, pstHeaderCseq->ulSeq);
            //SIP_Dlg_UpdateRemoteTarget(ulDlgID, pstHeaderContact->pucTemp);
            SIP_Dlg_UpdateRemoteURI(ulDlgID, pstHeaderFrom->stNameAddr.pstUri);
            SIP_Dlg_UpdateSecureFlag(ulDlgID, FALSE);
            //SIP_Dlg_UpdateRouteSet(ulDlgID, pstRouteSet);
            
            g_pstSipUasCB[ulUasID].ulDlgID = ulDlgID;

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

    ulTxnID = g_pstSipUasCB[ulUasID].ulTxnID;
    
    /* 收到终结响应后UAS就完成了 */
    if (pstSipResponse->uStartLine.stStatusLine.eStatusCode >= SIP_STATUS_CODE_200)
    {
        SIP_UAS_FreeCB(ulUasID);
    }

    /* 向TXN发送消息 */
    SIP_Txn_RecvDownMsg(ulTxnID, pstUbufSipMsg, NULL_PTR);

    *pulDlgID = ulTxnID;
    return SUCCESS;
}

/*rfc3261 8.2.1*/
ULONG SIP_UAS_MethodInspection(UBUF_HEADER_S *pstSipMsgUbuf)
{
    SIP_MSG_S     *pstSipMsg = NULL_PTR;

    pstSipMsg = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstSipMsgUbuf);
    if (pstSipMsg->uStartLine.stRequstLine.eMethod >= SIP_METHOD_BUTT)
    {
        return FAIL;
    }
    
    return SUCCESS;
}

/*rfc3261 8.2.2*/
ULONG SIP_UAS_HeaderInspection(UBUF_HEADER_S *pstSipMsgUbuf)
{
    /* 检查To头域 */

    /* 检查Request URI头域 */

    /* 检查Merged请求*/

    /* Require头域检查 */
    
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

ULONG SIP_UAS_GenerateResponse(UBUF_HEADER_S *pstUbufRequest, UBUF_HEADER_S *pstUbufResponse)
{
    SIP_MSG_S *pstUbufRequestMsg  = NULL_PTR;
    SIP_MSG_S *pstUbufResponseMsg = NULL_PTR;
    SIP_HEADER_TO_S *pstHeaderTo = NULL_PTR;
    ULONG ulRuleIndex;
    UCHAR acString[100];
    
    pstUbufRequestMsg  = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUbufRequest);
    pstUbufResponseMsg = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUbufResponse);
        
    /* 克隆From头域 */
    SIP_GetRuleIndex("From", &ulRuleIndex);
    SIP_Clone(ulRuleIndex,
              pstUbufRequestMsg->apstHeaders[SIP_HEADER_FROM], 
              pstUbufResponse, 
             &pstUbufResponseMsg->apstHeaders[SIP_HEADER_FROM]);
    
    /* 克隆Call-ID头域 */
    SIP_GetRuleIndex("Call-ID", &ulRuleIndex);
    SIP_Clone(ulRuleIndex,
              pstUbufRequestMsg->apstHeaders[SIP_HEADER_CALL_ID], 
              pstUbufResponse, 
             &pstUbufResponseMsg->apstHeaders[SIP_HEADER_CALL_ID]);

    /* 克隆CSeq头域 */
    SIP_GetRuleIndex("CSeq", &ulRuleIndex);
    SIP_Clone(ulRuleIndex,
              pstUbufRequestMsg->apstHeaders[SIP_HEADER_CSEQ], 
              pstUbufResponse, 
             &pstUbufResponseMsg->apstHeaders[SIP_HEADER_CSEQ]);
    
    /* 克隆Via头域 */
    SIP_GetRuleIndex("Via", &ulRuleIndex);
    SIP_Clone(ulRuleIndex,
              pstUbufRequestMsg->apstHeaders[SIP_HEADER_VIA], 
              pstUbufResponse, 
             &pstUbufResponseMsg->apstHeaders[SIP_HEADER_VIA]);

    
    /* 克隆To头域 */
    SIP_GetRuleIndex("To", &ulRuleIndex);
    SIP_Clone(ulRuleIndex,
              pstUbufRequestMsg->apstHeaders[SIP_HEADER_TO], 
              pstUbufResponse, 
             &pstUbufResponseMsg->apstHeaders[SIP_HEADER_TO]);


    /* 如果To头域没有tag，添加一个tag */
    pstHeaderTo = (SIP_HEADER_TO_S *)pstUbufResponseMsg->apstHeaders[SIP_HEADER_TO];
    if (pstHeaderTo->pucTag == NULL_PTR)
    {
        SIP_GenerateRandomString(acString, 100);
        UBUF_CLONE_STRING(acString, pstUbufResponse, pstHeaderTo->pucTag);
    }

    return SUCCESS;
}
