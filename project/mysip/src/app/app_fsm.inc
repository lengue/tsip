/*******************************************************************************
功能    : 应用模块程序
创建人  : 唐春平
创建日期: 2009.07.19
修改记录:
*******************************************************************************/
ULONG APP_FsmInitProc();
ULONG APP_FsmProc(APP_EVENT_E eEvent);
ULONG APP_Fsm_Idle_OffhookProc();
ULONG APP_Fsm_WaitRemoteAnswer_RemoteAnswerProc();
ULONG APP_Fsm_Active_RemoteReleaseProc();
ULONG APP_Fsm_Active_OnhokProc();
ULONG APP_Fsm_WaitClear_OnhookProc();
ULONG APP_Fsm_Idle_IncommingCallProc();
ULONG APP_Fsm_WaitLocalAnswer_OffhookProc();
ULONG APP_Fsm_Busy_IncommingCallProc();
