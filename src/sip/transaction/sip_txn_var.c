/*******************************************************************************
����    : SIPЭ��ջ����㹦�ܺ���
������  : �ƴ�ƽ
��������: 2009.01.10
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
#include "..\include\transaction\sip_transaction.h"

/* ��ģ���ڲ�ʹ�õĳ����ͽṹͷ�ļ� */
#include "sip_txn_const.inc"
#include "sip_txn_type.inc"

/* ��ģ���ڲ���������ͷ�ļ� */

/* ��ģ��ȫ�ֱ�������ͷ�ļ� */
#include "sip_txn_var.inc"

/* ���ӿ��ж����Լ����ӿ��ƿ� */
/* ���е�������ƿ� */
SIP_TXN_CB_S  *g_pstSipTxnCB = NULL_PTR;
COMM_QUEUE_S   g_stSipTxnCBQueue;
void          *g_pstSipTxnCBHash     = NULL_PTR;

SIP_TXN_FSM_S g_astSipTxnFsm[SIP_TXN_TYPE_BUTT][SIP_TXN_STATE_TERMINATED][SIP_TXN_EVENT_BUTT];

ULONG g_ulSipTxnT1 = 500;