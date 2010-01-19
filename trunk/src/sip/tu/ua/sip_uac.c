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

/* ��ģ���ڲ�ʹ�õĳ����ͽṹͷ�ļ� */
#include "sip_ua_const.inc"
#include "sip_ua_type.inc"

/* ��ģ���ڲ���������ͷ�ļ� */
#include "sip_uac.inc"

/* ��ģ��ȫ�ֱ�������ͷ�ļ� */
#include "sip_ua_var.inc"

/* UACģ���ʼ������ */
ULONG SIP_UAC_Init()
{
    COMM_CB_INIT(SIP_TU_UAC_NUM, sizeof(SIP_UAC_CB_S), g_pstSipUacCB, g_stSipUacCBQueue);
    return SUCCESS;
}

/* ����UAC���ƿ� */
ULONG SIP_UAC_AllocCB(ULONG *pulUacID)
{
    COMM_CB_ALLOC(g_pstSipUacCB, g_stSipUacCBQueue, *pulUacID);
    if(*pulUacID == NULL_USHORT)
    {
        return FAIL;
    }

    return SUCCESS;
}

/* �ͷ�UAC���ƿ� */
VOID SIP_UAC_FreeCB(ULONG ulUacID)
{
    COMM_CB_FREE(g_pstSipUacCB, g_stSipUacCBQueue, ulUacID);
    return;
}

/* Ӧ�÷���������Ϣ
ULONG ulAppID Ӧ��ID������Ӧ�ò�ƥ����Ӧ
ULONG ulDlgID �Ի�ID����Ϊ�ձ�ʶ�Ի�������
UBUF_HEADER_S * pstUbufSipMsg �����͵���Ϣ
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
        /* �Ի�������һЩ�ؼ�ͷ����Ҫ�Զ����� */
        SIP_UAC_GenerateRequest(pstUbufSipMsg);
    }
    else
    {
        /* �Ի�������ͨ���Ի��������ͷ�� */
        SIP_Dlg_GenerateRequest(ulDlgID, pstUbufSipMsg);
    }

    /* ����һ��UAC���ƿ� */
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
        /* ���������ɹ����� */
        SIP_UAC_LocateResult(&stResult, ulUacID);
    }
    else
    {
        /* δ�ɹ��ȴ������ɹ����� */
    }

    return SUCCESS;
}

/* �ײ�ʹ�øú����ϱ����� rfc3261 8.1.3 */
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

    /* �Ự�ͶԻ��Ĵ��� */
    pstSipRequest  = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUbufRequest);
    pstSipResponse = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUbufSipMsg);

    ulDlgID = g_pstSipUacCB[ulUacID].astFork[ucTxnSeq].ulDlgID;
    
    /*�Ի����INVITE�Ĵ����Ự*/
    if (pstSipRequest->uStartLine.stRequstLine.eMethod == SIP_METHOD_INVITE)
    {
        /* ��ʼINVITE�ķ�ʧ����Ӧ�ᴴ���Ի� */
        if ((ulDlgID == NULL_ULONG) 
          &&(pstSipResponse->uStartLine.stStatusLine.eStatusCode < SIP_STATUS_CODE_300))
        {
            pstHeaderCallID  = (SIP_HEADER_CALL_ID_S *)pstSipResponse->apstHeaders[SIP_HEADER_CALL_ID];
            pstHeaderFrom    = (SIP_HEADER_FROM_S *)pstSipResponse->apstHeaders[SIP_HEADER_FROM];
            pstHeaderTo      = (SIP_HEADER_TO_S *)pstSipResponse->apstHeaders[SIP_HEADER_TO];
            pstHeaderContact = (SIP_HEADER_CONTACT_S *)pstSipResponse->apstHeaders[SIP_HEADER_CONTACT];
            pstHeaderCseq    = (SIP_HEADER_CSEQ_S *)pstSipRequest->apstHeaders[SIP_HEADER_CSEQ];

            /* �����Ի� */
            SIP_Dlg_AllocDialog(pstHeaderCallID->pucCallID,
                                pstHeaderFrom->pucTag,
                                pstHeaderTo->pucTag, 
                               &ulDlgID);
            
            /* �����Ի� */
            SIP_Dlg_UpdateLocalURI(ulDlgID, pstHeaderFrom->stNameAddr.pstUri);
            SIP_Dlg_UpdateRemoteURI(ulDlgID, pstHeaderTo->stNameAddr.pstUri);
            SIP_Dlg_UpdateLocalSeq(ulDlgID, pstHeaderCseq->ulSeq);
            SIP_Dlg_UpdateRemoteTarget(ulDlgID, pstHeaderContact->pstParam->stAddr.pstUri);
            SIP_Dlg_UpdateSecureFlag(ulDlgID, FALSE);
            //SIP_Dlg_UpdateRouteSet(ulDlgID, pstRouteSet);
            
            g_pstSipUacCB[ulUacID].astFork[ucTxnSeq].ulDlgID = ulDlgID;

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
    
    /* �ϱ���Ӧ�ó��� */
    SIP_SendUpMsg(ulDlgID, 
                  ulUacID, 
                  g_pstSipUacCB[ulUacID].ulAppDlgID, 
                  g_pstSipUacCB[ulUacID].ulAppTxnID, 
                  pstUbufSipMsg);

    /* �յ��ս���Ӧ��UAS������� */
    if (pstSipResponse->uStartLine.stStatusLine.eStatusCode >= SIP_STATUS_CODE_200)
    {
        g_pstSipUacCB[ulUacID].astFork[ucTxnSeq].ulTxnID = NULL_ULONG;
        g_pstSipUacCB[ulUacID].astFork[ucTxnSeq].ulDlgID = NULL_ULONG;
    }

    /*  ����������ﶼ��ɣ���ɾ���ÿ��ƿ� */
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
        /* ֪ͨTU����ʧ�� */
    }

    ulUacID = ulPara;
    pstUacCB = &g_pstSipUacCB[ulUacID];

    /* �ҵ���һ����Ϳ������Viaͷ�� */
    SIP_UAC_AddViaHeader(pstUacCB->pstSipMsgUbuf);

    /* һ��һ�����ŷ� */
    pstSipMsg = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUacCB->pstSipMsgUbuf);
    if (pstSipMsg->uStartLine.stRequstLine.eMethod != SIP_METHOD_ACK)
    {
        /* ����ACKͨ�������� */

        /*����һ������*/
        SIP_Txn_AllocTxn(ulUacID, &ulTxnID);
        
        SIP_UAC_AllocTxnSeq(ulUacID, &ucSeq);
        pstUacCB->astFork[ucSeq].ulTxnID = ulTxnID;

        /*�������Ϸ�������*/
        SIP_Txn_RecvDownMsg(ulTxnID,
                            pstUacCB->pstSipMsgUbuf,
                           &pstResult->astLocations[0]);
    }
    else
    {
        /* ACKֱ�ӵ���TXP���� */
        SIP_Txp_RecvDownMsg(pstUacCB->pstSipMsgUbuf,
                           &pstResult->astLocations[0]);
    }

    return SUCCESS;
}

