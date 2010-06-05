/*******************************************************************************
功能    : SDP消息编码函数
创建人  : 唐春平
创建日期: 2010.04.29
修改记录:

编码原则:
1、 所有的编码函数形参一致
    UBUF_HEADER_S *pstUbufMsg     : 存储结构的UBUF
    void *pstStruct               : 编码结构的指针，结构体根据函数不同而不同
    SIP_SYNTAX_BUFFER_S *pstBuffer: 编码缓存结构
2、 每个函数首要的任务是明确pstStruct的结构类型
*******************************************************************************/
/* 外部依赖模块头文件 */
#include "common\common.h"
#include "abnf\abnf.h"
#include "ubuf\ubuf.h"

/* 本模块对外提供的常量和结构头文件 */
#include "..\..\include\sdp\sdp_const.h"
#include "..\..\include\sdp\sdp_type.h"

/* 本模块内部使用的常量和结构头文件 */
#include "sdp_const.inc"
#include "sdp_type.inc"

/* 本模块内部函数声明头文件 */
#include "sdp_code.inc"

/* 本模块全局变量声明头文件 */
#include "sdp_var.inc"

/*******************************************************************************
session-description = proto-version
                             origin-field
                             session-name-field
                             information-field
                             uri-field
                             email-fields
                             phone-fields
                             connection-field
                             bandwidth-fields
                             time-fields
                             key-field
                             attribute-fields
                             media-descriptions
*******************************************************************************/
ULONG SDP_CodeSessionDescription(void *pstStruct,
                                 SYNTAX_BUFFER_S *pstBuffer)
{
    SDP_MSG_S *pstSdpMsg= NULL_PTR;
    ULONG ulRet;
    
    pstSdpMsg = (SDP_MSG_S *)pstStruct;
    ulRet = SDP_GET_CODE_FUNC(SDP_ABNF_RULE_PROTO_VERSION)(&pstSdpMsg->stProtoVersion, pstBuffer);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }
    
    ulRet = SDP_GET_CODE_FUNC(SDP_ABNF_RULE_ORIGIN_FIELD)(&pstSdpMsg->stOriginField, pstBuffer);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }

    ulRet = SDP_GET_CODE_FUNC(SDP_ABNF_RULE_SESSION_NAME_FIELD)(&pstSdpMsg->stSessionNameField, pstBuffer);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }
    
    ulRet = SDP_GET_CODE_FUNC(SDP_ABNF_RULE_INFORMATION_FIELD)(pstSdpMsg->pstInformation, pstBuffer);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }

    ulRet = SDP_GET_CODE_FUNC(SDP_ABNF_RULE_URI_FIELD)(pstSdpMsg->pstUriField, pstBuffer);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }

    ulRet = SDP_GET_CODE_FUNC(SDP_ABNF_RULE_EMAIL_FIELDS)(pstSdpMsg->pstEmailAddresses, pstBuffer);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }

    ulRet = SDP_GET_CODE_FUNC(SDP_ABNF_RULE_PHONE_FIELDS)(pstSdpMsg->pstPhoneNumbers, pstBuffer);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }

    ulRet = SDP_GET_CODE_FUNC(SDP_ABNF_RULE_CONNECTION_FIELD)(pstSdpMsg->pstConnection, pstBuffer);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }

    ulRet = SDP_GET_CODE_FUNC(SDP_ABNF_RULE_BANDWIDTH_FIELDS)(pstSdpMsg->pstBandwidths, pstBuffer);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }

    ulRet = SDP_GET_CODE_FUNC(SDP_ABNF_RULE_TIME_FIELDS)(&pstSdpMsg->stTimeFields, pstBuffer);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }

    ulRet = SDP_GET_CODE_FUNC(SDP_ABNF_RULE_KEY_FIELD)(pstSdpMsg->pstKeyType, pstBuffer);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }

    ulRet = SDP_GET_CODE_FUNC(SDP_ABNF_RULE_ATTRIBUTE_FIELDS)(pstSdpMsg->pstAttributes, pstBuffer);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }

    ulRet = SDP_GET_CODE_FUNC(SDP_ABNF_RULE_MEDIA_DESCRIPTIONS)(pstSdpMsg->pstMediaDescriptions, pstBuffer);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }

    return SUCCESS;
}

