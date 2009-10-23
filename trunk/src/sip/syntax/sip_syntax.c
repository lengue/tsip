/*******************************************************************************
功能    : SIP协议栈语法与编码层功能函数
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/
/* 外部依赖模块头文件 */
#include "common\common.h"
#include "abnf\abnf.h"
#include "ubuf\ubuf.h"
#include "sip\uri.h"

/* 本模块对外提供的常量和结构头文件 */
#include "..\..\include\sip\sip_const.h"
#include "..\..\include\sip\sip_type.h"

#include "..\include\syntax\sip_syntax_const.h"
#include "..\include\syntax\sip_syntax_type.h"

/* 本模块内部使用的常量和结构头文件 */
#include "sip_syntax_const.inc"
#include "sip_syntax_type.inc"

/* 本模块内部函数声明头文件 */
#include "sip_syntax_code.inc"
#include "sip_syntax_decode.inc"

/* 本模块全局变量声明头文件 */
#include "sip_syntax_var.inc"

/*SIP协议栈语法编码层初始化*/
ULONG SIP_Syntax_Init(SIP_SYNTAX_CFG_S *pstCfg)
{
    ULONG ulLoop;
    ULONG ulLoop2;
    ULONG ulRet;

    /* 本模块只能初始化一次，后续一律直接返回OK */
    if (g_bSipSyntaxInit == TRUE)
    {
        return SUCCESS;
    }
    else
    {
        g_bSipSyntaxInit = TRUE;
    }

    /* 初始化依赖模块 */
    ulRet = ABNF_Init();
    if (ulRet != SUCCESS)
    {
        printf("\r\nABNF_Init failed");
        return FAIL;
    }

    /* 构建SIP的ABNF规则表 */
    ulRet = ABNF_BuildRuleList(pstCfg->pucAbnfDes, pstCfg->ulLen, &g_pstSipRuleList);
    if (ulRet != SUCCESS)
    {
        printf("\r\nABNF_BuildRuleList failed");
        return FAIL;
    }

    /* 构建SIP应用规则表，应用规则可以比匹配的ABNF规则少 */
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

/* SIP消息编码 */
ULONG SIP_Syntax_SipCode(UBUF_HEADER_S *pstUBuf,
                         UBUF_PTR upstSipMsg,
                         UCHAR *pucBuffer,
                         ULONG  ulBufferLen,
                         ULONG *pulMsgLen)
{
    ULONG ulRet;
    SIP_SYNTAX_BUFFER_S stBuffer;
    SIP_MSG_S *pstSipMsg = NULL_PTR;

    stBuffer.pucBuffer = pucBuffer;
    stBuffer.ulBufferLen = ulBufferLen;
    stBuffer.ulCurrentLen = 0;

    pstSipMsg = UBUF_UBufPtr2Ptr(pstUBuf, upstSipMsg);
    ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_SIP_MESSAGE)(pstUBuf,
                                                         pstSipMsg,
                                                        &stBuffer);
    if (ulRet == SUCCESS)
    {
        *pulMsgLen = stBuffer.ulCurrentLen;
    }

    return ulRet;
}

/* URI编码 */
ULONG SIP_Syntax_UriCode(UBUF_HEADER_S *pstUBuf,
                         UBUF_PTR upstUri,
                         UCHAR *pucBuffer,
                         ULONG  ulBufferLen,
                         ULONG *pulMsgLen)
{
    ULONG ulRet;
    SIP_SYNTAX_BUFFER_S stBuffer;
    URI_S *pstAddrSpec = NULL_PTR;

    stBuffer.pucBuffer    = pucBuffer;
    stBuffer.ulBufferLen  = ulBufferLen;
    stBuffer.ulCurrentLen = 0;

    pstAddrSpec = UBUF_UBufPtr2Ptr(pstUBuf, upstUri);
    ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_ADDR_SPEC)(pstUBuf,
                                                       pstAddrSpec,
                                                      &stBuffer);
    if (ulRet == SUCCESS)
    {
        *pulMsgLen = stBuffer.ulCurrentLen;
    }

    return ulRet;
}

/* SIP消息解码 */
ULONG SIP_Syntax_SipDecode(UCHAR *pucMsgString,
                           ULONG ulMsgLen,
                           UBUF_HEADER_S *pstUBuf,
                           UBUF_PTR *pupstSipMsg)
{
    ULONG ulRet;
    ABNF_GRAMMAR_NODE_S *pstNode   = NULL_PTR;

    if (NULL_PTR == pstUBuf)
    {
        return FAIL;
    }

    ulRet = ABNF_GrammarParse(pucMsgString,
                              ulMsgLen,
                              g_pstSipRuleList,
                              SIP_GET_RULE_INDEX(SIP_ABNF_RULE_SIP_MESSAGE),
                             &pstNode);
    if (ulRet != SUCCESS)
    {
        return FAIL;
    }

    ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_SIP_MESSAGE)(pstNode,
                                                          pucMsgString,
                                                          pstUBuf,
                                                          pupstSipMsg);
    if (ulRet != SUCCESS)
    {
        ABNF_FreeNodeTree(pstNode);
        return FAIL;
    }

    ABNF_FreeNodeTree(pstNode);
    return SUCCESS;
}

/* URI解码 */
ULONG SIP_Syntax_UriDecode(UCHAR *pucMsgString,
                           ULONG ulMsgLen,
                           UBUF_HEADER_S *pstUBuf,
                           UBUF_PTR *pupstUri)
{
    ULONG ulRet;
    ABNF_GRAMMAR_NODE_S *pstNode   = NULL_PTR;

    if (NULL_PTR == pstUBuf)
    {
        return FAIL;
    }

    ulRet = ABNF_GrammarParse(pucMsgString,
                              ulMsgLen,
                              g_pstSipRuleList,
                              SIP_GET_RULE_INDEX(SIP_ABNF_RULE_ADDR_SPEC),
                             &pstNode);
    if (ulRet != SUCCESS)
    {
        return FAIL;
    }

    ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_ADDR_SPEC)(pstNode,
                                                        pucMsgString,
                                                        pstUBuf,
                                                        pupstUri);
    if (ulRet != SUCCESS)
    {
        ABNF_FreeNodeTree(pstNode);
        return FAIL;
    }

    ABNF_FreeNodeTree(pstNode);
    return SUCCESS;
}

/* URI克隆 */
ULONG SIP_Syntax_UriClone(UBUF_HEADER_S *pstSrcUbufMsg,
                          UBUF_PTR       upSrcUPtr,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          UBUF_PTR      *pupDstUPtr)
{

    return SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_ADDR_SPEC)(pstSrcUbufMsg,
                                                       upSrcUPtr,
                                                       pstDstUbufMsg,
                                                       pupDstUPtr);
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

ULONG SIP_Syntax_Code()
{
}

ULONG SIP_Syntax_Decode()
{
}

ULONG SIP_Syntax_Clone()
{
}