/* UAC����һ���Ի������� */
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

    /* From ͷ����Ϊ�� */
    if (pstSipMsg->apstHeaders[SIP_HEADER_FROM] == NULL_PTR)
    {
        return FAIL;
    }

    /*����From tag */
    pstFrom = (SIP_HEADER_FROM_S *)pstSipMsg->apstHeaders[SIP_HEADER_FROM];
    if (pstFrom->pucTag == NULL_PTR)
    {
        SIP_GenerateRandomString(acString, 100);
        UBUF_CLONE_STRING(acString, pstUbufSipMsg, pstFrom->pucTag);
    }

    /* To ͷ����Ϊ�� */
    if (pstSipMsg->apstHeaders[SIP_HEADER_TO] == NULL_PTR)
    {
        return FAIL;
    }

    /* ���� Request URI����Toͷ���и�ֵһ�ݾͿ����� */
    if (pstSipMsg->uStartLine.stRequstLine.pstRequestURI == NULL_PTR)
    {
        pstTo = (SIP_HEADER_TO_S *)pstSipMsg->apstHeaders[SIP_HEADER_TO];
        SIP_Syntax_Clone(ulRuleIndex,
                         pstTo->stNameAddr.pstUri,
                         pstUbufSipMsg,
                         &pstSipMsg->uStartLine.stRequstLine.pstRequestURI);
    }

    /* ����Call-IDͷ�� */
    if (pstSipMsg->apstHeaders[SIP_HEADER_CALL_ID] == NULL_PTR)
    {
        pstCallID = UBUF_AddComponent(pstUbufSipMsg, sizeof(SIP_HEADER_CALL_ID_S));
        pstSipMsg->apstHeaders[SIP_HEADER_CALL_ID] = (SIP_HEADER_S *)pstCallID;
        pstCallID->stHeader.pstNext = NULL_PTR;

        SIP_GenerateRandomString(acString, 100);
        UBUF_CLONE_STRING(acString, pstUbufSipMsg, pstCallID->pucCallID);
    }

    /* ����Cseqͷ�� */
    if (pstSipMsg->apstHeaders[SIP_HEADER_CSEQ] == NULL_PTR)
    {
        pstCseq = UBUF_AddComponent(pstUbufSipMsg, sizeof(SIP_HEADER_CSEQ_S));
        pstSipMsg->apstHeaders[SIP_HEADER_CSEQ] = (SIP_HEADER_S *)pstCseq;
        pstCseq->stHeader.pstNext = NULL_PTR;
        pstCseq->eMethod = pstSipMsg->uStartLine.stRequstLine.eMethod;
        pstCseq->ulSeq = 0;
    }

    /* ����Max-Forwardsͷ�� */
    if (pstSipMsg->apstHeaders[SIP_HEADER_MAX_FORWARDS] == NULL_PTR)
    {
        pstMaxForwards = UBUF_AddComponent(pstUbufSipMsg, sizeof(SIP_HEADER_MAX_FORWARDS_S));
        pstSipMsg->apstHeaders[SIP_HEADER_MAX_FORWARDS] = (SIP_HEADER_S *)pstMaxForwards;
        pstMaxForwards->stHeader.pstNext = NULL_PTR;
        pstMaxForwards->ulMaxForwards = 70;
    }

    /*�Ի����INVITEҪ�����Ի������Ҫ��Contactͷ��*/
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

    /* ����Viaͷ�� */
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

/* ��ȡ������UAC�е���ţ�Ҳ���������ж�����ĺϷ���*/
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

/* ����һ��������� */
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

/* �ж������Ƿ��Ѿ���ɣ����������й�������������ﶼ���*/
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
