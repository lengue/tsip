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
    SIP_MSG_S *pstSipMsg = NULL_PTR;

    pstSipMsg = (SIP_MSG_S *)pSrcStruct;
    if (pstSipMsg->eMsgType == SIP_MSG_TYPE_REQUEST)
    {
        return SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_REQUEST)(pSrcStruct,
                                                         pstDstUbufMsg,
                                                         ppDstStruct);
    }
    else if (pstSipMsg->eMsgType == SIP_MSG_TYPE_RESPONSE)
    {
        return SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_RESPONSE)(pSrcStruct,
                                                          pstDstUbufMsg,
                                                          ppDstStruct);
    }

    return FAIL;
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
    ULONG ulRet;
    SIP_MSG_S *pstSrcSipMsg = NULL_PTR;
    SIP_MSG_S *pstDstSipMsg = NULL_PTR;
    void      *pstDstStruct = NULL_PTR;

    /* 获取原结构和目标结构指针 */
    pstSrcSipMsg = (SIP_MSG_S *)pSrcStruct;
    SIP_GET_COMPONET_PTR(pstDstSipMsg, SIP_MSG_S, pstDstUbufMsg, ppDstStruct);

    /* 如果目标是一个结构，需要使用临时指针用于参数传入 */
    pstDstStruct = &pstDstSipMsg->uStartLine.stRequstLine;
    ulRet = SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_REQUEST_LINE)(&pstSrcSipMsg->uStartLine.stRequstLine,
                                                            pstDstUbufMsg,
                                                           &pstDstStruct);
    if (ulRet!= SUCCESS)
    {
        return ulRet;
    }

    ulRet = SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_MESSAGE_HEADER)(pstSrcSipMsg,
                                                             pstDstUbufMsg,
                                                            &pstDstSipMsg);
    if (ulRet!= SUCCESS)
    {
        return ulRet;
    }

    ulRet = SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_MESSAGE_BODY)(pstSrcSipMsg,
                                                           pstDstUbufMsg,
                                                          &pstDstSipMsg);
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
ULONG SIP_CloneResponse(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    ULONG ulRet;
    SIP_MSG_S *pstSrcSipMsg = NULL_PTR;
    SIP_MSG_S *pstDstSipMsg = NULL_PTR;
    void      *pstDstStruct = NULL_PTR;

    pstSrcSipMsg = (SIP_MSG_S *)pSrcStruct;
    SIP_GET_COMPONET_PTR(pstDstSipMsg, SIP_MSG_S, pstDstUbufMsg, ppDstStruct);

    pstDstStruct = &pstDstSipMsg->uStartLine.stStatusLine;
    ulRet = SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_STATUS_LINE)(&pstSrcSipMsg->uStartLine.stStatusLine,
                                                            pstDstUbufMsg,
                                                           &pstDstStruct);
    if (ulRet!= SUCCESS)
    {
        return ulRet;
    }

    ulRet = SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_MESSAGE_HEADER)(pstSrcSipMsg,
                                                             pstDstUbufMsg,
                                                            &pstDstSipMsg);
    if (ulRet!= SUCCESS)
    {
        return ulRet;
    }

    ulRet = SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_MESSAGE_BODY)(pstSrcSipMsg,
                                                           pstDstUbufMsg,
                                                          &pstDstSipMsg);
    if (ulRet!= SUCCESS)
    {
        return ulRet;
    }

    return SUCCESS;
}

/*******************************************************************************
Request-Line   =  Method SP Request-URI SP SIP-Version CRLF
*******************************************************************************/
ULONG SIP_CloneRequestLine(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    ULONG ulRet;
    SIP_REQUEST_LINE_S *pstSrcRequestLine = NULL_PTR;
    SIP_REQUEST_LINE_S *pstDstRequestLine = NULL_PTR;
    void               *pstDstStruct      = NULL_PTR;

    pstSrcRequestLine = (SIP_REQUEST_LINE_S *)pSrcStruct;
    SIP_GET_COMPONET_PTR(pstDstRequestLine, SIP_REQUEST_LINE_S, pstDstUbufMsg, ppDstStruct);

    pstDstStruct = &pstDstRequestLine->eMethod;
    ulRet = SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_METHOD)(&pstSrcRequestLine->eMethod,
                                                      pstDstUbufMsg,
                                                     &pstDstStruct);
    if (ulRet!= SUCCESS)
    {
        return ulRet;
    }

    ulRet = SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_REQUEST_URI)(pstSrcRequestLine->pstRequestURI,
                                                          pstDstUbufMsg,
                                                         &pstDstRequestLine->pstRequestURI);
    if (ulRet!= SUCCESS)
    {
        return ulRet;
    }

    pstDstStruct = &pstDstRequestLine->ucVersion;
    ulRet = SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_SIP_VERSION)(&pstSrcRequestLine->ucVersion,
                                                           pstDstUbufMsg,
                                                          &pstDstStruct);
    if (ulRet!= SUCCESS)
    {
        return ulRet;
    }

    return SUCCESS;
}

