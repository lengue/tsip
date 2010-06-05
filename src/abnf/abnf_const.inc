/*******************************************************************************
����    : abnfģ���ڲ�ʹ�õĳ���
������  : �ƴ�ƽ
��������: 200.01.10
�޸ļ�¼:
*******************************************************************************/

/* ע�����������Ŀ */
#define ABNF_MAX_RULE_NUM 10

/* ���Ĺ����ABNF���������̶�Ϊ0��1 */
#define ABNF_CORE_RULE_LIST_INDEX 0
#define ABNF_RULE_LIST_INDEX      1

/* ÿ������Ļ������Ԫ�ص����� */
typedef enum tagABNF_ELEMENT_TYPE_E
{
    ABNF_ELEMENT_DATA_CHAR,   /* �����ַ����� */
    ABNF_ELEMENT_DATA_STRING, /* �����ַ������� */
    ABNF_ELEMENT_RULE,        /* �������� */
    ABNF_ELEMENT_SEQUENCE,    /* ˳����� */
    ABNF_ELEMENT_OPTIONAL,    /* ��ѡ��� */
    ABNF_ELEMENT_BUTT
}ABNF_ELEMENT_TYPE_E;

/* ABNF���Ĺ���ö�� */
typedef enum tagABNF_CORE_RULE_E
{
    ABNF_CORE_RULE_ALPHA,     /* 0 */
    ABNF_CORE_RULE_BIT,       /* 1 */
    ABNF_CORE_RULE_CHAR,      /* 2 */
    ABNF_CORE_RULE_CR,        /* 3 */
    ABNF_CORE_RULE_CRLF,      /* 4 */
    ABNF_CORE_RULE_CTL,       /* 5 */
    ABNF_CORE_RULE_DIGIT,     /* 6 */
    ABNF_CORE_RULE_DQUOTE,    /* 7 */
    ABNF_CORE_RULE_HEXDIG,    /* 8 */
    ABNF_CORE_RULE_HTAB,      /* 9 */
    ABNF_CORE_RULE_LF,        /* 10 */
    ABNF_CORE_RULE_LWSP,      /* 11 */
    ABNF_CORE_RULE_OCTET,     /* 12 */
    ABNF_CORE_RULE_SP,        /* 13 */
    ABNF_CORE_RULE_VCHAR,     /* 14 */
    ABNF_CORE_RULE_WSP,       /* 15 */
    ABNF_CORE_RULE_BUTT
}ABNF_CORE_RULE_E;

/* ABNF�ı�����ö�� */
typedef enum tagABNF_RULE_E
{
    ABNF_RULE_RULELIST,        /* 0 */
    ABNF_RULE_RULE,            /* 1 */
    ABNF_RULE_RULENAME,        /* 2 */
    ABNF_RULE_DEFINED_AS,      /* 3 */
    ABNF_RULE_ELEMENTS,        /* 4 */
    ABNF_RULE_C_WSP,           /* 5 */
    ABNF_RULE_C_NL,            /* 6 */
    ABNF_RULE_COMMENT,         /* 7 */
    ABNF_RULE_ALTERNATION,     /* 8 */
    ABNF_RULE_CONCATENATION,   /* 9 */
    ABNF_RULE_REPETITION,      /* 10 */
    ABNF_RULE_REPEAT,          /* 11 */
    ABNF_RULE_ELEMENT,         /* 12 */
    ABNF_RULE_GROUP,           /* 13 */
    ABNF_RULE_OPTION,          /* 14 */
    ABNF_RULE_CHAR_VAL,        /* 15 */
    ABNF_RULE_NUM_VAL,         /* 16 */
    ABNF_RULE_BIN_VAL,         /* 17 */
    ABNF_RULE_DEC_VAL,         /* 18 */
    ABNF_RULE_HEX_VAL,         /* 19 */
    ABNF_RULE_PROSE_VAL,       /* 20 */
    ABNF_RULE_BUTT
}ABNF_RULE_E;

#define ABNF_RULE_PARSE_FUNC(ulAppRuleIndex) g_astAbnfAppRule[ulAppRuleIndex].pfnParseFunc