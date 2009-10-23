/*******************************************************************************
功能    : abnf模块对外提供的常量
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/
/* 每条规则的基本组成元素的类型 */
typedef enum tagABNF_ELEMENT_TYPE_E
{
    ABNF_ELEMENT_DATA_CHAR,   /* 基本字符数据 */
    ABNF_ELEMENT_DATA_STRING, /* 基本字符串数据 */
    ABNF_ELEMENT_RULE,        /* 基本规则 */
    ABNF_ELEMENT_SEQUENCE,    /* 顺序组合 */
    ABNF_ELEMENT_OPTIONAL,    /* 可选组合 */
    ABNF_ELEMENT_BUTT
}ABNF_ELEMENT_TYPE_E;

/* 规则名字最长值 */
#define ABNF_MAX_RULE_NAME_LEN 31

#define ABNF_APP_RULE_HEADER \
    UCHAR aucName[ABNF_MAX_RULE_NAME_LEN + 1];   /* ABNF规则名字 */\
    ULONG ulRuleIndex;                           /* ABNF解析规则索引 */