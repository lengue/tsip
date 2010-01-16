/*******************************************************************************
功能    : .c对应的inc文件
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/
extern ABNF_RULE_S g_astCoreRuleTbl[ABNF_CORE_RULE_BUTT];
extern ABNF_RULE_LIST_S g_stCoreRules;
extern ABNF_RULE_S g_astAbnfRuleTbl[ABNF_RULE_BUTT];
extern ABNF_RULE_LIST_S g_stAbnfRules;
extern ABNF_APP_RULE_S g_astAbnfAppRule[ABNF_RULE_BUTT];
extern BOOL        g_bAbnfInit;
extern ABNF_RULE_LIST_S *g_pstAbnfBuiltRuleList;
extern ULONG        g_ulAbnfFailPos;