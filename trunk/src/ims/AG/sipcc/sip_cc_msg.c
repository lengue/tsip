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

/*主要是有关腿的MDU消息处理*/
void SIPCC_MduMsgProc(SIPCP_MDU_MSG_S *pstMduMsg)
{
}

/*主要是订阅和释放消息*/
void SIPCC_EmMsgProc(ULONG ulSpID)
{
    return;
}

/*定时器超时，一律有业务逻辑处理*/
void SIPCC_TimerMsgProc(ULONG ulSpID)
{
    return;
}