/*******************************************************************************
业务逻辑状态机处理函数
*******************************************************************************/
ULONG SIPSP_SLFsm(ULONG ulSpID,SIPCP_EVENT_S *pstEvent)
{
    ULONG  ulRet;
    USHORT usSlIndex;
    SIPCP_EVENT_S    stFailEvent;
    SIPCP_SL_ITEM_S  *pstSlItem;

    /*找到匹配项*/
    ulRet = SIPSP_SlItemMatch(ulSpID, pstEvent, &usSlIndex);
    if (ulRet != SUCCESS)
    {
        /* 没有找到匹配项，直接丢弃该事件 */
        return ulRet;
    }

    /*业务逻辑处理成功*/
    pstSlItem = &g_pstSlTbl[usSlIndex].stSlItem;
    ulRet = SIPSP_SlProcess(ulSpID,pstSlItem);
    if (ulRet == SUCCESS)
    {
        return SUCCESS;
    }

    /*业务逻辑处理失败，并且存在失败转化事件，使用失败转化事件处理*/
    if (pstSlItem->stEvent.eFailEvent != SIPCP_EVENT_BUTT)
    {
        VOS_MemCpy(&stFailEvent,pstEvent,sizeof(SIPCP_EVENT_S));
        stFailEvent.eEvent = pstSlItem->stEvent.eFailEvent;
        return SIPSP_SLFsm(ulSpID,&stFailEvent);
    }

    return FAIL;
}


/*******************************************************************************
业务逻辑处理函数
ulSpID:业务控制块索引
usSlIndex:业务逻辑索引
*******************************************************************************/
ULONG SIPSP_SlProcess(ULONG ulSpID, SIPCP_SL_ITEM_S *pstSlItem)
{
    ULONG ulRet = SUCCESS;
    UCHAR ucLoop;
    SIPCP_ACTION_E eAction;

    /* 事件触发的通用处理 */
    SIPSP_SlGeneralProc(pstSlItem);

    /* 执行ACTION */
    for (ucLoop = 0; ucLoop < SIPCP_ACTION_NUM; ucLoop++)
    {
        eAction = pstSlItem->astAction[ucLoop].eAction;
        if (eAction >= SIPCP_ACTION_BUTT)
        {
            /*遇到第一个无效的ACTION即认为后续ACTION均无效*/
            break;
        }

        ulRet = SIPCP_ActionDispatch(ulSpID,&pstSlItem->astAction[ucLoop]);
        if (ulRet != SUCCESS)
        {
            break;
        }
    }

    /*刷新综合呼叫腿状态*/
    ulRet = SIPSP_RefreshIntegerLegState(ulSpID);

    /*刷新业务ID*/
    ulRet = SIPSP_RefreshCurrentSID(ulSpID,pstSlItem);

    /*刷新业务ID*/
    ulRet = SIPSP_RefreshObserveEvent(ulSpID,pstSlItem);

    if (ulRet != SUCCESS)
    {
        /* 释放ACTION中申请的资源 */
        if(ulTmpConfID != NULL_ULONG)
        {
            SIPCP_KickConnFromConf(ulTmpConfID);
        }

        if(ulTmpConnID != NULL_ULONG)
        {
            SIPCP_ReleaseConn(ulTmpConnID);
        }

        /*丢弃操作*/
        g_ucMduMsgNum = 0;

        /*恢复业务控制块*/
        VOS_MemCpy(&g_pstSPCB[ulSpID], g_pstSPCBBak, sizeof(SIPCP_SPCB_S));

        /*ACTION执行过程中连接可能已经被打开了，必须重新打开*/
        for (ucLoop = 0; ucLoop < g_ucMduMsgNum; ucLoop++)
        {
            SIPCP_ActiveConn(g_pstSPCB[ulSpID].stResource.astConn[ucLoop]);
        }
    }
    else
    {
        /* MDU操作确认 */
        for (ucLoop = 0; ucLoop < g_ucMduMsgNum; ucLoop++)
        {
            SIPCP_SendMduMsg(&g_pstMduQueue[ucLoop]);
        }
    }

    return ulRet;
}

