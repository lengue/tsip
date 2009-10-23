/*******************************************************************************
����    : SIPЭ��ջ����㹦�ܺ���
������  : �ƴ�ƽ
��������: 2009.01.10
�޸ļ�¼:
*******************************************************************************/
/* �ⲿ����ģ��ͷ�ļ� */
#include "common\common.h"
#include "ubuf\ubuf.h"
#include "sip\uri.h"
#include "sip\sip.h"

/* ��ģ������ṩ�ĳ����ͽṹͷ�ļ� */
#include "connect\connect.h"

/* ��ģ���ڲ�ʹ�õĳ����ͽṹͷ�ļ� */
#include "connect_const.inc"
#include "connect_type.inc"

/* ��ģ���ڲ���������ͷ�ļ� */
#include "connect.inc"

/* ��ģ��ȫ�ֱ�������ͷ�ļ� */
#include "connect_var.inc"

ULONG CONN_ModuleInitProc()
{
    #if 0
    ULONG ulRet;
    SIP_TXP_CFG_S stSipCfg;

    memset(&stSipCfg, 0,sizeof(SIP_TXP_CFG_S));
    ulRet = SIP_ParseConfig(&stSipCfg);
    if (ulRet != SUCCESS)
    {
        printf("SIP_ParseConfig failed");
        return FAIL;
    }
    #endif
    return SUCCESS;
}

ULONG CONN_Config(CONN_CFG_S *g_pstConnCfg)
{
    CONN_LOCATION_S *pstCfg;
    DWORD          dwThreadID;
    HANDLE         hThread;
    WSADATA        wsd;
    ULONG          ulSize;
    ULONG          ulLoop;
    ULONG          ulRet;
    CONN_LISTEN_S *pstListener = NULL_PTR;

    /* ���ӿ��ƿ��ڴ����뼰��ʼ�� */
    ulSize = CONN_MAX_CONN * sizeof(CONN_CB_S);
    g_pstConnCB = (CONN_CB_S *)malloc(ulSize);
    if (g_pstConnCB == NULL_PTR)
    {
        printf("malloc for g_pstConnCB failed!\r\n");
        return FAIL;
    }

    ZeroMemory(g_pstConnCB, ulSize);
    for (ulLoop = 0; ulLoop < CONN_MAX_CONN; ulLoop++)
    {
        g_pstConnCB[ulLoop].ucUsed = FALSE;
        g_pstConnCB[ulLoop].ulPreConn = NULL_ULONG;
        g_pstConnCB[ulLoop].ulNextConn = ulLoop + 1;
    }
    g_pstConnCB[CONN_MAX_CONN - 1].ulNextConn = NULL_ULONG;

    /* ���ж��г�ʼ�� */
    g_stConnCBQueue.ulNum = CONN_MAX_CONN;
    g_stConnCBQueue.ulHead = 0;
    g_stConnCBQueue.ulTail = CONN_MAX_CONN - 1;

    memset(g_aulConnTransport, 0xff, sizeof(g_aulConnTransport));

    g_pfnConnMsgProc = g_pstConnCfg->pfnMsgProc;

    /* �������������� */
    if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
    {
        printf("WSAStartup failed!\r\n");
        return FAIL;
    }

    for (ulLoop = 0; ulLoop < g_pstConnCfg->ucListenNum; ulLoop++)
    {
        pstCfg = &g_pstConnCfg->stListen[ulLoop];

        /*����Э���ʼ������Э��*/
        switch (pstCfg->eProtocol)
        {
            case SIP_TRANSPORT_PROTOCOL_UDP:
                pstListener = CONN_AddListener(pstCfg);
                ulRet = CONN_UDPListenProc(pstListener);
                if (ulRet != SUCCESS)
                {
                    return ulRet;
                }
                break;

            case SIP_TRANSPORT_PROTOCOL_TCP:
                /* TCP��Ҫһֱ����������Ҫ�������������� */
                pstListener = CONN_AddListener(pstCfg);
                hThread = CreateThread(NULL,
                                       0,
                                       CONN_TCPListenTask,
                                       (PVOID)pstListener,
                                       0,
                                       &dwThreadID);
                CloseHandle(hThread);
                break;

            /* �����Ĵ���Э����ʱ��֧�� */
            case SIP_TRANSPORT_PROTOCOL_TLS:
            case SIP_TRANSPORT_PROTOCOL_SCTP:
                break;

            default:
                return FAIL;
        }

    }

    return SUCCESS;
}

