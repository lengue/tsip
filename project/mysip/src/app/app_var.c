/*******************************************************************************
����    : Ӧ��ģ�����
������  : �ƴ�ƽ
��������: 2009.07.19
�޸ļ�¼:
*******************************************************************************/
/* �ⲿ����ģ��ͷ�ļ� */
#include "common\common.h"
#include "ubuf\ubuf.h"
#include "sip\uri.h"
#include "sip\sip.h"

/* ��ģ������ṩ�ĳ����ͽṹͷ�ļ� */
#include "app\app.h"

/* ��ģ���ڲ�ʹ�õĳ����ͽṹͷ�ļ� */
#include "app_const.inc"
#include "app_type.inc"

/* ��ģ���ڲ���������ͷ�ļ� */

/* ��ģ��ȫ�ֱ�������ͷ�ļ� */
#include "app_var.inc"

APP_FN_FSM_PROC g_apfnAppFsm[APP_STATE_BUTT][APP_EVENT_BUTT];
APP_STATE_E g_eAppState;
UCHAR g_ucRemoteAlertFlag = FALSE;
UCHAR *g_pucAppPublicID = "sip:tangchunping@huawei.com";
ULONG g_ulStackDlgID;
ULONG g_ulStackTxnID;