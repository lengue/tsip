/*******************************************************************************
功能    : SIP消息解析函数
创建人  : 唐春平
创建日期: 200.01.10
修改记录:

解析原则:
1、 所有的解析函数形参一致
    ABNF_GRAMMAR_NODE_S *pstGrammarNode: 结构的ABNF节点表示
    UCHAR               *pucString     : 被解析的文本
    UBUF_HEADER_S       *pstUbuf       : 解析输出的UBUF结构
    void               **ppStruct     : 待解析结构指针
2、 如果结构只包含多个可选的子结构，则是否分配空间在解析子结构时再决定
3、 非原则2的情况根据ppStruct参数来判断是否需要分配空间，有效则认为调用者已经申
    请了空间，无须再申请，否则该函数申请
4、 对于结构的子结构解析，为了统一解析函数的形参，需要构造子结构UBUF指针传递给被
    调用函数
*******************************************************************************/
/* 外部依赖模块头文件 */
#include "common\common.h"
#include "abnf\abnf.h"
#include "ubuf\ubuf.h"
#include "sip\uri.h"

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
ULONG SIP_ParseSIPmessage(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                          UCHAR               *pucString,
                          UBUF_HEADER_S       *pstUbuf,
                          void               **ppStruct)
{
    ULONG ulRet;
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;

    /* 原则2: 如果结构只包含多个可选的子结构，则是否分配空间在解析子结构时再决定，
    下同 */

    pstNode = pstGrammarNode->pstChild;
    if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_REQUEST))
    {
        ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_REQUEST)(pstNode, pucString, pstUbuf, ppStruct);
    }
    else
    {
        ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_RESPONSE)(pstNode, pucString, pstUbuf, ppStruct);
    }

    return ulRet;
}

/*******************************************************************************
Request        =  Request-Line
                  *( message-header )
                  CRLF
                  [ message-body ]
*******************************************************************************/
ULONG SIP_ParseRequest(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                       UCHAR               *pucString,
                       UBUF_HEADER_S       *pstUbuf,
                       void               **ppStruct)
{
    ULONG ulRet;
    ABNF_GRAMMAR_NODE_S *pstNode   = NULL_PTR;
    SIP_MSG_S           *pstSipMsg = NULL_PTR;
    void                *pStruct   = NULL_PTR;

    /* 原则3: 如果不是确定分配或者不分配空间，则根据UBUF指针来判断，有效则认为调
    用者已经申请了空间，无须再申请，否则此处申请，下同 */
    SIP_GET_COMPONET_PTR(pstSipMsg, SIP_MSG_S, pstUbuf, ppStruct);

    pstSipMsg->eMsgType = SIP_MSG_TYPE_REQUEST;

    pstNode = pstGrammarNode->pstChild;
    while(pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_REQUEST_LINE))
        {
            pStruct = &pstSipMsg->uStartLine.stRequstLine;
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_REQUEST_LINE)(pstNode,
                                                                   pucString,
                                                                   pstUbuf,
                                                                  &pStruct);
            if (ulRet != SUCCESS)
            {
                /* 起始行不能错 */
                return FAIL;
            }
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_MESSAGE_HEADER))
        {
            SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_MESSAGE_HEADER)(pstNode,
                                                             pucString,
                                                             pstUbuf,
                                                             ppStruct);
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_MESSAGE_BODY))
        {
            SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_MESSAGE_BODY)(pstNode,
                                                           pucString,
                                                           pstUbuf,
                                                           ppStruct);
        }

        pstNode = pstNode->pstNextNode;
    }

    return SUCCESS;
}

/*******************************************************************************
Response          =  Status-Line
                     *( message-header )
                     CRLF
                     [ message-body ]
*******************************************************************************/
ULONG SIP_ParseResponse(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                        UCHAR               *pucString,
                        UBUF_HEADER_S       *pstUbuf,
                        void               **ppStruct)
{
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    ULONG       ulRet;
    SIP_MSG_S  *pstSipMsg = NULL_PTR;
    void       *pStruct   = NULL_PTR;

    SIP_GET_COMPONET_PTR(pstSipMsg, SIP_MSG_S, pstUbuf, ppStruct);

    pstSipMsg->eMsgType = SIP_MSG_TYPE_RESPONSE;

    pstNode = pstGrammarNode->pstChild;
    while(pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_STATUS_LINE))
        {
            pStruct = &pstSipMsg->uStartLine.stStatusLine;
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_STATUS_LINE)(pstNode,
                                                                  pucString,
                                                                  pstUbuf,
                                                                 &pStruct);
            if (ulRet != SUCCESS)
            {
                /* 起始行不能错 */
                return FAIL;
            }
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_MESSAGE_HEADER))
        {
            SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_MESSAGE_HEADER)(pstNode,
                                                            pucString,
                                                            pstUbuf,
                                                            ppStruct);
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_MESSAGE_BODY))
        {
            SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_MESSAGE_BODY)(pstNode,
                                                           pucString,
                                                           pstUbuf,
                                                           ppStruct);
        }

        pstNode = pstNode->pstNextNode;
    }

    return SUCCESS;
}

/*******************************************************************************
Request-Line   =  Method SP Request-URI SP SIP-Version CRLF
*******************************************************************************/
ULONG SIP_ParseRequestLine(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                           UCHAR               *pucString,
                           UBUF_HEADER_S       *pstUbuf,
                           void               **ppStruct)
{
    ULONG ulRet;
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    SIP_REQUEST_LINE_S  *pstRequestLine = NULL_PTR;
    void                *pStruct   = NULL_PTR;

    SIP_GET_COMPONET_PTR(pstRequestLine, SIP_REQUEST_LINE_S, pstUbuf, ppStruct);

    pstNode = pstGrammarNode->pstChild;
    while (pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_METHOD))
        {
            pStruct = &pstRequestLine->eMethod;
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_METHOD)(pstNode,
                                                             pucString,
                                                             pstUbuf,
                                                            &pStruct);
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_REQUEST_URI))
        {
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_REQUEST_URI)(pstNode,
                                                                  pucString,
                                                                  pstUbuf,
                                                                 &pstRequestLine->pstRequestURI);
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_SIP_VERSION))
        {
            pStruct = &pstRequestLine->ucVersion;
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_SIP_VERSION)(pstNode,
                                                                  pucString,
                                                                  pstUbuf,
                                                                 &pStruct);
        }

        if(ulRet != SUCCESS)
        {
            return FAIL;
        }

        pstNode = pstNode->pstNextNode;
    }

    return SUCCESS;
}

