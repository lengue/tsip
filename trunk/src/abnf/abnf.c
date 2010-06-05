/*******************************************************************************
功能    : ABNF模块功能函数
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/
/* 外部依赖模块头文件 */
#include "common\common.h"

/* 本模块对外提供的常量和结构头文件 */
#include "abnf\abnf_const.h"
#include "abnf\abnf_type.h"

/* 本模块内部使用的常量和结构头文件 */
#include "abnf_const.inc"
#include "abnf_type.inc"

/* 本模块内部函数声明头文件 */
#include "abnf.inc"

/* 本模块全局变量声明头文件 */
#include "abnf_var.inc"


/* 使用ABNF(rfc2234)来解析协议消息 */

/* 使用语法规则来解析协议消息，适合所有使用ABNF来描述的协议消息解析
对于使用ABNF中的规则，如果是一条可选的规则，那么应该将尽量大的匹配放在前面，否则
匹配可能发生错误

    消息文本       协议协议文本(ABNF描述，应用提供)
        |                  |
        |                  |<---------ABNF规则表(ABNF提供)
        |                  |         +核心规则表(ABNF提供)
        |                  |         +ABNF文法规则(即文法解析，ABNF提供)
        |                  |         +结构转化函数(即语法解析，ABNF提供)
        |                  v
        |<-------------协议规则表
        |             +核心规则表(ABNF提供)
        |             +ABNF文法规则(即文法解析，ABNF提供)
        |             +结构转化函数(即语法解析，应用提供)
        v
    消息结构
*/

/* ABNF模块初始化，解析之前必须执行 */
ULONG ABNF_Init()
{
    ULONG ulRet;

    ulRet = ABNF_InitCoreRuleTree();
    ulRet |= ABNF_InitAbnfRuleTree();

    memset(g_apstAbnfRuleList, 0x0, sizeof(g_apstAbnfRuleList));

    /* 核心规则和ABNF规则索引固定为0和1 */
    g_apstAbnfRuleList[ABNF_CORE_RULE_LIST_INDEX] = &g_stCoreRules;
    g_apstAbnfRuleList[ABNF_RULE_LIST_INDEX] = &g_stAbnfRules;

    return ulRet;
}

/* 申请规则索引 */
ULONG ABNF_AllocRuleListIndex(UCHAR *pucRuleListIndex)
{
    UCHAR ucLoop;
    
    for (ucLoop = 0; ucLoop < ABNF_MAX_RULE_NUM; ucLoop++)
    {
        if (g_apstAbnfRuleList[2+ucLoop] == NULL_PTR)
        {
            *pucRuleListIndex = 2 + ucLoop;
            return SUCCESS;
        }
    }

    return FAIL;
}

/* 注册规则，文本必须采用ABNF描述 
pucText 规则文本
ulLen     规则文本长度
pucDepend 依赖规则列表
pucRuleIndex 返回规则索引
*/
ULONG ABNF_RegistRuleList(UCHAR *pucText,
                          ULONG  ulLen,
                          UCHAR *pucDepends,
                          UCHAR  ucDependNum,
                          UCHAR *pucRuleListIndex)
{
    ULONG ulRet;
    UCHAR ucRuleListIndex;
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;

    /* 如果模块还没有初始化，先初始化 */
    if (g_bAbnfInit == FALSE)
    {
        ABNF_Init();
        g_bAbnfInit = TRUE;
    }

    /* 先申请规则索引 */
    ulRet = ABNF_AllocRuleListIndex(&ucRuleListIndex);
    if (ulRet != SUCCESS) 
    {
        return ulRet;
    }

    *pucRuleListIndex = ucRuleListIndex;
    g_apstAbnfRuleList[ucRuleListIndex] = malloc(sizeof(ABNF_RULE_LIST_S));
    g_apstAbnfRuleList[ucRuleListIndex]->ulRuleNum = 0;
    memset(g_apstAbnfRuleList[ucRuleListIndex]->aucDepends, 
           0xff, 
           sizeof(g_apstAbnfRuleList[ucRuleListIndex]->aucDepends));

    if (ucDependNum != 0)
    {
        memcpy(g_apstAbnfRuleList[ucRuleListIndex]->aucDepends,
               pucDepends,
               ucDependNum * sizeof(UCHAR));
    }

    /* 解析成文法结构*/
    ulRet = ABNF_GrammarParse(pucText,
                              ulLen,
                              ABNF_RULE_LIST_INDEX,
                              ABNF_RULE_RULELIST,
                             &pstNode);
    if (ulRet != SUCCESS)
    {
        return FAIL;
    }

    /* 解析成规则结构 */
    g_ucAbnfRegistRuleIndex = ucRuleListIndex;
    ulRet = ABNF_RULE_PARSE_FUNC(ABNF_RULE_RULELIST)(pstNode, pucText, &g_apstAbnfRuleList[ucRuleListIndex]);
    if (ulRet != SUCCESS)
    {
        ABNF_FreeRuleList(ucRuleListIndex);
        g_apstAbnfRuleList[ucRuleListIndex] = NULL_PTR;
    }

    ABNF_FreeNodeTree(pstNode);
    return ulRet;
}

/* 释放一个元素 */
ULONG ABNF_FreeRuleList(UCHAR ucRuleListIndex)
{
    ULONG ulLoop;
    ABNF_RULE_LIST_S *pstRuleListTemp = NULL_PTR;

    pstRuleListTemp = g_apstAbnfRuleList[ucRuleListIndex];

    /* 释放规则数组 */
    if (pstRuleListTemp->pstRules != NULL_PTR)
    {
        /* 先释放每个规则下的元素 */
        for(ulLoop = 0; ulLoop < pstRuleListTemp->ulRuleNum; ulLoop++)
        {
            if(pstRuleListTemp->pstRules[ulLoop].pstElements != NULL_PTR)
            {
                ABNF_FreeElement(pstRuleListTemp->pstRules[ulLoop].pstElements);
            }
        }

        free(pstRuleListTemp->pstRules);
    }

    /* 最后释放规则列表 */
    free(pstRuleListTemp);
    return SUCCESS;
}

/* 获取应用规则索引*/
ULONG ABNF_GetRuleIndex(UCHAR ucRuleListIndex, UCHAR *pucRuleName, ULONG *pulRuleIndex)
{
    ULONG ulLoop;
    ULONG ulRet;
    ABNF_RULE_LIST_S *pstRuleListTemp = NULL_PTR;

    pstRuleListTemp = g_apstAbnfRuleList[ucRuleListIndex];
    for (ulLoop = 0; ulLoop < pstRuleListTemp->ulRuleNum; ulLoop++)
    {
        ulRet = strcmp(pucRuleName,
                       pstRuleListTemp->pstRules[ulLoop].aucName);
        if (ulRet == SUCCESS)
        {
            *pulRuleIndex = ulLoop;
            return SUCCESS;
        }
    }

    return FAIL;
}

/* 文本解析，先解析文法，在构造语法结构
UCHAR *pucText                  用于匹配的文本
ABNF_RULE_S *pstRuleList        用于匹配的规则
ULONG ulRuleNum                 匹配规则总数
ULONG ulMatchRule               用于匹配的规则
ABNF_GRAMMAR_NODE_S **ppstNode  匹配生成的文法描述
*/
ULONG ABNF_GrammarParse(UCHAR *pucText,
                        ULONG  ulLen,
                        UCHAR  ucRuleListIndex,
                        ULONG  ulMatchRule,
                        ABNF_GRAMMAR_NODE_S **ppstNode)
{
    ULONG ulSize   = 0;
    ULONG ulRet;
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    ABNF_ELEMENT_S       stRuleElement;
    ABNF_MATCH_TARGET_S  stMatchTarget;
    ABNF_RULE_LIST_S *pstRuleListTemp = NULL_PTR;

    pstRuleListTemp = (ABNF_RULE_LIST_S *)g_apstAbnfRuleList[ucRuleListIndex];

    stMatchTarget.pucText = pucText;
    stMatchTarget.ulLen = ulLen;
    stMatchTarget.ucRuleListIndex = ucRuleListIndex;

    ABNF_SuccessRecord();

    /* 初始化匹配规则 */
    stRuleElement.ulMinRepeat = 1;
    stRuleElement.ulMaxRepeat = 1;
    stRuleElement.eType       = ABNF_ELEMENT_RULE;
    stRuleElement.u.stRule.ucRuleListIndex  = ucRuleListIndex;
    stRuleElement.u.stRule.ulIndex    = ulMatchRule;
    stRuleElement.pNextElement    = NULL_PTR;

    ulRet = ABNF_MatchElement(&stMatchTarget, 0, &stRuleElement, &ulSize, &pstNode);
    if (ulRet == SUCCESS)
    {
        *ppstNode = pstNode;
    }
    else
    {
        printf("\r\nABNF match fail![Name:%s Pos:%d]",
                pstRuleListTemp->pstRules[ulMatchRule].aucName,
                g_ulAbnfFailPos);
    }

    return ulRet;
}

/* 增加一个字符元素 */
ULONG ABNF_AddCharElement(ABNF_ELEMENT_S **ppstElement,
                          ULONG ulMinRepeat,
                          ULONG ulMaxRepeat,
                          UCHAR ucMinValue,
                          UCHAR ucMaxValue)
{
    ABNF_ELEMENT_S *pstElement = NULL_PTR;

#if DEBUG_ABNF_CORE
    printf("\r\nAddCharElement(%u,%u) value(%u-%u)",
            ulMinRepeat,
            ulMaxRepeat,
            ucMinValue,
            ucMaxValue);
#endif

    pstElement = malloc(sizeof(ABNF_ELEMENT_S));
    pstElement->ulMinRepeat = ulMinRepeat;
    pstElement->ulMaxRepeat = ulMaxRepeat;
    pstElement->eType       = ABNF_ELEMENT_DATA_CHAR;
    pstElement->u.stChar.ucMinValue = ucMinValue;
    pstElement->u.stChar.ucMaxValue = ucMaxValue;
    pstElement->pNextElement    = NULL_PTR;
    *ppstElement = pstElement;

    return SUCCESS;
}

/* 增加一个字符串元素 */
ULONG ABNF_AddStringElement(ABNF_ELEMENT_S **ppstElement,
                            ULONG ulMinRepeat,
                            ULONG ulMaxRepeat,
                            BOOL  bSensitive,
                            UCHAR *pucString)
{
    ABNF_ELEMENT_S *pstElement = NULL_PTR;

#if DEBUG_ABNF_CORE
    printf("\r\nAddStringElement(%u,%u) value(%s)",
            ulMinRepeat,
            ulMaxRepeat,
            pucString);
#endif

    pstElement = malloc(sizeof(ABNF_ELEMENT_S));
    pstElement->ulMinRepeat = ulMinRepeat;
    pstElement->ulMaxRepeat = ulMaxRepeat;
    pstElement->eType       = ABNF_ELEMENT_DATA_STRING;
    pstElement->u.stString.bSensitive = bSensitive;
    pstElement->u.stString.pucString = malloc(strlen(pucString)+1);
    strncpy(pstElement->u.stString.pucString, pucString, strlen(pucString));
    pstElement->u.stString.pucString[strlen(pucString)] = '\0';
    pstElement->pNextElement    = NULL_PTR;
    *ppstElement = pstElement;

    return SUCCESS;
}

