UCHAR *g_pucSdpBuffer = NULL_PTR;
ABNF_RULE_LIST_S *g_pstSdpRuleList = NULL_PTR;

/* 判断模块是否初始化标记 */
BOOL         g_bSdpInit = FALSE;

/* SIP ABNF规则算法表，包括编码，解析和克隆 */
SIP_APP_RULE_S g_astSdpAppRuleTbl[SIP_ABNF_RULE_BUTT] =
{
session-description,   
proto-version,         
origin-field,          
session-name-field,    
information-field,     
uri-field,             
email-fields,          
phone-fields,          
connection-field,      
bandwidth-fields,      
time-fields,           
repeat-fields,         
zone-adjustments,      
key-field,             
attribute-fields,      
media-descriptions,    
media-field,           
username,              
sess-id,               
sess-version,          
nettype,               
addrtype,              
uri,                   
email-address,         
address-and-comment,   
dispname-and-address,  
phone-number,          
phone,                 
connection-address,    
bwtype,                
bandwidth,             
start-time,            
stop-time,             
time,                  
repeat-interval,       
typed-time,            
fixed-len-time-unit,   
key-type,              
base64,                
base64-unit,           
base64-pad,            
base64-char,           
attribute,             
att-field,             
att-value,             
media,                 
fmt,                   
proto,                 
port,                  
unicast-address,       
multicast-address,     
IP4-multicast,         
m1,                    
IP6-multicast,         
ttl,                   
FQDN,                  
IP4-address,           
b1,                    
IP6-address,           
hexpart,               
hexseq,                
hex4,                  
extn-addr,             
text,                  
byte-string,           
non-ws-string,         
token-char,            
token,                 
email-safe,            
integer,               
alpha-numeric,         
POS-DIGIT,             
decimal-uchar,         
alpha-numeric,         
    //SIP_ABNF_RULE_SIP_MESSAGE,
    {"SIP-message"     , NULL_ULONG, SIP_CodeSIPmessage,            SIP_ParseSIPmessage,            SIP_CloneSIPmessage},
    //SIP_ABNF_RULE_REQUEST,
    {"Request"         , NULL_ULONG, SIP_CodeRequest,               SIP_ParseRequest,               SIP_CloneRequest},
    //SIP_ABNF_RULE_RESPONSE
    {"Response"        , NULL_ULONG, SIP_CodeResponse,              SIP_ParseResponse,              SIP_CloneResponse},
    //SIP_ABNF_RULE_REQUEST_LINE
    {"Request-Line"    , NULL_ULONG, SIP_CodeRequestLine,           SIP_ParseRequestLine,           SIP_CloneRequestLine},
    //SIP_ABNF_RULE_STATUS_LINE
    {"Status-Line"     , NULL_ULONG, SIP_CodeStatusLine,            SIP_ParseStatusLine,            SIP_CloneStatusLine},
    //SIP_ABNF_RULE_MESSAGE_HEADER
    {"message-header"  , NULL_ULONG, SIP_CodeMessageHeader,         SIP_ParseMessageHeader,         SIP_CloneMessageHeader},
    //SIP_ABNF_RULE_MESSAGE_BODY
    {"message-body"    , NULL_ULONG, SIP_CodeMessageBody,           SIP_ParseMessageBody,           SIP_CloneMessageBody},
     //SIP_ABNF_RULE_METHOD
    {"Method"          , NULL_ULONG, SIP_CodeMethod,                SIP_ParseMethod,                SIP_CloneMethod},
    //SIP_ABNF_RULE_INVITE_M
    {"INVITEm"         , NULL_ULONG, NULL_PTR,                      NULL_PTR,                       NULL_PTR},
    //SIP_ABNF_RULE_ACK_M
    {"ACKm"            , NULL_ULONG, NULL_PTR,                      NULL_PTR,                       NULL_PTR},
    //SIP_ABNF_RULE_OPTIONS_M
    {"OPTIONSm"        , NULL_ULONG, NULL_PTR,                      NULL_PTR,                       NULL_PTR},
     //SIP_ABNF_RULE_BYE_M
    {"BYEm"            , NULL_ULONG, NULL_PTR,                      NULL_PTR,                       NULL_PTR},
    //SIP_ABNF_RULE_CANCEL_M
    {"CANCELm"         , NULL_ULONG, NULL_PTR,                      NULL_PTR,                       NULL_PTR},
    //SIP_ABNF_RULE_REGISTER_M
    {"REGISTERm"       , NULL_ULONG, NULL_PTR,                      NULL_PTR,                       NULL_PTR},
     //SIP_ABNF_RULE_REQUEST_URI
    {"Request-URI"     , NULL_ULONG, SIP_CodeRequestURI,            SIP_ParseRequestURI,            SIP_CloneRequestURI},
    //SIP_ABNF_RULE_SIP_VERSION
    {"SIP-Version"     , NULL_ULONG, SIP_CodeSIPVersion,            SIP_ParseSIPVersion,            SIP_CloneSIPVersion},
    //SIP_ABNF_RULE_STATUS_CODE
    {"Status-Code"     , NULL_ULONG, SIP_CodeStatusCode,            SIP_ParseStatusCode,            SIP_CloneStatusCode},
    //SIP_ABNF_RULE_ACCEPT
    {"Accept"          , NULL_ULONG, SIP_CodeHeaderAccept,          SIP_ParseHeaderAccept,          SIP_CloneHeaderAccept},
    //SIP_ABNF_RULE_ACCEPT_ENCODING
    {"Accept-Encoding" , NULL_ULONG, SIP_CodeHeaderAcceptEncoding,  SIP_ParseHeaderAcceptEncoding,  SIP_CloneHeaderAcceptEncoding},
     //SIP_ABNF_RULE_CALL_ID
    {"Call-ID"         , NULL_ULONG, SIP_CodeHeaderCallID,          SIP_ParseHeaderCallID,          SIP_CloneHeaderCallID},
     //SIP_ABNF_RULE_CSEQ
    {"CSeq"            , NULL_ULONG, SIP_CodeHeaderCseq,            SIP_ParseHeaderCseq,            SIP_CloneHeaderCseq},
    //SIP_ABNF_RULE_FROM
    {"From"            , NULL_ULONG, SIP_CodeHeaderFrom,            SIP_ParseHeaderFrom,            SIP_CloneHeaderFrom},
     //SIP_ABNF_RULE_TO
    {"To"              , NULL_ULONG, SIP_CodeHeaderTo,              SIP_ParseHeaderTo,              SIP_CloneHeaderTo},
    //SIP_ABNF_RULE_VIA
    {"Via"             , NULL_ULONG, SIP_CodeHeaderVia,             SIP_ParseHeaderVia,             SIP_CloneHeaderVia},
    //SIP_ABNF_RULE_MAX_FORWARDS
    {"Max-Forwards"    , NULL_ULONG, SIP_CodeHeaderMaxForwards,     SIP_ParseHeaderMaxForwards,     SIP_CloneHeaderMaxForwards},
    //SIP_ABNF_RULE_SIP_URI
    {"SIP-URI"         , NULL_ULONG, SIP_CodeSipURI,                SIP_ParseSipURI,                SIP_CloneSipURI},
    //SIP_ABNF_RULE_SIPS_URI
    {"SIPS-URI"        , NULL_ULONG, SIP_CodeSipsURI,               SIP_ParseSipsURI,               SIP_CloneSipsURI},
    //SIP_ABNF_RULE_USERINFO
    {"userinfo"        , NULL_ULONG, SIP_CodeUserinfo,              SIP_ParseUserinfo,              SIP_CloneUserinfo},
    //SIP_ABNF_RULE_HOSTPORT
    {"hostport"        , NULL_ULONG, SIP_CodeHostport,              SIP_ParseHostport,              SIP_CloneHostport},
    //SIP_ABNF_RULE_USER
    {"user"            , NULL_ULONG, SIP_CodeUser,                  NULL_PTR,                       NULL_PTR},
    //SIP_ABNF_RULE_PASSWORD
    {"password"        , NULL_ULONG, NULL_PTR,                      NULL_PTR,                       NULL_PTR},
    //SIP_ABNF_RULE_HOST
    {"host"            , NULL_ULONG, SIP_CodeHost,                  SIP_ParseHost,                  SIP_CloneHost},
    //SIP_ABNF_RULE_PORT
    {"port"            , NULL_ULONG, SIP_CodePort,                  SIP_ParsePort,                  SIP_ClonePort},
    //SIP_ABNF_RULE_HOST_NAME
    {"hostname"        , NULL_ULONG, NULL_PTR,                      NULL_PTR,                       NULL_PTR},
    //SIP_ABNF_RULE_IPV4_ADDRESS
    {"IPv4address"     , NULL_ULONG, NULL_PTR,                      NULL_PTR,                       NULL_PTR},
    //SIP_ABNF_RULE_IPV6_ADDRESS
    {"IPv6address"     , NULL_ULONG, NULL_PTR,                      NULL_PTR,                       NULL_PTR},
    //SIP_ABNF_RULE_IPV6_REFERENCE
    {"IPv6reference"   , NULL_ULONG, NULL_PTR,                      NULL_PTR,                       NULL_PTR},
     //SIP_ABNF_RULE_FROM_SPEC
    {"from-spec"       , NULL_ULONG, SIP_CodeFromSpec,              SIP_ParseFromSpec,              SIP_CloneFromSpec},
    //SIP_ABNF_RULE_NAME_ADDR
    {"name-addr"       , NULL_ULONG, SIP_CodeNameAddr,              SIP_ParseNameAddr,              SIP_CloneNameAddr},
    //SIP_ABNF_RULE_ADDR_SPEC
    {"addr-spec"       , NULL_ULONG, SIP_CodeAddrSpec,              SIP_ParseAddrSpec,              SIP_CloneAddrSpec},
    //SIP_ABNF_RULE_FROM_PARAM
    {"from-param"      , NULL_ULONG, SIP_CodeFromParam,             SIP_ParseFromParam,             SIP_CloneFromParam},
    //SIP_ABNF_RULE_DISPLAY_NAME
    {"display-name"    , NULL_ULONG, NULL_PTR,                      NULL_PTR,                       NULL_PTR},
    //SIP_ABNF_RULE_TO_PARAM
    {"to-param"        , NULL_ULONG, SIP_CodeToParam,               SIP_ParseToParam,               SIP_CloneToParam},
    //SIP_ABNF_RULE_TAG_PARAM
    {"tag-param"       , NULL_ULONG, NULL_PTR,                      SIP_ParseTagParam,              NULL_PTR},
    //SIP_ABNF_RULE_CALLID
    {"callid"          , NULL_ULONG, NULL_PTR,                      SIP_ParseCallID,                NULL_PTR},
    //SIP_ABNF_RULE_VIA_PARM
    {"via-parm"        , NULL_ULONG, SIP_CodeViaParm,               SIP_ParseViaParm,               SIP_CloneViaParm},
    //SIP_ABNF_RULE_VIA_PARMS
    {"via-params"      , NULL_ULONG, SIP_CodeViaParams,             SIP_ParseViaParams,             SIP_CloneViaParams},
    //SIP_ABNF_RULE_SENT_PROTOCOL
    {"sent-protocol"   , NULL_ULONG, SIP_CodeSentProtocol,          SIP_ParseSentProtocol,          SIP_CloneSentProtocol},
    //SIP_ABNF_RULE_SENT_BY
    {"sent-by"         , NULL_ULONG, SIP_CodeSentBy,                SIP_ParseSentBy,                SIP_CloneSentBy},
    //SIP_ABNF_RULE_TRANSPORT
    {"transport"       , NULL_ULONG, SIP_CodeTransport,             SIP_ParseTransport,             SIP_CloneTransport},
    //SIP_ABNF_RULE_VIA_TTL
    {"via-ttl"         , NULL_ULONG, SIP_CodeViaTtl,                SIP_ParseViaTtl,                SIP_CloneViaTtl},
    //SIP_ABNF_RULE_VIA_MADDR
    {"via-maddr"       , NULL_ULONG, SIP_CodeViaMaddr,              SIP_ParseViaMaddr,              SIP_CloneViaMaddr},
    //SIP_ABNF_RULE_VIA_RECEIVED
    {"via-received"    , NULL_ULONG, SIP_CodeViaReceived,           SIP_ParseViaReceived,           SIP_CloneViaReceived},
    //SIP_ABNF_RULE_VIA_BRANCH
    {"via-branch"      , NULL_ULONG, SIP_CodeViaBranch,             SIP_ParseViaBranch,             NULL_PTR},
    //SIP_ABNF_RULE_TTL
    {"ttl"             , NULL_ULONG, NULL_PTR,                      NULL_PTR,                       NULL_PTR},
    //SIP_ABNF_RULE_TOKEN
    {"token"           , NULL_ULONG, NULL_PTR,                      NULL_PTR,                       NULL_PTR},
    //SIP_ABNF_RULE_STAR
    {"STAR"            , NULL_ULONG, NULL_PTR,                      NULL_PTR,                       NULL_PTR},
    //SIP_ABNF_RULE_CONTACT
    {"Contact"         , NULL_ULONG, SIP_CodeHeaderContact,         SIP_ParseHeaderContact,         NULL_PTR},
    //SIP_ABNF_RULE_CONTACT_PARAM
    {"contact-param"   , NULL_ULONG, SIP_CodeContactParam,          SIP_ParseContactParam,          NULL_PTR},    
};

