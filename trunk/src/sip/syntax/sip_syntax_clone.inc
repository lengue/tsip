/*******************************************************************************
功能    : SIP结构克隆
创建人  : 唐春平
创建日期: 2009.07.19
修改记录:
*******************************************************************************/
ULONG SIP_CloneSIPmessage(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct);
ULONG SIP_CloneRequest(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct);
ULONG SIP_CloneResponse(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct);
ULONG SIP_CloneRequestLine(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct);
ULONG SIP_CloneStatusLine(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct);
ULONG SIP_CloneMessageHeader(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct);
ULONG SIP_CloneMessageBody(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct);
ULONG SIP_CloneMethod(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct);
ULONG SIP_CloneRequestURI(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct);
ULONG SIP_CloneSIPVersion(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct);
ULONG SIP_CloneSipURI(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct);
ULONG SIP_CloneSipsURI(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct);
ULONG SIP_CloneUserinfo(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct);
ULONG SIP_CloneHostport(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct);
ULONG SIP_CloneHost(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct);
ULONG SIP_ClonePort(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct);
ULONG SIP_CloneStatusCode(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct);
ULONG SIP_CloneFromSpec(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct);
ULONG SIP_CloneFromParam(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct);
ULONG SIP_CloneToParam(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct);
ULONG SIP_CloneViaParm(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct);
ULONG SIP_CloneSentProtocol(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct);
ULONG SIP_CloneTransport(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct);
ULONG SIP_CloneSentBy(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct);
ULONG SIP_CloneViaParams(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct);
ULONG SIP_CloneViaTtl(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct);
ULONG SIP_CloneViaMaddr(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct);
ULONG SIP_CloneViaReceived(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct);
ULONG SIP_CloneViaBranch(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct);
ULONG SIP_CloneNameAddr(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct);
ULONG SIP_CloneAddrSpec(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct);
ULONG SIP_CloneHeaderAccept(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct);
ULONG SIP_CloneHeaderAcceptEncoding(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct);
ULONG SIP_CloneHeaderCallID(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct);
ULONG SIP_CloneHeaderCseq(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct);
ULONG SIP_CloneHeaderFrom(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct);
ULONG SIP_CloneHeaderTo(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct);
ULONG SIP_CloneHeaderVia(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct);
ULONG SIP_CloneHeaderMaxForwards(void          *pSrcStruct,
                                 UBUF_HEADER_S *pstDstUbufMsg,
                                 void         **ppDstStruct);
