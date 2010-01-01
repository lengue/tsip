/* IP地址和端口号必须是网络字节序的 */
typedef struct tagCONN_LOCATION_S
{
    CONN_TRANSPORT_PROTOCOL_E eProtocol;
    ULONG  ulIpAddr;
    USHORT usPort;
}CONN_LOCATION_S;

typedef ULONG (* CONN_FUNC_MSG_PROC)(CONN_LOCATION_S *pstLocation,
                                     UCHAR *pucMsg,
                                     ULONG ulMsgLen);
typedef struct tagCONN_CFG_S
{
    UCHAR  ucListenNum;
    CONN_LOCATION_S stListen[CONN_MAX_LISTEN_NUM];
    CONN_FUNC_MSG_PROC pfnMsgProc;
}CONN_CFG_S;
