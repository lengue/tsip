
typedef struct tagSIP_SESSION_S
{
    /* 会话状态 */
    SIP_NEGO_STATE_E ucSessionState;

    /* 早会话状态 */
    SIP_NEGO_STATE_E ucEarlySessionState;
}SIP_SESSION_S;


typedef struct tagSIP_SUBSCRIPTION_S
{
    UCHAR ucRole;/* 角色，订阅者还是通知者 */
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

typedef struct tagSIP_DLG_CB_S
{
    COMM_CB_HEADER
    UCHAR          ucState;         /* early/conformed */
    UBUF_HEADER_S *pstDialogID;     /* SIP_DIALOG_ID_S */
    UBUF_HEADER_S *pstLocalURI;     /* URI_S */
    UBUF_HEADER_S *pstRemoteURI;    /* URI_S */
    UBUF_HEADER_S *pstRemoteTarget; /* URI_S */
    UBUF_HEADER_S *pastRouteSet;    /* SIP_ROUTE_PARAM_S */
    ULONG  ulLocalSeq;
    ULONG  ulRemoteSeq;
    UCHAR  ucSecureFlag;
    void  *pstHashNode;
}SIP_DLG_CB_S;

typedef struct tagSIP_DIALOG_ID_S
{
    UCHAR *pucCallID;
    UCHAR *pucLocalTag;
    UCHAR *pucRemoteTag;
}SIP_DIALOG_ID_S;