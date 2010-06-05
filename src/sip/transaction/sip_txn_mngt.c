/*******************************************************************************
����    : SIPЭ��ջ����㹦�ܺ���
������  : �ƴ�ƽ
��������: 2009.01.10
�޸ļ�¼:
*******************************************************************************/
/* �ⲿ����ģ��ͷ�ļ� */
#include "common\common.h"
#include "abnf\abnf.h"
#include "ubuf\ubuf.h"
#include "uri\uri.h"
#include "sip\sip.h"
#include "s_hash\s_hash.h"

#include "..\include\shell\sip_shell.h"

/* ��ģ������ṩ�ĳ����ͽṹͷ�ļ� */
#include "..\include\transaction\sip_transaction.h"

/* ��ģ���ڲ�ʹ�õĳ����ͽṹͷ�ļ� */
#include "sip_txn_const.inc"
#include "sip_txn_type.inc"

/* ��ģ���ڲ���������ͷ�ļ� */
#include "sip_txn_fsm.inc"
#include "sip_txn_fsm.inc"
#include "sip_txn_mngt.inc"

/* ��ģ��ȫ�ֱ�������ͷ�ļ� */
#include "sip_txn_var.inc"

/* ����һ������ */
ULONG SIP_Txn_AllocTxn(ULONG ulCoreID, ULONG *pulTxnID)
{
    ULONG ulTxnID;
    SIP_TXN_CB_S  *pstSipTxnCB = NULL_PTR;

    COMM_CB_ALLOC(g_pstSipTxnCB, g_stSipTxnCBQueue, ulTxnID);
    if (ulTxnID == NULL_ULONG)
    {
        return FAIL;
    }

    pstSipTxnCB = &g_pstSipTxnCB[ulTxnID];

    /* ���ӿ��ƿ��ʼ�� */
    pstSipTxnCB->ulCoreID       = ulCoreID;
    pstSipTxnCB->eType          = SIP_TXN_TYPE_BUTT;
    pstSipTxnCB->eState         = SIP_TXN_STATE_INIT;
    pstSipTxnCB->pstUbufRequest = NULL_PTR;
    pstSipTxnCB->pstUbufResponse = NULL_PTR;
    pstSipTxnCB->ucReSendNum    = 0;
    memset(pstSipTxnCB->astTimers,
           0xff,
           SIP_TXN_MAX_FSM_TIMER*sizeof(SIP_TXN_TIMER_S));

    *pulTxnID = ulTxnID;
    return SUCCESS;
}

/* �ͷ�һ������ */
ULONG SIP_Txn_FreeTxn(ULONG ulTxnID)
{
    UCHAR         ucLoop;
    SIP_TXN_CB_S *pstSipTxnCB = NULL_PTR;

    pstSipTxnCB = &g_pstSipTxnCB[ulTxnID];
    
    /* ֹͣ��ʱ����Դ */
    for (ucLoop = 0; ucLoop < SIP_TXN_MAX_FSM_TIMER; ucLoop++)
    {
        if (pstSipTxnCB->astTimers[ucLoop].hTimerID != NULL_ULONG)
        {
            SIP_StopTimer(pstSipTxnCB->astTimers[ucLoop].hTimerID);
            pstSipTxnCB->astTimers[ucLoop].hTimerID   = NULL_ULONG;
            pstSipTxnCB->astTimers[ucLoop].eTimerName = NULL_ULONG;
        }
    }

    /* �ͷŷ�����ڴ� */
    if (pstSipTxnCB->pstUbufRequest != NULL_PTR)
    {
        HASH_DeleteNode(pstSipTxnCB->pstHashNode);
        UBUF_FreeBuffer(pstSipTxnCB->pstUbufRequest);
    }

    /* �ͷŷ�����ڴ� */
    if (pstSipTxnCB->pstUbufResponse != NULL_PTR)
    {
        UBUF_FreeBuffer(pstSipTxnCB->pstUbufResponse);
    }

    COMM_CB_FREE(g_pstSipTxnCB, g_stSipTxnCBQueue, ulTxnID);

    return SUCCESS;
}

