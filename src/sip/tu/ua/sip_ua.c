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

/* ��ģ������ṩ�ĳ����ͽṹͷ�ļ� */


/* ��ģ���ڲ�ʹ�õĳ����ͽṹͷ�ļ� */


/* ��ģ���ڲ���������ͷ�ļ� */
#include "sip_uac.inc"
#include "sip_uas.inc"

/* ��ģ��ȫ�ֱ�������ͷ�ļ� */
ULONG SIP_UA_Init()
{
    ULONG ulRet;

    ulRet  = SIP_UAC_Init();
    ulRet |= SIP_UAS_Init();

    return ulRet;
}

/* ��ģ��ȫ�ֱ�������ͷ�ļ� */
ULONG SIP_UA_RecvUpMsg(ULONG ulCoreID,
                       SIP_LOCATION_S *pstPeerAddr,
                       UBUF_HEADER_S *pstUbufSipMsg)
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
            ulRet = SIP_UAC_ProcessingResponse(ulCoreID, pstUbufSipMsg);
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
                         ULONG ulStackRef1,
                         ULONG ulStackRef2,
                         UBUF_HEADER_S *pstUbufSipMsg)
{
    SIP_MSG_S *pstSipMsg = NULL_PTR;
    ULONG ulRet;

    pstSipMsg = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUbufSipMsg);
    switch (pstSipMsg->eMsgType)
    {
        case SIP_MSG_TYPE_REQUEST:
            ulRet = SIP_UAC_SendRequest(ulAppRef2, ulStackRef1, pstUbufSipMsg);
            break;

        case SIP_MSG_TYPE_RESPONSE:
            ulRet = SIP_UAS_SendResponse(ulStackRef2, ulStackRef1, pstUbufSipMsg);
            break;

        default:
            ulRet = FAIL;
            break;
    }

    return ulRet;
}
