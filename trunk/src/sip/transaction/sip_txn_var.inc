/*******************************************************************************
功能    : SIP协议栈传输层功能函数
创建人  : 唐春平
创建日期: 2009.01.10
修改记录:
*******************************************************************************/
extern SIP_TXN_CB_S  *g_pstSipTxnCB;
extern COMM_QUEUE_S   g_stSipTxnCBQueue;
extern void          *g_pstSipTxnCBHash;
extern SIP_TXN_FSM_S g_astSipTxnFsm[SIP_TXN_TYPE_BUTT][SIP_TXN_STATE_TERMINATED][SIP_TXN_EVENT_BUTT];
extern ULONG g_ulSipTxnT1;