/* 增加一个规则元素 */
ULONG ABNF_AddRuleElement(ABNF_ELEMENT_S **ppstElement,
                          ULONG ulMinRepeat,
                          ULONG ulMaxRepeat,
                          UCHAR ucRuleListIndex,
                          ULONG ulIndex)
{
    ABNF_ELEMENT_S *pstElement = NULL_PTR;

#if DEBUG_ABNF_CORE
    printf("\r\nAddRuleElement(%u,%u) Rule(%u) index(%u)",
            ulMinRepeat,
            ulMaxRepeat,
            ucRuleListIndex,
            ulIndex);
#endif

    pstElement = malloc(sizeof(ABNF_ELEMENT_S));
    pstElement->ulMinRepeat = ulMinRepeat;
    pstElement->ulMaxRepeat = ulMaxRepeat;
    pstElement->eType       = ABNF_ELEMENT_RULE;
    pstElement->u.stRule.ucRuleListIndex  = ucRuleListIndex;
    pstElement->u.stRule.ulIndex    = ulIndex;
    pstElement->pNextElement    = NULL_PTR;
    *ppstElement = pstElement;

    return SUCCESS;
}

/* 增加一个序列元素 */
ULONG ABNF_AddSequenceElement(ABNF_ELEMENT_S **ppstElement,
                              ULONG ulMinRepeat,
                              ULONG ulMaxRepeat)
{
    ABNF_ELEMENT_S *pstElement = NULL_PTR;

#if DEBUG_ABNF_CORE
    printf("\r\nAddSequenceElement(%u,%u)",
            ulMinRepeat,
            ulMaxRepeat);
#endif

    pstElement = malloc(sizeof(ABNF_ELEMENT_S));
    pstElement->ulMinRepeat = ulMinRepeat;
    pstElement->ulMaxRepeat = ulMaxRepeat;
    pstElement->eType       = ABNF_ELEMENT_SEQUENCE;
    pstElement->u.pstFirstChild = NULL_PTR;
    pstElement->pNextElement    = NULL_PTR;
    *ppstElement = pstElement;

    return SUCCESS;
}

/* 增加一个可选元素 */
ULONG ABNF_AddOptionalElement(ABNF_ELEMENT_S **ppstElement,
                              ULONG ulMinRepeat,
                              ULONG ulMaxRepeat)
{
    ABNF_ELEMENT_S *pstElement = NULL_PTR;

#if DEBUG_ABNF_CORE
    printf("\r\nAddOptionalElement(%u,%u)",
            ulMinRepeat,
            ulMaxRepeat);
#endif

    pstElement = malloc(sizeof(ABNF_ELEMENT_S));
    pstElement->ulMinRepeat = ulMinRepeat;
    pstElement->ulMaxRepeat = ulMaxRepeat;
    pstElement->eType       = ABNF_ELEMENT_OPTIONAL;
    pstElement->u.pstFirstChild = NULL_PTR;
    pstElement->pNextElement    = NULL_PTR;
    *ppstElement = pstElement;

    return SUCCESS;
}

/* 释放一个元素 */
ULONG ABNF_FreeElement(ABNF_ELEMENT_S *pstElement)
{
    if (pstElement == NULL_PTR)
    {
        return SUCCESS;
    }

    if (pstElement->pNextElement != NULL_PTR)
    {
        ABNF_FreeElement(pstElement->pNextElement);
    }

    if (((pstElement->eType == ABNF_ELEMENT_SEQUENCE)
       ||(pstElement->eType == ABNF_ELEMENT_SEQUENCE))
      &&(pstElement->u.pstFirstChild != NULL_PTR))
    {
        ABNF_FreeElement(pstElement->u.pstFirstChild);
    }

    free(pstElement);
    return SUCCESS;
}

/* 初始化ABNF核心规则表 */
ULONG ABNF_InitCoreRuleTree()
{
    ABNF_ELEMENT_S **ppstElement = NULL_PTR;

    /* 初始化规则结构 */
    g_stCoreRules.ulRuleNum = ABNF_CORE_RULE_BUTT;
    memset(g_stCoreRules.aucDepends, 0xff, sizeof(g_stCoreRules.aucDepends));
    g_stCoreRules.pstRules = g_astCoreRuleTbl;

    /* ALPHA  = %x41-5A / %x61-7A  ; A-Z / a-z */
    ppstElement = &g_astCoreRuleTbl[ABNF_CORE_RULE_ALPHA].pstElements;
    ABNF_AddOptionalElement(ppstElement, 1, 1);
    {
        ppstElement = &(*ppstElement)->u.pstFirstChild;
        ABNF_AddCharElement(ppstElement, 1, 1, 0x41, 0x5a);

        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddCharElement(ppstElement, 1, 1, 0x61, 0x7a);
    }

    /* BIT    = "0" / "1" */
    ppstElement = &g_astCoreRuleTbl[ABNF_CORE_RULE_BIT].pstElements;
    ABNF_AddCharElement(ppstElement, 1, 1, '0', '1');

    /* CHAR   = %x01-7F */
    ppstElement = &g_astCoreRuleTbl[ABNF_CORE_RULE_CHAR].pstElements;
    ABNF_AddCharElement(ppstElement, 1, 1, 0x01, 0x7f);

    /* CR     = %x0D */
    ppstElement = &g_astCoreRuleTbl[ABNF_CORE_RULE_CR].pstElements;
    ABNF_AddCharElement(ppstElement, 1, 1, 0x0d, 0x0d);

    /* CRLF   = CR LF */
    ppstElement = &g_astCoreRuleTbl[ABNF_CORE_RULE_CRLF].pstElements;
    ABNF_AddSequenceElement(ppstElement, 1, 1);
    {
        ppstElement = &(*ppstElement)->u.pstFirstChild;
        ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_CORE_RULE_LIST_INDEX, ABNF_CORE_RULE_CR);

        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_CORE_RULE_LIST_INDEX, ABNF_CORE_RULE_LF);
    }

    /* CTL    = %x00-1F / %x7F */
    ppstElement = &g_astCoreRuleTbl[ABNF_CORE_RULE_CTL].pstElements;
    ABNF_AddOptionalElement(ppstElement, 1, 1);
    {
        ppstElement = &(*ppstElement)->u.pstFirstChild;
        ABNF_AddCharElement(ppstElement, 1, 1, 0x0, 0x1f);

        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddCharElement(ppstElement, 1, 1, 0x7f, 0x7f);
    }

    /* DIGIT  = %x30-39 */
    ppstElement = &g_astCoreRuleTbl[ABNF_CORE_RULE_DIGIT].pstElements;
    ABNF_AddCharElement(ppstElement, 1, 1, 0x30, 0x39);

    /* DQUOTE = %x22 */
    ppstElement = &g_astCoreRuleTbl[ABNF_CORE_RULE_DQUOTE].pstElements;
    ABNF_AddCharElement(ppstElement, 1, 1, 0x22, 0x22);

    /* HEXDIG = DIGIT / "A" / "B" / "C" / "D" / "E" / "F" */
    ppstElement = &g_astCoreRuleTbl[ABNF_CORE_RULE_HEXDIG].pstElements;
    ABNF_AddOptionalElement(ppstElement, 1, 1);
    {
        ppstElement = &(*ppstElement)->u.pstFirstChild;
        ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_CORE_RULE_LIST_INDEX, ABNF_CORE_RULE_DIGIT);

        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddCharElement(ppstElement, 1, 1, 'a', 'f');

        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddCharElement(ppstElement, 1, 1, 'A', 'F');
    }

    /* HTAB   = %x09 */
    ppstElement = &g_astCoreRuleTbl[ABNF_CORE_RULE_HTAB].pstElements;
    ABNF_AddCharElement(ppstElement, 1, 1, 0x09, 0x09);

    /* LF     = %x0A */
    ppstElement = &g_astCoreRuleTbl[ABNF_CORE_RULE_LF].pstElements;
    ABNF_AddCharElement(ppstElement, 1, 1, 0x0a, 0x0a);

    /* LWSP   = *(WSP / CRLF WSP) */
    ppstElement = &g_astCoreRuleTbl[ABNF_CORE_RULE_LWSP].pstElements;
    ABNF_AddOptionalElement(ppstElement, 0, NULL_ULONG);
    {
        ppstElement = &(*ppstElement)->u.pstFirstChild;
        ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_CORE_RULE_LIST_INDEX, ABNF_CORE_RULE_WSP);

        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddSequenceElement(ppstElement, 1, 1);
        {
            ppstElement = &(*ppstElement)->u.pstFirstChild;
            ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_CORE_RULE_LIST_INDEX, ABNF_CORE_RULE_CRLF);

            ppstElement = &(*ppstElement)->pNextElement;
            ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_CORE_RULE_LIST_INDEX, ABNF_CORE_RULE_WSP);
        }
    }

    /* OCTET  = %x00-FF */
    ppstElement = &g_astCoreRuleTbl[ABNF_CORE_RULE_OCTET].pstElements;
    ABNF_AddCharElement(ppstElement, 1, 1, 0x00, 0xff);

    /* SP     = %x20 */
    ppstElement = &g_astCoreRuleTbl[ABNF_CORE_RULE_SP].pstElements;
    ABNF_AddCharElement(ppstElement, 1, 1, 0x20, 0x20);

    /* VCHAR  = %x21-7E */
    ppstElement = &g_astCoreRuleTbl[ABNF_CORE_RULE_VCHAR].pstElements;
    ABNF_AddCharElement(ppstElement, 1, 1, 0x21, 0x7f);

    /* WSP    = SP / HTAB */
    ppstElement = &g_astCoreRuleTbl[ABNF_CORE_RULE_WSP].pstElements;
    ABNF_AddOptionalElement(ppstElement, 1, 1);
    {
        ppstElement = &(*ppstElement)->u.pstFirstChild;
        ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_CORE_RULE_LIST_INDEX, ABNF_CORE_RULE_SP);

        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_CORE_RULE_LIST_INDEX, ABNF_CORE_RULE_HTAB);
    }

    return SUCCESS;
}

