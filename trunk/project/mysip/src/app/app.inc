/*******************************************************************************
功能    : 应用模块程序
创建人  : 唐春平
创建日期: 2009.07.19
修改记录:
*******************************************************************************/
ULONG APP_StartUserInterface();
ULONG APP_InitProc();
ULONG APP_TimerProc(TIMER_MSG_S *pstTimerMsg);
ULONG APP_MsgProc(ULONG ulModuleID, void* pMsg);
ULONG APP_SipRequestProc(ULONG ulDlgID,
                         ULONG ulTxnID,
                         UBUF_HEADER_S *pstUbufSipMsg);
ULONG APP_SipResponseProc(ULONG ulDlgID,
                          ULONG ulReqID,
                          UBUF_HEADER_S *pstUbufSipMsg);
ULONG APP_SendDownMsg(ULONG ulAppRef1,
                       ULONG ulAppRef2,
                       ULONG ulStackRef1,
                       ULONG ulStackRef2,
                       UBUF_HEADER_S *pstUbufSipMsg);
ULONG APP_RecvUpMsg(ULONG ulStackRef1,
                    ULONG ulStackRef2,
                    ULONG ulAppRef1,
                    ULONG ulAppRef2,
                    UBUF_HEADER_S * pstUbufSipMsg);