/*******************************************************************************
����    : �ṩ��������
������  : �ƴ�ƽ
��������: 200.01.10
�޸ļ�¼:
*******************************************************************************/
ULONG SYS_SendMsg(ULONG  ulSrcModuleID,
                  ULONG  ulDstModuleID,
                  void  *pstMsg,
                  ULONG  ulLength);
DWORD WINAPI SYS_ModuleMsgProcTask(PVOID pvParam);
ULONG SYS_InitProc();