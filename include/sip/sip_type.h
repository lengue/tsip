/*******************************************************************************
功能    : SIP协议栈对外提供的结构
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/
/*SIP请求消息的请求行*/
typedef struct tagSIP_REQUEST_LINE_S
{
    SIP_METHOD_E eMethod;
    URI_S       *pstRequestURI;
    UCHAR        ucVersion;
}SIP_REQUEST_LINE_S;

/*SIP响应消息的状态行*/
typedef struct tagSIP_STATUS_LINE_S
{
    UCHAR ucVersion;
    SIP_STATUS_CODE_E eStatusCode;
}SIP_STATUS_LINE_S;

/* 头域的公共结构 */
typedef struct tagSIP_HEADER_S
{
    struct tagSIP_HEADER_S *pstNext;    /* 下一个头域值 */
}SIP_HEADER_S;

/* 消息体结构，适用于多BODY */
typedef struct tagSIP_BODY_S
{
    struct tagSIP_BODY_S *pstNext;     /* 下一个消息体SIP_BODY_S*/
    VOID *pstContentType;    /* 这些头域可以没有 */
    VOID *pstContentLanguage;
    VOID *pstContentDisposite;
    VOID *pstContentLength;
    UCHAR    pstBody[0];        /* 消息体结构 */
}SIP_BODY_S;


/* SIP消息结构 */
typedef struct tagSIP_MSG_S
{
    SIP_MSG_TYPE_E eMsgType;
    union
    {
        SIP_REQUEST_LINE_S  stRequstLine;
        SIP_STATUS_LINE_S   stStatusLine;
    }uStartLine;

    /* 消息体指针在Conent-Type头域的指针中指定，不单独列出 */
    SIP_HEADER_S *apstHeaders[SIP_HEADER_BUTT];

    SIP_BODY_S   *pstBodys;
}SIP_MSG_S;



/* 所有头域依赖的结构 */
typedef struct tagSIP_NAME_ADDR_S
{
    BOOL     bName;     /* 是否带<>符号 */
    UCHAR   *pucName;
    URI_S   *pstUri;
}SIP_NAME_ADDR_S;

typedef struct tagSIP_VIA_RECEIVED_S
{
    SIP_IP_TYPE_E eIpType;
    union
    {
        UCHAR *pucIPV4; /* char */
        UCHAR *pucIPV6; /* char */
    }u;
}SIP_VIA_RECEIVED_S;

typedef struct tagSIP_VIA_PARM_S
{
    SIP_TRANSPORT_PROTOCOL_E  eProtocolType;
    URI_HOST_PORT_S           stSendBy;

    /* 参数 */
    ULONG                     ulTtl;
    UCHAR                    *pucBranch;
    URI_HOST_S               *pstMaddr;
    SIP_VIA_RECEIVED_S       *pstReceived;
    struct tagSIP_VIA_PARM_S *pstNext;
}SIP_VIA_PARM_S;

typedef struct tagSIP_GENERIC_PARA_S
{
    UCHAR *pucToken;
    UCHAR *pucValue;
    struct tagSIP_GENERIC_PARA_S *pstNext;
}SIP_GENERIC_PARA_S;

typedef struct tagSIP_ROUTE_PARAM_S
{
    SIP_NAME_ADDR_S     stNameAddr;
    UCHAR               ucLr;
    SIP_GENERIC_PARA_S *pstPara;
    struct tagSIP_ROUTE_PARAM_S  *pstNext;
}SIP_ROUTE_PARAM_S;


/* 所有头域的结构 */
typedef struct tagSIP_HEADER_ACCEPT_S
{
    SIP_HEADER_S stHeader;
    UCHAR *pucTemp;
}SIP_HEADER_ACCEPT_S;

typedef struct tagSIP_HEADER_ACCEPT_ENCODING_S
{
    SIP_HEADER_S stHeader;
    UCHAR *pucTemp;
}SIP_HEADER_ACCEPT_ENCODING_S;

