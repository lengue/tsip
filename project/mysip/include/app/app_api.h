/*******************************************************************************
����    : Ӧ��ģ�����
������  : �ƴ�ƽ
��������: 2009.07.19
�޸ļ�¼:
*******************************************************************************/
extern ULONG APP_MsgProc(ULONG ulModuleID, void* pMsg);
extern ULONG APP_InitProc();
extern ULONG APP_StartUserInterface();
extern ULONG APP_RecvUpMsg(ULONG  ulStackRef1,
                            ULONG  ulStackRef2,
                            ULONG *pulAppRef1,
                            ULONG *pulAppRef2,
                            UBUF_HEADER_S * pstUbufSipMsg);