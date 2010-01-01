/*******************************************************************************
功能    : SIP协议栈语法与编码层全局变量
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/
/* 标准头文件 */
#include <stdio.h>

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
#include "sip_syntax_decode.inc"
#include "sip_syntax_clone.inc"

/* 本模块全局变量声明头文件 */
#include "sip_syntax_var.inc"

/* 判断模块是否初始化标记 */
BOOL         g_bSipSyntaxInit = FALSE;

ABNF_RULE_LIST_S *g_pstSipRuleList = NULL_PTR;

/* SIP ABNF规则算法表，包括编码，解析和克隆 */
SIP_APP_RULE_S g_astSipAppRuleTbl[SIP_ABNF_RULE_BUTT] =
{
    //SIP_ABNF_RULE_SIP_MESSAGE,
    {"SIP-message"     , NULL_ULONG, SIP_CodeSIPmessage,            SIP_ParseSIPmessage,             SIP_CloneSIPmessage},
    //SIP_ABNF_RULE_REQUEST,
    {"Request"         , NULL_ULONG, SIP_CodeRequest,               SIP_ParseRequest,                SIP_CloneRequest},
    //SIP_ABNF_RULE_RESPONSE
    {"Response"        , NULL_ULONG, SIP_CodeResponse,              SIP_ParseResponse,               SIP_CloneResponse},
    //SIP_ABNF_RULE_REQUEST_LINE
    {"Request-Line"    , NULL_ULONG, SIP_CodeRequestLine,           SIP_ParseRequestLine,            SIP_CloneRequestLine},
    //SIP_ABNF_RULE_STATUS_LINE
    {"Status-Line"     , NULL_ULONG, SIP_CodeStatusLine,            SIP_ParseStatusLine,             SIP_CloneStatusLine},
    //SIP_ABNF_RULE_MESSAGE_HEADER
    {"message-header"  , NULL_ULONG, SIP_CodeMessageHeader,         SIP_ParseMessageHeader,          SIP_CloneMessageHeader},
    //SIP_ABNF_RULE_MESSAGE_BODY
    {"message-body"    , NULL_ULONG, SIP_CodeMessageBody,           SIP_ParseMessageBody,            SIP_CloneMessageBody},
     //SIP_ABNF_RULE_METHOD
    {"Method"          , NULL_ULONG, SIP_CodeMethod,                SIP_ParseMethod,                 SIP_CloneMethod},
    //SIP_ABNF_RULE_INVITE_M
    {"INVITEm"         , NULL_ULONG, NULL_PTR,                      NULL_PTR,                        NULL_PTR},
    //SIP_ABNF_RULE_ACK_M
    {"ACKm"            , NULL_ULONG, NULL_PTR,                      NULL_PTR,                        NULL_PTR},
    //SIP_ABNF_RULE_OPTIONS_M
    {"OPTIONSm"        , NULL_ULONG, NULL_PTR,                      NULL_PTR,                        NULL_PTR},
     //SIP_ABNF_RULE_BYE_M
    {"BYEm"            , NULL_ULONG, NULL_PTR,                      NULL_PTR,                        NULL_PTR},
    //SIP_ABNF_RULE_CANCEL_M
    {"CANCELm"         , NULL_ULONG, NULL_PTR,                      NULL_PTR,                        NULL_PTR},
    //SIP_ABNF_RULE_REGISTER_M
    {"REGISTERm"       , NULL_ULONG, NULL_PTR,                      NULL_PTR,                        NULL_PTR},
     //SIP_ABNF_RULE_REQUEST_URI
    {"Request-URI"     , NULL_ULONG, SIP_CodeRequestURI,            SIP_ParseRequestURI,             SIP_CloneRequestURI},
    //SIP_ABNF_RULE_SIP_VERSION
    {"SIP-Version"     , NULL_ULONG, SIP_CodeSIPVersion,            SIP_ParseSIPVersion,             SIP_CloneSIPVersion},
    //SIP_ABNF_RULE_STATUS_CODE
    {"Status-Code"     , NULL_ULONG, SIP_CodeStatusCode,            SIP_ParseStatusCode,             SIP_CloneStatusCode},
    //SIP_ABNF_RULE_ACCEPT
    {"Accept"          , NULL_ULONG, SIP_CodeHeaderAccept,          SIP_ParseHeaderAccept,           SIP_CloneHeaderAccept},
    //SIP_ABNF_RULE_ACCEPT_ENCODING
    {"Accept-Encoding" , NULL_ULONG, SIP_CodeHeaderAcceptEncoding,  SIP_ParseHeaderAcceptEncoding,   SIP_CloneHeaderAcceptEncoding},
     //SIP_ABNF_RULE_CALL_ID
    {"Call-ID"         , NULL_ULONG, SIP_CodeHeaderCallID,          SIP_ParseHeaderCallID,           SIP_CloneHeaderCallID},
     //SIP_ABNF_RULE_CSEQ
    {"CSeq"            , NULL_ULONG, SIP_CodeHeaderCseq,            SIP_ParseHeaderCseq,             SIP_CloneHeaderCseq},
    //SIP_ABNF_RULE_FROM
    {"From"            , NULL_ULONG, SIP_CodeHeaderFrom,            SIP_ParseHeaderFrom,             SIP_CloneHeaderFrom},
     //SIP_ABNF_RULE_TO
    {"To"              , NULL_ULONG, SIP_CodeHeaderTo,              SIP_ParseHeaderTo,               SIP_CloneHeaderTo},
    //SIP_ABNF_RULE_VIA
    {"Via"             , NULL_ULONG, SIP_CodeHeaderVia,             SIP_ParseHeaderVia,              SIP_CloneHeaderVia},
    //SIP_ABNF_RULE_MAX_FORWARDS
    {"Max-Forwards"    , NULL_ULONG, SIP_CodeHeaderMaxForwards,     SIP_ParseHeaderMaxForwards,      SIP_CloneHeaderMaxForwards},
    //SIP_ABNF_RULE_SIP_URI
    {"SIP-URI"         , NULL_ULONG, SIP_CodeSipURI,                SIP_ParseSipURI,                 SIP_CloneSipURI},
    //SIP_ABNF_RULE_SIPS_URI
    {"SIPS-URI"        , NULL_ULONG, SIP_CodeSipsURI,               SIP_ParseSipsURI,                SIP_CloneSipsURI},
    //SIP_ABNF_RULE_USERINFO
    {"userinfo"        , NULL_ULONG, SIP_CodeUserinfo,              SIP_ParseUserinfo,               SIP_CloneUserinfo},
    //SIP_ABNF_RULE_HOSTPORT
    {"hostport"        , NULL_ULONG, SIP_CodeHostport,              SIP_ParseHostport,               SIP_CloneHostport},
    //SIP_ABNF_RULE_USER
    {"user"            , NULL_ULONG, SIP_CodeUser,                  NULL_PTR,                        NULL_PTR},
    //SIP_ABNF_RULE_PASSWORD
    {"password"        , NULL_ULONG, NULL_PTR,                      NULL_PTR,                        NULL_PTR},
    //SIP_ABNF_RULE_HOST
    {"host"            , NULL_ULONG, SIP_CodeHost,                  SIP_ParseHost,                   SIP_CloneHost},
    //SIP_ABNF_RULE_PORT
    {"port"            , NULL_ULONG, SIP_CodePort,                  SIP_ParsePort,                   SIP_ClonePort},
    //SIP_ABNF_RULE_HOST_NAME
    {"hostname"        , NULL_ULONG, NULL_PTR,                      NULL_PTR,                        NULL_PTR},
    //SIP_ABNF_RULE_IPV4_ADDRESS
    {"IPv4address"     , NULL_ULONG, NULL_PTR,                      NULL_PTR,                        NULL_PTR},
    //SIP_ABNF_RULE_IPV6_ADDRESS
    {"IPv6address"     , NULL_ULONG, NULL_PTR,                      NULL_PTR,                        NULL_PTR},
    //SIP_ABNF_RULE_IPV6_REFERENCE
    {"IPv6reference"   , NULL_ULONG, NULL_PTR,                      NULL_PTR,                        NULL_PTR},
     //SIP_ABNF_RULE_FROM_SPEC
    {"from-spec"       , NULL_ULONG, SIP_CodeFromSpec,              SIP_ParseFromSpec,               SIP_CloneFromSpec},
    //SIP_ABNF_RULE_NAME_ADDR
    {"name-addr"       , NULL_ULONG, SIP_CodeNameAddr,              SIP_ParseNameAddr,               SIP_CloneNameAddr},
    //SIP_ABNF_RULE_ADDR_SPEC
    {"addr-spec"       , NULL_ULONG, SIP_CodeAddrSpec,              SIP_ParseAddrSpec,               SIP_CloneAddrSpec},
    //SIP_ABNF_RULE_FROM_PARAM
    {"from-param"      , NULL_ULONG, SIP_CodeFromParam,             SIP_ParseFromParam,              SIP_CloneFromParam},
    //SIP_ABNF_RULE_DISPLAY_NAME
    {"display-name"    , NULL_ULONG, NULL_PTR,                      NULL_PTR,                        NULL_PTR},
    //SIP_ABNF_RULE_TO_PARAM
    {"to-param"        , NULL_ULONG, SIP_CodeToParam,               SIP_ParseToParam,                SIP_CloneToParam},
    //SIP_ABNF_RULE_TAG_PARAM
    {"tag-param"       , NULL_ULONG, NULL_PTR,                      SIP_ParseTagParam,               NULL_PTR},
    //SIP_ABNF_RULE_CALLID
    {"callid"          , NULL_ULONG, NULL_PTR,                      SIP_ParseCallID,                 NULL_PTR},
    //SIP_ABNF_RULE_VIA_PARM
    {"via-parm"        , NULL_ULONG, SIP_CodeViaParm,               SIP_ParseViaParm,                SIP_CloneViaParm},
    //SIP_ABNF_RULE_VIA_PARMS
    {"via-params"      , NULL_ULONG, SIP_CodeViaParams,             SIP_ParseViaParams,              SIP_CloneViaParams},
    //SIP_ABNF_RULE_SENT_PROTOCOL
    {"sent-protocol"   , NULL_ULONG, SIP_CodeSentProtocol,          SIP_ParseSentProtocol,           SIP_CloneSentProtocol},
    //SIP_ABNF_RULE_SENT_BY
    {"sent-by"         , NULL_ULONG, SIP_CodeSentBy,                SIP_ParseSentBy,                 SIP_CloneSentBy},
    //SIP_ABNF_RULE_TRANSPORT
    {"transport"       , NULL_ULONG, SIP_CodeTransport,             SIP_ParseTransport,              SIP_CloneTransport},
    //SIP_ABNF_RULE_VIA_TTL
    {"via-ttl"         , NULL_ULONG, SIP_CodeViaTtl,                SIP_ParseViaTtl,                 SIP_CloneViaTtl},
    //SIP_ABNF_RULE_VIA_MADDR
    {"via-maddr"       , NULL_ULONG, SIP_CodeViaMaddr,              SIP_ParseViaMaddr,               SIP_CloneViaMaddr},
    //SIP_ABNF_RULE_VIA_RECEIVED
    {"via-received"    , NULL_ULONG, SIP_CodeViaReceived,           SIP_ParseViaReceived,            SIP_CloneViaReceived},
    //SIP_ABNF_RULE_VIA_BRANCH
    {"via-branch"      , NULL_ULONG, SIP_CodeViaBranch,             SIP_ParseViaBranch,              NULL_PTR},
    //SIP_ABNF_RULE_TTL
    {"ttl"             , NULL_ULONG, NULL_PTR,                      NULL_PTR,                        NULL_PTR},
    //SIP_ABNF_RULE_TOKEN
    {"token"           , NULL_ULONG, NULL_PTR,                      NULL_PTR,                        NULL_PTR},
};

