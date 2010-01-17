/*******************************************************************************
功能    : .c对应的inc文件
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/
ULONG SIP_CodeSIPmessage(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeRequest(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeResponse(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeRequestLine(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeStatusLine(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeMessageHeader(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeMessageBody(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeMethod(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeRequestURI(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeSIPVersion(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeSipURI(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeSipsURI(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeUserinfo(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeUser(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeHostport(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeHost(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodePort(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeStatusCode(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeReasonPhrase(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeFromSpec(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeFromParam(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeToParam(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeViaParm(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeSentProtocol(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeTransport(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeSentBy(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeViaParams(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeViaTtl(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeViaMaddr(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeViaReceived(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeViaBranch(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeNameAddr(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeAddrSpec(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeHeaderAccept(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeHeaderAcceptEncoding(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeHeaderCallID(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeHeaderCseq(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeHeaderFrom(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeHeaderTo(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeHeaderVia(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeHeaderMaxForwards(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeHeaderContact(void *pstStruct,
                            SIP_SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeContactParam(void *pstStruct,
                           SIP_SYNTAX_BUFFER_S *pstBuffer);

