/* 判断模块是否初始化标记 */
BOOL   g_bSdpInit       = FALSE;
UCHAR *g_pucSdpBuffer   = NULL_PTR;
UCHAR  g_ucSdpRuleListIndex = NULL_PTR;

/* SDP ABNF规则算法表，包括编码，解析和克隆 */
SDP_APP_RULE_S g_astSdpAppRuleTbl[SDP_ABNF_RULE_BUTT] =
{
    //SDP_ABNF_RULE_SESSION_DESCRIPTION,
    {"session-description", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR},
    //SDP_ABNF_RULE_PROTO_VERSION,
    {"proto-version",       NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR},
    //SDP_ABNF_RULE_ORIGIN_FIELD,
    {"origin-field", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR},
    //SDP_ABNF_RULE_SESSION_NAME_FIELD,
    {"session-name-field", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR},
    //SDP_ABNF_RULE_INFORMATION_FIELD,
    {"information-field", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR},    
    //SDP_ABNF_RULE_URI_FIELD,
    {"uri-field", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR},        
    //SDP_ABNF_RULE_EMAIL_FIELDS,
    {"email-fields", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR},     
    //SDP_ABNF_RULE_PHONE_FIELDS,
    {"phone-fields", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR},     
    //SDP_ABNF_RULE_CONNECTION_FIELD,
    {"connection-field", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR},     
    //SDP_ABNF_RULE_BANDWIDTH_FIELDS,
    {"bandwidth-fields", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR},     
    //SDP_ABNF_RULE_TIME_FIELDS,
    {"time-fields", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR},    
    //SDP_ABNF_RULE_REPEAT_FIELDS,
    {"repeat-fields", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR},    
    //SDP_ABNF_RULE_ZONE_ADJUSTMENTS,
    {"zone-adjustments", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR},    
    //SDP_ABNF_RULE_KEY_FIELD,
    {"key-field", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR},    
    //SDP_ABNF_RULE_ATTRIBUTE_FIELDS,
    {"attribute-fields", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR},    
    //SDP_ABNF_RULE_MEDIA_DESCRIPTIONS,
    {"media-descriptions", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR},    
    //SDP_ABNF_RULE_MEDIA_FIELD,
    {"media-field", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_USERNAME,
    {"username", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_SESS_ID,
    {"sess-id", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_SESS_VERSION,
    {"sess-version", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_NETTYPE,
    {"nettype", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_ADDRTYPE,
    {"addrtype", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_URI,
    {"uri", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_EMAIL_ADDRESS,
    {"email-address", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_ADDRESS_AND_COMMENT,
    {"address-and-comment", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_DISPNAME_AND_ADDRESS,
    {"dispname-and-address", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_PHONE_NUMBER,
    {"phone-number", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_PHONE,
    {"phone", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_CONNECTION_ADDRESS,
    {"connection-address", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_BWTYPE,
    {"bwtype", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_BANDWIDTH,
    {"bandwidth", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_START_TIME,
    {"start-time", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_STOP_TIME,
    {"stop-time", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_TIME,
    {"time", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_REPEAT_INTERVAL,
    {"repeat-interval", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_TYPED_TIME,
    {"typed-time", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_FIXED_LEN_TIME_UNIT,
    {"fixed-len-time-unit", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_KEY_TYPE,
    {"key-type", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_BASE64,
    {"base64", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_BASE64_UNIT,
    {"base64-unit", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_BASE64_PAD,
    {"base64-pad", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_BASE64_CHAR,
    {"base64-char", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_ATTRIBUTE,
    {"attribute", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_ATT_FIELD,
    {"att-field", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_ATT_VALUE,
    {"att-value", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_MEDIA,
    {"media", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_FMT,
    {"fmt", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_PROTO,
    {"proto", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_PORT,
    {"port", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_UNICAST_ADDRESS,
    {"unicast-address", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR},             
    //SDP_ABNF_RULE_MULTICAST_ADDRESS,
    {"multicast-address", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_IP4_MULTICAST,
    {"IP4-multicast", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_M1,
    {"m1", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_IP6_MULTICAST,
    {"IP6-multicast", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_TTL,
    {"ttl", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_FQDN,
    {"FQDN", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_IP4_ADDRESS,
    {"IP4-address", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_B1,
    {"b1", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_IP6_ADDRESS,
    {"IP6-address", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_HEXPART,
    {"hexpart", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_HEXSEQ,
    {"hexseq", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_HEX4,
    {"hex4", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_EXTN_ADDR,
    {"extn-addr", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_TEXT,
    {"text", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_BYTE_STRING,
    {"byte-string", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_NON_WS_STRING,
    {"non-ws-string", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_TOKEN_CHAR,
    {"token-char", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_TOKEN,
    {"token", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_EMAIL_SAFE,
    {"email-safe", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_INTEGER,
    {"integer", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_ALPHA_NUMERIC,
    {"alpha-numeric", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_POS_DIGIT,
    {"POS-DIGIT", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR}, 
    //SDP_ABNF_RULE_DECIMAL_UCHAR,
    {"decimal-uchar", NULL_ULONG, NULL_PTR, NULL_PTR, NULL_PTR},  
};
