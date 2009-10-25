/*******************************************************************************
功能    : 应用模块程序
创建人  : 唐春平
创建日期: 2009.07.19
修改记录:
*******************************************************************************/
extern ULONG APP_MsgProc(ULONG ulModuleID, void* pMsg);
extern ULONG APP_InitProc();
extern ULONG APP_StartUserInterface();
extern ULONG APP_RecvUpMsg(ULONG  ulStackRef1,
                            ULONG  ulStackRef2,
                            ULONG *pulAppRef1,
                            ULONG *pulAppRef2,
                            UBUF_HEADER_S * pstUbufSipMsg);