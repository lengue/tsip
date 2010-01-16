/*******************************************************************************
����    : ABNFģ��ʹ�õ�ȫ�ֱ���
������  : �ƴ�ƽ
��������: 200.01.10
�޸ļ�¼:
*******************************************************************************/
/* �ⲿģ��ͷ�ļ� */
#include "common\common.h"

/* ��ģ������ṩ�ĳ����ͽṹͷ�ļ� */
#include "abnf\abnf_const.h"
#include "abnf\abnf_type.h"

/* ��ģ���ڲ�ʹ�õĳ����ͽṹͷ�ļ� */
#include "abnf_const.inc"
#include "abnf_type.inc"

/* ��ģ���ڲ���������ͷ�ļ� */
#include "abnf.inc"

/* ��ģ��ȫ�ֱ�������ͷ�ļ� */
#include "abnf_var.inc"


/* ��ʼ�����Ĺ���� */
ABNF_RULE_S g_astCoreRuleTbl[ABNF_CORE_RULE_BUTT] =
{
    {"ALPHA",   NULL_ULONG, NULL_PTR},
    {"BIT",     NULL_ULONG, NULL_PTR},
    {"CHAR",    NULL_ULONG, NULL_PTR},
    {"CR",      NULL_ULONG, NULL_PTR},
    {"CRLF",    NULL_ULONG, NULL_PTR},
    {"CTL",     NULL_ULONG, NULL_PTR},
    {"DIGIT",   NULL_ULONG, NULL_PTR},
    {"DQUOTE",  NULL_ULONG, NULL_PTR},
    {"HEXDIG",  NULL_ULONG, NULL_PTR},
    {"HTAB",    NULL_ULONG, NULL_PTR},
    {"LF",      NULL_ULONG, NULL_PTR},
    {"LWSP",    NULL_ULONG, NULL_PTR},
    {"OCTET",   NULL_ULONG, NULL_PTR},
    {"SP",      NULL_ULONG, NULL_PTR},
    {"VCHAR",   NULL_ULONG, NULL_PTR},
    {"WSP",     NULL_ULONG, NULL_PTR}
};

ABNF_RULE_LIST_S g_stCoreRules = {ABNF_CORE_RULE_BUTT, g_astCoreRuleTbl};

/* ��ʼ��ABNF����� */
ABNF_RULE_S g_astAbnfRuleTbl[ABNF_RULE_BUTT] =
{
    {"rulelist"     , ABNF_RULE_RULELIST,       NULL_PTR},
    {"rule"         , ABNF_RULE_RULE,           NULL_PTR},
    {"rulename"     , ABNF_RULE_RULENAME,       NULL_PTR},
    {"defined-as"   , ABNF_RULE_DEFINED_AS,     NULL_PTR},
    {"elements"     , ABNF_RULE_ELEMENTS,       NULL_PTR},
    {"c-wsp"        , ABNF_RULE_C_WSP,          NULL_PTR},
    {"c-nl"         , ABNF_RULE_C_NL,           NULL_PTR},
    {"comment"      , ABNF_RULE_COMMENT,        NULL_PTR},
    {"alternation"  , ABNF_RULE_ALTERNATION,    NULL_PTR},
    {"concatenation", ABNF_RULE_CONCATENATION,  NULL_PTR},
    {"repetition"   , ABNF_RULE_REPETITION,     NULL_PTR},
    {"repeat"       , ABNF_RULE_REPEAT,         NULL_PTR},
    {"element"      , ABNF_RULE_ELEMENT,        NULL_PTR},
    {"group"        , ABNF_RULE_GROUP,          NULL_PTR},
    {"option"       , ABNF_RULE_OPTION,         NULL_PTR},
    {"char-val"     , ABNF_RULE_CHAR_VAL,       NULL_PTR},
    {"num-val"      , ABNF_RULE_NUM_VAL,        NULL_PTR},
    {"bin-val "     , ABNF_RULE_BIN_VAL,        NULL_PTR},
    {"dec-val"      , ABNF_RULE_DEC_VAL,        NULL_PTR},
    {"hex-val"      , ABNF_RULE_HEX_VAL,        NULL_PTR},
    {"prose-val"    , ABNF_RULE_PROSE_VAL,      NULL_PTR}
};

ABNF_RULE_LIST_S g_stAbnfRules = {ABNF_RULE_BUTT, g_astAbnfRuleTbl};

/* ABNF��Ӧ�ù����ƥ�����һһ��Ӧ����������������Ҫ���չ�������ƥ��ABNF���� */
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

/* �ж�ģ���Ƿ��ʼ����� */
BOOL        g_bAbnfInit = FALSE;

ABNF_RULE_LIST_S *g_pstAbnfBuiltRuleList;

/* ջ�� */
ULONG        g_ulAbnfFailPos = NULL_ULONG;