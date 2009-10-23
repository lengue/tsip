/*******************************************************************************
功能    : 程序主文件头文件
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/
#if 0
extern ULONG SIP_InitProc();
extern ULONG SIP_MsgProc(ULONG ulModuleID, void *pMsg);

typedef struct tagSYS_SIP_MSG_S
{
    SIP_LOCATION_S stLocation;
    ULONG              ulConnID;
    ULONG              ulDialogID;
    ULONG              ulTransationID;
    UBUF_HEADER_S     *pstSipMsgUbuf;
}SYS_SIP_MSG_S;
#endif