/*******************************************************************************
proto-version = %x76 "=" 1*DIGIT CRLF
;this memo describes version 0
*******************************************************************************/
ULONG SDP_CodeProtoVersion(void *pstStruct,
                           SYNTAX_BUFFER_S *pstBuffer)
{
    SDP_PROTO_VERSION_S *pstProtoVersion= NULL_PTR;
    
    pstProtoVersion = (SDP_PROTO_VERSION_S *)pstStruct;

    SYNTAX_ADD_STRING(pstBuffer, "v=");
    SYNTAX_ADD_DEC_NUM(pstBuffer,pstProtoVersion->ulProtoVersion);
    SYNTAX_ADD_STRING(pstBuffer, "\r\n");

    return SUCCESS;
}

/*******************************************************************************
origin-field = %x6f "=" username SP sess-id SP sess-version SP
nettype SP addrtype SP unicast-address CRLF
*******************************************************************************/
ULONG SDP_CodeOriginField(void *pstStruct,
                          SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    SDP_ORIGIN_FIELD_S *pstOriginField= NULL_PTR;

    pstOriginField = (SDP_ORIGIN_FIELD_S *)pstStruct;

    SYNTAX_ADD_STRING(pstBuffer, "o=");

    SYNTAX_ADD_STRING(pstBuffer, pstOriginField->pucUserName);

    SYNTAX_ADD_STRING(pstBuffer, " ");

    SYNTAX_ADD_DEC_NUM(pstBuffer, pstOriginField->ulSessId);
    
    SYNTAX_ADD_STRING(pstBuffer, " ");

    SYNTAX_ADD_DEC_NUM(pstBuffer, pstOriginField->ulSessVersion);

    SYNTAX_ADD_STRING(pstBuffer, " ");

    SYNTAX_ADD_STRING(pstBuffer, pstOriginField->pucNetType);

    SYNTAX_ADD_STRING(pstBuffer, " ");

    SYNTAX_ADD_STRING(pstBuffer, pstOriginField->pucAddrType);

    SYNTAX_ADD_STRING(pstBuffer, " ");

    ulRet = SDP_GET_CODE_FUNC(SDP_ABNF_RULE_UNICAST_ADDRESS)(pstOriginField->ulUnicastAddress, pstBuffer);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }

    SYNTAX_ADD_STRING(pstBuffer, "\r\n");

    return SUCCESS;
}

/*******************************************************************************
session-name-field = %x73 "=" text CRLF
*******************************************************************************/
ULONG SDP_CodeSessionNameField(void *pstStruct,
                               SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    SDP_SESSION_NAME_FIELD_S *pstSessionNameField= NULL_PTR;

    pstSessionNameField = (SDP_SESSION_NAME_FIELD_S *)pstStruct;

    SYNTAX_ADD_STRING(pstBuffer, "s=");

    SYNTAX_ADD_STRING(pstBuffer, pstSessionNameField->pucSessionName);

    SYNTAX_ADD_STRING(pstBuffer, "\r\n");

    return SUCCESS;
}

/*******************************************************************************
information-field = [%x69 "=" text CRLF]
*******************************************************************************/
ULONG SDP_CodeInformationField(void *pstStruct,
                               SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    SDP_INFORMATION_FIELD_S *pstInformationField = NULL_PTR;

    if (pstBuffer == NULL_PTR)
    {
        return SUCCESS;
    }

    pstInformationField = (SDP_INFORMATION_FIELD_S *)pstStruct;

    SYNTAX_ADD_STRING(pstBuffer, "i=");

    SYNTAX_ADD_STRING(pstBuffer, pstInformationField->pucInformation);

    SYNTAX_ADD_STRING(pstBuffer, "\r\n");

    return SUCCESS;
}

/*******************************************************************************
uri-field = [%x75 "=" uri CRLF]
*******************************************************************************/
ULONG SDP_CodeUriField(void *pstStruct,
                       SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    SDP_URI_FIELD_S *pstUriField = NULL_PTR;

    if (pstBuffer == NULL_PTR)
    {
        return SUCCESS;
    }

    pstUriField = (SDP_URI_FIELD_S *)pstStruct;

    SYNTAX_ADD_STRING(pstBuffer, "u=");

    ulRet = SDP_GET_CODE_FUNC(SDP_ABNF_RULE_URI)(&pstUriField->stUri, pstBuffer);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }

    SYNTAX_ADD_STRING(pstBuffer, "\r\n");

    return SUCCESS;
}

