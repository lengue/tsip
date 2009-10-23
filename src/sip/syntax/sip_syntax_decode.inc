/*******************************************************************************
功能    : .c文件对应的inc文件
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/
ULONG SIP_ParseSIPmessage(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                          UCHAR               *pucString,
                          UBUF_HEADER_S       *pstUbuf,
                          UBUF_PTR            *pupStruct);
ULONG SIP_ParseRequest(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                       UCHAR               *pucString,
                       UBUF_HEADER_S       *pstUbuf,
                       UBUF_PTR            *pupStruct);
ULONG SIP_ParseResponse(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                        UCHAR               *pucString,
                        UBUF_HEADER_S       *pstUbuf,
                        UBUF_PTR            *pupStruct);
ULONG SIP_ParseRequestLine(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                           UCHAR               *pucString,
                           UBUF_HEADER_S       *pstUbuf,
                           UBUF_PTR            *pupStruct);
ULONG SIP_ParseStatusLine(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                          UCHAR               *pucString,
                          UBUF_HEADER_S       *pstUbuf,
                          UBUF_PTR            *pupStruct);
ULONG SIP_ParseMessageHeader(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                             UCHAR               *pucString,
                             UBUF_HEADER_S       *pstUbuf,
                             UBUF_PTR            *pupStruct);
ULONG SIP_ParseMessageBody(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                           UCHAR               *pucString,
                           UBUF_HEADER_S       *pstUbuf,
                           UBUF_PTR            *pupStruct);
ULONG SIP_ParseMethod(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                      UCHAR               *pucString,
                      UBUF_HEADER_S       *pstUbuf,
                      UBUF_PTR            *pupStruct);
ULONG SIP_ParseRequestURI(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                          UCHAR               *pucString,
                          UBUF_HEADER_S       *pstUbuf,
                          UBUF_PTR            *pupStruct);
ULONG SIP_ParseSIPVersion(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                          UCHAR               *pucString,
                          UBUF_HEADER_S       *pstUbuf,
                          UBUF_PTR            *pupStruct);
ULONG SIP_ParseStatusCode(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                          UCHAR               *pucString,
                          UBUF_HEADER_S       *pstUbuf,
                          UBUF_PTR            *pupStruct);
ULONG SIP_ParseSipURI(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                      UCHAR               *pucString,
                      UBUF_HEADER_S       *pstUbuf,
                      UBUF_PTR            *pupStruct);
ULONG SIP_ParseSipsURI(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                       UCHAR               *pucString,
                       UBUF_HEADER_S       *pstUbuf,
                       UBUF_PTR            *pupStruct);
ULONG SIP_ParseUserinfo(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                        UCHAR               *pucString,
                        UBUF_HEADER_S       *pstUbuf,
                        UBUF_PTR            *pupStruct);
ULONG SIP_ParseHostport(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                        UCHAR               *pucString,
                        UBUF_HEADER_S       *pstUbuf,
                        UBUF_PTR            *pupStruct);
ULONG SIP_ParseHost(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                    UCHAR               *pucString,
                    UBUF_HEADER_S       *pstUbuf,
                    UBUF_PTR            *pupStruct);
ULONG SIP_ParsePort(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                    UCHAR               *pucString,
                    UBUF_HEADER_S       *pstUbuf,
                    UBUF_PTR            *pupStruct);
ULONG SIP_ParseFromSpec(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                        UCHAR               *pucString,
                        UBUF_HEADER_S       *pstUbuf,
                        UBUF_PTR            *pupStruct);
ULONG SIP_ParseNameAddr(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                        UCHAR               *pucString,
                        UBUF_HEADER_S       *pstUbuf,
                        UBUF_PTR            *pupStruct);
ULONG SIP_ParseAddrSpec(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                        UCHAR               *pucString,
                        UBUF_HEADER_S       *pstUbuf,
                        UBUF_PTR            *pupStruct);
ULONG SIP_ParseFromParam(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                         UCHAR               *pucString,
                         UBUF_HEADER_S       *pstUbuf,
                         UBUF_PTR            *pupStruct);
ULONG SIP_ParseToParam(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                       UCHAR               *pucString,
                       UBUF_HEADER_S       *pstUbuf,
                       UBUF_PTR            *pupStruct);
ULONG SIP_ParseTagParam(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                       UCHAR               *pucString,
                       UBUF_HEADER_S       *pstUbuf,
                       UBUF_PTR            *pupStruct);
ULONG SIP_ParseCallID(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                       UCHAR               *pucString,
                       UBUF_HEADER_S       *pstUbuf,
                       UBUF_PTR            *pupStruct);
ULONG SIP_ParseViaParm(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                       UCHAR               *pucString,
                       UBUF_HEADER_S       *pstUbuf,
                       UBUF_PTR            *pupStruct);
ULONG SIP_ParseSentProtocol(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                            UCHAR               *pucString,
                            UBUF_HEADER_S       *pstUbuf,
                            UBUF_PTR            *pupStruct);
ULONG SIP_ParseTransport(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                         UCHAR               *pucString,
                         UBUF_HEADER_S       *pstUbuf,
                         UBUF_PTR            *pupStruct);
ULONG SIP_ParseSentBy(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                      UCHAR               *pucString,
                      UBUF_HEADER_S       *pstUbuf,
                      UBUF_PTR            *pupStruct);
ULONG SIP_ParseViaParams(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                         UCHAR               *pucString,
                         UBUF_HEADER_S       *pstUbuf,
                         UBUF_PTR            *pupStruct);
ULONG SIP_ParseViaTtl(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                      UCHAR               *pucString,
                      UBUF_HEADER_S       *pstUbuf,
                      UBUF_PTR            *pupStruct);
ULONG SIP_ParseViaMaddr(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                        UCHAR               *pucString,
                        UBUF_HEADER_S       *pstUbuf,
                        UBUF_PTR            *pupStruct);
ULONG SIP_ParseViaReceived(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                           UCHAR               *pucString,
                           UBUF_HEADER_S       *pstUbuf,
                           UBUF_PTR            *pupStruct);
ULONG SIP_ParseViaBranch(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                         UCHAR               *pucString,
                         UBUF_HEADER_S       *pstUbuf,
                         UBUF_PTR            *pupStruct);
ULONG SIP_ParseHeaderAccept(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                            UCHAR               *pucString,
                            UBUF_HEADER_S       *pstUbuf,
                            UBUF_PTR            *pupStruct);
ULONG SIP_ParseHeaderAcceptEncoding(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                                    UCHAR               *pucString,
                                    UBUF_HEADER_S       *pstUbuf,
                                    UBUF_PTR            *pupStruct);
ULONG SIP_ParseHeaderCallID(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                            UCHAR               *pucString,
                            UBUF_HEADER_S       *pstUbuf,
                            UBUF_PTR            *pupStruct);
ULONG SIP_ParseHeaderCseq(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                          UCHAR               *pucString,
                          UBUF_HEADER_S       *pstUbuf,
                          UBUF_PTR            *pupStruct);
ULONG SIP_ParseHeaderFrom(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                          UCHAR               *pucString,
                          UBUF_HEADER_S       *pstUbuf,
                          UBUF_PTR            *pupStruct);
ULONG SIP_ParseHeaderTo(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                        UCHAR               *pucString,
                        UBUF_HEADER_S       *pstUbuf,
                        UBUF_PTR            *pupStruct);
ULONG SIP_ParseHeaderVia(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                         UCHAR               *pucString,
                         UBUF_HEADER_S       *pstUbuf,
                         UBUF_PTR            *pupStruct);
ULONG SIP_ParseHeaderMaxForwards(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                                 UCHAR               *pucString,
                                 UBUF_HEADER_S       *pstUbuf,
                                 UBUF_PTR            *pupStruct);
