/*******************************************************************************
����    : ϵͳ���ƺ���ʵ��
          ��Ҫ����Ϣ���ƺ�ϵͳ��ʼ������
������  : �ƴ�ƽ
��������: 2009.07.19
�޸ļ�¼:
*******************************************************************************/
extern ULONG SYS_SendMsg(ULONG  ulSrcModuleID,
                  ULONG  ulDstModuleID,
                  void  *pstMsg,
                  ULONG  ulLength);
extern ULONG SYS_InitProc();
extern UCHAR *SYS_GetFileContent(UCHAR *pucFileName, ULONG *pulLen);
extern void   SYS_FreeFileContent(UCHAR *pucText);
extern ULONG  SYS_GeneralRandomString(CHAR *pcString, ULONG ulSize);