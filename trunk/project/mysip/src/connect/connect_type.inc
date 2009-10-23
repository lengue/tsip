/*******************************************************************************
功能    : 传输层结构定义
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/
/*连接管理结构*/
typedef struct tagCONN_CB_S
{
    UCHAR ucUsed;
    SIP_TRANSPORT_PROTOCOL_E eProtocol;
    ULONG  ulLocalAddr;  /* 本端IP地址 */
    USHORT usLocalPort;  /* 本端端口号 */
    ULONG  ulRemoteAddr; /* 远端IP地址 */
    USHORT usRemotePort; /* 远端端口号 */
    UCHAR  ucState;      /* 连接状态 */
    SOCKET sSocket;      /* 连接关联socket */
    UCHAR  *pucRecvBuff; /* 消息接受缓存，用于流传输协议的定界 */
    UCHAR  *pucSendBuff; /* 消息发送缓存，用于可靠连接 */
    HTIMER hKeepActive;  /* 保活定时器句柄 */
    HANDLE hThread;      /* 关联的收消息线程 */
    ULONG  ulPreConn;    /* 前一个连接，只在控制块被使用是有效 */
    ULONG  ulNextConn;   /* 后一个连接 */
}CONN_CB_S;

/*传输层监听地址结构*/
typedef struct tagCONN_LISTEN_S
{
    CONN_LOCATION_S stLocal;              /* 监听地址 */
    SOCKET          stSocket;             /* 监听socket */
    struct tagCONN_LISTEN_S *pstNext;
}CONN_LISTEN_S;

/*
以后可能出错时需要通知上层，因此需要保存发送缓存结构

|---------|
| 缓存头  |
|---------|
|消息1结构|
|消息1长度|
|---------|
|  消息1  |
|......   |
|---------|
|消息2结构|
|消息2长度|
|---------|
|  消息2  |
|......   |
|---------|
|......   |
|         |
|---------|
*/
typedef struct tagCONN_SEND_BUFF_HEADER_S
{
    ULONG ulBufferLen;  /* 缓存总长度 */
    ULONG ulCurrentLen; /* 使用长度   */
}CONN_SEND_BUFF_HEADER_S;

typedef struct tagCONN_SEND_MSG_HEADER_S
{
    /* 前面可以添加其他结构参数 */
    ULONG ulLen;
}CONN_SEND_MSG_HEADER_S;