/*******************************************************************************
功能    : SIP消息编码函数
创建人  : 唐春平
创建日期: 200.01.10
修改记录:

编码原则:
1、 所有的编码函数形参一致
    UBUF_HEADER_S *pstUbufMsg     : 存储结构的UBUF
    void *pstStruct               : 编码结构的指针，结构体根据函数不同而不同
    SYNTAX_BUFFER_S *pstBuffer: 编码缓存结构
2、 每个函数首要的任务是明确pstStruct的结构类型
*******************************************************************************/
/* 外部依赖模块头文件 */
#include "common\common.h"
#include "abnf\abnf.h"
#include "ubuf\ubuf.h"
#include "uri\uri.h"
#include "syntax_comm\syntax_comm.h"

/* 本模块对外提供的常量和结构头文件 */
#include "..\..\include\sip\sip_const.h"
#include "..\..\include\sip\sip_type.h"

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
ULONG SIP_CodeSIPmessage(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer)
{
    SIP_MSG_S *pstSipMsg= NULL_PTR;

    pstSipMsg = (SIP_MSG_S *)pstStruct;
    if (pstSipMsg->eMsgType == SIP_MSG_TYPE_REQUEST)
    {
        return SIP_GET_CODE_FUNC(SIP_ABNF_RULE_REQUEST)(pstSipMsg, pstBuffer);
    }
    else if (pstSipMsg->eMsgType == SIP_MSG_TYPE_RESPONSE)
    {
        return SIP_GET_CODE_FUNC(SIP_ABNF_RULE_RESPONSE)(pstSipMsg, pstBuffer);
    }

    return FAIL;
}

/*******************************************************************************
Request        =  Request-Line
                  *( message-header )
                  CRLF
                  [ message-body ]
*******************************************************************************/
ULONG SIP_CodeRequest(void *pstStruct,
                      SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    SIP_MSG_S *pstSipMsg= NULL_PTR;

    pstSipMsg = (SIP_MSG_S *)pstStruct;

    ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_REQUEST_LINE)(&pstSipMsg->uStartLine.stRequstLine, pstBuffer);
    if (ulRet!= SUCCESS)
    {
        return ulRet;
    }

    ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_MESSAGE_HEADER)(pstSipMsg, pstBuffer);
    if (ulRet!= SUCCESS)
    {
        return ulRet;
    }

    SYNTAX_ADD_STRING(pstBuffer, "\r\n");

    ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_MESSAGE_BODY)(pstSipMsg, pstBuffer);
    if (ulRet!= SUCCESS)
    {
        return ulRet;
    }

    return SUCCESS;
}

/*******************************************************************************
Response          =  Status-Line
                     *( message-header )
                     CRLF
                     [ message-body ]
*******************************************************************************/
ULONG SIP_CodeResponse(void *pstStruct,
                       SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    SIP_MSG_S *pstSipMsg= NULL_PTR;

    pstSipMsg = (SIP_MSG_S *)pstStruct;
    ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_STATUS_LINE)(&pstSipMsg->uStartLine.stStatusLine, pstBuffer);
    if (ulRet!= SUCCESS)
    {
        return ulRet;
    }

    ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_MESSAGE_HEADER)(pstSipMsg, pstBuffer);
    if (ulRet!= SUCCESS)
    {
        return ulRet;
    }

    SYNTAX_ADD_STRING(pstBuffer, "\r\n");

    ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_MESSAGE_BODY)(pstSipMsg, pstBuffer);
    if (ulRet!= SUCCESS)
    {
        return ulRet;
    }

    return SUCCESS;
}

/*******************************************************************************
Request-Line   =  Method SP Request-URI SP SIP-Version CRLF
*******************************************************************************/
ULONG SIP_CodeRequestLine(void *pstStruct,
                          SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    SIP_REQUEST_LINE_S *pstRequestLine = NULL_PTR;

    pstRequestLine = (SIP_REQUEST_LINE_S *)pstStruct;
    ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_METHOD)(&pstRequestLine->eMethod, pstBuffer);
    if (ulRet!= SUCCESS)
    {
        return ulRet;
    }

    SYNTAX_ADD_STRING(pstBuffer, " ");

    ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_REQUEST_URI)(pstRequestLine->pstRequestURI, pstBuffer);
    if (ulRet!= SUCCESS)
    {
        return ulRet;
    }

    SYNTAX_ADD_STRING(pstBuffer, " ");

    ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_SIP_VERSION)(&pstRequestLine->ucVersion, pstBuffer);
    if (ulRet!= SUCCESS)
    {
        return ulRet;
    }

    SYNTAX_ADD_STRING(pstBuffer, "\r\n");
    return SUCCESS;
}

