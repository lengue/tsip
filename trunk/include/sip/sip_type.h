/*******************************************************************************
����    : SIPЭ��ջ�����ṩ�Ľṹ
������  : �ƴ�ƽ
��������: 200.01.10
�޸ļ�¼:
*******************************************************************************/
/*SIP������Ϣ��������*/
typedef struct tagSIP_REQUEST_LINE_S
{
    SIP_METHOD_E eMethod;
    UBUF_PTR     upRequestURI; /* URI_S */
    UCHAR ucVersion;
}SIP_REQUEST_LINE_S;

/*SIP��Ӧ��Ϣ��״̬��*/
typedef struct tagSIP_STATUS_LINE_S
{
    UCHAR ucVersion;
    SIP_STATUS_CODE_E eStatusCode;
}SIP_STATUS_LINE_S;

typedef struct tagSIP_HEADER_S
{
    UBUF_PTR upstNext;     /* ��һ��ͷ��ֵ */
    UCHAR    pstSpec[0];  /* ���ݾ���ṹ�仯������Ҳ�� */
}SIP_HEADER_S;

/* ��Ϣ��ṹ�������ڶ�BODY */
typedef struct tagSIP_BODY_S
{
    UBUF_PTR upstNext;           /* ��һ����Ϣ��SIP_BODY_S*/
    UBUF_PTR upstContentType;    /* ��Щͷ�����û�� */
    UBUF_PTR upstContentLanguage;
    UBUF_PTR upstContentDisposite;
    UBUF_PTR upstContentLength;
    UCHAR    pstBody[0];        /* ��Ϣ��ṹ */
}SIP_BODY_S;


/* SIP��Ϣ�ṹ */
typedef struct tagSIP_MSG_S
{
    SIP_MSG_TYPE_E eMsgType;
    union
    {
        SIP_REQUEST_LINE_S  stRequstLine;
        SIP_STATUS_LINE_S   stStatusLine;
    }uStartLine;

    /* ��Ϣ��ָ����Conent-Typeͷ���ָ����ָ�����������г� */
    UBUF_PTR aupstHeaders[SIP_HEADER_BUTT]; /* SIP_HEADER_S */

    UBUF_PTR upstBodys; /* SIP_BODY_S */
}SIP_MSG_S;



/* ����ͷ�������Ľṹ */
typedef struct tagSIP_NAME_ADDR_S
{
    BOOL     bName;     /* �Ƿ��<>���� */
    UBUF_PTR upucName;  /* char */
    UBUF_PTR upstUri;   /* URI_S */
}SIP_NAME_ADDR_S;

typedef struct tagSIP_VIA_RECEIVED_S
{
    SIP_IP_TYPE_E eIpType;
    union
    {
        UBUF_PTR upucIPV4; /* char */
        UBUF_PTR upucIPV6; /* char */
    }u;
}SIP_VIA_RECEIVED_S;

typedef struct tagSIP_VIA_PARM_S
{
    SIP_TRANSPORT_PROTOCOL_E  eProtocolType;
    URI_HOST_PORT_S           stSendBy;

    /* ���� */
    ULONG                     ulTtl;
    UBUF_PTR                  upucBranch;   /* UCHAR */
    UBUF_PTR                  upstMaddr;    /* URI_HOST_S */
    UBUF_PTR                  upstReceived; /* SIP_VIA_RECEIVED_S */

    UBUF_PTR                  upstNext;     /* SIP_VIA_PARM_S */
}SIP_VIA_PARM_S;

typedef struct tagSIP_ROUTE_PARAM_S
{
    SIP_NAME_ADDR_S stNameAddr;
    UCHAR           ucLr;
    UBUF_PTR        upPara;  /* SIP_GENERIC_PARA_S */
    UBUF_PTR        upstNext;/* SIP_ROUTE_PARAM_S */
}SIP_ROUTE_PARAM_S;

typedef struct tagSIP_GENERIC_PARA_S
{
    UBUF_PTR upucToken;
    UBUF_PTR upucValue;
    UBUF_PTR upstNext; /* SIP_GENERIC_PARA_S */
}SIP_GENERIC_PARA_S;

/* ����ͷ��Ľṹ */
typedef struct tagSIP_HEADER_ACCEPT_S
{
    UBUF_PTR upucTemp;/*UCHAR*/
}SIP_HEADER_ACCEPT_S;

typedef struct tagSIP_HEADER_ACCEPT_ENCODING_S
{
    UBUF_PTR upucTemp;/*UCHAR*/
}SIP_HEADER_ACCEPT_ENCODING_S;

