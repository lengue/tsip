/*******************************************************************************
����    : abnfģ������ṩ��API����
������  : �ƴ�ƽ
��������: 200.01.10
�޸ļ�¼:
*******************************************************************************/
extern ULONG ABNF_Init();
extern ULONG ABNF_BuildRuleList(UCHAR *pucText,
                                ULONG  ulLen,
                                ABNF_RULE_LIST_S **ppstRuleList);
extern ULONG ABNF_FreeRuleList(ABNF_RULE_LIST_S *pstRuleList);
extern ULONG ABNF_GrammarParse(UCHAR *pucText,
                                ULONG  ulLen,
                                ABNF_RULE_LIST_S *pstRuleList,
                                ULONG ulMatchRule,
                                ABNF_GRAMMAR_NODE_S **ppstNode);
extern ULONG ABNF_FreeNodeTree(ABNF_GRAMMAR_NODE_S *pstGrammarNode);