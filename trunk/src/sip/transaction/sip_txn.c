/*******************************************************************************
功能    : SIP协议栈传输层功能函数
创建人  : 唐春平
创建日期: 2009.01.10
修改记录:
*******************************************************************************/
/* 外部依赖模块头文件 */
#include "common\common.h"
#include "abnf\abnf.h"
#include "ubuf\ubuf.h"
#include "sip\uri.h"
#include "sip\sip.h"
#include "s_hash\s_hash.h"

/* 本模块对外提供的常量和结构头文件 */
#include "..\include\transaction\sip_transaction.h"

/* 本模块内部使用的常量和结构头文件 */
#include "sip_txn_const.inc"
#include "sip_txn_type.inc"

/* 本模块内部函数声明头文件 */
#include "sip_txn_fsm.inc"
#include "sip_txn_mngt.inc"

/* 本模块全局变量声明头文件 */
#include "sip_txn_var.inc"

/*事务层初始化*/
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
    SIP_MSG_S      *pstSipMsg = NULL_PTR;
    SIP_TXN_CB_S   *pstSipTxnCB = NULL_PTR;

    pstSipTxnCB = &g_pstSipTxnCB[ulTxnID];

    /* 替换上次的发送消息，因为请求不会发送两次，因此
      不会释放初始请求消息*/
    UBUF_FreeBuffer(pstSipTxnCB->pstUbufSendMsg);
    pstSipTxnCB->pstUbufSendMsg = pstUbufSipMsg;

    pstSipMsg = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUbufSipMsg);
    if (pstSipMsg->eMsgType == SIP_MSG_TYPE_REQUEST)
    {
        /* 记录发送地址 */
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

        pstSipTxnCB->pstUbufInitMsg = pstUbufSipMsg;
        pstSipTxnCB->pstHashNode = HASH_AddNode(g_pstSipTxnCBHash, pstUbufSipMsg, ulTxnID);
    }
    else
    {
        /* 头域补充完整 */

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
    }

    SIP_Txn_FsmProc(ulTxnID, eEvent);
    return SUCCESS;
}

ULONG SIP_Txn_RecvUpMsg(ULONG ulTxnID,
                        UBUF_HEADER_S *pstUbufSipMsg,
                        SIP_LOCATION_S *pstPeerLocation)
{
    SIP_TXN_EVENT_E eEvent;
    SIP_MSG_S      *pstSipMsg   = NULL_PTR;
    SIP_TXN_CB_S   *pstSipTxnCB = NULL_PTR;

    pstSipTxnCB = &g_pstSipTxnCB[ulTxnID];

    /* 替换最新收到的消息 */
    UBUF_FreeBuffer(pstSipTxnCB->pstUbufRecvMsg);
    pstSipTxnCB->pstUbufRecvMsg = pstUbufSipMsg;
        
    pstSipMsg = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUbufSipMsg);
    if (pstSipMsg->eMsgType == SIP_MSG_TYPE_REQUEST)
    {
        /* 记录发送地址 */
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

        pstSipTxnCB->pstUbufInitMsg = pstUbufSipMsg;
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

    SIP_Txn_FsmProc(ulTxnID, eEvent);
    return SUCCESS;
}

ULONG SIP_Txn_TimerProc(ULONG ulTxnID, SIP_TXN_TIMER_NAME_E eTimer)
{
    SIP_TXN_EVENT_E      eEvent;

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

    SIP_Txn_FsmProc(ulTxnID, eEvent);
    return SUCCESS;
}
