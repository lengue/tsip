typedef struct tagSYNTAX_BUFFER_S
{
    ULONG  ulBufferLen;  /* 缓存长度 */
    ULONG  ulCurrentLen; /* 缓存使用长度 */
    UCHAR *pucBuffer;    /* 缓存指针 */
}SYNTAX_BUFFER_S;

typedef ULONG (* FUNC_SYNTAX_CODE) (void *pstStruct,
                                    SYNTAX_BUFFER_S *pstBuffer);
typedef ULONG (* FUNC_SYNTAX_DECODE)(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                                     UCHAR               *pucString,
                                     UBUF_HEADER_S       *pstUbuf,
                                     void               **ppStruct);
typedef ULONG (* FUNC_SYNTAX_CLONE)(void          *pSrcStruct,
                                    UBUF_HEADER_S *pstDstUbufMsg,
                                    void         **ppDstStruct);

/* SIP ABNF应用规则结构 */
typedef struct tagSYNTAX_APP_RULE_S
{
    ABNF_APP_RULE_HEADER
    FUNC_SYNTAX_CODE   pfnCode;
    FUNC_SYNTAX_DECODE pfnDecode;
    FUNC_SYNTAX_CLONE  pfnClone;
}SYNTAX_APP_RULE_S;
