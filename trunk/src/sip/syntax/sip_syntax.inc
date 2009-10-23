/*******************************************************************************
功能    : .c对应的头文件
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/
ULONG SIP_Syntax_Init(SIP_SYNTAX_CFG_S *pstCfg);
ULONG SIP_Syntax_SipCode(UBUF_HEADER_S *pstUBuf,
                         UBUF_PTR upstSipMsg,
                         UCHAR *pucBuffer,
                         ULONG  ulBufferLen,
                         ULONG *pulMsgLen);
ULONG SIP_Syntax_SipDecode(UCHAR *pucMsgString,
                           ULONG ulMsgLen,
                           UBUF_HEADER_S *pstUBuf,
                           UBUF_PTR *pupstSipMsg);