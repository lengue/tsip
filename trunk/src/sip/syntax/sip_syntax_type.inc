/*******************************************************************************
功能    : SIP协议栈语法与编码层内部使用结构
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/
typedef ULONG (* FUNC_SIP_SYNTAX_CODE) (UBUF_HEADER_S *pstUbufMsg,
                                        void *pstStruct,
                                        SIP_SYNTAX_BUFFER_S *pstBuffer);
typedef ULONG (* FUNC_SIP_SYNTAX_PARSE)(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                                        UCHAR               *pucString,
                                        UBUF_HEADER_S       *pstUbuf,
                                        UBUF_PTR            *pupStruct);
typedef ULONG (* FUNC_SIP_SYNTAX_CLONE)(UBUF_HEADER_S *pstSrcUbufMsg,
                                        UBUF_PTR       upSrcUPtr,
                                        UBUF_HEADER_S *pstDstUbufMsg,
                                        UBUF_PTR      *pupDstUPtr);
/* SIP ABNF应用规则结构 */
typedef struct tagSIP_APP_RULE_S
{
    ABNF_APP_RULE_HEADER
    FUNC_SIP_SYNTAX_CODE  pfnCode;
    FUNC_SIP_SYNTAX_PARSE pfnParse;
    FUNC_SIP_SYNTAX_CLONE pfnClone;
}SIP_APP_RULE_S;

typedef struct tagSIP_SYNTAX_HEADER_MAP_S
{
    ULONG ulAppRuleIndex;
    ULONG ulStructSize;
}SIP_SYNTAX_HEADER_MAP_S;