/*******************************************************************************
功能    : SIP结构克隆
创建人  : 唐春平
创建日期: 2009.07.19
修改记录:
*******************************************************************************/
/* 外部依赖模块头文件 */
#include "common\common.h"
#include "abnf\abnf.h"
#include "ubuf\ubuf.h"
#include "sip\uri.h"

/* 本模块对外提供的常量和结构头文件 */
#include "sip\sip_const.h"
#include "sip\sip_type.h"

#include "..\include\syntax\sip_syntax_const.h"
#include "..\include\syntax\sip_syntax_type.h"

/* 本模块内部使用的常量和结构头文件 */
#include "sip_syntax_const.inc"
#include "sip_syntax_type.inc"

/* 本模块内部函数声明头文件 */
#include "sip_syntax_code.inc"

/* 本模块全局变量声明头文件 */
#include "sip_syntax_var.inc"

/*******************************************************************************
SIP-message    =  Request / Response
*******************************************************************************/
ULONG SIP_CloneSIPmessage(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    return SUCCESS;
}

/*******************************************************************************
Request        =  Request-Line
                  *( message-header )
                  CRLF
                  [ message-body ]
*******************************************************************************/
ULONG SIP_CloneRequest(void          *pSrcStruct,
                       UBUF_HEADER_S *pstDstUbufMsg,
                       void         **ppDstStruct)
{
    return SUCCESS;
}

/*******************************************************************************
Response          =  Status-Line
                     *( message-header )
                     CRLF
                     [ message-body ]
*******************************************************************************/
ULONG SIP_CloneResponse(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    return SUCCESS;
}

/*******************************************************************************
Request-Line   =  Method SP Request-URI SP SIP-Version CRLF
*******************************************************************************/
ULONG SIP_CloneRequestLine(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    return SUCCESS;
}

/*******************************************************************************
Status-Line     =  SIP-Version SP Status-Code SP Reason-Phrase CRLF
*******************************************************************************/
ULONG SIP_CloneStatusLine(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    return SUCCESS;
}

/*******************************************************************************
message-header  =  (Accept
                /  Accept-Encoding
                /  Accept-Language
                /  Alert-Info
                /  Allow
                /  Authentication-Info
                /  Authorization
                /  Call-ID
                /  Call-Info
                /  Contact
                /  Content-Disposition
                /  Content-Encoding
                /  Content-Language
                /  Content-Length
                /  Content-Type
                /  CSeq
                /  Date
                /  Error-Info
                /  Expires
                /  From
                /  In-Reply-To
                /  Max-Forwards
                /  MIME-Version
                /  Min-Expires
                /  Organization
                /  Priority
                /  Proxy-Authenticate
                /  Proxy-Authorization
                /  Proxy-Require
                /  Record-Route
                /  Reply-To
                /  Require
                /  Retry-After
                /  Route
                /  Server
                /  Subject
                /  Supported
                /  Timestamp
                /  To
                /  Unsupported
                /  User-Agent
                /  Via
                /  Warning
                /  WWW-Authenticate
                /  extension-header) CRLF
*******************************************************************************/
ULONG SIP_CloneMessageHeader(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    return SUCCESS;
}

/*******************************************************************************
message-body  =  *OCTET
*******************************************************************************/
ULONG SIP_CloneMessageBody(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    return SUCCESS;
}

/*******************************************************************************
Method            =  INVITEm / ACKm / OPTIONSm / BYEm
                     / CANCELm / REGISTERm
                     / extension-method
*******************************************************************************/
ULONG SIP_CloneMethod(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    return SUCCESS;
}

/*******************************************************************************
Request-URI    =  SIP-URI / SIPS-URI / absoluteURI
*******************************************************************************/
ULONG SIP_CloneRequestURI(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    return SUCCESS;
}

/*******************************************************************************
SIP-Version    =  "SIP" "/" 1*DIGIT "." 1*DIGIT
*******************************************************************************/
ULONG SIP_CloneSIPVersion(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    return SUCCESS;
}

/*******************************************************************************
SIP-URI          =  "sip:" [ userinfo ] hostport
                    uri-parameters [ headers ]
*******************************************************************************/
ULONG SIP_CloneSipURI(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    return SUCCESS;
}

/*******************************************************************************
SIPS-URI         =  "sips:" [ userinfo ] hostport
                    uri-parameters [ headers ]
*******************************************************************************/
ULONG SIP_CloneSipsURI(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    return SUCCESS;
}

/*******************************************************************************
userinfo         =  ( user / telephone-subscriber ) [ ":" password ] "@"
*******************************************************************************/
ULONG SIP_CloneUserinfo(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    return SUCCESS;
}

/*******************************************************************************
userinfo         =  ( user / telephone-subscriber ) [ ":" password ] "@"
*******************************************************************************/
ULONG SIP_CloneUser(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    return SUCCESS;
}

/*******************************************************************************
hostport         =  host [ ":" port ]
*******************************************************************************/
ULONG SIP_CloneHostport(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    return SUCCESS;
}

/*******************************************************************************
host             =  hostname / IPv4address / IPv6reference
*******************************************************************************/
ULONG SIP_CloneHost(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    return SUCCESS;
}

/*******************************************************************************
port           =  1*DIGIT
*******************************************************************************/
ULONG SIP_ClonePort(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    return SUCCESS;
}

