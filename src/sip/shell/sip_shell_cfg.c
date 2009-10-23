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

ULONG SIP_Config(SIP_CFG_S *pstCfg)
{
    ULONG ulRet;

    /* ����ȫ�ֲ� */
    memcpy(&g_stSiphellCfg, &pstCfg->stShellCfg, sizeof(SIP_SHELL_CFG_S));

    /* ����TU�� */
    ulRet = SIP_TU_Init(&pstCfg->stTuCfg);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }

    /* ����TXN�� */
    ulRet = SIP_Txn_Init(&pstCfg->stTxnCfg);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }

    /* ����TXP�� */
    ulRet = SIP_Txp_Init(&pstCfg->stTxpCfg);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }

    /* ����SYNTAX */
    ulRet = SIP_Syntax_Init(&pstCfg->stSyntaxCfg);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }

    return SUCCESS;
}