/* 初始化ABNF规则库表 */
ULONG ABNF_InitAbnfRuleTree()
{
    ABNF_ELEMENT_S **ppstMark    = NULL_PTR;
    ABNF_ELEMENT_S **ppstElement = NULL_PTR;

    /* 初始化规则结构 */
    g_stAbnfRules.ulRuleNum = ABNF_RULE_BUTT;
    memset(g_stAbnfRules.aucDepends, 0xff, sizeof(g_stAbnfRules.aucDepends));
    g_stAbnfRules.pstRules = g_astAbnfRuleTbl;

    /* rulelist = 1*( rule / (*c-wsp c-nl) ) */
    ppstElement = &g_astAbnfRuleTbl[ABNF_RULE_RULELIST].pstElements;
    ABNF_AddOptionalElement(ppstElement, 1, NULL_ULONG);
    {
        ppstElement = &(*ppstElement)->u.pstFirstChild;
        ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_RULE_LIST_INDEX, ABNF_RULE_RULE);

        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddSequenceElement(ppstElement, 1, 1);
        {
            ppstElement = &(*ppstElement)->u.pstFirstChild;
            ABNF_AddRuleElement(ppstElement, 0, NULL_ULONG, ABNF_RULE_LIST_INDEX, ABNF_RULE_C_WSP);

            ppstElement = &(*ppstElement)->pNextElement;
            ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_RULE_LIST_INDEX, ABNF_RULE_C_NL);
        }
    }

    /* rule = rulename defined-as elements c-nl */
    ppstElement = &g_astAbnfRuleTbl[ABNF_RULE_RULE].pstElements;
    ABNF_AddSequenceElement(ppstElement, 1, 1);
    {
        ppstElement = &(*ppstElement)->u.pstFirstChild;
        ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_RULE_LIST_INDEX, ABNF_RULE_RULENAME);

        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_RULE_LIST_INDEX, ABNF_RULE_DEFINED_AS);

        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_RULE_LIST_INDEX, ABNF_RULE_ELEMENTS);

        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_RULE_LIST_INDEX, ABNF_RULE_C_NL);
    }

    /* rulename = ALPHA *(ALPHA / DIGIT / "-") */
    ppstElement = &g_astAbnfRuleTbl[ABNF_RULE_RULENAME].pstElements;
    ABNF_AddSequenceElement(ppstElement, 1, 1);
    {
        ppstElement = &(*ppstElement)->u.pstFirstChild;
        ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_CORE_RULE_LIST_INDEX, ABNF_CORE_RULE_ALPHA);

        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddOptionalElement(ppstElement, 0, NULL_ULONG);
        {
            ppstElement = &(*ppstElement)->u.pstFirstChild;
            ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_CORE_RULE_LIST_INDEX, ABNF_CORE_RULE_ALPHA);

            ppstElement = &(*ppstElement)->pNextElement;
            ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_CORE_RULE_LIST_INDEX, ABNF_CORE_RULE_DIGIT);

            ppstElement = &(*ppstElement)->pNextElement;
            ABNF_AddCharElement(ppstElement, 1, 1, '-', '-');
        }
    }

    /* defined-as = *c-wsp ("=" / "=/") *c-wsp */
    ppstElement = &g_astAbnfRuleTbl[ABNF_RULE_DEFINED_AS].pstElements;
    ABNF_AddSequenceElement(ppstElement, 1, 1);
    {
        ppstElement = &(*ppstElement)->u.pstFirstChild;
        ABNF_AddRuleElement(ppstElement, 0, NULL_ULONG, ABNF_RULE_LIST_INDEX, ABNF_RULE_C_WSP);

        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddOptionalElement(ppstElement, 1, 1);
        ppstMark = ppstElement;
        {
            ppstElement = &(*ppstElement)->u.pstFirstChild;
            ABNF_AddCharElement(ppstElement, 0, NULL_ULONG, '=', '=');

            ppstElement = &(*ppstElement)->pNextElement;
            ABNF_AddStringElement(ppstElement, 1, 1, FALSE, "=/");
        }

        ppstElement = ppstMark;
        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddRuleElement(ppstElement, 0, NULL_ULONG, ABNF_RULE_LIST_INDEX, ABNF_RULE_C_WSP);
    }

    /* elements = alternation *c-wsp */
    ppstElement = &g_astAbnfRuleTbl[ABNF_RULE_ELEMENTS].pstElements;
    ABNF_AddSequenceElement(ppstElement, 1, 1);
    {
        ppstElement = &(*ppstElement)->u.pstFirstChild;
        ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_RULE_LIST_INDEX, ABNF_RULE_ALTERNATION);

        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddRuleElement(ppstElement, 0, NULL_ULONG, ABNF_RULE_LIST_INDEX, ABNF_RULE_C_WSP);
    }

    /* c-wsp = WSP / (c-nl WSP) */
    ppstElement = &g_astAbnfRuleTbl[ABNF_RULE_C_WSP].pstElements;
    ABNF_AddOptionalElement(ppstElement, 1, 1);
    {
        ppstElement = &(*ppstElement)->u.pstFirstChild;
        ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_CORE_RULE_LIST_INDEX, ABNF_CORE_RULE_WSP);

        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddSequenceElement(ppstElement, 1, 1);
        {
            ppstElement = &(*ppstElement)->u.pstFirstChild;
            ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_RULE_LIST_INDEX, ABNF_RULE_C_NL);

            ppstElement = &(*ppstElement)->pNextElement;
            ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_CORE_RULE_LIST_INDEX, ABNF_CORE_RULE_WSP);
        }
    }

    /* c-nl = comment / CRLF */
    ppstElement = &g_astAbnfRuleTbl[ABNF_RULE_C_NL].pstElements;
    ABNF_AddOptionalElement(ppstElement, 1, 1);
    {
        ppstElement = &(*ppstElement)->u.pstFirstChild;
        ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_RULE_LIST_INDEX, ABNF_RULE_COMMENT);

        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_CORE_RULE_LIST_INDEX, ABNF_CORE_RULE_CRLF);
    }

    /* comment = ";" *(WSP / VCHAR) CRLF */
    ppstElement = &g_astAbnfRuleTbl[ABNF_RULE_COMMENT].pstElements;
    ABNF_AddSequenceElement(ppstElement, 1, 1);
    {
        ppstElement = &(*ppstElement)->u.pstFirstChild;
        ABNF_AddCharElement(ppstElement, 1, 1, ';', ';');

        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddOptionalElement(ppstElement, 0, NULL_ULONG);
        ppstMark = ppstElement;
        {
            ppstElement = &(*ppstElement)->u.pstFirstChild;
            ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_CORE_RULE_LIST_INDEX, ABNF_CORE_RULE_WSP);

            ppstElement = &(*ppstElement)->pNextElement;
            ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_CORE_RULE_LIST_INDEX, ABNF_CORE_RULE_VCHAR);
        }

        ppstElement = ppstMark;
        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_CORE_RULE_LIST_INDEX, ABNF_CORE_RULE_CRLF);
    }

    /* alternation    =  concatenation *(*c-wsp "/" *c-wsp concatenation) */
    ppstElement = &g_astAbnfRuleTbl[ABNF_RULE_ALTERNATION].pstElements;
    ABNF_AddSequenceElement(ppstElement, 1, 1);
    {
        ppstElement = &(*ppstElement)->u.pstFirstChild;
        ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_RULE_LIST_INDEX, ABNF_RULE_CONCATENATION);

        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddSequenceElement(ppstElement, 0, NULL_ULONG);
        {
            ppstElement = &(*ppstElement)->u.pstFirstChild;
            ABNF_AddRuleElement(ppstElement, 0, NULL_ULONG, ABNF_RULE_LIST_INDEX, ABNF_RULE_C_WSP);

            ppstElement = &(*ppstElement)->pNextElement;
            ABNF_AddCharElement(ppstElement, 1, 1, '/', '/');

            ppstElement = &(*ppstElement)->pNextElement;
            ABNF_AddRuleElement(ppstElement, 0, NULL_ULONG, ABNF_RULE_LIST_INDEX, ABNF_RULE_C_WSP);

            ppstElement = &(*ppstElement)->pNextElement;
            ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_RULE_LIST_INDEX, ABNF_RULE_CONCATENATION);
        }
    }

    /* concatenation = repetition *(1*c-wsp repetition) */
    ppstElement = &g_astAbnfRuleTbl[ABNF_RULE_CONCATENATION].pstElements;
    ABNF_AddSequenceElement(ppstElement, 1, 1);
    {
        ppstElement = &(*ppstElement)->u.pstFirstChild;
        ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_RULE_LIST_INDEX, ABNF_RULE_REPETITION);

        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddSequenceElement(ppstElement, 0, NULL_ULONG);
        {
            ppstElement = &(*ppstElement)->u.pstFirstChild;
            ABNF_AddRuleElement(ppstElement, 1, NULL_ULONG, ABNF_RULE_LIST_INDEX, ABNF_RULE_C_WSP);

            ppstElement = &(*ppstElement)->pNextElement;
            ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_RULE_LIST_INDEX, ABNF_RULE_REPETITION);
        }
    }

    /* repetition = [repeat] element */
    ppstElement = &g_astAbnfRuleTbl[ABNF_RULE_REPETITION].pstElements;
    ABNF_AddSequenceElement(ppstElement, 1, 1);
    {
        ppstElement = &(*ppstElement)->u.pstFirstChild;
        ABNF_AddRuleElement(ppstElement, 0, 1, ABNF_RULE_LIST_INDEX, ABNF_RULE_REPEAT);

        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_RULE_LIST_INDEX, ABNF_RULE_ELEMENT);
    }

    /* repeat = 1*DIGIT / (*DIGIT "*" *DIGIT) */
    /* 这个地方比较特殊，可选的第一部分是第二部分的一个子集，为了达到最大匹配的
    目的，将可选的部分掉了一个个*/
    ppstElement = &g_astAbnfRuleTbl[ABNF_RULE_REPEAT].pstElements;
    ABNF_AddOptionalElement(ppstElement, 1, 1);
    {
        ppstElement = &(*ppstElement)->u.pstFirstChild;
        ABNF_AddSequenceElement(ppstElement, 1, 1);
        ppstMark = ppstElement;
        {
            ppstElement = &(*ppstElement)->u.pstFirstChild;
            ABNF_AddRuleElement(ppstElement, 0, NULL_ULONG, ABNF_CORE_RULE_LIST_INDEX, ABNF_CORE_RULE_DIGIT);

            ppstElement = &(*ppstElement)->pNextElement;
            ABNF_AddCharElement(ppstElement, 1, 1, '*', '*');

            ppstElement = &(*ppstElement)->pNextElement;
            ABNF_AddRuleElement(ppstElement, 0, NULL_ULONG, ABNF_CORE_RULE_LIST_INDEX, ABNF_CORE_RULE_DIGIT);
        }

        ppstElement = ppstMark;
        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddRuleElement(ppstElement, 1, NULL_ULONG, ABNF_CORE_RULE_LIST_INDEX, ABNF_CORE_RULE_DIGIT);
    }

    /* element = rulename / group / option / char-val / num-val / prose-val */
    ppstElement = &g_astAbnfRuleTbl[ABNF_RULE_ELEMENT].pstElements;
    ABNF_AddOptionalElement(ppstElement, 1, 1);
    {
        ppstElement = &(*ppstElement)->u.pstFirstChild;
        ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_RULE_LIST_INDEX, ABNF_RULE_RULENAME);

        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_RULE_LIST_INDEX, ABNF_RULE_GROUP);

        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_RULE_LIST_INDEX, ABNF_RULE_OPTION);

        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_RULE_LIST_INDEX, ABNF_RULE_CHAR_VAL);

        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_RULE_LIST_INDEX, ABNF_RULE_NUM_VAL);

        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_RULE_LIST_INDEX, ABNF_RULE_PROSE_VAL);
    }

    /* group = "(" *c-wsp alternation *c-wsp ")" */
    ppstElement = &g_astAbnfRuleTbl[ABNF_RULE_GROUP].pstElements;
    ABNF_AddSequenceElement(ppstElement, 1, 1);
    {
        ppstElement = &(*ppstElement)->u.pstFirstChild;
        ABNF_AddCharElement(ppstElement, 1, 1, '(', '(');

        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddRuleElement(ppstElement, 0, NULL_ULONG, ABNF_RULE_LIST_INDEX, ABNF_RULE_C_WSP);

        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_RULE_LIST_INDEX, ABNF_RULE_ALTERNATION);

        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddRuleElement(ppstElement, 0, NULL_ULONG, ABNF_RULE_LIST_INDEX, ABNF_RULE_C_WSP);

        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddCharElement(ppstElement, 1, 1, ')', ')');
    }

    /* option = "[" *c-wsp alternation *c-wsp "]" */
    ppstElement = &g_astAbnfRuleTbl[ABNF_RULE_OPTION].pstElements;
    ABNF_AddSequenceElement(ppstElement, 1, 1);
    {
        ppstElement = &(*ppstElement)->u.pstFirstChild;
        ABNF_AddCharElement(ppstElement, 1, 1, '[', '[');

        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddRuleElement(ppstElement, 0, NULL_ULONG, ABNF_RULE_LIST_INDEX, ABNF_RULE_C_WSP);

        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_RULE_LIST_INDEX, ABNF_RULE_ALTERNATION);

        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddRuleElement(ppstElement, 0, NULL_ULONG, ABNF_RULE_LIST_INDEX, ABNF_RULE_C_WSP);

        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddCharElement(ppstElement, 1, 1, ']', ']');
    }

    /* char-val = DQUOTE *(%x20-21 / %x23-7E) DQUOTE */
    ppstElement = &g_astAbnfRuleTbl[ABNF_RULE_CHAR_VAL].pstElements;
    ABNF_AddSequenceElement(ppstElement, 1, 1);
    {
        ppstElement = &(*ppstElement)->u.pstFirstChild;
        ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_CORE_RULE_LIST_INDEX, ABNF_CORE_RULE_DQUOTE);

        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddOptionalElement(ppstElement, 0, NULL_ULONG);
        ppstMark = ppstElement;
        {
            ppstElement = &(*ppstElement)->u.pstFirstChild;
            ABNF_AddCharElement(ppstElement, 1, 1, 0x20, 0x21);

            ppstElement = &(*ppstElement)->pNextElement;
            ABNF_AddCharElement(ppstElement, 1, 1, 0x23, 0x7e);
        }

        ppstElement = ppstMark;
        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_CORE_RULE_LIST_INDEX, ABNF_CORE_RULE_DQUOTE);
    }

    /* num-val = "%" (bin-val / dec-val / hex-val) */
    ppstElement = &g_astAbnfRuleTbl[ABNF_RULE_NUM_VAL].pstElements;
    ABNF_AddSequenceElement(ppstElement, 1, 1);
    {
        ppstElement = &(*ppstElement)->u.pstFirstChild;
        ABNF_AddCharElement(ppstElement, 1, 1, '%', '%');

        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddOptionalElement(ppstElement, 1, 1);
        {
            ppstElement = &(*ppstElement)->u.pstFirstChild;
            ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_RULE_LIST_INDEX, ABNF_RULE_BIN_VAL);

            ppstElement = &(*ppstElement)->pNextElement;
            ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_RULE_LIST_INDEX, ABNF_RULE_DEC_VAL);

            ppstElement = &(*ppstElement)->pNextElement;
            ABNF_AddRuleElement(ppstElement, 1, 1, ABNF_RULE_LIST_INDEX, ABNF_RULE_HEX_VAL);
        }
    }

    /* bin-val = "b" 1*BIT [ 1*("." 1*BIT) / ("-" 1*BIT) ] */
    ppstElement = &g_astAbnfRuleTbl[ABNF_RULE_BIN_VAL].pstElements;
    ABNF_AddSequenceElement(ppstElement, 1, 1);
    {
        ppstElement = &(*ppstElement)->u.pstFirstChild;
        ABNF_AddStringElement(ppstElement, 1, 1, FALSE, "b");

        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddRuleElement(ppstElement, 1, NULL_ULONG, ABNF_CORE_RULE_LIST_INDEX, ABNF_CORE_RULE_BIT);

        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddOptionalElement(ppstElement, 0, 1);
        {
            ppstElement = &(*ppstElement)->u.pstFirstChild;
            ABNF_AddSequenceElement(ppstElement, 1, NULL_ULONG);
            ppstMark = ppstElement;
            {
                ppstElement = &(*ppstElement)->u.pstFirstChild;
                ABNF_AddCharElement(ppstElement, 1, 1, '.', '.');

                ppstElement = &(*ppstElement)->pNextElement;
                ABNF_AddRuleElement(ppstElement, 1, NULL_UCHAR, ABNF_CORE_RULE_LIST_INDEX, ABNF_CORE_RULE_BIT);
            }

            ppstElement = ppstMark;
            ppstElement = &(*ppstElement)->pNextElement;
            ABNF_AddSequenceElement(ppstElement, 1, NULL_ULONG);
            {
                ppstElement = &(*ppstElement)->u.pstFirstChild;
                ABNF_AddCharElement(ppstElement, 1, 1, '-', '-');

                ppstElement = &(*ppstElement)->pNextElement;
                ABNF_AddRuleElement(ppstElement, 1, NULL_UCHAR, ABNF_CORE_RULE_LIST_INDEX, ABNF_CORE_RULE_BIT);
            }
        }
    }

    /* dec-val = "d" 1*DIGIT [ 1*("." 1*DIGIT) / ("-" 1*DIGIT) ] */
    ppstElement = &g_astAbnfRuleTbl[ABNF_RULE_DEC_VAL].pstElements;
    ABNF_AddSequenceElement(ppstElement, 1, 1);
    {
        ppstElement = &(*ppstElement)->u.pstFirstChild;
        ABNF_AddStringElement(ppstElement, 1, 1, FALSE, "d");

        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddRuleElement(ppstElement, 1, NULL_ULONG, ABNF_CORE_RULE_LIST_INDEX, ABNF_CORE_RULE_DIGIT);

        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddOptionalElement(ppstElement, 0, 1);
        {
            ppstElement = &(*ppstElement)->u.pstFirstChild;
            ABNF_AddSequenceElement(ppstElement, 1, NULL_ULONG);
            ppstMark = ppstElement;
            {
                ppstElement = &(*ppstElement)->u.pstFirstChild;
                ABNF_AddCharElement(ppstElement, 1, 1, '.', '.');

                ppstElement = &(*ppstElement)->pNextElement;
                ABNF_AddRuleElement(ppstElement, 1, NULL_UCHAR, ABNF_CORE_RULE_LIST_INDEX, ABNF_CORE_RULE_DIGIT);
            }

            ppstElement = ppstMark;
            ppstElement = &(*ppstElement)->pNextElement;
            ABNF_AddSequenceElement(ppstElement, 1, NULL_ULONG);
            {
                ppstElement = &(*ppstElement)->u.pstFirstChild;
                ABNF_AddCharElement(ppstElement, 1, 1, '-', '-');

                ppstElement = &(*ppstElement)->pNextElement;
                ABNF_AddRuleElement(ppstElement, 1, NULL_UCHAR, ABNF_CORE_RULE_LIST_INDEX, ABNF_CORE_RULE_DIGIT);
            }
        }
    }

    /* hex-val = "x" 1*HEXDIG [ 1*("." 1*HEXDIG) / ("-" 1*HEXDIG) ] */
    ppstElement = &g_astAbnfRuleTbl[ABNF_RULE_HEX_VAL].pstElements;
    ABNF_AddSequenceElement(ppstElement, 1, 1);
    {
        ppstElement = &(*ppstElement)->u.pstFirstChild;
        ABNF_AddStringElement(ppstElement, 1, 1, FALSE, "x");

        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddRuleElement(ppstElement, 1, NULL_ULONG, ABNF_CORE_RULE_LIST_INDEX, ABNF_CORE_RULE_HEXDIG);

        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddOptionalElement(ppstElement, 0, 1);
        {
            ppstElement = &(*ppstElement)->u.pstFirstChild;
            ABNF_AddSequenceElement(ppstElement, 1, NULL_ULONG);
            ppstMark = ppstElement;
            {
                ppstElement = &(*ppstElement)->u.pstFirstChild;
                ABNF_AddCharElement(ppstElement, 1, 1, '.', '.');

                ppstElement = &(*ppstElement)->pNextElement;
                ABNF_AddRuleElement(ppstElement, 1, NULL_UCHAR, ABNF_CORE_RULE_LIST_INDEX, ABNF_CORE_RULE_HEXDIG);
            }

            ppstElement = ppstMark;
            ppstElement = &(*ppstElement)->pNextElement;
            ABNF_AddSequenceElement(ppstElement, 1, NULL_ULONG);
            {
                ppstElement = &(*ppstElement)->u.pstFirstChild;
                ABNF_AddCharElement(ppstElement, 1, 1, '-', '-');

                ppstElement = &(*ppstElement)->pNextElement;
                ABNF_AddRuleElement(ppstElement, 1, NULL_UCHAR, ABNF_CORE_RULE_LIST_INDEX, ABNF_CORE_RULE_HEXDIG);
            }
        }
    }

    /* prose-val = "<" *(%x20-3D / %x3F-7E) ">" */
    ppstElement = &g_astAbnfRuleTbl[ABNF_RULE_PROSE_VAL].pstElements;
    ABNF_AddSequenceElement(ppstElement, 1, 1);
    {
        ppstElement = &(*ppstElement)->u.pstFirstChild;
        ABNF_AddCharElement(ppstElement, 1, 1, '<', '<');

        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddOptionalElement(ppstElement, 0, NULL_ULONG);
        ppstMark = ppstElement;
        {
            ppstElement = &(*ppstElement)->u.pstFirstChild;
            ABNF_AddCharElement(ppstElement, 1, 1, 0x20, 0x3d);

            ppstElement = &(*ppstElement)->pNextElement;
            ABNF_AddCharElement(ppstElement, 1, 1, 0x3e, 0x7e);
        }

        ppstElement = ppstMark;
        ppstElement = &(*ppstElement)->pNextElement;
        ABNF_AddCharElement(ppstElement, 1, 1, '>', '>');
    }

    return SUCCESS;
}

