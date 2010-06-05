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
ULONG SDP_Init(SDP_CFG_S *pstCfg)
{
    ULONG ulLoop;
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
    ulRet = ABNF_RegistRuleList(pstCfg->pucAbnfDes, pstCfg->ulLen, &g_ucSdpRuleListIndex);
    if (ulRet != SUCCESS)
    {
        printf("\r\nABNF_BuildRuleList failed");
        return FAIL;
    }

    /* ����SIPӦ�ù����Ӧ�ù�����Ա�ƥ���ABNF������ */
    for (ulLoop = 0; ulLoop < SIP_ABNF_RULE_BUTT; ulLoop++)
    {
        ulRet = ABNF_GetRuleIndex(g_ucSdpRuleListIndex, 
                                  g_astSdpAppRuleTbl[ulLoop].aucName, 
                                 &g_astSdpAppRuleTbl[ulLoop].ulRuleIndex);
        if (ulRet != SUCCESS)
        {
            return FAIL;
        }
    }

    return SUCCESS;
}

/* SDP���� */
ULONG SDP_Code(ULONG  ulRuleIndex,
               void  *pstStruct,
               UCHAR *pucBuffer,
               ULONG  ulBufferLen,
               ULONG *pulMsgLen)
{
    ULONG ulRet;
    SIP_SYNTAX_BUFFER_S stBuffer;

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

/* SDP���� */
ULONG SDP_Decode(ULONG  ulRuleIndex,
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
                              g_ucSdpRuleListIndex,
                              ABNF_GET_RULE_INDEX(g_astSipAppRuleTbl,ulRuleIndex),
                             &pstNode);
    if (ulRet != SUCCESS)
    {
        return FAIL;
    }

    ulRet = SIP_GET_PARSE_FUNC(ulRuleIndex)(pstNode,
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
ULONG SDP_Clone(ULONG          ulRuleIndex,
                void          *pSrcStruct,
                UBUF_HEADER_S *pstDstUbuf,
                void         **ppDstStruct)
{
    return SIP_GET_CLONE_FUNC(ulRuleIndex)(pSrcStruct, pstDstUbuf, ppDstStruct);
}