/*******************************************************************************
Status-Line     =  SIP-Version SP Status-Code SP Reason-Phrase CRLF
*******************************************************************************/
ULONG SIP_CodeStatusLine(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    SIP_STATUS_LINE_S *pstStateLine = NULL_PTR;

    pstStateLine = (SIP_STATUS_LINE_S *)pstStruct;
    ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_SIP_VERSION)(&pstStateLine->ucVersion, pstBuffer);
    if (ulRet!= SUCCESS)
    {
        return ulRet;
    }

    SYNTAX_ADD_STRING(pstBuffer, " ");

    ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_STATUS_CODE)(&pstStateLine->eStatusCode, pstBuffer);
    if (ulRet!= SUCCESS)
    {
        return ulRet;
    }

    SYNTAX_ADD_STRING(pstBuffer, " ");

    ulRet = SIP_CodeReasonPhrase(&pstStateLine->eStatusCode, pstBuffer);
    if (ulRet!= SUCCESS)
    {
        return ulRet;
    }

    SYNTAX_ADD_STRING(pstBuffer, "\r\n");
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
ULONG SIP_CodeMessageHeader(void *pstStruct,
                            SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    ULONG ulLoop;
    ULONG ulAppIndex;
    SIP_MSG_S    *pstSipMsg  = NULL_PTR;
    SIP_HEADER_S *pstHeader  = NULL_PTR;

    pstSipMsg = (SIP_MSG_S *)pstStruct;
    for (ulLoop = 0; ulLoop < SIP_HEADER_BUTT; ulLoop++)
    {
        ulAppIndex = SIP_GET_HEADER_RULE_INDEX(ulLoop);
        if ((ulAppIndex >= SIP_ABNF_RULE_BUTT) 
          ||(SIP_GET_CODE_FUNC(ulAppIndex)== NULL_PTR))
        {
            /* 没有编码函数不处理 */
            continue;
        }

        /* 可能存在多个相同头域，挨个编码 */
        pstHeader = pstSipMsg->apstHeaders[ulLoop];
        while (pstHeader!= NULL_PTR)
        {
            ulRet = SIP_GET_CODE_FUNC(ulAppIndex)(pstHeader,
                                                  pstBuffer);
            if (ulRet == SUCCESS)
            {
                SYNTAX_ADD_STRING(pstBuffer, "\r\n");
            }
            else
            {
                return FAIL;
            }

            pstHeader = pstHeader->pstNext;
        }
    }

    return SUCCESS;
}

/*******************************************************************************
message-body  =  *OCTET
*******************************************************************************/
ULONG SIP_CodeMessageBody(void *pstStruct,
                          SYNTAX_BUFFER_S *pstBuffer)
{
    SIP_MSG_S *pstSipMsg = NULL_PTR;

    pstStruct = (SIP_MSG_S *)pstStruct;
    return SUCCESS;
}

/*******************************************************************************
Method            =  INVITEm / ACKm / OPTIONSm / BYEm
                     / CANCELm / REGISTERm
                     / extension-method
*******************************************************************************/
ULONG SIP_CodeMethod(void *pstStruct,
                     SYNTAX_BUFFER_S *pstBuffer)
{
    SIP_METHOD_E eMethod;

    eMethod = *((SIP_METHOD_E *)pstStruct);
    switch(eMethod)
    {
        case SIP_METHOD_INVITE:
            SYNTAX_ADD_STRING(pstBuffer, "INVITE");
            break;

        case SIP_METHOD_ACK:
            SYNTAX_ADD_STRING(pstBuffer, "ACK");
            break;

        case SIP_METHOD_OPTIONS:
            SYNTAX_ADD_STRING(pstBuffer,"OPTIONS");
            break;

        case SIP_METHOD_BYE:
            SYNTAX_ADD_STRING(pstBuffer, "BYE");
            break;

        case SIP_METHOD_CANCEL:
            SYNTAX_ADD_STRING(pstBuffer, "CANCEL");
            break;

        case SIP_METHOD_REGISTER:
            SYNTAX_ADD_STRING(pstBuffer, "REGISTER");
            break;

       default:
            return FAIL;
    }

    return SUCCESS;
}

/*******************************************************************************
Request-URI    =  SIP-URI / SIPS-URI / absoluteURI
*******************************************************************************/
ULONG SIP_CodeRequestURI(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer)
{
    URI_S *pstRequestURI = NULL_PTR;

    pstRequestURI = (URI_S *)pstStruct;
    if (pstRequestURI->eUriType == URI_TYPE_SIP)
    {
        return SIP_GET_CODE_FUNC(SIP_ABNF_RULE_SIP_URI)(&pstRequestURI->u.stSipUri, pstBuffer);
    }
    else if (pstRequestURI->eUriType == URI_TYPE_SIPS)
    {
        return SIP_GET_CODE_FUNC(SIP_ABNF_RULE_SIPS_URI)(&pstRequestURI->u.stSipUri, pstBuffer);
    }

    return FAIL;
}

/*******************************************************************************
SIP-Version    =  "SIP" "/" 1*DIGIT "." 1*DIGIT
*******************************************************************************/
ULONG SIP_CodeSIPVersion(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer)
{
    UCHAR ucVersion;

    ucVersion = *((UCHAR *)pstStruct);
    SYNTAX_ADD_STRING(pstBuffer, "SIP/");
    SYNTAX_ADD_DEC_NUM(pstBuffer, ucVersion);
    /*小数部分直接设0*/
    SYNTAX_ADD_STRING(pstBuffer, ".0");
    return SUCCESS;
}

/*******************************************************************************
SIP-URI          =  "sip:" [ userinfo ] hostport
                    uri-parameters [ headers ]
*******************************************************************************/
ULONG SIP_CodeSipURI(void *pstStruct,
                     SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    URI_USER_INFO_S *pstUserInfo = NULL_PTR;
    SIP_URI_S *pstSipURI = NULL_PTR;

    pstSipURI = (SIP_URI_S *)pstStruct;

    SYNTAX_ADD_STRING(pstBuffer, "sip:");

    if (pstSipURI->pstUserInfo != NULL_PTR)
    {
        ulRet= SIP_GET_CODE_FUNC(SIP_ABNF_RULE_USERINFO)(pstSipURI->pstUserInfo, pstBuffer);
        if (ulRet!= SUCCESS)
        {
            return ulRet;
        }
    }

    ulRet= SIP_GET_CODE_FUNC(SIP_ABNF_RULE_HOSTPORT)(&pstSipURI->stHostPort, pstBuffer);
    if (ulRet!= SUCCESS)
    {
        return ulRet;
    }

    /*以后再添加*/

    return SUCCESS;
}

