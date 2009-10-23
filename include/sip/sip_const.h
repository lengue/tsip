/*******************************************************************************
功能    : SIP协议栈对外提供的常量
创建人  : 唐春平
创建日期: 2009.01.10
修改记录:
*******************************************************************************/
#define SIP_MAX_UBUF_MSG_LEN 4000
#define SIP_MAX_TEXT_MSG_LEN 4000

/* IP地址值字符串最大长度，按照IPV6来计算 */
#define SIP_MAX_IP_STR_LEN   32

/*SIP消息类型*/
typedef enum tagSIP_MSG_TYPE_E
{
    SIP_MSG_TYPE_REQUEST,  /* 请求 */
    SIP_MSG_TYPE_RESPONSE, /* 响应 */
    SIP_MSG_TYPE_BUTT
}SIP_MSG_TYPE_E;

/* SIP协议头域枚举 */
typedef enum tagSIP_HEADER_E
{
    SIP_HEADER_ACCEPT,
    SIP_HEADER_ACCEPT_ENCODING,
    SIP_HEADER_ACCEPT_LANGUAGE,
    SIP_HEADER_ALERT_INFO,
    SIP_HEADER_ALLOW,
    SIP_HEADER_AUTHENTICATICATION_INFO,
    SIP_HEADER_AUTHORIZATION,
    SIP_HEADER_CALL_ID,
    SIP_HEADER_CALL_INFO,
    SIP_HEADER_CONTACT,
    SIP_HEADER_CONTENT_DISPOSITION,
    SIP_HEADER_CONTENT_ENCODING,
    SIP_HEADER_CONTENT_LANGUAGE,
    SIP_HEADER_CONTENT_LENTH,
    SIP_HEADER_CONTENT_TYPE,
    SIP_HEADER_CSEQ,
    SIP_HEADER_DATA,
    SIP_HEADER_ERROR_INFO,
    SIP_HEADER_EXPIRES,
    SIP_HEADER_FROM,
    SIP_HEADER_IN_REPLY_TO,
    SIP_HEADER_MAX_FORWARDS,
    SIP_HEADER_MIN_EXPIRES,
    SIP_HEADER_MIME_VERSION,
    SIP_HEADER_ORGANIZATION,
    SIP_HEADER_PRIORITY,
    SIP_HEADER_PROXY_AUTHENTICATE,
    SIP_HEADER_PROXY_AUTHORIZATION,
    SIP_HEADER_PROXY_REQUIRE,
    SIP_HEADER_RECORD_ROUTE,
    SIP_HEADER_REPLY_TO,
    SIP_HEADER_REQUIRE,
    SIP_HEADER_RETRY_AFTER,
    SIP_HEADER_ROUTE,
    SIP_HEADER_SERVER,
    SIP_HEADER_SUBJECT,
    SIP_HEADER_SUPPORTED,
    SIP_HEADER_TIMESTAMP,
    SIP_HEADER_TO,
    SIP_HEADER_UNSUPPORTED,
    SIP_HEADER_USER_AGENT,
    SIP_HEADER_VIA,
    SIP_HEADER_WARNING,
    SIP_HEADER_WWW_AUTHENTICATE,

    SIP_HEADER_BUTT
}SIP_HEADER_E;

/* SIP请求方法 */
typedef enum tagSIP_METHOD_E
{
    SIP_METHOD_INVITE,
    SIP_METHOD_ACK,
    SIP_METHOD_OPTIONS,
    SIP_METHOD_BYE,
    SIP_METHOD_CANCEL,
    SIP_METHOD_REGISTER,

    SIP_METHOD_BUTT
}SIP_METHOD_E;

