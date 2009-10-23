/*******************************************************************************
����    : SIP��Ϣ��������
������  : �ƴ�ƽ
��������: 200.01.10
�޸ļ�¼:

����ԭ��:
1�� ���еĽ��������β�һ��
    ABNF_GRAMMAR_NODE_S *pstGrammarNode: �ṹ��ABNF�ڵ��ʾ
    UCHAR               *pucString     : ���������ı�
    UBUF_HEADER_S       *pstUbuf       : ���������UBUF�ṹ
    UBUF_PTR            *pupStruct     : �������ṹ����UBUF�е�ƫ��(UBUFָ��)
2�� ����ṹֻ���������ѡ���ӽṹ�����Ƿ����ռ��ڽ����ӽṹʱ�پ���
3�� ��ԭ��2���������pupStruct�������ж��Ƿ���Ҫ����ռ䣬��Ч����Ϊ�������Ѿ���
    ���˿ռ䣬���������룬����ú�������
4�� ���ڽṹ���ӽṹ������Ϊ��ͳһ�����������βΣ���Ҫ�����ӽṹUBUFָ�봫�ݸ���
    ���ú���
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
ULONG SIP_ParseSIPmessage(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                          UCHAR               *pucString,
                          UBUF_HEADER_S       *pstUbuf,
                          UBUF_PTR            *pupStruct)
{
    ULONG ulRet;
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;

    /* ԭ��2: ����ṹֻ���������ѡ���ӽṹ�����Ƿ����ռ��ڽ����ӽṹʱ�پ�����
    ��ͬ */

    pstNode = pstGrammarNode->pstChild;
    if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_REQUEST))
    {
        ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_REQUEST)(pstNode, pucString, pstUbuf, pupStruct);
    }
    else
    {
        ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_RESPONSE)(pstNode, pucString, pstUbuf, pupStruct);
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
                       UBUF_PTR            *pupStruct)
{
    ULONG ulRet;
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    SIP_MSG_S           *pstSipMsg = NULL_PTR;
    UBUF_PTR             upStruct = UBUF_NULL_PTR;

    /* ԭ��3: �������ȷ��������߲�����ռ䣬�����UBUFָ�����жϣ���Ч����Ϊ��
    �����Ѿ������˿ռ䣬���������룬����˴����룬��ͬ */
    SIP_GET_COMPONET_PTR(pstSipMsg, SIP_MSG_S, pstUbuf, pupStruct);

    pstSipMsg->eMsgType = SIP_MSG_TYPE_REQUEST;

    pstNode = pstGrammarNode->pstChild;
    while(pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_REQUEST_LINE))
        {
            /* ԭ��4: ���ڽṹ���ӽṹ������Ϊ��ͳһ�����������βΣ���Ҫ�����ӽ�
            ��UBUFָ�봫�ݸ������ú�������ͬ */
            upStruct = (ULONG)*pupStruct
                      +(ULONG)&pstSipMsg->uStartLine.stRequstLine
                      -(ULONG)pstSipMsg;
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_REQUEST_LINE)(pstNode,
                                                                   pucString,
                                                                   pstUbuf,
                                                                  &upStruct);
            if (ulRet != SUCCESS)
            {
                /* ��ʼ�в��ܴ� */
                return FAIL;
            }
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_MESSAGE_HEADER))
        {
            SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_MESSAGE_HEADER)(pstNode,
                                                             pucString,
                                                             pstUbuf,
                                                             pupStruct);
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_MESSAGE_BODY))
        {
            SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_MESSAGE_BODY)(pstNode,
                                                           pucString,
                                                           pstUbuf,
                                                           pupStruct);
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
                        UBUF_PTR            *pupStruct)
{
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    ULONG       ulRet;
    SIP_MSG_S  *pstSipMsg = NULL_PTR;
    UBUF_PTR    upStruct = UBUF_NULL_PTR;

    SIP_GET_COMPONET_PTR(pstSipMsg, SIP_MSG_S, pstUbuf, pupStruct);

    pstSipMsg->eMsgType = SIP_MSG_TYPE_RESPONSE;

    pstNode = pstGrammarNode->pstChild;
    while(pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_STATUS_LINE))
        {
            upStruct = *pupStruct
                      +(ULONG)&pstSipMsg->uStartLine.stStatusLine
                      -(ULONG)pstSipMsg;
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_STATUS_LINE)(pstNode,
                                                                  pucString,
                                                                  pstUbuf,
                                                                 &upStruct);
            if (ulRet != SUCCESS)
            {
                /* ��ʼ�в��ܴ� */
                return FAIL;
            }
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_MESSAGE_HEADER))
        {
            SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_MESSAGE_HEADER)(pstNode,
                                                            pucString,
                                                            pstUbuf,
                                                            pupStruct);
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_MESSAGE_BODY))
        {
            SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_MESSAGE_BODY)(pstNode,
                                                           pucString,
                                                           pstUbuf,
                                                           pupStruct);
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
                           UBUF_PTR            *pupStruct)
{
    ULONG ulRet;
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    SIP_REQUEST_LINE_S  *pstRequestLine = NULL_PTR;
    UBUF_PTR    upStruct = UBUF_NULL_PTR;

    SIP_GET_COMPONET_PTR(pstRequestLine, SIP_REQUEST_LINE_S, pstUbuf, pupStruct);

    pstNode = pstGrammarNode->pstChild;
    while (pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_METHOD))
        {
            upStruct = *pupStruct
                      +(ULONG)&pstRequestLine->eMethod
                      -(ULONG)pstRequestLine;
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_METHOD)(pstNode,
                                                             pucString,
                                                             pstUbuf,
                                                            &upStruct);
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_REQUEST_URI))
        {
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_REQUEST_URI)(pstNode,
                                                                  pucString,
                                                                  pstUbuf,
                                                                 &pstRequestLine->upRequestURI);
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_SIP_VERSION))
        {
            upStruct = *pupStruct
                      +(ULONG)&pstRequestLine->ucVersion
                      -(ULONG)pstRequestLine;
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_SIP_VERSION)(pstNode,
                                                                  pucString,
                                                                  pstUbuf,
                                                                 &upStruct);
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
                          UBUF_PTR            *pupStruct)
{
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    ULONG ulRet = SUCCESS;
    SIP_STATUS_LINE_S *pstStatusLine = NULL_PTR;
    UBUF_PTR    upStruct = UBUF_NULL_PTR;

    SIP_GET_COMPONET_PTR(pstStatusLine, SIP_STATUS_LINE_S, pstUbuf, pupStruct);

    pstNode = pstGrammarNode->pstChild;
    while (pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_SIP_VERSION))
        {
            upStruct = *pupStruct
                      +(ULONG)&pstStatusLine->ucVersion
                      -(ULONG)pstStatusLine;
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_SIP_VERSION)(pstNode,
                                                                pucString,
                                                                pstUbuf,
                                                               &upStruct);
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_STATUS_CODE))
        {
            upStruct = *pupStruct
                      +(ULONG)&pstStatusLine->eStatusCode
                      -(ULONG)pstStatusLine;
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_STATUS_CODE)(pstNode,
                                                                  pucString,
                                                                  pstUbuf,
                                                                 &upStruct);
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
                             UBUF_PTR            *pupStruct)
{
    ULONG ulHeader;
    ULONG ulAppIndex;
    ULONG ulStructSize;
    ABNF_GRAMMAR_NODE_S *pstNode   = NULL_PTR;
    UBUF_PTR             upStruct  = UBUF_NULL_PTR;
    UBUF_PTR            *pupstHeader = NULL_PTR;
    SIP_HEADER_S        *pstHeader = NULL_PTR;
    SIP_MSG_S           *pstSipMsg = NULL_PTR;

    SIP_GET_COMPONET_PTR(pstSipMsg, SIP_MSG_S, pstUbuf, pupStruct);

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
            /* û�н��뺯�������� */
            return SUCCESS;
        }

        /* �ҵ����һ����Ч��ͷ�� */
        pupstHeader = &pstSipMsg->aupstHeaders[ulHeader];
        while(*pupstHeader != UBUF_NULL_PTR)
        {
            pstHeader = (SIP_HEADER_S *)UBUF_UBufPtr2Ptr(pstUbuf, *pupstHeader);
            pupstHeader = &pstHeader->upstNext;
        }

        ulStructSize = SIP_GET_HEADER_SPEC_SIZE(ulHeader);
        pstHeader = UBUF_AddComponent(pstUbuf,
                                      sizeof(SIP_HEADER_S) + ulStructSize,
                                      pupstHeader);
        memset(pstHeader, 0xff, sizeof(SIP_HEADER_S) + ulStructSize);
        upStruct = *pupstHeader + sizeof(SIP_HEADER_S);
        return SIP_GET_PARSE_FUNC(ulAppIndex)(pstNode,
                                              pucString,
                                              pstUbuf,
                                             &upStruct);
    }

    return SUCCESS;
}