typedef struct tagSIP_HEADER_ACCEPT_LANGUAGE_S
{
    SIP_HEADER_S stHeader;
    UCHAR *pucTemp;
}SIP_HEADER_ACCEPT_LANGUAGE_S;

typedef struct tagSIP_HEADER_ALERT_INFO_S
{
    SIP_HEADER_S stHeader;
    UCHAR *pucTemp;
}SIP_HEADER_ALERT_INFO_S;

typedef struct tagSIP_HEADER_ALLOW_S
{
    SIP_HEADER_S stHeader;
    UCHAR *pucTemp;
}SIP_HEADER_ALLOW_S;

typedef struct tagSIP_HEADER_AUTHENTICATICATION_INFO_S
{
    SIP_HEADER_S stHeader;
    UCHAR *pucTemp;
}SIP_HEADER_AUTHENTICATICATION_INFO_S;

typedef struct tagSIP_HEADER_AUTHORIZATION_S
{
    SIP_HEADER_S stHeader;
    UCHAR *pucTemp;
}SIP_HEADER_AUTHORIZATION_S;

typedef struct tagSIP_HEADER_CALL_ID_S
{
    SIP_HEADER_S stHeader;
    UCHAR *pucCallID;
}SIP_HEADER_CALL_ID_S;

typedef struct tagSIP_HEADER_CALL_INFO_S
{
    SIP_HEADER_S stHeader;
    UCHAR *pucTemp;
}SIP_HEADER_CALL_INFO_S;

typedef struct tagSIP_HEADER_CONTACT_S
{
    SIP_HEADER_S stHeader;
    UCHAR *pucTemp;
}SIP_HEADER_CONTACT_S;

typedef struct tagSIP_HEADER_CONTENT_DISPOSITION_S
{
    SIP_HEADER_S stHeader;
    UCHAR *pucTemp;
}SIP_HEADER_CONTENT_DISPOSITION_S;

typedef struct tagSIP_HEADER_CONTENT_ENCODING_S
{
    SIP_HEADER_S stHeader;
    UCHAR *pucTemp;
}SIP_HEADER_CONTENT_ENCODING_S;

typedef struct tagSIP_HEADER_CONTENT_LANGUAGE_S
{
    SIP_HEADER_S stHeader;
    UCHAR *pucTemp;
}SIP_HEADER_CONTENT_LANGUAGE_S;

typedef struct tagSIP_HEADER_CONTENT_LENTH_S
{
    SIP_HEADER_S stHeader;
    UCHAR *pucTemp;
}SIP_HEADER_CONTENT_LENTH_S;

typedef struct tagSIP_HEADER_CONTENT_TYPE_S
{
    SIP_HEADER_S stHeader;
    UCHAR *pucTemp;
}SIP_HEADER_CONTENT_TYPE_S;

typedef struct tagSIP_HEADER_CSEQ_S
{
    SIP_HEADER_S stHeader;
    ULONG        ulSeq;
    SIP_METHOD_E eMethod;
}SIP_HEADER_CSEQ_S;

typedef struct tagSIP_HEADER_DATA_S
{
    SIP_HEADER_S stHeader;
    UCHAR *pucTemp;
}SIP_HEADER_DATA_S;

typedef struct tagSIP_HEADER_ERROR_INFO_S
{
    SIP_HEADER_S stHeader;
    UCHAR *pucTemp;
}SIP_HEADER_ERROR_INFO_S;

typedef struct tagSIP_HEADER_EXPIRES_S
{
    SIP_HEADER_S stHeader;
    UCHAR *pucTemp;
}SIP_HEADER_EXPIRES_S;

typedef struct tagSIP_HEADER_FROM_S
{
    SIP_HEADER_S stHeader;
    SIP_NAME_ADDR_S stNameAddr;
    UCHAR          *pucTag;/*UCHAR*/
}SIP_HEADER_FROM_S;

typedef struct tagSIP_HEADER_IN_REPLY_TO_S
{
    SIP_HEADER_S stHeader;
    UCHAR *pucTemp;
}SIP_HEADER_IN_REPLY_TO_S;