/*******************************************************************************
SIPS-URI         =  "sips:" [ userinfo ] hostport
                    uri-parameters [ headers ]
*******************************************************************************/
ULONG SIP_CodeSipsURI(void *pstStruct,
                      SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    URI_USER_INFO_S *pstUserInfo = NULL_PTR;
    SIP_URI_S *pstSipsURI = NULL_PTR;

    pstSipsURI = (SIP_URI_S *)pstStruct;
    SYNTAX_ADD_STRING(pstBuffer, "sips:");

    if (pstSipsURI->pstUserInfo != NULL_PTR)
    {
        ulRet= SIP_GET_CODE_FUNC(SIP_ABNF_RULE_USERINFO)(pstSipsURI->pstUserInfo, pstBuffer);
        if (ulRet!= SUCCESS)
        {
            return ulRet;
        }
    }

    ulRet= SIP_GET_CODE_FUNC(SIP_ABNF_RULE_HOSTPORT)(&pstSipsURI->stHostPort, pstBuffer);
    if (ulRet!= SUCCESS)
    {
        return ulRet;
    }

    /*以后再添加*/

    return SUCCESS;
}

/*******************************************************************************
userinfo         =  ( user / telephone-subscriber ) [ ":" password ] "@"
*******************************************************************************/
ULONG SIP_CodeUserinfo(void *pstStruct,
                       SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    UCHAR *pucString = NULL_PTR;
    URI_USER_INFO_S *pstUserInfo = NULL_PTR;

    pstUserInfo = (URI_USER_INFO_S *)pstStruct;
    pucString   = pstUserInfo->pucUserInfo;
    ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_USER)(pucString, pstBuffer);
    if (ulRet!= SUCCESS)
    {
        return ulRet;
    }

    if (pstUserInfo->pucPassword!= NULL_PTR)
    {
        SYNTAX_ADD_STRING(pstBuffer, ":");
        pucString = pstUserInfo->pucPassword;
        SYNTAX_ADD_STRING(pstBuffer, pucString);
    }

    SYNTAX_ADD_STRING(pstBuffer, "@");

    return SUCCESS;
}

/*******************************************************************************
userinfo         =  ( user / telephone-subscriber ) [ ":" password ] "@"
*******************************************************************************/
ULONG SIP_CodeUser(void *pstStruct,
                   SYNTAX_BUFFER_S *pstBuffer)
{
    UCHAR *pucUserInfo = NULL_PTR;

    pucUserInfo = (UCHAR *)pstStruct;
    SYNTAX_ADD_STRING(pstBuffer, pucUserInfo);
    return SUCCESS;
}

/*******************************************************************************
hostport         =  host [ ":" port ]
*******************************************************************************/
ULONG SIP_CodeHostport(void *pstStruct,
                       SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    URI_HOST_PORT_S *pstHostPort = NULL_PTR;

    pstHostPort = (URI_HOST_PORT_S *)pstStruct;
    ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_HOST)(&pstHostPort->stHost, pstBuffer);
    if (ulRet!= SUCCESS)
    {
        return ulRet;
    }

    if (pstHostPort->usPort != NULL_USHORT)
    {
        SYNTAX_ADD_STRING(pstBuffer, ":");
        ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_PORT)(&pstHostPort->usPort, pstBuffer);
    }

    return SUCCESS;
}

/*******************************************************************************
host             =  hostname / IPv4address / IPv6reference
*******************************************************************************/
ULONG SIP_CodeHost(void *pstStruct,
                   SYNTAX_BUFFER_S *pstBuffer)
{
    UCHAR      *pucString = NULL_PTR;
    URI_HOST_S *pstHost = NULL_PTR;

    pstHost = (URI_HOST_S*)pstStruct;
    pucString = pstHost->pucAddrStr;
    SYNTAX_ADD_STRING(pstBuffer, pucString);

    return SUCCESS;
}

