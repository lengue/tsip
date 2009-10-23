/*******************************************************************************
����    : �������Ϣ����(WIN32ר��)
������  : �ƴ�ƽ
��������: 200.01.10
�޸ļ�¼:
*******************************************************************************/
/* �ⲿ����ģ��ͷ�ļ� */
#include "common\common.h"
#include "abnf\abnf.h"
#include "ubuf\ubuf.h"
#include "sip\uri.h"
#include "sip\sip.h"

#include "..\include\syntax\sip_syntax.h"
#include "..\include\shell\sip_shell.h"
#include "..\include\transaction\sip_transaction.h"
#include "..\include\tu\sip_tu.h"

/* ��ģ������ṩ�ĳ����ͽṹͷ�ļ� */
#include "..\include\transport\sip_transport.h"

/* ��ģ���ڲ�ʹ�õĳ����ͽṹͷ�ļ� */

/* ��ģ���ڲ���������ͷ�ļ� */
#include "sip_transport.inc"

/* ��ģ��ȫ�ֱ�������ͷ�ļ� */
#include "sip_transport_var.inc"

ULONG SIP_Txp_Init(SIP_TXP_CFG_S *pstCfg)
{
    g_pucSipTextMsg = malloc(SIP_MAX_TEXT_MSG_LEN);
    g_pucSipTxpSendBy = malloc(100);
    strcpy(g_pucSipTxpSendBy, pstCfg->pucSendby);
    return SUCCESS;
}

/* �յ��ϲ��������Ϣ
pstSipMsgUbuf ���ڷ��͵���Ϣ
pstPeerLocation ��ϢĿ�Ķ˵�ַ
*/
ULONG SIP_Txp_RecvDownMsg(UBUF_HEADER_S  *pstSipMsgUbuf,
                          SIP_LOCATION_S *pstPeerLocation)
{
    ULONG ulRet;
    SIP_MSG_S *pstSipMsg = NULL_PTR;

    pstSipMsg = UBUF_UBufPtr2Ptr(pstSipMsgUbuf, 0);

    /* �����������Ҫ���Viaͷ�� */
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

/* �յ����������������Ϣ */
ULONG SIP_Txp_RecvUpMsg(SIP_LOCATION_S *pstPeerLocation,
                        UBUF_HEADER_S  *pstUbufSipMsg)
{
    ULONG ulRet;
    SIP_MSG_S     *pstSipMsg = NULL_PTR;

    pstSipMsg = UBUF_UBufPtr2Ptr(pstUbufSipMsg, 0);

    /* ����Ϣ���ʹ��� */
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
    UBUF_PTR   upTempUbufPtr;
    SIP_MSG_S *pstSipMsg = NULL_PTR;
    SIP_HEADER_S *pstHeader = NULL_PTR;
    SIP_HEADER_VIA_S *pstViaHeader = NULL_PTR;
    SIP_VIA_PARM_S *pstViaPara = NULL_PTR;

    pstSipMsg = UBUF_UBufPtr2Ptr(pstUbufSipMsg, 0);

    /* ���sent-by�ֶ� */
    upTempUbufPtr = pstSipMsg->aupstHeaders[SIP_HEADER_VIA];
    pstHeader = UBUF_UBufPtr2Ptr(pstUbufSipMsg, upTempUbufPtr);
    pstViaHeader = (SIP_HEADER_VIA_S *)pstHeader->pstSpec;
    pstViaPara= UBUF_UBufPtr2Ptr(pstUbufSipMsg, pstViaHeader->upstViaParm);
    pucAddr = UBUF_UBufPtr2Ptr(pstUbufSipMsg,
                               pstViaPara->stSendBy.stHost.upucAddrStr);
    if ((pstViaPara->stSendBy.stHost.eHostType == URI_HOST_DOMAIN)
      ||(strncmp(pucAddr,pstPeerLocation->aucIPStr, strlen(pstPeerLocation->aucIPStr))))
    {
        /* ���������������߽��ܵ���IP��ַ��ԭ����ͬ����Ҫ����received���� */
    }

    /* ƥ������������� */
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
    UBUF_PTR   upTempUbufPtr;
    SIP_MSG_S *pstSipMsg = NULL_PTR;
    SIP_HEADER_S *pstHeader = NULL_PTR;
    SIP_HEADER_VIA_S *pstViaHeader = NULL_PTR;
    SIP_VIA_PARM_S *pstViaPara = NULL_PTR;

    pstSipMsg = UBUF_UBufPtr2Ptr(pstUbufSipMsg, 0);

    upTempUbufPtr = pstSipMsg->aupstHeaders[SIP_HEADER_VIA];
    pstHeader = UBUF_UBufPtr2Ptr(pstUbufSipMsg, upTempUbufPtr);
    pstViaHeader = (SIP_HEADER_VIA_S *)pstHeader->pstSpec;
    pstViaPara= UBUF_UBufPtr2Ptr(pstUbufSipMsg, pstViaHeader->upstViaParm);

    /*���send-by�ֶ�*/
    if (ulRet != SUCCESS)
    {
        /* ���ʧ��������Ϣ */
        return FAIL;
    }

    /* ƥ������ */
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


/* �����������Viaͷ��ʹ�ô���㷢������˳��rfc3261 18.1.1 */
ULONG SIP_Txp_AddViaHeaderInRequest(UBUF_HEADER_S *pstUbufSipMsg,
                                    SIP_LOCATION_S *pstPeerLocation)
{
    SIP_MSG_S        *pstSipMsg = NULL_PTR;
    SIP_HEADER_S     *pstHeader = NULL_PTR;
    SIP_HEADER_VIA_S *pstVia    = NULL_PTR;
    SIP_VIA_PARM_S   *pstViaPara =  NULL_PTR;
    UCHAR            *pucString = NULL_PTR;

    pstSipMsg = UBUF_UBufPtr2Ptr(pstUbufSipMsg, 0);

    /* ����Viaͷ�� */
    pstHeader = UBUF_UBufPtr2Ptr(pstUbufSipMsg, pstSipMsg->aupstHeaders[SIP_HEADER_VIA]);
    pstVia = (SIP_HEADER_VIA_S *)pstHeader->pstSpec;
    pstViaPara = UBUF_UBufPtr2Ptr(pstUbufSipMsg,
                                  pstVia->upstViaParm);
#if 0
    /* Ŀ���ַ�Ƕಥ��ַ */
    if ((pstPeerLocation->ulIpAddr&0xe0000000) == 0xe0000000)
    {
        /* Viaͷ��������maddr���� */
        /* Viaͷ��������ttl����,ttl=1����Ҫ���ڵ��������û����� */
    }
#endif

    /* Viaͷ��������send-by�ֶ� */

    /* ���ӷ��Ͷ˵�IP����FQDN */
    pstViaPara->eProtocolType = pstPeerLocation->eProtocol;
    pstViaPara->stSendBy.usPort = NULL_USHORT;
    pstViaPara->stSendBy.stHost.eHostType = URI_HOST_DOMAIN;

    pucString = UBUF_AddComponent(pstUbufSipMsg,
                                  strlen(g_pucSipTxpSendBy) + 1,
                                 &pstViaPara->stSendBy.stHost.upucAddrStr);
    memcpy(pucString, g_pucSipTxpSendBy, strlen(g_pucSipTxpSendBy)+1);
    return SUCCESS;
}
