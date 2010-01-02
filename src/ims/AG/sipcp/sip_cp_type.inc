/*�¼��ṹ*/
#ifndef SIP_CP_TYPE_H
#define SIP_CP_TYPE_H

typedef struct tagSIPCP_EVENT_S
{
    SIPCP_EVENT_E  eEvent;                          /*�����¼�*/
    SIPCP_EVENT_E  eFailEvent;                      /*ҵ���߼�����ʧ�ܵ�ת���¼�*/
    UCHAR        aucParaList[SIPCP_EVENT_PARA_NUM]; /*�¼���������ͬ�¼��������岻ͬ*/
}SIPCP_EVENT_S;

/*�ж��ṹ*/
typedef struct tagSIPCP_ACTION_S
{
    SIPCP_ACTION_E eAction;
    UCHAR          aucParaList[SIPCP_ACTION_PARA_NUM];
}SIPCP_ACTION_S;

/*ҵ���߼������Ľṹ*/
typedef struct tagSIPCP_SL_CORE_S
{
    SIPCP_EVENT_S    stEvent;                     /*�����¼�*/
    ULONG            ulCondition;                 /*����������ο�SIP_CONDITION_E*/
    USHORT           usFlagMask;                  /*���⴦��������룬�ο�SIP_FLAG_E*/
    SIPCP_ACTION_S     astAction[SIPCP_ACTION_NUM]; /*�ж��б�*/
    SIPCP_SERVICE_ID_E eNextHSID;                 /*��һ����ҵ���߼�*/
    USHORT           usSIDMask;                   /*ҵ����������*/
}SIPCP_SL_CORE_S;

/*ҵ���߼�������ṹ*/
typedef struct tagSIPCP_SL_ITEM_S
{
    SIPCP_SL_CORE_S   stSlCore;                  /* ҵ���߼����� */
    USHORT            usNextItem;                /* ��һ�߼������� */
}SIPCP_SL_ITEM_S;

/*ҵ���߼����ұ��ṹ*/
typedef struct tagSIPCP_SL_FIND_S
{
    USHORT usFirstIndex;  /* ��һ��ҵ���߼����� */
    USHORT usLastIndex;   /* ���һ��ҵ���߼����� */
}SIPCP_SL_FIND_S;

/*ҵ���߼��洢���ṹ���洢���ݿ�*/
typedef struct tagSIPCP_SL_STORE_S
{
    SIPCP_SERVICE_ID_E eHSID;             /*����ҵ��ID*/
    SIPCP_LEG_STATE_E  eOLS;              /*������״̬*/
    SIPCP_LEGS_STATE_E eCLS;              /*������״̬*/
    SIPCP_SL_CORE_S    stSlCore;          /*ҵ���߼���*/
}SIPCP_SL_STORE_S;

/*ҵ���߼�ӳ��洢���ṹ���洢���ݿ�*/
typedef struct tagSIPCP_SL_MAPPING_S
{
    UCHAR  ucIsUsed;                   /*ʹ�ñ��*/
    UCHAR  aucVersion[SIPCP_SL_VERSION_LEN];  /*����ҵ��ID*/
    USHORT usFirstIndex;               /*��һ��ҵ���߼����������ҵ���߼����е�Ϊλ��*/
    USHORT ucTotal;                    /*��������*/
}SIPCP_SL_MAPPING_S;

/*refer�������*/
typedef struct tagSIPCP_REFER_PARA_S
{
    UCHAR ucReferType;
    ULONG ulLegID;
}SIPCP_REFER_PARA_S;

typedef struct tagSIPCP_CREATE_PARA_S
{
    UCHAR ucReferType;
    ULONG ulLegID;
}SIPCP_CREATE_PARA_S;

/*MDU��Ϣ�ṹ*/
typedef struct tagSIPCP_MDU_MSG_S
{
    ULONG ulSpID;
    UCHAR ucSeq;
    ULONG ulEmID;
    UCHAR ucBchn;
    ULONG ulLegID;
    SIPCP_MDU_TYPE_E    eMsgType;    /*SIPCP_MDU_MSG_TYPE_E*/
    ULONG               ulCauseCode; /*�ڲ���Ӧ��*/
    struct union
    {
        SIPCP_REFER_PARA_S  stReferPara;
        SIPCP_CREATE_PARA_S stCreatePara;
    }u;                              /*��ͬMDU��Ϣ˽�в���*/
}SIPCP_MDU_MSG_S;

/*��ͼ�����ṹ*/
typedef struct tagSIPCP_DMM_MNT_S
{
    ULONG ulDmmIndex;
    UCHAR ucDialMode;
}SIPCP_DMM_MNT_S;

/*�źŹ����ṹ*/
typedef struct tagSIPCP_SIGNAL_MNT_S
{
    SIPCP_SIGNAL_TYPE_E eSignalID;
}SIPCP_SIGNAL_MNT_S;

/*���ӹ����ṹ*/
typedef struct tagSIPCP_CONN_MNT_S
{
    ULONG ulConnID;                    /*����ID*/
    UCHAR ucRefLegSeq;                 /*���������*/
    UCHAR ucLocalPlay;                 /*���˷������*/
    UCHAR ucRemotePlay;                /*Զ�˷������*/
    UCHAR ucMix;                       /*�������*/
}SIPCP_CONN_MNT_S;

