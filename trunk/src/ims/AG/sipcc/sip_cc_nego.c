/*接收到的SIP消息媒体协商机制检查，检查消息是否应该携带提供/应答
1、应该携带应答/提供但是没有携带返回错误
2、不应该携带应答/提供但是却携带返回错误
3、其他情况返回成功*/
ULONG SIPCC_Nego_MsgCheck()
{
    UCHAR ucSdpExist;
    UCHAR ucMsgType;
    UCHAR ucSdpType;
    UCHAR ucInviteNegoState;
    UCHAR ucGlobalNegoState;
    UCHAR ucNonInviteNegoState;
    UCHAR ulRet;

    if(ucSdpExist == TRUE)
    {
        /* 存在SDP的情况 */
        switch (ucMsgType)
        {
            case SIPCC_MEDIA_NEGO_MSG_INVITE:
                ucSdpType = SIPCC_MEDIA_NEGO_TYPE_OFFER;
                break;

            case SIPCC_MEDIA_NEGO_MSG_RELIABLE_1XX:
                if (ucInviteNegoState == SIPCC_MEDIA_NEGO_STATE_NULL)
                {
                    ucSdpType = SIPCC_MEDIA_NEGO_TYPE_OFFER;
                }
                else if(ucInviteNegoState == SIPCC_MEDIA_NEGO_STATE_SEND_OFFER)
                {
                    ucSdpType = SIPCC_MEDIA_NEGO_TYPE_ANSWER;
                }
                break;

            case SIPCC_MEDIA_NEGO_MSG_UNRELIABLE_RSP:
                if(ucInviteNegoState == SIPCC_MEDIA_NEGO_STATE_SEND_OFFER)
                {
                    ucSdpType = SIPCC_MEDIA_NEGO_TYPE_ANSWER;
                }
                break;

            case SIPCC_MEDIA_NEGO_MSG_ACK:
                if(ucInviteNegoState == SIPCC_MEDIA_NEGO_STATE_SEND_OFFER)
                {
                    ucSdpType = SIPCC_MEDIA_NEGO_TYPE_ANSWER;
                }
                break;

            case SIPCC_MEDIA_NEGO_MSG_PRACK:
                if (ucInviteNegoState == SIPCC_MEDIA_NEGO_STATE_COMLETE)
                {
                    ucSdpType = SIPCC_MEDIA_NEGO_TYPE_OFFER;
                }
                else if(ucInviteNegoState == SIPCC_MEDIA_NEGO_STATE_SEND_OFFER)
                {
                    ucSdpType = SIPCC_MEDIA_NEGO_TYPE_ANSWER;
                }
                break;

            case SIPCC_MEDIA_NEGO_MSG_PRACK_RSP:
                if (ucNonInviteNegoState == SIPCC_MEDIA_NEGO_STATE_SEND_OFFER)
                {
                    ucSdpType = SIPCC_MEDIA_NEGO_TYPE_ANSWER;
                }
                break;

            case SIPCC_MEDIA_NEGO_MSG_UPDATE:
                if(ucGlobalNegoState == SIPCC_MEDIA_NEGO_STATE_COMLETE)
                {
                    ucSdpType = SIPCC_MEDIA_NEGO_TYPE_OFFER;
                }
                break;

            case SIPCC_MEDIA_NEGO_MSG_UPDATE_RSP:
                if (ucNonInviteNegoState == SIPCC_MEDIA_NEGO_STATE_SEND_OFFER)
                {
                    ucSdpType = SIPCC_MEDIA_NEGO_TYPE_ANSWER;
                }
                break;

            default:
                break;
        }
    }
    else
    {
        /* 不存在SDP的情况 */
        switch(ucMsgType)
        {
            case SIPCC_MEDIA_NEGO_MSG_INVITE:
            case SIPCC_MEDIA_NEGO_MSG_UNRELIABLE_RSP:
            case SIPCC_MEDIA_NEGO_MSG_UPDATE:
                break;

            case SIPCC_MEDIA_NEGO_MSG_RELIABLE_1XX:
                if (ucInviteNegoState == SIPCC_MEDIA_NEGO_STATE_NULL)
                {
                    ulRet = SIPCC_ERROR_CODE_MISSED_SDP;
                }
                break;

            case SIPCC_MEDIA_NEGO_MSG_ACK:
                if(ucInviteNegoState == SIPCC_MEDIA_NEGO_STATE_SEND_OFFER)
                {
                    ulRet = SIPCC_ERROR_CODE_MISSED_SDP;
                }
                break;

            case SIPCC_MEDIA_NEGO_MSG_PRACK:
                if (ucInviteNegoState == SIPCC_MEDIA_NEGO_STATE_SEND_OFFER)
                {
                    ulRet = SIPCC_ERROR_CODE_MISSED_SDP;
                }
                break;

            case SIPCC_MEDIA_NEGO_MSG_PRACK_RSP:
                if(ucNonInviteNegoState == SIPCC_MEDIA_NEGO_STATE_SEND_OFFER)
                {
                    ulRet = SIPCC_ERROR_CODE_MISSED_SDP;
                }
                break;

            case SIPCC_MEDIA_NEGO_MSG_UPDATE_RSP:
                if(ucNonInviteNegoState == SIPCC_MEDIA_NEGO_STATE_SEND_OFFER)
                {
                    ulRet = SIPCC_ERROR_CODE_MISSED_SDP;
                }
                break;

            default:
                break;
        }
    }

    /*根据SDP设置状态*/
    if()
    {
    }

    /*媒体协商*/
    if()
    {
    }
    return;
}

/* 外发消息协商判断，判断消息是否应该携带应答/提供
1、应该携带应答/提供返回成功
2、不应该携带媒体描述符返回失败*/
ULONG SIPCC_Nego_MsgJudge()
{
    return;
}

/*同对端媒体描述符进行媒体协商*/
ULONG SIPCC_Nego_Proc()
{
    return;
}

/*本端创建Offer*/
ULONG SIPCC_Nego_CreateOffer()
{
    return;
}

/*本端创建Answer*/
ULONG SIPCC_Nego_CreateAnswer()
{
    return;
}
