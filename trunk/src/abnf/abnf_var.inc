/*******************************************************************************
����    : .c��Ӧ��inc�ļ�
������  : �ƴ�ƽ
��������: 200.01.10
�޸ļ�¼:
*******************************************************************************/
extern ABNF_RULE_S g_astCoreRuleTbl[ABNF_CORE_RULE_BUTT];
extern ABNF_RULE_LIST_S g_stCoreRules;
extern ABNF_RULE_S g_astAbnfRuleTbl[ABNF_RULE_BUTT];
extern ABNF_RULE_LIST_S g_stAbnfRules;
extern ABNF_APP_RULE_S g_astAbnfAppRule[ABNF_RULE_BUTT];
extern BOOL        g_bAbnfInit;
extern UCHAR       *g_pucAbnfMatchText;
extern ULONG        g_ulAbnfMatchTextLen;
extern ABNF_RULE_LIST_S *g_pstAbnfMatchRuleList;
extern ABNF_RULE_LIST_S *g_pstAbnfBuiltRuleList;
extern ULONG        g_ulAbnfFailPos;