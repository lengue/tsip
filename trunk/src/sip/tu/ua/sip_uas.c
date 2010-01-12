/*******************************************************************************
����    : �������Ϣ����(WIN32ר��)
������  : �ƴ�ƽ
��������: 200.01.10
�޸ļ�¼:
*******************************************************************************/
/* �ⲿ����ģ��ͷ�ļ� */
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

/* ��ģ������ṩ�ĳ����ͽṹͷ�ļ� */
#include "sip_ua_const.inc"
#include "sip_ua_type.inc"

/* ��ģ���ڲ�ʹ�õĳ����ͽṹͷ�ļ� */


/* ��ģ���ڲ���������ͷ�ļ� */
#include "sip_uac.inc"
#include "sip_uas.inc"

/* ��ģ��ȫ�ֱ�������ͷ�ļ� */
#include "sip_ua_var.inc"

/*UAS��ʼ��*/
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

/* �ײ�ʹ�øú����ϱ����� rfc3261 8.2.5 */
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
    
    /* �������� */
    if (ulUasID == NULL_ULONG)
    {
        /* ��ʼ������ */
        SIP_UAS_AllocCB(&ulUasID);
        SIP_Txn_AllocTxn(ulUasID, &ulTxnID);
        g_pstSipUasCB[ulUasID].ulTxnID = ulTxnID;

        /* ����Ϣ�·���TXN�� */
        SIP_Txn_RecvUpMsg(ulTxnID, pstUbufSipMsg, pstPeerAddr);
        return SUCCESS;
    }

    pstSipMsg = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUbufSipMsg);
    pstHeaderTo = (SIP_HEADER_TO_S *)pstSipMsg->apstHeaders[SIP_HEADER_TO];
    if (pstHeaderTo->pucTag != NULL_PTR)
    {
        /* ƥ��Ի� */
    }

    if (ulDlgID == NULL_ULONG)
    {
        /* ��֤ */
        
        /* �Ի������� */
        ulRet = SIP_UAS_MethodInspection(pstUbufSipMsg);
        if(ulRet != SUCCESS)
        {
            /* ����405��Ӧ������Allowͷ�� */
        }

        /* ͷ����*/
        ulRet = SIP_UAS_HeaderInspection(pstUbufSipMsg);
        if(ulRet != SUCCESS)
        {
            /* ͷ����ȷ�������� */
        }


        /* ���ݼ�� */
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

        /* �뷽����صĴ��� */
    }
    else
    {

    }

    /* �ϱ��û� */
    SIP_SendUpMsg(0, ulUasID, ulAppRef1, ulAppRef2, pstUbufSipMsg);
    return SUCCESS;
}