CONN_LISTEN_S* CONN_AddListener(CONN_LOCATION_S *pstLocalLocation)
{
    CONN_LISTEN_S **ppstListener = NULL_PTR;
    CONN_LISTEN_S  *pstListener  = NULL_PTR;

    ppstListener = &g_pstConnListen;
    while(*ppstListener != NULL_PTR)
    {
        ppstListener = &(*ppstListener)->pstNext;
    }

    pstListener = (CONN_LISTEN_S *)malloc(sizeof(CONN_LISTEN_S));
    memcpy(&pstListener->stLocal, pstLocalLocation, sizeof(SIP_LOCATION_S));
    //pstListener->stSocket = NULL_ULONG;
    pstListener->pstNext  = NULL_PTR;

    *ppstListener = pstListener;
    return pstListener;
}

/* �������ӣ��������ӿ��ƿ������͹��� */
ULONG CONN_AllocConnection(CONN_LOCATION_S *pstPeerLocation,
                           ULONG *pulConnID)
{
    ULONG ulConnID;
    ULONG ulTempConnID;
    CONN_SEND_BUFF_HEADER_S *pstTemp = NULL_PTR;
    CONN_CB_S *pstConnCB = NULL_PTR;
    CONN_CB_S *pstTempConnCB = NULL_PTR;

    /* ���û�����ӿ��ƿ��ʧ���� */
    if(g_stConnCBQueue.ulNum == 0)
    {
        return FAIL;
    }

    /* ������ƿ� */
    g_stConnCBQueue.ulNum--;
    ulConnID = g_stConnCBQueue.ulHead;
    pstConnCB = &g_pstConnCB[ulConnID];

    if (g_stConnCBQueue.ulNum == 0)
    {
        g_stConnCBQueue.ulHead = NULL_ULONG;
        g_stConnCBQueue.ulTail = NULL_ULONG;
    }
    else
    {
        g_stConnCBQueue.ulHead = pstConnCB->ulNextConn;
    }

    /* ���ӳ�ʼ�� */
    pstConnCB->ucUsed = TRUE;
    pstConnCB->eProtocol    = pstPeerLocation->eProtocol;
    pstConnCB->ulLocalAddr  = NULL_ULONG;
    pstConnCB->usLocalPort  = NULL_USHORT;
    pstConnCB->ulRemoteAddr = pstPeerLocation->ulIpAddr;
    pstConnCB->usRemotePort = pstPeerLocation->usPort;
    pstConnCB->ucState      = CONN_STATE_DEACTIVE;
    pstConnCB->sSocket      = INVALID_SOCKET;
    pstConnCB->pucRecvBuff  = malloc(CONN_RECV_BUFFER);
    pstConnCB->hKeepActive  = NULL_PTR;

    /* ���ͻ���˽���нṹ�� */
    pstConnCB->pucSendBuff  = malloc(CONN_SEND_BUFFER + sizeof(CONN_SEND_BUFF_HEADER_S));
    pstTemp = (CONN_SEND_BUFF_HEADER_S *)pstConnCB->pucSendBuff;
    pstTemp->ulBufferLen  = CONN_SEND_BUFFER;
    pstTemp->ulCurrentLen = 0;

    /* ���ӹ��أ����ڶ���ǰ�棬�޸�˫������ */
    pstConnCB->ulPreConn    = NULL_ULONG;
    pstConnCB->ulNextConn   = g_aulConnTransport[pstPeerLocation->eProtocol];
    ulTempConnID = pstConnCB->ulNextConn;
    if (ulTempConnID != NULL_ULONG)
    {
        pstTempConnCB = &g_pstConnCB[ulTempConnID];
        pstTempConnCB->ulPreConn = ulConnID;
    }

    g_aulConnTransport[pstPeerLocation->eProtocol] = ulConnID;

    /* ���Ӽ���Ĺ������ݽ������Ӻͷ������Ӳ�ͬ���ھ���Ĳ�������ִ�� */
    *pulConnID = ulConnID;
    return SUCCESS;
}

