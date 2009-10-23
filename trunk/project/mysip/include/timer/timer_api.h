/*******************************************************************************
功能    : TIMER模块对外API头文件
创建人  : 唐春平
创建日期: 2009.07.19
修改记录:
*******************************************************************************/
extern ULONG TIMER_Init();
extern ULONG TIMER_Start(ULONG ulModuleID,
                          ULONG ulName,
                          ULONG ulLen,
                          ULONG ulPara,
                          TIMER_TYPE_E eType,
                          ULONG *pulHandle);
extern VOID TIMER_Stop(ULONG ulHandle);