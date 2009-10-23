/*���յ���SIP��Ϣý��Э�̻��Ƽ�飬�����Ϣ�Ƿ�Ӧ��Я���ṩ/Ӧ��
1��Ӧ��Я��Ӧ��/�ṩ����û��Я�����ش���
2����Ӧ��Я��Ӧ��/�ṩ����ȴЯ�����ش���
3������������سɹ�*/
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
        /* ����SDP����� */
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
        /* ������SDP����� */
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

    /*����SDP����״̬*/
    if()
    {
    }

    /*ý��Э��*/
    if()
    {
    }
    return;
}

/* �ⷢ��ϢЭ���жϣ��ж���Ϣ�Ƿ�Ӧ��Я��Ӧ��/�ṩ
1��Ӧ��Я��Ӧ��/�ṩ���سɹ�
2����Ӧ��Я��ý������������ʧ��*/
ULONG SIPCC_Nego_MsgJudge()
{
    return;
}

/*ͬ�Զ�ý������������ý��Э��*/
ULONG SIPCC_Nego_Proc()
{
    return;
}

/*���˴���Offer*/
ULONG SIPCC_Nego_CreateOffer()
{
    return;
}

/*���˴���Answer*/
ULONG SIPCC_Nego_CreateAnswer()
{
    return;
}
