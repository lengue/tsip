/*******************************************************************************
功能    : 传输层消息管理(WIN32专用)
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/
/* 外部依赖模块头文件 */
#include "common\common.h"
#include "abnf\abnf.h"
#include "ubuf\ubuf.h"
#include "sip\uri.h"
#include "sip\sip.h"

#include "..\include\syntax\sip_syntax.h"
#include "..\include\shell\sip_shell.h"
#include "..\include\transaction\sip_transaction.h"
#include "..\include\tu\sip_tu.h"

/* 本模块对外提供的常量和结构头文件 */
#include "..\include\transport\sip_transport.h"

/* 本模块内部使用的常量和结构头文件 */

/* 本模块内部函数声明头文件 */
#include "sip_transport.inc"

/* 本模块全局变量声明头文件 */
#include "sip_transport_var.inc"

ULONG SIP_Txp_Init(SIP_TXP_CFG_S *pstCfg)
{
    g_pucSipTextMsg = malloc(SIP_MAX_TEXT_MSG_LEN);
    g_pucSipTxpSendBy = malloc(100);
    strcpy(g_pucSipTxpSendBy, pstCfg->pucSendby);
    return SUCCESS;
}

/* 收到上层的下行消息
pstSipMsgUbuf 用于发送的消息
pstPeerLocation 消息目的端地址
*/
ULONG SIP_Txp_RecvDownMsg(UBUF_HEADER_S  *pstSipMsgUbuf,
                          SIP_LOCATION_S *pstPeerLocation)
{
    ULONG ulRet;
    SIP_MSG_S *pstSipMsg = NULL_PTR;

    pstSipMsg = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstSipMsgUbuf);

    /* 如果是请求需要添加Via头域 */
    if (pstSipMsg->eMsgType == SIP_MSG_TYPE_REQUEST)
    {
        ulRet = SIP_Txp_AddViaHeaderInRequest(pstSipMsgUbuf, pstPeerLocation);
        if (ulRet != SUCCESS)
        {
            return FAIL;
        }
    }

    SIP_SendDownMsg(pstSipMsgUbuf,
                    pstPeerLocation);

    return SUCCESS;
}

/* 收到网络侧来的上行消息 */
ULONG SIP_Txp_RecvUpMsg(SIP_LOCATION_S *pstPeerLocation,
                        UBUF_HEADER_S  *pstUbufSipMsg)
{
    ULONG ulRet;
    SIP_MSG_S     *pstSipMsg = NULL_PTR;

    pstSipMsg = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUbufSipMsg);

    /* 按消息类型处理 */
    if (pstSipMsg->eMsgType == SIP_MSG_TYPE_REQUEST)
    {
        ulRet = SIP_Txp_ReceiveRequest(pstPeerLocation, pstUbufSipMsg);
    }
    else
    {
        ulRet = SIP_Txp_ReceiveReponse(pstPeerLocation, pstUbufSipMsg);
    }

    return ulRet;
}

ULONG SIP_Txp_ReceiveRequest(SIP_LOCATION_S *pstPeerLocation,
                             UBUF_HEADER_S *pstUbufSipMsg)
{
    ULONG ulRet;
    UCHAR *pucAddr = NULL_PTR;
    ULONG ulTxnID;
    SIP_MSG_S *pstSipMsg = NULL_PTR;
    SIP_HEADER_VIA_S *pstViaHeader = NULL_PTR;
    SIP_VIA_PARM_S   *pstViaPara = NULL_PTR;

    pstSipMsg = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUbufSipMsg);

    /* 检查sent-by字段 */
    pstViaHeader = (SIP_HEADER_VIA_S *)pstSipMsg->apstHeaders[SIP_HEADER_VIA];
    pstViaPara= pstViaHeader->pstViaParm;
    pucAddr = pstViaPara->stSendBy.stHost.pucAddrStr;
    if ((pstViaPara->stSendBy.stHost.eHostType == URI_HOST_DOMAIN)
      ||(strncmp(pucAddr,pstPeerLocation->aucIPStr, strlen(pstPeerLocation->aucIPStr))))
    {
        /* 包含的是域名或者接受到的IP地址和原来不同则需要增加received参数 */
    }

    /* 匹配服务器端事务 */
    ulRet = SIP_Txn_Match(pstUbufSipMsg, &ulTxnID);
    if (ulRet != SUCCESS)
    {
        SIP_TU_RecvUpMsg(NULL_ULONG, pstPeerLocation, pstUbufSipMsg);
    }
    else
    {
        SIP_Txn_RecvUpMsg(ulTxnID, pstUbufSipMsg);
    }

    return SUCCESS;
}

