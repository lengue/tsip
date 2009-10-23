/*******************************************************************************
功能    : TIMER模块功能函数声明
创建人  : 唐春平
创建日期: 2009.07.19
修改记录:
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