/*******************************************************************************
Status-Line     =  SIP-Version SP Status-Code SP Reason-Phrase CRLF
*******************************************************************************/
ULONG SIP_CloneStatusLine(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    ULONG ulRet;
    SIP_STATUS_LINE_S *pstSrcStateLine = NULL_PTR;
    SIP_STATUS_LINE_S *pstDstStateLine = NULL_PTR;
    void              *pstDstStruct    = NULL_PTR;

    pstSrcStateLine = (SIP_STATUS_LINE_S *)pSrcStruct;
    SIP_GET_COMPONET_PTR(pstDstStateLine, SIP_STATUS_LINE_S, pstDstUbufMsg, ppDstStruct);

    pstDstStruct = &pstDstStateLine->ucVersion;
    ulRet = SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_SIP_VERSION)(&pstSrcStateLine->ucVersion,
                                                           pstDstUbufMsg,
                                                          &pstDstStruct);
    if (ulRet!= SUCCESS)
    {
        return ulRet;
    }

    pstDstStruct = &pstDstStateLine->ucVersion;
    ulRet = SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_STATUS_CODE)(&pstSrcStateLine->eStatusCode,
                                                           pstDstUbufMsg,
                                                          &pstDstStruct);
    if (ulRet!= SUCCESS)
    {
        return ulRet;
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
ULONG SIP_CloneMessageHeader(void          *pSrcStruct,
                             UBUF_HEADER_S *pstDstUbufMsg,
                             void         **ppDstStruct)
{
    ULONG ulRet;
    ULONG ulLoop;
    ULONG ulAppIndex;
    SIP_MSG_S    *pstSrcSipMsg  = NULL_PTR;
    SIP_MSG_S    *pstDstSipMsg  = NULL_PTR;
    SIP_HEADER_S *pstSrcHeader  = NULL_PTR;
    SIP_HEADER_S *pstDstHeader  = NULL_PTR;

    pstSrcSipMsg = (SIP_MSG_S *)pSrcStruct;
    SIP_GET_COMPONET_PTR(pstDstSipMsg, SIP_MSG_S, pstDstUbufMsg, ppDstStruct);

    for (ulLoop = 0; ulLoop < SIP_HEADER_BUTT; ulLoop++)
    {
        ulAppIndex = SIP_GET_HEADER_RULE_INDEX(ulLoop);
        if (SIP_GET_CLONE_FUNC(ulAppIndex)== NULL_PTR)
        {
            /* 没有克隆函数不处理 */
            continue;
        }

        /* 可能存在多个相同头域，挨个编码 */
        pstSrcHeader = pstSrcSipMsg->apstHeaders[ulLoop];
        pstDstHeader = pstDstSipMsg->apstHeaders[ulLoop];
        while (pstSrcHeader != NULL_PTR)
        {
            ulRet = SIP_GET_CLONE_FUNC(ulAppIndex)(pstSrcHeader,
                                                   pstDstUbufMsg,
                                                  &pstDstHeader);
            if (ulRet != SUCCESS)
            {
                return FAIL;
            }

            pstSrcHeader = pstSrcHeader->pstNext;
            pstDstHeader = pstDstHeader->pstNext;
        }
    }

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
    SIP_METHOD_E *peSrcMethod = NULL_PTR;
    SIP_METHOD_E *peDstMethod = NULL_PTR;

    peSrcMethod = (SIP_METHOD_E *)pSrcStruct;
    SIP_GET_COMPONET_PTR(peDstMethod, SIP_METHOD_E, pstDstUbufMsg, ppDstStruct);

    *peDstMethod = *peSrcMethod;

    return SUCCESS;
}

/*******************************************************************************
Request-URI    =  SIP-URI / SIPS-URI / absoluteURI
*******************************************************************************/
ULONG SIP_CloneRequestURI(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    URI_S *pstSrcRequestURI = NULL_PTR;
    URI_S *pstDstRequestURI = NULL_PTR;
    void  *pstDstStruct     = NULL_PTR;

    pstSrcRequestURI = (URI_S *)pSrcStruct;
    SIP_GET_COMPONET_PTR(pstDstRequestURI, URI_S, pstDstUbufMsg, ppDstStruct);

    if (pstSrcRequestURI->eUriType == URI_TYPE_SIP)
    {
        pstDstRequestURI->eUriType = URI_TYPE_SIP;
        pstDstStruct = &pstDstRequestURI->u.stSipUri;
        return SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_SIP_URI)(&pstSrcRequestURI->u.stSipUri,
                                                         pstDstUbufMsg,
                                                        &pstDstStruct);
    }
    else if (pstSrcRequestURI->eUriType == URI_TYPE_SIPS)
    {
        pstDstRequestURI->eUriType = URI_TYPE_SIPS;
        pstDstStruct = &pstDstRequestURI->u.stSipUri;
        return SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_SIPS_URI)(&pstSrcRequestURI->u.stSipUri,
                                                           pstDstUbufMsg,
                                                          &pstDstStruct);
    }

    return FAIL;
}

