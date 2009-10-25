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

#include "..\include\syntax\sip_syntax.h"
#include "..\include\transport\sip_transport.h"
#include "..\include\transaction\sip_transaction.h"
#include "..\include\tu\sip_tu.h"
#include "..\include\shell\sip_shell.h"

/* ��ģ������ṩ�ĳ����ͽṹͷ�ļ� */


/* ��ģ���ڲ�ʹ�õĳ����ͽṹͷ�ļ� */
#include "sip_shell_const.inc"
#include "sip_shell_type.inc"

/* ��ģ���ڲ���������ͷ�ļ� */
#include "sip_shell_cfg.inc"

/* ��ģ��ȫ�ֱ�������ͷ�ļ� */
#include "sip_shell_var.inc"

/* SIPЭ��ջ���յ����Դ�����������Ϣ */
ULONG SIP_SendUpRequestMsg(ULONG ulDlgID,
                           ULONG ulUasID,
                           UBUF_HEADER_S * pstUbufSipMsg)
{
    /* ����ע��Ĺ��Ӻ��� */
    if (g_stSiphellCfg.pfnSendUpRequestMsg == NULL_PTR)
    {
        return FAIL;
    }

    return g_stSiphellCfg.pfnSendUpRequestMsg(ulDlgID,
                                              ulUasID,
                                              pstUbufSipMsg);
}

ULONG SIP_SendUpResponseMsg(ULONG ulAppID,
                            ULONG ulDlgID,
                            UBUF_HEADER_S * pstUbufSipMsg)
{
    /* ����ע��Ĺ��Ӻ��� */
    if (g_stSiphellCfg.pfnSendUpResponseMsg == NULL_PTR)
    {
        return FAIL;
    }

    return g_stSiphellCfg.pfnSendUpResponseMsg(ulAppID,
                                               ulDlgID,
                                               pstUbufSipMsg);
}

/* ����Э��ջ����SIP��Ϣ�ӿ� */
ULONG SIP_SendDownMsg(UBUF_HEADER_S  *pstSipMsgUbuf,
                      SIP_LOCATION_S *pstPeerLocation)
{
    /* ����ע��Ĺ��Ӻ��� */
    if (g_stSiphellCfg.pfnSendDownMsg == NULL_PTR)
    {
        return FAIL;
    }

    return g_stSiphellCfg.pfnSendDownMsg(pstSipMsgUbuf,
                                         pstPeerLocation);
}

ULONG SIP_StartTimer(ULONG ulName,
                     ULONG ulLen,
                     ULONG ulPara,
                     SIP_TIMER_TYPE_E eType,
                     ULONG *pulHandle)
{
    /* ����ע��Ĺ��Ӻ��� */
    if (g_stSiphellCfg.pfnStartTimer == NULL_PTR)
    {
        return FAIL;
    }

    return g_stSiphellCfg.pfnStartTimer(ulName,
                                        ulLen,
                                        ulPara,
                                        eType,
                                        pulHandle);
}

ULONG SIP_StopTimer(ULONG ulHandle)
{
    /* ����ע��Ĺ��Ӻ��� */
    if (g_stSiphellCfg.pfnStopTimer == NULL_PTR)
    {
        return FAIL;
    }

    g_stSiphellCfg.pfnStopTimer(ulHandle);
	return SUCCESS;
}

ULONG SIP_GenerateRandomString(CHAR *pcBuffer, ULONG ulBufferLen)
{
    /* ����ע��Ĺ��Ӻ��� */
    if (g_stSiphellCfg.pfnGenerateRandom == NULL_PTR)
    {
        return FAIL;
    }

    g_stSiphellCfg.pfnGenerateRandom(pcBuffer, ulBufferLen);
	return SUCCESS;
}