/*******************************************************************************
port           =  1*DIGIT
*******************************************************************************/
ULONG SIP_CodePort(void *pstStruct,
                   SYNTAX_BUFFER_S *pstBuffer)
{
    USHORT usPort;

    usPort = *((USHORT *)pstStruct);
    SYNTAX_ADD_DEC_NUM(pstBuffer, usPort);
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
ULONG SIP_CodeStatusCode(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer)
{
    SIP_STATUS_CODE_E eStatusCode;

    eStatusCode = *((SIP_STATUS_CODE_E *)pstStruct);
    SYNTAX_ADD_DEC_NUM(pstBuffer, eStatusCode);

    return SUCCESS;
}

/*******************************************************************************
Reason-Phrase   =  *(reserved / unreserved / escaped
                   / UTF8-NONASCII / UTF8-CONT / SP / HTAB)
*******************************************************************************/
ULONG SIP_CodeReasonPhrase(void *pstStruct,
                           SYNTAX_BUFFER_S *pstBuffer)
{
    SIP_STATUS_CODE_E eStatusCode;

    eStatusCode = *((SIP_STATUS_CODE_E *)pstStruct);
    switch (eStatusCode)
    {
        case SIP_STATUS_CODE_100:
            SYNTAX_ADD_STRING(pstBuffer, "Trying");
            break;

        case SIP_STATUS_CODE_180:
            SYNTAX_ADD_STRING(pstBuffer, "Ringing");
            break;

        case SIP_STATUS_CODE_181:
            SYNTAX_ADD_STRING(pstBuffer, "Call Is Being Forwarded");
            break;

        case SIP_STATUS_CODE_182:
            SYNTAX_ADD_STRING(pstBuffer, "Queued");
            break;

        case SIP_STATUS_CODE_183:
            SYNTAX_ADD_STRING(pstBuffer, "Session Progress");
            break;

        case SIP_STATUS_CODE_200:
            SYNTAX_ADD_STRING(pstBuffer, "OK");
            break;

        case SIP_STATUS_CODE_300:
            SYNTAX_ADD_STRING(pstBuffer, "Multiple Choices");
            break;

        case SIP_STATUS_CODE_301:
            SYNTAX_ADD_STRING(pstBuffer, "Moved Permanently");
            break;

        case SIP_STATUS_CODE_302:
            SYNTAX_ADD_STRING(pstBuffer, "Moved Temporarily");
            break;

        case SIP_STATUS_CODE_305:
            SYNTAX_ADD_STRING(pstBuffer, "Use Proxy");
            break;

        case SIP_STATUS_CODE_380:
            SYNTAX_ADD_STRING(pstBuffer, "Alternative Service");
            break;

        case SIP_STATUS_CODE_400:
            SYNTAX_ADD_STRING(pstBuffer, "Bad Request");
            break;

        case SIP_STATUS_CODE_401:
            SYNTAX_ADD_STRING(pstBuffer, "Unauthorized");
            break;

        case SIP_STATUS_CODE_402:
            SYNTAX_ADD_STRING(pstBuffer, "Payment Required");
            break;

        case SIP_STATUS_CODE_403:
            SYNTAX_ADD_STRING(pstBuffer, "Forbidden");
            break;

        case SIP_STATUS_CODE_404:
            SYNTAX_ADD_STRING(pstBuffer, "Not Found");
            break;

        case SIP_STATUS_CODE_405:
            SYNTAX_ADD_STRING(pstBuffer, "Method Not Allowed");
            break;

        case SIP_STATUS_CODE_406:
            SYNTAX_ADD_STRING(pstBuffer, "Not Acceptable");
            break;

        case SIP_STATUS_CODE_407:
            SYNTAX_ADD_STRING(pstBuffer, "Proxy Authentication Required");
            break;

        case SIP_STATUS_CODE_408:
            SYNTAX_ADD_STRING(pstBuffer, "Request Timeout");
            break;

        case SIP_STATUS_CODE_410:
            SYNTAX_ADD_STRING(pstBuffer, "Gone");
            break;

        case SIP_STATUS_CODE_413:
            SYNTAX_ADD_STRING(pstBuffer, "Request Entity Too Large");
            break;

        case SIP_STATUS_CODE_414:
            SYNTAX_ADD_STRING(pstBuffer, "Request-URI Too Long");
            break;

        case SIP_STATUS_CODE_415:
            SYNTAX_ADD_STRING(pstBuffer, "Unsupported Media Type");
            break;

        case SIP_STATUS_CODE_416:
            SYNTAX_ADD_STRING(pstBuffer, "Unsupported URI Scheme");
            break;

        case SIP_STATUS_CODE_420:
            SYNTAX_ADD_STRING(pstBuffer, "Bad Extensione");
            break;

        case SIP_STATUS_CODE_421:
            SYNTAX_ADD_STRING(pstBuffer, "Extension Required");
            break;

        case SIP_STATUS_CODE_423:
            SYNTAX_ADD_STRING(pstBuffer, "Interval Too Brief");
            break;

        case SIP_STATUS_CODE_480:
            SYNTAX_ADD_STRING(pstBuffer, "Temporarily Unavailable");
            break;

        case SIP_STATUS_CODE_481:
            SYNTAX_ADD_STRING(pstBuffer, "Call/Transaction Does Not Exist");
            break;

        case SIP_STATUS_CODE_482:
            SYNTAX_ADD_STRING(pstBuffer, "Loop Detected");
            break;

        case SIP_STATUS_CODE_483:
            SYNTAX_ADD_STRING(pstBuffer, "Too Many Hops");
            break;

        case SIP_STATUS_CODE_484:
            SYNTAX_ADD_STRING(pstBuffer, "Address Incomplete");
            break;

        case SIP_STATUS_CODE_485:
            SYNTAX_ADD_STRING(pstBuffer, "Ambiguous");
            break;

        case SIP_STATUS_CODE_486:
            SYNTAX_ADD_STRING(pstBuffer,"Busy Here");
            break;

        case SIP_STATUS_CODE_487:
            SYNTAX_ADD_STRING(pstBuffer, "Request Terminated");
            break;

        case SIP_STATUS_CODE_488:
            SYNTAX_ADD_STRING(pstBuffer, "Not Acceptable Here");
            break;

        case SIP_STATUS_CODE_491:
            SYNTAX_ADD_STRING(pstBuffer, "Request Pending");
            break;

        case SIP_STATUS_CODE_493:
            SYNTAX_ADD_STRING(pstBuffer, "Undecipherable");
            break;

        case SIP_STATUS_CODE_500:
            SYNTAX_ADD_STRING(pstBuffer, "Server Internal Error");
            break;

        case SIP_STATUS_CODE_501:
            SYNTAX_ADD_STRING(pstBuffer, "Not Implemented");
            break;

        case SIP_STATUS_CODE_502:
            SYNTAX_ADD_STRING(pstBuffer, "Bad Gateway");
            break;

        case SIP_STATUS_CODE_503:
            SYNTAX_ADD_STRING(pstBuffer, "Service Unavailable");
            break;

        case SIP_STATUS_CODE_504:
            SYNTAX_ADD_STRING(pstBuffer, "Server Time-out");
            break;

        case SIP_STATUS_CODE_505:
            SYNTAX_ADD_STRING(pstBuffer, "Version Not Supported");
            break;

        case SIP_STATUS_CODE_513:
            SYNTAX_ADD_STRING(pstBuffer, "Message Too Large");
            break;

        case SIP_STATUS_CODE_600:
            SYNTAX_ADD_STRING(pstBuffer, "Busy Everywhere");
            break;

        case SIP_STATUS_CODE_603:
            SYNTAX_ADD_STRING(pstBuffer, "Decline");
            break;

        case SIP_STATUS_CODE_604:
            SYNTAX_ADD_STRING(pstBuffer, "Does Not Exist Anywhere");
            break;

        case SIP_STATUS_CODE_606:
            SYNTAX_ADD_STRING(pstBuffer, "Not Acceptable");
            break;

        default:
            SYNTAX_ADD_STRING(pstBuffer, "Unknown Status Code");
            break;
    }

    return SUCCESS;
}

/*******************************************************************************
from-spec   =  ( name-addr / addr-spec )
               *( SEMI from-param )
*******************************************************************************/
ULONG SIP_CodeFromSpec(void *pstStruct,
                       SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    SIP_HEADER_FROM_S *pstFrom = NULL_PTR;
    URI_S             *pstUri = NULL_PTR;

    pstFrom = (SIP_HEADER_FROM_S *)pstStruct;
    if (pstFrom->stNameAddr.bName == TRUE)
    {
        ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_NAME_ADDR)(&pstFrom->stNameAddr, pstBuffer);
    }
    else
    {
        pstUri = pstFrom->stNameAddr.pstUri;
        ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_ADDR_SPEC)(pstUri, pstBuffer);
    }

    if (ulRet!= SUCCESS)
    {
        return ulRet;
    }

    ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_FROM_PARAM)(pstFrom, pstBuffer);
    if (ulRet!= SUCCESS)
    {
        return ulRet;
    }

    return SUCCESS;
}

