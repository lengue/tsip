/**********************************************************************
����    : COMMONͨ�ú���ʵ��
������  : �ƴ�ƽ
��������: 200.01.10
�޸ļ�¼:
**********************************************************************/
/* �ⲿ����ģ��ͷ�ļ� */
#include "common\common.h"
#include "ubuf\ubuf.h"
#include "sip\uri.h"
#include "sip\sip.h"
#include "app\app.h"
#include "timer\timer.h"
#include "sipadapt\sipadapt.h"

/* ��ģ������ṩ�ĳ����ͽṹͷ�ļ� */
#include "system\system.h"

/* ��ģ���ڲ�ʹ�õĳ����ͽṹͷ�ļ� */

/* ��ģ���ڲ���������ͷ�ļ� */

/* ��ģ��ȫ�ֱ�������ͷ�ļ� */
#include "system_var.inc"

SYS_MODULE_CB_S   *g_pstMsgCB = NULL_PTR;
SYS_MODULE_INFO_S g_astModuleCfg[SYS_MODULE_BUTT] =
{
    {APP_InitProc,          APP_MsgProc},
    {TIMER_Init,            NULL_PTR},
    {SIP_ADPT_Init,         SIP_APDT_MsgProc}
};