/*******************************************************************************
message-body  =  *OCTET
*******************************************************************************/
ULONG SIP_ParseMessageBody(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                           UCHAR               *pucString,
                           UBUF_HEADER_S       *pstUbuf,
                           UBUF_PTR            *pupStruct)
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
                      UBUF_PTR            *pupStruct)
{
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    SIP_METHOD_E        *peMthod  = NULL_PTR;

    SIP_GET_COMPONET_PTR(peMthod, SIP_METHOD_E, pstUbuf, pupStruct);

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
                          UBUF_PTR            *pupStruct)
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
                                                          pupStruct);
    }
    else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_SIPS_URI))
    {
        ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_SIPS_URI)(pstNode,
                                                           pucString,
                                                           pstUbuf,
                                                           pupStruct);
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
                          UBUF_PTR            *pupStruct)
{
    UCHAR *pucChar = NULL_PTR;
    UCHAR  ucVersion= 0;
    UCHAR *pucVersion = NULL_PTR;

    SIP_GET_COMPONET_PTR(pucVersion, UCHAR, pstUbuf, pupStruct);

    /* ��Ҫ����"SIP/"��ֻ���������֣�С������ */
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
                          UBUF_PTR            *pupStruct)
{
    UCHAR *pucChar = NULL_PTR;
    ULONG  ulStatusCode = 0;
    SIP_STATUS_CODE_E *peStatusCode = NULL_PTR;

    SIP_GET_COMPONET_PTR(peStatusCode, SIP_STATUS_CODE_E, pstUbuf, pupStruct);

    /* ֱ�Ӽ�������� */
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
                      UBUF_PTR            *pupStruct)
{
    ULONG ulRet;
    ABNF_GRAMMAR_NODE_S *pstNode     = NULL_PTR;
    URI_USER_INFO_S     *pstUserInfo = NULL_PTR;
    URI_S *pstUri = NULL_PTR;
    UBUF_PTR    upStruct = UBUF_NULL_PTR;

    SIP_GET_COMPONET_PTR(pstUri, URI_S, pstUbuf, pupStruct);

    pstUri->eUriType = URI_TYPE_SIP;
    pstNode = pstGrammarNode->pstChild;
    while (pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_USERINFO))
        {
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_USERINFO)(pstNode,
                                                               pucString,
                                                               pstUbuf,
                                                              &pstUri->u.stSipUri.upstUserInfo);
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_HOSTPORT))
        {
            upStruct = *pupStruct
                      +(ULONG)&pstUri->u.stSipUri.stHostPort
                      -(ULONG)pstUri;
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_HOSTPORT)(pstNode,
                                                               pucString,
                                                               pstUbuf,
                                                              &upStruct);
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
                       UBUF_PTR            *pupStruct)
{
    ULONG ulRet;
    ABNF_GRAMMAR_NODE_S *pstNode     = NULL_PTR;
    URI_USER_INFO_S     *pstUserInfo = NULL_PTR;
    URI_S               *pstUri      = NULL_PTR;
    UBUF_PTR             upStruct    = UBUF_NULL_PTR;

    SIP_GET_COMPONET_PTR(pstUri, URI_S, pstUbuf, pupStruct);

    pstUri->eUriType = URI_TYPE_SIPS;
    pstNode = pstGrammarNode->pstChild;
    while (pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_USERINFO))
        {
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_USERINFO)(pstNode,
                                                               pucString,
                                                               pstUbuf,
                                                              &pstUri->u.stSipUri.upstUserInfo);
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_HOSTPORT))
        {
            upStruct = *pupStruct
                      +(ULONG)&pstUri->u.stSipUri.stHostPort
                      -(ULONG)pstUri;
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_HOSTPORT)(pstNode,
                                                               pucString,
                                                               pstUbuf,
                                                              &upStruct);
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
                        UBUF_PTR            *pupStruct)
{
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    UCHAR *pucStr = NULL_PTR;
    URI_USER_INFO_S *pstUserInfo = NULL_PTR;

    SIP_GET_COMPONET_PTR(pstUserInfo, URI_USER_INFO_S, pstUbuf, pupStruct);

    pstNode = pstGrammarNode->pstChild;
    while (pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_USER))
        {
            SIP_GET_STRING(pucStr, pstNode, pucString, pstUbuf, &pstUserInfo->upucUserInfo);
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_USER))
        {
            SIP_GET_STRING(pucStr, pstNode, pucString, pstUbuf, &pstUserInfo->upucPassword);
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
                        UBUF_PTR            *pupStruct)
{
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    ULONG ulRet;
    URI_HOST_PORT_S *pstHostPort = NULL_PTR;
    UBUF_PTR upStruct = UBUF_NULL_PTR;

    SIP_GET_COMPONET_PTR(pstHostPort, URI_HOST_PORT_S, pstUbuf, pupStruct);

    /* ��ʼ����0�ֶ� */
    pstHostPort->usPort = NULL_USHORT;

    pstNode = pstGrammarNode->pstChild;
    while (pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_HOST))
        {
            upStruct = *pupStruct
                      +(ULONG)&pstHostPort->stHost
                      -(ULONG)pstHostPort;
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_HOST)(pstNode,
                                                          pucString,
                                                          pstUbuf,
                                                         &upStruct);
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_PORT))
        {
            upStruct = *pupStruct
                      +(ULONG)&pstHostPort->usPort
                      -(ULONG)pstHostPort;
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_PORT)(pstNode,
                                                                  pucString,
                                                                  pstUbuf,
                                                                 &upStruct);
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
                    UBUF_PTR            *pupStruct)
{
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    UCHAR       *pucStrPtr = NULL_PTR;
    URI_HOST_S *pstHost = NULL_PTR;

    SIP_GET_COMPONET_PTR(pstHost, URI_HOST_S, pstUbuf, pupStruct);

    pstNode = pstGrammarNode->pstChild;
    if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_HOST_NAME))
    {
        pstHost->eHostType = URI_HOST_DOMAIN;
        /* ABNF��û�м��Ϸ��ԣ�����Ҫ��Ӽ�� */
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

    SIP_GET_STRING(pucStrPtr, pstNode, pucString, pstUbuf, &pstHost->upucAddrStr);

    return SUCCESS;
}