typedef struct tagSIP_HEADER_ACCEPT_LANGUAGE_S
{
    UBUF_PTR upucTemp;/*UCHAR*/
}SIP_HEADER_ACCEPT_LANGUAGE_S;

typedef struct tagSIP_HEADER_ALERT_INFO_S
{
    UBUF_PTR upucTemp;/*UCHAR*/
}SIP_HEADER_ALERT_INFO_S;

typedef struct tagSIP_HEADER_ALLOW_S
{
    UBUF_PTR upucTemp;/*UCHAR*/
}SIP_HEADER_ALLOW_S;

typedef struct tagSIP_HEADER_AUTHENTICATICATION_INFO_S
{
    UBUF_PTR upucTemp;/*UCHAR*/
}SIP_HEADER_AUTHENTICATICATION_INFO_S;

typedef struct tagSIP_HEADER_AUTHORIZATION_S
{
    UBUF_PTR upucTemp;/*UCHAR*/
}SIP_HEADER_AUTHORIZATION_S;

typedef struct tagSIP_HEADER_CALL_ID_S
{
    UBUF_PTR upucCallID;/*UCHAR*/
}SIP_HEADER_CALL_ID_S;

typedef struct tagSIP_HEADER_CALL_INFO_S
{
    UBUF_PTR upucTemp;/*UCHAR*/
}SIP_HEADER_CALL_INFO_S;

typedef struct tagSIP_HEADER_CONTACT_S
{
    UBUF_PTR upucTemp;/*UCHAR*/
}SIP_HEADER_CONTACT_S;

typedef struct tagSIP_HEADER_CONTENT_DISPOSITION_S
{
    UBUF_PTR upucTemp;/*UCHAR*/
}SIP_HEADER_CONTENT_DISPOSITION_S;

typedef struct tagSIP_HEADER_CONTENT_ENCODING_S
{
    UBUF_PTR upucTemp;/*UCHAR*/
}SIP_HEADER_CONTENT_ENCODING_S;

typedef struct tagSIP_HEADER_CONTENT_LANGUAGE_S
{
    UBUF_PTR upucTemp;/*UCHAR*/
}SIP_HEADER_CONTENT_LANGUAGE_S;

typedef struct tagSIP_HEADER_CONTENT_LENTH_S
{
    UBUF_PTR upucTemp;/*UCHAR*/
}SIP_HEADER_CONTENT_LENTH_S;

typedef struct tagSIP_HEADER_CONTENT_TYPE_S
{
    UBUF_PTR upucTemp;/*UCHAR*/
}SIP_HEADER_CONTENT_TYPE_S;

typedef struct tagSIP_HEADER_CSEQ_S
{
    ULONG        ulSeq;
    SIP_METHOD_E eMethod;
}SIP_HEADER_CSEQ_S;

typedef struct tagSIP_HEADER_DATA_S
{
    UBUF_PTR upucTemp;/*UCHAR*/
}SIP_HEADER_DATA_S;

typedef struct tagSIP_HEADER_ERROR_INFO_S
{
    UBUF_PTR upucTemp;/*UCHAR*/
}SIP_HEADER_ERROR_INFO_S;

typedef struct tagSIP_HEADER_EXPIRES_S
{
    UBUF_PTR upucTemp;/*UCHAR*/
}SIP_HEADER_EXPIRES_S;

typedef struct tagSIP_HEADER_FROM_S
{
    SIP_NAME_ADDR_S stNameAddr;
    UBUF_PTR        upucTag;/*UCHAR*/
}SIP_HEADER_FROM_S;

typedef struct tagSIP_HEADER_IN_REPLY_TO_S
{
    UBUF_PTR upucTemp;/*UCHAR*/
}SIP_HEADER_IN_REPLY_TO_S;

typedef struct tagSIP_HEADER_MAX_FORWARDS_S
{
    ULONG ulMaxForwards;
}SIP_HEADER_MAX_FORWARDS_S;

typedef struct tagSIP_HEADER_MIN_EXPIRES_S
{
    UBUF_PTR upucTemp;/*UCHAR*/
}SIP_HEADER_MIN_EXPIRES_S;

typedef struct tagSIP_HEADER_MIME_VERSION_S
{
    UBUF_PTR upucTemp;/*UCHAR*/
}SIP_HEADER_MIME_VERSION_S;

typedef struct tagSIP_HEADER_ORGANIZATION_S
{
    UBUF_PTR upucTemp;/*UCHAR*/
}SIP_HEADER_ORGANIZATION_S;

typedef struct tagSIP_HEADER_PRIORITY_S
{
    UBUF_PTR upucTemp;/*UCHAR*/
}SIP_HEADER_PRIORITY_S;