/* 增加一个节点 */
ABNF_GRAMMAR_NODE_S* ABNF_AllocNode(ULONG ulOffset, UCHAR ucRuleListIndex, ULONG ulIndex)
{
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;

    pstNode = malloc(sizeof(ABNF_GRAMMAR_NODE_S));
    pstNode->ulOffset    = ulOffset;
    pstNode->ulSize      = 0;
    pstNode->ucRuleListIndex   = ucRuleListIndex;
    pstNode->ulIndex     = ulIndex;
    pstNode->pstChild    = NULL_PTR;
    pstNode->pstNextNode = NULL_PTR;

    return pstNode;
}

/* 释放节点树 */
ULONG ABNF_FreeNodeTree(ABNF_GRAMMAR_NODE_S *pstGrammarNode)
{
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;

    if(pstGrammarNode == NULL_PTR)
    {
        return SUCCESS;
    }

    /* 递归释放所有后续节点 */
    pstNode = pstGrammarNode->pstNextNode;
    if(pstNode != NULL_PTR)
    {
        ABNF_FreeNodeTree(pstNode);
    }

    /* 递归释放所有子节点 */
    pstNode = pstGrammarNode->pstChild;
    if(pstNode != NULL_PTR)
    {
        ABNF_FreeNodeTree(pstNode);
    }

    /* 释放本节点 */
    free(pstNode);

    return SUCCESS;
}

