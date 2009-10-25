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

/* �ײ�ʹ�øú����ϱ����� rfc3261 8.2.5
*/
ULONG SIP_UAS_ProcessingRequest(ULONG ulTxnID,
                                SIP_LOCATION_S *pstPeerAddr,
                                UBUF_HEADER_S  *pstUbufSipMsg)
{
    ULONG ulDlgID = NULL_ULONG;
    ULONG ulRet;
    ULONG ulUasID;

    /* �������� */
    if (ulTxnID == NULL_ULONG)
    {
        SIP_Txn_AllocTxn(pstPeerAddr, &ulTxnID);
        SIP_Txn_RecvUpMsg(ulTxnID, pstUbufSipMsg);
        return SUCCESS;
    }


    /* ƥ��Ի� */

    SIP_UAS_AllocCB(&ulUasID);
    g_pstSipUasCB[ulUasID].ulTxnID       = ulTxnID;
    g_pstSipUasCB[ulUasID].pstSipMsgUbuf = pstUbufSipMsg;
    if(ulDlgID == NULL_ULONG)
    {
        /* �Ի������� */
        ulRet = SIP_UAS_MethodInspection(pstUbufSipMsg);
        if(ulRet != SUCCESS)
        {
            /* ����405��Ӧ������Allowͷ�� */
        }

        ulRet = SIP_UAS_HeaderInspection(pstUbufSipMsg);
        if(ulRet != SUCCESS)
        {
            /*  */
        }

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
    SIP_SendUpRequestMsg(ulDlgID, ulUasID, pstUbufSipMsg);
    return SUCCESS;
}

/*
Ӧ�ò㷢����Ӧ��Ϣ
ULONG ulTxnID ����ID������ƥ������
ULONG *pulDlgID  �����Ϊ�գ���ʶ�������ߴ���һ���Ի���
UBUF_HEADER_S * pstUbufSipMsg �����͵���Ϣ
*/
ULONG SIP_UAS_SendResponse(IN  ULONG ulUasID,
                           OUT ULONG *pulDlgID,
                           UBUF_HEADER_S * pstUbufSipMsg)
{
    /* ������ͷ�� */
    SIP_UAS_GenerateResponse(ulUasID, pstUbufSipMsg);

    /* ��TXN������Ϣ */
    SIP_Txn_RecvUpMsg(g_pstSipUasCB[ulUasID].ulTxnID, pstUbufSipMsg);

    return SUCCESS;
}

/*rfc3261 8.2.1*/
ULONG SIP_UAS_MethodInspection(UBUF_HEADER_S *pstSipMsgUbuf)
{
    return SUCCESS;
}

/*rfc3261 8.2.2*/
ULONG SIP_UAS_HeaderInspection(UBUF_HEADER_S *pstSipMsgUbuf)
{
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

ULONG SIP_UAS_GenerateResponse(ULONG ulUasID, UBUF_HEADER_S * pstUbufSipMsg)
{
    UBUF_HEADER_S * pstUbufRequestMsg = NULL_PTR;

    pstUbufRequestMsg = g_pstSipUasCB[ulUasID].pstSipMsgUbuf;

    /* ��¡Fromͷ�� */
    /* ��¡Call-IDͷ�� */
    /* ��¡CSeqͷ�� */
    /* ��¡Viaͷ�� */
    /* ��¡Toͷ�� */
    /* ���Toͷ��û��tag�����һ��tag */

}