/*******************************************************************************
email-fields = *(%x65 "=" email-address CRLF)
*******************************************************************************/
ULONG SDP_CodeEmailFields(void *pstStruct,
                          SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    SDP_EMAIL_ADDRESS_S *pstEmailAddress = NULL_PTR;

    if (pstBuffer == NULL_PTR)
    {
        return SUCCESS;
    }

    pstEmailAddress = (SDP_EMAIL_ADDRESS_S *)pstBuffer;

    while (pstEmailAddress != NULL_PTR)
    {
        SYNTAX_ADD_STRING(pstBuffer, "e=");

        ulRet = SDP_GET_CODE_FUNC(SDP_ABNF_RULE_EMAIL_ADDRESS)(pstEmailAddress->pstNext, pstBuffer);
        if (ulRet != SUCCESS)
        {
            return ulRet;
        }

        SYNTAX_ADD_STRING(pstBuffer, "\r\n");
        pstEmailAddress = pstEmailAddress->pstNext;
    }

    return SUCCESS;
}

/*******************************************************************************
phone-fields = *(%x70 "=" phone-number CRLF)
*******************************************************************************/
ULONG SDP_CodePhoneFields(void *pstStruct,
                          SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    SDP_PHONE_NUMBER_S *pstPhoneNumber = NULL_PTR;

    if (pstBuffer == NULL_PTR)
    {
        return SUCCESS;
    }

    pstPhoneNumber = (SDP_PHONE_NUMBER_S *)pstStruct;

    while (pstPhoneNumber != NULL_PTR)
    {
        SYNTAX_ADD_STRING(pstBuffer, "p=");
        
        ulRet = SDP_GET_CODE_FUNC(SDP_ABNF_RULE_PHONE_NUMBER)(pstPhoneNumber, pstBuffer);
        if (ulRet != SUCCESS)
        {
            return ulRet;
        }

        SYNTAX_ADD_STRING(pstBuffer, "\r\n");
        pstPhoneNumber = pstPhoneNumber->pstNext;
    }

    return SUCCESS;
}

/*******************************************************************************
connection-field = [%x63 "=" nettype SP addrtype SP
                            connection-address CRLF]
                            ;a connection field must be present
                            ;in every media description or at the
                            ;session-level
*******************************************************************************/
ULONG SDP_CodeConnectionField(void *pstStruct,
                              SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    SDP_CONNECTION_S *pstConnect = NULL_PTR;

    if (pstBuffer == NULL_PTR)
    {
        return SUCCESS;
    }

    pstConnect = (SDP_CONNECTION_S *)pstStruct;

    SYNTAX_ADD_STRING(pstBuffer, "c=");

    ulRet = SDP_GET_CODE_FUNC(SDP_ABNF_RULE_NETTYPE)(pstConnect->ulNetType, pstBuffer);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }

    SYNTAX_ADD_STRING(pstBuffer, " ");

    ulRet = SDP_GET_CODE_FUNC(SDP_ABNF_RULE_ADDRTYPE)(pstConnect->AddrType, pstBuffer);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }

    SYNTAX_ADD_STRING(pstBuffer, " ");

    ulRet = SDP_GET_CODE_FUNC(SDP_ABNF_RULE_CONNECTION_ADDRESS)(pstConnect->ConnectionAddress, pstBuffer);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }

    SYNTAX_ADD_STRING(pstBuffer, "\r\n");

    return SUCCESS;
}


