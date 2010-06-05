/*******************************************************************************
功能    : SDP协议栈语法与编码层功能函数
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/
/* 外部依赖模块头文件 */
#include "common\common.h"
#include "abnf\abnf.h"
#include "ubuf\ubuf.h"

/* 本模块对外提供的常量和结构头文件 */
#include "..\..\include\sdp\sdp.h"

/* 本模块内部使用的常量和结构头文件 */
#include "sdp_const.inc"
#include "sdp_type.inc"

/* 本模块内部函数声明头文件 */
#include "sdp.inc"

/* 本模块全局变量声明头文件 */
#include "sip_var.inc"


/*SDP协议栈初始化*/
ULONG SDP_Init(SDP_CFG_S *pstCfg)
{
    ULONG ulLoop;
    ULONG ulRet;

    /* 本模块只能初始化一次，后续一律直接返回OK */
    if (g_bSdpInit == TRUE)
    {
        return SUCCESS;
    }
    else
    {
        g_bSdpInit = TRUE;
    }

    g_pucSdpBuffer = malloc(SDP_MAX_TEXT_MSG_LEN);

    /* 初始化依赖模块 */
    ulRet = ABNF_Init();
    if (ulRet != SUCCESS)
    {
        printf("\r\nABNF_Init failed");
        return FAIL;
    }

    /* 构建SIP的ABNF规则表 */
    ulRet = ABNF_RegistRuleList(pstCfg->pucAbnfDes, pstCfg->ulLen, &g_ucSdpRuleListIndex);
    if (ulRet != SUCCESS)
    {
        printf("\r\nABNF_BuildRuleList failed");
        return FAIL;
    }

    /* 构建SIP应用规则表，应用规则可以比匹配的ABNF规则少 */
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

/* SDP编码 */
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

/* SDP解码 */
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

/* 克隆就是编码再解码*/
ULONG SDP_Clone(ULONG          ulRuleIndex,
                void          *pSrcStruct,
                UBUF_HEADER_S *pstDstUbuf,
                void         **ppDstStruct)
{
    return SIP_GET_CLONE_FUNC(ulRuleIndex)(pSrcStruct, pstDstUbuf, ppDstStruct);
}

