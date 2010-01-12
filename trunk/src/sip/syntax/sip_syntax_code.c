/*******************************************************************************
����    : SIP��Ϣ���뺯��
������  : �ƴ�ƽ
��������: 200.01.10
�޸ļ�¼:

����ԭ��:
1�� ���еı��뺯���β�һ��
    UBUF_HEADER_S *pstUbufMsg     : �洢�ṹ��UBUF
    void *pstStruct               : ����ṹ��ָ�룬�ṹ����ݺ�����ͬ����ͬ
    SIP_SYNTAX_BUFFER_S *pstBuffer: ���뻺��ṹ
2�� ÿ��������Ҫ����������ȷpstStruct�Ľṹ����
*******************************************************************************/
/* �ⲿ����ģ��ͷ�ļ� */
#include "common\common.h"
#include "abnf\abnf.h"
#include "ubuf\ubuf.h"
#include "sip\uri.h"

/* ��ģ������ṩ�ĳ����ͽṹͷ�ļ� */
#include "..\..\include\sip\sip_const.h"
#include "..\..\include\sip\sip_type.h"

#include "..\include\syntax\sip_syntax_const.h"
#include "..\include\syntax\sip_syntax_type.h"

/* ��ģ���ڲ�ʹ�õĳ����ͽṹͷ�ļ� */
#include "sip_syntax_const.inc"
#include "sip_syntax_type.inc"

/* ��ģ���ڲ���������ͷ�ļ� */
#include "sip_syntax_code.inc"

/* ��ģ��ȫ�ֱ�������ͷ�ļ� */
#include "sip_syntax_var.inc"

/*******************************************************************************
SIP-message    =  Request / Response
*******************************************************************************/
ULONG SIP_CodeSIPmessage(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer)
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
                      SIP_SYNTAX_BUFFER_S *pstBuffer)
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

    SIP_ADD_STRING(pstBuffer, "\r\n");

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
                       SIP_SYNTAX_BUFFER_S *pstBuffer)
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

    SIP_ADD_STRING(pstBuffer, "\r\n");

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
                          SIP_SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    SIP_REQUEST_LINE_S *pstRequestLine = NULL_PTR;

    pstRequestLine = (SIP_REQUEST_LINE_S *)pstStruct;
    ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_METHOD)(&pstRequestLine->eMethod, pstBuffer);
    if (ulRet!= SUCCESS)
    {
        return ulRet;
    }

    SIP_ADD_STRING(pstBuffer, " ");

    ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_REQUEST_URI)(pstRequestLine->pstRequestURI, pstBuffer);
    if (ulRet!= SUCCESS)
    {
        return ulRet;
    }

    SIP_ADD_STRING(pstBuffer, " ");

    ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_SIP_VERSION)(&pstRequestLine->ucVersion, pstBuffer);
    if (ulRet!= SUCCESS)
    {
        return ulRet;
    }

    SIP_ADD_STRING(pstBuffer, "\r\n");
    return SUCCESS;
}

