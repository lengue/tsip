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

#include "..\..\include\syntax\sip_syntax.h"
#include "..\..\include\transport\sip_transport.h"
#include "..\..\include\transaction\sip_transaction.h"
#include "..\..\include\tu\sip_tu.h"

#include "..\include\locate\sip_locate.h"
#include "..\include\dialog\sip_dlg.h"

/* ��ģ������ṩ�ĳ����ͽṹͷ�ļ� */

/* ��ģ���ڲ�ʹ�õĳ����ͽṹͷ�ļ� */
#include "sip_ua_const.inc"
#include "sip_ua_type.inc"

/* ��ģ���ڲ���������ͷ�ļ� */
#include "sip_uac.inc"
#include "sip_uas.inc"

/* ��ģ��ȫ�ֱ�������ͷ�ļ� */
#include "sip_ua_var.inc"

ULONG SIP_UA_Init(UCHAR *pucContact)
{
    ULONG ulRet;

    g_pucSipUaContact = malloc(100);
    strcpy(g_pucSipUaContact, pucContact);

    ulRet  = SIP_UAC_Init();
    ulRet |= SIP_UAS_Init();
    
    ulRet |= SIP_Dlg_Init();

    return ulRet;
}

/* ��ģ��ȫ�ֱ�������ͷ�ļ� */
ULONG SIP_UA_RecvUpMsg(ULONG ulCoreID,
                       ULONG ulTxnID,
                       SIP_LOCATION_S *pstPeerAddr,
                       UBUF_HEADER_S  *pstUbufSipMsg)
{
    SIP_MSG_S *pstSipMsg = NULL_PTR;
    ULONG ulRet;

    pstSipMsg = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUbufSipMsg);
    switch (pstSipMsg->eMsgType)
    {
        case SIP_MSG_TYPE_REQUEST:
            ulRet = SIP_UAS_ProcessingRequest(ulCoreID, pstPeerAddr, pstUbufSipMsg);
            break;

        case SIP_MSG_TYPE_RESPONSE:
            ulRet = SIP_UAC_ProcessingResponse(ulCoreID, ulTxnID, pstUbufSipMsg);
            break;

        default:
            ulRet = FAIL;
            break;
    }

    return ulRet;
}

/* ��ģ��ȫ�ֱ�������ͷ�ļ� */
ULONG SIP_UA_RecvDownMsg(ULONG ulAppRef1,
                         ULONG ulAppRef2,
                         ULONG *pulStackRef1,
                         ULONG *pulStackRef2,
                         UBUF_HEADER_S *pstUbufSipMsg)
{
    SIP_MSG_S *pstSipMsg = NULL_PTR;
    ULONG ulRet;

    pstSipMsg = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUbufSipMsg);
    switch (pstSipMsg->eMsgType)
    {
        case SIP_MSG_TYPE_REQUEST:
            ulRet = SIP_UAC_SendRequest(ulAppRef1, ulAppRef2, *pulStackRef1, pulStackRef2, pstUbufSipMsg);
            break;

        case SIP_MSG_TYPE_RESPONSE:
            ulRet = SIP_UAS_SendResponse(ulAppRef1, ulAppRef2, pulStackRef1, *pulStackRef2, pstUbufSipMsg);
            break;

        default:
            ulRet = FAIL;
            break;
    }

    return ulRet;
}


/* ӳ��Э��ջ��Ӧ�õĲο���1��������Ӧ�ò㴦��������
����Ӧ�����Ի�ʱ���� */
ULONG SIP_UA_MapRef1(ULONG ulStackRef1, ULONG ulAppRef1)
{
    return SIP_Dlg_IDMap(ulStackRef1, ulAppRef1);
}

/* ӳ��Э��ջ��Ӧ�õĲο���2��������Ӧ�ò㴦��������
������ʱ���� */
ULONG SIP_UA_MapRef2(ULONG ulStackRef2, ULONG ulAppRef2)
{
    return SIP_UAS_IDMap(ulStackRef2, ulAppRef2);
}