/*******************************************************************************
SIP-Version    =  "SIP" "/" 1*DIGIT "." 1*DIGIT
*******************************************************************************/
ULONG SIP_CloneSIPVersion(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    UCHAR *pucSrcVersion = NULL_PTR;
    UCHAR *pucDstVersion = NULL_PTR;

    pucSrcVersion = (UCHAR *)pSrcStruct;
    SIP_GET_COMPONET_PTR(pucDstVersion, UCHAR, pstDstUbufMsg, ppDstStruct);

    *pucDstVersion = *pucSrcVersion;

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
    ULONG ulRet;
    SIP_URI_S *pstSrcSipURI = NULL_PTR;
    SIP_URI_S *pstDstSipURI = NULL_PTR;
    void      *pstDstStruct = NULL_PTR;

    pstSrcSipURI = (SIP_URI_S *)pSrcStruct;
    SIP_GET_COMPONET_PTR(pstDstSipURI, SIP_URI_S, pstDstUbufMsg, ppDstStruct);

    if (pstSrcSipURI->pstUserInfo != NULL_PTR)
    {
        ulRet= SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_USERINFO)(pstSrcSipURI->pstUserInfo,
                                                          pstDstUbufMsg,
                                                         &pstDstSipURI->pstUserInfo);
        if (ulRet!= SUCCESS)
        {
            return ulRet;
        }
    }

    pstDstStruct = &pstDstSipURI->stHostPort;
    ulRet= SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_HOSTPORT)(&pstSrcSipURI->stHostPort,
                                                       pstDstUbufMsg,
                                                      &pstDstStruct);
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
ULONG SIP_CloneSipsURI(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    ULONG ulRet;
    SIP_URI_S *pstSrcSipURI = NULL_PTR;
    SIP_URI_S *pstDstSipURI = NULL_PTR;
    void      *pstDstStruct = NULL_PTR;

    pstSrcSipURI = (SIP_URI_S *)pSrcStruct;
    SIP_GET_COMPONET_PTR(pstDstSipURI, SIP_URI_S, pstDstUbufMsg, ppDstStruct);

    if (pstSrcSipURI->pstUserInfo != NULL_PTR)
    {
        ulRet= SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_USERINFO)(pstSrcSipURI->pstUserInfo,
                                                          pstDstUbufMsg,
                                                         &pstDstSipURI->pstUserInfo);
        if (ulRet!= SUCCESS)
        {
            return ulRet;
        }
    }

    pstDstStruct = &pstDstSipURI->stHostPort;
    ulRet= SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_HOSTPORT)(&pstSrcSipURI->stHostPort,
                                                       pstDstUbufMsg,
                                                      &pstDstStruct);
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
ULONG SIP_CloneUserinfo(void          *pSrcStruct,
                        UBUF_HEADER_S *pstDstUbufMsg,
                        void         **ppDstStruct)
{
    URI_USER_INFO_S *pstSrcUserInfo = NULL_PTR;
    URI_USER_INFO_S *pstDstUserInfo = NULL_PTR;

    pstSrcUserInfo = (URI_USER_INFO_S *)pSrcStruct;
    SIP_GET_COMPONET_PTR(pstDstUserInfo, URI_USER_INFO_S, pstDstUbufMsg, ppDstStruct);

    SIP_CLONE_STRING(pstDstUbufMsg,
                     pstDstUserInfo->pucUserInfo,
                     pstSrcUserInfo->pucUserInfo);

    if (pstSrcUserInfo->pucPassword != NULL_PTR)
    {
        SIP_CLONE_STRING(pstDstUbufMsg, 
                         pstDstUserInfo->pucPassword,
                         pstSrcUserInfo->pucPassword);
    }

    return SUCCESS;
}

/*******************************************************************************
hostport         =  host [ ":" port ]
*******************************************************************************/
ULONG SIP_CloneHostport(void          *pSrcStruct,
                        UBUF_HEADER_S *pstDstUbufMsg,
                        void         **ppDstStruct)
{
    ULONG ulRet;
    URI_HOST_PORT_S *pstSrcHostPort = NULL_PTR;
    URI_HOST_PORT_S *pstDstHostPort = NULL_PTR;
    void            *pstDstStruct   = NULL_PTR;

    pstSrcHostPort = (URI_HOST_PORT_S *)pSrcStruct;
    SIP_GET_COMPONET_PTR(pstDstHostPort, URI_HOST_PORT_S, pstDstUbufMsg, ppDstStruct);

    pstDstStruct = &pstDstHostPort->stHost;
    ulRet = SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_HOST)(&pstSrcHostPort->stHost,
                                                    pstDstUbufMsg,
                                                   &pstDstStruct);
    if (ulRet!= SUCCESS)
    {
        return ulRet;
    }

    pstDstHostPort->usPort = pstSrcHostPort->usPort;

    return SUCCESS;
}