/*******************************************************************************
bandwidth-fields = *(%x62 "=" bwtype ":" bandwidth CRLF)
*******************************************************************************/
ULONG SDP_CodeBandwidthFields(void *pstStruct,
                              SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    SDP_BANDWIDTH_S *pstBandwidth = NULL_PTR;

    if (pstBuffer == NULL_PTR)
    {
        return SUCCESS;
    }

    pstBandwidth = (SDP_BANDWIDTH_S *)pstStruct;

    while (pstBandwidth != NULL_PTR)
    {
        SYNTAX_ADD_STRING(pstBuffer, "b=");
        
        SYNTAX_ADD_STRING(pstBuffer, pstBandwidth->pucBwType);
        
        SYNTAX_ADD_STRING(pstBuffer, ":");
        
        SYNTAX_ADD_DEC_NUM(pstBuffer, pstBandwidth->ulBandWidth);
        
        SYNTAX_ADD_STRING(pstBuffer, "\r\n");

        pstBandwidth = pstBandwidth->pstNext;
    }

    return SUCCESS;
}

/*******************************************************************************
time-fields = 1*( %x74 "=" start-time SP stop-time
                    *(CRLF repeat-fields) CRLF)
                    [zone-adjustments CRLF]
*******************************************************************************/
ULONG SDP_CodeTimeFields(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    SDP_TIME_FIELDS_S   *pstTimeField = NULL_PTR;
    SDP_TIME_S          *pstTime      = NULL_PTR;
    SDP_REPEAT_FIELDS_S *pstRepeatFields = NULL_PTR;

    if (pstBuffer == NULL_PTR)
    {
        return FAIL;
    }

    pstTimeField = (SDP_TIME_FIELDS_S *)pstStruct;
    
    pstTime = &pstTimeField->stTime;
    while (pstTime != NULL_PTR)
    {
        SYNTAX_ADD_STRING(pstBuffer, "t=");
        
        ulRet = SDP_GET_CODE_FUNC(SDP_ABNF_RULE_START_TIME)(pstTime->ulStartTime, pstBuffer);
        if (ulRet != SUCCESS)
        {
            return ulRet;
        }
        
        SYNTAX_ADD_STRING(pstBuffer, " ");
        
        ulRet = SDP_GET_CODE_FUNC(SDP_ABNF_RULE_STOP_TIME)(pstTime->ulStopTime, pstBuffer);
        if (ulRet != SUCCESS)
        {
            return ulRet;
        }

        pstRepeatFields = pstTime->pstRepeatFields;
        while (pstRepeatFields != NULL_PTR)
        {
            SYNTAX_ADD_STRING(pstBuffer, "\r\n");
            
            ulRet = SDP_GET_CODE_FUNC(SDP_ABNF_RULE_REPEAT_FIELDS)(pstRepeatFields, pstBuffer);
            if (ulRet != SUCCESS)
            {
                return ulRet;
            }

            pstRepeatFields = pstRepeatFields->pstNext;
        }
        
        SYNTAX_ADD_STRING(pstBuffer, "\r\n");

        pstTime = pstTime->pstNext;
    }

    if (pstTimeField->pstZoneAdjustments != NULL_PTR)
    {
        ulRet = SDP_GET_CODE_FUNC(SDP_ABNF_RULE_ZONE_ADJUSTMENTS)(pstTimeField->pstZoneAdjustments, pstBuffer);
        if (ulRet != SUCCESS)
        {
            return ulRet;
        }
        
        SYNTAX_ADD_STRING(pstBuffer, "\r\n");
    }

    return SUCCESS;
}

/*******************************************************************************
key-field = [%x6b "=" key-type CRLF]
*******************************************************************************/
ULONG SDP_CodeKeyField(void *pstStruct,
                       SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    SDP_KEY_TYPE_S *pstKeyType = NULL_PTR;

    if (pstBuffer == NULL_PTR)
    {
        return SUCCESS;
    }

    pstKeyType = (SDP_KEY_TYPE_S *)pstStruct;

    SYNTAX_ADD_STRING(pstBuffer, "k=");
    
    ulRet = SDP_GET_CODE_FUNC(SDP_ABNF_RULE_KEY_TYPE)(pstKeyType, pstBuffer);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }
    
    SYNTAX_ADD_STRING(pstBuffer, "\r\n");

    return SUCCESS;
}

