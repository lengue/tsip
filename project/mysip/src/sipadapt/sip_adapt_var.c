/*******************************************************************************
����    : SIPЭ��ջ����㹦�ܺ���
������  : �ƴ�ƽ
��������: 2009.01.10
�޸ļ�¼:
*******************************************************************************/
/* �ⲿ����ģ��ͷ�ļ� */
#include "common\common.h"
#include "ubuf\ubuf.h"
#include "uri\uri.h"
#include "sip\sip.h"
#include "system\system.h"
#include "connect\connect.h"
#include "timer\timer.h"

/* ��ģ������ṩ�ĳ����ͽṹͷ�ļ� */

/* ��ģ���ڲ�ʹ�õĳ����ͽṹͷ�ļ� */
#include "sip_adapt_const.inc"
#include "sip_adapt_type.inc"

/* ��ģ���ڲ���������ͷ�ļ� */
#include "sip_adapt.inc"

/* ��ģ��ȫ�ֱ�������ͷ�ļ� */
#include "sip_adapt_var.inc"

FUNC_SIP_ADAPT_PARSE g_apfnSipAdaptCfgParse[]=
{
    SIP_ADPT_BuildTuConfig,
    SIP_ADPT_BuildTransactionConfig,
    SIP_ADPT_BuildTransportConfig,
    SIP_ADPT_BuildSyntaxtConfig,
    SIP_ADPT_BuildConnConfig
};

FUNC_SIP_ADAPT_FREE g_apfnSipAdaptCfgFree[]=
{
    SIP_ADPT_FreeTuConfig,
    SIP_ADPT_FreeTransactionConfig,
    SIP_ADPT_FreeTransportConfig,
    SIP_ADPT_FreeSyntaxConfig,
    SIP_ADPT_FreeConnConfig
};

UCHAR *g_pucSipAdptBuffer;
