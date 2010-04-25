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
#include "sip\uri.h"
#include "sip\sip.h"
#include "s_hash\s_hash.h"

/* ��ģ������ṩ�ĳ����ͽṹͷ�ļ� */
#include "..\include\transaction\sip_transaction.h"

/* ��ģ���ڲ�ʹ�õĳ����ͽṹͷ�ļ� */
#include "sip_txn_const.inc"
#include "sip_txn_type.inc"

/* ��ģ���ڲ���������ͷ�ļ� */
#include "sip_txn_fsm.inc"
#include "sip_txn_mngt.inc"

/* ��ģ��ȫ�ֱ�������ͷ�ļ� */
#include "sip_txn_var.inc"

/*������ʼ��*/
ULONG SIP_Txn_Init(SIP_TXN_CFG_S *pstCfg)
{
    COMM_CB_INIT(SIP_TXN_CB_NUM, sizeof(SIP_TXN_CB_S), g_pstSipTxnCB, g_stSipTxnCBQueue);
    g_ulSipTxnT1 = pstCfg->ulTimerT1;

    SIP_Txn_FsmInit();
    g_pstSipTxnCBHash = HASH_CreateTable(SIP_TXN_CB_NUM,
                                         SIP_Txn_MakeKey,
                                         SIP_Txn_Compare);
    return SUCCESS;
}

ULONG SIP_Txn_RecvDownMsg(ULONG ulTxnID,
                          UBUF_HEADER_S  *pstUbufSipMsg,
                          SIP_LOCATION_S *pstPeerLocation)
{
    SIP_TXN_EVENT_E eEvent;
    ULONG ulRuleIndex;
    SIP_MSG_S      *pstSipMsg = NULL_PTR;
    SIP_MSG_S      *pstSipMsgTemp = NULL_PTR;
    SIP_TXN_CB_S   *pstSipTxnCB = NULL_PTR;
    UBUF_HEADER_S  *pstUbufBuffer = NULL_PTR;

    pstSipTxnCB = &g_pstSipTxnCB[ulTxnID];
    pstSipMsg = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUbufSipMsg);

    pstUbufBuffer = UBUF_AllocUBuf(SIP_MAX_UBUF_MSG_LEN);
    SIP_GetRuleIndex("SIP-message", &ulRuleIndex);
    SIP_Clone(ulRuleIndex, pstSipMsg, pstUbufBuffer, &pstSipMsgTemp);

    if (pstSipMsg->eMsgType == SIP_MSG_TYPE_REQUEST)
    {
        /* ��¼���͵�ַ */
        memcpy(&pstSipTxnCB->stPeer, pstPeerLocation, sizeof(SIP_LOCATION_S));

        eEvent = SIP_TXN_EVENT_SEND_REQUEST;
        if(pstSipMsg->uStartLine.stRequstLine.eMethod == SIP_METHOD_INVITE)
        {
            pstSipTxnCB->eType = SIP_TXN_TYPE_INVITE_CLIENT;
        }
        else
        {
            pstSipTxnCB->eType = SIP_TXN_TYPE_NON_INVITE_CLIENT;
        }

        /* �����漰���ط���ƥ�䣬��Ҫ����һ�� */
        pstSipTxnCB->pstUbufRequest = pstUbufBuffer;
        pstSipTxnCB->pstHashNode = HASH_AddNode(g_pstSipTxnCBHash, pstUbufSipMsg, ulTxnID);
    }
    else
    {
        /* ͷ�򲹳����� */
        if (pstSipMsg->uStartLine.stStatusLine.eStatusCode < SIP_STATUS_CODE_200)
        {
            eEvent = SIP_TXN_EVENT_SEND_1XX_RESPONSE;
        }
        else if (pstSipMsg->uStartLine.stStatusLine.eStatusCode < SIP_STATUS_CODE_300)
        {
            eEvent = SIP_TXN_EVENT_SEND_2XX_RESPONSE;
        }
        else
        {
            eEvent = SIP_TXN_EVENT_SEND_3456XX_RESPONSE;
        }

        pstSipTxnCB->pstUbufResponse = pstUbufBuffer;
    }

    SIP_Txn_FsmProc(ulTxnID, eEvent, pstUbufSipMsg);
    
    return SUCCESS;
}