/*******************************************************************************
host             =  hostname / IPv4address / IPv6reference
*******************************************************************************/
ULONG SIP_CloneHost(void          *pSrcStruct,
                    UBUF_HEADER_S *pstDstUbufMsg,
                    void         **ppDstStruct)
{
    URI_HOST_S *pstSrcHost = NULL_PTR;
    URI_HOST_S *pstDstHost = NULL_PTR;

    pstSrcHost = (URI_HOST_S *)pSrcStruct;
    SIP_GET_COMPONET_PTR(pstDstHost, URI_HOST_S, pstDstUbufMsg, ppDstStruct);
    if(pstSrcHost->pucAddrStr == NULL_PTR)
    {
        return SUCCESS;
    }
    
    pstDstHost->eHostType = pstSrcHost->eHostType;
    SIP_CLONE_STRING(pstDstUbufMsg, 
                     pstDstHost->pucAddrStr,
                     pstSrcHost->pucAddrStr);

    return SUCCESS;
}

/*******************************************************************************
port           =  1*DIGIT
*******************************************************************************/
ULONG SIP_ClonePort(void          *pSrcStruct,
                    UBUF_HEADER_S *pstDstUbufMsg,
                    void         **ppDstStruct)
{
    USHORT *pusSrcPort = NULL_PTR;
    USHORT *pusDstPort = NULL_PTR;

    pusSrcPort = (USHORT *)pSrcStruct;
    SIP_GET_COMPONET_PTR(pusDstPort, USHORT, pstDstUbufMsg, ppDstStruct);

    *pusDstPort = *pusSrcPort;

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
    SIP_STATUS_CODE_E *peSrcStatusCode = NULL_PTR;
    SIP_STATUS_CODE_E *peDstStatusCode = NULL_PTR;

    peSrcStatusCode = (SIP_STATUS_CODE_E *)pSrcStruct;
    SIP_GET_COMPONET_PTR(peDstStatusCode, SIP_STATUS_CODE_E, pstDstUbufMsg, ppDstStruct);

    *peDstStatusCode = *peSrcStatusCode;

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
    ULONG ulRet;
    SIP_HEADER_FROM_S *pstSrcFrom = NULL_PTR;
    SIP_HEADER_FROM_S *pstDstFrom = NULL_PTR;
    void              *pstDstStruct = NULL_PTR;

    pstSrcFrom = (SIP_HEADER_FROM_S *)pSrcStruct;
    SIP_GET_COMPONET_PTR(pstDstFrom, SIP_HEADER_FROM_S, pstDstUbufMsg, ppDstStruct);

    if (pstSrcFrom->stNameAddr.bName == TRUE)
    {
        pstDstFrom->stNameAddr.bName = TRUE;
        pstDstStruct = &pstDstFrom->stNameAddr;
        ulRet = SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_NAME_ADDR)(&pstSrcFrom->stNameAddr,
                                                             pstDstUbufMsg,
                                                            &pstDstStruct);
    }
    else
    {
        pstDstFrom->stNameAddr.bName = FALSE;
        ulRet = SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_ADDR_SPEC)(pstSrcFrom->stNameAddr.pstUri,
                                                            pstDstUbufMsg,
                                                           &pstDstFrom->stNameAddr.pstUri);
    }

    if (ulRet!= SUCCESS)
    {
        return ulRet;
    }

    ulRet = SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_FROM_PARAM)(pstSrcFrom,
                                                         pstDstUbufMsg,
                                                        &pstDstFrom);
    if (ulRet!= SUCCESS)
    {
        return ulRet;
    }

    return SUCCESS;
}

/*******************************************************************************
from-param  =  tag-param / generic-param
*******************************************************************************/
ULONG SIP_CloneFromParam(void          *pSrcStruct,
                         UBUF_HEADER_S *pstDstUbufMsg,
                         void         **ppDstStruct)
{
    SIP_HEADER_FROM_S *pstSrcFrom = NULL_PTR;
    SIP_HEADER_FROM_S *pstDstFrom = NULL_PTR;

    pstSrcFrom = (SIP_HEADER_FROM_S *)pSrcStruct;
    SIP_GET_COMPONET_PTR(pstDstFrom, SIP_HEADER_FROM_S, pstDstUbufMsg, ppDstStruct);

    if (pstSrcFrom->pucTag != NULL_PTR)
    {
        SIP_CLONE_STRING(pstDstUbufMsg,
                         pstDstFrom->pucTag,
                         pstSrcFrom->pucTag);
    }

    return SUCCESS;
}

/*******************************************************************************
to-param  =  tag-param / generic-param
*******************************************************************************/
ULONG SIP_CloneToParam(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    SIP_HEADER_TO_S *pstSrcTo = NULL_PTR;
    SIP_HEADER_TO_S *pstDstTo = NULL_PTR;

    pstSrcTo = (SIP_HEADER_TO_S *)pSrcStruct;
    SIP_GET_COMPONET_PTR(pstDstTo, SIP_HEADER_TO_S, pstDstUbufMsg, ppDstStruct);

    if (pstSrcTo->pucTag != NULL_PTR)
    {
        SIP_CLONE_STRING(pstDstUbufMsg,
                         pstDstTo->pucTag,
                         pstSrcTo->pucTag);
    }

    return SUCCESS;
}