/*******************************************************************************
from-param  =  tag-param / generic-param
*******************************************************************************/
ULONG SIP_CodeFromParam(void *pstStruct,
                        SYNTAX_BUFFER_S *pstBuffer)
{
    UCHAR *pucString = NULL_PTR;
    SIP_HEADER_FROM_S *pstFrom = NULL_PTR;

    pstFrom = (SIP_HEADER_FROM_S *)pstStruct;
    if (pstFrom->pucTag != NULL_PTR)
    {
        SYNTAX_ADD_STRING(pstBuffer, ";");
        SYNTAX_ADD_STRING(pstBuffer, "tag=");
        pucString = pstFrom->pucTag;
        SYNTAX_ADD_STRING(pstBuffer, pucString);
    }

    return SUCCESS;
}

/*******************************************************************************
to-param  =  tag-param / generic-param
*******************************************************************************/
ULONG SIP_CodeToParam(void *pstStruct,
                      SYNTAX_BUFFER_S *pstBuffer)
{
    UCHAR *pucString = NULL_PTR;
    SIP_HEADER_TO_S *pstTo = NULL_PTR;

    pstTo = (SIP_HEADER_TO_S *)pstStruct;
    if (pstTo->pucTag != NULL_PTR)
    {
        SYNTAX_ADD_STRING(pstBuffer, ";");
        SYNTAX_ADD_STRING(pstBuffer, "tag=");
        pucString = pstTo->pucTag;
        SYNTAX_ADD_STRING(pstBuffer, pucString);
    }

    return SUCCESS;
}

/*******************************************************************************
via-parm          =  sent-protocol LWS sent-by *( SEMI via-params )
*******************************************************************************/
ULONG SIP_CodeViaParm(void *pstStruct,
                      SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    SIP_VIA_PARM_S *pstViaParm = NULL_PTR;

    pstViaParm = (SIP_VIA_PARM_S *)pstStruct;
    ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_SENT_PROTOCOL)(&pstViaParm->eProtocolType, pstBuffer);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }

    SYNTAX_ADD_STRING(pstBuffer, " ");

    ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_SENT_BY)(&pstViaParm->stSendBy, pstBuffer);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }

    ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_VIA_PARMS)(pstViaParm, pstBuffer);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }

    return SUCCESS;
}

/*******************************************************************************
sent-protocol     =  protocol-name SLASH protocol-version
                     SLASH transport
*******************************************************************************/
ULONG SIP_CodeSentProtocol(void *pstStruct,
                           SYNTAX_BUFFER_S *pstBuffer)
{
    SIP_TRANSPORT_PROTOCOL_E  eProtocolType;

    eProtocolType = *((SIP_TRANSPORT_PROTOCOL_E *)pstStruct);

    SYNTAX_ADD_STRING(pstBuffer, "SIP/2.0/");

    return SIP_GET_CODE_FUNC(SIP_ABNF_RULE_TRANSPORT)(&eProtocolType, pstBuffer);
}