/*******************************************************************************
port           =  1*DIGIT
*******************************************************************************/
ULONG SIP_ParsePort(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                    UCHAR               *pucString,
                    UBUF_HEADER_S       *pstUbuf,
                    UBUF_PTR            *pupStruct)
{
    UCHAR *pucChar = NULL_PTR;
    USHORT usPort = 0;
    USHORT *pusPort = NULL_PTR;

    SIP_GET_COMPONET_PTR(pusPort, USHORT, pstUbuf, pupStruct);

    /* ֱ�Ӽ�������� */
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
                        UBUF_PTR            *pupStruct)
{
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    ULONG ulRet;
    SIP_HEADER_FROM_S *pstFrom = NULL_PTR;
    UBUF_PTR    upStruct = UBUF_NULL_PTR;

    SIP_GET_COMPONET_PTR(pstFrom, SIP_HEADER_FROM_S, pstUbuf, pupStruct);

    pstNode = pstGrammarNode->pstChild;
    while (pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_NAME_ADDR))
        {
            pstFrom->stNameAddr.bName = TRUE;
            upStruct = *pupStruct
                      +(ULONG)&pstFrom->stNameAddr
                      -(ULONG)pstFrom;
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_NAME_ADDR)(pstNode,
                                                                pucString,
                                                                pstUbuf,
                                                               &upStruct);
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_ADDR_SPEC))
        {
            pstFrom->stNameAddr.bName = FALSE;
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_ADDR_SPEC)(pstNode,
                                                                pucString,
                                                                pstUbuf,
                                                               &pstFrom->stNameAddr.upstUri);
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_FROM_PARAM))
        {
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_FROM_PARAM)(pstNode,
                                                                 pucString,
                                                                 pstUbuf,
                                                                 pupStruct);
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
                        UBUF_PTR            *pupStruct)
{
    ABNF_GRAMMAR_NODE_S *pstNode     = NULL_PTR;
    ULONG                ulRet;
    UCHAR               *pucStrPtr   = NULL_PTR;
    SIP_NAME_ADDR_S     *pstNameAddr = NULL_PTR;

    SIP_GET_COMPONET_PTR(pstNameAddr, SIP_NAME_ADDR_S, pstUbuf, pupStruct);

    pstNode = pstGrammarNode->pstChild;
    while (pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_DISPLAY_NAME))
        {
            SIP_GET_STRING(pucStrPtr, pstNode, pucString, pstUbuf, &pstNameAddr->upucName);
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_ADDR_SPEC))
        {
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_ADDR_SPEC)(pstNode,
                                                                pucString,
                                                                pstUbuf,
                                                               &pstNameAddr->upstUri);
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
                        UBUF_PTR            *pupStruct)
{
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    ULONG       ulRet;

    pstNode = pstGrammarNode->pstChild;
    if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_SIP_URI))
    {
        ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_SIP_URI)(pstNode,
                                                          pucString,
                                                          pstUbuf,
                                                          pupStruct);
    }
    else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_SIPS_URI))
    {
        ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_SIPS_URI)(pstNode,
                                                          pucString,
                                                          pstUbuf,
                                                          pupStruct);
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
                         UBUF_PTR            *pupStruct)
{
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    SIP_HEADER_FROM_S   *pstFrom = NULL_PTR;

    SIP_GET_COMPONET_PTR(pstFrom, SIP_HEADER_FROM_S, pstUbuf, pupStruct);

    /* Ŀǰֻ����tag���� */
    pstNode = pstGrammarNode->pstChild;
    if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_TAG_PARAM))
    {
        return SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_TAG_PARAM)(pstNode,
                                                           pucString,
                                                           pstUbuf,
                                                          &pstFrom->upucTag);
    }

    return SUCCESS;
}

