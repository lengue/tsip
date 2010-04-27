/*******************************************************************************
功能    : abnf模块内部使用的结构
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/
/* 元素结构 */
typedef struct tagABNF_ELEMENT_S
{
    /* 重复次数 */
    ULONG ulMinRepeat;
    ULONG ulMaxRepeat;

    /* 用于顺序或者可选组合索引 */
    struct tagABNF_ELEMENT_S *pNextElement;

    /* 标识类型 */
    ABNF_ELEMENT_TYPE_E eType;
    union
    {
        /* 单个字符数据 */
        struct
        {
            UCHAR ucMinValue; /* 最小值 */
            UCHAR ucMaxValue; /* 最大值，单个字符时等于最小值 */
        }stChar;

        /* 标记索引 */
        struct
        {
            BOOL  bCoreFlag;  /* 标识是否是核心规则 */
            ULONG ulIndex;    /* 规则索引 */
        }stRule;

        /* 基本字符串数据 */
        struct
        {
            BOOL   bSensitive; /* 是否大小写敏感 */
            UCHAR *pucString;  /* 字符串指针 */
        }stString;

        /* 组合的第一个子元素，适用于序列和组合 */
        struct tagABNF_ELEMENT_S *pstFirstChild;
    }u;
}ABNF_ELEMENT_S;

/* 单条规则结构 */
typedef struct tagABNF_RULE_S
{
    UCHAR             aucName[ABNF_MAX_RULE_NAME_LEN + 1];   /* 规则名字 */
    ABNF_ELEMENT_S   *pstElements;
}ABNF_RULE_S;

/* 规则结构 */
typedef struct tagABNF_RULE_LIST_S
{
    ULONG        ulRuleNum; /* 规则数目 */
    ABNF_RULE_S *pstRules;  /* 规则数组 */
}ABNF_RULE_LIST_S;

typedef ULONG (*ABNF_PARSE_FUNC)(ABNF_GRAMMAR_NODE_S *pstNode, UCHAR *pucText, void **ppStruct);

typedef struct tagABNF_APP_RULE_S
{
    ABNF_APP_RULE_HEADER
    ABNF_PARSE_FUNC pfnParseFunc;
}ABNF_APP_RULE_S;

typedef struct tagABNF_MATCH_TARGET_S
{
    UCHAR            *pucText;
    ULONG             ulLen;
    ABNF_RULE_LIST_S *pstRuleList;
}ABNF_MATCH_TARGET_S;