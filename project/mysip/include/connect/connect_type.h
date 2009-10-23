/* IP地址和端口号必须是网络字节序的 */
typedef struct tagCONN_LOCATION_S
{
    SIP_TRANSPORT_PROTOCOL_E eProtocol;
    ULONG   ulIpAddr;/* IPV4 */
    USHORT usPort;
}CONN_LOCATION_S;

typedef ULONG (* CONN_FUNC_MSG_PROC)(SIP_LOCATION_S *pstLocation,
                                     UCHAR *pucMsg,
                                     ULONG ulMsgLen);
typedef struct tagCONN_CFG_S
{
    UCHAR  ucListenNum;
    CONN_LOCATION_S stListen[SIP_TXP_MAX_LISTEN_NUM];
    CONN_FUNC_MSG_PROC pfnMsgProc;
}CONN_CFG_S;