/*
Ӧ�ò㷢����Ӧ��Ϣ
ULONG ulTxnID ����ID������ƥ������
ULONG *pulDlgID  �����Ϊ�գ���ʶ�������ߴ���һ���Ի���
UBUF_HEADER_S * pstUbufSipMsg �����͵���Ϣ
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
    
    /* ���ͨ��ͷ�� */
    SIP_UAS_GenerateResponse(pstUbufRequest, pstUbufSipMsg);

    /* �Ự�ͶԻ��Ĵ��� */
    pstSipRequest  = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUbufSipMsg);
    pstSipResponse = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUbufSipMsg);

    ulDlgID = g_pstSipUasCB[ulUasID].ulDlgID;

    /*�Ի����INVITE�Ĵ����Ự*/
    if (pstSipRequest->uStartLine.stRequstLine.eMethod == SIP_METHOD_INVITE)
    {
        /* ��ʼINVITE�ķ�ʧ����Ӧ�ᴴ���Ի� */
        if ((ulDlgID == NULL_ULONG) 
          &&(pstSipResponse->uStartLine.stStatusLine.eStatusCode < SIP_STATUS_CODE_300))
        {
            pstHeaderCallID = (SIP_HEADER_CALL_ID_S *)pstSipResponse->apstHeaders[SIP_HEADER_CALL_ID];
            pstHeaderFrom   = (SIP_HEADER_FROM_S *)pstSipResponse->apstHeaders[SIP_HEADER_FROM];
            pstHeaderTo     = (SIP_HEADER_TO_S *)pstSipResponse->apstHeaders[SIP_HEADER_TO];
            pstHeaderContact = (SIP_HEADER_CONTACT_S *)pstSipRequest->apstHeaders[SIP_HEADER_CONTACT];
            pstHeaderCseq    = (SIP_HEADER_CSEQ_S *)pstSipRequest->apstHeaders[SIP_HEADER_CSEQ];

            /* �����Ի� */
            SIP_Dlg_AllocDialog(pstHeaderCallID->pucCallID,
                                pstHeaderTo->pucTag, 
                                pstHeaderFrom->pucTag, 
                               &ulDlgID);
            
            /* �����Ի� */
            SIP_Dlg_UpdateLocalURI(ulDlgID, pstHeaderTo->stNameAddr.pstUri);
            SIP_Dlg_UpdateRemoteSeq(ulDlgID, pstHeaderCseq->ulSeq);
            //SIP_Dlg_UpdateRemoteTarget(ulDlgID, pstHeaderContact->pucTemp);
            SIP_Dlg_UpdateRemoteURI(ulDlgID, pstHeaderFrom->stNameAddr.pstUri);
            SIP_Dlg_UpdateSecureFlag(ulDlgID, FALSE);
            //SIP_Dlg_UpdateRouteSet(ulDlgID, pstRouteSet);
            
            g_pstSipUasCB[ulUasID].ulDlgID = ulDlgID;

            /* ��ʱ��Ӧ������Ự���ɹ�����Ӧ����ȷ�϶Ի�*/
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
                /*�ɹ���Ӧȷ�϶Ի���ʧ����Ӧ�ͷ���Ի�����ʱ��Ӧ������*/
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
    
    /* �յ��ս���Ӧ��UAS������� */
    if (pstSipResponse->uStartLine.stStatusLine.eStatusCode >= SIP_STATUS_CODE_200)
    {
        SIP_UAS_FreeCB(ulUasID);
    }

    /* ��TXN������Ϣ */
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
    /* ���Toͷ�� */

    /* ���Request URIͷ�� */

    /* ���Merged����*/

    /* Requireͷ���� */
    
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
        
    /* ��¡Fromͷ�� */
    SIP_GetRuleIndex("From", &ulRuleIndex);
    SIP_Clone(ulRuleIndex,
              pstUbufRequestMsg->apstHeaders[SIP_HEADER_FROM], 
              pstUbufResponse, 
             &pstUbufResponseMsg->apstHeaders[SIP_HEADER_FROM]);
    
    /* ��¡Call-IDͷ�� */
    SIP_GetRuleIndex("Call-ID", &ulRuleIndex);
    SIP_Clone(ulRuleIndex,
              pstUbufRequestMsg->apstHeaders[SIP_HEADER_CALL_ID], 
              pstUbufResponse, 
             &pstUbufResponseMsg->apstHeaders[SIP_HEADER_CALL_ID]);

    /* ��¡CSeqͷ�� */
    SIP_GetRuleIndex("CSeq", &ulRuleIndex);
    SIP_Clone(ulRuleIndex,
              pstUbufRequestMsg->apstHeaders[SIP_HEADER_CSEQ], 
              pstUbufResponse, 
             &pstUbufResponseMsg->apstHeaders[SIP_HEADER_CSEQ]);
    
    /* ��¡Viaͷ�� */
    SIP_GetRuleIndex("Via", &ulRuleIndex);
    SIP_Clone(ulRuleIndex,
              pstUbufRequestMsg->apstHeaders[SIP_HEADER_VIA], 
              pstUbufResponse, 
             &pstUbufResponseMsg->apstHeaders[SIP_HEADER_VIA]);

    
    /* ��¡Toͷ�� */
    SIP_GetRuleIndex("To", &ulRuleIndex);
    SIP_Clone(ulRuleIndex,
              pstUbufRequestMsg->apstHeaders[SIP_HEADER_TO], 
              pstUbufResponse, 
             &pstUbufResponseMsg->apstHeaders[SIP_HEADER_TO]);


    /* ���Toͷ��û��tag�����һ��tag */
    pstHeaderTo = (SIP_HEADER_TO_S *)pstUbufResponseMsg->apstHeaders[SIP_HEADER_TO];
    if (pstHeaderTo->pucTag == NULL_PTR)
    {
        SIP_GenerateRandomString(acString, 100);
        UBUF_CLONE_STRING(acString, pstUbufResponse, pstHeaderTo->pucTag);
    }

    return SUCCESS;
}