/*******************************************************************************
transport         =  "UDP" / "TCP" / "TLS" / "SCTP"
                     / other-transport
*******************************************************************************/
ULONG SIP_CodeTransport(void *pstStruct,
                        SYNTAX_BUFFER_S *pstBuffer)
{
    SIP_TRANSPORT_PROTOCOL_E  eProtocolType;

    eProtocolType = *((SIP_TRANSPORT_PROTOCOL_E *)pstStruct);
    switch (eProtocolType)
    {
        case SIP_TRANSPORT_PROTOCOL_TCP:
            SYNTAX_ADD_STRING(pstBuffer, "TCP");
            break;

        case SIP_TRANSPORT_PROTOCOL_TLS:
            SYNTAX_ADD_STRING(pstBuffer, "TLS");
            break;

        case SIP_TRANSPORT_PROTOCOL_UDP:
            SYNTAX_ADD_STRING(pstBuffer, "UDP");
            break;

        case SIP_TRANSPORT_PROTOCOL_SCTP:
            SYNTAX_ADD_STRING(pstBuffer, "SCTP");
            break;

        default:
            return FAIL;
    }

    return SUCCESS;
}

/*******************************************************************************
sent-by           =  host [ COLON port ]
*******************************************************************************/
ULONG SIP_CodeSentBy(void *pstStruct,
                     SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    URI_HOST_PORT_S *pstSendBy = NULL_PTR;

    pstSendBy = (URI_HOST_PORT_S *)pstStruct;
    ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_HOST)(&pstSendBy->stHost, pstBuffer);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }

    if (pstSendBy->usPort != NULL_USHORT)
    {
        SYNTAX_ADD_STRING(pstBuffer, ":");
        ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_PORT)(&pstSendBy->usPort, pstBuffer);
        if (ulRet != SUCCESS)
        {
            return ulRet;
        }
    }

    return SUCCESS;
}

/*******************************************************************************
via-params        =  via-ttl / via-maddr
                     / via-received / via-branch
                     / via-extension
*******************************************************************************/
ULONG SIP_CodeViaParams(void *pstStruct,
                        SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    UCHAR      *pucString = NULL_PTR;
    URI_HOST_S *pstMaddr = NULL_PTR;
    SIP_VIA_RECEIVED_S *pstReceived = NULL_PTR;
    SIP_VIA_PARM_S *pstViaParm = NULL_PTR;

    pstViaParm = (SIP_VIA_PARM_S *)pstStruct;
    if (pstViaParm->ulTtl != NULL_ULONG)
    {
        SYNTAX_ADD_STRING(pstBuffer, ";");
        ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_VIA_TTL)(&pstViaParm->ulTtl, pstBuffer);
        if (ulRet!= SUCCESS)
        {
            return FAIL;
        }
    }

    if (pstViaParm->pucBranch != NULL_PTR)
    {
        SYNTAX_ADD_STRING(pstBuffer, ";");
        pucString = pstViaParm->pucBranch;
        ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_VIA_BRANCH)(pucString, pstBuffer);
        if (ulRet!= SUCCESS)
        {
            return FAIL;
        }
    }

    if (pstViaParm->pstMaddr != NULL_PTR)
    {
        SYNTAX_ADD_STRING(pstBuffer, ";");
        pstMaddr = pstViaParm->pstMaddr;
        ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_VIA_MADDR)(pstMaddr, pstBuffer);
        if (ulRet!= SUCCESS)
        {
            return FAIL;
        }
    }

    if (pstViaParm->pstReceived != NULL_PTR)
    {
        SYNTAX_ADD_STRING(pstBuffer, ";");
        pstReceived = pstViaParm->pstReceived;
        ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_VIA_RECEIVED)(pstReceived, pstBuffer);
        if (ulRet!= SUCCESS)
        {
            return FAIL;
        }
    }

    return SUCCESS;
}

/*******************************************************************************
via-ttl           =  "ttl" EQUAL ttl
*******************************************************************************/
ULONG SIP_CodeViaTtl(void *pstStruct,
                     SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulTtl;

    ulTtl = *((ULONG *)pstStruct);
    SYNTAX_ADD_STRING(pstBuffer, "ttl=");
    SYNTAX_ADD_DEC_NUM(pstBuffer, ulTtl);
    return SUCCESS;
}

/*******************************************************************************
via-maddr         =  "maddr" EQUAL host
*******************************************************************************/
ULONG SIP_CodeViaMaddr(void *pstStruct,
                       SYNTAX_BUFFER_S *pstBuffer)
{
    URI_HOST_S *pstMaddr = NULL_PTR;

    pstMaddr= (URI_HOST_S *)pstStruct;
    SYNTAX_ADD_STRING(pstBuffer,"maddr=");
    return SIP_GET_CODE_FUNC(SIP_ABNF_RULE_HOST)(pstMaddr, pstBuffer);
}

