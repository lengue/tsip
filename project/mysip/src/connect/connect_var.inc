/*******************************************************************************
功能    : SIP协议栈传输层功能函数
创建人  : 唐春平
创建日期: 2009.01.10
修改记录:
*******************************************************************************/
/* 本端主机配置: 必须配置，配置为IP则绑定IP，否则不绑定IP，填写Via头域 */
//UCHAR *g_pucSipTxpHost;

/* 传输层管理，按照传输协议排列，记录该协议下连接队列 */
extern ULONG           g_aulConnTransport[SIP_TRANSPORT_PROTOCOL_BUTT];

/* 监控连接队列 */
extern CONN_LISTEN_S  *g_pstConnListen;
extern CONN_FUNC_MSG_PROC g_pfnConnMsgProc;

/* 连接空闲队列以及连接控制块 */
extern COMM_QUEUE_S    g_stConnCBQueue;
extern CONN_CB_S      *g_pstConnCB;