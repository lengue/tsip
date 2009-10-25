/*******************************************************************************
功能    : .c对应的头文件
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/
ULONG SIP_Syntax_Init(SIP_SYNTAX_CFG_S *pstCfg);
ULONG SIP_Syntax_GetRuleIndex(UCHAR *pucRuleName, ULONG *pulRuleIndex);
ULONG SIP_Syntax_Code(ULONG  ulRuleIndex,
                      void  *pstStruct,
                      UCHAR *pucBuffer,
                      ULONG  ulBufferLen,
                      ULONG *pulMsgLen);
ULONG SIP_Syntax_Decode(ULONG  ulRuleIndex,
                        UCHAR *pucMsgString,
                        ULONG  ulMsgLen,
                        UBUF_HEADER_S *pstUBuf,
                        void **ppstStruct);
ULONG SIP_Syntax_Clone(ULONG          ulRuleIndex,
                       void          *pSrcStruct,
                       UBUF_HEADER_S *pstDstUbuf,
                       void         **ppDstStruct);