ULONG SIP_Txn_RecvUpMsg(ULONG ulTxnID,
                        UBUF_HEADER_S *pstUbufSipMsg,
                        SIP_LOCATION_S *pstPeerLocation)
{
    SIP_TXN_EVENT_E eEvent;
    SIP_MSG_S      *pstSipMsg   = NULL_PTR;
    SIP_MSG_S      *pstSipMsgTemp = NULL_PTR;
    SIP_TXN_CB_S   *pstSipTxnCB = NULL_PTR;
    ULONG           ulRuleIndex;

    pstSipTxnCB = &g_pstSipTxnCB[ulTxnID];
    pstSipMsg = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUbufSipMsg);
    if (pstSipMsg->eMsgType == SIP_MSG_TYPE_REQUEST)
    {
        /* ��¼���͵�ַ */
        memcpy(&pstSipTxnCB->stPeer, pstPeerLocation, sizeof(SIP_LOCATION_S));

        if (pstSipMsg->uStartLine.stRequstLine.eMethod == SIP_METHOD_ACK)
        {
            eEvent = SIP_TXN_EVENT_RECV_ACK;
        }
        else if(pstSipMsg->uStartLine.stRequstLine.eMethod == SIP_METHOD_INVITE)
        {
            pstSipTxnCB->eType = SIP_TXN_TYPE_INVITE_SERVER;
            eEvent = SIP_TXN_EVENT_RECV_REQUEST;
        }
        else
        {
            pstSipTxnCB->eType = SIP_TXN_TYPE_NON_INVITE_SERVER;
            eEvent = SIP_TXN_EVENT_RECV_REQUEST;
        }

        /* �����漰���ط���ƥ�䣬��Ҫ����һ�� */
        pstSipTxnCB->pstUbufRequest = UBUF_AllocUBuf(SIP_MAX_UBUF_MSG_LEN);
        SIP_GetRuleIndex("SIP-message", &ulRuleIndex);
        SIP_Clone(ulRuleIndex, pstSipMsg, pstSipTxnCB->pstUbufRequest, &pstSipMsgTemp);
        pstSipTxnCB->pstHashNode = HASH_AddNode(g_pstSipTxnCBHash, pstUbufSipMsg, ulTxnID);
    }
    else
    {
        if (pstSipMsg->uStartLine.stStatusLine.eStatusCode < SIP_STATUS_CODE_200)
        {
            eEvent = SIP_TXN_EVENT_RECV_1XX_RESPONSE;
        }
        else if (pstSipMsg->uStartLine.stStatusLine.eStatusCode < SIP_STATUS_CODE_300)
        {
            eEvent = SIP_TXN_EVENT_RECV_2XX_RESPONSE;
        }
        else
        {
            eEvent = SIP_TXN_EVENT_RECV_3456XX_RESPONSE;
        }
    }

    SIP_Txn_FsmProc(ulTxnID, eEvent, pstUbufSipMsg);
    
    return SUCCESS;
}

ULONG SIP_Txn_TimerProc(ULONG ulTxnID, SIP_TXN_TIMER_NAME_E eTimer)
{
    SIP_TXN_EVENT_E      eEvent;
    UCHAR                ucSeq;
    ULONG                ulRet;
    SIP_TXN_CB_S   *pstSipTxnCB = NULL_PTR;

    pstSipTxnCB = &g_pstSipTxnCB[ulTxnID];
    ulRet = SIP_Txn_LookupTimer(ulTxnID, eTimer, &ucSeq);
    if (ulRet != SUCCESS)
    {
        /* �Ҳ���˵������û����Ҳ���ô��� */
        return ulRet;
    }
    else
    {
        /* �ҵ��󽫾����� */
        pstSipTxnCB->astTimers[ucSeq].eTimerName = NULL_ULONG;
        pstSipTxnCB->astTimers[ucSeq].hTimerID   = NULL_ULONG;
    }

    switch (eTimer)
    {
        case SIP_TXN_TIMER_A:
            eEvent = SIP_TXN_EVENT_TIMER_A_FIRE;
            break;

        case SIP_TXN_TIMER_B:
            eEvent = SIP_TXN_EVENT_TIMER_B_FIRE;
            break;

        case SIP_TXN_TIMER_D:
            eEvent = SIP_TXN_EVENT_TIMER_D_FIRE;
            break;

        case SIP_TXN_TIMER_E:
            eEvent = SIP_TXN_EVENT_TIMER_E_FIRE;
            break;

        case SIP_TXN_TIMER_F:
            eEvent = SIP_TXN_EVENT_TIMER_F_FIRE;
            break;

        case SIP_TXN_TIMER_K:
            eEvent = SIP_TXN_EVENT_TIMER_K_FIRE;
            break;

        case SIP_TXN_TIMER_G:
            eEvent = SIP_TXN_EVENT_TIMER_G_FIRE;
            break;

        case SIP_TXN_TIMER_H:
            eEvent = SIP_TXN_EVENT_TIMER_H_FIRE;
            break;

        case SIP_TXN_TIMER_I:
            eEvent = SIP_TXN_EVENT_TIMER_I_FIRE;
            break;

        case SIP_TXN_TIMER_J:
            eEvent = SIP_TXN_EVENT_TIMER_J_FIRE;
            break;

        default:
            return FAIL;
    }

    SIP_Txn_FsmProc(ulTxnID, eEvent, NULL_PTR);
    return SUCCESS;
}
