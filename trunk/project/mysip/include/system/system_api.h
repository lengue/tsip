/*******************************************************************************
功能    : 系统机制函数实现
          主要是消息机制和系统初始化机制
创建人  : 唐春平
创建日期: 2009.07.19
修改记录:
*******************************************************************************/
extern ULONG SYS_SendMsg(ULONG  ulSrcModuleID,
                  ULONG  ulDstModuleID,
                  void  *pstMsg,
                  ULONG  ulLength);
extern ULONG SYS_InitProc();
extern UCHAR *SYS_GetFileContent(UCHAR *pucFileName, ULONG *pulLen);
extern void   SYS_FreeFileContent(UCHAR *pucText);
extern ULONG  SYS_GeneralRandomString(CHAR *pcString, ULONG ulSize);