/*******************************************************************************
����    : SIPЭ��ջ�﷨�����㹦�ܺ���
������  : �ƴ�ƽ
��������: 200.01.10
�޸ļ�¼:
*******************************************************************************/
/* �ⲿ����ģ��ͷ�ļ� */
#include "common\common.h"
#include "abnf\abnf.h"
#include "ubuf\ubuf.h"
#include "uri\uri.h"
#include "syntax_comm\syntax_comm.h"

/* ��ģ������ṩ�ĳ����ͽṹͷ�ļ� */
#include "..\..\include\sip\sip_const.h"
#include "..\..\include\sip\sip_type.h"

#include "..\include\syntax\sip_syntax_const.h"
#include "..\include\syntax\sip_syntax_type.h"

/* ��ģ���ڲ�ʹ�õĳ����ͽṹͷ�ļ� */
#include "sip_syntax_const.inc"
#include "sip_syntax_type.inc"

/* ��ģ���ڲ���������ͷ�ļ� */
#include "sip_syntax_code.inc"
#include "sip_syntax_decode.inc"

/* ��ģ��ȫ�ֱ�������ͷ�ļ� */
#include "sip_syntax_var.inc"

/*SIPЭ��ջ�﷨������ʼ��*/
ULONG SIP_Syntax_Init(SIP_SYNTAX_CFG_S *pstCfg)
{
    ULONG ulLoop;
    ULONG ulRet;

    /* ��ģ��ֻ�ܳ�ʼ��һ�Σ�����һ��ֱ�ӷ���OK */
    if (g_bSipSyntaxInit == TRUE)
    {
        return SUCCESS;
    }
    else
    {
        g_bSipSyntaxInit = TRUE;
    }

    g_pucSipSyntaxBuffer = malloc(SIP_MAX_TEXT_MSG_LEN);

    /* ����SIP��ABNF����� */
    ulRet = ABNF_RegistRuleList(pstCfg->pucAbnfDes, 
                                pstCfg->ulLen, 
                                NULL_PTR, 
                                0, 
                                &g_ucSipRuleListIndex);
    if (ulRet != SUCCESS)
    {
        printf("\r\nABNF_BuildRuleList failed");
        return FAIL;
    }

    /* ����SIPӦ�ù����Ӧ�ù�����Ա�ƥ���ABNF������ */
    for (ulLoop = 0; ulLoop < SIP_ABNF_RULE_BUTT; ulLoop++)
    {
        ulRet = ABNF_GetRuleIndex(g_ucSipRuleListIndex, 
                                  g_astSipAppRuleTbl[ulLoop].aucName, 
                                 &g_astSipAppRuleTbl[ulLoop].ulRuleIndex);
        if (ulRet != SUCCESS)
        {
            return FAIL;
        }
    }

    return SUCCESS;
}

ULONG SIP_Syntax_GetRuleIndex(UCHAR *pucRuleName, ULONG *pulRuleIndex)
{
    ULONG ulLoop;
    ULONG ulRet;

    for (ulLoop = 0; ulLoop < SIP_ABNF_RULE_BUTT; ulLoop++)
    {
        ulRet = strcmp(pucRuleName, g_astSipAppRuleTbl[ulLoop].aucName);
        if (ulRet == SUCCESS)
        {
            *pulRuleIndex = ulLoop;
            return SUCCESS;
        }
    }

    return FAIL;
}

ULONG SIP_Syntax_Code(ULONG  ulRuleIndex,
                      void  *pstStruct,
                      UCHAR *pucBuffer,
                      ULONG  ulBufferLen,
                      ULONG *pulMsgLen)
{
    ULONG ulRet;
    SYNTAX_BUFFER_S stBuffer;

    stBuffer.pucBuffer    = pucBuffer;
    stBuffer.ulBufferLen  = ulBufferLen;
    stBuffer.ulCurrentLen = 0;

    ulRet = SIP_GET_CODE_FUNC(ulRuleIndex)(pstStruct, &stBuffer);
    if (ulRet == SUCCESS)
    {
        *pulMsgLen = stBuffer.ulCurrentLen;
    }

    return ulRet;
}

ULONG SIP_Syntax_Decode(ULONG  ulRuleIndex,
                        UCHAR *pucMsgString,
                        ULONG  ulMsgLen,
                        UBUF_HEADER_S *pstUBuf,
                        void **ppstStruct)
{
    ULONG ulRet;
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;

    if (NULL_PTR == pstUBuf)
    {
        return FAIL;
    }

    ulRet = ABNF_GrammarParse(pucMsgString,
                              ulMsgLen,
                              g_ucSipRuleListIndex,
                              ABNF_GET_RULE_INDEX(g_astSipAppRuleTbl,ulRuleIndex),
                             &pstNode);
    if (ulRet != SUCCESS)
    {
        return FAIL;
    }

    ulRet = SIP_GET_DECODE_FUNC(ulRuleIndex)(pstNode,
                                            pucMsgString,
                                            pstUBuf,
                                            ppstStruct);
    if (ulRet != SUCCESS)
    {
        ABNF_FreeNodeTree(pstNode);
        return FAIL;
    }

    ABNF_FreeNodeTree(pstNode);
    return SUCCESS;
}

/* ��¡���Ǳ����ٽ���*/
ULONG SIP_Syntax_Clone(ULONG          ulRuleIndex,
                       void          *pSrcStruct,
                       UBUF_HEADER_S *pstDstUbuf,
                       void         **ppDstStruct)
{
    return SIP_GET_CLONE_FUNC(ulRuleIndex)(pSrcStruct, pstDstUbuf, ppDstStruct);
}
