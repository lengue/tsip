/*******************************************************************************
����    : �����ȫ�ֱ�������
������  : �ƴ�ƽ
��������: 200.01.10
�޸ļ�¼:
*******************************************************************************/
/* �ⲿ����ģ��ͷ�ļ� */
#include "common\common.h"
#include "ubuf\ubuf.h"
#include "sip\uri.h"
#include "sip\sip.h"

/* ��ģ������ṩ�ĳ����ͽṹͷ�ļ� */
#include "connect\connect.h"

/* ��ģ���ڲ�ʹ�õĳ����ͽṹͷ�ļ� */
#include "connect_const.inc"
#include "connect_type.inc"

/* ��ģ���ڲ���������ͷ�ļ� */
/* ��ģ��ȫ�ֱ�������ͷ�ļ� */


/* ������������: �������ã�����ΪIP���IP�����򲻰�IP����дViaͷ�� */
//UCHAR *g_pucSipTxpHost;

/* �����������մ���Э�����У���¼��Э�������Ӷ��� */
ULONG                 g_aulConnTransport[SIP_TRANSPORT_PROTOCOL_BUTT];

/* ������Ӷ��� */
CONN_LISTEN_S     *g_pstConnListen;
CONN_FUNC_MSG_PROC g_pfnConnMsgProc = NULL_PTR;

/* ���ӿ��ж����Լ����ӿ��ƿ� */
COMM_QUEUE_S           g_stConnCBQueue;
CONN_CB_S            *g_pstConnCB;