/*******************************************************************************
attribute-fields = *(%x61 "=" attribute CRLF)
*******************************************************************************/
ULONG SDP_CodeAttributeFields(void *pstStruct,
                              SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    SDP_ATTRIBUTE_S *pstAttribute = NULL_PTR;

    if (pstBuffer == NULL_PTR)
    {
        return SUCCESS;
    }

    pstAttribute = (SDP_ATTRIBUTE_S *)pstStruct;

    while (pstAttribute != NULL_PTR)
    {
        SYNTAX_ADD_STRING(pstBuffer, "a=");
        
        ulRet = SDP_GET_CODE_FUNC(SDP_ABNF_RULE_ATTRIBUTE)(pstAttribute, pstBuffer);
        if (ulRet != SUCCESS)
        {
            return ulRet;
        }

        SYNTAX_ADD_STRING(pstBuffer, "\r\n");
        pstAttribute = pstAttribute->pstNext;
    }

    return SUCCESS;
}

/*******************************************************************************
media-descriptions = *( media-field
                                 information-field
                                 *connection-field
                                 bandwidth-fields
                                 key-field
                                 attribute-fields )
*******************************************************************************/
ULONG SDP_CodeMediaDescriptions(void *pstStruct,
                                SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    SDP_MEDIA_DESCRIPTIONS_S *pstMediaDescriptions = NULL_PTR;
    SDP_CONNECTION_S         *pstConnection = NULL_PTR;

    if (pstBuffer == NULL_PTR)
    {
        return SUCCESS;
    }

    pstMediaDescriptions = (SDP_MEDIA_DESCRIPTIONS_S *)pstStruct;
    while (pstMediaDescriptions != NULL_PTR)
    {
        ulRet = SDP_GET_CODE_FUNC(SDP_ABNF_RULE_MEDIA_FIELD)(&pstMediaDescriptions->stMediaField, pstBuffer);
        if (ulRet != SUCCESS)
        {
            return ulRet;
        }

        ulRet = SDP_GET_CODE_FUNC(SDP_ABNF_RULE_INFORMATION_FIELD)(pstMediaDescriptions->pstInformation, pstBuffer);
        if (ulRet != SUCCESS)
        {
            return ulRet;
        }

        pstConnection = pstMediaDescriptions->pstConnection;
        while (pstConnection != NULL_PTR)
        {
            ulRet = SDP_GET_CODE_FUNC(SDP_ABNF_RULE_CONNECTION_FIELD)(pstConnection, pstBuffer);
            if (ulRet != SUCCESS)
            {
                return ulRet;
            }

            pstConnection = pstConnection->pstNext;
        }

        ulRet = SDP_GET_CODE_FUNC(SDP_ABNF_RULE_BANDWIDTH_FIELDS)(pstMediaDescriptions->pstBandwidths, pstBuffer);
        if (ulRet != SUCCESS)
        {
            return ulRet;
        }
        
        ulRet = SDP_GET_CODE_FUNC(SDP_ABNF_RULE_KEY_FIELD)(pstMediaDescriptions->pstKeyType, pstBuffer);
        if (ulRet != SUCCESS)
        {
            return ulRet;
        }
        
        ulRet = SDP_GET_CODE_FUNC(SDP_ABNF_RULE_ATTRIBUTE_FIELDS)(pstMediaDescriptions->pstAttributes, pstBuffer);
        if (ulRet != SUCCESS)
        {
            return ulRet;
        }

        pstMediaDescriptions = pstMediaDescriptions->pstNext;
    }

    return SUCCESS;
}

