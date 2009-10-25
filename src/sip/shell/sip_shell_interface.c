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

ULONG SIP_TimeoutProc(ULONG ulName, ULONG ulPara)
{
    /* �������ַַ���ʱ�� */
    SIP_Txn_TimerProc(ulPara, ulName);

    return SUCCESS;
}


ULONG SIP_GetRuleIndex(UCHAR *pucRuleName, ULONG *pulRuleIndex)
{
    return SIP_Syntax_GetRuleIndex(pucRuleName, pulRuleIndex);
}