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

/* ��ģ������ṩ�ĳ����ͽṹͷ�ļ� */
#include "sip_ua_const.inc"
#include "sip_ua_type.inc"

/* ��ģ���ڲ�ʹ�õĳ����ͽṹͷ�ļ� */


/* ��ģ���ڲ���������ͷ�ļ� */
#include "sip_uac.inc"
#include "sip_uas.inc"

/* ��ģ��ȫ�ֱ�������ͷ�ļ� */
SIP_UAC_CB_S   *g_pstSipUacCB = NULL_PTR;
COMM_QUEUE_S   g_stSipUacCBQueue;
SIP_UAS_CB_S   *g_pstSipUasCB = NULL_PTR;
COMM_QUEUE_S   g_stSipUasCBQueue;