/* 匹配一个元素 */
ULONG ABNF_MatchElement(ABNF_MATCH_TARGET_S *pstTarget,
                        ULONG ulOffset,
                        ABNF_ELEMENT_S *pstElement,
                        ULONG *pulSize,
                        ABNF_GRAMMAR_NODE_S **ppstNode)
{
    ULONG ulRet = SUCCESS;
    ULONG ulSize = 0;
    ULONG ulMatchSize = 0;
    ULONG ulRepeat = 0;
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    ABNF_GRAMMAR_NODE_S **ppstTempNode = NULL_PTR;

    /* 每个元素根据次数进行匹配，直到匹配到最大次数或者匹配失败为止 */
    ppstTempNode = &pstNode;
    while (ulRepeat < pstElement->ulMaxRepeat)
    {
        switch (pstElement->eType)
        {
            case ABNF_ELEMENT_DATA_CHAR:
                ulRet = ABNF_MatchCharElement(pstTarget,
                                              ulOffset + ulMatchSize,
                                              pstElement,
                                             &ulSize,
                                              ppstTempNode);
                break;

            case ABNF_ELEMENT_DATA_STRING:
                ulRet = ABNF_MatchStringElement(pstTarget,
                                                ulOffset + ulMatchSize,
                                                pstElement,
                                               &ulSize,
                                                ppstTempNode);
                break;

            case ABNF_ELEMENT_RULE:
                ulRet = ABNF_MatchRuleElement(pstTarget,
                                              ulOffset + ulMatchSize,
                                              pstElement,
                                             &ulSize,
                                              ppstTempNode);
                break;

            case ABNF_ELEMENT_SEQUENCE:
                ulRet = ABNF_MatchSequenceElement(pstTarget,
                                                  ulOffset + ulMatchSize,
                                                  pstElement,
                                                 &ulSize,
                                                  ppstTempNode);
                break;

            case ABNF_ELEMENT_OPTIONAL:
                ulRet = ABNF_MatchOptionalElement(pstTarget,
                                                  ulOffset + ulMatchSize,
                                                  pstElement,
                                                 &ulSize,
                                                  ppstTempNode);
                break;

            default:
                break;
        }

        if (ulRet != SUCCESS)
        {
            break;
        }

        /* 一直找到最后一个节点 */
        while(*ppstTempNode != NULL_PTR)
        {
            ppstTempNode = &(*ppstTempNode)->pstNextNode;
        }

        ulMatchSize += ulSize;
        ulRepeat++;
    }

    /* 匹配次数 */
    if (ulRepeat < pstElement->ulMinRepeat)
    {
        ABNF_FreeNodeTree(pstNode);
        ABNF_FailRecord(ulOffset);
        return FAIL;
    }

    *pulSize = ulMatchSize;
    *ppstNode = pstNode;

    ABNF_SuccessRecord();
    return SUCCESS;
}

/* 匹配一个字符元素 */
ULONG ABNF_MatchCharElement(ABNF_MATCH_TARGET_S *pstTarget,
                            ULONG ulOffset,
                            ABNF_ELEMENT_S *pstElement,
                            ULONG *pulSize,
                            ABNF_GRAMMAR_NODE_S **ppstNode)
{
    UCHAR *pucChar;

    /* 已经匹配到尾部，匹配失败 */
    if (ulOffset >= pstTarget->ulLen)
    {
        return FAIL;
    }

    pucChar = pstTarget->pucText + ulOffset;
    if ((*pucChar >= pstElement->u.stChar.ucMinValue)
      &&(*pucChar <= pstElement->u.stChar.ucMaxValue))
    {
        *pulSize = 1;
        return SUCCESS;
    }

    return FAIL;
}

/* 匹配一个字符串元素 */
ULONG ABNF_MatchStringElement(ABNF_MATCH_TARGET_S *pstTarget,
                              ULONG ulOffset,
                              ABNF_ELEMENT_S *pstElement,
                              ULONG *pulSize,
                              ABNF_GRAMMAR_NODE_S **ppstNode)
{
    ULONG ulRet;
    UCHAR *pucString;

    /* 已经匹配到尾部，匹配失败 */
    if ((ulOffset + strlen(pstElement->u.stString.pucString)) > pstTarget->ulLen)
    {
        return FAIL;
    }

    pucString = pstTarget->pucText + ulOffset;

    if (pstElement->u.stString.bSensitive == FALSE)
    {
        ulRet = strncmp(pucString,
                        pstElement->u.stString.pucString,
                        strlen(pstElement->u.stString.pucString));
    }
    else
    {
        ulRet = strnicmp(pucString,
                         pstElement->u.stString.pucString,
                         strlen(pstElement->u.stString.pucString));
    }

    if (ulRet == SUCCESS)
    {
        *pulSize = (ULONG)strlen(pstElement->u.stString.pucString);
        return SUCCESS;
    }

    return FAIL;
}

/* 匹配一个规则元素 */
ULONG ABNF_MatchRuleElement(ABNF_MATCH_TARGET_S *pstTarget,
                            ULONG ulOffset,
                            ABNF_ELEMENT_S *pstElement,
                            ULONG *pulSize,
                            ABNF_GRAMMAR_NODE_S **ppstNode)
{
    ABNF_ELEMENT_S *pstMatchElement = NULL_PTR;
    ABNF_GRAMMAR_NODE_S *pstNode    = NULL_PTR;
    ULONG ulRet;
    ULONG ulSize;
    UCHAR ucRuleListIndex;
    ULONG ulIndex;

    ucRuleListIndex = pstElement->u.stRule.ucRuleListIndex;
    ulIndex = pstElement->u.stRule.ulIndex;
    pstMatchElement = g_apstAbnfRuleList[ucRuleListIndex]->pstRules[ulIndex].pstElements;

    #if DEBUG_ABNF_CORE
    printf("\r\nMatchRule(%s) start at %d", g_apstAbnfRuleList[ucRuleListIndex]->pstRules[ulIndex].aucName, ulOffset);
    #endif

    pstNode = ABNF_AllocNode(ulOffset, ucRuleListIndex, ulIndex);
    ulRet = ABNF_MatchElement(pstTarget, ulOffset, pstMatchElement, &ulSize, &pstNode->pstChild);
    
    #if DEBUG_ABNF_CORE
    printf("\r\nMatchRule(%s) result is %d", g_apstAbnfRuleList[ucRuleListIndex]->pstRules[ulIndex].aucName, ulRet);
    #endif
    
    if(ulRet != SUCCESS)
    {
        ABNF_FreeNodeTree(pstNode);
        return FAIL;
    }
    else if (ulSize == 0)
    {
        /* 什么也不匹配的规则也是允许的 */
        ABNF_FreeNodeTree(pstNode);
        pstNode = NULL_PTR;
    }
    else
    {
        pstNode->ulSize = ulSize;
    }

    *pulSize  = ulSize;
    *ppstNode = pstNode;

    return SUCCESS;
}

/* 匹配一个组合序列 */
ULONG ABNF_MatchSequenceElement(ABNF_MATCH_TARGET_S *pstTarget,
                                ULONG ulOffset,
                                ABNF_ELEMENT_S *pstElement,
                                ULONG *pulSize,
                                ABNF_GRAMMAR_NODE_S **ppstNode)
{
    ULONG ulRet;
    ULONG ulSize;
    ULONG ulMatchSize = 0;
    ABNF_ELEMENT_S *pstMatchElement = NULL_PTR;
    ABNF_GRAMMAR_NODE_S *pstNode       = NULL_PTR;
    ABNF_GRAMMAR_NODE_S **ppstTempNode = NULL_PTR;

    ppstTempNode = &pstNode;
    pstMatchElement = pstElement->u.pstFirstChild;
    while (pstMatchElement != NULL_PTR)
    {
        ulRet = ABNF_MatchElement(pstTarget,
                                  ulOffset+ulMatchSize,
                                  pstMatchElement,
                                 &ulSize,
                                  ppstTempNode);
        if (ulRet == SUCCESS)
        {
            while (*ppstTempNode != NULL_PTR)
            {
                ppstTempNode = &(*ppstTempNode)->pstNextNode;
            }

            ulMatchSize += ulSize;
            pstMatchElement = pstMatchElement->pNextElement;
            continue;
        }
        else
        {
            ABNF_FreeNodeTree(pstNode);
            return FAIL;
        }
    }

    *ppstNode = pstNode;
    *pulSize = ulMatchSize;

    return SUCCESS;
}

/* 匹配一个可选序列 */
ULONG ABNF_MatchOptionalElement(ABNF_MATCH_TARGET_S *pstTarget,
                                ULONG ulOffset,
                                ABNF_ELEMENT_S *pstElement,
                                ULONG *pulSize,
                                ABNF_GRAMMAR_NODE_S **ppstNode)
{
    ABNF_ELEMENT_S *pstMatchElement;
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    ULONG ulRet;
    ULONG ulSize;

    pstMatchElement = pstElement->u.pstFirstChild;

    while (pstMatchElement != NULL_PTR)
    {
        ulRet = ABNF_MatchElement(pstTarget, ulOffset, pstMatchElement, &ulSize, &pstNode);
        if(ulRet != SUCCESS)
        {
            ABNF_FreeNodeTree(pstNode);
            pstNode = NULL_PTR;
            pstMatchElement = pstMatchElement->pNextElement;
            continue;
        }
        else
        {
            *pulSize = ulSize;
            *ppstNode = pstNode;
            return SUCCESS;
        }
    }

    return FAIL;
}

