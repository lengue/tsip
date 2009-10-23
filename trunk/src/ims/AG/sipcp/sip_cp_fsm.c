/*******************************************************************************
ҵ���߼�״̬��������
*******************************************************************************/
ULONG SIPSP_SLFsm(ULONG ulSpID,SIPCP_EVENT_S *pstEvent)
{
    ULONG  ulRet;
    USHORT usSlIndex;
    SIPCP_EVENT_S    stFailEvent;
    SIPCP_SL_ITEM_S  *pstSlItem;

    /*�ҵ�ƥ����*/
    ulRet = SIPSP_SlItemMatch(ulSpID, pstEvent, &usSlIndex);
    if (ulRet != SUCCESS)
    {
        /* û���ҵ�ƥ���ֱ�Ӷ������¼� */
        return ulRet;
    }

    /*ҵ���߼�����ɹ�*/
    pstSlItem = &g_pstSlTbl[usSlIndex].stSlItem;
    ulRet = SIPSP_SlProcess(ulSpID,pstSlItem);
    if (ulRet == SUCCESS)
    {
        return SUCCESS;
    }

    /*ҵ���߼�����ʧ�ܣ����Ҵ���ʧ��ת���¼���ʹ��ʧ��ת���¼�����*/
    if (pstSlItem->stEvent.eFailEvent != SIPCP_EVENT_BUTT)
    {
        VOS_MemCpy(&stFailEvent,pstEvent,sizeof(SIPCP_EVENT_S));
        stFailEvent.eEvent = pstSlItem->stEvent.eFailEvent;
        return SIPSP_SLFsm(ulSpID,&stFailEvent);
    }

    return FAIL;
}


/*******************************************************************************
ҵ���߼�������
ulSpID:ҵ����ƿ�����
usSlIndex:ҵ���߼�����
*******************************************************************************/
ULONG SIPSP_SlProcess(ULONG ulSpID, SIPCP_SL_ITEM_S *pstSlItem)
{
    ULONG ulRet = SUCCESS;
    UCHAR ucLoop;
    SIPCP_ACTION_E eAction;

    /* �¼�������ͨ�ô��� */
    SIPSP_SlGeneralProc(pstSlItem);

    /* ִ��ACTION */
    for (ucLoop = 0; ucLoop < SIPCP_ACTION_NUM; ucLoop++)
    {
        eAction = pstSlItem->astAction[ucLoop].eAction;
        if (eAction >= SIPCP_ACTION_BUTT)
        {
            /*������һ����Ч��ACTION����Ϊ����ACTION����Ч*/
            break;
        }

        ulRet = SIPCP_ActionDispatch(ulSpID,&pstSlItem->astAction[ucLoop]);
        if (ulRet != SUCCESS)
        {
            break;
        }
    }

    /*ˢ���ۺϺ�����״̬*/
    ulRet = SIPSP_RefreshIntegerLegState(ulSpID);

    /*ˢ��ҵ��ID*/
    ulRet = SIPSP_RefreshCurrentSID(ulSpID,pstSlItem);

    /*ˢ��ҵ��ID*/
    ulRet = SIPSP_RefreshObserveEvent(ulSpID,pstSlItem);

    if (ulRet != SUCCESS)
    {
        /* �ͷ�ACTION���������Դ */
        if(ulTmpConfID != NULL_ULONG)
        {
            SIPCP_KickConnFromConf(ulTmpConfID);
        }

        if(ulTmpConnID != NULL_ULONG)
        {
            SIPCP_ReleaseConn(ulTmpConnID);
        }

        /*��������*/
        g_ucMduMsgNum = 0;

        /*�ָ�ҵ����ƿ�*/
        VOS_MemCpy(&g_pstSPCB[ulSpID], g_pstSPCBBak, sizeof(SIPCP_SPCB_S));

        /*ACTIONִ�й��������ӿ����Ѿ������ˣ��������´�*/
        for (ucLoop = 0; ucLoop < g_ucMduMsgNum; ucLoop++)
        {
            SIPCP_ActiveConn(g_pstSPCB[ulSpID].stResource.astConn[ucLoop]);
        }
    }
    else
    {
        /* MDU����ȷ�� */
        for (ucLoop = 0; ucLoop < g_ucMduMsgNum; ucLoop++)
        {
            SIPCP_SendMduMsg(&g_pstMduQueue[ucLoop]);
        }
    }

    return ulRet;
}

/*******************************************************************************
�¼�������ͨ�ô���
*******************************************************************************/
void SIPSP_SlGeneralProc(ULONG ulSpID, SIPCP_SL_ITEM_S *pstSlItem)
{
    /*û��KA�����Ҫͣ��*/
    if(!(pstSlItem->usFlagMask&SIPCP_FLAG_KA))
    {
        SIPCP_StopSignal(ulSpID);
    }

    SIPCP_StopDmm();

    /*ֹͣ��ʱ��*/
    return;
}

