/*******************************************************************************
����    : SIPЭ��ջ����㹦�ܺ���
������  : �ƴ�ƽ
��������: 2009.01.10
�޸ļ�¼:
*******************************************************************************/
extern SIP_TXN_CB_S  *g_pstSipTxnCB;
extern COMM_QUEUE_S   g_stSipTxnCBQueue;
extern void          *g_pstSipTxnCBHash;
extern UBUF_HEADER_S *g_pstSipTxnRecvMsg;
extern SIP_TXN_FSM_S g_astSipTxnFsm[SIP_TXN_TYPE_BUTT][SIP_TXN_STATE_TERMINATED][SIP_TXN_EVENT_BUTT];
extern ULONG g_ulSipTxnT1;