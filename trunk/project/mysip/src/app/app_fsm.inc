/*******************************************************************************
����    : Ӧ��ģ�����
������  : �ƴ�ƽ
��������: 2009.07.19
�޸ļ�¼:
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