/*******************************************************************************
功能    : ABNF模块使用的全局变量
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/
/* 外部模块头文件 */
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


/* 初始化核心规则表 */
ABNF_RULE_S g_astCoreRuleTbl[ABNF_CORE_RULE_BUTT] =
{
    {"ALPHA",  NULL_PTR},
    {"BIT",    NULL_PTR},
    {"CHAR",   NULL_PTR},
    {"CR",     NULL_PTR},
    {"CRLF",   NULL_PTR},
    {"CTL",    NULL_PTR},
    {"DIGIT",  NULL_PTR},
    {"DQUOTE", NULL_PTR},
    {"HEXDIG", NULL_PTR},
    {"HTAB",   NULL_PTR},
    {"LF",     NULL_PTR},
    {"LWSP",   NULL_PTR},
    {"OCTET",  NULL_PTR},
    {"SP",     NULL_PTR},
    {"VCHAR",  NULL_PTR},
    {"WSP",    NULL_PTR}
};

ABNF_RULE_LIST_S g_stCoreRules = {ABNF_CORE_RULE_BUTT, g_astCoreRuleTbl};

/* 初始化ABNF规则表 */
ABNF_RULE_S g_astAbnfRuleTbl[ABNF_RULE_BUTT] =
{
    {"rulelist"     , NULL_PTR},
    {"rule"         , NULL_PTR},
    {"rulename"     , NULL_PTR},
    {"defined-as"   , NULL_PTR},
    {"elements"     , NULL_PTR},
    {"c-wsp"        , NULL_PTR},
    {"c-nl"         , NULL_PTR},
    {"comment"      , NULL_PTR},
    {"alternation"  , NULL_PTR},
    {"concatenation", NULL_PTR},
    {"repetition"   , NULL_PTR},
    {"repeat"       , NULL_PTR},
    {"element"      , NULL_PTR},
    {"group"        , NULL_PTR},
    {"option"       , NULL_PTR},
    {"char-val"     , NULL_PTR},
    {"num-val"      , NULL_PTR},
    {"bin-val "     , NULL_PTR},
    {"dec-val"      , NULL_PTR},
    {"hex-val"      , NULL_PTR},
    {"prose-val"    , NULL_PTR}
};

ABNF_RULE_LIST_S g_stAbnfRules = {ABNF_RULE_BUTT, g_astAbnfRuleTbl};

/* ABNF的应用规则和匹配规则一一对应，但是其他规则需要按照规则名字匹配ABNF规则 */
ABNF_APP_RULE_S g_astAbnfAppRule[ABNF_RULE_BUTT] =
{
    {"rulelist"     , ABNF_RULE_RULELIST,       ABNF_ParseRuleList},
    {"rule"         , ABNF_RULE_RULE,           ABNF_ParseRule},
    {"rulename"     , ABNF_RULE_RULENAME,       ABNF_ParseRuleName},
    {"defined-as"   , ABNF_RULE_DEFINED_AS,     NULL_PTR},
    {"elements"     , ABNF_RULE_ELEMENTS,       ABNF_ParseElements},
    {"c-wsp"        , ABNF_RULE_C_WSP,          NULL_PTR},
    {"c-nl"         , ABNF_RULE_C_NL,           NULL_PTR},
    {"comment"      , ABNF_RULE_COMMENT,        NULL_PTR},
    {"alternation"  , ABNF_RULE_ALTERNATION,    ABNF_ParseAlternation},
    {"concatenation", ABNF_RULE_CONCATENATION,  ABNF_ParseConcatenation},
    {"repetition"   , ABNF_RULE_REPETITION,     ABNF_ParseRepetition},
    {"repeat"       , ABNF_RULE_REPEAT,         ABNF_ParseRepeat},
    {"element"      , ABNF_RULE_ELEMENT,        ABNF_ParseElement},
    {"group"        , ABNF_RULE_GROUP,          ABNF_ParseGroup},
    {"option"       , ABNF_RULE_OPTION,         ABNF_ParseOption},
    {"char-val"     , ABNF_RULE_CHAR_VAL,       ABNF_ParseCharVal},
    {"num-val"      , ABNF_RULE_NUM_VAL,        ABNF_ParseNumVal},
    {"bin-val "     , ABNF_RULE_BIN_VAL,        ABNF_ParseBinVal},
    {"dec-val"      , ABNF_RULE_DEC_VAL,        ABNF_ParseDecVal},
    {"hex-val"      , ABNF_RULE_HEX_VAL,        ABNF_ParseHexVal},
    {"prose-val"    , ABNF_RULE_PROSE_VAL,      NULL_PTR}
};

/* 判断模块是否初始化标记 */
BOOL        g_bAbnfInit = FALSE;

ABNF_RULE_LIST_S *g_pstAbnfBuiltRuleList;

/* 栈顶 */
ULONG        g_ulAbnfFailPos = NULL_ULONG;