/*******************************************************************************
via-parm          =  sent-protocol LWS sent-by *( SEMI via-params )
*******************************************************************************/
ULONG SIP_CloneViaParm(void          *pSrcStruct,
                       UBUF_HEADER_S *pstDstUbufMsg,
                       void         **ppDstStruct)
{
    ULONG           ulRet;
    SIP_VIA_PARM_S *pstSrcViaParm = NULL_PTR;
    SIP_VIA_PARM_S *pstDstViaParm = NULL_PTR;
    void           *pstDstStruct  = NULL_PTR;

    pstSrcViaParm = (SIP_VIA_PARM_S *)pSrcStruct;
    SIP_GET_COMPONET_PTR(pstDstViaParm, SIP_VIA_PARM_S, pstDstUbufMsg, ppDstStruct);

    pstDstStruct = &pstDstViaParm->eProtocolType;
    ulRet = SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_SENT_PROTOCOL)(&pstSrcViaParm->eProtocolType,
                                                             pstDstUbufMsg,
                                                            &pstDstStruct);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }

    pstDstStruct = &pstDstViaParm->stSendBy;
    ulRet = SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_SENT_BY)(&pstSrcViaParm->stSendBy,
                                                       pstDstUbufMsg,
                                                      &pstDstStruct);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }

    ulRet = SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_VIA_PARMS)(pstSrcViaParm,
                                                        pstDstUbufMsg,
                                                       &pstDstViaParm);
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
ULONG SIP_CloneSentProtocol(void          *pSrcStruct,
                            UBUF_HEADER_S *pstDstUbufMsg,
                            void         **ppDstStruct)
{
    SIP_TRANSPORT_PROTOCOL_E  *peSrcProtocolType = NULL_PTR;
    SIP_TRANSPORT_PROTOCOL_E  *peDstProtocolType = NULL_PTR;

    peSrcProtocolType = (SIP_TRANSPORT_PROTOCOL_E *)pSrcStruct;
    SIP_GET_COMPONET_PTR(peDstProtocolType, SIP_TRANSPORT_PROTOCOL_E, pstDstUbufMsg, ppDstStruct);

    return SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_TRANSPORT)(peSrcProtocolType,
                                                      pstDstUbufMsg,
                                                     &peDstProtocolType);
}

/*******************************************************************************
transport         =  "UDP" / "TCP" / "TLS" / "SCTP"
                     / other-transport
*******************************************************************************/
ULONG SIP_CloneTransport(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    SIP_TRANSPORT_PROTOCOL_E  *peSrcProtocolType = NULL_PTR;
    SIP_TRANSPORT_PROTOCOL_E  *peDstProtocolType = NULL_PTR;

    peSrcProtocolType = (SIP_TRANSPORT_PROTOCOL_E *)pSrcStruct;
    SIP_GET_COMPONET_PTR(peDstProtocolType, SIP_TRANSPORT_PROTOCOL_E, pstDstUbufMsg, ppDstStruct);

    *peDstProtocolType = *peSrcProtocolType;
    return SUCCESS;
}

/*******************************************************************************
sent-by           =  host [ COLON port ]
*******************************************************************************/
ULONG SIP_CloneSentBy(void          *pSrcStruct,
                      UBUF_HEADER_S *pstDstUbufMsg,
                      void         **ppDstStruct)
{
    ULONG ulRet;
    URI_HOST_PORT_S *pstSrcSendBy = NULL_PTR;
    URI_HOST_PORT_S *pstDstSendBy = NULL_PTR;
    void            *pstDstStruct = NULL_PTR;

    pstSrcSendBy = (URI_HOST_PORT_S *)pSrcStruct;
    SIP_GET_COMPONET_PTR(pstDstSendBy, URI_HOST_PORT_S, pstDstUbufMsg, ppDstStruct);

    pstDstStruct = &pstDstSendBy->stHost;
    ulRet = SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_HOST)(&pstSrcSendBy->stHost,
                                                    pstDstUbufMsg,
                                                   &pstDstStruct);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }

    if (pstSrcSendBy->usPort != NULL_USHORT)
    {
        pstDstStruct = &pstDstSendBy->usPort;
        ulRet = SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_PORT)(&pstSrcSendBy->usPort,
                                                        pstDstUbufMsg,
                                                       &pstDstStruct);
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
ULONG SIP_CloneViaParams(void          *pSrcStruct,
                         UBUF_HEADER_S *pstDstUbufMsg,
                         void         **ppDstStruct)
{
    ULONG ulRet;
    SIP_VIA_PARM_S *pstSrcViaParm = NULL_PTR;
    SIP_VIA_PARM_S *pstDstViaParm = NULL_PTR;
    void           *pstDstStruct  = NULL_PTR;

    pstSrcViaParm = (SIP_VIA_PARM_S *)pSrcStruct;
    SIP_GET_COMPONET_PTR(pstDstViaParm, SIP_VIA_PARM_S, pstDstUbufMsg, ppDstStruct);

    if (pstSrcViaParm->ulTtl != NULL_ULONG)
    {
        pstDstStruct = &pstDstViaParm->ulTtl;
        ulRet = SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_VIA_TTL)(&pstSrcViaParm->ulTtl,
                                                           pstDstUbufMsg,
                                                          &pstDstStruct);
        if (ulRet!= SUCCESS)
        {
            return FAIL;
        }
    }

    if (pstSrcViaParm->pucBranch != NULL_PTR)
    {
        SIP_CLONE_STRING(pstDstUbufMsg,
                         pstDstViaParm->pucBranch,
                         pstSrcViaParm->pucBranch);
    }

    if (pstSrcViaParm->pstMaddr != NULL_PTR)
    {
        ulRet = SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_VIA_MADDR)(pstSrcViaParm->pstMaddr,
                                                            pstDstUbufMsg,
                                                           &pstDstViaParm->pstMaddr);
        if (ulRet!= SUCCESS)
        {
            return FAIL;
        }
    }

    if (pstSrcViaParm->pstReceived != NULL_PTR)
    {
        ulRet = SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_VIA_RECEIVED)(pstSrcViaParm->pstReceived,
                                                               pstDstUbufMsg,
                                                              &pstDstViaParm->pstReceived);
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
ULONG SIP_CloneViaTtl(void          *pSrcStruct,
                      UBUF_HEADER_S *pstDstUbufMsg,
                      void         **ppDstStruct)
{
    ULONG *pulSrcTtl = NULL_PTR;
    ULONG *pulDstTtl = NULL_PTR;

    pulSrcTtl = (ULONG *)pSrcStruct;
    SIP_GET_COMPONET_PTR(pulDstTtl, ULONG, pstDstUbufMsg, ppDstStruct);

    *pulDstTtl = *pulSrcTtl;

    return SUCCESS;
}

