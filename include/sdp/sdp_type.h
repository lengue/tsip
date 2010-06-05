typedef struct tagSDP_CFG_S
{
    UCHAR *pucAbnfDes; /* ABNF语法描述字符串 */
    ULONG  ulLen;      /* ABNF语法描述长度 */
}SDP_CFG_S;

typedef struct tagSDP_CONNECTION_S
{
    ULONG ulNetType;
    ULONG AddrType;
    ULONG ConnectionAddress;
    struct tagSDP_CONNECTION_S *pstNext;
}SDP_CONNECTION_S;

typedef struct tagSDP_BANDWIDTH_S
{
    UCHAR *pucBwType;
    ULONG ulBandWidth;
    struct tagSDP_BANDWIDTH_S *pstNext;
}SDP_BANDWIDTH_S;

typedef struct tagSDP_REPEAT_FIELDS_S
{
    ULONG ulRepeatInterval;
    ULONG ulTypedTime;    
    struct tagSDP_REPEAT_FIELDS_S *pstNext;
}SDP_REPEAT_FIELDS_S;

typedef struct tagSDP_TIME_S
{
    ULONG ulStartTime;
    ULONG ulStopTime;
    SDP_REPEAT_FIELDS_S *pstRepeatFields;
    struct tagSDP_TIME_S *pstNext;
}SDP_TIME_S;

typedef struct tagSDP_ZONE_ADJUSTMENT_S
{
    ULONG ulTime;
    ULONG ulTypedTime;
    struct tagSDP_ZONE_ADJUSTMENT_S *pstNext;
}SDP_ZONE_ADJUSTMENT_S;

typedef struct tagSDP_ZONE_ADJUSTMENTS_S
{
    SDP_ZONE_ADJUSTMENT_S stZoneAdjustment;
}SDP_ZONE_ADJUSTMENTS_S;

typedef struct tagSDP_PROTO_VERSION_S
{
    ULONG ulProtoVersion;
}SDP_PROTO_VERSION_S;

typedef struct tagSDP_ORIGIN_FIELD_S
{
    UCHAR *pucUserName;
    ULONG ulSessId;
    ULONG ulSessVersion;
    UCHAR *pucNetType;
    UCHAR *pucAddrType;
    ULONG ulUnicastAddress;
}SDP_ORIGIN_FIELD_S;

typedef struct tagSDP_SESSION_NAME_FIELD_S
{
    UCHAR *pucSessionName;
}SDP_SESSION_NAME_FIELD_S;

typedef struct tagSDP_INFORMATION_FIELD_S
{
    UCHAR *pucInformation;
}SDP_INFORMATION_FIELD_S;

typedef struct tagSDP_URI_FIELD_S
{
    URI_S stUri;
}SDP_URI_FIELD_S;

typedef struct tagSDP_EMAIL_ADDRESS_S
{
    UCHAR *pucDispname;
    ULONG  ulAddrSpec;
    UCHAR *pucComment;
    struct tagSDP_EMAIL_ADDRESS_S *pstNext;
}SDP_EMAIL_ADDRESS_S;

typedef struct tagSDP_PHONE_NUMBER_S
{
    UCHAR *pucDispname;
    UCHAR *pucPhoneNumber;
    UCHAR *pucComment;
    struct tagSDP_PHONE_NUMBER_S *pstNext;
}SDP_PHONE_NUMBER_S;

typedef struct tagSDP_TIME_FIELDS_S
{
    SDP_TIME_S stTime;
    SDP_ZONE_ADJUSTMENTS_S *pstZoneAdjustments;
}SDP_TIME_FIELDS_S;

typedef struct tagSDP_KEY_TYPE_S
{
    ULONG ulType;
    union
    {
        ULONG ulClear;
        ULONG ulBase64;
        ULONG ulUri;
    }u;
}SDP_KEY_TYPE_S;

typedef struct tagSDP_ATTRIBUTE_S
{
    UCHAR *pucAttField;
    UCHAR *pucAttValue;
    struct tagSDP_ATTRIBUTE_S *pstNext;
}SDP_ATTRIBUTE_S;

typedef struct tagSDP_FMT_S
{
    UCHAR *pucFmt;
    struct tagSDP_FMT_S *pstNext;
}SDP_FMT_S;

typedef struct tagSDP_MEDIA_FIELD_S
{
    ULONG ulMedia;
    ULONG ulPort;
    ULONG ulInteger;
    ULONG ulProto;
    SDP_FMT_S stFmts;
}SDP_MEDIA_FIELD_S;

typedef struct tagSDP_MEDIA_DESCRIPTIONS_S
{
    SDP_MEDIA_FIELD_S         stMediaField;
    SDP_INFORMATION_FIELD_S  *pstInformation;
    SDP_CONNECTION_S         *pstConnection;
    SDP_BANDWIDTH_S          *pstBandwidths;
    SDP_KEY_TYPE_S           *pstKeyType;
    SDP_ATTRIBUTE_S          *pstAttributes;
    struct tagSDP_MEDIA_DESCRIPTIONS_S *pstNext;
}SDP_MEDIA_DESCRIPTIONS_S;

/* SDP消息结构 */
typedef struct tagSDP_MSG_S
{
    SDP_PROTO_VERSION_S       stProtoVersion;
    SDP_ORIGIN_FIELD_S        stOriginField;
    SDP_SESSION_NAME_FIELD_S  stSessionNameField;
    SDP_INFORMATION_FIELD_S  *pstInformationField;
    SDP_URI_FIELD_S          *pstUriField;          
    SDP_EMAIL_ADDRESS_S      *pstEmailAddresses;
    SDP_PHONE_NUMBER_S       *pstPhoneNumbers;
    SDP_CONNECTION_S         *pstConnection;
    SDP_BANDWIDTH_S          *pstBandwidths;
    SDP_TIME_FIELDS_S         stTimeFields;
    SDP_KEY_TYPE_S           *pstKeyType;
    SDP_ATTRIBUTE_S          *pstAttributes;
    SDP_MEDIA_DESCRIPTIONS_S *pstMediaDescriptions;
}SDP_MSG_S;
