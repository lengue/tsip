/*******************************************************************************
����    : �����ṹ����
������  : �ƴ�ƽ
��������: 200.01.10
�޸ļ�¼:
*******************************************************************************/
/*���ӹ����ṹ*/
typedef struct tagCONN_CB_S
{
    UCHAR ucUsed;
    CONN_TRANSPORT_PROTOCOL_E eProtocol;
    ULONG  ulLocalAddr;  /* ����IP��ַ�������ֽ��� */
    USHORT usLocalPort;  /* ���˶˿ںţ������ֽ��� */
    ULONG  ulRemoteAddr; /* Զ��IP��ַ�������ֽ��� */
    USHORT usRemotePort; /* Զ�˶˿ںţ������ֽ��� */
    UCHAR  ucState;      /* ����״̬ */
    SOCKET sSocket;      /* ���ӹ���socket */
    UCHAR  *pucRecvBuff; /* ��Ϣ���ܻ��棬����������Э��Ķ��� */
    UCHAR  *pucSendBuff; /* ��Ϣ���ͻ��棬���ڿɿ����� */
    HTIMER hKeepActive;  /* ���ʱ����� */
    HANDLE hThread;      /* ����������Ϣ�߳� */
    ULONG  ulPreConn;    /* ǰһ�����ӣ�ֻ�ڿ��ƿ鱻ʹ������Ч */
    ULONG  ulNextConn;   /* ��һ������ */
}CONN_CB_S;

/*����������ַ�ṹ*/
typedef struct tagCONN_LISTEN_S
{
    CONN_LOCATION_S stLocal;              /* ������ַ */
    SOCKET          stSocket;             /* ����socket */
    struct tagCONN_LISTEN_S *pstNext;
}CONN_LISTEN_S;

/*
�Ժ���ܳ���ʱ��Ҫ֪ͨ�ϲ㣬�����Ҫ���淢�ͻ���ṹ

|---------|
| ����ͷ  |
|---------|
|��Ϣ1�ṹ|
|��Ϣ1����|
|---------|
|  ��Ϣ1  |
|......   |
|---------|
|��Ϣ2�ṹ|
|��Ϣ2����|
|---------|
|  ��Ϣ2  |
|......   |
|---------|
|......   |
|         |
|---------|
*/
typedef struct tagCONN_SEND_BUFF_HEADER_S
{
    ULONG ulBufferLen;  /* �����ܳ��� */
    ULONG ulCurrentLen; /* ʹ�ó���   */
}CONN_SEND_BUFF_HEADER_S;

typedef struct tagCONN_SEND_MSG_HEADER_S
{
    /* ǰ��������������ṹ���� */
    ULONG ulLen;
}CONN_SEND_MSG_HEADER_S;