/*******************************************************************************
事件触发的通用处理
*******************************************************************************/
void SIPSP_SlGeneralProc(ULONG ulSpID, SIPCP_SL_ITEM_S *pstSlItem)
{
    /*没有KA标记需要停音*/
    if(!(pstSlItem->usFlagMask&SIPCP_FLAG_KA))
    {
        SIPCP_StopSignal(ulSpID);
    }

    SIPCP_StopDmm();

    /*停止定时器*/
    return;
}

/*******************************************************************************
获取匹配的业务逻辑项
eInpoutEvent:输入事件
eOEvent:观察事件
输出结果:SUCCESS  事件匹配
         FAIL 事件不匹配
*******************************************************************************/
ULONG SIPSP_SlItemMatch(ULONG ulSpID,SIPCP_EVENT_S *pstEvent,USHORT *pusSlIndex)
{
    ULONG  ulRet;
    UCHAR  ucSeq;
    USHORT usSlIndex;
    SIPCP_SL_CORE_S   *pstSlCore;
    SIPCP_SL_ITEM_S   *pstSlItem;

    ucSeq     =  g_pstSPCB[ulSpID].stService.ucCtlLegSeq;
    usSlIndex = &g_ausSlIndexTbl[g_pstSPCB[ulSpID].ulVagNo]
                                [g_pstSPCB[ulSpID].stService.eHomeSID]
                                [g_pstSPCB[ulSpID].stService.eOLS]
                                [g_pstSPCB[ulSpID].stService.eICLS];
    while (usSlIndex!= NULL_USHORT)
    {
        pstSlItem = &g_pstSlTbl[usSlIndex];
        pstSlCore = &pstSlItem->stSlItem;

        /*事件匹配*/
        ulRet = SIPSP_EventMatch(pstEvent,&pstSlCore->stEvent);
        if (ulRet != SUCCESS)
        {
            usSlIndex = pstSlItem->usNextItem;
            continue;
        }

        /*入口条件匹配*/
        ulRet = SIPSP_ConditionMatch(ulSpID,&pstSlCore->ulCondition);
        if (ulRet != SUCCESS)
        {
            usSlIndex = pstSlItem->usNextItem;
            continue;
        }

        /*业务权限匹配*/
        ulRet = SIPSP_ServiceRightMatch(ulSpID,&pstSlCore->usSIDMask);
        if (ulRet != SUCCESS)
        {
            usSlIndex = pstSlItem->usNextItem;
            continue;
        }

        *pusSlIndex = usSlIndex;
    }

    return FAIL;
}


