typedef struct tagSIP_TXN_TIMER_S
{
    SIP_TXN_TIMER_NAME_E    eTimerName;
    ULONG                   ulimerLen;
    ULONG                   hTimerID;
}SIP_TXN_TIMER_S;

typedef struct tagSIP_TXN_CB_S
{
    COMM_CB_HEADER
    ULONG             ulCoreID;
    SIP_TXN_TYPE_E    eType;
    SIP_TXN_STATE_E   eState;
    SIP_LOCATION_S    stPeer;
    UBUF_HEADER_S    *pstUbufInitMsg; /* �����������������Ϣ */
    UBUF_HEADER_S    *pstUbufSendMsg; /* ���һ�η��͵�SIP��Ϣ */
    UBUF_HEADER_S    *pstUbufRecvMsg; /* ���һ���յ���SIP��Ϣ */
    SIP_TXN_TIMER_S   astTimers[SIP_TXN_MAX_FSM_TIMER]; /* ��ʱ������ */
}SIP_TXN_CB_S;

typedef ULONG (*FN_SIP_TXN_ACTION)(ULONG ulTxnID, ULONG ulPara);

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