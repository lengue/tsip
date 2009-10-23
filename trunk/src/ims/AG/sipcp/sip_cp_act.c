/*******************************************************************************
业务逻辑处理函数
ulSpID:业务控制块索引
usSlIndex:业务逻辑索引
*******************************************************************************/
ULONG SIPCP_ActionDispatch(ULONG ulSpID,SIPCP_ACTION_S *pstAction)
{
    ULONG ulRet;

    switch (pstAction->eAction)
    {
        case SIPCP_ACTION_CREATE_LEG:
            ulRet =  SIPCP_Act_CreateLeg(pstAction->aucParaList[0],
                                         pstAction->aucParaList[1]);
            break;

        case SIPCP_ACTION_CONFIRM_LEG:
            ulRet =  SIPCP_Act_ConfirmLeg(pstAction->aucParaList[0],
                                          pstAction->aucParaList[1]);
            break;

        case SIPCP_ACTION_RELEASE_LEG:
            ulRet =  SIPCP_Act_ReleaseLeg(pstAction->aucParaList[0],
                                          pstAction->aucParaList[1]);
            break;

        case SIPCP_ACTION_REFER_LEG:
            ulRet =  SIPCP_Act_ReferLeg(pstAction->aucParaList[0],
                                        pstAction->aucParaList[1]);
            break;

        case SIPCP_ACTION_HOLD_LEG:
            ulRet =  SIPCP_Act_HoldLeg(pstAction->aucParaList[0],
                                       pstAction->aucParaList[1]);
            break;

        case SIPCP_ACTION_RESUME_LEG:
            ulRet =  SIPCP_Act_ResumeLeg(pstAction->aucParaList[0],
                                         pstAction->aucParaList[1]);
            break;

        case SIPCP_ACTION_DEACTIVE_LEG:
            ulRet =  SIPCP_Act_DeactiveLeg(pstAction->aucParaList[0],
                                           pstAction->aucParaList[1]);
            break;

        case SIPCP_ACTION_ACTIVE_LEG:
            ulRet =  SIPCP_Act_ActiveLeg(pstAction->aucParaList[0],
                                         pstAction->aucParaList[1]);
            break;

        case SIPCP_ACTION_START_TIMER:
            ulRet =  SIPCP_Act_StartTimer(pstAction->aucParaList[0]);
            break;

        case SIPCP_ACTION_STOP_TIMER:
            ulRet =  SIPCP_Act_StopTimer(pstAction->aucParaList[0]);
            break;

        case SIPCP_ACTION_START_SIGNAL:
            ulRet =  SIPCP_Act_StartSignal(pstAction->aucParaList[0]);
            break;

        case SIPCP_ACTION_SET_CURRENT_LEG:
            ulRet =  SIPCP_Act_SetCurrentLeg(pstAction->aucParaList[0]);
            break;

        default:
            return FAIL;
    }

    return SUCCESS;
}

/*******************************************************************************
action列表，顺序最好遵循ACTION的ID顺序来操作


有一个前提，认为如果存在释放资源的ACTION，则认为该业务逻辑不会存在再新申请资源的
ACTION，因此如果是释放资源的操作业务逻辑无法恢复，失败认为是一个异常。
*******************************************************************************/
ULONG SIPCP_Act_CreateLeg(SIPCP_LEG_TYPE_E eLegType,SIPCP_CREATE_METHOD_E eCreateMethod)
{
    switch (eCreateMethod)
    {
        case SIPCP_CREATE_METHOD_USER:
        case SIPCP_CREATE_METHOD_HOTLINE:
        case SIPCP_CREATE_METHOD_CONF:
        case SIPCP_CREATE_METHOD_SCC:
        default:
            break;
    }
    /*发送MDU*/
}

ULONG SIPCP_Act_ConfirmLeg(SIPCP_LEG_TYPE_E eLegType,SIPCP_CREATE_METHOD_E eCreateMethod)
{
    switch (eCreateMethod)
    {
        case SIPCP_CREATE_METHOD_USER:
        case SIPCP_CREATE_METHOD_HOTLINE:
        case SIPCP_CREATE_METHOD_CONF:
        case SIPCP_CREATE_METHOD_SCC:
        default:
            break;
    }
    /*发送MDU*/
}

ULONG SIPCP_Act_ReleaseLeg(SIPCP_LEG_TYPE_E eLegType,BOOL bCurrentLeg, UCHAR ucReason)
{
    ULONG ulRet;
    ULONG ulLegID;
    UCHAR ucLegSeq;
    UCHAR ucConnSeq;
    ULONG ulConn;
    ULONG ulConn2;
    ULONG ulConf;

    /*获取腿腿ID和序号*/
    ulRet = SIPCP_GetLegFromType(eLegType, bCurrentLeg, &ulLegID, &ucLegSeq);
    if(ulRet != SUCCESS)
    {
        return ulRet;
    }
    /*发送MDU*/
}

