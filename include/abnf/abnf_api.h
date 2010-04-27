/*******************************************************************************
����    : abnfģ������ṩ��API����
������  : �ƴ�ƽ
��������: 200.01.10
�޸ļ�¼:
*******************************************************************************/
extern ULONG ABNF_Init();
extern ULONG ABNF_BuildRuleList(UCHAR *pucText,
                                ULONG  ulLen,
                                void **ppstRuleList);
extern ULONG ABNF_GetRuleIndex(void *pstRuleList, 
                               UCHAR *pucRuleName, 
                               ULONG *pulRuleIndex);
extern ULONG ABNF_GrammarParse(UCHAR *pucText,
                                ULONG ulLen,
                                void *pstRuleList,
                                ULONG ulMatchRule,
                                ABNF_GRAMMAR_NODE_S **ppstNode);
extern ULONG ABNF_FreeNodeTree(ABNF_GRAMMAR_NODE_S *pstGrammarNode);