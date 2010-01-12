/*******************************************************************************
����    : SIPЭ��ջ�﷨�������ڲ�ʹ�ýṹ
������  : �ƴ�ƽ
��������: 200.01.10
�޸ļ�¼:
*******************************************************************************/
typedef ULONG (* FUNC_SIP_SYNTAX_CODE) (void *pstStruct,
                                        SIP_SYNTAX_BUFFER_S *pstBuffer);
typedef ULONG (* FUNC_SIP_SYNTAX_PARSE)(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                                        UCHAR               *pucString,
                                        UBUF_HEADER_S       *pstUbuf,
                                        void               **ppStruct);
/* SIP ABNFӦ�ù���ṹ */
typedef struct tagSIP_APP_RULE_S
{
    ABNF_APP_RULE_HEADER
    FUNC_SIP_SYNTAX_CODE  pfnCode;
    FUNC_SIP_SYNTAX_PARSE pfnParse;
}SIP_APP_RULE_S;

typedef struct tagSIP_SYNTAX_HEADER_MAP_S
{
    ULONG ulAppRuleIndex;
    ULONG ulStructSize;
}SIP_SYNTAX_HEADER_MAP_S;