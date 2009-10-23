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
ULONG SIP_UA_RecvUpMsg(ULONG ulTxnID,
                       SIP_LOCATION_S *pstPeerAddr,
                       UBUF_HEADER_S *pstUbufSipMsg)
{
    SIP_MSG_S *pstSipMsg = NULL_PTR;
    ULONG ulRet;

    pstSipMsg = UBUF_UBufPtr2Ptr(pstUbufSipMsg, 0);
    switch (pstSipMsg->eMsgType)
    {
        case SIP_MSG_TYPE_REQUEST:
            ulRet = SIP_UAS_ProcessingRequest(ulTxnID, pstPeerAddr, pstUbufSipMsg);
            break;

        case SIP_MSG_TYPE_RESPONSE:
            //ulRet = SIP_UAC_ProcessingResponse(ulTxnID, pstUbufSipMsg);
            break;

        default:
            ulRet = FAIL;
            break;
    }

    return ulRet;
}

