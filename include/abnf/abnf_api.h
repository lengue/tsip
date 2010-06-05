/*******************************************************************************
����    : abnfģ������ṩ��API����
������  : �ƴ�ƽ
��������: 200.01.10
�޸ļ�¼:
*******************************************************************************/
extern ULONG ABNF_RegistRuleList(UCHAR *pucText,
                                 ULONG  ulLen,
                                 UCHAR *pucDepends,
                                 UCHAR  ucDependNum,
                                 UCHAR *pucRuleListIndex);
extern ULONG ABNF_GetRuleIndex(UCHAR ucRuleListIndex, UCHAR *pucRuleName, ULONG *pulRuleIndex);
extern ULONG ABNF_GrammarParse(UCHAR *pucText,
                        ULONG  ulLen,
                        UCHAR  ucRuleListIndex,
                        ULONG  ulMatchRule,
                        ABNF_GRAMMAR_NODE_S **ppstNode);
extern ULONG ABNF_FreeNodeTree(ABNF_GRAMMAR_NODE_S *pstGrammarNode);