
typedef enum tagSIPCC_MEDIA_NEGO_MSG_E
{
    SIPCC_MEDIA_NEGO_MSG_INVITE,
    SIPCC_MEDIA_NEGO_MSG_RELIABLE_1XX,
    SIPCC_MEDIA_NEGO_MSG_RELIABLE_2XX,
    SIPCC_MEDIA_NEGO_MSG_UNRELIABLE_RSP,
    SIPCC_MEDIA_NEGO_MSG_ACK,
    SIPCC_MEDIA_NEGO_MSG_PRACK,
    SIPCC_MEDIA_NEGO_MSG_PRACK_RSP,
    SIPCC_MEDIA_NEGO_MSG_UPDATE,
    SIPCC_MEDIA_NEGO_MSG_UPDATE_RSP,
    SIPCC_MEDIA_NEGO_MSG_BUTT
}SIPCC_MEDIA_NEGO_MSG_E;

typedef enum tagSIPCC_MEDIA_NEGO_TYPE_E
{
    SIPCC_MEDIA_NEGO_TYPE_OFFER,
    SIPCC_MEDIA_NEGO_TYPE_ANSWER,
    SIPCC_MEDIA_NEGO_TYPE_BUTT
}SIPCC_MEDIA_NEGO_TYPE_E;

typedef enum tagSIPCC_MEDIA_NEGO_STATE_E
{
    SIPCC_MEDIA_NEGO_STATE_NULL,
    SIPCC_MEDIA_NEGO_STATE_SEND_OFFER,
    SIPCC_MEDIA_NEGO_STATE_RECV_OFFER,
    SIPCC_MEDIA_NEGO_STATE_COMLETE,
    SIPCC_MEDIA_NEGO_STATE_BUTT
}SIPCC_MEDIA_NEGO_STATE_E;

typedef enum tagSIPCC_ERROR_CODE_E
{
    SIPCC_ERROR_CODE_REDUNDANT_SDP,
    SIPCC_ERROR_CODE_MISSED_SDP,
    SIPCC_ERROR_CODE_MEDIA_NEGO_FAIL,
    SIPCC_ERROR_CODE_SYSTEM,
    SIPCC_ERROR_CODE_BUTT
}SIPCC_ERROR_CODE_E;