/* 查找规则在规则表中的序号，可以是被依赖的规则，也可以是应用协议规则 */
ULONG ABNF_FindRuleByName(UCHAR *pucName,
                          UCHAR *pucRuleListIndex,
                          ULONG *pulRuleIndex)
{
    ULONG ulLoop;
    ULONG ulRet;
    UCHAR ucFindRuleIndex;
    UCHAR *pucDepends = NULL_PTR;

    /* 先在核心规则中寻找 */
    ucFindRuleIndex = ABNF_CORE_RULE_LIST_INDEX;
    ulRet = ABNF_GetRuleIndex(ucFindRuleIndex, pucName, pulRuleIndex);
    if (ulRet == SUCCESS)
    {
        *pucRuleListIndex = ucFindRuleIndex;
        return ulRet;
    }

    /* 再在被依赖规则中找 */
    pucDepends = g_apstAbnfRuleList[g_ucAbnfRegistRuleIndex]->aucDepends;
    for (ulLoop = 0; ulLoop < ABNF_MAX_DEPEND_RULE_NUM; ulLoop++)
    {
        ucFindRuleIndex = pucDepends[ulLoop];
        if (ucFindRuleIndex == NULL_UCHAR)
        {
            break;
        }
        
        ulRet = ABNF_GetRuleIndex(ucFindRuleIndex, pucName, pulRuleIndex);
        if (ulRet == SUCCESS)
        {
            *pucRuleListIndex = ucFindRuleIndex;
            return ulRet;
        }
    }
    
    /* 最后在自己的规则中寻找 */
    ucFindRuleIndex = g_ucAbnfRegistRuleIndex;
    ulRet = ABNF_GetRuleIndex(ucFindRuleIndex, pucName, pulRuleIndex);
    if (ulRet == SUCCESS)
    {
        *pucRuleListIndex = ucFindRuleIndex;
        return ulRet;
    }

    /* 都找不到就出错了 */
    printf("\r\nABNF_FindRuleByName \"%s\" failed",pucName);

    return FAIL;
}

/* ABNF描述文本的结构构造函数列表
ABNF_RULE_LIST_S **ppStruct
*/
/*******************************************************************************
rulelist       =  1*( rule / (*c-wsp c-nl) )
*******************************************************************************/
ULONG ABNF_ParseRuleList(ABNF_GRAMMAR_NODE_S *pstNode, UCHAR *pucText, void **ppStruct)
{
    ULONG ulRuleNum = 0;
    ULONG ulLoop = 0;
    ULONG ulRet = SUCCESS;
    ABNF_RULE_S         *pstRule      = NULL_PTR;
    ABNF_RULE_LIST_S    *pstRuleList  = NULL_PTR;
    ABNF_GRAMMAR_NODE_S *pstTempNode  = NULL_PTR;
    ABNF_GRAMMAR_NODE_S *pstTempNode2 = NULL_PTR;

    pstRuleList = (ABNF_RULE_LIST_S *)*ppStruct;

    /* 构造规则列表，先简单认为规则名不重复，不处理=/的情况 */
    pstTempNode = pstNode->pstChild;
    while (pstTempNode != NULL_PTR)
    {
        if (ABNF_RULE_MATCH(pstTempNode, ABNF_RULE_LIST_INDEX, g_astAbnfAppRule, ABNF_RULE_RULE))
        {
            ulRuleNum++;
        }

        pstTempNode = pstTempNode->pstNextNode;
    }
    
    pstRuleList->ulRuleNum = ulRuleNum;
    pstRuleList->pstRules = malloc(ulRuleNum * sizeof(ABNF_RULE_S));
    pstRule = pstRuleList->pstRules;

    /* 初始化规则列表 */
    pstTempNode = pstNode->pstChild;
    while (pstTempNode != NULL_PTR)
    {
        if (!ABNF_RULE_MATCH(pstTempNode, ABNF_RULE_LIST_INDEX, g_astAbnfAppRule, ABNF_RULE_RULE))
        {
            pstTempNode = pstTempNode->pstNextNode;
            continue;
        }

        pstTempNode2 = pstTempNode->pstChild;

        strncpy(pstRule[ulLoop].aucName,
                pucText + pstTempNode2->ulOffset,
                pstTempNode2->ulSize);
        pstRule[ulLoop].aucName[pstTempNode2->ulSize] = '\0';
        pstRule[ulLoop].pstElements    = NULL_PTR;

        ulLoop++;
        pstTempNode = pstTempNode->pstNextNode;
    }

    /* 解析每个规则 */
    pstTempNode = pstNode->pstChild;
    while (pstTempNode != NULL_PTR)
    {
        if (!ABNF_RULE_MATCH(pstTempNode, ABNF_RULE_LIST_INDEX, g_astAbnfAppRule, ABNF_RULE_RULE))
        {
            pstTempNode = pstTempNode->pstNextNode;
            continue;
        }

        ulRet = ABNF_RULE_PARSE_FUNC(ABNF_RULE_RULE)(pstTempNode, pucText, &pstRuleList->pstRules);
        if (ulRet != SUCCESS)
        {
            return ulRet;
        }

        pstTempNode = pstTempNode->pstNextNode;
    }

    return SUCCESS;
}


/*******************************************************************************
rule           =  rulename defined-as elements c-nl
*******************************************************************************/
ULONG ABNF_ParseRule(ABNF_GRAMMAR_NODE_S *pstNode, UCHAR *pucText, void **ppStruct)
{
    ABNF_RULE_S      *pstRule     = NULL_PTR;
    ABNF_GRAMMAR_NODE_S *pstTempNode  = NULL_PTR;
    ULONG ulRuleIndex;
    UCHAR ucRuleListIndex;
    ULONG ulRet = SUCCESS;
    UCHAR aucRuleName[ABNF_MAX_RULE_NAME_LEN + 1];

    pstTempNode = pstNode->pstChild;

    /* 查找构造的规则索引 */
    while (pstTempNode != NULL_PTR)
    {
        if (!ABNF_RULE_MATCH(pstTempNode, ABNF_RULE_LIST_INDEX, g_astAbnfAppRule, ABNF_RULE_RULENAME))
        {
            pstTempNode = pstTempNode->pstNextNode;
            continue;
        }

        strncpy(aucRuleName,
                pucText + pstTempNode->ulOffset,
                pstTempNode->ulSize);
        aucRuleName[pstTempNode->ulSize] = '\0';

        ulRet = ABNF_FindRuleByName(aucRuleName,
                                    &ucRuleListIndex,
                                    &ulRuleIndex);
        if ((ucRuleListIndex != g_ucAbnfRegistRuleIndex) || (ulRet != SUCCESS))
        {
            /* 名字不能和被依赖的相同 */
            return FAIL;
        }
        break;
    }

    /*匹配规则*/
    pstRule = &((ABNF_RULE_S *)*ppStruct)[ulRuleIndex];
    pstTempNode = pstTempNode->pstNextNode;
    while (pstTempNode != NULL_PTR)
    {
        if (!ABNF_RULE_MATCH(pstTempNode, ABNF_RULE_LIST_INDEX, g_astAbnfAppRule, ABNF_RULE_ELEMENTS))
        {
            pstTempNode = pstTempNode->pstNextNode;
            continue;
        }

        ulRet = ABNF_RULE_PARSE_FUNC(ABNF_RULE_ELEMENTS)(pstTempNode, pucText, &pstRule->pstElements);
        if (ulRet != SUCCESS)
        {
            return ulRet;
        }
        break;
    }

    return SUCCESS;
}

/*******************************************************************************
rulename       =  ALPHA *(ALPHA / DIGIT / "-")
*******************************************************************************/
ULONG ABNF_ParseRuleName(ABNF_GRAMMAR_NODE_S *pstNode, UCHAR *pucText, void **ppStruct)
{
    UCHAR ucRuleListIndex;
    ULONG ulRuleIndex;
    ULONG ulRet = SUCCESS;
    UCHAR aucRuleName[ABNF_MAX_RULE_NAME_LEN+1];

    strncpy(aucRuleName,
            pucText + pstNode->ulOffset,
            pstNode->ulSize);
    aucRuleName[pstNode->ulSize] ='\0';

    ulRet = ABNF_FindRuleByName(aucRuleName,
                               &ucRuleListIndex,
                               &ulRuleIndex);
    if (ulRet != SUCCESS)
    {
        return FAIL;
    }

    ABNF_AddRuleElement((ABNF_ELEMENT_S **)ppStruct,
                        1,
                        1,
                        ucRuleListIndex,
                        ulRuleIndex);

    return SUCCESS;
}

/*******************************************************************************
elements       =  alternation *c-wsp
*******************************************************************************/
ULONG ABNF_ParseElements(ABNF_GRAMMAR_NODE_S *pstNode, UCHAR *pucText, void **ppStruct)
{
    ULONG ulRet = SUCCESS;
    ABNF_GRAMMAR_NODE_S *pstTempNode  = NULL_PTR;

    pstTempNode = pstNode->pstChild;
    while(pstTempNode != NULL_PTR)
    {
        if (!ABNF_RULE_MATCH(pstTempNode, ABNF_RULE_LIST_INDEX, g_astAbnfAppRule, ABNF_RULE_ALTERNATION))
        {
            pstTempNode = pstTempNode->pstNextNode;
            continue;
        }

        ulRet = ABNF_RULE_PARSE_FUNC(ABNF_RULE_ALTERNATION)(pstTempNode, pucText, ppStruct);
        if (ulRet != SUCCESS)
        {
            return ulRet;
        }
        break;
    }

    return SUCCESS;
}

/*******************************************************************************
alternation    =  concatenation *(*c-wsp "/" *c-wsp concatenation)
*******************************************************************************/
ULONG ABNF_ParseAlternation(ABNF_GRAMMAR_NODE_S *pstNode, UCHAR *pucText, void **ppStruct)
{
    ULONG ulRet = SUCCESS;
    ABNF_ELEMENT_S  **ppstElement     = NULL_PTR;
    ABNF_GRAMMAR_NODE_S *pstTempNode  = NULL_PTR;
    ULONG ulElementNum = 0;

    pstTempNode = pstNode->pstChild;
    while(pstTempNode != NULL_PTR)
    {
        if (ABNF_RULE_MATCH(pstTempNode, ABNF_RULE_LIST_INDEX, g_astAbnfAppRule, ABNF_RULE_CONCATENATION))
        {
            ulElementNum++;
        }

        pstTempNode = pstTempNode->pstNextNode;
    }

    if (ulElementNum > 1)
    {
        /* 存在多个可选元素才申请可选节点，后续节点挂在该节点的下面 */
        ABNF_AddOptionalElement((ABNF_ELEMENT_S **)ppStruct, 1, 1);
        ppstElement = &((ABNF_ELEMENT_S *)*ppStruct)->u.pstFirstChild;
    }
    else
    {
        ppstElement = (ABNF_ELEMENT_S **)ppStruct;
    }

    pstTempNode = pstNode->pstChild;
    while (pstTempNode != NULL_PTR)
    {
        if (!ABNF_RULE_MATCH(pstTempNode, ABNF_RULE_LIST_INDEX, g_astAbnfAppRule, ABNF_RULE_CONCATENATION))
        {
            pstTempNode = pstTempNode->pstNextNode;
            continue;
        }

        ulRet = ABNF_RULE_PARSE_FUNC(ABNF_RULE_CONCATENATION)(pstTempNode, pucText, ppstElement);
        if (ulRet != SUCCESS)
        {
            return ulRet;
        }

        if (*ppstElement != NULL_PTR)
        {
            ppstElement = &(*ppstElement)->pNextElement;
        }

        pstTempNode = pstTempNode->pstNextNode;
    }

    return SUCCESS;
}