/*******************************************************************************
Status-Line     =  SIP-Version SP Status-Code SP Reason-Phrase CRLF
*******************************************************************************/
ULONG SIP_ParseStatusLine(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                          UCHAR               *pucString,
                          UBUF_HEADER_S       *pstUbuf,
                          void               **ppStruct)
{
    ULONG ulRet = SUCCESS;
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    SIP_STATUS_LINE_S   *pstStatusLine = NULL_PTR;
    void                *pStruct   = NULL_PTR;

    SIP_GET_COMPONET_PTR(pstStatusLine, SIP_STATUS_LINE_S, pstUbuf, ppStruct);

    pstNode = pstGrammarNode->pstChild;
    while (pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_SIP_VERSION))
        {
            pStruct = &pstStatusLine->ucVersion;
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_SIP_VERSION)(pstNode,
                                                                pucString,
                                                                pstUbuf,
                                                               &pStruct);
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_STATUS_CODE))
        {
            pStruct = &pstStatusLine->eStatusCode;
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_STATUS_CODE)(pstNode,
                                                                  pucString,
                                                                  pstUbuf,
                                                                 &pStruct);
        }

        if(ulRet != SUCCESS)
        {
            return FAIL;
        }

        pstNode = pstNode->pstNextNode;
    }

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
ULONG SIP_ParseMessageHeader(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                             UCHAR               *pucString,
                             UBUF_HEADER_S       *pstUbuf,
                             void               **ppStruct)
{
    ULONG ulHeader;
    ULONG ulAppIndex;
    ABNF_GRAMMAR_NODE_S *pstNode   = NULL_PTR;
    void                *pStruct   = NULL_PTR;
    SIP_HEADER_S       **ppstHeader = NULL_PTR;
    SIP_MSG_S           *pstSipMsg = NULL_PTR;

    SIP_GET_COMPONET_PTR(pstSipMsg, SIP_MSG_S, pstUbuf, ppStruct);

    pstNode = pstGrammarNode->pstChild;
    for(ulHeader = 0; ulHeader < SIP_HEADER_BUTT; ulHeader++)
    {
        ulAppIndex = SIP_GET_HEADER_RULE_INDEX(ulHeader);
        if (ulAppIndex >= SIP_ABNF_RULE_BUTT)
        {
            continue;
        }

        if (!SIP_RULE_MATCH(pstNode, ulAppIndex))
        {
            continue;
        }

        if (SIP_GET_PARSE_FUNC(ulAppIndex)== NULL_PTR)
        {
            /* 没有解码函数不处理 */
            return SUCCESS;
        }

        /* 找到最后一个有效的头域 */
        ppstHeader = &pstSipMsg->apstHeaders[ulHeader];
        while(*ppstHeader != NULL_PTR)
        {
            ppstHeader = &(*ppstHeader)->pstNext;
        }

        *ppstHeader = UBUF_AddComponent(pstUbuf, SIP_GET_HEADER_SPEC_SIZE(ulHeader));
        memset(*ppstHeader, 0, SIP_GET_HEADER_SPEC_SIZE(ulHeader));
        return SIP_GET_PARSE_FUNC(ulAppIndex)(pstNode,
                                              pucString,
                                              pstUbuf,
                                              ppstHeader);
    }

    return SUCCESS;
}

/*******************************************************************************
message-body  =  *OCTET
*******************************************************************************/
ULONG SIP_ParseMessageBody(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                           UCHAR               *pucString,
                           UBUF_HEADER_S       *pstUbuf,
                           void               **ppStruct)
{
    return SUCCESS;
}

/*******************************************************************************
Method            =  INVITEm / ACKm / OPTIONSm / BYEm
                     / CANCELm / REGISTERm
                     / extension-method
*******************************************************************************/
ULONG SIP_ParseMethod(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                      UCHAR               *pucString,
                      UBUF_HEADER_S       *pstUbuf,
                      void               **ppStruct)
{
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    SIP_METHOD_E        *peMthod  = NULL_PTR;

    SIP_GET_COMPONET_PTR(peMthod, SIP_METHOD_E, pstUbuf, ppStruct);

    pstNode = pstGrammarNode->pstChild;
    if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_INVITE_M))
    {
        *peMthod = SIP_METHOD_INVITE;
    }
    else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_ACK_M))
    {
        *peMthod = SIP_METHOD_ACK;
    }
    else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_OPTIONS_M))
    {
        *peMthod = SIP_METHOD_OPTIONS;
    }
    else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_BYE_M))
    {
        *peMthod = SIP_METHOD_BYE;
    }
    else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_CANCEL_M))
    {
        *peMthod = SIP_METHOD_CANCEL;
    }
    else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_REGISTER_M))
    {
        *peMthod = SIP_METHOD_REGISTER;
    }
    else
    {
        return FAIL;
    }

    return SUCCESS;
}

/*******************************************************************************
Request-URI    =  SIP-URI / SIPS-URI / absoluteURI
*******************************************************************************/
ULONG SIP_ParseRequestURI(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                          UCHAR               *pucString,
                          UBUF_HEADER_S       *pstUbuf,
                          void               **ppStruct)
{
    ULONG ulRet;
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    URI_S *pstUri = NULL_PTR;

    pstNode = pstGrammarNode->pstChild;
    if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_SIP_URI))
    {
        ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_SIP_URI)(pstNode,
                                                          pucString,
                                                          pstUbuf,
                                                          ppStruct);
    }
    else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_SIPS_URI))
    {
        ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_SIPS_URI)(pstNode,
                                                           pucString,
                                                           pstUbuf,
                                                           ppStruct);
    }
    else
    {
        return FAIL;
    }

    return ulRet;
}