/*******************************************************************************
via-maddr         =  "maddr" EQUAL host
*******************************************************************************/
ULONG SIP_CloneViaMaddr(void          *pSrcStruct,
                        UBUF_HEADER_S *pstDstUbufMsg,
                        void         **ppDstStruct)
{
    URI_HOST_S *pstSrcMaddr = NULL_PTR;
    URI_HOST_S *pstDstMaddr = NULL_PTR;

    pstSrcMaddr = (URI_HOST_S *)pSrcStruct;
    SIP_GET_COMPONET_PTR(pstDstMaddr, URI_HOST_S, pstDstUbufMsg, ppDstStruct);

    return SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_HOST)(pstSrcMaddr,
                                                  pstDstUbufMsg,
                                                 &pstDstMaddr);
}

/*******************************************************************************
via-received      =  "received" EQUAL (IPv4address / IPv6address)
*******************************************************************************/
ULONG SIP_CloneViaReceived(void          *pSrcStruct,
                           UBUF_HEADER_S *pstDstUbufMsg,
                           void         **ppDstStruct)
{
    UCHAR *pucString = NULL_PTR;
    SIP_VIA_RECEIVED_S *pstSrcReceived = NULL_PTR;
    SIP_VIA_RECEIVED_S *pstDstReceived = NULL_PTR;

    pstSrcReceived = (SIP_VIA_RECEIVED_S *)pSrcStruct;
    SIP_GET_COMPONET_PTR(pstDstReceived, SIP_VIA_RECEIVED_S, pstDstUbufMsg, ppDstStruct);

    if (pstSrcReceived->eIpType == SIP_IP_TYPE_IPV4)
    {
        pstDstReceived->eIpType = SIP_IP_TYPE_IPV4;
        SIP_CLONE_STRING(pstDstUbufMsg,
                         pstDstReceived->u.pucIPV4,
                         pstSrcReceived->u.pucIPV4);
    }
    else
    {
        pstDstReceived->eIpType = SIP_IP_TYPE_IPV6;
        SIP_CLONE_STRING(pstDstUbufMsg,
                         pstDstReceived->u.pucIPV6,
                         pstSrcReceived->u.pucIPV6);
    }

    return SUCCESS;
}

/*******************************************************************************
name-addr      =  [ display-name ] LAQUOT addr-spec RAQUOT
*******************************************************************************/
ULONG SIP_CloneNameAddr(void          *pSrcStruct,
                        UBUF_HEADER_S *pstDstUbufMsg,
                        void         **ppDstStruct)
{
    ULONG ulRet;
    SIP_NAME_ADDR_S *pstSrcNameAddr = NULL_PTR;
    SIP_NAME_ADDR_S *pstDstNameAddr = NULL_PTR;

    pstSrcNameAddr = (SIP_NAME_ADDR_S *)pSrcStruct;
    SIP_GET_COMPONET_PTR(pstDstNameAddr, SIP_NAME_ADDR_S, pstDstUbufMsg, ppDstStruct);

    if (pstSrcNameAddr->pucName != NULL_PTR)
    {
        SIP_CLONE_STRING(pstDstUbufMsg,
                         pstDstNameAddr->pucName,
                         pstSrcNameAddr->pucName);
    }

    ulRet = SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_ADDR_SPEC)(pstSrcNameAddr->pstUri,
                                                        pstDstUbufMsg,
                                                       &pstDstNameAddr->pstUri);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }

    return SUCCESS;
}

