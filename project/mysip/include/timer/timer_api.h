/*******************************************************************************
����    : TIMERģ�����APIͷ�ļ�
������  : �ƴ�ƽ
��������: 2009.07.19
�޸ļ�¼:
*******************************************************************************/
extern ULONG TIMER_Init();
extern ULONG TIMER_Start(ULONG ulModuleID,
                          ULONG ulName,
                          ULONG ulLen,
                          ULONG ulPara,
                          TIMER_TYPE_E eType,
                          ULONG *pulHandle);
extern VOID TIMER_Stop(ULONG ulHandle);