/*******************************************************************************
SIP-Version    =  "SIP" "/" 1*DIGIT "." 1*DIGIT
*******************************************************************************/
ULONG SIP_ParseSIPVersion(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                          UCHAR               *pucString,
                          UBUF_HEADER_S       *pstUbuf,
                          void               **ppStruct)
{
    UCHAR *pucChar = NULL_PTR;
    UCHAR  ucVersion= 0;
    UCHAR *pucVersion = NULL_PTR;

    SIP_GET_COMPONET_PTR(pucVersion, UCHAR, pstUbuf, ppStruct);

    /* 需要跳过"SIP/"，只算整数部分，小数不管 */
    pucChar = SIP_GET_OFFSET_POINT(pucString, pstGrammarNode->ulOffset + 4);
    SIP_GET_DEC_NUM(ucVersion, pucChar);

    *pucVersion = ucVersion;
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
ULONG SIP_ParseStatusCode(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                          UCHAR               *pucString,
                          UBUF_HEADER_S       *pstUbuf,
                          void               **ppStruct)
{
    UCHAR *pucChar = NULL_PTR;
    ULONG  ulStatusCode = 0;
    SIP_STATUS_CODE_E *peStatusCode = NULL_PTR;

    SIP_GET_COMPONET_PTR(peStatusCode, SIP_STATUS_CODE_E, pstUbuf, ppStruct);

    /* 直接计算错误码 */
    pucChar = SIP_GET_OFFSET_POINT(pucString, pstGrammarNode->ulOffset);
    SIP_GET_DEC_NUM(ulStatusCode, pucChar);

    *peStatusCode = ulStatusCode;
    return SUCCESS;
}

/*******************************************************************************
SIP-URI          =  "sip:" [ userinfo ] hostport
                    uri-parameters [ headers ]
*******************************************************************************/
ULONG SIP_ParseSipURI(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                      UCHAR               *pucString,
                      UBUF_HEADER_S       *pstUbuf,
                      void               **ppStruct)
{
    ULONG ulRet;
    ABNF_GRAMMAR_NODE_S *pstNode     = NULL_PTR;
    URI_USER_INFO_S     *pstUserInfo = NULL_PTR;
    URI_S               *pstUri      = NULL_PTR;
    void                *pStruct     = NULL_PTR;

    SIP_GET_COMPONET_PTR(pstUri, URI_S, pstUbuf, ppStruct);

    pstUri->eUriType = URI_TYPE_SIP;
    pstNode = pstGrammarNode->pstChild;
    while (pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_USERINFO))
        {
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_USERINFO)(pstNode,
                                                               pucString,
                                                               pstUbuf,
                                                              &pstUri->u.stSipUri.pstUserInfo);
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_HOSTPORT))
        {
            pStruct = &pstUri->u.stSipUri.stHostPort;
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_HOSTPORT)(pstNode,
                                                               pucString,
                                                               pstUbuf,
                                                              &pStruct);
        }

        pstNode = pstNode->pstNextNode;
    }

    return SUCCESS;
}

/*******************************************************************************
SIPS-URI         =  "sips:" [ userinfo ] hostport
                    uri-parameters [ headers ]
*******************************************************************************/
ULONG SIP_ParseSipsURI(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                       UCHAR               *pucString,
                       UBUF_HEADER_S       *pstUbuf,
                       void               **ppStruct)
{
    ULONG ulRet;
    ABNF_GRAMMAR_NODE_S *pstNode     = NULL_PTR;
    URI_S               *pstUri      = NULL_PTR;
    void                *pStruct   = NULL_PTR;

    SIP_GET_COMPONET_PTR(pstUri, URI_S, pstUbuf, ppStruct);

    pstUri->eUriType = URI_TYPE_SIPS;
    pstNode = pstGrammarNode->pstChild;
    while (pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_USERINFO))
        {
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_USERINFO)(pstNode,
                                                               pucString,
                                                               pstUbuf,
                                                              &pstUri->u.stSipUri.pstUserInfo);
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_HOSTPORT))
        {
            pStruct = &pstUri->u.stSipUri.stHostPort;
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_HOSTPORT)(pstNode,
                                                               pucString,
                                                               pstUbuf,
                                                              &pStruct);
        }

        pstNode = pstNode->pstNextNode;
    }

    return SUCCESS;
}

/*******************************************************************************
userinfo         =  ( user / telephone-subscriber ) [ ":" password ] "@"
*******************************************************************************/
ULONG SIP_ParseUserinfo(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                        UCHAR               *pucString,
                        UBUF_HEADER_S       *pstUbuf,
                        void               **ppStruct)
{
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    UCHAR *pucStr = NULL_PTR;
    URI_USER_INFO_S *pstUserInfo = NULL_PTR;

    SIP_GET_COMPONET_PTR(pstUserInfo, URI_USER_INFO_S, pstUbuf, ppStruct);

    pstNode = pstGrammarNode->pstChild;
    while (pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_USER))
        {
            SIP_GET_STRING(pucStr, pstNode, pucString, pstUbuf, &pstUserInfo->pucUserInfo);
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_USER))
        {
            SIP_GET_STRING(pucStr, pstNode, pucString, pstUbuf, &pstUserInfo->pucPassword);
        }

        pstNode = pstNode->pstNextNode;
    }

    return SUCCESS;
}