/* 头域参数表 */
SIP_SYNTAX_HEADER_MAP_S g_astSipSyntaxHeaderTbl[SIP_HEADER_BUTT] =
{
    //SIP_HEADER_ACCEPT,
    {SIP_ABNF_RULE_ACCEPT,          sizeof(SIP_HEADER_ACCEPT_S)},
    //SIP_HEADER_ACCEPT_ENCODING,
    {SIP_ABNF_RULE_ACCEPT_ENCODING, sizeof(SIP_HEADER_ACCEPT_ENCODING_S)},
    //SIP_HEADER_ACCEPT_LANGUAGE,
    {SIP_ABNF_RULE_BUTT,            sizeof(SIP_HEADER_ACCEPT_LANGUAGE_S)},
    //SIP_HEADER_ALERT_INFO,
    {SIP_ABNF_RULE_BUTT,            sizeof(SIP_HEADER_ALERT_INFO_S)},
    //SIP_HEADER_ALLOW,
    {SIP_ABNF_RULE_BUTT,            sizeof(SIP_HEADER_ALLOW_S)},
    //SIP_HEADER_AUTHENTICATICATION_INFO,
    {SIP_ABNF_RULE_BUTT,            sizeof(SIP_HEADER_AUTHENTICATICATION_INFO_S)},
    //SIP_HEADER_AUTHORIZATION,
    {SIP_ABNF_RULE_BUTT,            sizeof(SIP_HEADER_AUTHORIZATION_S)},
    //SIP_HEADER_CALL_ID,
    {SIP_ABNF_RULE_CALL_ID,         sizeof(SIP_HEADER_CALL_ID_S)},
    //SIP_HEADER_CALL_INFO,
    {SIP_ABNF_RULE_BUTT,            sizeof(SIP_HEADER_CALL_INFO_S)},
    //SIP_HEADER_CONTACT,
    {SIP_ABNF_RULE_BUTT,            sizeof(SIP_HEADER_CONTACT_S)},
    //SIP_HEADER_CONTENT_DISPOSITION,
    {SIP_ABNF_RULE_BUTT,            sizeof(SIP_HEADER_CONTENT_DISPOSITION_S)},
    //SIP_HEADER_CONTENT_ENCODING,
    {SIP_ABNF_RULE_BUTT,            sizeof(SIP_HEADER_CONTENT_ENCODING_S)},
    //SIP_HEADER_CONTENT_LANGUAGE,
    {SIP_ABNF_RULE_BUTT,            sizeof(SIP_HEADER_CONTENT_LANGUAGE_S)},
    //SIP_HEADER_CONTENT_LENTH,
    {SIP_ABNF_RULE_BUTT,            sizeof(SIP_HEADER_CONTENT_LENTH_S)},
    //SIP_HEADER_CONTENT_TYPE,
    {SIP_ABNF_RULE_BUTT,            sizeof(SIP_HEADER_CONTENT_TYPE_S)},
    //SIP_HEADER_CSEQ,
    {SIP_ABNF_RULE_CSEQ,            sizeof(SIP_HEADER_CSEQ_S)},
    //SIP_HEADER_DATA,
    {SIP_ABNF_RULE_BUTT,            sizeof(SIP_HEADER_DATA_S)},
    //SIP_HEADER_ERROR_INFO,
    {SIP_ABNF_RULE_BUTT,            sizeof(SIP_HEADER_ERROR_INFO_S)},
    //SIP_HEADER_EXPIRES,
    {SIP_ABNF_RULE_BUTT,            sizeof(SIP_HEADER_EXPIRES_S)},
    //SIP_HEADER_FROM,
    {SIP_ABNF_RULE_FROM,            sizeof(SIP_HEADER_FROM_S)},
    //SIP_HEADER_IN_REPLY_TO,
    {SIP_ABNF_RULE_BUTT,            sizeof(SIP_HEADER_IN_REPLY_TO_S)},
    //SIP_HEADER_MAX_FORWARDS,
    {SIP_ABNF_RULE_MAX_FORWARDS,    sizeof(SIP_HEADER_MAX_FORWARDS_S)},
    //SIP_HEADER_MIN_EXPIRES,
    {SIP_ABNF_RULE_BUTT,            sizeof(SIP_HEADER_MIN_EXPIRES_S)},
    //SIP_HEADER_MIME_VERSION,
    {SIP_ABNF_RULE_BUTT,            sizeof(SIP_HEADER_MIME_VERSION_S)},
    //SIP_HEADER_ORGANIZATION,
    {SIP_ABNF_RULE_BUTT,            sizeof(SIP_HEADER_ORGANIZATION_S)},
    //SIP_HEADER_PRIORITY,
    {SIP_ABNF_RULE_BUTT,            sizeof(SIP_HEADER_PRIORITY_S)},
    //SIP_HEADER_PROXY_AUTHENTICATE,
    {SIP_ABNF_RULE_BUTT,            sizeof(SIP_HEADER_PROXY_AUTHENTICATE_S)},
    //SIP_HEADER_PROXY_AUTHORIZATION,
    {SIP_ABNF_RULE_BUTT,            sizeof(SIP_HEADER_PROXY_AUTHORIZATION_S)},
    //SIP_HEADER_PROXY_REQUIRE,
    {SIP_ABNF_RULE_BUTT,            sizeof(SIP_HEADER_PROXY_REQUIRE_S)},
    //SIP_HEADER_RECORD_ROUTE,
    {SIP_ABNF_RULE_BUTT,            sizeof(SIP_HEADER_RECORD_ROUTE_S)},
    //SIP_HEADER_REPLY_TO,
    {SIP_ABNF_RULE_BUTT,            sizeof(SIP_HEADER_REPLY_TO_S)},
    //SIP_HEADER_REQUIRE,
    {SIP_ABNF_RULE_BUTT,            sizeof(SIP_HEADER_REQUIRE_S)},
    //SIP_HEADER_RETRY_AFTER,
    {SIP_ABNF_RULE_BUTT,            sizeof(SIP_HEADER_RETRY_AFTER_S)},
    //SIP_HEADER_ROUTE,
    {SIP_ABNF_RULE_BUTT,            sizeof(SIP_HEADER_ROUTE_S)},
    //SIP_HEADER_SERVER,
    {SIP_ABNF_RULE_BUTT,            sizeof(SIP_HEADER_SERVER_S)},
    //SIP_HEADER_SUBJECT,
    {SIP_ABNF_RULE_BUTT,            sizeof(SIP_HEADER_SUBJECT_S)},
    //SIP_HEADER_SUPPORTED,
    {SIP_ABNF_RULE_BUTT,            sizeof(SIP_HEADER_SUPPORTED_S)},
    //SIP_HEADER_TIMESTAMP,
    {SIP_ABNF_RULE_BUTT,            sizeof(SIP_HEADER_TIMESTAMP_S)},
    //SIP_HEADER_TO,
    {SIP_ABNF_RULE_TO,              sizeof(SIP_HEADER_TO_S)},
    //SIP_HEADER_UNSUPPORTED,
    {SIP_ABNF_RULE_BUTT,            sizeof(SIP_HEADER_UNSUPPORTED_S)},
    //SIP_HEADER_USER_AGENT,
    {SIP_ABNF_RULE_BUTT,            sizeof(SIP_HEADER_USER_AGENT_S)},
    //SIP_HEADER_VIA,
    {SIP_ABNF_RULE_VIA,             sizeof(SIP_HEADER_VIA_S)},
    //SIP_HEADER_WARNING,
    {SIP_ABNF_RULE_BUTT,            sizeof(SIP_HEADER_WARNING_S)},
    //SIP_HEADER_WWW_AUTHENTICATE,
    {SIP_ABNF_RULE_BUTT,            sizeof(SIP_HEADER_WWW_AUTHENTICATE_S)}
};