/*******************************************************************************
concatenation  =  repetition *(1*c-wsp repetition)
*******************************************************************************/
ULONG ABNF_ParseConcatenation(ABNF_GRAMMAR_NODE_S *pstNode, UCHAR *pucText, void **ppStruct)
{
    ULONG ulRet = SUCCESS;
    ABNF_ELEMENT_S **ppstElement = NULL_PTR;
    ABNF_GRAMMAR_NODE_S *pstTempNode  = NULL_PTR;
    ULONG ulElementNum = 0;

    pstTempNode = pstNode->pstChild;
    while(pstTempNode != NULL_PTR)
    {
        if (ABNF_RULE_MATCH(pstTempNode, ABNF_RULE_LIST_INDEX, g_astAbnfAppRule, ABNF_RULE_REPETITION))
        {
            ulElementNum++;
        }

        pstTempNode = pstTempNode->pstNextNode;
    }

    if (ulElementNum > 1)
    {
        /* 存在多个可选元素才申请序列节点，后续节点挂在该节点的下面 */
        ABNF_AddSequenceElement((ABNF_ELEMENT_S **)ppStruct, 1, 1);
        ppstElement = &((ABNF_ELEMENT_S *)*ppStruct)->u.pstFirstChild;
    }
    else
    {
        ppstElement = (ABNF_ELEMENT_S **)ppStruct;
    }

    pstTempNode = pstNode->pstChild;
    while (pstTempNode != NULL_PTR)
    {
        if (!ABNF_RULE_MATCH(pstTempNode, ABNF_RULE_LIST_INDEX, g_astAbnfAppRule, ABNF_RULE_REPETITION))
        {
            pstTempNode = pstTempNode->pstNextNode;
            continue;
        }

        ulRet = ABNF_RULE_PARSE_FUNC(ABNF_RULE_REPETITION)(pstTempNode, pucText, ppstElement);
        if (ulRet != SUCCESS)
        {
            return ulRet;
        }
        ppstElement = &(*ppstElement)->pNextElement;
        pstTempNode = pstTempNode->pstNextNode;
    }

    return SUCCESS;
}

/*******************************************************************************
repetition     =  [repeat] element
*******************************************************************************/
ULONG ABNF_ParseRepetition(ABNF_GRAMMAR_NODE_S *pstNode, UCHAR *pucText, void **ppStruct)
{
    ULONG ulRet = SUCCESS;
    ABNF_GRAMMAR_NODE_S *pstTempNode  = NULL_PTR;

    /* 先解析element */
    pstTempNode = pstNode->pstChild;
    while (pstTempNode != NULL_PTR)
    {
        if (!ABNF_RULE_MATCH(pstTempNode, ABNF_RULE_LIST_INDEX, g_astAbnfAppRule, ABNF_RULE_ELEMENT))
        {
            pstTempNode = pstTempNode->pstNextNode;
            continue;
        }

        ulRet = ABNF_RULE_PARSE_FUNC(ABNF_RULE_ELEMENT)(pstTempNode, pucText, ppStruct);
        if (ulRet != SUCCESS)
        {
            return ulRet;
        }
        break;
    }

    /* 再修改element */
    pstTempNode = pstNode->pstChild;
    while (pstTempNode != NULL_PTR)
    {
        if (!ABNF_RULE_MATCH(pstTempNode, ABNF_RULE_LIST_INDEX, g_astAbnfAppRule, ABNF_RULE_REPEAT))
        {
            pstTempNode = pstTempNode->pstNextNode;
            continue;
        }

        ulRet = ABNF_RULE_PARSE_FUNC(ABNF_RULE_REPEAT)(pstTempNode, pucText, ppStruct);
        if (ulRet != SUCCESS)
        {
            return ulRet;
        }
        break;
    }

    return SUCCESS;
}

/*******************************************************************************
repeat         =  1*DIGIT / (*DIGIT "*" *DIGIT)
*******************************************************************************/
ULONG ABNF_ParseRepeat(ABNF_GRAMMAR_NODE_S *pstNode, UCHAR *pucText, void **ppStruct)
{
    ULONG ulMinRepeat = 0;
    ULONG ulMaxRepeat = NULL_ULONG;
    ULONG ulLoop = 0;
    UCHAR *pucChar = NULL_PTR;
    ABNF_ELEMENT_S *pstElement = NULL_PTR;

    /* 查找最小循环值 */
    while (ulLoop < pstNode->ulSize)
    {
        pucChar = pucText + pstNode->ulOffset + ulLoop;
        if (*pucChar >= '0' && *pucChar <= '9')
        {
            ulMinRepeat = ulMinRepeat*10 + (*pucChar - '0');
        }
        else
        {
            break;
        }

        ulLoop++;
    }

    if (ulLoop == pstNode->ulSize)
    {
        /* 没有携带*号 */
        ulMaxRepeat = ulMinRepeat;
    }
    else if (*pucChar == '*')
    {
        /* 跳过*号 */
        ulLoop++;
        if(ulLoop == pstNode->ulSize)
        {
            /* 没有最大值 */
            ulMaxRepeat = NULL_ULONG;
        }
        else
        {
            ulMaxRepeat = 0;
            while (ulLoop < pstNode->ulSize)
            {
                pucChar = pucText + pstNode->ulOffset + ulLoop;
                if (*pucChar >= '0' && *pucChar <= '9')
                {
                    ulMaxRepeat = ulMaxRepeat*10 + (*pucChar - '0');
                }
                else
                {
                    break;
                }

                ulLoop++;
            }
        }
    }
    else
    {
        return FAIL;
    }

    pstElement = (ABNF_ELEMENT_S *)*ppStruct;
    pstElement->ulMinRepeat = ulMinRepeat;
    pstElement->ulMaxRepeat = ulMaxRepeat;

    return SUCCESS;
}

/*******************************************************************************
element        =  rulename / group / option / char-val / num-val / prose-val
*******************************************************************************/
ULONG ABNF_ParseElement(ABNF_GRAMMAR_NODE_S *pstNode, UCHAR *pucText, void **ppStruct)
{
    ULONG ulRet = SUCCESS;
    ABNF_GRAMMAR_NODE_S *pstTempNode = NULL_PTR;

    /* 先解析element */
    pstTempNode = pstNode->pstChild;
    while (pstTempNode != NULL_PTR)
    {
        if (ABNF_RULE_MATCH(pstTempNode, ABNF_RULE_LIST_INDEX, g_astAbnfAppRule, ABNF_RULE_RULENAME))
        {
            ulRet = ABNF_RULE_PARSE_FUNC(ABNF_RULE_RULENAME)(pstTempNode, pucText, ppStruct);
        }
        else if (ABNF_RULE_MATCH(pstTempNode, ABNF_RULE_LIST_INDEX, g_astAbnfAppRule, ABNF_RULE_GROUP))
        {
            ulRet = ABNF_RULE_PARSE_FUNC(ABNF_RULE_GROUP)(pstTempNode, pucText, ppStruct);
        }
        else if (ABNF_RULE_MATCH(pstTempNode, ABNF_RULE_LIST_INDEX, g_astAbnfAppRule, ABNF_RULE_OPTION))
        {
            ulRet = ABNF_RULE_PARSE_FUNC(ABNF_RULE_OPTION)(pstTempNode, pucText, ppStruct);
        }
        else if (ABNF_RULE_MATCH(pstTempNode, ABNF_RULE_LIST_INDEX, g_astAbnfAppRule, ABNF_RULE_CHAR_VAL))
        {
            ulRet = ABNF_RULE_PARSE_FUNC(ABNF_RULE_CHAR_VAL)(pstTempNode, pucText, ppStruct);
        }
        else if (ABNF_RULE_MATCH(pstTempNode, ABNF_RULE_LIST_INDEX, g_astAbnfAppRule, ABNF_RULE_NUM_VAL))
        {
            ulRet = ABNF_RULE_PARSE_FUNC(ABNF_RULE_NUM_VAL)(pstTempNode, pucText, ppStruct);
        }
        else if (ABNF_RULE_MATCH(pstTempNode, ABNF_RULE_LIST_INDEX, g_astAbnfAppRule, ABNF_RULE_PROSE_VAL))
        {
            //ulRet = ABNF_RULE_PARSE_FUNC(ABNF_RULE_PROSE_VAL)(pstTempNode, pucText,  ppStruct);
        }
        else
        {
            pstTempNode = pstTempNode->pstNextNode;
            continue;
        }

        if (ulRet != SUCCESS)
        {
            return ulRet;
        }
        break;
    }

    return SUCCESS;
}

/*******************************************************************************
group          =  "(" *c-wsp alternation *c-wsp ")"
*******************************************************************************/
ULONG ABNF_ParseGroup(ABNF_GRAMMAR_NODE_S *pstNode, UCHAR *pucText, void **ppStruct)
{
    ULONG ulRet = SUCCESS;
    ABNF_GRAMMAR_NODE_S *pstTempNode  = NULL_PTR;

    /* 先解析element */
    pstTempNode = pstNode->pstChild;
    while (pstTempNode != NULL_PTR)
    {
        if (!ABNF_RULE_MATCH(pstTempNode, ABNF_RULE_LIST_INDEX, g_astAbnfAppRule, ABNF_RULE_ALTERNATION))
        {
            pstTempNode = pstTempNode->pstNextNode;
            continue;
        }

        ulRet = ABNF_RULE_PARSE_FUNC(ABNF_RULE_ALTERNATION)(pstTempNode, pucText, ppStruct);
        if (ulRet != SUCCESS)
        {
            return ulRet;
        }
        break;
    }

    return SUCCESS;
}

/*******************************************************************************
option         =  "[" *c-wsp alternation *c-wsp "]"
*******************************************************************************/
ULONG ABNF_ParseOption(ABNF_GRAMMAR_NODE_S *pstNode, UCHAR *pucText, void **ppStruct)
{
    ULONG ulRet = SUCCESS;
    ABNF_GRAMMAR_NODE_S *pstTempNode  = NULL_PTR;
    ABNF_ELEMENT_S *pstElement = NULL_PTR;

    /* 先解析element */
    pstTempNode = pstNode->pstChild;
    while (pstTempNode != NULL_PTR)
    {
        if (!ABNF_RULE_MATCH(pstTempNode, ABNF_RULE_LIST_INDEX, g_astAbnfAppRule, ABNF_RULE_ALTERNATION))
        {
            pstTempNode = pstTempNode->pstNextNode;
            continue;
        }

        ulRet = ABNF_RULE_PARSE_FUNC(ABNF_RULE_ALTERNATION)(pstTempNode, pucText, ppStruct);
        if (ulRet != SUCCESS)
        {
            return ulRet;
        }
        break;
    }

    pstElement = (ABNF_ELEMENT_S *)*ppStruct;
    pstElement->ulMinRepeat = 0;
    pstElement->ulMaxRepeat = 1;

    return SUCCESS;
}

