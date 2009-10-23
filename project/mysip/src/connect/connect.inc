/*******************************************************************************
功能    : 传输层全局变量声明
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/
extern ULONG CONN_ModuleInitProc();
extern ULONG CONN_Config(CONN_CFG_S *g_pstConnCfg);
extern CONN_LISTEN_S* CONN_AddListener(CONN_LOCATION_S *pstLocalLocation);
extern ULONG CONN_AllocConnection(CONN_LOCATION_S *pstPeerLocation,
                           ULONG *pulConnID);
extern ULONG CONN_ActiveConnection(ULONG ulConnID);
extern ULONG CONN_FindConnection(CONN_LOCATION_S *pstPeerLocation,
                             ULONG *pulConnID);
extern VOID CONN_ReleaseConnection(ULONG ulConnID);
extern ULONG CONN_SendInConnection(UCHAR *pucMsg,
                            ULONG ulMsgLen,
                            ULONG ulConnID,
                            CONN_LOCATION_S *pstPeerLocation);
extern ULONG CONN_UDPListenProc(CONN_LISTEN_S *pstListener);
extern DWORD WINAPI CONN_TCPListenTask(LPVOID lpParam);
extern DWORD WINAPI CONN_UDPRecvTask(LPVOID lpParam);
extern DWORD WINAPI CONN_TCPRecvTask(LPVOID lpParam);
extern DWORD WINAPI CONN_TCPConnectTask(LPVOID lpParam);
extern ULONG CONN_SendUDPConnMsg(UCHAR *pucMsg,
                          ULONG ulMsgLen,
                          ULONG ulConnID,
                          CONN_LOCATION_S *pstPeerLocation);
extern ULONG CONN_SendTCPConnMsg(UCHAR *pucMsg, ULONG ulMsgLen, ULONG ulConnID);
extern ULONG CONN_SendBuffMsg(ULONG ulConnID);
extern ULONG CONN_BuffSendMsg(UCHAR *pucMsg,ULONG ulMsgLen, ULONG ulConnID);
extern ULONG CONN_SendMsg(UCHAR *pucTextMsg,
                   ULONG ulMsgLen,
                   SIP_LOCATION_S *pstLocation);