/*******************************************************************************
Status-Line     =  SIP-Version SP Status-Code SP Reason-Phrase CRLF
*******************************************************************************/
ULONG SIP_CodeStatusLine(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    SIP_STATUS_LINE_S *pstStateLine = NULL_PTR;

    pstStateLine = (SIP_STATUS_LINE_S *)pstStruct;
    ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_SIP_VERSION)(&pstStateLine->ucVersion, pstBuffer);
    if (ulRet!= SUCCESS)
    {
        return ulRet;
    }

    SIP_ADD_STRING(pstBuffer, " ");

    ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_STATUS_CODE)(&pstStateLine->eStatusCode, pstBuffer);
    if (ulRet!= SUCCESS)
    {
        return ulRet;
    }

    SIP_ADD_STRING(pstBuffer, " ");

    ulRet = SIP_CodeReasonPhrase(&pstStateLine->eStatusCode, pstBuffer);
    if (ulRet!= SUCCESS)
    {
        return ulRet;
    }

    SIP_ADD_STRING(pstBuffer, "\r\n");
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
                            SIP_SYNTAX_BUFFER_S *pstBuffer)
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
        if (SIP_GET_CODE_FUNC(ulAppIndex)== NULL_PTR)
        {
            /* û�б��뺯�������� */
            continue;
        }

        /* ���ܴ��ڶ����ͬͷ�򣬰������� */
        pstHeader = pstSipMsg->apstHeaders[ulLoop];
        while (pstHeader!= NULL_PTR)
        {
            ulRet = SIP_GET_CODE_FUNC(ulAppIndex)(pstHeader,
                                                  pstBuffer);
            if (ulRet == SUCCESS)
            {
                SIP_ADD_STRING(pstBuffer, "\r\n");
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
                          SIP_SYNTAX_BUFFER_S *pstBuffer)
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
                     SIP_SYNTAX_BUFFER_S *pstBuffer)
{
    SIP_METHOD_E eMethod;

    eMethod = *((SIP_METHOD_E *)pstStruct);
    switch(eMethod)
    {
        case SIP_METHOD_INVITE:
            SIP_ADD_STRING(pstBuffer, "INVITE");
            break;

        case SIP_METHOD_ACK:
            SIP_ADD_STRING(pstBuffer, "ACK");
            break;

        case SIP_METHOD_OPTIONS:
            SIP_ADD_STRING(pstBuffer,"OPTIONS");
            break;

        case SIP_METHOD_BYE:
            SIP_ADD_STRING(pstBuffer, "BYE");
            break;

        case SIP_METHOD_CANCEL:
            SIP_ADD_STRING(pstBuffer, "CANCEL");
            break;

        case SIP_METHOD_REGISTER:
            SIP_ADD_STRING(pstBuffer, "REGISTER");
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
                         SIP_SYNTAX_BUFFER_S *pstBuffer)
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
                         SIP_SYNTAX_BUFFER_S *pstBuffer)
{
    UCHAR ucVersion;

    ucVersion = *((UCHAR *)pstStruct);
    SIP_ADD_STRING(pstBuffer, "SIP/");
    SIP_ADD_DEC_NUM(pstBuffer, ucVersion);
    /*С������ֱ����0*/
    SIP_ADD_STRING(pstBuffer, ".0");
    return SUCCESS;
}

/*******************************************************************************
SIP-URI          =  "sip:" [ userinfo ] hostport
                    uri-parameters [ headers ]
*******************************************************************************/
ULONG SIP_CodeSipURI(void *pstStruct,
                     SIP_SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    URI_USER_INFO_S *pstUserInfo = NULL_PTR;
    SIP_URI_S *pstSipURI = NULL_PTR;

    pstSipURI = (SIP_URI_S *)pstStruct;

    SIP_ADD_STRING(pstBuffer, "sip:");

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

    /*�Ժ�������*/

    return SUCCESS;
}

/*******************************************************************************
SIPS-URI         =  "sips:" [ userinfo ] hostport
                    uri-parameters [ headers ]
*******************************************************************************/
ULONG SIP_CodeSipsURI(void *pstStruct,
                      SIP_SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    URI_USER_INFO_S *pstUserInfo = NULL_PTR;
    SIP_URI_S *pstSipsURI = NULL_PTR;

    pstSipsURI = (SIP_URI_S *)pstStruct;
    SIP_ADD_STRING(pstBuffer, "sips:");

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

    /*�Ժ�������*/

    return SUCCESS;
}

/*******************************************************************************
userinfo         =  ( user / telephone-subscriber ) [ ":" password ] "@"
*******************************************************************************/
ULONG SIP_CodeUserinfo(void *pstStruct,
                       SIP_SYNTAX_BUFFER_S *pstBuffer)
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
        SIP_ADD_STRING(pstBuffer, ":");
        pucString = pstUserInfo->pucPassword;
        SIP_ADD_STRING(pstBuffer, pucString);
    }

    SIP_ADD_STRING(pstBuffer, "@");

    return SUCCESS;
}

