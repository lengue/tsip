/*******************************************************************************
功能    : abnf模块对外提供的API函数
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
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