typedef struct tagSIP_HEADER_PROXY_AUTHENTICATE_S
{
    UBUF_PTR upucTemp;/*UCHAR*/
}SIP_HEADER_PROXY_AUTHENTICATE_S;

typedef struct tagSIP_HEADER_PROXY_AUTHORIZATION_S
{
    UBUF_PTR upucTemp;/*UCHAR*/
}SIP_HEADER_PROXY_AUTHORIZATION_S;

typedef struct tagSIP_HEADER_PROXY_REQUIRE_S
{
    UBUF_PTR upucTemp;/*UCHAR*/
}SIP_HEADER_PROXY_REQUIRE_S;

typedef struct tagSIP_HEADER_RECORD_ROUTE_S
{
    UBUF_PTR upucTemp;/*UCHAR*/
}SIP_HEADER_RECORD_ROUTE_S;

typedef struct tagSIP_HEADER_REPLY_TO_S
{
    UBUF_PTR upucTemp;/*UCHAR*/
}SIP_HEADER_REPLY_TO_S;

typedef struct tagSIP_HEADER_REQUIRE_S
{
    UBUF_PTR upucTemp;/*UCHAR*/
}SIP_HEADER_REQUIRE_S;

typedef struct tagSIP_HEADER_RETRY_AFTER_S
{
    UBUF_PTR upucTemp;/*UCHAR*/
}SIP_HEADER_RETRY_AFTER_S;

typedef struct tagSIP_HEADER_ROUTE_S
{
    UBUF_PTR          upstRouteParam; /* SIP_ROUTE_PARAM_S */
}SIP_HEADER_ROUTE_S;

typedef struct tagSIP_HEADER_SERVER_S
{
    UBUF_PTR upucTemp;/*UCHAR*/
}SIP_HEADER_SERVER_S;

typedef struct tagSIP_HEADER_SUBJECT_S
{
    UBUF_PTR upucTemp;/*UCHAR*/
}SIP_HEADER_SUBJECT_S;

typedef struct tagSIP_HEADER_SUPPORTED_S
{
    UBUF_PTR upucTemp;/*UCHAR*/
}SIP_HEADER_SUPPORTED_S;

typedef struct tagSIP_HEADER_TIMESTAMP_S
{
    UBUF_PTR upucTemp;/*UCHAR*/
}SIP_HEADER_TIMESTAMP_S;

typedef struct tagSIP_HEADER_TO_S
{
    SIP_NAME_ADDR_S stNameAddr;
    UBUF_PTR        upucTag;/*UCHAR*/
}SIP_HEADER_TO_S;

typedef struct tagSIP_HEADER_UNSUPPORTED_S
{
    UBUF_PTR upucTemp;/*UCHAR*/
}SIP_HEADER_UNSUPPORTED_S;

typedef struct tagSIP_HEADER_USER_AGENT_S
{
    UBUF_PTR upucTemp;/*UCHAR*/
}SIP_HEADER_USER_AGENT_S;

typedef struct tagSIP_HEADER_VIA_S
{
    UBUF_PTR upstViaParm;/*SIP_VIA_PARM_S*/
}SIP_HEADER_VIA_S;

typedef struct tagSIP_HEADER_WARNING_S
{
    UBUF_PTR upucTemp;/*UCHAR*/
}SIP_HEADER_WARNING_S;

typedef struct tagSIP_HEADER_WWW_AUTHENTICATE_S
{
    UBUF_PTR upucTemp;/*UCHAR*/
}SIP_HEADER_WWW_AUTHENTICATE_S;

typedef struct tagSIP_LOCATION_S
{
    SIP_TRANSPORT_PROTOCOL_E eProtocol;
    UCHAR  aucIPStr[SIP_MAX_IP_STR_LEN];/*IPV4����IPV6�ĵ�ַ�ַ��� SIP_IP_TYPE_E*/
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
    UCHAR *pucAbnfDes; /* ABNF�﷨�����ַ��� */
    ULONG  ulLen;      /* ABNF�﷨�������� */
}SIP_SYNTAX_CFG_S;

typedef struct tagSIP_TXP_CFG_S
{
    UCHAR *pucSendby; /* send-by�ֶ���д���� */
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

/* ��Ʒ���������� */
typedef struct tagSIP_CFG_S
{
    SIP_TU_CFG_S     stTuCfg;
    SIP_TXN_CFG_S    stTxnCfg;
    SIP_TXP_CFG_S    stTxpCfg;
    SIP_SYNTAX_CFG_S stSyntaxCfg;
    SIP_SHELL_CFG_S  stShellCfg;
}SIP_CFG_S;