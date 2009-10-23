/*******************************************************************************
功能    : 提供函数声明
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/
ULONG SYS_SendMsg(ULONG  ulSrcModuleID,
                  ULONG  ulDstModuleID,
                  void  *pstMsg,
                  ULONG  ulLength);
DWORD WINAPI SYS_ModuleMsgProcTask(PVOID pvParam);
ULONG SYS_InitProc();