typedef struct tagSIP_HEADER_MAX_FORWARDS_S
{
    SIP_HEADER_S stHeader;
    ULONG ulMaxForwards;
}SIP_HEADER_MAX_FORWARDS_S;

typedef struct tagSIP_HEADER_MIN_EXPIRES_S
{
    SIP_HEADER_S stHeader;
    UCHAR *pucTemp;
}SIP_HEADER_MIN_EXPIRES_S;

typedef struct tagSIP_HEADER_MIME_VERSION_S
{
    SIP_HEADER_S stHeader;
    UCHAR *pucTemp;
}SIP_HEADER_MIME_VERSION_S;

typedef struct tagSIP_HEADER_ORGANIZATION_S
{
    SIP_HEADER_S stHeader;
    UCHAR *pucTemp;
}SIP_HEADER_ORGANIZATION_S;

typedef struct tagSIP_HEADER_PRIORITY_S
{
    SIP_HEADER_S stHeader;
    UCHAR *pucTemp;
}SIP_HEADER_PRIORITY_S;

typedef struct tagSIP_HEADER_PROXY_AUTHENTICATE_S
{
    SIP_HEADER_S stHeader;
    UCHAR *pucTemp;
}SIP_HEADER_PROXY_AUTHENTICATE_S;

typedef struct tagSIP_HEADER_PROXY_AUTHORIZATION_S
{
    SIP_HEADER_S stHeader;
    UCHAR *pucTemp;
}SIP_HEADER_PROXY_AUTHORIZATION_S;

typedef struct tagSIP_HEADER_PROXY_REQUIRE_S
{
    SIP_HEADER_S stHeader;
    UCHAR *pucTemp;
}SIP_HEADER_PROXY_REQUIRE_S;

typedef struct tagSIP_HEADER_RECORD_ROUTE_S
{
    SIP_HEADER_S stHeader;
    UCHAR *pucTemp;
}SIP_HEADER_RECORD_ROUTE_S;

typedef struct tagSIP_HEADER_REPLY_TO_S
{
    SIP_HEADER_S stHeader;
    UCHAR *pucTemp;
}SIP_HEADER_REPLY_TO_S;

typedef struct tagSIP_HEADER_REQUIRE_S
{
    SIP_HEADER_S stHeader;
    UCHAR *pucTemp;
}SIP_HEADER_REQUIRE_S;

typedef struct tagSIP_HEADER_RETRY_AFTER_S
{
    SIP_HEADER_S stHeader;
    UCHAR *pucTemp;
}SIP_HEADER_RETRY_AFTER_S;

typedef struct tagSIP_HEADER_ROUTE_S
{
    SIP_HEADER_S stHeader;
    SIP_ROUTE_PARAM_S *pstRouteParam;
}SIP_HEADER_ROUTE_S;

typedef struct tagSIP_HEADER_SERVER_S
{
    SIP_HEADER_S stHeader;
    UCHAR *pucTemp;
}SIP_HEADER_SERVER_S;

typedef struct tagSIP_HEADER_SUBJECT_S
{
    SIP_HEADER_S stHeader;
    UCHAR *pucTemp;
}SIP_HEADER_SUBJECT_S;

typedef struct tagSIP_HEADER_SUPPORTED_S
{
    SIP_HEADER_S stHeader;
    UCHAR *pucTemp;
}SIP_HEADER_SUPPORTED_S;

typedef struct tagSIP_HEADER_TIMESTAMP_S
{
    SIP_HEADER_S stHeader;
    UCHAR *pucTemp;
}SIP_HEADER_TIMESTAMP_S;

typedef struct tagSIP_HEADER_TO_S
{
    SIP_HEADER_S stHeader;
    SIP_NAME_ADDR_S stNameAddr;
    UCHAR          *pucTag;
}SIP_HEADER_TO_S;

typedef struct tagSIP_HEADER_UNSUPPORTED_S
{
    SIP_HEADER_S stHeader;
    UCHAR *pucTemp;
}SIP_HEADER_UNSUPPORTED_S;