ULONG SIP_Txp_ReceiveReponse(SIP_LOCATION_S *pstPeerLocation,
                             UBUF_HEADER_S *pstUbufSipMsg)
{
    ULONG ulRet = SUCCESS;
//    ULONG ulIpAddr;
    ULONG ulTxnID;
    SIP_MSG_S *pstSipMsg = NULL_PTR;
    SIP_HEADER_VIA_S *pstViaHeader = NULL_PTR;
    SIP_VIA_PARM_S *pstViaPara = NULL_PTR;

    pstSipMsg    = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUbufSipMsg);
    pstViaHeader = (SIP_HEADER_VIA_S *)pstSipMsg->apstHeaders[SIP_HEADER_VIA];
    pstViaPara   = pstViaHeader->pstViaParm;

    /*检查send-by字段*/
    if (pstViaPara == NULL_PTR)
    {
        /* 检查失败则丢弃消息 */
        return FAIL;
    }

    /* 匹配事务 */
    ulRet = SIP_Txn_Match(pstUbufSipMsg, &ulTxnID);
    if (ulRet != SUCCESS)
    {
        SIP_TU_RecvUpMsg(NULL_ULONG, pstPeerLocation, pstUbufSipMsg);
    }
    else
    {
        SIP_Txn_RecvUpMsg(ulTxnID, pstUbufSipMsg);
    }

    return SUCCESS;
}


/* 在请求中添加Via头域，使用传输层发送请求，顺从rfc3261 18.1.1 */
ULONG SIP_Txp_AddViaHeaderInRequest(UBUF_HEADER_S *pstUbufSipMsg,
                                    SIP_LOCATION_S *pstPeerLocation)
{
    SIP_MSG_S        *pstSipMsg = NULL_PTR;
    SIP_HEADER_VIA_S *pstVia    = NULL_PTR;
    SIP_VIA_PARM_S   *pstViaPara =  NULL_PTR;
    UCHAR            *pucString = NULL_PTR;

    pstSipMsg = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUbufSipMsg);

    /* 生成Via头域 */
    pstVia     = (SIP_HEADER_VIA_S *)pstSipMsg->apstHeaders[SIP_HEADER_VIA];
    pstViaPara = pstVia->pstViaParm;
#if 0
    /* 目标地址是多播地址 */
    if ((pstPeerLocation->ulIpAddr&0xe0000000) == 0xe0000000)
    {
        /* Via头域中增加maddr参数 */
        /* Via头域中增加ttl参数,ttl=1，主要用于单跳发现用户代理 */
    }
#endif

    /* Via头域中增加send-by字段 */

    /* 增加发送端的IP或者FQDN */
    pstViaPara->eProtocolType = pstPeerLocation->eProtocol;
    pstViaPara->stSendBy.usPort = NULL_USHORT;
    pstViaPara->stSendBy.stHost.eHostType = URI_HOST_DOMAIN;

    pucString = UBUF_AddComponent(pstUbufSipMsg, (ULONG)strlen(g_pucSipTxpSendBy) + 1);
    pstViaPara->stSendBy.stHost.pucAddrStr = pucString;
    memcpy(pucString, g_pucSipTxpSendBy, strlen(g_pucSipTxpSendBy)+1);
    return SUCCESS;
}
