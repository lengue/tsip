/*******************************************************************************
������Ϣ����������:ͨ�ô���->ҵ���߼�->��������
ͨ�ô����ʾ����Ҫҵ���߼������������ҵ���߼��Ƿ���Ӧ�����Ĳ���
�����Ҫ���ύҵ���߼���
���ҵ���߼������ش���(����ʧ�ܻ�����û��ƥ����)���ύ��������

ҵ���߼�����ʧ�ܱ��뱣��ԭ״̬����
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

/*��Ҫ���й��ȵ�MDU��Ϣ����*/
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

/*��Ҫ���û�����Ϣ*/
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

/*��Ҫ�Ƕ��ĺ��ͷ���Ϣ*/
void SIPCP_EmMsgProc(ULONG ulSpID)
{
    return;
}

/*��Ҫ��DSP��ص���Ϣ*/
void SIPCP_MpMsgProc(ULONG ulSpID)
{
    return;
}

/*��ʱ����ʱ��һ����ҵ���߼�����*/
void SIPCP_TimerMsgProc(ULONG ulSpID)
{
    return;
}

/*��Ϣ��ͨ�ô��������Ҫҵ���߼����������������pucEvent��������Ϊ�Ϸ���Чֵ*/
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
                /*����������ռ䣬����ʧ��ֱ���ͷŸ���*/
                /*����ɹ�*/
            };
            break;

        default:
            break;
    }
    return SUCCESS;
}

/*��Ϣ�ı�������ֻ��Ҫ��ҵ���߼�����ʧ�ܲ���*/
void SIPCP_LegMsg_ProtectProc(SIPCP_MDU_MSG_S *pstMduMsg)
{
    SIPCP_MDU_MSG_S stMduMsg;
    switch (pstMduMsg->cMsgType)
    {
        /*û��ҵ���߼�������������Ҫ�ͷŸ���*/
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

/*��Ϣ��ͨ�ô��������Ҫҵ���߼����������������pucEvent��������Ϊ�Ϸ���Чֵ*/
ULONG SIPCP_UserMsg_CommProc(ULONG          *pulSpID,
                            SIPCP_MDU_MSG_S *pstMduMsg,
                            UCHAR           *pucEvent,
                            UCHAR            ucFinished)
{
    if(*pulSpID == NULL_ULONG)&&(pstMduMsg->cMsgType != OFF_HOOK))
    {
        /*��ժ��������SPIDΪNULL��������*/
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

        /*ժ���ñ��*/
        /*�һ��ñ��*/
        /*FSK_START�����ź�ģ��*/
        default:
            break;
    }

    return SUCCESS;
}

/*��Ϣ�ı�������ֻ��Ҫ��ҵ���߼�����ʧ�ܲ���*/
void SIPCP_UserMsg_ProtectProc(SIPCP_MDU_MSG_S *pstMduMsg)
{
    SIPCP_MDU_MSG_S stMduMsg;
    switch (pstMduMsg->cMsgType)
    {
        /*�һ��ͷ�����ҵ��*/
        default:
            break;
    }

    return;
}


/*���г�ʼ��*/
void SIPCP_CallerInit(SIPCP_MDU_MSG_S *pstMduMsg)
{
    /* 1������SPID�ͱ������ӣ�����ʧ���ͷ���������Դ��ֱ�ӷ��� */
    /* 2������ɹ��ж��Ƿ���ܺ��У����������ֱ�ӷ�æ������������ */
    return;
}

/*���г�ʼ��*/
void SIPCP_CalleeInit(SIPCP_MDU_MSG_S *pstMduMsg)
{
    ULONG ulEmID;
    ULONG ulSpID;

    SIPEM_FromEmIDGetSpID(ulEmID, &ulSpID);
    /* 1�����ж��Ƿ���ܺ��У����������ֱ���ͷŸ��� */
    /* 2������SPID�ͱ������ӣ�����ʧ���ͷ���������Դͬʱ�Ƿ��Ǹ����ȣ��������� */
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

/*���г�ʼ��*/
ULONG SIPCP_CanAcceptCall(ULONG ulEmID)
{
    return SUCCESS;
}

/*���г�ʼ��*/
ULONG SIPEM_FromEmIDGetSpID(ULONG ulEmID,ULONG *pulSpID)
{
    return SUCCESS;
}

/*���г�ʼ��*/
ULONG SIPCP_CheckReleaseSPCB(ULONG ulEmID,ULONG *pulSpID)
{
    return SUCCESS;
}