/* ���ӵļ��û�н�������Ҫ������ͬ�±��ʱ����Ҫ����һ�� */
ULONG CONN_ActiveConnection(ULONG ulConnID)
{
    DWORD          dwThreadID;
    HANDLE         hThread;

    switch (g_pstConnCB[ulConnID].eProtocol)
    {
        /* UDP����Ҫ���� */
        case SIP_TRANSPORT_PROTOCOL_UDP:
            break;


        case SIP_TRANSPORT_PROTOCOL_TCP:
            /* �Ǽ���̬��Ҫ�������Ӽ��� */
            if (g_pstConnCB[ulConnID].ucState != CONN_STATE_ACTIVE)
            {
                hThread = CreateThread(NULL,
                                       0,
                                       CONN_TCPConnectTask,
                                       (PVOID)ulConnID,
                                       0,
                                       &dwThreadID);
                CloseHandle(hThread);
            }

            /* ����һ�����ӱ��ʱ�� */
            break;

        /* �ݲ�֧�� */
        case SIP_TRANSPORT_PROTOCOL_SCTP:
        case SIP_TRANSPORT_PROTOCOL_TLS:
            break;

        default:
            return FAIL;
    }

    return SUCCESS;
}

/* ����Զ�˵�ַ�������� */
ULONG CONN_FindConnection(CONN_LOCATION_S *pstPeerLocation,
                          ULONG *pulConnID)
{
    ULONG ulConnID;
    CONN_CB_S *pstConnCB = NULL_PTR;

    /* ����Э�������ҵ����ӿ��ƿ������ʼ���� */
    ulConnID = g_aulConnTransport[pstPeerLocation->eProtocol];

    /* �ڶ����в��� */
    while (ulConnID != NULL_ULONG)
    {
        pstConnCB = &g_pstConnCB[ulConnID];
        if (((pstConnCB->usRemotePort == NULL_USHORT)
           ||(pstConnCB->usRemotePort == pstPeerLocation->usPort))
          &&((pstConnCB->ulRemoteAddr == NULL_ULONG)
           ||(pstConnCB->ulRemoteAddr == pstPeerLocation->ulIpAddr)))
        {
            *pulConnID = ulConnID;
            return SUCCESS;
        }

        ulConnID = pstConnCB->ulNextConn;
    }

    return FAIL;
}

VOID CONN_ReleaseConnection(ULONG ulConnID)
{
    ULONG ulTempConnID;
    ULONG ulPreConnID;
    ULONG ulNextConnID;
    CONN_CB_S *pstConnCB     = NULL_PTR;
    CONN_CB_S *pstTempConnCB = NULL_PTR;

    pstConnCB = &g_pstConnCB[ulConnID];

    /* ж������ */
    ulPreConnID  = pstConnCB->ulPreConn;
    ulNextConnID = pstConnCB->ulNextConn;
    if (ulNextConnID != NULL_ULONG)
    {
        pstTempConnCB = &g_pstConnCB[ulNextConnID];
        pstTempConnCB->ulPreConn = ulPreConnID;
    }

    if (ulPreConnID != NULL_ULONG)
    {
        pstTempConnCB = &g_pstConnCB[ulPreConnID];
        pstTempConnCB->ulNextConn = ulNextConnID;
    }
    else
    {
        g_aulConnTransport[pstConnCB->eProtocol] = ulNextConnID;
    }

    /* �ͷ���Դ */
    free(pstConnCB->pucRecvBuff);
    free(pstConnCB->pucRecvBuff);

    /* �ͷ�socket */


    /* �ͷ����ӿ��ƿ� */
    pstConnCB->ucUsed = FALSE;
    pstConnCB->ulNextConn = NULL_ULONG;

    g_stConnCBQueue.ulNum++;
    if (g_stConnCBQueue.ulNum == 1)
    {
        g_stConnCBQueue.ulHead = ulConnID;
        g_stConnCBQueue.ulTail = ulConnID;
    }
    else
    {
        ulTempConnID = g_stConnCBQueue.ulTail;
        pstTempConnCB = &g_pstConnCB[ulTempConnID];
        pstTempConnCB->ulNextConn = ulConnID;
        g_stConnCBQueue.ulTail = ulConnID;
    }

    return;
}