/*******************************************************************************
to-param  =  tag-param / generic-param
*******************************************************************************/
ULONG SIP_ParseToParam(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                       UCHAR               *pucString,
                       UBUF_HEADER_S       *pstUbuf,
                       UBUF_PTR            *pupStruct)
{
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    SIP_HEADER_TO_S     *pstTo = NULL_PTR;

    SIP_GET_COMPONET_PTR(pstTo, SIP_HEADER_TO_S, pstUbuf, pupStruct);

    /* Ŀǰֻ����tag���� */
    pstNode = pstGrammarNode->pstChild;
    if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_TAG_PARAM))
    {
        return SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_TAG_PARAM)(pstNode,
                                                           pucString,
                                                           pstUbuf,
                                                          &pstTo->upucTag);
    }

    return SUCCESS;
}

/*******************************************************************************
tag-param   =  "tag" EQUAL token
*******************************************************************************/
ULONG SIP_ParseTagParam(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                       UCHAR               *pucString,
                       UBUF_HEADER_S       *pstUbuf,
                       UBUF_PTR            *pupStruct)
{
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    UCHAR *pucStrPtr = NULL_PTR;

    pstNode = pstGrammarNode->pstChild;
    while (pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_TOKEN))
        {
            SIP_GET_STRING(pucStrPtr, pstNode, pucString, pstUbuf, pupStruct);
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
                       UBUF_PTR            *pupStruct)
{
    UCHAR *pucStrPtr = NULL_PTR;
    SIP_HEADER_CALL_ID_S *pstCallID = NULL_PTR;

    SIP_GET_COMPONET_PTR(pstCallID, SIP_HEADER_CALL_ID_S, pstUbuf, pupStruct);

    /* �ȼ�ʵ�� */
    SIP_GET_STRING(pucStrPtr, pstGrammarNode, pucString, pstUbuf, &pstCallID->upucCallID);

    return SUCCESS;
}

