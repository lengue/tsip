/*******************************************************************************
功能    : abnf模块对外提供的常量
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/
/* 规则名字最长值 */
#define ABNF_MAX_RULE_NAME_LEN 31

#define ABNF_APP_RULE_HEADER \
    UCHAR aucName[ABNF_MAX_RULE_NAME_LEN + 1];   /* ABNF规则名字 */\
    ULONG ulRuleIndex;                           /* ABNF解析规则索引 */

/* 检查是否匹配应用规则 */
#define ABNF_RULE_MATCH(pstNode, pstAppRule, ulAppRuleIndex) \
 ((pstNode->bCoreFlag == FALSE) \
&&(pstNode->ulIndex == pstAppRule[ulAppRuleIndex].ulRuleIndex))