/*******************************************************************************
addr-spec      =  SIP-URI / SIPS-URI / absoluteURI
*******************************************************************************/
ULONG SIP_CloneAddrSpec(void          *pSrcStruct,
                        UBUF_HEADER_S *pstDstUbufMsg,
                        void         **ppDstStruct)
{
    ULONG ulRet           = SUCCESS;
    URI_S *pstSrcAddrSpec = NULL_PTR;
    URI_S *pstDstAddrSpec = NULL_PTR;
    void  *pstDstStruct   = NULL_PTR;

    pstSrcAddrSpec = (URI_S *)pSrcStruct;
    SIP_GET_COMPONET_PTR(pstDstAddrSpec, URI_S, pstDstUbufMsg, ppDstStruct);

    pstDstStruct = &pstDstAddrSpec->u.stSipUri;
    switch (pstSrcAddrSpec->eUriType)
    {
        case URI_TYPE_SIP:
            ulRet = SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_SIP_URI)(&pstSrcAddrSpec->u.stSipUri,
                                                               pstDstUbufMsg,
                                                              &pstDstStruct);
            break;

        case URI_TYPE_SIPS:
            ulRet = SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_SIPS_URI)(&pstSrcAddrSpec->u.stSipUri,
                                                               pstDstUbufMsg,
                                                              &pstDstStruct);
            break;

        default:
            return FAIL;
    }

    return ulRet;
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
    SIP_HEADER_ACCEPT_S *pstSrcAccept = NULL_PTR;
    SIP_HEADER_ACCEPT_S *pstDstAccept = NULL_PTR;

    pstSrcAccept = (SIP_HEADER_ACCEPT_S *)pSrcStruct;
    SIP_GET_COMPONET_PTR(pstDstAccept, SIP_HEADER_ACCEPT_S, pstDstUbufMsg, ppDstStruct);

    SIP_CLONE_STRING(pstDstUbufMsg,
                     pstDstAccept->pucTemp,
                     pstSrcAccept->pucTemp);

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
    SIP_HEADER_ACCEPT_ENCODING_S *pstSrcAcceptEncoding = NULL_PTR;
    SIP_HEADER_ACCEPT_ENCODING_S *pstDstAcceptEncoding = NULL_PTR;

    pstSrcAcceptEncoding = (SIP_HEADER_ACCEPT_ENCODING_S *)pSrcStruct;
    SIP_GET_COMPONET_PTR(pstDstAcceptEncoding, SIP_HEADER_ACCEPT_ENCODING_S, pstDstUbufMsg, ppDstStruct);

    SIP_CLONE_STRING(pstDstUbufMsg,
                     pstDstAcceptEncoding->pucTemp,
                     pstSrcAcceptEncoding->pucTemp);

    return SUCCESS;
}

/*******************************************************************************
Call-ID  =  ( "Call-ID" / "i" ) HCOLON callid
*******************************************************************************/
ULONG SIP_CloneHeaderCallID(void          *pSrcStruct,
                            UBUF_HEADER_S *pstDstUbufMsg,
                            void         **ppDstStruct)
{
    SIP_HEADER_CALL_ID_S *pstSrcCallID= NULL_PTR;
    SIP_HEADER_CALL_ID_S *pstDstCallID= NULL_PTR;

    pstSrcCallID = (SIP_HEADER_CALL_ID_S *)pSrcStruct;
    SIP_GET_COMPONET_PTR(pstDstCallID, SIP_HEADER_CALL_ID_S, pstDstUbufMsg, ppDstStruct);

    SIP_CLONE_STRING(pstDstUbufMsg,
                     pstDstCallID->pucCallID,
                     pstSrcCallID->pucCallID);

    return SUCCESS;
}

/*******************************************************************************
CSeq  =  "CSeq" HCOLON 1*DIGIT LWS Method
*******************************************************************************/
ULONG SIP_CloneHeaderCseq(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    ULONG ulRet;
    SIP_HEADER_CSEQ_S *pstSrcCseq   = NULL_PTR;
    SIP_HEADER_CSEQ_S *pstDstCseq   = NULL_PTR;
    void              *pstDstStruct = NULL_PTR;

    pstSrcCseq = (SIP_HEADER_CSEQ_S *)pSrcStruct;
    SIP_GET_COMPONET_PTR(pstDstCseq, SIP_HEADER_CSEQ_S, pstDstUbufMsg, ppDstStruct);

    pstDstCseq->ulSeq = pstSrcCseq->ulSeq;

    pstDstStruct = &pstDstCseq->eMethod;
    ulRet = SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_METHOD)(&pstSrcCseq->eMethod,
                                                      pstDstUbufMsg,
                                                     &pstDstStruct);

    return ulRet;
}

