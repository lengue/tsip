/*******************************************************************************
功能    : abnf模块内部使用的结构
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/
typedef struct tagABNF_MATCH_TARGET_S
{
    UCHAR            *pucText;
    ULONG             ulLen;
    ABNF_RULE_LIST_S *pstRuleList;
}ABNF_MATCH_TARGET_S;