/*******************************************************************************
media-field = %x6d "=" media SP port ["/" integer]
                    SP proto 1*(SP fmt) CRLF
*******************************************************************************/
ULONG SDP_CodeMediaField(void *pstStruct,
                         SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    SDP_MEDIA_FIELD_S *pstMediaField = NULL_PTR;
    SDP_FMT_S         *pstFmts = NULL_PTR;

    if (pstBuffer == NULL_PTR)
    {
        return SUCCESS;
    }

    pstMediaField = (SDP_MEDIA_FIELD_S *)pstStruct;

    SYNTAX_ADD_STRING(pstBuffer, "m=");
    
    ulRet = SDP_GET_CODE_FUNC(SDP_ABNF_RULE_MEDIA)(pstMediaField->ulMedia, pstBuffer);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }

    SYNTAX_ADD_STRING(pstBuffer, " ");

    ulRet = SDP_GET_CODE_FUNC(SDP_ABNF_RULE_PORT)(pstMediaField->ulPort, pstBuffer);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }

    if (pstMediaField->ulInteger != NULL_ULONG)
    {
        SYNTAX_ADD_STRING(pstBuffer, "/");

        ulRet = SDP_GET_CODE_FUNC(SDP_ABNF_RULE_INTEGER)(pstMediaField->ulInteger, pstBuffer);
        if (ulRet != SUCCESS)
        {
            return ulRet;
        }
    }
    
    SYNTAX_ADD_STRING(pstBuffer, " ");
    
    ulRet = SDP_GET_CODE_FUNC(SDP_ABNF_RULE_PROTO)(pstMediaField->ulProto, pstBuffer);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }
    
    pstFmts = &pstMediaField->stFmts;
    while (pstFmts != NULL_PTR)
    {
        SYNTAX_ADD_STRING(pstBuffer, " ");
        
        ulRet = SDP_GET_CODE_FUNC(SDP_ABNF_RULE_FMT)(pstFmts, pstBuffer);
        if (ulRet != SUCCESS)
        {
            return ulRet;
        }

        pstFmts = pstFmts->pstNext;
    }

    SYNTAX_ADD_STRING(pstBuffer, "\r\n");

    return SUCCESS;
}


/*******************************************************************************
; sub-rules of ’e=’, see RFC 2822 for definitions
email-address = address-and-comment / dispname-and-address
                      / addr-spec
*******************************************************************************/
ULONG SDP_CodeEmailAddress(void *pstStruct,
                           SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    SDP_EMAIL_ADDRESS_S *pstEmailAddress = NULL_PTR;

    if (pstBuffer == NULL_PTR)
    {
        return SUCCESS;
    }

    pstEmailAddress = (SDP_EMAIL_ADDRESS_S *)pstStruct;

    if (pstEmailAddress->pucDispname != NULL_PTR)
    {
        SYNTAX_ADD_STRING(pstBuffer, pstEmailAddress->pucDispname);
        SYNTAX_ADD_STRING(pstBuffer, " <");
    }

    ulRet = SDP_GET_CODE_FUNC(SDP_ABNF_RULE_addr_spec)(pstEmailAddress->ulAddrSpec, pstBuffer);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }

    if (pstEmailAddress->pucDispname != NULL_PTR)
    {
        SYNTAX_ADD_STRING(pstBuffer, ">");
    }

    if (pstEmailAddress->pucComment != NULL_PTR)
    {
        SYNTAX_ADD_STRING(pstBuffer, " (");
        SYNTAX_ADD_STRING(pstBuffer, pstEmailAddress->pucComment);
        SYNTAX_ADD_STRING(pstBuffer, ")");
    }

    return SUCCESS;
}

/*******************************************************************************
phone-number = phone *SP "(" 1*email-safe ")" /
                        1*email-safe "<" phone ">" /
                        phone
*******************************************************************************/
ULONG SDP_CodePhoneNumber(void *pstStruct,
                          SYNTAX_BUFFER_S *pstBuffer)
{
    ULONG ulRet;
    SDP_PHONE_NUMBER_S *pstPhoneNumber = NULL_PTR;

    if (pstBuffer == NULL_PTR)
    {
        return SUCCESS;
    }

    pstPhoneNumber = (SDP_PHONE_NUMBER_S *)pstStruct;
    
    if (pstPhoneNumber->pucDispname != NULL_PTR)
    {
        SYNTAX_ADD_STRING(pstBuffer, pstPhoneNumber->pucDispname);
        SYNTAX_ADD_STRING(pstBuffer, " <");
    }

    SYNTAX_ADD_STRING(pstBuffer, pstPhoneNumber->pucPhoneNumber);

    if (pstPhoneNumber->pucDispname != NULL_PTR)
    {
        SYNTAX_ADD_STRING(pstBuffer, ">");
    }

    if (pstPhoneNumber->pucComment != NULL_PTR)
    {
        SYNTAX_ADD_STRING(pstBuffer, " (");
        SYNTAX_ADD_STRING(pstBuffer, pstPhoneNumber->pucComment);
        SYNTAX_ADD_STRING(pstBuffer, ")");
    }

    return SUCCESS;
}