ULONG SIP_Txn_Match(UBUF_HEADER_S *pstUbufSipMsg, ULONG *pulTxnID)
{
    ULONG ulTxnID;

    /* ������SIPʵ�壬ֱ�ӷ��ش��� */
#if 0
    if ()
    {
        return FAIL;
    }
#endif
    ulTxnID = (USHORT)HASH_Find((VOID *)g_pstSipTxnCBHash, (VOID *)pstUbufSipMsg);
    if (ulTxnID == NULL_USHORT)
    {
        return FAIL;
    }

    *pulTxnID = ulTxnID;
    return SUCCESS;
}

/* keyֵ�����㷨 */
ULONG SIP_Txn_MakeKey(void *pCompared)
{
    UCHAR *pucChar;
    ULONG  ulKey = 0;
    UBUF_HEADER_S    *pstUbufSipMsg = NULL_PTR;
    SIP_MSG_S        *pstSipMsg     = NULL_PTR;
    SIP_HEADER_VIA_S *pstHeaderVia  = NULL_PTR;
    SIP_VIA_PARM_S   *pstViaPara    = NULL_PTR;

    pstUbufSipMsg = (UBUF_HEADER_S *)pCompared;
    pstSipMsg    = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUbufSipMsg);
    pstHeaderVia = (SIP_HEADER_VIA_S *)pstSipMsg->apstHeaders[SIP_HEADER_VIA];
    pstViaPara   = pstHeaderVia->pstViaParm;
    pucChar      = pstViaPara->pucBranch;
    while(*pucChar != 0)
    {
        ulKey += *pucChar;
        pucChar++;
    }

    return ulKey;
}

/* �ȽϺ��� */
ULONG SIP_Txn_Compare(void *pCompared, ULONG ulPara)
{
    ULONG ulRet;
    UBUF_HEADER_S    *pstUbufSipMsg = NULL_PTR;
    SIP_MSG_S        *pstSipMsg     = NULL_PTR;
    SIP_TXN_CB_S     *pstSipTxnCB = NULL_PTR;

    pstUbufSipMsg = (UBUF_HEADER_S *)pCompared;
    pstSipMsg     = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUbufSipMsg);
    pstSipTxnCB = &g_pstSipTxnCB[ulPara];

    if (pstSipMsg->eMsgType == SIP_MSG_TYPE_REQUEST)
    {
        ulRet = SIP_Txn_CompareRequest(pstUbufSipMsg, g_pstSipTxnCB[ulPara].pstUbufRequest);
    }
    else
    {
        ulRet = SIP_Txn_CompareResponse(pstUbufSipMsg, g_pstSipTxnCB[ulPara].pstUbufRequest);
    }

    return ulRet;
}

/* ƥ����Ӧ���ͻ������񣬲μ�rfc3261 17.1.3 */
ULONG SIP_Txn_CompareResponse(UBUF_HEADER_S *pstUbufResponse,
                              UBUF_HEADER_S *pstUbufTxnInitMsg)
{
    SIP_MSG_S *pstSipMsg = NULL_PTR;
    SIP_HEADER_VIA_S *pstHeaderVia;
    SIP_VIA_PARM_S   *pstViaPara = NULL_PTR;
    SIP_HEADER_CSEQ_S *pstHeaderCseq;
    UCHAR *pucBranch1 = NULL_PTR;
    UCHAR *pucBranch2 = NULL_PTR;
    SIP_METHOD_E eMethod1;
    SIP_METHOD_E eMethod2;

    pstSipMsg     = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUbufResponse);
    pstHeaderVia  = (SIP_HEADER_VIA_S *)pstSipMsg->apstHeaders[SIP_HEADER_VIA];
    pstViaPara    = pstHeaderVia->pstViaParm;
    pucBranch1    = pstViaPara->pucBranch;
    pstHeaderCseq = (SIP_HEADER_CSEQ_S *)pstSipMsg->apstHeaders[SIP_HEADER_CSEQ];
    eMethod1      = pstHeaderCseq->eMethod;

    pstSipMsg     = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUbufTxnInitMsg);
    pstHeaderVia  = (SIP_HEADER_VIA_S *)pstSipMsg->apstHeaders[SIP_HEADER_VIA];
    pstViaPara    = pstHeaderVia->pstViaParm;
    pucBranch2    = pstViaPara->pucBranch;
    pstHeaderCseq = (SIP_HEADER_CSEQ_S *)pstSipMsg->apstHeaders[SIP_HEADER_CSEQ];
    eMethod2      = pstHeaderCseq->eMethod;

    if (eMethod1 != eMethod2)
    {
        return FAIL;
    }

    if(strcmp(pucBranch1, pucBranch1) != 0)
    {
        return FAIL;
    }

    return SUCCESS;
}