/*���ӵļ����Ҫ�Ǳ��˷�������������*/
ULONG CONN_SendInConnection(UCHAR *pucMsg,
                            ULONG ulMsgLen,
                            ULONG ulConnID,
                            CONN_LOCATION_S *pstPeerLocation)
{
    ULONG ulRet = FAIL;

    switch (g_pstConnCB[ulConnID].eProtocol)
    {
        case SIP_TRANSPORT_PROTOCOL_TCP:
            ulRet = CONN_SendTCPConnMsg(pucMsg, ulMsgLen, ulConnID);
            break;

        case SIP_TRANSPORT_PROTOCOL_UDP:
            ulRet = CONN_SendUDPConnMsg(pucMsg, ulMsgLen, ulConnID, pstPeerLocation);
            break;

        case SIP_TRANSPORT_PROTOCOL_TLS:
        case SIP_TRANSPORT_PROTOCOL_SCTP:
            /* ��֧�� */
            break;

        default:
            break;
    }

    return ulRet;
}

ULONG CONN_UDPListenProc(CONN_LISTEN_S *pstListener)
{
    SOCKET      sSocket;
    SOCKADDR_IN stLocal;
    ULONG       ulConnID;
    ULONG       ulRet;
    HANDLE      hThread;
    DWORD       dwThreadId;
    CONN_LOCATION_S stPeerLocation;

    /* ����socket */
    sSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (sSocket == INVALID_SOCKET)
    {
        printf("socket() failed;\n");
        return FAIL;
    }

    /* bind socket */
    stLocal.sin_family = AF_INET;
    stLocal.sin_port = htons(pstListener->stLocal.usPort);
    if (pstListener->stLocal.ulIpAddr != NULL_ULONG)
        stLocal.sin_addr.s_addr = htonl(pstListener->stLocal.ulIpAddr);
    else
		stLocal.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sSocket, (SOCKADDR *)&stLocal, sizeof(stLocal)) == SOCKET_ERROR)
    {
		printf("bind() failed: \n");
		return FAIL;
    }

    /* UDP����Զ�� */
    stPeerLocation.eProtocol = SIP_TRANSPORT_PROTOCOL_UDP;
    stPeerLocation.ulIpAddr = NULL_ULONG;
    stPeerLocation.usPort   = NULL_USHORT;
    ulRet = CONN_AllocConnection(&stPeerLocation, &ulConnID);
    if (ulRet != SUCCESS)
    {
        printf("SIP_Txp_AllocConnCB failed\r\n");
        return FAIL;
    }

    g_pstConnCB[ulConnID].ulLocalAddr  = pstListener->stLocal.ulIpAddr;
    g_pstConnCB[ulConnID].usLocalPort  = pstListener->stLocal.usPort;
    /* UDP�����ӣ���Ϊһֱ���Ǽ���� */
    g_pstConnCB[ulConnID].ucState  = CONN_STATE_ACTIVE;
    g_pstConnCB[ulConnID].sSocket  = sSocket;
    hThread = CreateThread(NULL,
                           0,
                           CONN_UDPRecvTask,
                           (LPVOID)ulConnID,
                           0,
                           &dwThreadId);

    g_pstConnCB[ulConnID].hThread  = hThread;
    return SUCCESS;
}

