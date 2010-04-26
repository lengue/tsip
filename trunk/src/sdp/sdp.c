/*******************************************************************************
����    : SDPЭ��ջ�﷨�����㹦�ܺ���
������  : �ƴ�ƽ
��������: 200.01.10
�޸ļ�¼:
*******************************************************************************/
/* �ⲿ����ģ��ͷ�ļ� */
#include "common\common.h"
#include "abnf\abnf.h"
#include "ubuf\ubuf.h"

/* ��ģ������ṩ�ĳ����ͽṹͷ�ļ� */
#include "..\..\include\sdp\sdp.h"

/* ��ģ���ڲ�ʹ�õĳ����ͽṹͷ�ļ� */
#include "sdp_const.inc"
#include "sdp_type.inc"

/* ��ģ���ڲ���������ͷ�ļ� */
#include "sdp.inc"

/* ��ģ��ȫ�ֱ�������ͷ�ļ� */
#include "sip_var.inc"


/*SDPЭ��ջ��ʼ��*/
ULONG SDP_Init()
{
    ULONG ulLoop;
    ULONG ulLoop2;
    ULONG ulRet;

    /* ��ģ��ֻ�ܳ�ʼ��һ�Σ�����һ��ֱ�ӷ���OK */
    if (g_bSdpInit == TRUE)
    {
        return SUCCESS;
    }
    else
    {
        g_bSdpInit = TRUE;
    }

    g_pucSdpBuffer = malloc(SDP_MAX_TEXT_MSG_LEN);

    /* ��ʼ������ģ�� */
    ulRet = ABNF_Init();
    if (ulRet != SUCCESS)
    {
        printf("\r\nABNF_Init failed");
        return FAIL;
    }

    /* ����SIP��ABNF����� */
    ulRet = ABNF_BuildRuleList(pstCfg->pucAbnfDes, pstCfg->ulLen, &g_pstSdpRuleList);
    if (ulRet != SUCCESS)
    {
        printf("\r\nABNF_BuildRuleList failed");
        return FAIL;
    }

    /* ����SIPӦ�ù����Ӧ�ù�����Ա�ƥ���ABNF������ */
    for (ulLoop = 0; ulLoop < SIP_ABNF_RULE_BUTT; ulLoop++)
    {
        for (ulLoop2 = 0; ulLoop2 < g_pstSipRuleList->ulRuleNum; ulLoop2++)
        {
            ulRet = strcmp(g_astSipAppRuleTbl[ulLoop].aucName,
                           g_pstSipRuleList->pstRules[ulLoop2].aucName);
            if (ulRet == SUCCESS)
            {
                g_pstSipRuleList->pstRules[ulLoop2].ulAppRuleIndex = ulLoop;
                g_astSipAppRuleTbl[ulLoop].ulRuleIndex = ulLoop2;
                break;
            }
        }

        if (ulLoop2 >= g_pstSipRuleList->ulRuleNum)
        {
            return FAIL;
        }
    }

    return SUCCESS;
}


/* SDP���� */
ULONG SDP_Code(void  *pstStruct,
               UCHAR *pucBuffer,
               ULONG  ulBufferLen,
               ULONG *pulMsgLen)
{
}

/* SDP���� */
ULONG SDP_Decode(UCHAR *pucMsgString,
                 ULONG  ulMsgLen,
                 UBUF_HEADER_S *pstUBuf,
                 void **ppstStruct)
{
}