/*******************************************************************************
From        =  ( "From" / "f" ) HCOLON from-spec
*******************************************************************************/
ULONG SIP_CloneHeaderFrom(void          *pSrcStruct,
                          UBUF_HEADER_S *pstDstUbufMsg,
                          void         **ppDstStruct)
{
    SIP_HEADER_FROM_S *pstSrcFrom = NULL_PTR;
    SIP_HEADER_FROM_S *pstDstFrom = NULL_PTR;

    pstSrcFrom = (SIP_HEADER_FROM_S *)pSrcStruct;
    SIP_GET_COMPONET_PTR(pstDstFrom, SIP_HEADER_FROM_S, pstDstUbufMsg, ppDstStruct);

    return SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_FROM_SPEC)(pstSrcFrom,
                                                       pstDstUbufMsg,
                                                      &pstDstFrom);
}

/*******************************************************************************
To        =  ( "To" / "t" ) HCOLON ( name-addr
             / addr-spec ) *( SEMI to-param )
*******************************************************************************/
ULONG SIP_CloneHeaderTo(void          *pSrcStruct,
                        UBUF_HEADER_S *pstDstUbufMsg,
                        void         **ppDstStruct)
{
    ULONG ulRet;
    SIP_HEADER_TO_S *pstSrcTo = NULL_PTR;
    SIP_HEADER_TO_S *pstDstTo = NULL_PTR;
    void            *pstDstStruct = NULL_PTR;

    pstSrcTo = (SIP_HEADER_TO_S *)pSrcStruct;
    SIP_GET_COMPONET_PTR(pstDstTo, SIP_HEADER_TO_S, pstDstUbufMsg, ppDstStruct);

    if (pstSrcTo->stNameAddr.bName == TRUE)
    {
        pstDstTo->stNameAddr.bName = TRUE;
        pstDstStruct = &pstDstTo->stNameAddr;
        ulRet = SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_NAME_ADDR)(&pstSrcTo->stNameAddr,
                                                             pstDstUbufMsg,
                                                            &pstDstStruct);
    }
    else
    {
        pstDstTo->stNameAddr.bName = FALSE;
        ulRet = SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_ADDR_SPEC)(pstSrcTo->stNameAddr.pstUri,
                                                            pstDstUbufMsg,
                                                           &pstDstTo->stNameAddr.pstUri);
    }

    if (ulRet!= SUCCESS)
    {
        return ulRet;
    }

    ulRet = SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_TO_PARAM)(pstSrcTo,
                                                       pstDstUbufMsg,
                                                      &pstDstTo);
    if (ulRet!= SUCCESS)
    {
        return ulRet;
    }

    return SUCCESS;
}

/*******************************************************************************
Via               =  ( "Via" / "v" ) HCOLON via-parm *(COMMA via-parm)
*******************************************************************************/
ULONG SIP_CloneHeaderVia(void          *pSrcStruct,
                         UBUF_HEADER_S *pstDstUbufMsg,
                         void         **ppDstStruct)
{
    ULONG ulRet;
    SIP_HEADER_VIA_S *pstSrcVia = NULL_PTR;
    SIP_HEADER_VIA_S *pstDstVia = NULL_PTR;
    SIP_VIA_PARM_S   **ppstSrcViaParm = NULL_PTR;
    SIP_VIA_PARM_S   **ppstDstViaParm = NULL_PTR;

    pstSrcVia = (SIP_HEADER_VIA_S *)pSrcStruct;
    SIP_GET_COMPONET_PTR(pstDstVia, SIP_HEADER_VIA_S, pstDstUbufMsg, ppDstStruct);

    ppstSrcViaParm = &pstSrcVia->pstViaParm;
    ppstDstViaParm = &pstDstVia->pstViaParm;
    while(*ppstSrcViaParm != NULL_PTR)
    {
        ulRet = SIP_GET_CLONE_FUNC(SIP_ABNF_RULE_VIA_PARM)(*ppstSrcViaParm,
                                                           pstDstUbufMsg,
                                                           ppstDstViaParm);
        if (ulRet != NULL_PTR)
        {
            return ulRet;
        }

        ppstSrcViaParm = &(*ppstSrcViaParm)->pstNext;
        ppstDstViaParm = &(*ppstDstViaParm)->pstNext;
    }

    return SUCCESS;
}

/*******************************************************************************
Max-Forwards  =  "Max-Forwards" HCOLON 1*DIGIT
*******************************************************************************/
ULONG SIP_CloneHeaderMaxForwards(void          *pSrcStruct,
                                 UBUF_HEADER_S *pstDstUbufMsg,
                                 void         **ppDstStruct)
{
    SIP_HEADER_MAX_FORWARDS_S *pstSrcMaxForwards = NULL_PTR;
    SIP_HEADER_MAX_FORWARDS_S *pstDstMaxForwards = NULL_PTR;

    pstSrcMaxForwards = (SIP_HEADER_MAX_FORWARDS_S *)pSrcStruct;
    SIP_GET_COMPONET_PTR(pstDstMaxForwards, SIP_HEADER_MAX_FORWARDS_S, pstDstUbufMsg, ppDstStruct);

    pstDstMaxForwards->ulMaxForwards = pstSrcMaxForwards->ulMaxForwards;

    return SUCCESS;
}