/*******************************************************************************
userinfo         =  ( user / telephone-subscriber ) [ ":" password ] "@"
*******************************************************************************/
ULONG SIP_CodeUser(void *pstStruct,
                   SIP_SYNTAX_BUFFER_S *pstBuffer)
{
    UCHAR *pucUserInfo = NULL_PTR;

    pucUserInfo = (UCHAR *)pstStruct;
    SIP_ADD_STRING(pstBuffer, pucUserInfo);
    return SUCCESS;
}

/*******************************************************************************
hostport         =  host [ ":" port ]
*******************************************************************************/
ULONG SIP_CodeHostport(void *pstStruct,
                       SIP_SYNTAX_BUFFER_S *pstBuffer)
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
        SIP_ADD_STRING(pstBuffer, ":");
        ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_PORT)(&pstHostPort->usPort, pstBuffer);
    }

    return SUCCESS;
}

/*******************************************************************************
host             =  hostname / IPv4address / IPv6reference
*******************************************************************************/
ULONG SIP_CodeHost(void *pstStruct,
                   SIP_SYNTAX_BUFFER_S *pstBuffer)
{
    UCHAR      *pucString = NULL_PTR;
    URI_HOST_S *pstHost = NULL_PTR;

    pstHost = (URI_HOST_S*)pstStruct;
    pucString = pstHost->pucAddrStr;
    SIP_ADD_STRING(pstBuffer, pucString);

    return SUCCESS;
}

/*******************************************************************************
port           =  1*DIGIT
*******************************************************************************/
ULONG SIP_CodePort(void *pstStruct,
                   SIP_SYNTAX_BUFFER_S *pstBuffer)
{
    USHORT usPort;

    usPort = *((USHORT *)pstStruct);
    SIP_ADD_DEC_NUM(pstBuffer, usPort);
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
                         SIP_SYNTAX_BUFFER_S *pstBuffer)
{
    SIP_STATUS_CODE_E eStatusCode;

    eStatusCode = *((SIP_STATUS_CODE_E *)pstStruct);
    SIP_ADD_DEC_NUM(pstBuffer, eStatusCode);

    return SUCCESS;
}