/*******************************************************************************
hostport         =  host [ ":" port ]
*******************************************************************************/
ULONG SIP_ParseHostport(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                        UCHAR               *pucString,
                        UBUF_HEADER_S       *pstUbuf,
                        void               **ppStruct)
{
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    ULONG ulRet;
    URI_HOST_PORT_S *pstHostPort = NULL_PTR;
    void                *pStruct   = NULL_PTR;

    SIP_GET_COMPONET_PTR(pstHostPort, URI_HOST_PORT_S, pstUbuf, ppStruct);

    /* 初始化非0字段 */
    pstHostPort->usPort = NULL_USHORT;

    pstNode = pstGrammarNode->pstChild;
    while (pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_HOST))
        {
            pStruct = &pstHostPort->stHost;
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_HOST)(pstNode,
                                                          pucString,
                                                          pstUbuf,
                                                         &pStruct);
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_PORT))
        {
            pStruct = &pstHostPort->usPort;
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_PORT)(pstNode,
                                                                  pucString,
                                                                  pstUbuf,
                                                                 &pStruct);
        }

        pstNode = pstNode->pstNextNode;
    }

    return SUCCESS;
}

/*******************************************************************************
host             =  hostname / IPv4address / IPv6reference
*******************************************************************************/
ULONG SIP_ParseHost(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                    UCHAR               *pucString,
                    UBUF_HEADER_S       *pstUbuf,
                    void               **ppStruct)
{
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    UCHAR       *pucStrPtr = NULL_PTR;
    URI_HOST_S *pstHost = NULL_PTR;

    SIP_GET_COMPONET_PTR(pstHost, URI_HOST_S, pstUbuf, ppStruct);

    pstNode = pstGrammarNode->pstChild;
    if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_HOST_NAME))
    {
        pstHost->eHostType = URI_HOST_DOMAIN;
        /* ABNF中没有检查合法性，这里要添加检查 */
    }
    else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_IPV4_ADDRESS))
    {
        pstHost->eHostType = URI_HOST_IPV4;
    }
    else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_IPV6_REFERENCE))
    {
        pstHost->eHostType = URI_HOST_IPV6;
    }
    else
    {
        return FAIL;
    }

    SIP_GET_STRING(pucStrPtr, pstNode, pucString, pstUbuf, &pstHost->pucAddrStr);

    return SUCCESS;
}

/*******************************************************************************
port           =  1*DIGIT
*******************************************************************************/
ULONG SIP_ParsePort(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                    UCHAR               *pucString,
                    UBUF_HEADER_S       *pstUbuf,
                    void               **ppStruct)
{
    UCHAR *pucChar = NULL_PTR;
    USHORT usPort = 0;
    USHORT *pusPort = NULL_PTR;

    SIP_GET_COMPONET_PTR(pusPort, USHORT, pstUbuf, ppStruct);

    /* 直接计算错误码 */
    pucChar = SIP_GET_OFFSET_POINT(pucString, pstGrammarNode->ulOffset);
    SIP_GET_DEC_NUM(usPort, pucChar);
    *pusPort = usPort;

    return SUCCESS;
}

/*******************************************************************************
from-spec   =  ( name-addr / addr-spec )
               *( SEMI from-param )
*******************************************************************************/
ULONG SIP_ParseFromSpec(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                        UCHAR               *pucString,
                        UBUF_HEADER_S       *pstUbuf,
                        void               **ppStruct)
{
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    ULONG ulRet;
    SIP_HEADER_FROM_S   *pstFrom = NULL_PTR;
    void                *pStruct   = NULL_PTR;

    SIP_GET_COMPONET_PTR(pstFrom, SIP_HEADER_FROM_S, pstUbuf, ppStruct);

    pstNode = pstGrammarNode->pstChild;
    while (pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_NAME_ADDR))
        {
            pstFrom->stNameAddr.bName = TRUE;
            pStruct = &pstFrom->stNameAddr;
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_NAME_ADDR)(pstNode,
                                                                pucString,
                                                                pstUbuf,
                                                               &pStruct);
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_ADDR_SPEC))
        {
            pstFrom->stNameAddr.bName = FALSE;
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_ADDR_SPEC)(pstNode,
                                                                pucString,
                                                                pstUbuf,
                                                               &pstFrom->stNameAddr.pstUri);
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_FROM_PARAM))
        {
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_FROM_PARAM)(pstNode,
                                                                 pucString,
                                                                 pstUbuf,
                                                                 ppStruct);
        }

        pstNode = pstNode->pstNextNode;
    }

    return SUCCESS;
}

/*******************************************************************************
name-addr      =  [ display-name ] LAQUOT addr-spec RAQUOT
*******************************************************************************/
ULONG SIP_ParseNameAddr(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                        UCHAR               *pucString,
                        UBUF_HEADER_S       *pstUbuf,
                        void               **ppStruct)
{
    ABNF_GRAMMAR_NODE_S *pstNode     = NULL_PTR;
    ULONG                ulRet;
    UCHAR               *pucStrPtr   = NULL_PTR;
    SIP_NAME_ADDR_S     *pstNameAddr = NULL_PTR;

    SIP_GET_COMPONET_PTR(pstNameAddr, SIP_NAME_ADDR_S, pstUbuf, ppStruct);

    pstNode = pstGrammarNode->pstChild;
    while (pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_DISPLAY_NAME))
        {
            SIP_GET_STRING(pucStrPtr, pstNode, pucString, pstUbuf, &pstNameAddr->pucName);
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_ADDR_SPEC))
        {
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_ADDR_SPEC)(pstNode,
                                                                pucString,
                                                                pstUbuf,
                                                               &pstNameAddr->pstUri);
        }

        pstNode = pstNode->pstNextNode;
    }

    return ulRet;
}

/*******************************************************************************
addr-spec      =  SIP-URI / SIPS-URI / absoluteURI
*******************************************************************************/
ULONG SIP_ParseAddrSpec(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                        UCHAR               *pucString,
                        UBUF_HEADER_S       *pstUbuf,
                        void               **ppStruct)
{
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    ULONG       ulRet;

    pstNode = pstGrammarNode->pstChild;
    if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_SIP_URI))
    {
        ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_SIP_URI)(pstNode,
                                                          pucString,
                                                          pstUbuf,
                                                          ppStruct);
    }
    else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_SIPS_URI))
    {
        ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_SIPS_URI)(pstNode,
                                                          pucString,
                                                          pstUbuf,
                                                          ppStruct);
    }
    else
    {
        return FAIL;
    }

    return ulRet;
}

