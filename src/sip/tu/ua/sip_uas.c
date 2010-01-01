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

/* �ײ�ʹ�øú����ϱ����� rfc3261 8.2.5 */
ULONG SIP_UAS_ProcessingRequest(ULONG ulCoreID,
                                SIP_LOCATION_S *pstPeerAddr,
                                UBUF_HEADER_S  *pstUbufSipMsg)
{
    ULONG ulDlgID = NULL_ULONG;
    ULONG ulRet;
    ULONG ulUasID;
    ULONG ulTxnID;
    ULONG ulAppRef1 = NULL_ULONG;
    ULONG ulAppRef2 = NULL_ULONG;
    SIP_MSG_S *pstSipMsg = NULL_PTR;
    SIP_HEADER_TO_S *pstHeaderTo = NULL_PTR;

    ulUasID = ulCoreID;
    
    /* �������� */
    if (ulUasID == NULL_ULONG)
    {
        /* ��ʼ������ */
        SIP_UAS_AllocCB(&ulUasID);
        SIP_Txn_AllocTxn(ulUasID, &ulTxnID);
        g_pstSipUasCB[ulUasID].ulTxnID = ulTxnID;
        g_pstSipUasCB[ulUasID].pstSipMsgUbuf = pstUbufSipMsg;

        /* ����Ϣ�·���TXN�� */
        SIP_Txn_RecvUpMsg(ulTxnID, pstUbufSipMsg, pstPeerAddr);
        return SUCCESS;
    }

    pstSipMsg = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUbufSipMsg);
    pstHeaderTo = (SIP_HEADER_TO_S *)pstSipMsg->apstHeaders[SIP_HEADER_TO];
    if (pstHeaderTo->pucTag != NULL_PTR)
    {
        /* ƥ��Ի� */
    }

    if (ulDlgID == NULL_ULONG)
    {
        /* ��֤ */
        
        /* �Ի������� */
        ulRet = SIP_UAS_MethodInspection(pstUbufSipMsg);
        if(ulRet != SUCCESS)
        {
            /* ����405��Ӧ������Allowͷ�� */
        }

        /* ͷ����*/
        ulRet = SIP_UAS_HeaderInspection(pstUbufSipMsg);
        if(ulRet != SUCCESS)
        {
            /* ͷ����ȷ�������� */
        }


        /* ���ݼ�� */
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
    SIP_SendUpMsg(0, ulUasID, ulAppRef1, ulAppRef2, pstUbufSipMsg);
    return SUCCESS;
}

/*
Ӧ�ò㷢����Ӧ��Ϣ
ULONG ulTxnID ����ID������ƥ������
ULONG *pulDlgID  �����Ϊ�գ���ʶ�������ߴ���һ���Ի���
UBUF_HEADER_S * pstUbufSipMsg �����͵���Ϣ
*/
ULONG SIP_UAS_SendResponse(IN  ULONG ulUasID,
                           OUT ULONG ulDlgID,
                           UBUF_HEADER_S * pstUbufSipMsg)
{
    /* ������ͷ�� */
    SIP_UAS_GenerateResponse(ulUasID, pstUbufSipMsg);

    /* ��TXN������Ϣ */
    SIP_Txn_RecvDownMsg(g_pstSipUasCB[ulUasID].ulTxnID,
                        pstUbufSipMsg,
                        NULL_PTR);

    return SUCCESS;
}

/*rfc3261 8.2.1*/
ULONG SIP_UAS_MethodInspection(UBUF_HEADER_S *pstSipMsgUbuf)
{
    SIP_MSG_S     *pstSipMsg = NULL_PTR;

    pstSipMsg = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstSipMsgUbuf);
    if (pstSipMsg->uStartLine.stRequstLine.eMethod >= SIP_METHOD_BUTT)
    {
        return FAIL;
    }
    
    return SUCCESS;
}

/*rfc3261 8.2.2*/
ULONG SIP_UAS_HeaderInspection(UBUF_HEADER_S *pstSipMsgUbuf)
{
    /* ���Toͷ�� */

    /* ���Request URIͷ�� */

    /* ���Merged����*/

    /* Requireͷ���� */
    
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
    SIP_MSG_S *pstUbufRequestMsg = NULL_PTR;
    SIP_MSG_S *pstUbufResponseMsg = NULL_PTR;
    SIP_HEADER_TO_S *pstHeaderTo = NULL_PTR;
    ULONG ulRuleIndex;
    
    pstUbufRequestMsg  = (SIP_MSG_S *)UBUF_GET_MSG_PTR(g_pstSipUasCB[ulUasID].pstSipMsgUbuf);
    pstUbufResponseMsg = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUbufSipMsg);
        
    /* ��¡Fromͷ�� */
    SIP_GetRuleIndex("From", &ulRuleIndex);
    SIP_Clone(ulRuleIndex,
              pstUbufRequestMsg->apstHeaders[SIP_HEADER_FROM], 
              pstUbufSipMsg, 
             &pstUbufResponseMsg->apstHeaders[SIP_HEADER_FROM]);
    
    /* ��¡Call-IDͷ�� */
    SIP_GetRuleIndex("Call-ID", &ulRuleIndex);
    SIP_Clone(ulRuleIndex,
              pstUbufRequestMsg->apstHeaders[SIP_HEADER_CALL_ID], 
              pstUbufSipMsg, 
             &pstUbufResponseMsg->apstHeaders[SIP_HEADER_CALL_ID]);

    /* ��¡CSeqͷ�� */
    SIP_GetRuleIndex("CSeq", &ulRuleIndex);
    SIP_Clone(ulRuleIndex,
              pstUbufRequestMsg->apstHeaders[SIP_HEADER_CSEQ], 
              pstUbufSipMsg, 
             &pstUbufResponseMsg->apstHeaders[SIP_HEADER_CSEQ]);
    
    /* ��¡Viaͷ�� */
    SIP_GetRuleIndex("Via", &ulRuleIndex);
    SIP_Clone(ulRuleIndex,
              pstUbufRequestMsg->apstHeaders[SIP_HEADER_VIA], 
              pstUbufSipMsg, 
             &pstUbufResponseMsg->apstHeaders[SIP_HEADER_VIA]);

    
    /* ��¡Toͷ�� */
    SIP_GetRuleIndex("To", &ulRuleIndex);
    SIP_Clone(ulRuleIndex,
              pstUbufRequestMsg->apstHeaders[SIP_HEADER_TO], 
              pstUbufSipMsg, 
             &pstUbufResponseMsg->apstHeaders[SIP_HEADER_TO]);


    /* ���Toͷ��û��tag�����һ��tag */
    pstHeaderTo = pstUbufResponseMsg->apstHeaders[SIP_HEADER_TO];
    if (pstHeaderTo->pucTag == NULL_PTR)
    {
        
    }

    return SUCCESS;
}
