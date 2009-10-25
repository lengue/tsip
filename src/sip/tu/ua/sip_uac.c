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
        /* �Ի�������һЩ�ؼ�ͷ����Ҫ�Զ����� */
        SIP_UAC_GenerateRequest(pstUbufSipMsg);
    }
    else
    {
        /* �Ի�������ͨ���Ի��������ͷ�� */
    }

    /* ����һ��UAC���ƿ� */
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
ULONG SIP_UAC_ProcessingResponse(ULONG ulAppRef1,
                                 ULONG ulAppRef2,
                                 SIP_MSG_S *pstSipMsg)
{
    /* �鿴�Ƿ񴴽��Ի� */
    /* ��������˶Ի�Я���Ի����� */
    /* �ϱ���Ӧ�ó��� */
    /* �����������Ӧ��ɾ���ÿ��ƿ� */
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
        SIP_Txn_AllocTxn(&pstResult->astLocations[0], &ulTxnID);

        /*�������Ϸ�������*/
        SIP_Txn_RecvDownMsg(ulTxnID,
                            pstUacCB->pstSipMsgUbuf);
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

    /* From ͷ����Ϊ�� */
    if (pstSipMsg->apstHeaders[SIP_HEADER_FROM] == NULL_PTR)
    {
        return FAIL;
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
        SIP_Syntax_GetRuleIndex("addr-spec", &ulRuleIndex);
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
        pucString = UBUF_AddComponent(pstUbufSipMsg,
                                     (ULONG)strlen(acString)+1);
        pstCallID->pucCallID = pucString;
        memcpy(pucString, acString, strlen(acString)+1);
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
    pucString = UBUF_AddComponent(pstUbufSipMsg,
                                  (ULONG)strlen(acBranch) + 1);
    pstViaPara->pucBranch = pucString;
    memcpy(pucString, acBranch, strlen(acBranch)+1);
    return SUCCESS;
}
