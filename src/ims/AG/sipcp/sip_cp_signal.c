/*******************************************************************************
信号处理，指本端播放的信号，向远端播放的信号不在此处实现
*******************************************************************************/
ULONG SIPCP_StartSignal(ULONG ulSpID, SIPCP_SIGNAL_TYPE_E eSignalType)
{
    ULONG ulRet;

    switch(eSignalType)
    {
        case SIPCP_SIGNAL_MULT_CID1:
            ulRet = SIPCP_Signal_CID1_Start(ulSpID);
            break;

        case SIPCP_SIGNAL_MULT_CID2:
            ulRet = SIPCP_Signal_CID2_Start(ulSpID);
            break;

        case SIPCP_SIGNAL_MULT_AOCE:
            ulRet = SIPCP_Signal_AOCE_Start(ulSpID);
            break;

        case SIPCP_SIGNAL_MULT_WAITHOOK:
            ulRet = SIPCP_Signal_WAITHOOK_Start(ulSpID);
            break;
    }

    return SUCCESS;
}

void SIPCP_StopSignal(ULONG ulSpID)
{
    SIPCP_SIGNAL_TYPE_E eSignalType;
    switch(eSignalType)
    {
        case SIPCP_SIGNAL_MULT_CID1:
            SIPCP_Signal_CID1_Stop(ulSpID);
            break;

        case SIPCP_SIGNAL_MULT_CID2:
            SIPCP_Signal_CID2_Stop(ulSpID);
            break;

        case SIPCP_SIGNAL_MULT_AOCE:
            SIPCP_Signal_AOCE_Stop(ulSpID);
            break;

        case SIPCP_SIGNAL_MULT_WAITHOOK:
            SIPCP_Signal_WAITHOOK_Stop(ulSpID);
            break;
    }
    return SUCCESS;
}

void SIPCP_Signal_TimeOut(ULONG ulSpID)
{
    SIPCP_SIGNAL_TYPE_E eSignalType;
    switch(eSignalType)
    {
        case SIPCP_SIGNAL_MULT_CID1:
            SIPCP_Signal_CID1_Stop(ulSpID);
            break;

        case SIPCP_SIGNAL_MULT_CID2:
            SIPCP_Signal_CID2_Stop(ulSpID);
            break;

        case SIPCP_SIGNAL_MULT_AOCE:
            SIPCP_Signal_AOCE_Stop(ulSpID);
            break;

        case SIPCP_SIGNAL_MULT_WAITHOOK:
            SIPCP_Signal_WAITHOOK_Stop(ulSpID);
            break;
    }
    return SUCCESS;
}

void SIPCP_Signal_SendReady(ULONG ulSpID)
{
    SIPCP_SIGNAL_TYPE_E eSignalType;
    switch(eSignalType)
    {
        case SIPCP_SIGNAL_MULT_CID1:
            SIPCP_Signal_CID1_Stop(ulSpID);
            break;

        case SIPCP_SIGNAL_MULT_CID2:
            SIPCP_Signal_CID2_Stop(ulSpID);
            break;

        case SIPCP_SIGNAL_MULT_AOCE:
            SIPCP_Signal_AOCE_Stop(ulSpID);
            break;

        case SIPCP_SIGNAL_MULT_WAITHOOK:
            SIPCP_Signal_WAITHOOK_Stop(ulSpID);
            break;
    }
    return SUCCESS;
}

void SIPCP_Signal_SendComp(ULONG ulSpID)
{
    SIPCP_SIGNAL_TYPE_E eSignalType;
    switch(eSignalType)
    {
        case SIPCP_SIGNAL_MULT_CID1:
            SIPCP_Signal_CID1_Stop(ulSpID);
            break;

        case SIPCP_SIGNAL_MULT_CID2:
            SIPCP_Signal_CID2_Stop(ulSpID);
            break;

        case SIPCP_SIGNAL_MULT_AOCE:
            SIPCP_Signal_AOCE_Stop(ulSpID);
            break;

        case SIPCP_SIGNAL_MULT_WAITHOOK:
            SIPCP_Signal_WAITHOOK_Stop(ulSpID);
            break;
    }
    return SUCCESS;
}

ULONG SIPCP_Signal_CID1_Start(ULONG ulSpID)
{
    return SUCCESS;
}

ULONG SIPCP_Signal_CID1_Stop(ULONG ulSpID)
{
    return SUCCESS;
}

ULONG SIPCP_Signal_CID1_SendReady(ULONG ulSpID)
{
    return SUCCESS;
}

ULONG SIPCP_Signal_CID1_SendComp(ULONG ulSpID)
{
    return SUCCESS;
}

ULONG SIPCP_Signal_CID1_TimeOut(ULONG ulSpID)
{
    return SUCCESS;
}

ULONG SIPCP_Signal_CID2_Start(ULONG ulSpID)
{
    return SUCCESS;
}

ULONG SIPCP_Signal_CID2_Stop(ULONG ulSpID)
{
    return SUCCESS;
}

ULONG SIPCP_Signal_CID2_SendReady(ULONG ulSpID)
{
    return SUCCESS;
}

ULONG SIPCP_Signal_CID2_SendComp(ULONG ulSpID)
{
    return SUCCESS;
}

ULONG SIPCP_Signal_CID2_TimeOut(ULONG ulSpID)
{
    return SUCCESS;
}

ULONG SIPCP_Signal_AOCE_Start(ULONG ulSpID)
{
    return SUCCESS;
}

ULONG SIPCP_Signal_AOCE_Stop(ULONG ulSpID)
{
    return SUCCESS;
}

ULONG SIPCP_Signal_AOCE_SendReady(ULONG ulSpID)
{
    return SUCCESS;
}

ULONG SIPCP_Signal_AOCE_SendComp(ULONG ulSpID)
{
    return SUCCESS;
}

ULONG SIPCP_Signal_AOCE_TimeOut(ULONG ulSpID)
{
    return SUCCESS;
}

ULONG SIPCP_Signal_WAITHOOK_Start(ULONG ulSpID)
{
    return SUCCESS;
}

ULONG SIPCP_Signal_WAITHOOK_Stop(ULONG ulSpID)
{
    return SUCCESS;
}

ULONG SIPCP_Signal_WAITHOOK_TimeOut(ULONG ulSpID)
{
    return SUCCESS;
}