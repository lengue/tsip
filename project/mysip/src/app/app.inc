/*******************************************************************************
����    : Ӧ��ģ�����
������  : �ƴ�ƽ
��������: 2009.07.19
�޸ļ�¼:
*******************************************************************************/
ULONG APP_StartUserInterface();
ULONG APP_InitProc();
ULONG APP_TimerProc(TIMER_MSG_S *pstTimerMsg);
ULONG APP_MsgProc(ULONG ulModuleID, void* pMsg);
ULONG APP_SipMsgProc(ULONG ulDlgID,
                     ULONG ulTxnID,
                     UBUF_HEADER_S *pstUbufSipMsg);
ULONG APP_SipRequestProc(ULONG ulDlgID,
                         ULONG ulTxnID,
                         UBUF_HEADER_S *pstUbufSipMsg);
ULONG APP_SipResponseProc(ULONG ulDlgID,
                          ULONG ulReqID,
                          UBUF_HEADER_S *pstUbufSipMsg);