/*******************************************************************************
Reason-Phrase   =  *(reserved / unreserved / escaped
                   / UTF8-NONASCII / UTF8-CONT / SP / HTAB)
*******************************************************************************/
ULONG SIP_CodeReasonPhrase(void *pstStruct,
                           SIP_SYNTAX_BUFFER_S *pstBuffer)
{
    SIP_STATUS_CODE_E eStatusCode;

    eStatusCode = *((SIP_STATUS_CODE_E *)pstStruct);
    switch (eStatusCode)
    {
        case SIP_STATUS_CODE_100:
            SIP_ADD_STRING(pstBuffer, "Trying");
            break;

        case SIP_STATUS_CODE_180:
            SIP_ADD_STRING(pstBuffer, "Ringing");
            break;

        case SIP_STATUS_CODE_181:
            SIP_ADD_STRING(pstBuffer, "Call Is Being Forwarded");
            break;

        case SIP_STATUS_CODE_182:
            SIP_ADD_STRING(pstBuffer, "Queued");
            break;

        case SIP_STATUS_CODE_183:
            SIP_ADD_STRING(pstBuffer, "Session Progress");
            break;

        case SIP_STATUS_CODE_200:
            SIP_ADD_STRING(pstBuffer, "OK");
            break;

        case SIP_STATUS_CODE_300:
            SIP_ADD_STRING(pstBuffer, "Multiple Choices");
            break;

        case SIP_STATUS_CODE_301:
            SIP_ADD_STRING(pstBuffer, "Moved Permanently");
            break;

        case SIP_STATUS_CODE_302:
            SIP_ADD_STRING(pstBuffer, "Moved Temporarily");
            break;

        case SIP_STATUS_CODE_305:
            SIP_ADD_STRING(pstBuffer, "Use Proxy");
            break;

        case SIP_STATUS_CODE_380:
            SIP_ADD_STRING(pstBuffer, "Alternative Service");
            break;

        case SIP_STATUS_CODE_400:
            SIP_ADD_STRING(pstBuffer, "Bad Request");
            break;

        case SIP_STATUS_CODE_401:
            SIP_ADD_STRING(pstBuffer, "Unauthorized");
            break;

        case SIP_STATUS_CODE_402:
            SIP_ADD_STRING(pstBuffer, "Payment Required");
            break;

        case SIP_STATUS_CODE_403:
            SIP_ADD_STRING(pstBuffer, "Forbidden");
            break;

        case SIP_STATUS_CODE_404:
            SIP_ADD_STRING(pstBuffer, "Not Found");
            break;

        case SIP_STATUS_CODE_405:
            SIP_ADD_STRING(pstBuffer, "Method Not Allowed");
            break;

        case SIP_STATUS_CODE_406:
            SIP_ADD_STRING(pstBuffer, "Not Acceptable");
            break;

        case SIP_STATUS_CODE_407:
            SIP_ADD_STRING(pstBuffer, "Proxy Authentication Required");
            break;

        case SIP_STATUS_CODE_408:
            SIP_ADD_STRING(pstBuffer, "Request Timeout");
            break;

        case SIP_STATUS_CODE_410:
            SIP_ADD_STRING(pstBuffer, "Gone");
            break;

        case SIP_STATUS_CODE_413:
            SIP_ADD_STRING(pstBuffer, "Request Entity Too Large");
            break;

        case SIP_STATUS_CODE_414:
            SIP_ADD_STRING(pstBuffer, "Request-URI Too Long");
            break;

        case SIP_STATUS_CODE_415:
            SIP_ADD_STRING(pstBuffer, "Unsupported Media Type");
            break;

        case SIP_STATUS_CODE_416:
            SIP_ADD_STRING(pstBuffer, "Unsupported URI Scheme");
            break;

        case SIP_STATUS_CODE_420:
            SIP_ADD_STRING(pstBuffer, "Bad Extensione");
            break;

        case SIP_STATUS_CODE_421:
            SIP_ADD_STRING(pstBuffer, "Extension Required");
            break;

        case SIP_STATUS_CODE_423:
            SIP_ADD_STRING(pstBuffer, "Interval Too Brief");
            break;

        case SIP_STATUS_CODE_480:
            SIP_ADD_STRING(pstBuffer, "Temporarily Unavailable");
            break;

        case SIP_STATUS_CODE_481:
            SIP_ADD_STRING(pstBuffer, "Call/Transaction Does Not Exist");
            break;

        case SIP_STATUS_CODE_482:
            SIP_ADD_STRING(pstBuffer, "Loop Detected");
            break;

        case SIP_STATUS_CODE_483:
            SIP_ADD_STRING(pstBuffer, "Too Many Hops");
            break;

        case SIP_STATUS_CODE_484:
            SIP_ADD_STRING(pstBuffer, "Address Incomplete");
            break;

        case SIP_STATUS_CODE_485:
            SIP_ADD_STRING(pstBuffer, "Ambiguous");
            break;

        case SIP_STATUS_CODE_486:
            SIP_ADD_STRING(pstBuffer,"Busy Here");
            break;

        case SIP_STATUS_CODE_487:
            SIP_ADD_STRING(pstBuffer, "Request Terminated");
            break;

        case SIP_STATUS_CODE_488:
            SIP_ADD_STRING(pstBuffer, "Not Acceptable Here");
            break;

        case SIP_STATUS_CODE_491:
            SIP_ADD_STRING(pstBuffer, "Request Pending");
            break;

        case SIP_STATUS_CODE_493:
            SIP_ADD_STRING(pstBuffer, "Undecipherable");
            break;

        case SIP_STATUS_CODE_500:
            SIP_ADD_STRING(pstBuffer, "Server Internal Error");
            break;

        case SIP_STATUS_CODE_501:
            SIP_ADD_STRING(pstBuffer, "Not Implemented");
            break;

        case SIP_STATUS_CODE_502:
            SIP_ADD_STRING(pstBuffer, "Bad Gateway");
            break;

        case SIP_STATUS_CODE_503:
            SIP_ADD_STRING(pstBuffer, "Service Unavailable");
            break;

        case SIP_STATUS_CODE_504:
            SIP_ADD_STRING(pstBuffer, "Server Time-out");
            break;

        case SIP_STATUS_CODE_505:
            SIP_ADD_STRING(pstBuffer, "Version Not Supported");
            break;

        case SIP_STATUS_CODE_513:
            SIP_ADD_STRING(pstBuffer, "Message Too Large");
            break;

        case SIP_STATUS_CODE_600:
            SIP_ADD_STRING(pstBuffer, "Busy Everywhere");
            break;

        case SIP_STATUS_CODE_603:
            SIP_ADD_STRING(pstBuffer, "Decline");
            break;

        case SIP_STATUS_CODE_604:
            SIP_ADD_STRING(pstBuffer, "Does Not Exist Anywhere");
            break;

        case SIP_STATUS_CODE_606:
            SIP_ADD_STRING(pstBuffer, "Not Acceptable");
            break;

        default:
            SIP_ADD_STRING(pstBuffer, "Unknown Status Code");
            break;
    }

    return SUCCESS;
}

