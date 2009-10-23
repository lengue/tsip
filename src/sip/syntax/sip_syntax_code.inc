/*******************************************************************************
功能    : .c对应的inc文件
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/
ULONG SIP_CodeSIPmessage(UBUF_HEADER_S *pstUbufMsg,
                         void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeRequest(UBUF_HEADER_S *pstUbufMsg,
                         void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeResponse(UBUF_HEADER_S *pstUbufMsg,
                         void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeRequestLine(UBUF_HEADER_S *pstUbufMsg,
                         void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeStatusLine(UBUF_HEADER_S *pstUbufMsg,
                         void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeMessageHeader(UBUF_HEADER_S *pstUbufMsg,
                         void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeMessageBody(UBUF_HEADER_S *pstUbufMsg,
                         void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeMethod(UBUF_HEADER_S *pstUbufMsg,
                         void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeRequestURI(UBUF_HEADER_S *pstUbufMsg,
                         void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeSIPVersion(UBUF_HEADER_S *pstUbufMsg,
                         void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeSipURI(UBUF_HEADER_S *pstUbufMsg,
                         void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeSipsURI(UBUF_HEADER_S *pstUbufMsg,
                         void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeUserinfo(UBUF_HEADER_S *pstUbufMsg,
                         void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeUser(UBUF_HEADER_S *pstUbufMsg,
                         void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeHostport(UBUF_HEADER_S *pstUbufMsg,
                         void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeHost(UBUF_HEADER_S *pstUbufMsg,
                         void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodePort(UBUF_HEADER_S *pstUbufMsg,
                         void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeStatusCode(UBUF_HEADER_S *pstUbufMsg,
                         void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeReasonPhrase(UBUF_HEADER_S *pstUbufMsg,
                         void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeFromSpec(UBUF_HEADER_S *pstUbufMsg,
                         void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeFromParam(UBUF_HEADER_S *pstUbufMsg,
                         void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeToParam(UBUF_HEADER_S *pstUbufMsg,
                         void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeViaParm(UBUF_HEADER_S *pstUbufMsg,
                         void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeSentProtocol(UBUF_HEADER_S *pstUbufMsg,
                         void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeTransport(UBUF_HEADER_S *pstUbufMsg,
                         void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeSentBy(UBUF_HEADER_S *pstUbufMsg,
                         void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeViaParams(UBUF_HEADER_S *pstUbufMsg,
                         void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeViaTtl(UBUF_HEADER_S *pstUbufMsg,
                         void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeViaMaddr(UBUF_HEADER_S *pstUbufMsg,
                         void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeViaReceived(UBUF_HEADER_S *pstUbufMsg,
                         void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeViaBranch(UBUF_HEADER_S *pstUbufMsg,
                         void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeNameAddr(UBUF_HEADER_S *pstUbufMsg,
                         void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeAddrSpec(UBUF_HEADER_S *pstUbufMsg,
                         void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeHeaderAccept(UBUF_HEADER_S *pstUbufMsg,
                         void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeHeaderAcceptEncoding(UBUF_HEADER_S *pstUbufMsg,
                         void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeHeaderCallID(UBUF_HEADER_S *pstUbufMsg,
                         void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeHeaderCseq(UBUF_HEADER_S *pstUbufMsg,
                         void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeHeaderFrom(UBUF_HEADER_S *pstUbufMsg,
                         void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeHeaderTo(UBUF_HEADER_S *pstUbufMsg,
                         void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeHeaderVia(UBUF_HEADER_S *pstUbufMsg,
                         void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeHeaderMaxForwards(UBUF_HEADER_S *pstUbufMsg,
                         void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