/* �ͻ������������Ӧƥ�����󣬲μ�rfc3261 17.2.3 */
ULONG SIP_Txn_CompareRequest(UBUF_HEADER_S *pstUbufRequest,
                             UBUF_HEADER_S *pstUbufTxnInitMsg)
{
    SIP_MSG_S *pstSipMsg = NULL_PTR;
    SIP_HEADER_VIA_S *pstHeaderVia;
    SIP_VIA_PARM_S *pstViaPara = NULL_PTR;
    SIP_HEADER_CSEQ_S *pstHeaderCseq;
    UCHAR *pucBranch1  = NULL_PTR;
    UCHAR *pucBranch2 = NULL_PTR;
    SIP_METHOD_E eMethod1;
    SIP_METHOD_E eMethod2;

    pstSipMsg     = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUbufRequest);
    pstHeaderVia  = (SIP_HEADER_VIA_S *)pstSipMsg->apstHeaders[SIP_HEADER_VIA];
    pstViaPara    = pstHeaderVia->pstViaParm;
    pucBranch1    = pstViaPara->pucBranch;
    pstHeaderCseq = (SIP_HEADER_CSEQ_S *)pstSipMsg->apstHeaders[SIP_HEADER_CSEQ];
    eMethod1      = pstHeaderCseq->eMethod;

    pstSipMsg     = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUbufTxnInitMsg);
    pstHeaderVia  = (SIP_HEADER_VIA_S *)pstSipMsg->apstHeaders[SIP_HEADER_VIA];
    pstViaPara    = pstHeaderVia->pstViaParm;
    pucBranch2    = pstViaPara->pucBranch;
    pstHeaderCseq = (SIP_HEADER_CSEQ_S *)pstSipMsg->apstHeaders[SIP_HEADER_CSEQ];
    eMethod2      = pstHeaderCseq->eMethod;

    /* ʹ��ħ���ַ������ж��Ƿ�˳��3261 */
    if(strncpy(pucBranch1, "z9hG4bK", strlen("z9hG4bK")) == 0)
    {
        /*��ѭrfc3261*/

        if ((eMethod1 != eMethod2) && (eMethod1 != SIP_METHOD_ACK))
        {
            return FAIL;
        }

#if 0
        if (SIP_Txn_CompareHost() != SUCCESS)
        {
            return FAIL;
        }
#endif
        if(strcpy(pucBranch1, pucBranch2) != 0)
        {
            return FAIL;
        }
    }
    else
    {
        /* ����rfc2543�����ݲ�֧�� */
        return FAIL;
    }

    return SUCCESS;
}

ULONG SIP_Txn_CompareSendBy(UBUF_HEADER_S *pstUbufRequest,
                            UBUF_HEADER_S *pstUbufTxnInitMsg)
{
    return SUCCESS;
}

/* ����branch���� */
ULONG SIP_Txn_GenerateBranch(ULONG  *pulTxnID)
{
    return SUCCESS;
}

/* ����branch���� */
UBUF_HEADER_S* SIP_Txn_GetInitMsg(ULONG ulTxnID)
{
    return g_pstSipTxnCB[ulTxnID].pstUbufRequest;
}