/*******************************************************************************
from-spec   =  ( name-addr / addr-spec )
               *( SEMI from-param )
*******************************************************************************/
ULONG SIP_CodeFromSpec(void *pstStruct,
                       SIP_SYNTAX_BUFFER_S *pstBuffer)
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
                        SIP_SYNTAX_BUFFER_S *pstBuffer)
{
    UCHAR *pucString = NULL_PTR;
    SIP_HEADER_FROM_S *pstFrom = NULL_PTR;

    pstFrom = (SIP_HEADER_FROM_S *)pstStruct;
    if (pstFrom->pucTag != NULL_PTR)
    {
        SIP_ADD_STRING(pstBuffer, ";");
        SIP_ADD_STRING(pstBuffer, "tag=");
        pucString = pstFrom->pucTag;
        SIP_ADD_STRING(pstBuffer, pucString);
    }

    return SUCCESS;
}

/*******************************************************************************
to-param  =  tag-param / generic-param
*******************************************************************************/
ULONG SIP_CodeToParam(void *pstStruct,
                      SIP_SYNTAX_BUFFER_S *pstBuffer)
{
    UCHAR *pucString = NULL_PTR;
    SIP_HEADER_TO_S *pstTo = NULL_PTR;

    pstTo = (SIP_HEADER_TO_S *)pstStruct;
    if (pstTo->pucTag != NULL_PTR)
    {
        SIP_ADD_STRING(pstBuffer, ";");
        SIP_ADD_STRING(pstBuffer, "tag=");
        pucString = pstTo->pucTag;
        SIP_ADD_STRING(pstBuffer, pucString);
    }

    return SUCCESS;
}