/* SIP响应码 */
typedef enum tagSIP_STATUS_CODE_E
{
    /*Provisional 1xx*/
    SIP_STATUS_CODE_100=100, //"Trying"
    SIP_STATUS_CODE_180=180, //"Ringing"
    SIP_STATUS_CODE_181=181, //"Call Is Being Forwarded"
    SIP_STATUS_CODE_182=182, //"Queued"
    SIP_STATUS_CODE_183=183, //"Session Progress"

    /*Successful 2xx*/
    SIP_STATUS_CODE_200=200, //"OK"

    /*Redirection 3xx*/
    SIP_STATUS_CODE_300=300, //"Multiple Choices"
    SIP_STATUS_CODE_301=301, //"Moved Permanently"
    SIP_STATUS_CODE_302=302, //"Moved Temporarily"
    SIP_STATUS_CODE_305=305, //"Use Proxy"
    SIP_STATUS_CODE_380=380, //"Alternative Service"

    /*Request Failure 4xx*/
    SIP_STATUS_CODE_400=400, //"Bad Request"
    SIP_STATUS_CODE_401=401, //"Unauthorized"
    SIP_STATUS_CODE_402=402, //"Payment Required"
    SIP_STATUS_CODE_403=403, //"Forbidden"
    SIP_STATUS_CODE_404=404, //"Not Found"
    SIP_STATUS_CODE_405=405, //"Method Not Allowed"
    SIP_STATUS_CODE_406=406, //"Not Acceptable"
    SIP_STATUS_CODE_407=407, //"Proxy Authentication Required"
    SIP_STATUS_CODE_408=408, //"Request Timeout"
    SIP_STATUS_CODE_410=410, //"Gone"
    SIP_STATUS_CODE_413=413, //"Request Entity Too Large"
    SIP_STATUS_CODE_414=414, //"Request-URI Too Long"
    SIP_STATUS_CODE_415=415, //"Unsupported Media Type"
    SIP_STATUS_CODE_416=416, //"Unsupported URI Scheme"
    SIP_STATUS_CODE_420=420, //"Bad Extensione"
    SIP_STATUS_CODE_421=421, //"Extension Required"
    SIP_STATUS_CODE_423=423, //"Interval Too Brief"
    SIP_STATUS_CODE_480=480, //"Temporarily Unavailable"
    SIP_STATUS_CODE_481=481, //"Call/Transaction Does Not Exist"
    SIP_STATUS_CODE_482=482, //"Loop Detected"
    SIP_STATUS_CODE_483=483, //"Too Many Hops"
    SIP_STATUS_CODE_484=484, //"Address Incomplete"
    SIP_STATUS_CODE_485=485, //"Ambiguous"
    SIP_STATUS_CODE_486=486, //"Busy Here"
    SIP_STATUS_CODE_487=487, //"Request Terminated"
    SIP_STATUS_CODE_488=488, //"Not Acceptable Here"
    SIP_STATUS_CODE_491=491, //"Request Pending"
    SIP_STATUS_CODE_493=493, //"Undecipherable"

    /*Server Failure 5xx*/
    SIP_STATUS_CODE_500=500, //"Server Internal Error"
    SIP_STATUS_CODE_501=501, //"Not Implemented"
    SIP_STATUS_CODE_502=502, //"Bad Gateway"
    SIP_STATUS_CODE_503=503, //"Service Unavailable"
    SIP_STATUS_CODE_504=504, //"Server Time-out"
    SIP_STATUS_CODE_505=505, //"Version Not Supported"
    SIP_STATUS_CODE_513=513, //"Message Too Large"

    /*Global Failures 6xx*/
    SIP_STATUS_CODE_600=600, //"Busy Everywhere"
    SIP_STATUS_CODE_603=603, //"Decline"
    SIP_STATUS_CODE_604=604, //"Does Not Exist Anywhere"
    SIP_STATUS_CODE_606=606, //"Not Acceptable"

    SIP_STATUS_CODE_BUTT
}SIP_STATUS_CODE_E;

typedef enum tagSIP_TRANSPORT_PROTOCOL_E
{
    SIP_TRANSPORT_PROTOCOL_TCP,
    SIP_TRANSPORT_PROTOCOL_TLS,
    SIP_TRANSPORT_PROTOCOL_UDP,
    SIP_TRANSPORT_PROTOCOL_SCTP,
    SIP_TRANSPORT_PROTOCOL_BUTT
}SIP_TRANSPORT_PROTOCOL_E;

typedef enum tagSIP_IP_TYPE_E
{
    SIP_IP_TYPE_IPV4,
    SIP_IP_TYPE_IPV6,
    SIP_IP_TYPE_BUTT
}SIP_IP_TYPE_E;

typedef enum tagSIP_TIMER_TYPE_E
{
    SIP_TIMER_LOOP,
    SIP_TIMER_NO_LOOP,
    SIP_TIMER_BUTT
}SIP_TIMER_TYPE_E;

/*SIP Core类型*/
typedef enum tagSIP_CORE_TYPE_E
{
    SIP_CORE_UA,  /* UAC&UAS */
    SIP_CORE_REGISTAR,
    SIP_CORE_STATEFULL_PROXY,
    SIP_CORE_STATELESS_PROXY,
    SIP_CORE_TYPE_BUTT
}SIP_CORE_TYPE_E;