/*目的腿->参考腿*/
ULONG SIPCP_Act_ReferLeg(SIPCP_LEG_TYPE_E eByLegType,
                       BOOL           bByCurrentLeg,
                       SIPCP_LEG_TYPE_E
                       SIPCP_LEG_TYPE_E eToLegType,
                       BOOL           bToCurrentLeg)
{
    ULONG ulRet;
    ULONG ulLegID;
    UCHAR ucLegSeq;
    UCHAR ucConnSeq;
    ULONG ulConn;
    ULONG ulConn2;
    ULONG ulConf;

    /*获取腿腿ID和序号*/
    ulRet = SIPCP_GetLegFromType(eLegType, eCurrentLeg, &ulLegID, &ucLegSeq);
    if(ulRet != SUCCESS)
    {
        return ulRet;
    }

    /*发送MDU*/
}

/*HOLD一条腿，即修改流模式*/
ULONG SIPCP_Act_HoldLeg(SIPCP_LEG_TYPE_E eLegType,BOOL bCurrentLeg)
{
    ULONG ulRet;
    ULONG ulLegID;
    UCHAR ucLegSeq;
    UCHAR ucConnSeq;
    ULONG ulConn;
    ULONG ulConn2;
    ULONG ulConf;

    /*获取腿腿ID和序号*/
    ulRet = SIPCP_GetLegFromType(eLegType, eCurrentLeg, &ulLegID, &ucLegSeq);
    if(ulRet != SUCCESS)
    {
        return ulRet;
    }

    /*发送MDU*/
}

/*恢复一条腿，即恢复流模式*/
ULONG SIPCP_Act_ResumeLeg(SIPCP_LEG_TYPE_E eLegType,BOOL bCurrentLeg)
{
    ULONG ulRet;
    ULONG ulLegID;
    UCHAR ucLegSeq;
    UCHAR ucConnSeq;
    ULONG ulConn;
    ULONG ulConn2;
    ULONG ulConf;

    /*获取腿腿ID和序号*/
    ulRet = SIPCP_GetLegFromType(eLegType, eCurrentLeg, &ulLegID, &ucLegSeq);
    if(ulRet != SUCCESS)
    {
        return ulRet;
    }

    /*发送MDU*/
}

/* 设置当前呼叫腿，只能设置为呼叫腿 */
ULONG SIPCP_Act_SetCurrentLeg(BOOL bCurrentLeg)
{
    ULONG ulRet;
    ULONG ulLegID;
    UCHAR ucSeq;

    /*获取腿腿ID和序号*/
    ulRet = SIPCP_GetLegFromType(bCurrentLeg, &ulLegID, &ucSeq);
    if(ulRet != SUCCESS)
    {
        return ulRet;
    }

    g_pstSPCB[ulCurSpID].stService.ucCurLegSeq = ucSeq;
    return SUCCESS;
}

/* 激活腿，即给腿绑定连接 */
ULONG SIPCP_Act_ActiveLeg(SIPCP_LEG_TYPE_E    eLegType,
                          SIPCP_CLEG_TYPE_E   eCurrentLeg,
                          SIPCP_ACTIVE_MODE_E eActiveMode)
{
    ULONG ulRet;
    ULONG ulLegID;
    UCHAR ucLegSeq;
    UCHAR ucConnSeq;
    ULONG ulConn;
    ULONG ulConn2;
    ULONG ulConf;

    /*获取腿腿ID和序号*/
    ulRet = SIPCP_GetLegFromType(eLegType, eCurrentLeg, &ulLegID, &ucLegSeq);
    if(ulRet != SUCCESS)
    {
        return ulRet;
    }

    /* 已存在连接，上报错误 */
    if (g_pstSPCB[ulCurSpID].stService.astLeg[ucLegSeq].ucConnSeq != NULL_UCHAR)
    {
        return FAIL;
    }

    switch(eActiveMode)
    {
        /* 抢占模式 */
        case SIPCP_ACTIVE_MODE_PREEMPT:
            g_pstSPCB[ulCurSpID].stService.astLeg[ucLegSeq].ucConnSeq
                = g_pstSPCB[ulCurSpID].stResource.ucResvConn;
            break;

        /* 混音模式 */
        case SIPCP_ACTIVE_MODE_MIX:
            SIPCP_AllocConf(&ulConf);
            SIPCP_ActiveConn(&ulConn);
            ulRet = SIPCP_AddConnToConf(ulConf,ulConn,ulConn2);
            if(ulRet != SUCCESS)
            {

            }
            break;

        /* 远端放音模式 */
        case SIPCP_ACTIVE_MODE_R_TONE:
            break;

        default:
            break;
    }

    return SUCCESS;
}