typedef struct tagSIP_HEADER_USER_AGENT_S
{
    SIP_HEADER_S stHeader;
    UCHAR *pucTemp;
}SIP_HEADER_USER_AGENT_S;

typedef struct tagSIP_HEADER_VIA_S
{
    SIP_HEADER_S stHeader;
    SIP_VIA_PARM_S *pstViaParm;
}SIP_HEADER_VIA_S;

typedef struct tagSIP_HEADER_WARNING_S
{
    SIP_HEADER_S stHeader;
    UCHAR *pucTemp;
}SIP_HEADER_WARNING_S;

typedef struct tagSIP_HEADER_WWW_AUTHENTICATE_S
{
    SIP_HEADER_S stHeader;
    UCHAR *pucTemp;
}SIP_HEADER_WWW_AUTHENTICATE_S;

typedef struct tagSIP_LOCATION_S
{
    SIP_TRANSPORT_PROTOCOL_E eProtocol;
    UCHAR  aucIPStr[SIP_MAX_IP_STR_LEN];/*IPV4或者IPV6的地址字符串 SIP_IP_TYPE_E*/
    USHORT usPort;
}SIP_LOCATION_S;

typedef ULONG (*SIP_START_TIMER_PROC)(ULONG ulName,
                                      ULONG ulLen,
                                      ULONG ulPara,
                                      SIP_TIMER_TYPE_E eType,
                                      ULONG *pulHandle);
typedef VOID (*SIP_STOP_TIMER_PROC)(ULONG ulHandle);
typedef ULONG (*SIP_SEND_UP_REQUEST_MSG_PROC)(ULONG ulDlgID,
                                              ULONG ulUasID,
                                              UBUF_HEADER_S * pstUbufSipMsg);
typedef ULONG (*SIP_SEND_UP_RESPONSE_MSG_PROC)(ULONG ulAppID,
                                               ULONG ulDlgID,
                                               UBUF_HEADER_S * pstUbufSipMsg);
typedef ULONG (*SIP_SEND_DOWN_MSG_PROC)(UBUF_HEADER_S  *pstSipMsgUbuf,
                                        SIP_LOCATION_S *pstPeerLocation);

typedef ULONG (*SIP_GENERATE_RANDOM_STRING_PROC)(CHAR *pcBuffer,
                                                 ULONG ulBufferLen);

typedef struct tagSIP_SYNTAX_CFG_S
{
    UCHAR *pucAbnfDes; /* ABNF语法描述字符串 */
    ULONG  ulLen;      /* ABNF语法描述长度 */
}SIP_SYNTAX_CFG_S;

typedef struct tagSIP_TXP_CFG_S
{
    UCHAR *pucSendby; /* send-by字段填写内容 */
}SIP_TXP_CFG_S;

typedef struct tagSIP_TU_CFG_S
{
    SIP_CORE_TYPE_E eType;
}SIP_TU_CFG_S;

typedef struct tagSIP_TXN_CFG_S
{
    ULONG ulTimerT1;
}SIP_TXN_CFG_S;

typedef struct tagSIP_SHELL_CFG_S
{
    SIP_START_TIMER_PROC   pfnStartTimer;
    SIP_STOP_TIMER_PROC    pfnStopTimer;
    SIP_SEND_UP_REQUEST_MSG_PROC   pfnSendUpRequestMsg;
    SIP_SEND_UP_RESPONSE_MSG_PROC  pfnSendUpResponseMsg;
    SIP_SEND_DOWN_MSG_PROC pfnSendDownMsg;
    SIP_GENERATE_RANDOM_STRING_PROC pfnGenerateRandom;
}SIP_SHELL_CFG_S;

/* 产品化函数钩子 */
typedef struct tagSIP_CFG_S
{
    SIP_TU_CFG_S     stTuCfg;
    SIP_TXN_CFG_S    stTxnCfg;
    SIP_TXP_CFG_S    stTxpCfg;
    SIP_SYNTAX_CFG_S stSyntaxCfg;
    SIP_SHELL_CFG_S  stShellCfg;
}SIP_CFG_S;
