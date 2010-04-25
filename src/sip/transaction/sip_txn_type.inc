typedef struct tagSIP_TXN_TIMER_S
{
    SIP_TXN_TIMER_NAME_E    eTimerName;
    ULONG                   hTimerID;
}SIP_TXN_TIMER_S;

typedef struct tagSIP_TXN_CB_S
{
    COMM_CB_HEADER
    ULONG             ulCoreID;
    void             *pstHashNode;
    SIP_TXN_TYPE_E    eType;
    SIP_TXN_STATE_E   eState;
    SIP_LOCATION_S    stPeer;
    UCHAR             ucReSendNum;
    UBUF_HEADER_S    *pstUbufRequest;  /* 创建该事务的请求消息 */
    UBUF_HEADER_S    *pstUbufResponse; /* 该事务的响应消息 */
    SIP_TXN_TIMER_S   astTimers[SIP_TXN_MAX_FSM_TIMER]; /* 定时器个数 */
}SIP_TXN_CB_S;

typedef ULONG (*FN_SIP_TXN_ACTION)(ULONG ulTxnID, ULONG ulPara, UBUF_HEADER_S *pstUbufSipMsg);

typedef struct tagSIP_TXN_ACTION_S
{
    FN_SIP_TXN_ACTION pfnProc;
    ULONG             ulPara;
}SIP_TXN_ACTION_S;

typedef struct tagSIP_TXN_FSM_S
{
    ULONG              ulValid;
    SIP_TXN_ACTION_S   astActions[SIP_TXN_MAX_FSM_ACTION];
    SIP_TXN_STATE_E    eNextState;
}SIP_TXN_FSM_S;