/* 去激活腿，即去掉腿的绑定连接 */
ULONG SIPCP_Act_DeactiveLeg(SIPCP_LEG_TYPE_E eLegType,SIPCP_CLEG_TYPE_E eCurrentLeg)
{
    ULONG ulRet;
    ULONG ulLegID;
    UCHAR ucLegSeq;
    UCHAR ucConnSeq;
    ULONG ulConn;
    ULONG ulConn2;
    ULONG ulConf;

    /*获取腿腿ID和序号*/
    ulRet = SIPCP_GetLegFromType(eLegType, eCurrentLeg, &ulLegID, &ucLegSeq);
    if(ulRet != SUCCESS)
    {
        return ulRet;
    }

    /*  */
    if (g_pstSPCB[ulCurSpID].stService.astLeg[ucLegSeq].ucConnSeq == NULL_UCHAR)
    {
        return SUCCESS;
    }

    if (g_pstSPCB[ulCurSpID].stResource.ulConfID != NULL_ULONG)
    {
        SIPCP_KickConnFromConf(ulConf);
    }

    SIPCP_ReleaseConn(ulConn);
    return SUCCESS;
}

/* 播放信号 */
ULONG SIPCP_Act_StartSignal(SIPCP_SIGNAL_E eSignal)
{
    /*有会议室通过会议室播放，没有会议室通过保留连接播放*/
}

/* 播放信号 */
ULONG SIPCP_Act_StartTimer(SIPCP_SIGNAL_E eSignal)
{
    /*有会议室通过会议室播放，没有会议室通过保留连接播放*/
}

/* 播放信号 */
ULONG SIPCP_Act_StopTimer(SIPCP_SIGNAL_E eSignal)
{
    /*有会议室通过会议室播放，没有会议室通过保留连接播放*/
}

/* 播放信号 */
ULONG SIPCP_StopDmm(ULONG ulSpID)
{
    /*有会议室通过会议室播放，没有会议室通过保留连接播放*/
}

/* 根据腿类型和当前腿标记获取腿信息 */
ULONG SIPCP_GetLegFromType(SIPCP_LEG_TYPE_E  eLegType,
                           SIPCP_CLEG_TYPE_E eCurrentLeg,
                           ULONG *pulLegID,
                           UCHAR *pucSeq)
{
    UCHAR ucLoop;
    UCHAR ucSeq = NULL_UCHAR;

    switch (eLegType)
    {
        case SIPCP_LEG_TYPE_CONTROL:
            if (g_pstSPCB[ulCurSpID].stService.ucCtlLegSeq != NULL_UCHAR)
            {
                /* 找到控制腿 */
                ucSeq = g_pstSPCB[ulCurSpID].stService.ucCtlLegSeq;
            }
            break;

        case SIPCP_LEG_TYPE_CALL:
            if ((eCurrentLeg == SIPCP_CLEG_TYPE_CURR)
              &&(g_pstSPCB[ulCurSpID].stService.ucCurLegSeq == NULL_UCHAR))
            {
                return FAIL;
            }

            if (eCurrentLeg == SIPCP_CLEG_TYPE_CURR)
            {
                /* 找到当前呼叫腿 */
                ucSeq = g_pstSPCB[ulCurSpID].stService.ucCurLegSeq;
                break;
            }

            for (ucLoop = 0; ucLoop < SIPCP_LEG_NUM_IN_SPCB; ucLoop++)
            {
                if ((ucLoop == g_pstSPCB[ulCurSpID].stService.ucCtlLegSeq)
                  ||(ucLoop == g_pstSPCB[ulCurSpID].stService.ucCurLegSeq))
                {
                    continue;
                }

                if (g_pstSPCB[ulCurSpID].stService.astLeg[ucLoop] != NULL_ULONG)
                {
                    /* 找到非当前呼叫腿 */
                    ucSeq = ucLoop;
                    break;
                }
            }
            break;

        default:
            return FAIL;
    }

    if(ucSeq == NULL_UCHAR)
    {
        return FAIL;
    }

    *pucSeq = ucSeq;
    *pulLegID = g_pstSPCB[ulCurSpID].stService.astLeg[ucSeq];
    return SUCCESS;
}

/* 申请连接 */
void SIPCP_AllocConn(ULONG *pulConn)
{
    /*有会议室通过会议室播放，没有会议室通过保留连接播放*/
}

/* 申请连接 */
void SIPCP_ReleaseConn(ULONG *pulConn)
{
    /*有会议室通过会议室播放，没有会议室通过保留连接播放*/
}

/* 激活连接 */
void SIPCP_ActiveConn(ULONG ulConn)
{
    /*有会议室通过会议室播放，没有会议室通过保留连接播放*/
}

/* 激活连接 */
void SIPCP_AllocConf(ULONG ulSpID,ULONG *pulConf)
{
    /*有会议室通过会议室播放，没有会议室通过保留连接播放*/
}

/* 激活连接 */
void SIPCP_KickConnFromConf(ULONG ulConf)
{
    /*有会议室通过会议室播放，没有会议室通过保留连接播放*/
}

/* 将连接加入会议室 */
void SIPCP_AddConnToConf(ULONG ulConf,ULONG ulConn1,ULONG ulConn2)
{
    /*有会议室通过会议室播放，没有会议室通过保留连接播放*/
}