/*******************************************************************************
from-param  =  tag-param / generic-param
*******************************************************************************/
ULONG SIP_ParseFromParam(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                         UCHAR               *pucString,
                         UBUF_HEADER_S       *pstUbuf,
                         void               **ppStruct)
{
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    SIP_HEADER_FROM_S   *pstFrom = NULL_PTR;

    SIP_GET_COMPONET_PTR(pstFrom, SIP_HEADER_FROM_S, pstUbuf, ppStruct);

    /* 目前只处理tag参数 */
    pstNode = pstGrammarNode->pstChild;
    if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_TAG_PARAM))
    {
        return SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_TAG_PARAM)(pstNode,
                                                           pucString,
                                                           pstUbuf,
                                                          &pstFrom->pucTag);
    }

    return SUCCESS;
}

/*******************************************************************************
to-param  =  tag-param / generic-param
*******************************************************************************/
ULONG SIP_ParseToParam(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                       UCHAR               *pucString,
                       UBUF_HEADER_S       *pstUbuf,
                       void               **ppStruct)
{
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    SIP_HEADER_TO_S     *pstTo = NULL_PTR;

    SIP_GET_COMPONET_PTR(pstTo, SIP_HEADER_TO_S, pstUbuf, ppStruct);

    /* 目前只处理tag参数 */
    pstNode = pstGrammarNode->pstChild;
    if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_TAG_PARAM))
    {
        return SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_TAG_PARAM)(pstNode,
                                                           pucString,
                                                           pstUbuf,
                                                          &pstTo->pucTag);
    }

    return SUCCESS;
}

/*******************************************************************************
tag-param   =  "tag" EQUAL token
*******************************************************************************/
ULONG SIP_ParseTagParam(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                       UCHAR               *pucString,
                       UBUF_HEADER_S       *pstUbuf,
                       void               **ppStruct)
{
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    UCHAR *pucStrPtr = NULL_PTR;

    pstNode = pstGrammarNode->pstChild;
    while (pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_TOKEN))
        {
            SIP_GET_STRING(pucStrPtr, pstNode, pucString, pstUbuf, ppStruct);
            return SUCCESS;
        }

        pstNode = pstNode->pstNextNode;
    }

    return FAIL;
}

/*******************************************************************************
callid   =  word [ "@" word ]
*******************************************************************************/
ULONG SIP_ParseCallID(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                       UCHAR               *pucString,
                       UBUF_HEADER_S       *pstUbuf,
                       void               **ppStruct)
{
    UCHAR *pucStrPtr = NULL_PTR;
    SIP_HEADER_CALL_ID_S *pstCallID = NULL_PTR;

    SIP_GET_COMPONET_PTR(pstCallID, SIP_HEADER_CALL_ID_S, pstUbuf, ppStruct);

    /* 先简单实现 */
    SIP_GET_STRING(pucStrPtr, pstGrammarNode, pucString, pstUbuf, &pstCallID->pucCallID);

    return SUCCESS;
}

/*******************************************************************************
via-parm          =  sent-protocol LWS sent-by *( SEMI via-params )
*******************************************************************************/
ULONG SIP_ParseViaParm(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                       UCHAR               *pucString,
                       UBUF_HEADER_S       *pstUbuf,
                       void               **ppStruct)
{
    ULONG ulRet;
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    void                *pStruct   = NULL_PTR;
    SIP_VIA_PARM_S *pstViaParm = NULL_PTR;

    SIP_GET_COMPONET_PTR(pstViaParm, SIP_VIA_PARM_S, pstUbuf, ppStruct);

    /* 初始化非0字段 */
    pstViaParm->ulTtl = NULL_ULONG;

    pstNode = pstGrammarNode->pstChild;
    while (pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_SENT_PROTOCOL))
        {
            pStruct = &pstViaParm->eProtocolType;
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_SENT_PROTOCOL)(pstNode,
                                                                    pucString,
                                                                    pstUbuf,
                                                                   &pStruct);
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_SENT_BY))
        {
            pStruct = &pstViaParm->stSendBy;
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_SENT_BY)(pstNode,
                                                              pucString,
                                                              pstUbuf,
                                                             &pStruct);
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_VIA_PARMS))
        {
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_VIA_PARMS)(pstNode,
                                                                pucString,
                                                                pstUbuf,
                                                                ppStruct);
        }

        pstNode = pstNode->pstNextNode;
    }

    return ulRet;
}

/*******************************************************************************
sent-protocol     =  protocol-name SLASH protocol-version
                     SLASH transport
*******************************************************************************/
ULONG SIP_ParseSentProtocol(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                            UCHAR               *pucString,
                            UBUF_HEADER_S       *pstUbuf,
                            void               **ppStruct)
{
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    ULONG ulRet;

    pstNode = pstGrammarNode->pstChild;
    while (pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_TRANSPORT))
        {
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_TRANSPORT)(pstNode,
                                                                pucString,
                                                                pstUbuf,
                                                                ppStruct);
            break;
        }

        pstNode = pstNode->pstNextNode;
    }

    return ulRet;
}