DWORD WINAPI CONN_TCPListenTask(LPVOID lpParam)
{
    ULONG         ulSize;
    SOCKET        sListen;
    SOCKET        sClient;
    ULONG         ulConnID;
    HANDLE        hThread;
    DWORD         dwThreadId;
    SOCKADDR_IN   stLocal;
    SOCKADDR_IN   stPeer;
    CONN_LISTEN_S  *pstListener;
    CONN_LOCATION_S stLocation;

    pstListener = (CONN_LISTEN_S *)lpParam;

    /*����socket*/
    sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sListen == SOCKET_ERROR)
    {
        printf("socket() failed: %d\n", WSAGetLastError());
        return FAIL;
    }

    pstListener->stSocket = sListen;

    /*bind socket*/
    stLocal.sin_family = AF_INET;
    stLocal.sin_port = htons(pstListener->stLocal.usPort);
    if (pstListener->stLocal.ulIpAddr != NULL_ULONG)
    {
        stLocal.sin_addr.s_addr = htonl(pstListener->stLocal.ulIpAddr);
    }
    else
    {
        stLocal.sin_addr.s_addr = htonl(INADDR_ANY);
    }

    if (bind(sListen, (struct sockaddr *)&stLocal,
            sizeof(stLocal)) == SOCKET_ERROR)
    {
        printf("bind() failed: %d\n", WSAGetLastError());
        return 1;
    }

    /*listen socket*/
    listen(sListen, 8);

    while(1)
    {
        /*accept from socket*/
        ulSize = sizeof(stPeer);
        sClient = accept(sListen, &stPeer, &ulSize);
        if (sClient == INVALID_SOCKET)
        {
            printf("accept() failed: %d\n", WSAGetLastError());
            break;
        }

        /* ��������SIP_TXP_CONNECTION_S */
        stLocation.eProtocol = SIP_TRANSPORT_PROTOCOL_TCP;
        stLocation.ulIpAddr      = ntohl(stPeer.sin_addr.s_addr);
        stLocation.usPort        = ntohs(stPeer.sin_port);
        CONN_AllocConnection(&stLocation, &ulConnID);

        /* ��¼��Ϣ */
        g_pstConnCB[ulConnID].ulLocalAddr  = pstListener->stLocal.ulIpAddr;
        g_pstConnCB[ulConnID].usLocalPort  = pstListener->stLocal.usPort;
        g_pstConnCB[ulConnID].ucState      = CONN_STATE_ACTIVE;
        g_pstConnCB[ulConnID].sSocket      = sClient;

        printf("\r\n TCP connection create success:"
               "\r\nlocal  IP:%x"
               "\r\nLocal  Port:%d"
               "\r\nRemote IP:%x"
               "\r\nRemote Port:%d",
               g_pstConnCB[ulConnID].ulLocalAddr,
               g_pstConnCB[ulConnID].usLocalPort,
               stLocation.ulIpAddr,
               stLocation.usPort);

        hThread = CreateThread(NULL,
                               0,
                               CONN_TCPRecvTask,
                               (LPVOID)ulConnID,
                               0,
                               &dwThreadId);
        g_pstConnCB[ulConnID].hThread  = hThread;
    }

    return SUCCESS;
}

DWORD WINAPI CONN_UDPRecvTask(LPVOID lpParam)
{
    ULONG ulRet;
    ULONG ulConnID;
    SOCKADDR_IN   stSender;
    ULONG ulLen;
    CONN_LOCATION_S stLocation;

    ulLen = sizeof(SOCKADDR);
    ulConnID = (ULONG)lpParam;
    stLocation.eProtocol = SIP_TRANSPORT_PROTOCOL_TCP;

    while(1)
    {
        /*recv from socket*/
        ulRet = recvfrom(g_pstConnCB[ulConnID].sSocket,
                         g_pstConnCB[ulConnID].pucRecvBuff,
                         CONN_RECV_BUFFER,
                         0,
                        (SOCKADDR *)&stSender,
                        &ulLen);
        if ((ulRet != 0) && (ulRet != SOCKET_ERROR))
        {
            /* ֪ͨ�ϲ� */
            g_pstConnCB[ulConnID].pucRecvBuff[ulRet] = '\0';
            g_pfnConnMsgProc(&stLocation, g_pstConnCB[ulConnID].pucRecvBuff, ulRet);
        }
    }

    return SUCCESS;
}

DWORD WINAPI CONN_TCPRecvTask(LPVOID lpParam)
{
    ULONG ulRet;
    ULONG ulConnID;
    ULONG ulLen;
    UBUF_HEADER_S *pstUBuf = NULL_PTR;
    CONN_LOCATION_S stLocation;

    ulConnID = (ULONG)lpParam;
    stLocation.eProtocol = SIP_TRANSPORT_PROTOCOL_TCP;
    stLocation.ulIpAddr = g_pstConnCB[ulConnID].ulRemoteAddr;
    stLocation.usPort = g_pstConnCB[ulConnID].usRemotePort;
    while(1)
    {
        /* recv from socket */
        ulRet = recv(g_pstConnCB[ulConnID].sSocket,
                     g_pstConnCB[ulConnID].pucRecvBuff,
                     CONN_RECV_BUFFER,
                     0);
        if ((ulRet != 0) && (ulRet != SOCKET_ERROR))
        {
            ulLen = ulRet;
            /*��Ϣ����*/
            if(1)
            {
                /*��Ϣ����ɹ�*/
                g_pstConnCB[ulConnID].pucRecvBuff[ulRet] = '\0';
                g_pfnConnMsgProc(&stLocation, g_pstConnCB[ulConnID].pucRecvBuff, ulRet);
            }
            else
            {
                /*����ʧ�ܻ����ǻ�������������δ����ɹ�*/
                /* ������Ϣ����ʼ��ͷ */
            }
        }
    }

    return SUCCESS;
}