/*******************************************************************************
via-parm          =  sent-protocol LWS sent-by *( SEMI via-params )
*******************************************************************************/
ULONG SIP_ParseViaParm(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                       UCHAR               *pucString,
                       UBUF_HEADER_S       *pstUbuf,
                       UBUF_PTR            *pupStruct)
{
    ULONG ulRet;
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    UBUF_PTR        upStruct = UBUF_NULL_PTR;
    SIP_VIA_PARM_S *pstViaParm = NULL_PTR;

    SIP_GET_COMPONET_PTR(pstViaParm, SIP_VIA_PARM_S, pstUbuf, pupStruct);

    /* ��ʼ����0�ֶ� */
    pstViaParm->ulTtl = NULL_ULONG;

    pstNode = pstGrammarNode->pstChild;
    while (pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_SENT_PROTOCOL))
        {
            upStruct = *pupStruct
                      +(ULONG)&pstViaParm->eProtocolType
                      -(ULONG)pstViaParm;
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_SENT_PROTOCOL)(pstNode,
                                                                    pucString,
                                                                    pstUbuf,
                                                                   &upStruct);
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_SENT_BY))
        {
            upStruct = *pupStruct
                      +(ULONG)&pstViaParm->stSendBy
                      -(ULONG)pstViaParm;
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_SENT_BY)(pstNode,
                                                              pucString,
                                                              pstUbuf,
                                                             &upStruct);
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_VIA_PARMS))
        {
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_VIA_PARMS)(pstNode,
                                                                pucString,
                                                                pstUbuf,
                                                                pupStruct);
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
                            UBUF_PTR            *pupStruct)
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
                                                                pupStruct);
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
                         UBUF_PTR            *pupStruct)
{
    UCHAR      *pucStrPtr = NULL_PTR;
    SIP_TRANSPORT_PROTOCOL_E *peProtocol = NULL_PTR;

    SIP_GET_COMPONET_PTR(peProtocol, SIP_TRANSPORT_PROTOCOL_E, pstUbuf, pupStruct);

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
                      UBUF_PTR            *pupStruct)
{
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    ULONG ulRet;
    UBUF_PTR    upStruct = UBUF_NULL_PTR;
    URI_HOST_PORT_S     *pstSentBy = NULL_PTR;

    SIP_GET_COMPONET_PTR(pstSentBy, URI_HOST_PORT_S, pstUbuf, pupStruct);

    /* ��ʼ����0�ֶ� */
    pstSentBy->usPort = NULL_USHORT;

    pstNode = pstGrammarNode->pstChild;
    while (pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_HOST))
        {
            upStruct = *pupStruct
                      +(ULONG)&pstSentBy->stHost
                      -(ULONG)pstSentBy;
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_HOST)(pstNode,
                                                           pucString,
                                                           pstUbuf,
                                                          &upStruct);
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_PORT))
        {
            upStruct = *pupStruct
                      +(ULONG)&pstSentBy->usPort
                      -(ULONG)pstSentBy;
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_HOST)(pstNode,
                                                           pucString,
                                                           pstUbuf,
                                                          &upStruct);
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
                         UBUF_PTR            *pupStruct)
{
    ULONG ulRet = SUCCESS;
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    URI_HOST_S          *pstHost = NULL_PTR;
    SIP_VIA_RECEIVED_S  *pstViaReceived = NULL_PTR;
    UBUF_PTR    upStruct = UBUF_NULL_PTR;
    SIP_VIA_PARM_S *pstViaParm = NULL_PTR;

    SIP_GET_COMPONET_PTR(pstViaParm, SIP_VIA_PARM_S, pstUbuf, pupStruct);

    pstNode = pstGrammarNode->pstChild;
    while (pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_VIA_TTL))
        {
            upStruct = *pupStruct
                      +(ULONG)&pstViaParm->ulTtl
                      -(ULONG)pstViaParm;
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_VIA_TTL)(pstNode,
                                                              pucString,
                                                              pstUbuf,
                                                             &upStruct);
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_VIA_MADDR))
        {
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_VIA_MADDR)(pstNode,
                                                                pucString,
                                                                pstUbuf,
                                                               &pstViaParm->upstMaddr);
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_VIA_RECEIVED))
        {
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_VIA_RECEIVED)(pstNode,
                                                                   pucString,
                                                                   pstUbuf,
                                                                  &pstViaParm->upstReceived);
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_VIA_BRANCH))
        {
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_VIA_BRANCH)(pstNode,
                                                                 pucString,
                                                                 pstUbuf,
                                                                 pupStruct);
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
                      UBUF_PTR            *pupStruct)
{
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    UCHAR *pucChar = NULL_PTR;
    ULONG ulTtl;
    ULONG *pulTtl = NULL_PTR;

    SIP_GET_COMPONET_PTR(pulTtl, ULONG, pstUbuf, pupStruct);

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
                        UBUF_PTR            *pupStruct)
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
                                                           pupStruct);
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
                           UBUF_PTR            *pupStruct)
{
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    UCHAR *pucStrPtr = NULL_PTR;
    SIP_VIA_RECEIVED_S  *pstViaReceived = NULL_PTR;

    SIP_GET_COMPONET_PTR(pstViaReceived, SIP_VIA_RECEIVED_S, pstUbuf, pupStruct);

    pstNode = pstGrammarNode->pstChild;
    while (pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_IPV4_ADDRESS))
        {
            pstViaReceived->eIpType = SIP_IP_TYPE_IPV4;
            SIP_GET_STRING(pucStrPtr, pstNode, pucString, pstUbuf, &pstViaReceived->u.upucIPV4);
            break;
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_IPV6_ADDRESS))
        {
            pstViaReceived->eIpType = SIP_IP_TYPE_IPV6;
            SIP_GET_STRING(pucStrPtr, pstNode, pucString, pstUbuf, &pstViaReceived->u.upucIPV6);
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
                         UBUF_PTR            *pupStruct)
{
    ULONG ulRet = SUCCESS;
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    UCHAR *pucStrPtr = NULL_PTR;
    SIP_VIA_PARM_S *pstViaParm = NULL_PTR;

    SIP_GET_COMPONET_PTR(pstViaParm, SIP_VIA_PARM_S, pstUbuf, pupStruct);

    pstNode = pstGrammarNode->pstChild;
    while (pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_TOKEN))
        {
            SIP_GET_STRING(pucStrPtr, pstNode, pucString, pstUbuf, &pstViaParm->upucBranch);
            break;
        }

        pstNode = pstNode->pstNextNode;
    }

    return ulRet;
}