/*******************************************************************************
��ȡƥ���ҵ���߼���
eInpoutEvent:�����¼�
eOEvent:�۲��¼�
������:SUCCESS  �¼�ƥ��
         FAIL �¼���ƥ��
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

        /*�¼�ƥ��*/
        ulRet = SIPSP_EventMatch(pstEvent,&pstSlCore->stEvent);
        if (ulRet != SUCCESS)
        {
            usSlIndex = pstSlItem->usNextItem;
            continue;
        }

        /*�������ƥ��*/
        ulRet = SIPSP_ConditionMatch(ulSpID,&pstSlCore->ulCondition);
        if (ulRet != SUCCESS)
        {
            usSlIndex = pstSlItem->usNextItem;
            continue;
        }

        /*ҵ��Ȩ��ƥ��*/
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
�¼�ƥ�亯��
eInpoutEvent:�����¼�
eOEvent:�۲��¼�
������:SUCCESS  �¼�ƥ��
         FAIL �¼���ƥ��
*******************************************************************************/
ULONG SIPSP_EventMatch(SIPCP_EVENT_S *pstInpoutEvent,SIPCP_EVENT_S *pstOEvent)
{
    switch (pstInpoutEvent->eEvent)
    {
        /*���������¼�*/
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

        /*�����¼�*/
        case SIPCP_EVENT_OFFHOOK:
        case SIPCP_EVENT_ONHOOK:
        case SIPCP_EVENT_HOOK:
        /*��ͼ�¼�*/
        case SIPCP_EVENT_SCC:
        case SIPCP_EVENT_INIT:
        case SIPCP_EVENT_DIAL_FAIL:
        /*�ڲ��¼�*/
        case SIPCP_EVENT_SRV_TIMEOUT:
        case SIPCP_EVENT_SIGNAL_COMP:
            if (pstInpoutEvent->eEvent == pstOEvent->eEvent)
            {
                return SUCCESS;
            }
            break;

        /*�����¼�*/
        case SIPCP_EVENT_LEG_PEER_ALERT:
        case SIPCP_EVENT_LEG_CREATE_SUCCESS:
        case SIPCP_EVENT_LEG_RELEASE:
        case SIPCP_EVENT_LEG_NEGO_SUCCESS:
        case SIPCP_EVENT_LEG_NEGO_FAIL:
        case SIPCP_EVENT_LEG_REFER_SUCCESS:
        case SIPCP_EVENT_LEG_REFER_FAIL:
        case SIPCP_EVENT_LEG_CREATE_IND:
            /*�Ƚ��¼�ID*/
            if (pstInpoutEvent->eEvent != pstOEvent->eEvent)
            {
                break;
            }

            /*�۲��κ��ȣ��������ж�*/
            if (pstOEvent->aucParaList[0] == SIPCP_LEG_TYPE_ANY)
            {
                return SUCCESS;
            }

            /* �Ƚ���ID */
            if (pstInpoutEvent->aucParaList[0] != pstOEvent->aucParaList[0])
            {
                break;
            }

            /* �����ȵ��˿�����Ϊƥ��ɹ� */
            if (pstInpoutEvent->aucParaList[0] == SIPCP_LEG_TYPE_CONTROL)
            {
                return SUCCESS;
            }

            /*�ȽϺ���������*/
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
�������ƥ�亯��
eInpoutEvent:�����¼�
eOEvent:�۲��¼�
������:SUCCESS  �¼�ƥ��
         FAIL �¼���ƥ��
*******************************************************************************/
ULONG SIPSP_ConditionMatch(ULONG ulSpID,ULONG ulConditionMask)
{
    /*������*/
    if(ulConditionMask == SIPCP_CONDITION_MASK_NULL)
    {
        return SUCCESS;
    }

    /*����*/
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
ҵ��Ȩ������ƥ�亯��
eInpoutEvent:�����¼�
eOEvent:�۲��¼�
������:SUCCESS  �¼�ƥ��
         FAIL �¼���ƥ��
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
ˢ���ۺϺ�����״̬
*******************************************************************************/
void SIPSP_RefreshIntegerLegState(ULONG ulSpID)
{
    /*��ȡ��ǰ��״̬����ȡ�ǵ�ǰ��״̬*/
    return;
}

/*******************************************************************************
ˢ���ۺϺ�����״̬
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
ˢ�¹۲��¼�
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

        /*�������ƥ��*/
        ulRet = SIPSP_ConditionMatch(ulSpID,&pstSlItem->ulCondition);
        if (ulRet != SUCCESS)
        {
            usSlIndex = pstSlIndex->usNextItem;
            continue;
        }

        /*ҵ��Ȩ��ƥ��*/
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