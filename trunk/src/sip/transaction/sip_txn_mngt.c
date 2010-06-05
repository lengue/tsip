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
#include "uri\uri.h"
#include "sip\sip.h"
#include "s_hash\s_hash.h"

#include "..\include\shell\sip_shell.h"

/* 本模块对外提供的常量和结构头文件 */
#include "..\include\transaction\sip_transaction.h"

/* 本模块内部使用的常量和结构头文件 */
#include "sip_txn_const.inc"
#include "sip_txn_type.inc"

/* 本模块内部函数声明头文件 */
#include "sip_txn_fsm.inc"
#include "sip_txn_fsm.inc"
#include "sip_txn_mngt.inc"

/* 本模块全局变量声明头文件 */
#include "sip_txn_var.inc"

/* 分配一个事务 */
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

    /* 连接控制块初始化 */
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

/* 释放一个事务 */
ULONG SIP_Txn_FreeTxn(ULONG ulTxnID)
{
    UCHAR         ucLoop;
    SIP_TXN_CB_S *pstSipTxnCB = NULL_PTR;

    pstSipTxnCB = &g_pstSipTxnCB[ulTxnID];
    
    /* 停止定时器资源 */
    for (ucLoop = 0; ucLoop < SIP_TXN_MAX_FSM_TIMER; ucLoop++)
    {
        if (pstSipTxnCB->astTimers[ucLoop].hTimerID != NULL_ULONG)
        {
            SIP_StopTimer(pstSipTxnCB->astTimers[ucLoop].hTimerID);
            pstSipTxnCB->astTimers[ucLoop].hTimerID   = NULL_ULONG;
            pstSipTxnCB->astTimers[ucLoop].eTimerName = NULL_ULONG;
        }
    }

    /* 释放分配的内存 */
    if (pstSipTxnCB->pstUbufRequest != NULL_PTR)
    {
        HASH_DeleteNode(pstSipTxnCB->pstHashNode);
        UBUF_FreeBuffer(pstSipTxnCB->pstUbufRequest);
    }

    /* 释放分配的内存 */
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

    /* 非事务SIP实体，直接返回错误 */
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

/* key值生成算法 */
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

/* 比较函数 */
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

/* 匹配响应到客户端事务，参见rfc3261 17.1.3 */
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

/* 客户端事务根据响应匹配请求，参见rfc3261 17.2.3 */
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

    /* 使用魔法字符串来判断是否顺从3261 */
    if(strncpy(pucBranch1, "z9hG4bK", strlen("z9hG4bK")) == 0)
    {
        /*遵循rfc3261*/

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
        /* 兼容rfc2543处理，暂不支持 */
        return FAIL;
    }

    return SUCCESS;
}

ULONG SIP_Txn_CompareSendBy(UBUF_HEADER_S *pstUbufRequest,
                            UBUF_HEADER_S *pstUbufTxnInitMsg)
{
    return SUCCESS;
}

/* 生成branch参数 */
ULONG SIP_Txn_GenerateBranch(ULONG  *pulTxnID)
{
    return SUCCESS;
}

/* 生成branch参数 */
UBUF_HEADER_S* SIP_Txn_GetInitMsg(ULONG ulTxnID)
{
    return g_pstSipTxnCB[ulTxnID].pstUbufRequest;
}