/*******************************************************************************
via-received      =  "received" EQUAL (IPv4address / IPv6address)
*******************************************************************************/
ULONG SIP_CodeViaReceived(void *pstStruct,
                          SYNTAX_BUFFER_S *pstBuffer)
{
    UCHAR *pucString = NULL_PTR;
    SIP_VIA_RECEIVED_S *pstReceived = NULL_PTR;

    pstReceived = (SIP_VIA_RECEIVED_S *)pstStruct;
    SYNTAX_ADD_STRING(pstBuffer, "received=");
    if (pstReceived->eIpType == SIP_IP_TYPE_IPV4)
    {
        pucString = pstReceived->u.pucIPV4;
        SYNTAX_ADD_STRING(pstBuffer, pucString);
    }
    else
    {
        pucString = pstReceived->u.pucIPV6;
        SYNTAX_ADD_STRING(pstBuffer, pucString);
    }

    return SUCCESS;
}

/*******************************************************************************
via-branch        =  "branch" EQUAL token
*******************************************************************************/
ULONG SIP_CodeViaBranch(void *pstStruct,
                        SYNTAX_BUFFER_S *pstBuffer)
{
    UCHAR *pucBranch = NULL_PTR;
    pucBranch = (UCHAR *)pstStruct;

    SYNTAX_ADD_STRING(pstBuffer, "branch=");
    SYNTAX_ADD_STRING(pstBuffer, pucBranch);
    return SUCCESS;
}

/*******************************************************************************
name-addr      =  [ display-name ] LAQUOT addr-spec RAQUOT
*******************************************************************************/
ULONG SIP_CodeNameAddr(void *pstStruct,
                       SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    UCHAR *pucString = NULL_PTR;
    SIP_NAME_ADDR_S *pstNameAddr = NULL_PTR;
    URI_S           *pstUri = NULL_PTR;

    pstNameAddr = (SIP_NAME_ADDR_S *)pstStruct;
    if (pstNameAddr->pucName != NULL_PTR)
    {
        pucString = pstNameAddr->pucName;
        SYNTAX_ADD_STRING(pstBuffer, pucString);
    }

    SYNTAX_ADD_STRING(pstBuffer, "<");

    pstUri = pstNameAddr->pstUri;
    ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_ADDR_SPEC)(pstUri, pstBuffer);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }

    SYNTAX_ADD_STRING(pstBuffer, ">");

    return SUCCESS;
}

/*******************************************************************************
addr-spec      =  SIP-URI / SIPS-URI / absoluteURI
*******************************************************************************/
ULONG SIP_CodeAddrSpec(void *pstStruct,
                       SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    URI_S *pstAddrSpec = NULL_PTR;

    pstAddrSpec = (URI_S *)pstStruct;
    switch (pstAddrSpec->eUriType)
    {
        case URI_TYPE_SIP:
            ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_SIP_URI)(&pstAddrSpec->u.stSipUri, pstBuffer);
            break;

        case URI_TYPE_SIPS:
            ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_SIPS_URI)(&pstAddrSpec->u.stSipUri, pstBuffer);
            break;

        default:
            return FAIL;
    }

    return ulRet;
}

/*******************************************************************************
contact-param  =  (name-addr / addr-spec) *(SEMI contact-params)
*******************************************************************************/
ULONG SIP_CodeContactParam(void *pstStruct,
                           SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    SIP_CONTACT_PARAM_S *pstContactParam = NULL_PTR;
    URI_S               *pstUri = NULL_PTR;

    pstContactParam = (SIP_CONTACT_PARAM_S *)pstStruct;
    if (pstContactParam->stAddr.bName == TRUE)
    {
        ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_NAME_ADDR)(&pstContactParam->stAddr, pstBuffer);
    }
    else
    {
        pstUri = pstContactParam->stAddr.pstUri;
        ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_ADDR_SPEC)(pstUri, pstBuffer);
    }

    if (ulRet!= SUCCESS)
    {
        return ulRet;
    }

    /* 参数以后再加 */
    
    return SUCCESS;
}

/* 所有的头域 */
/*******************************************************************************
Accept         =  "Accept" HCOLON
                   [ accept-range *(COMMA accept-range) ]
*******************************************************************************/
ULONG SIP_CodeHeaderAccept(void *pstStruct,
                           SYNTAX_BUFFER_S *pstBuffer)
{
    SIP_HEADER_ACCEPT_S *pstAccept = NULL_PTR;

    pstAccept = (SIP_HEADER_ACCEPT_S *)pstStruct;

    SYNTAX_ADD_STRING(pstBuffer, "Accept: ");

    return SUCCESS;
}

/*******************************************************************************
Accept-Encoding  =  "Accept-Encoding" HCOLON
                     [ encoding *(COMMA encoding) ]
*******************************************************************************/
ULONG SIP_CodeHeaderAcceptEncoding(void *pstStruct,
                                   SYNTAX_BUFFER_S *pstBuffer)
{
    SIP_HEADER_ACCEPT_ENCODING_S *pstAcceptEncoding = NULL_PTR;

    pstAcceptEncoding = (SIP_HEADER_ACCEPT_ENCODING_S *)pstStruct;
    SYNTAX_ADD_STRING(pstBuffer, "Accept-Encoding: ");

    return SUCCESS;
}

/*******************************************************************************
Call-ID  =  ( "Call-ID" / "i" ) HCOLON callid
*******************************************************************************/
ULONG SIP_CodeHeaderCallID(void *pstStruct,
                           SYNTAX_BUFFER_S *pstBuffer)
{
    SIP_HEADER_CALL_ID_S *pstCallID= NULL_PTR;
    UCHAR *pucString = NULL_PTR;

    pstCallID = (SIP_HEADER_CALL_ID_S *)pstStruct;

    SYNTAX_ADD_STRING(pstBuffer, "Call-ID: ");
    pucString = pstCallID->pucCallID;
    SYNTAX_ADD_STRING(pstBuffer, pucString);
    return SUCCESS;
}