/*******************************************************************************
transport         =  "UDP" / "TCP" / "TLS" / "SCTP"
                     / other-transport
*******************************************************************************/
ULONG SIP_ParseTransport(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                         UCHAR               *pucString,
                         UBUF_HEADER_S       *pstUbuf,
                         void               **ppStruct)
{
    UCHAR      *pucStrPtr = NULL_PTR;
    SIP_TRANSPORT_PROTOCOL_E *peProtocol = NULL_PTR;

    SIP_GET_COMPONET_PTR(peProtocol, SIP_TRANSPORT_PROTOCOL_E, pstUbuf, ppStruct);

    pucStrPtr = SIP_GET_OFFSET_POINT(pucString, pstGrammarNode->ulOffset);
    if (SUCCESS == strncmp(pucStrPtr, "UDP", strlen("UDP")))
    {
        *peProtocol = SIP_TRANSPORT_PROTOCOL_UDP;
    }
    else if (SUCCESS == strncmp(pucStrPtr, "TCP", strlen("TCP")))
    {
        *peProtocol = SIP_TRANSPORT_PROTOCOL_TCP;
    }
    else if (SUCCESS == strncmp(pucStrPtr, "TLS", strlen("TLS")))
    {
        *peProtocol = SIP_TRANSPORT_PROTOCOL_TLS;
    }
    else if (SUCCESS == strncmp(pucStrPtr, "SCTP", strlen("SCTP")))
    {
        *peProtocol = SIP_TRANSPORT_PROTOCOL_SCTP;
    }
    else
    {
        return FAIL;
    }

    return SUCCESS;
}

/*******************************************************************************
sent-by           =  host [ COLON port ]
*******************************************************************************/
ULONG SIP_ParseSentBy(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                      UCHAR               *pucString,
                      UBUF_HEADER_S       *pstUbuf,
                      void               **ppStruct)
{
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    ULONG ulRet;
    void                *pStruct   = NULL_PTR;
    URI_HOST_PORT_S     *pstSentBy = NULL_PTR;

    SIP_GET_COMPONET_PTR(pstSentBy, URI_HOST_PORT_S, pstUbuf, ppStruct);

    /* 初始化非0字段 */
    pstSentBy->usPort = NULL_USHORT;

    pstNode = pstGrammarNode->pstChild;
    while (pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_HOST))
        {
            pStruct = &pstSentBy->stHost;
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_HOST)(pstNode,
                                                           pucString,
                                                           pstUbuf,
                                                          &pStruct);
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_PORT))
        {
            pStruct = &pstSentBy->usPort;
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_HOST)(pstNode,
                                                           pucString,
                                                           pstUbuf,
                                                          &pStruct);
        }

        pstNode = pstNode->pstNextNode;
    }

    return ulRet;
}

/*******************************************************************************
via-params        =  via-ttl / via-maddr
                     / via-received / via-branch
                     / via-extension
*******************************************************************************/
ULONG SIP_ParseViaParams(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                         UCHAR               *pucString,
                         UBUF_HEADER_S       *pstUbuf,
                         void               **ppStruct)
{
    ULONG ulRet = SUCCESS;
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    URI_HOST_S          *pstHost = NULL_PTR;
    SIP_VIA_RECEIVED_S  *pstViaReceived = NULL_PTR;
    void                *pStruct   = NULL_PTR;
    SIP_VIA_PARM_S *pstViaParm = NULL_PTR;

    SIP_GET_COMPONET_PTR(pstViaParm, SIP_VIA_PARM_S, pstUbuf, ppStruct);

    pstNode = pstGrammarNode->pstChild;
    while (pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_VIA_TTL))
        {
            pStruct = &pstViaParm->ulTtl;
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_VIA_TTL)(pstNode,
                                                              pucString,
                                                              pstUbuf,
                                                             &pStruct);
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_VIA_MADDR))
        {
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_VIA_MADDR)(pstNode,
                                                                pucString,
                                                                pstUbuf,
                                                               &pstViaParm->pstMaddr);
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_VIA_RECEIVED))
        {
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_VIA_RECEIVED)(pstNode,
                                                                   pucString,
                                                                   pstUbuf,
                                                                  &pstViaParm->pstReceived);
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_VIA_BRANCH))
        {
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_VIA_BRANCH)(pstNode,
                                                                 pucString,
                                                                 pstUbuf,
                                                                 ppStruct);
        }

        pstNode = pstNode->pstNextNode;
    }

    return ulRet;
}

/*******************************************************************************
via-ttl           =  "ttl" EQUAL ttl
*******************************************************************************/
ULONG SIP_ParseViaTtl(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                      UCHAR               *pucString,
                      UBUF_HEADER_S       *pstUbuf,
                      void               **ppStruct)
{
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    UCHAR *pucChar = NULL_PTR;
    ULONG ulTtl;
    ULONG *pulTtl = NULL_PTR;

    SIP_GET_COMPONET_PTR(pulTtl, ULONG, pstUbuf, ppStruct);

    pstNode = pstGrammarNode->pstChild;
    while (pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_TTL))
        {
            pucChar = SIP_GET_OFFSET_POINT(pucString, pstNode->ulOffset);
            SIP_GET_DEC_NUM(ulTtl, pucChar);
            break;
        }

        pstNode = pstNode->pstNextNode;
    }

    *pulTtl = ulTtl;
    return SUCCESS;
}

/*******************************************************************************
via-maddr         =  "maddr" EQUAL host
*******************************************************************************/
ULONG SIP_ParseViaMaddr(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                        UCHAR               *pucString,
                        UBUF_HEADER_S       *pstUbuf,
                        void               **ppStruct)
{
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    ULONG ulRet;

    pstNode = pstGrammarNode->pstChild;
    while (pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_HOST))
        {
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_HOST)(pstNode,
                                                           pucString,
                                                           pstUbuf,
                                                           ppStruct);
            break;
        }

        pstNode = pstNode->pstNextNode;
    }

    return ulRet;
}

