/*******************************************************************************
ҵ���߼�������
ulSpID:ҵ����ƿ�����
usSlIndex:ҵ���߼�����
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
action�б�˳�������ѭACTION��ID˳��������


��һ��ǰ�ᣬ��Ϊ��������ͷ���Դ��ACTION������Ϊ��ҵ���߼������������������Դ��
ACTION�����������ͷ���Դ�Ĳ���ҵ���߼��޷��ָ���ʧ����Ϊ��һ���쳣��
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
    /*����MDU*/
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
    /*����MDU*/
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

    /*��ȡ����ID�����*/
    ulRet = SIPCP_GetLegFromType(eLegType, bCurrentLeg, &ulLegID, &ucLegSeq);
    if(ulRet != SUCCESS)
    {
        return ulRet;
    }
    /*����MDU*/
}

/*Ŀ����->�ο���*/
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

    /*��ȡ����ID�����*/
    ulRet = SIPCP_GetLegFromType(eLegType, eCurrentLeg, &ulLegID, &ucLegSeq);
    if(ulRet != SUCCESS)
    {
        return ulRet;
    }

    /*����MDU*/
}

/*HOLDһ���ȣ����޸���ģʽ*/
ULONG SIPCP_Act_HoldLeg(SIPCP_LEG_TYPE_E eLegType,BOOL bCurrentLeg)
{
    ULONG ulRet;
    ULONG ulLegID;
    UCHAR ucLegSeq;
    UCHAR ucConnSeq;
    ULONG ulConn;
    ULONG ulConn2;
    ULONG ulConf;

    /*��ȡ����ID�����*/
    ulRet = SIPCP_GetLegFromType(eLegType, eCurrentLeg, &ulLegID, &ucLegSeq);
    if(ulRet != SUCCESS)
    {
        return ulRet;
    }

    /*����MDU*/
}

/*�ָ�һ���ȣ����ָ���ģʽ*/
ULONG SIPCP_Act_ResumeLeg(SIPCP_LEG_TYPE_E eLegType,BOOL bCurrentLeg)
{
    ULONG ulRet;
    ULONG ulLegID;
    UCHAR ucLegSeq;
    UCHAR ucConnSeq;
    ULONG ulConn;
    ULONG ulConn2;
    ULONG ulConf;

    /*��ȡ����ID�����*/
    ulRet = SIPCP_GetLegFromType(eLegType, eCurrentLeg, &ulLegID, &ucLegSeq);
    if(ulRet != SUCCESS)
    {
        return ulRet;
    }

    /*����MDU*/
}

/* ���õ�ǰ�����ȣ�ֻ������Ϊ������ */
ULONG SIPCP_Act_SetCurrentLeg(BOOL bCurrentLeg)
{
    ULONG ulRet;
    ULONG ulLegID;
    UCHAR ucSeq;

    /*��ȡ����ID�����*/
    ulRet = SIPCP_GetLegFromType(bCurrentLeg, &ulLegID, &ucSeq);
    if(ulRet != SUCCESS)
    {
        return ulRet;
    }

    g_pstSPCB[ulCurSpID].stService.ucCurLegSeq = ucSeq;
    return SUCCESS;
}

/* �����ȣ������Ȱ����� */
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

    /*��ȡ����ID�����*/
    ulRet = SIPCP_GetLegFromType(eLegType, eCurrentLeg, &ulLegID, &ucLegSeq);
    if(ulRet != SUCCESS)
    {
        return ulRet;
    }

    /* �Ѵ������ӣ��ϱ����� */
    if (g_pstSPCB[ulCurSpID].stService.astLeg[ucLegSeq].ucConnSeq != NULL_UCHAR)
    {
        return FAIL;
    }

    switch(eActiveMode)
    {
        /* ��ռģʽ */
        case SIPCP_ACTIVE_MODE_PREEMPT:
            g_pstSPCB[ulCurSpID].stService.astLeg[ucLegSeq].ucConnSeq
                = g_pstSPCB[ulCurSpID].stResource.ucResvConn;
            break;

        /* ����ģʽ */
        case SIPCP_ACTIVE_MODE_MIX:
            SIPCP_AllocConf(&ulConf);
            SIPCP_ActiveConn(&ulConn);
            ulRet = SIPCP_AddConnToConf(ulConf,ulConn,ulConn2);
            if(ulRet != SUCCESS)
            {

            }
            break;

        /* Զ�˷���ģʽ */
        case SIPCP_ACTIVE_MODE_R_TONE:
            break;

        default:
            break;
    }

    return SUCCESS;
}

/* ȥ�����ȣ���ȥ���ȵİ����� */
ULONG SIPCP_Act_DeactiveLeg(SIPCP_LEG_TYPE_E eLegType,SIPCP_CLEG_TYPE_E eCurrentLeg)
{
    ULONG ulRet;
    ULONG ulLegID;
    UCHAR ucLegSeq;
    UCHAR ucConnSeq;
    ULONG ulConn;
    ULONG ulConn2;
    ULONG ulConf;

    /*��ȡ����ID�����*/
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

/* �����ź� */
ULONG SIPCP_Act_StartSignal(SIPCP_SIGNAL_E eSignal)
{
    /*�л�����ͨ�������Ҳ��ţ�û�л�����ͨ���������Ӳ���*/
}

/* �����ź� */
ULONG SIPCP_Act_StartTimer(SIPCP_SIGNAL_E eSignal)
{
    /*�л�����ͨ�������Ҳ��ţ�û�л�����ͨ���������Ӳ���*/
}

/* �����ź� */
ULONG SIPCP_Act_StopTimer(SIPCP_SIGNAL_E eSignal)
{
    /*�л�����ͨ�������Ҳ��ţ�û�л�����ͨ���������Ӳ���*/
}

/* �����ź� */
ULONG SIPCP_StopDmm(ULONG ulSpID)
{
    /*�л�����ͨ�������Ҳ��ţ�û�л�����ͨ���������Ӳ���*/
}

/* ���������ͺ͵�ǰ�ȱ�ǻ�ȡ����Ϣ */
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
                /* �ҵ������� */
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
                /* �ҵ���ǰ������ */
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
                    /* �ҵ��ǵ�ǰ������ */
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

/* �������� */
void SIPCP_AllocConn(ULONG *pulConn)
{
    /*�л�����ͨ�������Ҳ��ţ�û�л�����ͨ���������Ӳ���*/
}

/* �������� */
void SIPCP_ReleaseConn(ULONG *pulConn)
{
    /*�л�����ͨ�������Ҳ��ţ�û�л�����ͨ���������Ӳ���*/
}

/* �������� */
void SIPCP_ActiveConn(ULONG ulConn)
{
    /*�л�����ͨ�������Ҳ��ţ�û�л�����ͨ���������Ӳ���*/
}

/* �������� */
void SIPCP_AllocConf(ULONG ulSpID,ULONG *pulConf)
{
    /*�л�����ͨ�������Ҳ��ţ�û�л�����ͨ���������Ӳ���*/
}

/* �������� */
void SIPCP_KickConnFromConf(ULONG ulConf)
{
    /*�л�����ͨ�������Ҳ��ţ�û�л�����ͨ���������Ӳ���*/
}

/* �����Ӽ�������� */
void SIPCP_AddConnToConf(ULONG ulConf,ULONG ulConn1,ULONG ulConn2)
{
    /*�л�����ͨ�������Ҳ��ţ�û�л�����ͨ���������Ӳ���*/
}