/*******************************************************************************
char-val       =  DQUOTE *(%x20-21 / %x23-7E) DQUOTE
*******************************************************************************/
ULONG ABNF_ParseCharVal(ABNF_GRAMMAR_NODE_S *pstNode, UCHAR *pucText, void **ppStruct)
{
    UCHAR aucString[100]={'\0'};

    /* 需要去掉前后的"号 */
    strncpy(aucString,
            pucText + pstNode->ulOffset + 1,
            pstNode->ulSize - 2);
    aucString[pstNode->ulSize - 2] = '\0';

    ABNF_AddStringElement((ABNF_ELEMENT_S **)ppStruct,
                          1,
                          1,
                          FALSE,
                          aucString);

    return SUCCESS;
}

/*******************************************************************************
num-val        =  "%" (bin-val / dec-val / hex-val)
*******************************************************************************/
ULONG ABNF_ParseNumVal(ABNF_GRAMMAR_NODE_S *pstNode, UCHAR *pucText, void **ppStruct)
{
    ULONG ulRet = SUCCESS;
    ABNF_GRAMMAR_NODE_S *pstTempNode = NULL_PTR;

    /* 先解析element */
    pstTempNode = pstNode->pstChild;
    while (pstTempNode != NULL_PTR)
    {
        if (ABNF_RULE_MATCH(pstTempNode, ABNF_RULE_LIST_INDEX, g_astAbnfAppRule, ABNF_RULE_BIN_VAL))
        {
            ulRet = ABNF_RULE_PARSE_FUNC(ABNF_RULE_BIN_VAL)(pstTempNode, pucText, ppStruct);
        }
        else if (ABNF_RULE_MATCH(pstTempNode, ABNF_RULE_LIST_INDEX, g_astAbnfAppRule, ABNF_RULE_DEC_VAL))
        {
            ulRet = ABNF_RULE_PARSE_FUNC(ABNF_RULE_DEC_VAL)(pstTempNode, pucText, ppStruct);
        }
        else if (ABNF_RULE_MATCH(pstTempNode, ABNF_RULE_LIST_INDEX, g_astAbnfAppRule, ABNF_RULE_HEX_VAL))
        {
            ulRet = ABNF_RULE_PARSE_FUNC(ABNF_RULE_HEX_VAL)(pstTempNode, pucText, ppStruct);
        }
        else
        {
            pstTempNode = pstTempNode->pstNextNode;
            continue;
        }

        if (ulRet != SUCCESS)
        {
            return ulRet;
        }

        break;
    }

    return SUCCESS;
}

/*******************************************************************************
bin-val        =  "b" 1*BIT [ 1*("." 1*BIT) / ("-" 1*BIT) ]
*******************************************************************************/
ULONG ABNF_ParseBinVal(ABNF_GRAMMAR_NODE_S *pstNode, UCHAR *pucText, void **ppStruct)
{
    UCHAR  ucHeadChar = 0;
    UCHAR  ucEndChar = 0;
    UCHAR  ucStringLen = 0;
    UCHAR *pucChar;
    #define MAX_STRING_LEN 50
    UCHAR  aucString[MAX_STRING_LEN + 1];

    /*过滤b字符*/
    pucChar = pucText + pstNode->ulOffset + 1;

    while ((*pucChar >= '0') && (*pucChar <= '1'))
    {
        ucHeadChar = ucHeadChar*2 + (*pucChar-'0');
        pucChar++;
    }

    if (*pucChar == '-')
    {
        pucChar++;
        while ((*pucChar >= '0') && (*pucChar <= '1'))
        {
            ucEndChar = ucEndChar*2 + (*pucChar-'0');
            pucChar++;
        }

        ABNF_AddCharElement((ABNF_ELEMENT_S **)ppStruct,
                            1,
                            1,
                            ucHeadChar,
                            ucEndChar);
    }
    else if (*pucChar == '.')
    {
        aucString[0] = ucHeadChar;
        ucStringLen = 1;

        while (*pucChar == '.')
        {
            ucStringLen++;
            if (ucStringLen > MAX_STRING_LEN)
            {
                return FAIL;
            }

            pucChar++;
            ucEndChar = 0;
            while ((*pucChar >= '0') && (*pucChar <= '1'))
            {
                ucEndChar = ucEndChar*2 + (*pucChar-'0');
                pucChar++;
            }

            aucString[ucStringLen -1] = ucEndChar;
        }

        aucString[ucStringLen] = '\0';
        ABNF_AddStringElement((ABNF_ELEMENT_S **)ppStruct,
                              1,
                              1,
                              TRUE,
                              aucString);
    }
    else
    {
        ABNF_AddCharElement((ABNF_ELEMENT_S **)ppStruct,
                            1,
                            1,
                            ucHeadChar,
                            ucHeadChar);
    }


    return SUCCESS;
}

/*******************************************************************************
dec-val        =  "d" 1*DIGIT [ 1*("." 1*DIGIT) / ("-" 1*DIGIT) ]
*******************************************************************************/
ULONG ABNF_ParseDecVal(ABNF_GRAMMAR_NODE_S *pstNode, UCHAR *pucText, void **ppStruct)
{
    UCHAR  ucHeadChar = 0;
    UCHAR  ucEndChar = 0;
    UCHAR  ucStringLen = 0;
    UCHAR *pucChar;
    #define MAX_STRING_LEN 50
    UCHAR  aucString[MAX_STRING_LEN + 1];

    /*过滤d字符*/
    pucChar = pucText + pstNode->ulOffset + 1;

    while ((*pucChar >= '0') && (*pucChar <= '9'))
    {
        ucHeadChar = ucHeadChar*10 + (*pucChar-'0');
        pucChar++;
    }

    if (*pucChar == '-')
    {
        pucChar++;
        while ((*pucChar >= '0') && (*pucChar <= '9'))
        {
            ucEndChar = ucEndChar*10 + (*pucChar-'0');
            pucChar++;
        }

        ABNF_AddCharElement((ABNF_ELEMENT_S **)ppStruct,
                            1,
                            1,
                            ucHeadChar,
                            ucEndChar);
    }
    else if (*pucChar == '.')
    {
        aucString[0] = ucHeadChar;
        ucStringLen = 1;

        while (*pucChar == '.')
        {
            ucStringLen++;
            if (ucStringLen > MAX_STRING_LEN)
            {
                return FAIL;
            }

            pucChar++;
            ucEndChar = 0;
            while ((*pucChar >= '0') && (*pucChar <= '9'))
            {
                ucEndChar = ucEndChar*10 + (*pucChar-'0');
                pucChar++;
            }

            aucString[ucStringLen -1] = ucEndChar;
        }

        aucString[ucStringLen] = '\0';
        ABNF_AddStringElement((ABNF_ELEMENT_S **)ppStruct,
                              1,
                              1,
                              TRUE,
                              aucString);
    }
    else
    {
        ABNF_AddCharElement((ABNF_ELEMENT_S **)ppStruct,
                            1,
                            1,
                            ucHeadChar,
                            ucHeadChar);
    }


    return SUCCESS;
}

/*******************************************************************************
hex-val        =  "x" 1*HEXDIG [ 1*("." 1*HEXDIG) / ("-" 1*HEXDIG) ]
*******************************************************************************/
ULONG ABNF_ParseHexVal(ABNF_GRAMMAR_NODE_S *pstNode, UCHAR *pucText, void **ppStruct)
{
    UCHAR  ucHeadChar = 0;
    UCHAR  ucEndChar = 0;
    UCHAR  ucValue;
    UCHAR  ucStringLen = 0;
    UCHAR *pucChar;
    #define MAX_STRING_LEN 50
    UCHAR  aucString[MAX_STRING_LEN + 1];

    /*过滤x字符*/
    pucChar = pucText + pstNode->ulOffset + 1;

    while (((*pucChar >= '0') && (*pucChar <= '9'))
         ||((*pucChar >= 'a') && (*pucChar <= 'f'))
         ||((*pucChar >= 'A') && (*pucChar <= 'F')))
    {
        if((*pucChar >= '0') && (*pucChar <= '9'))
        {
            ucValue = *pucChar - '0';
        }
        else if((*pucChar >= 'a') && (*pucChar <= 'f'))
        {
            ucValue = *pucChar - 'a' + 10;
        }
        else
        {
            ucValue = *pucChar - 'A' + 10;
        }

        ucHeadChar = ucHeadChar*16 + ucValue;
        pucChar++;
    }

    if (*pucChar == '-')
    {
        pucChar++;
        while (((*pucChar >= '0') && (*pucChar <= '9'))
             ||((*pucChar >= 'a') && (*pucChar <= 'f'))
             ||((*pucChar >= 'A') && (*pucChar <= 'F')))
        {
            if((*pucChar >= '0') && (*pucChar <= '9'))
            {
                ucValue = *pucChar - '0';
            }
            else if((*pucChar >= 'a') && (*pucChar <= 'f'))
            {
                ucValue = *pucChar - 'a' + 10;
            }
            else
            {
                ucValue = *pucChar - 'A' + 10;
            }

            ucEndChar = ucEndChar*16 + ucValue;
            pucChar++;
        }

        ABNF_AddCharElement((ABNF_ELEMENT_S **)ppStruct,
                            1,
                            1,
                            ucHeadChar,
                            ucEndChar);
    }
    else if (*pucChar == '.')
    {
        aucString[0] = ucHeadChar;
        ucStringLen = 1;

        while (*pucChar == '.')
        {
            ucStringLen++;
            if (ucStringLen > MAX_STRING_LEN)
            {
                return FAIL;
            }

            pucChar++;
            ucEndChar = 0;
            while (((*pucChar >= '0') && (*pucChar <= '9'))
                 ||((*pucChar >= 'a') && (*pucChar <= 'f'))
                 ||((*pucChar >= 'A') && (*pucChar <= 'F')))
            {
                if((*pucChar >= '0') && (*pucChar <= '9'))
                {
                    ucValue = *pucChar - '0';
                }
                else if((*pucChar >= 'a') && (*pucChar <= 'f'))
                {
                    ucValue = *pucChar - 'a' + 10;
                }
                else
                {
                    ucValue = *pucChar - 'A' + 10;
                }

                ucEndChar = ucEndChar*16 + ucValue;
                pucChar++;
            }

            aucString[ucStringLen -1] = ucEndChar;
        }

        aucString[ucStringLen] = '\0';
        ABNF_AddStringElement((ABNF_ELEMENT_S **)ppStruct,
                              1,
                              1,
                              TRUE,
                              aucString);
    }
    else
    {
        ABNF_AddCharElement((ABNF_ELEMENT_S **)ppStruct,
                            1,
                            1,
                            ucHeadChar,
                            ucHeadChar);
    }


    return SUCCESS;
}

/* 匹配成功记录 */
VOID ABNF_SuccessRecord()
{
    /* 一旦匹配成功，就将失败位置清空 */
    g_ulAbnfFailPos = NULL_ULONG;
    return;
}

/* 匹配失败记录 */
VOID ABNF_FailRecord(ULONG ulPos)
{
    if (g_ulAbnfFailPos == NULL_ULONG)
    {
        /* 如果是首次失败，这将该位置置为失败的位置 */
        g_ulAbnfFailPos = ulPos;
    }

    return;
}