/*******************************************************************************
via-received      =  "received" EQUAL (IPv4address / IPv6address)
*******************************************************************************/
ULONG SIP_ParseViaReceived(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                           UCHAR               *pucString,
                           UBUF_HEADER_S       *pstUbuf,
                           void               **ppStruct)
{
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    UCHAR *pucStrPtr = NULL_PTR;
    SIP_VIA_RECEIVED_S  *pstViaReceived = NULL_PTR;

    SIP_GET_COMPONET_PTR(pstViaReceived, SIP_VIA_RECEIVED_S, pstUbuf, ppStruct);

    pstNode = pstGrammarNode->pstChild;
    while (pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_IPV4_ADDRESS))
        {
            pstViaReceived->eIpType = SIP_IP_TYPE_IPV4;
            SIP_GET_STRING(pucStrPtr, pstNode, pucString, pstUbuf, &pstViaReceived->u.pucIPV4);
            break;
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_IPV6_ADDRESS))
        {
            pstViaReceived->eIpType = SIP_IP_TYPE_IPV6;
            SIP_GET_STRING(pucStrPtr, pstNode, pucString, pstUbuf, &pstViaReceived->u.pucIPV6);
            break;
        }

        pstNode = pstNode->pstNextNode;
    }

    return SUCCESS;
}

/*******************************************************************************
via-branch        =  "branch" EQUAL token
*******************************************************************************/
ULONG SIP_ParseViaBranch(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                         UCHAR               *pucString,
                         UBUF_HEADER_S       *pstUbuf,
                         void               **ppStruct)
{
    ULONG ulRet = SUCCESS;
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    UCHAR *pucStrPtr = NULL_PTR;
    SIP_VIA_PARM_S *pstViaParm = NULL_PTR;

    SIP_GET_COMPONET_PTR(pstViaParm, SIP_VIA_PARM_S, pstUbuf, ppStruct);

    pstNode = pstGrammarNode->pstChild;
    while (pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_TOKEN))
        {
            SIP_GET_STRING(pucStrPtr, pstNode, pucString, pstUbuf, &pstViaParm->pucBranch);
            break;
        }

        pstNode = pstNode->pstNextNode;
    }

    return ulRet;
}

/*******************************************************************************
contact-param  =  (name-addr / addr-spec) *(SEMI contact-params)
*******************************************************************************/
ULONG SIP_ParseContactParam(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                            UCHAR               *pucString,
                            UBUF_HEADER_S       *pstUbuf,
                            void               **ppStruct)
{
    ULONG ulRet = SUCCESS;
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    SIP_CONTACT_PARAM_S *pstContactParam = NULL_PTR;
    void                *pStruct   = NULL_PTR;

    SIP_GET_COMPONET_PTR(pstContactParam, SIP_CONTACT_PARAM_S, pstUbuf, ppStruct);
    pstContactParam->ulExpires = NULL_ULONG;
    pstContactParam->pstNext   = NULL_PTR;

    pstNode = pstGrammarNode->pstChild;
    while (pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_NAME_ADDR))
        {
            pStruct = &pstContactParam->stAddr;
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_NAME_ADDR)(pstNode,
                                                                pucString,
                                                                pstUbuf,
                                                               &pStruct);
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_ADDR_SPEC))
        {
            pStruct = &pstContactParam->stAddr;
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_ADDR_SPEC)(pstNode,
                                                                pucString,
                                                                pstUbuf,
                                                               &pStruct);
        }

        pstNode = pstNode->pstNextNode;
    }

    return ulRet;
}


/* 下面是所有头域的编码函数 */
/*******************************************************************************
Accept         =  "Accept" HCOLON
                   [ accept-range *(COMMA accept-range) ]
*******************************************************************************/
ULONG SIP_ParseHeaderAccept(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                            UCHAR               *pucString,
                            UBUF_HEADER_S       *pstUbuf,
                            void               **ppStruct)
{
    return SUCCESS;
}

/*******************************************************************************
Accept-Encoding  =  "Accept-Encoding" HCOLON
                     [ encoding *(COMMA encoding) ]
*******************************************************************************/
ULONG SIP_ParseHeaderAcceptEncoding(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                                    UCHAR               *pucString,
                                    UBUF_HEADER_S       *pstUbuf,
                                    void               **ppStruct)
{
    return SUCCESS;
}

/*******************************************************************************
Call-ID  =  ( "Call-ID" / "i" ) HCOLON callid
*******************************************************************************/
ULONG SIP_ParseHeaderCallID(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                            UCHAR               *pucString,
                            UBUF_HEADER_S       *pstUbuf,
                            void               **ppStruct)
{
    ULONG ulRet;
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;

    pstNode = pstGrammarNode->pstChild;
    while (pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_CALLID))
        {
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_CALLID)(pstNode,
                                                             pucString,
                                                             pstUbuf,
                                                             ppStruct);
            if (ulRet != SUCCESS)
            {
                return FAIL;
            }

            break;
        }

        pstNode = pstNode->pstNextNode;
    }

    return SUCCESS;
}

/*******************************************************************************
CSeq  =  "CSeq" HCOLON 1*DIGIT LWS Method
*******************************************************************************/
ULONG SIP_ParseHeaderCseq(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                          UCHAR               *pucString,
                          UBUF_HEADER_S       *pstUbuf,
                          void               **ppStruct)
{
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    SIP_HEADER_CSEQ_S *pstSeq = NULL_PTR;
    UCHAR *pucChar = NULL_PTR;
    ULONG ulRet;
    void                *pStruct   = NULL_PTR;

    SIP_GET_COMPONET_PTR(pstSeq, SIP_HEADER_CSEQ_S, pstUbuf, ppStruct);

    /* 直接获取序号值，至少跳过"CSeq" */
    pucChar = SIP_GET_OFFSET_POINT(pucString, pstGrammarNode->ulOffset+ strlen("CSeq"));
    while ((*pucChar < '0')||(*pucChar > '9'))
    {
        pucChar++;
    }

    SIP_GET_DEC_NUM(pstSeq->ulSeq, pucChar);

    /* 获取方法 */
    pstNode = pstGrammarNode->pstChild;
    while (pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_METHOD))
        {
            pStruct = &pstSeq->eMethod;
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_METHOD)(pstNode,
                                                             pucString,
                                                             pstUbuf,
                                                            &pStruct);
        }

        pstNode = pstNode->pstNextNode;
    }

    return SUCCESS;
}