/*******************************************************************************
Status-Code     =  Informational
               /   Redirection
               /   Success
               /   Client-Error
               /   Server-Error
               /   Global-Failure
               /   extension-code
*******************************************************************************/
ULONG SIP_CloneStatusCode(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    return SUCCESS;
}

/*******************************************************************************
Reason-Phrase   =  *(reserved / unreserved / escaped
                   / UTF8-NONASCII / UTF8-CONT / SP / HTAB)
*******************************************************************************/
ULONG SIP_CloneReasonPhrase(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    return SUCCESS;
}

/*******************************************************************************
from-spec   =  ( name-addr / addr-spec )
               *( SEMI from-param )
*******************************************************************************/
ULONG SIP_CloneFromSpec(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    return SUCCESS;
}

/*******************************************************************************
from-param  =  tag-param / generic-param
*******************************************************************************/
ULONG SIP_CloneFromParam(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    return SUCCESS;
}

/*******************************************************************************
to-param  =  tag-param / generic-param
*******************************************************************************/
ULONG SIP_CloneToParam(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    return SUCCESS;
}

/*******************************************************************************
via-parm          =  sent-protocol LWS sent-by *( SEMI via-params )
*******************************************************************************/
ULONG SIP_CloneViaParm(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    return SUCCESS;
}

/*******************************************************************************
sent-protocol     =  protocol-name SLASH protocol-version
                     SLASH transport
*******************************************************************************/
ULONG SIP_CloneSentProtocol(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    return SUCCESS;
}

/*******************************************************************************
transport         =  "UDP" / "TCP" / "TLS" / "SCTP"
                     / other-transport
*******************************************************************************/
ULONG SIP_CloneTransport(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    return SUCCESS;
}

/*******************************************************************************
sent-by           =  host [ COLON port ]
*******************************************************************************/
ULONG SIP_CloneSentBy(UBUF_HEADER_S *pstSrcUbufMsg,
                          void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    return SUCCESS;
}

/*******************************************************************************
via-params        =  via-ttl / via-maddr
                     / via-received / via-branch
                     / via-extension
*******************************************************************************/
ULONG SIP_CloneViaParams(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    return SUCCESS;
}

/*******************************************************************************
via-ttl           =  "ttl" EQUAL ttl
*******************************************************************************/
ULONG SIP_CloneViaTtl(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    return SUCCESS;
}

/*******************************************************************************
via-maddr         =  "maddr" EQUAL host
*******************************************************************************/
ULONG SIP_CloneViaMaddr(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    return SUCCESS;
}

/*******************************************************************************
via-received      =  "received" EQUAL (IPv4address / IPv6address)
*******************************************************************************/
ULONG SIP_CloneViaReceived(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    return SUCCESS;
}

/*******************************************************************************
via-branch        =  "branch" EQUAL token
*******************************************************************************/
ULONG SIP_CloneViaBranch(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    return SUCCESS;
}

/*******************************************************************************
name-addr      =  [ display-name ] LAQUOT addr-spec RAQUOT
*******************************************************************************/
ULONG SIP_CloneNameAddr(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    return SUCCESS;
}

/*******************************************************************************
addr-spec      =  SIP-URI / SIPS-URI / absoluteURI
*******************************************************************************/
ULONG SIP_CloneAddrSpec(void          *pSrcStruct,
                        UBUF_HEADER_S *pstDstUbufMsg,
                        void         **ppDstStruct)
{
    return SUCCESS;
}

/* 所有的头域 */
/*******************************************************************************
Accept         =  "Accept" HCOLON
                   [ accept-range *(COMMA accept-range) ]
*******************************************************************************/
ULONG SIP_CloneHeaderAccept(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    return SUCCESS;
}

/*******************************************************************************
Accept-Encoding  =  "Accept-Encoding" HCOLON
                     [ encoding *(COMMA encoding) ]
*******************************************************************************/
ULONG SIP_CloneHeaderAcceptEncoding(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    return SUCCESS;
}

/*******************************************************************************
Call-ID  =  ( "Call-ID" / "i" ) HCOLON callid
*******************************************************************************/
ULONG SIP_CloneHeaderCallID(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    return SUCCESS;
}

/*******************************************************************************
CSeq  =  "CSeq" HCOLON 1*DIGIT LWS Method
*******************************************************************************/
ULONG SIP_CloneHeaderCseq(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    return SUCCESS;
}

/*******************************************************************************
From        =  ( "From" / "f" ) HCOLON from-spec
*******************************************************************************/
ULONG SIP_CloneHeaderFrom(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    return SUCCESS;
}

/*******************************************************************************
To        =  ( "To" / "t" ) HCOLON ( name-addr
             / addr-spec ) *( SEMI to-param )
*******************************************************************************/
ULONG SIP_CloneHeaderTo(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    return SUCCESS;
}

/*******************************************************************************
Via               =  ( "Via" / "v" ) HCOLON via-parm *(COMMA via-parm)
*******************************************************************************/
ULONG SIP_CloneHeaderVia(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    return SUCCESS;
}

/*******************************************************************************
Max-Forwards  =  "Max-Forwards" HCOLON 1*DIGIT
*******************************************************************************/
ULONG SIP_CloneHeaderMaxForwards(void          *pSrcStruct,
                                 UBUF_HEADER_S *pstDstUbufMsg,
                                 void         **ppDstStruct)
{
    return SUCCESS;
}