/*******************************************************************************
via-parm          =  sent-protocol LWS sent-by *( SEMI via-params )
*******************************************************************************/
ULONG SIP_CodeViaParm(void *pstStruct,
                      SIP_SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    SIP_VIA_PARM_S *pstViaParm = NULL_PTR;

    pstViaParm = (SIP_VIA_PARM_S *)pstStruct;
    ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_SENT_PROTOCOL)(&pstViaParm->eProtocolType, pstBuffer);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }

    SIP_ADD_STRING(pstBuffer, " ");

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
                           SIP_SYNTAX_BUFFER_S *pstBuffer)
{
    SIP_TRANSPORT_PROTOCOL_E  eProtocolType;

    eProtocolType = *((SIP_TRANSPORT_PROTOCOL_E *)pstStruct);

    SIP_ADD_STRING(pstBuffer, "SIP/2.0/");

    return SIP_GET_CODE_FUNC(SIP_ABNF_RULE_TRANSPORT)(&eProtocolType, pstBuffer);
}

/*******************************************************************************
transport         =  "UDP" / "TCP" / "TLS" / "SCTP"
                     / other-transport
*******************************************************************************/
ULONG SIP_CodeTransport(void *pstStruct,
                        SIP_SYNTAX_BUFFER_S *pstBuffer)
{
    SIP_TRANSPORT_PROTOCOL_E  eProtocolType;

    eProtocolType = *((SIP_TRANSPORT_PROTOCOL_E *)pstStruct);
    switch (eProtocolType)
    {
        case SIP_TRANSPORT_PROTOCOL_TCP:
            SIP_ADD_STRING(pstBuffer, "TCP");
            break;

        case SIP_TRANSPORT_PROTOCOL_TLS:
            SIP_ADD_STRING(pstBuffer, "TLS");
            break;

        case SIP_TRANSPORT_PROTOCOL_UDP:
            SIP_ADD_STRING(pstBuffer, "UDP");
            break;

        case SIP_TRANSPORT_PROTOCOL_SCTP:
            SIP_ADD_STRING(pstBuffer, "SCTP");
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
                     SIP_SYNTAX_BUFFER_S *pstBuffer)
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
        SIP_ADD_STRING(pstBuffer, ":");
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
                        SIP_SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    UCHAR      *pucString = NULL_PTR;
    URI_HOST_S *pstMaddr = NULL_PTR;
    SIP_VIA_RECEIVED_S *pstReceived = NULL_PTR;
    SIP_VIA_PARM_S *pstViaParm = NULL_PTR;

    pstViaParm = (SIP_VIA_PARM_S *)pstStruct;
    if (pstViaParm->ulTtl != NULL_ULONG)
    {
        SIP_ADD_STRING(pstBuffer, ";");
        ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_VIA_TTL)(&pstViaParm->ulTtl, pstBuffer);
        if (ulRet!= SUCCESS)
        {
            return FAIL;
        }
    }

    if (pstViaParm->pucBranch != NULL_PTR)
    {
        SIP_ADD_STRING(pstBuffer, ";");
        pucString = pstViaParm->pucBranch;
        ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_VIA_BRANCH)(pucString, pstBuffer);
        if (ulRet!= SUCCESS)
        {
            return FAIL;
        }
    }

    if (pstViaParm->pstMaddr != NULL_PTR)
    {
        SIP_ADD_STRING(pstBuffer, ";");
        pstMaddr = pstViaParm->pstMaddr;
        ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_VIA_MADDR)(pstMaddr, pstBuffer);
        if (ulRet!= SUCCESS)
        {
            return FAIL;
        }
    }

    if (pstViaParm->pstReceived != NULL_PTR)
    {
        SIP_ADD_STRING(pstBuffer, ";");
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
                     SIP_SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulTtl;

    ulTtl = *((ULONG *)pstStruct);
    SIP_ADD_STRING(pstBuffer, "ttl=");
    SIP_ADD_DEC_NUM(pstBuffer, ulTtl);
    return SUCCESS;
}

/*******************************************************************************
via-maddr         =  "maddr" EQUAL host
*******************************************************************************/
ULONG SIP_CodeViaMaddr(void *pstStruct,
                       SIP_SYNTAX_BUFFER_S *pstBuffer)
{
    URI_HOST_S *pstMaddr = NULL_PTR;

    pstMaddr= (URI_HOST_S *)pstStruct;
    SIP_ADD_STRING(pstBuffer,"maddr=");
    return SIP_GET_CODE_FUNC(SIP_ABNF_RULE_HOST)(pstMaddr, pstBuffer);
}