/*******************************************************************************
CSeq  =  "CSeq" HCOLON 1*DIGIT LWS Method
*******************************************************************************/
ULONG SIP_CodeHeaderCseq(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    SIP_HEADER_CSEQ_S *pstCseq = NULL_PTR;

    pstCseq = (SIP_HEADER_CSEQ_S *)pstStruct;

    SYNTAX_ADD_STRING(pstBuffer, "CSeq: ");
    SYNTAX_ADD_DEC_NUM(pstBuffer, pstCseq->ulSeq);
    SYNTAX_ADD_STRING(pstBuffer, " ");

    ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_METHOD)(&pstCseq->eMethod, pstBuffer);

    return ulRet;
}

/*******************************************************************************
From        =  ( "From" / "f" ) HCOLON from-spec
*******************************************************************************/
ULONG SIP_CodeHeaderFrom(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    SIP_HEADER_FROM_S *pstFrom = NULL_PTR;

    pstFrom = (SIP_HEADER_FROM_S *)pstStruct;
    SYNTAX_ADD_STRING(pstBuffer, "From: ");

    ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_FROM_SPEC)(pstFrom, pstBuffer);
    return ulRet;
}

/*******************************************************************************
To        =  ( "To" / "t" ) HCOLON ( name-addr
             / addr-spec ) *( SEMI to-param )
*******************************************************************************/
ULONG SIP_CodeHeaderTo(void *pstStruct,
                       SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    SIP_HEADER_TO_S *pstTo = NULL_PTR;
    URI_S *pstUri = NULL_PTR;

    pstTo = (SIP_HEADER_TO_S *)pstStruct;

    SYNTAX_ADD_STRING(pstBuffer, "To: ");

    if (pstTo->stNameAddr.bName == TRUE)
    {
        ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_NAME_ADDR)(&pstTo->stNameAddr, pstBuffer);
    }
    else
    {
        pstUri = pstTo->stNameAddr.pstUri;
        ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_ADDR_SPEC)(pstUri, pstBuffer);
    }

    if (ulRet!= SUCCESS)
    {
        return ulRet;
    }

    ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_TO_PARAM)(pstTo, pstBuffer);
    if (ulRet!= SUCCESS)
    {
        return ulRet;
    }

    return SUCCESS;
}

/*******************************************************************************
Via               =  ( "Via" / "v" ) HCOLON via-parm *(COMMA via-parm)
*******************************************************************************/
ULONG SIP_CodeHeaderVia(void *pstStruct,
                        SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    SIP_VIA_PARM_S   *pstViaParm = NULL_PTR;
    SIP_HEADER_VIA_S *pstVia = NULL_PTR;
    BOOL  bFirst = TRUE;

    pstVia = (SIP_HEADER_VIA_S *)pstStruct;
    SYNTAX_ADD_STRING(pstBuffer, "Via: ");

    pstViaParm = pstVia->pstViaParm;
    while(pstViaParm != NULL_PTR)
    {
        if (bFirst != TRUE)
        {
            SYNTAX_ADD_STRING(pstBuffer, ",");
        }

        ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_VIA_PARM)(pstViaParm, pstBuffer);
        if (ulRet != NULL_PTR)
        {
            return ulRet;
        }

        bFirst = FALSE;
        pstViaParm = pstViaParm->pstNext;
    }

    return SUCCESS;
}

/*******************************************************************************
Max-Forwards  =  "Max-Forwards" HCOLON 1*DIGIT
*******************************************************************************/
ULONG SIP_CodeHeaderMaxForwards(void *pstStruct,
                                SYNTAX_BUFFER_S *pstBuffer)
{
    SIP_HEADER_MAX_FORWARDS_S *pstMaxForwards = NULL_PTR;

    pstMaxForwards = (SIP_HEADER_MAX_FORWARDS_S *)pstStruct;

    SYNTAX_ADD_STRING(pstBuffer, "Max-Forwards: ");
    SYNTAX_ADD_DEC_NUM(pstBuffer, pstMaxForwards->ulMaxForwards);

    return SUCCESS;
}

/*******************************************************************************
Contact        =  ("Contact" / "m" ) HCOLON
                  ( STAR / (contact-param *(COMMA contact-param)))
*******************************************************************************/
ULONG SIP_CodeHeaderContact(void *pstStruct,
                            SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    SIP_CONTACT_PARAM_S   *pstContactParam = NULL_PTR;
    SIP_HEADER_CONTACT_S  *pstContact      = NULL_PTR;
    BOOL  bFirst = TRUE;

    pstContact = (SIP_HEADER_CONTACT_S *)pstStruct;
    SYNTAX_ADD_STRING(pstBuffer, "Contact: ");

    if (pstContact->ucIsStar == TRUE)
    {
        SYNTAX_ADD_STRING(pstBuffer, "*");    
        return SUCCESS;
    }

    pstContactParam = pstContact->pstParam;
    while (pstContactParam != NULL_PTR)
    {
        if (bFirst != TRUE)
        {
            SYNTAX_ADD_STRING(pstBuffer, ",");
        }

        ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_CONTACT_PARAM)(pstContactParam, pstBuffer);
        if (ulRet != NULL_PTR)
        {
            return ulRet;
        }

        bFirst = FALSE;
        pstContactParam = pstContactParam->pstNext;
    }

    return SUCCESS;
}

