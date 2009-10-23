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

/* 分配一个事务 */
ULONG SIP_Txn_AllocTxn(SIP_LOCATION_S *pstPeerAddr,
                       ULONG          *pulTxnID)
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
    pstSipTxnCB->eType         = SIP_TXN_TYPE_BUTT;
    pstSipTxnCB->eState        = SIP_TXN_STATE_INIT;
    pstSipTxnCB->pstUbufInitMsg = NULL_PTR;
    pstSipTxnCB->pstUbufSendMsg = NULL_PTR;
    pstSipTxnCB->pstUbufRecvMsg = NULL_PTR;
    memset(pstSipTxnCB->astTimers,
           0xff,
           SIP_TXN_MAX_FSM_TIMER*sizeof(SIP_TXN_TIMER_S));

    memcpy(&pstSipTxnCB->stPeer, pstPeerAddr, sizeof(SIP_LOCATION_S));

    *pulTxnID = ulTxnID;
    return SUCCESS;
}

/* 释放一个事务 */
ULONG SIP_Txn_FreeTxn(ULONG ulTxnID)
{
    /* 停止定时器资源 */

    /* 释放分配的内存 */

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
    SIP_HEADER_S     *pstHeader     = NULL_PTR;
    SIP_HEADER_VIA_S *pstHeaderVia  = NULL_PTR;
    SIP_VIA_PARM_S   *pstViaPara    = NULL_PTR;

    pstUbufSipMsg = (UBUF_HEADER_S *)pCompared;
    pstSipMsg    = UBUF_UBufPtr2Ptr(pstUbufSipMsg, 0);
    pstHeader    = UBUF_UBufPtr2Ptr(pstUbufSipMsg, pstSipMsg->aupstHeaders[SIP_HEADER_VIA]);
    pstHeaderVia = pstHeader->pstSpec;
    pstViaPara   = UBUF_UBufPtr2Ptr(pstUbufSipMsg, pstHeaderVia->upstViaParm);
    pucChar      = UBUF_UBufPtr2Ptr(pstUbufSipMsg, pstViaPara->upucBranch);
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
    pstSipMsg     = UBUF_UBufPtr2Ptr(pstUbufSipMsg, 0);
    pstSipTxnCB = &g_pstSipTxnCB[ulPara];

    if (pstSipMsg->eMsgType == SIP_MSG_TYPE_REQUEST)
    {
        ulRet = SIP_Txn_CompareRequest(pstUbufSipMsg, g_pstSipTxnCB[ulPara].pstUbufInitMsg);
    }
    else
    {
        ulRet = SIP_Txn_CompareResponse(pstUbufSipMsg, g_pstSipTxnCB[ulPara].pstUbufInitMsg);
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

    pstSipMsg     = UBUF_UBufPtr2Ptr(pstUbufResponse, 0);
    pstHeaderVia  = UBUF_UBufPtr2Ptr(pstUbufResponse, pstSipMsg->aupstHeaders[SIP_HEADER_VIA]);
    pstViaPara    = UBUF_UBufPtr2Ptr(pstUbufResponse, pstHeaderVia->upstViaParm);
    pucBranch1    = UBUF_UBufPtr2Ptr(pstUbufResponse, pstViaPara->upucBranch);
    pstHeaderCseq = UBUF_UBufPtr2Ptr(pstUbufResponse, pstSipMsg->aupstHeaders[SIP_HEADER_CSEQ]);
    eMethod1      = pstHeaderCseq->eMethod;

    pstSipMsg     = UBUF_UBufPtr2Ptr(pstUbufTxnInitMsg, 0);
    pstHeaderVia  = UBUF_UBufPtr2Ptr(pstUbufTxnInitMsg, pstSipMsg->aupstHeaders[SIP_HEADER_VIA]);
    pstViaPara    = UBUF_UBufPtr2Ptr(pstUbufTxnInitMsg, pstHeaderVia->upstViaParm);
    pucBranch2    = UBUF_UBufPtr2Ptr(pstUbufTxnInitMsg, pstViaPara->upucBranch);
    pstHeaderCseq = UBUF_UBufPtr2Ptr(pstUbufTxnInitMsg, pstSipMsg->aupstHeaders[SIP_HEADER_CSEQ]);
    eMethod2      = pstHeaderCseq->eMethod;

    if (eMethod1 != eMethod2)
    {
        return FAIL;
    }

    if(strcpy(pucBranch1, pucBranch1) != 0)
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
    SIP_VIA_PARM_S *pstViaPara1 = NULL_PTR;
    SIP_VIA_PARM_S *pstViaPara2 = NULL_PTR;
    SIP_HEADER_CSEQ_S *pstHeaderCseq;
    UCHAR *pucBranch1  = NULL_PTR;
    UCHAR *pucBranch2 = NULL_PTR;
    SIP_METHOD_E eMethod1;
    SIP_METHOD_E eMethod2;

    pstSipMsg     = UBUF_UBufPtr2Ptr(pstUbufRequest, 0);
    pstHeaderVia  = UBUF_UBufPtr2Ptr(pstUbufRequest, pstSipMsg->aupstHeaders[SIP_HEADER_VIA]);
    pstViaPara1   = UBUF_UBufPtr2Ptr(pstUbufRequest, pstHeaderVia->upstViaParm);
    pucBranch1    = UBUF_UBufPtr2Ptr(pstUbufRequest, pstViaPara1->upucBranch);
    pstHeaderCseq = UBUF_UBufPtr2Ptr(pstUbufRequest, pstSipMsg->aupstHeaders[SIP_HEADER_CSEQ]);
    eMethod1      = pstHeaderCseq->eMethod;

    pstSipMsg     = UBUF_UBufPtr2Ptr(pstUbufTxnInitMsg, 0);
    pstHeaderVia  = UBUF_UBufPtr2Ptr(pstUbufTxnInitMsg, pstSipMsg->aupstHeaders[SIP_HEADER_VIA]);
    pstViaPara2   = UBUF_UBufPtr2Ptr(pstUbufTxnInitMsg, pstHeaderVia->upstViaParm);
    pucBranch2    = UBUF_UBufPtr2Ptr(pstUbufTxnInitMsg, pstViaPara1->upucBranch);
    pstHeaderCseq = UBUF_UBufPtr2Ptr(pstUbufTxnInitMsg, pstSipMsg->aupstHeaders[SIP_HEADER_CSEQ]);
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
