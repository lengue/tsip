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
#include "s_hash\s_hash.h"

/* ��ģ������ṩ�ĳ����ͽṹͷ�ļ� */
#include "..\include\dialog\sip_dlg.h"

/* ��ģ���ڲ�ʹ�õĳ����ͽṹͷ�ļ� */
#include "sip_dlg_const.inc"
#include "sip_dlg_type.inc"

/* ��ģ���ڲ���������ͷ�ļ� */

/* ��ģ��ȫ�ֱ�������ͷ�ļ� */
#include "sip_dlg_var.inc"

SIP_DLG_CB_S *g_pstSipDlgCB   = NULL_PTR;
COMM_QUEUE_S  g_stSipDlgCBQueue;
void         *g_pstSipDlgHash = NULL_PTR;