DWORD WINAPI CONN_TCPConnectTask(LPVOID lpParam)
{
    int         iRet;
    int         iLen;
    SOCKET      sClient;
    SOCKADDR_IN stLocal;
    SOCKADDR_IN stPeer;
    ULONG       ulConnID;
    HANDLE      hThread;
    DWORD       dwThreadID;
    ULONG ulRemoteIP;
    USHORT usRemotePort;

    ulConnID = lpParam;

    /* ����socket */
    sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sClient == INVALID_SOCKET)
    {
        printf("socket() failed: %d\n", WSAGetLastError());
        return FAIL;
    }

    /* ����״̬ */
    g_pstConnCB[ulConnID].sSocket = sClient;

    stPeer.sin_family = AF_INET;
    stPeer.sin_port   = htons(g_pstConnCB[ulConnID].usRemotePort);
    stPeer.sin_addr.s_addr = htonl(g_pstConnCB[ulConnID].ulRemoteAddr);

    iRet = connect(sClient, (struct sockaddr *)&stPeer, sizeof(stPeer));
    if (iRet == SOCKET_ERROR)
    {
        printf("connect() failed: %d\n", WSAGetLastError());

        /* ֪ͨ���еȴ����͵���Ϣʧ�� */

        /* �ͷ���Դ */
        closesocket(sClient);
        CONN_ReleaseConnection(ulConnID);

        return FAIL;
    }

    g_pstConnCB[ulConnID].ucState  = CONN_STATE_ACTIVE;

    iLen = sizeof(stLocal);
    getsockname(sClient, &stLocal, &iLen);

    g_pstConnCB[ulConnID].ulLocalAddr = ntohl(stLocal.sin_addr.s_addr);
    g_pstConnCB[ulConnID].usLocalPort = ntohs(stLocal.sin_port);

    getpeername(sClient, &stPeer, &iLen);
    ulRemoteIP   = ntohl(stPeer.sin_addr.s_addr);
    usRemotePort = ntohs(stPeer.sin_port);
    printf("\r\n TCP connection create success:"
           "\r\nlocal  IP:%x"
           "\r\nLocal  Port:%d"
           "\r\nRemote IP:%x"
           "\r\nRemote Port:%d",
           g_pstConnCB[ulConnID].ulLocalAddr,
           g_pstConnCB[ulConnID].usLocalPort,
           ulRemoteIP,
           usRemotePort);

    /* �����д�������Ϣ���ͳ�ȥ */
    CONN_SendBuffMsg(ulConnID);

    /* �������ս��� */
    hThread = CreateThread(NULL,
                           0,
                           CONN_TCPRecvTask,
                           (LPVOID)ulConnID,
                           0,
                           &dwThreadID);

    g_pstConnCB[ulConnID].hThread  = hThread;

    return SUCCESS;
}


ULONG CONN_SendUDPConnMsg(UCHAR *pucMsg,
                          ULONG ulMsgLen,
                          ULONG ulConnID,
                          CONN_LOCATION_S *pstPeerLocation)
{
    SOCKADDR_IN stRecvAddr;
    int iRet;

    stRecvAddr.sin_family = AF_INET;
    stRecvAddr.sin_port = pstPeerLocation->usPort;
    stRecvAddr.sin_addr.s_addr = pstPeerLocation->ulIpAddr;

    iRet = sendto(g_pstConnCB[ulConnID].sSocket,
                   pucMsg,
                   ulMsgLen,
                   0,
                   (SOCKADDR *)&stRecvAddr,
                   sizeof(stRecvAddr));
    if ((iRet == SOCKET_ERROR)||((ULONG)iRet < ulMsgLen))
    {
        printf("sendto err,the result is %d",iRet);
        return FAIL;
    }

    return SUCCESS;
}

ULONG CONN_SendTCPConnMsg(UCHAR *pucMsg, ULONG ulMsgLen, ULONG ulConnID)
{
    int iRet;

    if (g_pstConnCB[ulConnID].ucState != CONN_STATE_ACTIVE)
    {
        /* �������״̬δ����⽫��Ϣ���뷢�ͻ��� */
        CONN_BuffSendMsg(pucMsg, ulMsgLen, ulConnID);
        return SUCCESS;
    }

    /* ����ֱ�ӷ��� */
    iRet = send(g_pstConnCB[ulConnID].sSocket,
                pucMsg,
                ulMsgLen,
                0);
    if ((iRet == 0)||(iRet == SOCKET_ERROR))
    {
        printf("send error,the result is %d",iRet);
        return FAIL;
    }

    return SUCCESS;
}