/*��Դ�����ṹ*/
typedef struct tagSIPCP_RESOURCE_MNT_S
{
    ULONG ulConfID;                                 /*������ԴID*/
    UCHAR ucConnNum;                                /*������Ŀ*/
    UCHAR ucResvConn;                               /*����������ţ������ͷ�*/
    SIPCP_CONN_MNT_S astConn[SIPCP_CONN_NUM_IN_SPCB]; /*��������*/
}SIPCP_RESOURCE_MNT_S;

/*�����Խṹ*/
typedef struct tagSIPCP_MEDIA_ATTR_S
{
    ULONG ulTmp;
}SIPCP_MEDIA_ATTR_S;

/*******************************************************************************
�ȹ����ṹ�е������Ա�ʾ�������Ѿ�Э�̺õ������ԣ�
�ȿ��ƽṹ�е������Ա�ʾ����������Э�̵�������
��������(RTP�����¼���Ǵ�ͨ��ʹ�õ�������)
*******************************************************************************/
/*ҵ���ȹ����ṹ*/
typedef struct tagSIPCP_LEG_MNT_S
{
    ULONG              ulLegID;         /*�ȿ��ƿ�����,NULL_ULONGΪ��Чֵ*/
    SIPCP_LEG_STATE_E  eState;          /*��״̬*/
    SIPCP_MEDIA_ATTR_S stMediaAttr;     /*�ȵ�ǰ��������*/
    UCHAR              ucNegotiate;     /*�Ƿ�����Э��*/
    UCHAR              ucConnSeq;       /*�����������*/
    UCHAR              bFocus :1;       /*Focus���*/
}SIPCP_LEG_MNT_S;

typedef struct tagSIPCP_OBSERVE_EVENT_S
{
    USHORT         ulSlIndex;                         /*ҵ���߼�������*/
    SIPCP_EVENT_E  eEvent;                            /*�����¼�*/
    UCHAR          aucParaList[SIPCP_EVENT_PARA_NUM]; /*�¼���������ͬ�¼��������岻ͬ*/
}SIPCP_OBSERVE_EVENT_S;

/*ҵ���߼������ṹ*/
typedef struct tagSIPCP_SERVICE_MNT_S
{
    SIPCP_SERVICE_ID_E eHomeSID;                      /*����ҵ��ID*/
    SIPCP_LEG_STATE_E  eOLS;                          /*���������״̬*/
    SIPCP_LEGS_STATE_E eICLS;                         /*�����ۺϺ�����״̬*/
    SIPCP_SERVICE_ID_E eCurSID;                       /*��ǰҵ��ID*/
    UCHAR              ucCurLegSeq;                   /*��ǰ������*/
    UCHAR              ucCtlLegSeq;                   /*����������*/
    UCHAR              ucValidLegNum;                 /*��Ч����Ŀ*/
    UCHAR              ucAllocedLegNum;               /*ռ������Ŀ*/
    SIPCP_LEG_MNT_S    astLeg[SIPCP_LEG_NUM_IN_SPCB]; /*�ȼ�¼*/
    HTIMER             hSrvTimer;                     /*ҵ��ʱ�����*/
    SIPCP_TIMER_E      eSrvTimerName;                 /*ҵ��ʱ������*/
    ULONG              ulSrvTimerLen;                 /*ҵ��ʱ��ʱ��*/
    SIPCP_OBSERVE_EVENT_S astOEvent[SIPCP_MAX_OEVENT_NUM]; /*�ȼ�¼*/
}SIPCP_SERVICE_MNT_S;

/*ҵ���߼������ṹ*/
typedef struct tagSIPCP_SERVICE_RIGHT_S
{
    UCHAR  ulSrvPriority[SIPCP_SERVICE_ID_BUTT];  /*ҵ�����ȼ�*/
    ULONG  ulSrvMask;                             /*ҵ��֧������*/
    ULONG  ulHotlineLen;                          /*����ʱ��*/
}SIPCP_SERVICE_RIGHT_S;

/*ҵ����ƿ�*/
typedef struct tagSIPCP_SPCB_S
{
    UCHAR ucUsed;

    /*��������BasicAttr*/
    ULONG ulEmID;
    UCHAR ucBsn;
    ULONG ulVagNo;

    /*��̬����DynamicAttr*/
    UCHAR bWaitRelease :1;/*�ȴ��ͷű��*/
    UCHAR bDiscFlag    :1;/*ժ�һ���־*/
    UCHAR bReversePol  :1;/*�����Ա��*/
    UCHAR bAoceReady   :1;/*AOCE���ͱ��*/

    /*��չ����ExternAttr*/
    UCHAR *pstAoceStr;

    /*��ͼ�ӿ��ƿ�*/
    SIPCP_DMM_MNT_S stDmmCtr;

    /*�ź��ӿ��ƿ�*/
    SIPCP_SIGNAL_MNT_S stSignal;

    /*��Դ�ӿ��ƿ�*/
    SIPCP_RESOURCE_MNT_S stResource;

    /*ҵ���߼��ӿ��ƿ�*/
    SIPCP_SERVICE_MNT_S  stService;

    /*ҵ��Ȩ�޻���*/
    SIPCP_SERVICE_RIGHT_S stSrvRight;

    USHORT usNextIndex;
}SIPCP_SPCB_S;

#else
#endif