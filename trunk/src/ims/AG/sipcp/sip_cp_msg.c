/*******************************************************************************
各种消息处理流程是:通用处理->业务逻辑->保护处理
通用处理表示不需要业务逻辑处理或者无论业务逻辑是否处理都应该做的操作
如果需要，提交业务逻辑，
如果业务逻辑处理返回错误(处理失败或者是没有匹配项)，提交保护处理。

业务逻辑处理失败必须保持原状态不变
*******************************************************************************/
void SIPCP_MsgProc(ULONG *pMsg)
{
    ULONG ulPid;
    switch(ulPid)
    {
        case PID_SIPCC:
            SIPCP_LegMsgProc(pMsg);
            break;

        case PID_SIPEM:
            SIPCP_EmMsgProc();
            break;

        case PID_SIPMP:
            SIPCP_MpMsgProc();
            break;

        case PID_TIMER:
            SIPCP_TimerMsgProc();
            break;

        case PID_VPM:
            SIPCP_UserMsgProc();
            break;

        default:
            break;
    }

    return;
}

/*主要是有关腿的MDU消息处理*/
void SIPCP_LegMsgProc(SIPCP_MDU_MSG_S *pstMduMsg)
{
    UCHAR ucEvent;
    ULONG ulSpID;
    ULONG ulRet;

    ulRet = SIPCP_LegMsg_CommProc(ulSpID,pstMduMsg,&ucEvent);
    if (ulRet != SUCCESS)
    {
        return;
    }

    if(ucEvent >= SIPCP_EVENT_BUTT)
    {
        return;
    }

    ulRet = SIPSP_SLFsm(ulSpID, ucEvent);
    if (ulRet == SUCCESS)
    {
        return;
    }

    SIPCP_LegMsg_ProtectProc(ulSpID,pstMduMsg);
    return;
}

/*主要是用户侧消息*/
void SIPCP_UserMsgProc(ULONG ulEmID)
{
    UCHAR ucEvent;
    ULONG ulRet;
    ULONG ulSpID;

    SIPEM_FromEmIDGetSpID(ulEmID,&ulSpID);

    ulRet = SIPCP_UserMsg_CommProc(ulSpID);
    if (ulRet != SUCCESS)
    {
        return;
    }

    if(ucEvent >= SIPCP_EVENT_BUTT)
    {
        return;
    }

    ulRet = SIPSP_SLFsm(ulSpID, ucEvent);
    if (ulRet == SUCCESS)
    {
        return;
    }

    SIPCP_LegMsg_ProtectProc(ulSpID);
    return;
}

/*主要是订阅和释放消息*/
void SIPCP_EmMsgProc(ULONG ulSpID)
{
    return;
}

/*主要是DSP相关的消息*/
void SIPCP_MpMsgProc(ULONG ulSpID)
{
    return;
}

/*定时器超时，一律有业务逻辑处理*/
void SIPCP_TimerMsgProc(ULONG ulSpID)
{
    return;
}

/*消息的通用处理，如果需要业务逻辑继续处理，输出参数pucEvent必须设置为合法有效值*/
ULONG SIPCP_LegMsg_CommProc(ULONG ulSpID,SIPCP_MDU_MSG_S *pstMduMsg,UCHAR *pucEvent)
{
    switch(pstMduMsg->cMsgType)
    {
        case SIPCP_MDU_RELEASE_LEG_IND:
            if(ulSpID == NULL_ULONG)
            {
                SIPCP_CalleeInit();
            }
            else
            {
                /*给该退申请空间，申请失败直接释放该退*/
                /*申请成功*/
            };
            break;

        default:
            break;
    }
    return SUCCESS;
}

/*消息的保护处理，只需要在业务逻辑处理失败才做*/
void SIPCP_LegMsg_ProtectProc(SIPCP_MDU_MSG_S *pstMduMsg)
{
    SIPCP_MDU_MSG_S stMduMsg;
    switch (pstMduMsg->cMsgType)
    {
        /*没有业务逻辑处理的情况下需要释放该腿*/
        case SIPCP_MDU_CREAT_LEG_IND:
            stMduMsg.eMsgType       = SIPCP_MDU_CREAT_LEG_CFM;
            stMduMsg.ulLegID        = pstMduMsg->ulLegID;
            stMduMsg.ulCauseCode    = 486;
            SIP_SendMduMsg(&stMduMsg);
            break;

        default:
            break;
    }

    return;
}

