/*******************************************************************************
����    : SIPЭ��ջ�﷨����������ṩAPI
������  : �ƴ�ƽ
��������: 200.01.10
�޸ļ�¼:
*******************************************************************************/
extern ULONG SIP_Syntax_Init(SIP_SYNTAX_CFG_S *pstCfg);
extern ULONG SIP_Syntax_GetRuleIndex(UCHAR *pucRuleName, ULONG *pulRuleIndex);
extern ULONG SIP_Syntax_Code(ULONG  ulRuleIndex,
                      void  *pstStruct,
                      UCHAR *pucBuffer,
                      ULONG  ulBufferLen,
                      ULONG *pulMsgLen);
extern ULONG SIP_Syntax_Decode(ULONG  ulRuleIndex,
                        UCHAR *pucMsgString,
                        ULONG  ulMsgLen,
                        UBUF_HEADER_S *pstUBuf,
                        void **ppstStruct);
extern ULONG SIP_Syntax_Clone(ULONG          ulRuleIndex,
                       void          *pSrcStruct,
                       UBUF_HEADER_S *pstDstUbuf,
                       void         **ppDstStruct);