/*******************************************************************************
From        =  ( "From" / "f" ) HCOLON from-spec
*******************************************************************************/
ULONG SIP_ParseHeaderFrom(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                          UCHAR               *pucString,
                          UBUF_HEADER_S       *pstUbuf,
                          void               **ppStruct)
{
    ULONG ulRet;
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;

    pstNode = pstGrammarNode->pstChild;
    while (pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_FROM_SPEC))
        {
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_FROM_SPEC)(pstNode,
                                                                pucString,
                                                                pstUbuf,
                                                                ppStruct);
            if (ulRet != SUCCESS)
            {
                return FAIL;
            }

            break;
        }

        pstNode = pstNode->pstNextNode;
    }

    return SUCCESS;
}

/*******************************************************************************
To        =  ( "To" / "t" ) HCOLON ( name-addr
             / addr-spec ) *( SEMI to-param )
*******************************************************************************/
ULONG SIP_ParseHeaderTo(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                        UCHAR               *pucString,
                        UBUF_HEADER_S       *pstUbuf,
                        void               **ppStruct)
{
    ULONG ulRet;
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    SIP_HEADER_TO_S     *pstTo   = NULL_PTR;
    void                *pStruct   = NULL_PTR;

    SIP_GET_COMPONET_PTR(pstTo, SIP_HEADER_TO_S, pstUbuf, ppStruct);

    pstNode = pstGrammarNode->pstChild;
    while (pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_NAME_ADDR))
        {
            pstTo->stNameAddr.bName = TRUE;
            pStruct = &pstTo->stNameAddr;
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_NAME_ADDR)(pstNode,
                                                                pucString,
                                                                pstUbuf,
                                                               &pStruct);
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_ADDR_SPEC))
        {
            pstTo->stNameAddr.bName = FALSE;
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_ADDR_SPEC)(pstNode,
                                                                pucString,
                                                                pstUbuf,
                                                               &pstTo->stNameAddr.pstUri);
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_TO_PARAM))
        {
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_ADDR_SPEC)(pstNode,
                                                                pucString,
                                                                pstUbuf,
                                                                ppStruct);
        }

        pstNode = pstNode->pstNextNode;
    }

    return SUCCESS;
}

/*******************************************************************************
Via               =  ( "Via" / "v" ) HCOLON via-parm *(COMMA via-parm)
*******************************************************************************/
ULONG SIP_ParseHeaderVia(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                         UCHAR               *pucString,
                         UBUF_HEADER_S       *pstUbuf,
                         void               **ppStruct)
{
    ULONG ulRet;
    ABNF_GRAMMAR_NODE_S *pstNode    = NULL_PTR;
    SIP_HEADER_VIA_S    *pstVia     = NULL_PTR;
    SIP_VIA_PARM_S     **ppstViaParm = NULL_PTR;

    SIP_GET_COMPONET_PTR(pstVia, SIP_HEADER_VIA_S, pstUbuf, ppStruct);

    ppstViaParm = &pstVia->pstViaParm;
    pstNode = pstGrammarNode->pstChild;
    while (pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_VIA_PARM))
        {
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_VIA_PARM)(pstNode,
                                                               pucString,
                                                               pstUbuf,
                                                               ppstViaParm);
            if (ulRet != SUCCESS)
            {
                return FAIL;
            }
            else
            {
                ppstViaParm = &(*ppstViaParm)->pstNext;
            }
        }

        pstNode = pstNode->pstNextNode;
    }

    return SUCCESS;
}

/*******************************************************************************
Max-Forwards  =  "Max-Forwards" HCOLON 1*DIGIT
*******************************************************************************/
ULONG SIP_ParseHeaderMaxForwards(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                                 UCHAR               *pucString,
                                 UBUF_HEADER_S       *pstUbuf,
                                 void               **ppStruct)
{
    SIP_HEADER_MAX_FORWARDS_S *pstMaxForwards = NULL_PTR;
    UCHAR *pucChar = NULL_PTR;

    SIP_GET_COMPONET_PTR(pstMaxForwards, SIP_HEADER_MAX_FORWARDS_S, pstUbuf, ppStruct);

    /* 直接获取数字，至少跳过"Max-Forwards" */
    pucChar = SIP_GET_OFFSET_POINT(pucString, pstGrammarNode->ulOffset + strlen("Max-Forwards"));
    while ((*pucChar < '0')||(*pucChar > '9'))
    {
        pucChar++;
    }

    SIP_GET_DEC_NUM(pstMaxForwards->ulMaxForwards, pucChar);

    return SUCCESS;
}

/*******************************************************************************
Contact        =  ("Contact" / "m" ) HCOLON
                  ( STAR / (contact-param *(COMMA contact-param)))
*******************************************************************************/
ULONG SIP_ParseHeaderContact(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                             UCHAR               *pucString,
                             UBUF_HEADER_S       *pstUbuf,
                             void               **ppStruct)
{
    ULONG ulRet;
    ABNF_GRAMMAR_NODE_S  *pstNode    = NULL_PTR;
    SIP_HEADER_CONTACT_S *pstContact = NULL_PTR;
    SIP_CONTACT_PARAM_S **ppstParm   = NULL_PTR;

    SIP_GET_COMPONET_PTR(pstContact, SIP_HEADER_CONTACT_S, pstUbuf, ppStruct);
    pstContact->ucIsStar = FALSE;
    pstContact->pstParam = NULL_PTR;

    ppstParm = &pstContact->pstParam;
    pstNode = pstGrammarNode->pstChild;
    while (pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_STAR))
        {
            pstContact->ucIsStar = TRUE;
            return SUCCESS;
        }

        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_CONTACT_PARAM))
        {
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_CONTACT_PARAM)(pstNode,
                                                                    pucString,
                                                                    pstUbuf,
                                                                    ppstParm);
            if (ulRet != SUCCESS)
            {
                return FAIL;
            }
            else
            {
                ppstParm = &(*ppstParm)->pstNext;
            }

        }

        pstNode = pstNode->pstNextNode;
    }

    return SUCCESS;
}

