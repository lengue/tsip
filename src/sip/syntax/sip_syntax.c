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
#include "uri\uri.h"
#include "syntax_comm\syntax_comm.h"

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

    g_pucSipSyntaxBuffer = malloc(SIP_MAX_TEXT_MSG_LEN);

    /* 构建SIP的ABNF规则表 */
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

    /* 构建SIP应用规则表，应用规则可以比匹配的ABNF规则少 */
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

/* 克隆就是编码再解码*/
ULONG SIP_Syntax_Clone(ULONG          ulRuleIndex,
                       void          *pSrcStruct,
                       UBUF_HEADER_S *pstDstUbuf,
                       void         **ppDstStruct)
{
    return SIP_GET_CLONE_FUNC(ulRuleIndex)(pSrcStruct, pstDstUbuf, ppDstStruct);
}
