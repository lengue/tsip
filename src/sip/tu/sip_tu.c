/*******************************************************************************
����    : ��������ӹ���(WIN32ר��)
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

#include "include\ua\sip_ua.h"
/* ��ģ������ṩ�ĳ����ͽṹͷ�ļ� */

/* ��ģ���ڲ�ʹ�õĳ����ͽṹͷ�ļ� */

/* ��ģ���ڲ���������ͷ�ļ� */


/* ��ģ��ȫ�ֱ�������ͷ�ļ� */

/* ���ڶ�λ�����Ŀ��λ�� */
#include "sip_tu_var.inc"

ULONG SIP_TU_Init(SIP_TU_CFG_S *pstCfg)
{
    ULONG ulRet;

    g_eSipTuCore = pstCfg->eType;
    switch (g_eSipTuCore)
    {
    	case SIP_CORE_UA:
            ulRet = SIP_UA_Init();
            break;

        default:
            break;
    }

    return SUCCESS;
}

ULONG SIP_TU_RecvUpMsg(ULONG ulCoreID,
                       SIP_LOCATION_S *pstPeerAddr,
                       UBUF_HEADER_S  *pstUbufSipMsg)
{
    ULONG ulRet;

    switch (g_eSipTuCore)
    {
        case SIP_CORE_UA:
            ulRet = SIP_UA_RecvUpMsg(ulCoreID, pstPeerAddr, pstUbufSipMsg);
            break;

        default:
            break;
    }

    return SUCCESS;
}

ULONG SIP_TU_RecvDownMsg(ULONG ulAppRef1,
                         ULONG ulAppRef2,
                         ULONG ulStackRef1,
                         ULONG ulStackRef2,
                         UBUF_HEADER_S  *pstUbufSipMsg)
{
    ULONG ulRet;

    switch (g_eSipTuCore)
    {
        case SIP_CORE_UA:
            ulRet = SIP_UA_RecvDownMsg(ulAppRef1,
                                       ulAppRef2,
                                       ulStackRef1,
                                       ulStackRef2,
                                       pstUbufSipMsg);
            break;

        default:
            break;
    }

    return SUCCESS;
}

