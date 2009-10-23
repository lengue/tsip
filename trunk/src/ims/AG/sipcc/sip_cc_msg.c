/*******************************************************************************
*******************************************************************************/
void SIPCC_MsgProc(ULONG *pMsg)
{
    ULONG ulPid;
    switch(ulPid)
    {
        case PID_SIPCP:
            SIPCC_MduMsgProc(pMsg);
            break;

        case PID_SIPEM:
            SIPCC_EmMsgProc();
            break;

        case PID_TIMER:
            SIPCC_TimerMsgProc();
            break;

        default:
            break;
    }

    return;
}

/*��Ҫ���й��ȵ�MDU��Ϣ����*/
void SIPCC_MduMsgProc(SIPCP_MDU_MSG_S *pstMduMsg)
{
}

/*��Ҫ�Ƕ��ĺ��ͷ���Ϣ*/
void SIPCC_EmMsgProc(ULONG ulSpID)
{
    return;
}

/*��ʱ����ʱ��һ����ҵ���߼�����*/
void SIPCC_TimerMsgProc(ULONG ulSpID)
{
    return;
}