/*******************************************************************************
via-received      =  "received" EQUAL (IPv4address / IPv6address)
*******************************************************************************/
ULONG SIP_CodeViaReceived(void *pstStruct,
                          SIP_SYNTAX_BUFFER_S *pstBuffer)
{
    UCHAR *pucString = NULL_PTR;
    SIP_VIA_RECEIVED_S *pstReceived = NULL_PTR;

    pstReceived = (SIP_VIA_RECEIVED_S *)pstStruct;
    SIP_ADD_STRING(pstBuffer, "received=");
    if (pstReceived->eIpType == SIP_IP_TYPE_IPV4)
    {
        pucString = pstReceived->u.pucIPV4;
        SIP_ADD_STRING(pstBuffer, pucString);
    }
    else
    {
        pucString = pstReceived->u.pucIPV6;
        SIP_ADD_STRING(pstBuffer, pucString);
    }

    return SUCCESS;
}

/*******************************************************************************
via-branch        =  "branch" EQUAL token
*******************************************************************************/
ULONG SIP_CodeViaBranch(void *pstStruct,
                        SIP_SYNTAX_BUFFER_S *pstBuffer)
{
    UCHAR *pucBranch = NULL_PTR;
    pucBranch = (UCHAR *)pstStruct;

    SIP_ADD_STRING(pstBuffer, "branch=");
    SIP_ADD_STRING(pstBuffer, pucBranch);
    return SUCCESS;
}

/*******************************************************************************
name-addr      =  [ display-name ] LAQUOT addr-spec RAQUOT
*******************************************************************************/
ULONG SIP_CodeNameAddr(void *pstStruct,
                       SIP_SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    UCHAR *pucString = NULL_PTR;
    SIP_NAME_ADDR_S *pstNameAddr = NULL_PTR;
    URI_S           *pstUri = NULL_PTR;

    pstNameAddr = (SIP_NAME_ADDR_S *)pstStruct;
    if (pstNameAddr->pucName != NULL_PTR)
    {
        pucString = pstNameAddr->pucName;
        SIP_ADD_STRING(pstBuffer, pucString);
    }

    SIP_ADD_STRING(pstBuffer, "<");

    pstUri = pstNameAddr->pstUri;
    ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_ADDR_SPEC)(pstUri, pstBuffer);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }

    SIP_ADD_STRING(pstBuffer, ">");

    return SUCCESS;
}

/*******************************************************************************
addr-spec      =  SIP-URI / SIPS-URI / absoluteURI
*******************************************************************************/
ULONG SIP_CodeAddrSpec(void *pstStruct,
                       SIP_SYNTAX_BUFFER_S *pstBuffer)
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

/* ���е�ͷ�� */
/*******************************************************************************
Accept         =  "Accept" HCOLON
                   [ accept-range *(COMMA accept-range) ]
*******************************************************************************/
ULONG SIP_CodeHeaderAccept(void *pstStruct,
                           SIP_SYNTAX_BUFFER_S *pstBuffer)
{
    SIP_HEADER_ACCEPT_S *pstAccept = NULL_PTR;

    pstAccept = (SIP_HEADER_ACCEPT_S *)pstStruct;

    SIP_ADD_STRING(pstBuffer, "Accept: ");

    return SUCCESS;
}

/*******************************************************************************
Accept-Encoding  =  "Accept-Encoding" HCOLON
                     [ encoding *(COMMA encoding) ]
*******************************************************************************/
ULONG SIP_CodeHeaderAcceptEncoding(void *pstStruct,
                                   SIP_SYNTAX_BUFFER_S *pstBuffer)
{
    SIP_HEADER_ACCEPT_ENCODING_S *pstAcceptEncoding = NULL_PTR;

    pstAcceptEncoding = (SIP_HEADER_ACCEPT_ENCODING_S *)pstStruct;
    SIP_ADD_STRING(pstBuffer, "Accept-Encoding: ");

    return SUCCESS;
}