/*******************************************************************************
事件匹配函数
eInpoutEvent:输入事件
eOEvent:观察事件
输出结果:SUCCESS  事件匹配
         FAIL 事件不匹配
*******************************************************************************/
ULONG SIPSP_EventMatch(SIPCP_EVENT_S *pstInpoutEvent,SIPCP_EVENT_S *pstOEvent)
{
    switch (pstInpoutEvent->eEvent)
    {
        /*单个拨号事件*/
        case SIPCP_EVENT_SOC0:
        case SIPCP_EVENT_SOC1:
        case SIPCP_EVENT_SOC2:
        case SIPCP_EVENT_SOC3:
        case SIPCP_EVENT_SOC4:
        case SIPCP_EVENT_SOC5:
        case SIPCP_EVENT_SOC6:
        case SIPCP_EVENT_SOC7:
        case SIPCP_EVENT_SOC8:
        case SIPCP_EVENT_SOC9:
        case SIPCP_EVENT_SOCE:
        case SIPCP_EVENT_SOCF:
            if ((SIPCP_EVENT_SOCX == pstOEvent->eEvent)
              ||(pstInpoutEvent->eEvent == pstOEvent->eEvent))
            {
                return SUCCESS;
            }
            break;

        /*物理事件*/
        case SIPCP_EVENT_OFFHOOK:
        case SIPCP_EVENT_ONHOOK:
        case SIPCP_EVENT_HOOK:
        /*数图事件*/
        case SIPCP_EVENT_SCC:
        case SIPCP_EVENT_INIT:
        case SIPCP_EVENT_DIAL_FAIL:
        /*内部事件*/
        case SIPCP_EVENT_SRV_TIMEOUT:
        case SIPCP_EVENT_SIGNAL_COMP:
            if (pstInpoutEvent->eEvent == pstOEvent->eEvent)
            {
                return SUCCESS;
            }
            break;

        /*网络事件*/
        case SIPCP_EVENT_LEG_PEER_ALERT:
        case SIPCP_EVENT_LEG_CREATE_SUCCESS:
        case SIPCP_EVENT_LEG_RELEASE:
        case SIPCP_EVENT_LEG_NEGO_SUCCESS:
        case SIPCP_EVENT_LEG_NEGO_FAIL:
        case SIPCP_EVENT_LEG_REFER_SUCCESS:
        case SIPCP_EVENT_LEG_REFER_FAIL:
        case SIPCP_EVENT_LEG_CREATE_IND:
            /*比较事件ID*/
            if (pstInpoutEvent->eEvent != pstOEvent->eEvent)
            {
                break;
            }

            /*观察任何腿，无需再判断*/
            if (pstOEvent->aucParaList[0] == SIPCP_LEG_TYPE_ANY)
            {
                return SUCCESS;
            }

            /* 比较腿ID */
            if (pstInpoutEvent->aucParaList[0] != pstOEvent->aucParaList[0])
            {
                break;
            }

            /* 控制腿到此可以认为匹配成功 */
            if (pstInpoutEvent->aucParaList[0] == SIPCP_LEG_TYPE_CONTROL)
            {
                return SUCCESS;
            }

            /*比较呼叫腿类型*/
            if ((pstOEvent->aucParaList[1] == SIPCP_CLEG_TYPE_ANY)
              ||(pstInpoutEvent->aucParaList[1] == pstOEvent->aucParaList[1])
            {
                return SUCCESS;
            }
            break;

        default:
            break;
    }

    return FAIL;
}

/*******************************************************************************
入口条件匹配函数
eInpoutEvent:输入事件
eOEvent:观察事件
输出结果:SUCCESS  事件匹配
         FAIL 事件不匹配
*******************************************************************************/
ULONG SIPSP_ConditionMatch(ULONG ulSpID,ULONG ulConditionMask)
{
    /*无条件*/
    if(ulConditionMask == SIPCP_CONDITION_MASK_NULL)
    {
        return SUCCESS;
    }

    /*主叫*/
    if(ulConditionMask&SIPCP_CONDITION_CALLER)
    {

    }

    if(ulConditionMask&SIPCP_CONDITION_CALLEE)
    {

    }

    if(ulConditionMask&SIPCP_CONDITION_CALLEE_FIRST)
    {

    }

    return FAIL;
}


/*******************************************************************************
业务权限条件匹配函数
eInpoutEvent:输入事件
eOEvent:观察事件
输出结果:SUCCESS  事件匹配
         FAIL 事件不匹配
*******************************************************************************/
ULONG SIPSP_ServiceRightMatch(ULONG ulSpID,ULONG ulConditionMask)
{
    if(g_pstSPCB[ulSpID].stSrvRight.ulSrvMask & ulConditionMask)
    {
        return SUCCESS;
    }

    return FAIL;
}

/*******************************************************************************
刷新综合呼叫腿状态
*******************************************************************************/
void SIPSP_RefreshIntegerLegState(ULONG ulSpID)
{
    /*先取当前腿状态，再取非当前腿状态*/
    return;
}

/*******************************************************************************
刷新综合呼叫腿状态
*******************************************************************************/
void SIPSP_RefreshCurrentSID(ULONG ulSpID,SIPCP_SL_ITEM_S *pstSlItem)
{
    ULONG ulVaildRight;
    ULONG ulSID;
    ULONG ulSIDMask;
    UCHAR ucLoop;

    g_pstSPCB[ulSpID].stService.eHomeSID = pstSlItem->eNextHSID;
    ulVaildRight = g_pstSPCB[ulSpID].stSrvRight.ulSrvMask & pstSlItem->usSIDMask;

    for(ucLoop = 0;ucLoop < SIPCP_SERVICE_ID_BUTT;ucLoop++)
    {
        SIPCP_GetSMaskFromSID(g_pstSPCB[ulSpID].stSrvRight.ulSrvPriority[ucLoop],
                            ulSIDMask);
        if(ulVaildRight&ulSIDMask)
        {
            g_pstSPCB[ulSpID].stService.eCurSID
                = g_pstSPCB[ulSpID].stSrvRight.ulSrvPriority[ucLoop];
            return;
        }
    }

    return;
}

/*******************************************************************************
刷新观察事件
*******************************************************************************/
void SIPSP_RefreshObserveEvent(ULONG ulSpID)
{
    ULONG  ulVagNo;
    ULONG  ulRet;
    UCHAR  ucSeq;
    UCHAR  ucSccFlag  = FALSE;
    UCHAR  ucInitFlag = FALSE;
    USHORT usSlIndex;
    SIPCP_SERVICE_ID_E eSID;
    SIPCP_LEG_STATE_E  eCtlLegState;
    SIPCP_LEGS_STATE_E eCallLegState;
    SIPCP_SL_INDEX_S  *pstSlIndex;
    SIPCP_SL_ITEM_S   *pstSlItem;

    ulVagNo       = g_pstSPCB[ulSpID].ulVagNo;
    eSID          = g_pstSPCB[ulSpID].stService.eHomeSID;
    eCtlLegState  = g_pstSPCB[ulSpID].stService.eOLS;
    eCallLegState = g_pstSPCB[ulSpID].stService.eICLS;
    ucSeq         = g_pstSPCB[ulSpID].stService.ucCtlLegSeq;

    if (g_pstSPCB[ulSpID].stDmmCtr.ulDmmIndex != NULL_ULONG)
    {
        SIPCP_StopDmm();
    }

    usSlIndex = &g_ausSlIndexTbl[ulVagNo][eSID][eCtlLegState][eCallLegState];
    while (usSlIndex!= NULL_USHORT)
    {
        pstSlIndex = &g_pstSlTbl[usSlIndex];
        pstSlItem = &pstSlIndex->stSlItem;

        /*入口条件匹配*/
        ulRet = SIPSP_ConditionMatch(ulSpID,&pstSlItem->ulCondition);
        if (ulRet != SUCCESS)
        {
            usSlIndex = pstSlIndex->usNextItem;
            continue;
        }

        /*业务权限匹配*/
        ulRet = SIPSP_ServiceRightMatch(ulSpID,&pstSlItem->usSIDMask);
        if (ulRet != SUCCESS)
        {
            usSlIndex = pstSlIndex->usNextItem;
            continue;
        }

        if(pstSlItem->stEvent.eEvent == SIPCP_EVENT_SCC)
        {
            ucSccFlag = TRUE;
        }

        if(pstSlItem->stEvent.eEvent == SIPCP_EVENT_INIT)
        {
            ucInitFlag = TRUE;
        }

        usSlIndex = pstSlIndex->usNextItem;
    }

    if(ucSccFlag == TRUE || ucInitFlag == TRUE)
    {
        SIPCP_StartDmm();
    }

    return FAIL;
}