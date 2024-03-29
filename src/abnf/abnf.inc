/*******************************************************************************
功能    : abnf.c对应的inc文件
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/
ULONG ABNF_Init();
ULONG ABNF_RegistRuleList(UCHAR *pucText,
                          ULONG  ulLen,
                          UCHAR *pucDepends,
                          UCHAR  ucDependNum,
                          UCHAR *pucRuleListIndex);
ULONG ABNF_FreeRuleList(UCHAR ucRuleIndex);
ULONG ABNF_GetRuleIndex(UCHAR ucRuleListIndex, UCHAR *pucRuleName, ULONG *pulRuleIndex);
ULONG ABNF_GrammarParse(UCHAR *pucText,
                        ULONG  ulLen,
                        UCHAR  ucRuleListIndex,
                        ULONG  ulMatchRule,
                        ABNF_GRAMMAR_NODE_S **ppstNode);
ULONG ABNF_AddCharElement(ABNF_ELEMENT_S **ppstElement,
                          ULONG ulMinRepeat,
                          ULONG ulMaxRepeat,
                          UCHAR ucMinValue,
                          UCHAR ucMaxValue);
ULONG ABNF_AddStringElement(ABNF_ELEMENT_S **ppstElement,
                            ULONG ulMinRepeat,
                            ULONG ulMaxRepeat,
                            BOOL  bSensitive,
                            UCHAR *pucString);
ULONG ABNF_AddRuleElement(ABNF_ELEMENT_S **ppstElement,
                          ULONG ulMinRepeat,
                          ULONG ulMaxRepeat,
                          UCHAR ucRuleListIndex,
                          ULONG ulIndex);
ULONG ABNF_AddSequenceElement(ABNF_ELEMENT_S **ppstElement,
                              ULONG ulMinRepeat,
                              ULONG ulMaxRepeat);
ULONG ABNF_AddOptionalElement(ABNF_ELEMENT_S **ppstElement,
                              ULONG ulMinRepeat,
                              ULONG ulMaxRepeat);
ULONG ABNF_FreeElement(ABNF_ELEMENT_S *pstElement);
ULONG ABNF_InitCoreRuleTree();
ULONG ABNF_InitAbnfRuleTree();
ABNF_GRAMMAR_NODE_S* ABNF_AllocNode(ULONG ulOffset, UCHAR ucRuleListIndex, ULONG ulIndex);
ULONG ABNF_FreeNodeTree(ABNF_GRAMMAR_NODE_S *pstGrammarNode);
ULONG ABNF_MatchElement(ABNF_MATCH_TARGET_S *pstTarget,
                        ULONG ulOffset,
                        ABNF_ELEMENT_S *pstElement,
                        ULONG *pulSize,
                        ABNF_GRAMMAR_NODE_S **ppstNode);
ULONG ABNF_MatchCharElement(ABNF_MATCH_TARGET_S *pstTarget,
                            ULONG ulOffset,
                            ABNF_ELEMENT_S *pstElement,
                            ULONG *pulSize,
                            ABNF_GRAMMAR_NODE_S **ppstNode);
ULONG ABNF_MatchStringElement(ABNF_MATCH_TARGET_S *pstTarget,
                              ULONG ulOffset,
                              ABNF_ELEMENT_S *pstElement,
                              ULONG *pulSize,
                              ABNF_GRAMMAR_NODE_S **ppstNode);
ULONG ABNF_MatchRuleElement(ABNF_MATCH_TARGET_S *pstTarget,
                            ULONG ulOffset,
                            ABNF_ELEMENT_S *pstElement,
                            ULONG *pulSize,
                            ABNF_GRAMMAR_NODE_S **ppstNode);
ULONG ABNF_MatchSequenceElement(ABNF_MATCH_TARGET_S *pstTarget,
                                ULONG ulOffset,
                                ABNF_ELEMENT_S *pstElement,
                                ULONG *pulSize,
                                ABNF_GRAMMAR_NODE_S **ppstNode);
ULONG ABNF_MatchOptionalElement(ABNF_MATCH_TARGET_S *pstTarget,
                                ULONG ulOffset,
                                ABNF_ELEMENT_S *pstElement,
                                ULONG *pulSize,
                                ABNF_GRAMMAR_NODE_S **ppstNode);
ULONG ABNF_FindRuleByName(UCHAR *pucName,
                          UCHAR *ucRuleListIndex,
                          ULONG *pulRuleIndex);
ULONG ABNF_ParseRuleList(ABNF_GRAMMAR_NODE_S *pstNode, UCHAR *pucText, void **ppStruct);
ULONG ABNF_ParseRule(ABNF_GRAMMAR_NODE_S *pstNode, UCHAR *pucText, void **ppStruct);
ULONG ABNF_ParseRuleName(ABNF_GRAMMAR_NODE_S *pstNode, UCHAR *pucText, void **ppStruct);
ULONG ABNF_ParseElements(ABNF_GRAMMAR_NODE_S *pstNode, UCHAR *pucText, void **ppStruct);
ULONG ABNF_ParseAlternation(ABNF_GRAMMAR_NODE_S *pstNode, UCHAR *pucText, void **ppStruct);
ULONG ABNF_ParseConcatenation(ABNF_GRAMMAR_NODE_S *pstNode, UCHAR *pucText, void **ppStruct);
ULONG ABNF_ParseRepetition(ABNF_GRAMMAR_NODE_S *pstNode, UCHAR *pucText, void **ppStruct);
ULONG ABNF_ParseRepeat(ABNF_GRAMMAR_NODE_S *pstNode, UCHAR *pucText, void **ppStruct);
ULONG ABNF_ParseElement(ABNF_GRAMMAR_NODE_S *pstNode, UCHAR *pucText, void **ppStruct);
ULONG ABNF_ParseGroup(ABNF_GRAMMAR_NODE_S *pstNode, UCHAR *pucText, void **ppStruct);
ULONG ABNF_ParseOption(ABNF_GRAMMAR_NODE_S *pstNode, UCHAR *pucText, void **ppStruct);
ULONG ABNF_ParseCharVal(ABNF_GRAMMAR_NODE_S *pstNode, UCHAR *pucText, void **ppStruct);
ULONG ABNF_ParseNumVal(ABNF_GRAMMAR_NODE_S *pstNode, UCHAR *pucText, void **ppStruct);
ULONG ABNF_ParseBinVal(ABNF_GRAMMAR_NODE_S *pstNode, UCHAR *pucText, void **ppStruct);
ULONG ABNF_ParseDecVal(ABNF_GRAMMAR_NODE_S *pstNode, UCHAR *pucText, void **ppStruct);
ULONG ABNF_ParseHexVal(ABNF_GRAMMAR_NODE_S *pstNode, UCHAR *pucText, void **ppStruct);
VOID ABNF_SuccessRecord();
VOID ABNF_FailRecord(ULONG ulPos);
