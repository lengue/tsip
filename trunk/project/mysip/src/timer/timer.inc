/*******************************************************************************
����    : TIMERģ�鹦�ܺ�������
������  : �ƴ�ƽ
��������: 2009.07.19
�޸ļ�¼:
*******************************************************************************/
ULONG TIMER_Init();
ULONG TIMER_AllocCB(ULONG *pulTimerID);
VOID TIMER_ReleaseCB(ULONG ulTimerID);
ULONG TIMER_Start(ULONG ulModuleID,
                  ULONG ulName,
                  ULONG ulLen,
                  ULONG ulPara,
                  TIMER_TYPE_E eType,
                  ULONG *pulHandle);
VOID CALLBACK TIMER_TimeoutProc(PVOID lpParameter, BOOLEAN TimerOrWaitFired);
VOID TIMER_Stop(ULONG ulHandle);