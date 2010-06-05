/*******************************************************************************
功能    : .c对应的inc文件
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/
ULONG SIP_CodeSIPmessage(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeRequest(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeResponse(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeRequestLine(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeStatusLine(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeMessageHeader(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeMessageBody(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeMethod(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeRequestURI(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeSIPVersion(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeSipURI(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeSipsURI(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeUserinfo(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeUser(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeHostport(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeHost(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodePort(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeStatusCode(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeReasonPhrase(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeFromSpec(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeFromParam(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeToParam(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeViaParm(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeSentProtocol(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeTransport(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeSentBy(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeViaParams(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeViaTtl(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeViaMaddr(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeViaReceived(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeViaBranch(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeNameAddr(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeAddrSpec(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeHeaderAccept(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeHeaderAcceptEncoding(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeHeaderCallID(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeHeaderCseq(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeHeaderFrom(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeHeaderTo(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeHeaderVia(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeHeaderMaxForwards(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeHeaderContact(void *pstStruct,
                            SYNTAX_BUFFER_S *pstBuffer);
ULONG SIP_CodeContactParam(void *pstStruct,
                           SYNTAX_BUFFER_S *pstBuffer);

