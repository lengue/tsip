/*******************************************************************************
����    : SIPЭ��ջ�﷨����������ṩAPI
������  : �ƴ�ƽ
��������: 200.01.10
�޸ļ�¼:
*******************************************************************************/
extern ULONG SIP_Syntax_Init(SIP_SYNTAX_CFG_S *pstCfg);
extern ULONG SIP_Syntax_SipCode(UBUF_HEADER_S *pstUBuf,
                         UBUF_PTR upstSipMsg,
                         UCHAR *pucBuffer,
                         ULONG  ulBufferLen,
                         ULONG *pulMsgLen);
extern ULONG SIP_Syntax_SipDecode(UCHAR *pucMsgString,
                           ULONG ulMsgLen,
                           UBUF_HEADER_S *pstUBuf,
                           UBUF_PTR *pupstSipMsg);
extern ULONG SIP_Syntax_UriCode(UBUF_HEADER_S *pstUBuf,
                                 UBUF_PTR upstUri,
                                 UCHAR *pucBuffer,
                                 ULONG  ulBufferLen,
                                 ULONG *pulMsgLen);
extern ULONG SIP_Syntax_UriDecode(UCHAR *pucMsgString,
                           ULONG ulMsgLen,
                           UBUF_HEADER_S *pstUBuf,
                           UBUF_PTR *pupstUri);
extern ULONG SIP_Syntax_UriClone(UBUF_HEADER_S *pstSrcUbufMsg,
                          UBUF_PTR       upSrcUPtr,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          UBUF_PTR      *pupDstUPtr);