/* ���ͻ����������Ϣ */
ULONG CONN_SendBuffMsg(ULONG ulConnID)
{
    CONN_SEND_BUFF_HEADER_S *pstSendBuff = NULL_PTR;
    CONN_SEND_MSG_HEADER_S *pstMsgHeader = NULL_PTR;
    UCHAR *pucMsgAddr = NULL_PTR;
    ULONG  ulSendLen;
    ULONG  ulMsgLen;

    /* ֻ�м���̬�����Ӳ������ͻ�����Ϣ */
    if (g_pstConnCB[ulConnID].ucState != CONN_STATE_ACTIVE)
    {
        return FAIL;
    }

    pstSendBuff = (CONN_SEND_BUFF_HEADER_S *)g_pstConnCB[ulConnID].pucSendBuff;
    ulSendLen = 0;

    while (ulSendLen < pstSendBuff->ulCurrentLen)
    {
        pstMsgHeader = (UCHAR *)pstSendBuff
                      + sizeof(CONN_SEND_BUFF_HEADER_S)
                      + ulSendLen;
        pucMsgAddr = (UCHAR *)pstMsgHeader + sizeof(CONN_SEND_MSG_HEADER_S);
        ulMsgLen = pstMsgHeader->ulLen;
        CONN_SendInConnection(pucMsgAddr, ulMsgLen, ulConnID, NULL_PTR);
        ulSendLen += sizeof(CONN_SEND_MSG_HEADER_S) + ulMsgLen;
    }

    pstSendBuff->ulCurrentLen = 0;
    return SUCCESS;
}

/* ���ͻ����������Ϣ */
ULONG CONN_BuffSendMsg(UCHAR *pucMsg,ULONG ulMsgLen, ULONG ulConnID)
{
    CONN_SEND_BUFF_HEADER_S *pstSendBuff = NULL_PTR;
    CONN_SEND_MSG_HEADER_S *pstMsgHeader = NULL_PTR;
    UCHAR *pucMsgAddr = NULL_PTR;

    pstSendBuff = (CONN_SEND_BUFF_HEADER_S *)g_pstConnCB[ulConnID].pucSendBuff;
    if ((pstSendBuff->ulCurrentLen + ulMsgLen + sizeof(CONN_SEND_MSG_HEADER_S))
       > pstSendBuff->ulBufferLen)
    {
        return FAIL;
    }

    pstMsgHeader = (UCHAR *)pstSendBuff
                   + pstSendBuff->ulCurrentLen
                   + sizeof(CONN_SEND_BUFF_HEADER_S);
    pstMsgHeader->ulLen = ulMsgLen;
    pucMsgAddr = (UCHAR *)pstMsgHeader + sizeof(CONN_SEND_MSG_HEADER_S);
    memcpy(pucMsgAddr, pucMsg, ulMsgLen);

    pstSendBuff->ulCurrentLen += ulMsgLen + sizeof(CONN_SEND_MSG_HEADER_S);
    return SUCCESS;
}

/* ���ӵ���Ϣ������ں��� */
ULONG CONN_SendMsg(UCHAR *pucTextMsg,
                   ULONG ulMsgLen,
                   SIP_LOCATION_S *pstLocation)
{
    ULONG ulSendConnID;
    ULONG ulRet;
    CONN_LOCATION_S stLocation;

    stLocation.eProtocol = pstLocation->eProtocol;
    stLocation.ulIpAddr  = inet_addr(pstLocation->aucIPStr);
    stLocation.usPort    = htons(pstLocation->usPort);

    /* ��λ������Ϣ�����ӣ�����pstLocationѰ������ */
    ulRet = CONN_FindConnection(&stLocation, &ulSendConnID);
    if (ulRet != SUCCESS)
    {
        CONN_AllocConnection(&stLocation, &ulSendConnID);
    }

    /* ·��MTU����Ϣ���ȵĲ�С��200����ʹ�÷����صĴ���Э�鴫�ͣ�������� */

    /* �ڸ������Ϸ�����Ϣ�Ļ����ͼ���һ�¸����� */
    CONN_ActiveConnection(ulSendConnID);

    /* �������Ϸ�����Ϣ */
    CONN_SendInConnection(pucTextMsg,
                          ulMsgLen,
                          ulSendConnID,
                         &stLocation);
    return SUCCESS;
}
