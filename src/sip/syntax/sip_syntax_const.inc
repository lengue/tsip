/*******************************************************************************
����    : SIPЭ��ջ�﷨�������ڲ�ʹ�ó���
������  : �ƴ�ƽ
��������: 200.01.10
�޸ļ�¼:
*******************************************************************************/
typedef enum tagSIP_ABNF_RULE_E
{
    SIP_ABNF_RULE_SIP_MESSAGE,
    SIP_ABNF_RULE_REQUEST,
    SIP_ABNF_RULE_RESPONSE,
    SIP_ABNF_RULE_REQUEST_LINE,
    SIP_ABNF_RULE_STATUS_LINE,
    SIP_ABNF_RULE_MESSAGE_HEADER,
    SIP_ABNF_RULE_MESSAGE_BODY,
    SIP_ABNF_RULE_METHOD,
    SIP_ABNF_RULE_INVITE_M,
    SIP_ABNF_RULE_ACK_M,
    SIP_ABNF_RULE_OPTIONS_M,
    SIP_ABNF_RULE_BYE_M,
    SIP_ABNF_RULE_CANCEL_M,
    SIP_ABNF_RULE_REGISTER_M,
    SIP_ABNF_RULE_REQUEST_URI,
    SIP_ABNF_RULE_SIP_VERSION,
    SIP_ABNF_RULE_STATUS_CODE,
    SIP_ABNF_RULE_ACCEPT,
    SIP_ABNF_RULE_ACCEPT_ENCODING,
    SIP_ABNF_RULE_CALL_ID,
    SIP_ABNF_RULE_CSEQ,
    SIP_ABNF_RULE_FROM,
    SIP_ABNF_RULE_TO,
    SIP_ABNF_RULE_VIA,
    SIP_ABNF_RULE_MAX_FORWARDS,
    SIP_ABNF_RULE_SIP_URI,
    SIP_ABNF_RULE_SIPS_URI,
    SIP_ABNF_RULE_USERINFO,
    SIP_ABNF_RULE_HOSTPORT,
    SIP_ABNF_RULE_USER,
    SIP_ABNF_RULE_PASSWORD,
    SIP_ABNF_RULE_HOST,
    SIP_ABNF_RULE_PORT,
    SIP_ABNF_RULE_HOST_NAME,
    SIP_ABNF_RULE_IPV4_ADDRESS,
    SIP_ABNF_RULE_IPV6_ADDRESS,
    SIP_ABNF_RULE_IPV6_REFERENCE,
    SIP_ABNF_RULE_FROM_SPEC,
    SIP_ABNF_RULE_NAME_ADDR,
    SIP_ABNF_RULE_ADDR_SPEC,
    SIP_ABNF_RULE_FROM_PARAM,
    SIP_ABNF_RULE_DISPLAY_NAME,
    SIP_ABNF_RULE_TO_PARAM,
    SIP_ABNF_RULE_TAG_PARAM,
    SIP_ABNF_RULE_CALLID,
    SIP_ABNF_RULE_VIA_PARM,
    SIP_ABNF_RULE_VIA_PARMS,
    SIP_ABNF_RULE_SENT_PROTOCOL,
    SIP_ABNF_RULE_SENT_BY,
    SIP_ABNF_RULE_TRANSPORT,
    SIP_ABNF_RULE_VIA_TTL,
    SIP_ABNF_RULE_VIA_MADDR,
    SIP_ABNF_RULE_VIA_RECEIVED,
    SIP_ABNF_RULE_VIA_BRANCH,
    SIP_ABNF_RULE_TTL,
    SIP_ABNF_RULE_TOKEN,

    SIP_ABNF_RULE_BUTT
}SIP_ABNF_RULE_E;

#define SIP_GET_RULE_INDEX(ulAppRuleIndex) g_astSipAppRuleTbl[ulAppRuleIndex].ulRuleIndex
#define SIP_GET_CODE_FUNC(ulAppRuleIndex)  g_astSipAppRuleTbl[ulAppRuleIndex].pfnCode
#define SIP_GET_PARSE_FUNC(ulAppRuleIndex) g_astSipAppRuleTbl[ulAppRuleIndex].pfnParse
#define SIP_GET_CLONE_FUNC(ulAppRuleIndex) g_astSipAppRuleTbl[ulAppRuleIndex].pfnClone

#define SIP_GET_HEADER_RULE_INDEX(eHeader) g_astSipSyntaxHeaderTbl[eHeader].ulAppRuleIndex
#define SIP_GET_HEADER_SPEC_SIZE(eHeader)  g_astSipSyntaxHeaderTbl[eHeader].ulStructSize

/* SIP����ƥ���жϺ� */
#define SIP_RULE_MATCH(pstNode, ulAppRuleIndex) \
 ((pstNode->bCoreFlag == FALSE) \
&&(pstNode->ulIndex == g_astSipAppRuleTbl[ulAppRuleIndex].ulRuleIndex))

/* ��ȡƫ�ƺ�ĵ�ַָ�� */
#define SIP_GET_OFFSET_POINT(pucString, ulOffset) pucString + (ulOffset)

/* ����ר�ú�*/
/* �ӻ�ȡ�ַ���ֵ�� */
#define SIP_GET_STRING(pucDest, pstNode, pucString, pstUbuf, ppStruct) \
{ \
    *(ppStruct) = UBUF_AddComponent(pstUbuf, (pstNode)->ulSize + 1);\
    (pucDest) = *(ppStruct);\
    strncpy((pucDest), pucString + (pstNode)->ulOffset, (pstNode)->ulSize); \
    (pucDest)[(pstNode)->ulSize] = '\0'; \
}

/* ��ȡʮ�������ֺ� */
#define SIP_GET_DEC_NUM(Number, pucChar) \
{\
    Number = 0;\
    while ((*pucChar >= '0') && (*pucChar <= '9'))\
    {\
        Number = Number*10 + *pucChar - '0';\
        pucChar++;\
    }\
}

/* �жϻ�ȡ����ռ� */
#define SIP_GET_COMPONET_PTR(pstComponet, Type, pstUbuf, ppStruct)\
{\
    if (*ppStruct == NULL_PTR)\
    {\
        *ppStruct = UBUF_AddComponent(pstUbuf, sizeof(Type));\
        memset(*ppStruct, 0, sizeof(Type));\
    }\
\
    pstComponet = *ppStruct;\
}

/* ����ר�ú� */
/* ����Ϣ����������һ����ͨ�ַ���ֵ */
#define SIP_ADD_STRING(pstBuffer, pucString) \
{\
    strncpy(pstBuffer->pucBuffer + pstBuffer->ulCurrentLen,\
            (pucString), \
            strlen(pucString)); \
    pstBuffer->ulCurrentLen += (ULONG)strlen(pucString); \
}

/* ����Ϣ����������һ��10�������� */
#define SIP_ADD_DEC_NUM(pstBuffer, Number) \
{\
    itoa(Number, pstBuffer->pucBuffer + pstBuffer->ulCurrentLen, 10);\
    pstBuffer->ulCurrentLen += (ULONG)strlen(pstBuffer->pucBuffer + pstBuffer->ulCurrentLen); \
}