/*******************************************************************************
Call-ID  =  ( "Call-ID" / "i" ) HCOLON callid
*******************************************************************************/
ULONG SIP_CodeHeaderCallID(void *pstStruct,
                           SIP_SYNTAX_BUFFER_S *pstBuffer)
{
    SIP_HEADER_CALL_ID_S *pstCallID= NULL_PTR;
    UCHAR *pucString = NULL_PTR;

    pstCallID = (SIP_HEADER_CALL_ID_S *)pstStruct;

    SIP_ADD_STRING(pstBuffer, "Call-ID: ");
    pucString = pstCallID->pucCallID;
    SIP_ADD_STRING(pstBuffer, pucString);
    return SUCCESS;
}

/*******************************************************************************
CSeq  =  "CSeq" HCOLON 1*DIGIT LWS Method
*******************************************************************************/
ULONG SIP_CodeHeaderCseq(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    SIP_HEADER_CSEQ_S *pstCseq = NULL_PTR;

    pstCseq = (SIP_HEADER_CSEQ_S *)pstStruct;

    SIP_ADD_STRING(pstBuffer, "CSeq: ");
    SIP_ADD_DEC_NUM(pstBuffer, pstCseq->ulSeq);
    SIP_ADD_STRING(pstBuffer, " ");

    ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_METHOD)(&pstCseq->eMethod, pstBuffer);

    return ulRet;
}

/*******************************************************************************
From        =  ( "From" / "f" ) HCOLON from-spec
*******************************************************************************/
ULONG SIP_CodeHeaderFrom(void *pstStruct,
                         SIP_SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    SIP_HEADER_FROM_S *pstFrom = NULL_PTR;

    pstFrom = (SIP_HEADER_FROM_S *)pstStruct;
    SIP_ADD_STRING(pstBuffer, "From: ");

    ulRet = SIP_GET_CODE_FUNC(SIP_ABNF_RULE_FROM_SPEC)(pstFrom, pstBuffer);
    return ulRet;
}

/*******************************************************************************
To        =  ( "To" / "t" ) HCOLON ( name-addr
             / addr-spec ) *( SEMI to-param )
*******************************************************************************/
ULONG SIP_CodeHeaderTo(void *pstStruct,
                       SIP_SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    SIP_HEADER_TO_S *pstTo = NULL_PTR;
    URI_S *pstUri = NULL_PTR;

    pstTo = (SIP_HEADER_TO_S *)pstStruct;

    SIP_ADD_STRING(pstBuffer, "To: ");

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
                        SIP_SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    SIP_VIA_PARM_S   *pstViaParm = NULL_PTR;
    SIP_HEADER_VIA_S *pstVia = NULL_PTR;
    BOOL  bFirst = TRUE;

    pstVia = (SIP_HEADER_VIA_S *)pstStruct;
    SIP_ADD_STRING(pstBuffer, "Via: ");

    pstViaParm = pstVia->pstViaParm;
    while(pstViaParm != NULL_PTR)
    {
        if (bFirst != TRUE)
        {
            SIP_ADD_STRING(pstBuffer, ",");
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
                                SIP_SYNTAX_BUFFER_S *pstBuffer)
{
    SIP_HEADER_MAX_FORWARDS_S *pstMaxForwards = NULL_PTR;

    pstMaxForwards = (SIP_HEADER_MAX_FORWARDS_S *)pstStruct;

    SIP_ADD_STRING(pstBuffer, "Max-Forwards: ");
    SIP_ADD_DEC_NUM(pstBuffer, pstMaxForwards->ulMaxForwards);

    return SUCCESS;
}