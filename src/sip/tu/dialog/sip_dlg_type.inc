
typedef struct tagSIP_SESSION_S
{
    /* �Ự״̬ */
    SIP_NEGO_STATE_E ucSessionState;

    /* ��Ự״̬ */
    SIP_NEGO_STATE_E ucEarlySessionState;
}SIP_SESSION_S;


typedef struct tagSIP_SUBSCRIPTION_S
{
    UCHAR ucRole;/* ��ɫ�������߻���֪ͨ�� */
    ULONG ulDuration;
    HTIMER hRefreshTimer;
    HTIMER hRefreshTimer;
}SIP_SUBSCRIPTION_S;


typedef struct tagSIP_APPLICATION_S
{
    ULONG ulDlgIndex;
    SIP_APPLICATION_TYPE_E eTypde;
    union
    {
        SIP_SESSION_S stSession;
        SIP_SUBSCRIPTION_S stSubscription;
    }uApplication;
}SIP_APPLICATION_S;


typedef struct tagSIP_DIALOG_S
{
    UCHAR aucDialogID[10];
    UCHAR aucLocalTag[10];
    UCHAR aucRemoteTag[10];

    UCHAR ucState; /*init/early/conformed*/
    ULONG ucLocalSeq;
    ULONG ucRemoteSeq;
    SIP_URI_S stLocalURI;
    SIP_URI_S stRemoteURI;
    HOST_PORT_S stRemoteTarget;
    SIP_URI_S   astRouteSet[10];
    UCHAR ucSecureFlag;

    UCHAR ustApp[10];/* �¹ҵĶ��ĺͻỰ */
}SIP_DIALOG_S;