/* ����������ͷ��ı��뺯�� */
/*******************************************************************************
Accept         =  "Accept" HCOLON
                   [ accept-range *(COMMA accept-range) ]
*******************************************************************************/
ULONG SIP_ParseHeaderAccept(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                            UCHAR               *pucString,
                            UBUF_HEADER_S       *pstUbuf,
                            UBUF_PTR            *pupStruct)
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
                                    UBUF_PTR            *pupStruct)
{
    return SUCCESS;
}

/*******************************************************************************
Call-ID  =  ( "Call-ID" / "i" ) HCOLON callid
*******************************************************************************/
ULONG SIP_ParseHeaderCallID(ABNF_GRAMMAR_NODE_S *pstGrammarNode,
                            UCHAR               *pucString,
                            UBUF_HEADER_S       *pstUbuf,
                            UBUF_PTR            *pupStruct)
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
                                                             pupStruct);
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
                          UBUF_PTR            *pupStruct)
{
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    SIP_HEADER_CSEQ_S *pstSeq = NULL_PTR;
    UCHAR *pucChar = NULL_PTR;
    ULONG ulRet;
    UBUF_PTR    upStruct = UBUF_NULL_PTR;

    SIP_GET_COMPONET_PTR(pstSeq, SIP_HEADER_CSEQ_S, pstUbuf, pupStruct);

    /* ֱ�ӻ�ȡ���ֵ����������"CSeq" */
    pucChar = SIP_GET_OFFSET_POINT(pucString, pstGrammarNode->ulOffset+ strlen("CSeq"));
    while ((*pucChar < '0')||(*pucChar > '9'))
    {
        pucChar++;
    }

    SIP_GET_DEC_NUM(pstSeq->ulSeq, pucChar);

    /* ��ȡ���� */
    pstNode = pstGrammarNode->pstChild;
    while (pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_METHOD))
        {
            upStruct = *pupStruct
                      +(ULONG)&pstSeq->eMethod
                      -(ULONG)pstSeq;
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_METHOD)(pstNode,
                                                             pucString,
                                                             pstUbuf,
                                                            &upStruct);
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
                          UBUF_PTR            *pupStruct)
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
                                                                pupStruct);
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
                        UBUF_PTR            *pupStruct)
{
    ULONG ulRet;
    ABNF_GRAMMAR_NODE_S *pstNode = NULL_PTR;
    SIP_HEADER_TO_S     *pstTo   = NULL_PTR;
    UBUF_PTR    upStruct = UBUF_NULL_PTR;

    SIP_GET_COMPONET_PTR(pstTo, SIP_HEADER_TO_S, pstUbuf, pupStruct);

    pstNode = pstGrammarNode->pstChild;
    while (pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_NAME_ADDR))
        {
            pstTo->stNameAddr.bName = TRUE;
            upStruct = *pupStruct
                      +(ULONG)&pstTo->stNameAddr
                      -(ULONG)pstTo;
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_NAME_ADDR)(pstNode,
                                                                pucString,
                                                                pstUbuf,
                                                               &upStruct);
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_ADDR_SPEC))
        {
            pstTo->stNameAddr.bName = FALSE;
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_ADDR_SPEC)(pstNode,
                                                                pucString,
                                                                pstUbuf,
                                                               &pstTo->stNameAddr.upstUri);
        }
        else if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_TO_PARAM))
        {
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_ADDR_SPEC)(pstNode,
                                                                pucString,
                                                                pstUbuf,
                                                                pupStruct);
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
                         UBUF_PTR            *pupStruct)
{
    ULONG ulRet;
    ABNF_GRAMMAR_NODE_S *pstNode    = NULL_PTR;
    SIP_HEADER_VIA_S    *pstVia     = NULL_PTR;
    UBUF_PTR            *pupstViaParm = NULL_PTR;
    SIP_VIA_PARM_S      *pstViaParm = NULL_PTR;

    SIP_GET_COMPONET_PTR(pstVia, SIP_HEADER_VIA_S, pstUbuf, pupStruct);

    pupstViaParm = &pstVia->upstViaParm;
    pstNode = pstGrammarNode->pstChild;
    while (pstNode != NULL_PTR)
    {
        if (SIP_RULE_MATCH(pstNode, SIP_ABNF_RULE_VIA_PARM))
        {
            ulRet = SIP_GET_PARSE_FUNC(SIP_ABNF_RULE_VIA_PARM)(pstNode,
                                                               pucString,
                                                               pstUbuf,
                                                               pupstViaParm);
            if (ulRet != SUCCESS)
            {
                return FAIL;
            }
            else
            {
                pupstViaParm = &pstViaParm->upstNext;
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
                                 UBUF_PTR            *pupStruct)
{
    SIP_HEADER_MAX_FORWARDS_S *pstMaxForwards = NULL_PTR;
    UCHAR *pucChar = NULL_PTR;

    SIP_GET_COMPONET_PTR(pstMaxForwards, SIP_HEADER_MAX_FORWARDS_S, pstUbuf, pupStruct);

    /* ֱ�ӻ�ȡ���֣���������"Max-Forwards" */
    pucChar = SIP_GET_OFFSET_POINT(pucString, pstGrammarNode->ulOffset + strlen("Max-Forwards"));
    while ((*pucChar < '0')||(*pucChar > '9'))
    {
        pucChar++;
    }

    SIP_GET_DEC_NUM(pstMaxForwards->ulMaxForwards, pucChar);

    return SUCCESS;
}