/*消息的通用处理，如果需要业务逻辑继续处理，输出参数pucEvent必须设置为合法有效值*/
ULONG SIPCP_UserMsg_CommProc(ULONG          *pulSpID,
                            SIPCP_MDU_MSG_S *pstMduMsg,
                            UCHAR           *pucEvent,
                            UCHAR            ucFinished)
{
    if(*pulSpID == NULL_ULONG)&&(pstMduMsg->cMsgType != OFF_HOOK))
    {
        /*非摘机条件下SPID为NULL，不处理*/
        return SUCCESS;
    }

    switch(pstMduMsg->cMsgType)
    {
        case OFF_HOOK:
            SIPCP_StopSignal(*pulSpID);
            if(*pulSpID == NULL_ULONG)
            {
                SIPCP_CalleeInit();
            }
            break;

        case ON_HOOK:
            SIPCP_StopSignal(*pulSpID);
            break;

        case HOOKING:

        case DIAL:

        /*摘机置标记*/
        /*挂机置标记*/
        /*FSK_START发给信号模块*/
        default:
            break;
    }

    return SUCCESS;
}

/*消息的保护处理，只需要在业务逻辑处理失败才做*/
void SIPCP_UserMsg_ProtectProc(SIPCP_MDU_MSG_S *pstMduMsg)
{
    SIPCP_MDU_MSG_S stMduMsg;
    switch (pstMduMsg->cMsgType)
    {
        /*挂机释放所有业务*/
        default:
            break;
    }

    return;
}


/*主叫初始化*/
void SIPCP_CallerInit(SIPCP_MDU_MSG_S *pstMduMsg)
{
    /* 1、申请SPID和保留连接，申请失败释放已申请资源，直接返回 */
    /* 2、申请成功判断是否接受呼叫，如果不接受直接放忙音，其他继续 */
    return;
}

/*被叫初始化*/
void SIPCP_CalleeInit(SIPCP_MDU_MSG_S *pstMduMsg)
{
    ULONG ulEmID;
    ULONG ulSpID;

    SIPEM_FromEmIDGetSpID(ulEmID, &ulSpID);
    /* 1、则判断是否接受呼叫，如果不接受直接释放该腿 */
    /* 2、申请SPID和保留连接，申请失败释放已申请资源同时是否那个该腿，其他继续 */
    return;
}

void SIPCP_TriggleRelease(SIPCP_MDU_MSG_S *pstMduMsg)
{
    ULONG ulSpID;
    if (g_pstSPCB[ulSpID].stService.ucValidLegNum != 0)
    {
        return FAIL;
    }

    if (g_pstSPCB[ulSpID].bDiscFlag == TRUE
      &&g_pstSPCB[ulSpID].bAoceReady == TRUE)
    {
        SIPCP_StartSignal(ulSpID, SIPCP_SIGNAL_MULT_AOCE);
    }
    else if(g_pstSPCB[ulSpID].bDiscFlag != TRUE
        &&&&g_pstSPCB[ulSpID].bWaitRelease != TRUE)
    {
        SIPCP_StartSignal(ulSpID, SIPCP_SIGNAL_MULT_AOCE);
    }

    SIPCP_CheckReleaseSPCB();
    return;
}

/*被叫初始化*/
ULONG SIPCP_CanAcceptCall(ULONG ulEmID)
{
    return SUCCESS;
}

/*被叫初始化*/
ULONG SIPEM_FromEmIDGetSpID(ULONG ulEmID,ULONG *pulSpID)
{
    return SUCCESS;
}

/*被叫初始化*/
ULONG SIPCP_CheckReleaseSPCB(ULONG ulEmID,ULONG *pulSpID)
{
    return SUCCESS;
}