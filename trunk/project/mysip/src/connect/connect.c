/*******************************************************************************
功能    : SIP协议栈传输层功能函数
创建人  : 唐春平
创建日期: 2009.01.10
修改记录:
*******************************************************************************/
/* 外部依赖模块头文件 */
#include "common\common.h"
#include "ubuf\ubuf.h"
#include "sip\uri.h"
#include "sip\sip.h"

/* 本模块对外提供的常量和结构头文件 */
#include "connect\connect.h"

/* 本模块内部使用的常量和结构头文件 */
#include "connect_const.inc"
#include "connect_type.inc"

/* 本模块内部函数声明头文件 */
#include "connect.inc"

/* 本模块全局变量声明头文件 */
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

    /* 连接控制块内存申请及初始化 */
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

    /* 空闲队列初始化 */
    g_stConnCBQueue.ulNum = CONN_MAX_CONN;
    g_stConnCBQueue.ulHead = 0;
    g_stConnCBQueue.ulTail = CONN_MAX_CONN - 1;

    memset(g_aulConnTransport, 0xff, sizeof(g_aulConnTransport));

    g_pfnConnMsgProc = g_pstConnCfg->pfnMsgProc;

    /* 各监听任务启动 */
    if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
    {
        printf("WSAStartup failed!\r\n");
        return FAIL;
    }

    for (ulLoop = 0; ulLoop < g_pstConnCfg->ucListenNum; ulLoop++)
    {
        pstCfg = &g_pstConnCfg->stListen[ulLoop];

        /*根据协议初始化监听协议*/
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
                /* TCP需要一直监听，所以要启动单独的任务 */
                pstListener = CONN_AddListener(pstCfg);
                hThread = CreateThread(NULL,
                                       0,
                                       CONN_TCPListenTask,
                                       (PVOID)pstListener,
                                       0,
                                       &dwThreadID);
                CloseHandle(hThread);
                break;

            /* 其他的传输协议暂时不支持 */
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

/* 申请连接，包括连接控制块的申请和挂载 */
ULONG CONN_AllocConnection(CONN_LOCATION_S *pstPeerLocation,
                           ULONG *pulConnID)
{
    ULONG ulConnID;
    ULONG ulTempConnID;
    CONN_SEND_BUFF_HEADER_S *pstTemp = NULL_PTR;
    CONN_CB_S *pstConnCB = NULL_PTR;
    CONN_CB_S *pstTempConnCB = NULL_PTR;

    /* 如果没有连接控制块就失败了 */
    if(g_stConnCBQueue.ulNum == 0)
    {
        return FAIL;
    }

    /* 申请控制块 */
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

    /* 连接初始化 */
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

    /* 发送缓存私是有结构的 */
    pstConnCB->pucSendBuff  = malloc(CONN_SEND_BUFFER + sizeof(CONN_SEND_BUFF_HEADER_S));
    pstTemp = (CONN_SEND_BUFF_HEADER_S *)pstConnCB->pucSendBuff;
    pstTemp->ulBufferLen  = CONN_SEND_BUFFER;
    pstTemp->ulCurrentLen = 0;

    /* 连接挂载，加在队列前面，修改双向链表 */
    pstConnCB->ulPreConn    = NULL_ULONG;
    pstConnCB->ulNextConn   = g_aulConnTransport[pstPeerLocation->eProtocol];
    ulTempConnID = pstConnCB->ulNextConn;
    if (ulTempConnID != NULL_ULONG)
    {
        pstTempConnCB = &g_pstConnCB[ulTempConnID];
        pstTempConnCB->ulPreConn = ulConnID;
    }

    g_aulConnTransport[pstPeerLocation->eProtocol] = ulConnID;

    /* 连接激活的工作根据接受连接和发起连接不同，在具体的操作里面执行 */
    *pulConnID = ulConnID;
    return SUCCESS;
}

/* 连接的激活，没有建连的需要建连，同事保活定时器需要重启一下 */
ULONG CONN_ActiveConnection(ULONG ulConnID)
{
    DWORD          dwThreadID;
    HANDLE         hThread;

    switch (g_pstConnCB[ulConnID].eProtocol)
    {
        /* UDP不需要激活 */
        case SIP_TRANSPORT_PROTOCOL_UDP:
            break;


        case SIP_TRANSPORT_PROTOCOL_TCP:
            /* 非激活态需要建立连接激活 */
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

            /* 重启一下连接保活定时器 */
            break;

        /* 暂不支持 */
        case SIP_TRANSPORT_PROTOCOL_SCTP:
        case SIP_TRANSPORT_PROTOCOL_TLS:
            break;

        default:
            return FAIL;
    }

    return SUCCESS;
}

/* 根据远端地址查找连接 */
ULONG CONN_FindConnection(CONN_LOCATION_S *pstPeerLocation,
                          ULONG *pulConnID)
{
    ULONG ulConnID;
    CONN_CB_S *pstConnCB = NULL_PTR;

    /* 根据协议类型找到连接控制块队列起始索引 */
    ulConnID = g_aulConnTransport[pstPeerLocation->eProtocol];

    /* 在队列中查找 */
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

    /* 卸载连接 */
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

    /* 释放资源 */
    free(pstConnCB->pucRecvBuff);
    free(pstConnCB->pucRecvBuff);

    /* 释放socket */


    /* 释放连接控制块 */
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

/*连接的激活，主要是本端发起建联请求的情况*/
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
            /* 不支持 */
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

    /* 创建socket */
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

    /* UDP不绑定远端 */
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
    /* UDP无连接，认为一直都是激活的 */
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

    /*创建socket*/
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

        /* 申请连接SIP_TXP_CONNECTION_S */
        stLocation.eProtocol = SIP_TRANSPORT_PROTOCOL_TCP;
        stLocation.ulIpAddr      = ntohl(stPeer.sin_addr.s_addr);
        stLocation.usPort        = ntohs(stPeer.sin_port);
        CONN_AllocConnection(&stLocation, &ulConnID);

        /* 记录信息 */
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
            /* 通知上层 */
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
            /*消息定界*/
            if(1)
            {
                /*消息定界成功*/
                g_pstConnCB[ulConnID].pucRecvBuff[ulRet] = '\0';
                g_pfnConnMsgProc(&stLocation, g_pstConnCB[ulConnID].pucRecvBuff, ulRet);
            }
            else
            {
                /*定界失败或者是缓存区已满但还未定界成功*/
                /* 丢弃消息，开始定头 */
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

    /* 申请socket */
    sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sClient == INVALID_SOCKET)
    {
        printf("socket() failed: %d\n", WSAGetLastError());
        return FAIL;
    }

    /* 设置状态 */
    g_pstConnCB[ulConnID].sSocket = sClient;

    stPeer.sin_family = AF_INET;
    stPeer.sin_port   = htons(g_pstConnCB[ulConnID].usRemotePort);
    stPeer.sin_addr.s_addr = htonl(g_pstConnCB[ulConnID].ulRemoteAddr);

    iRet = connect(sClient, (struct sockaddr *)&stPeer, sizeof(stPeer));
    if (iRet == SOCKET_ERROR)
    {
        printf("connect() failed: %d\n", WSAGetLastError());

        /* 通知所有等待发送的消息失败 */

        /* 释放资源 */
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

    /* 将所有待发送消息发送出去 */
    CONN_SendBuffMsg(ulConnID);

    /* 创建接收进程 */
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
        /* 如果连接状态未激活，这将消息放入发送缓存 */
        CONN_BuffSendMsg(pucMsg, ulMsgLen, ulConnID);
        return SUCCESS;
    }

    /* 否则直接发送 */
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

/* 发送缓存的所有消息 */
ULONG CONN_SendBuffMsg(ULONG ulConnID)
{
    CONN_SEND_BUFF_HEADER_S *pstSendBuff = NULL_PTR;
    CONN_SEND_MSG_HEADER_S *pstMsgHeader = NULL_PTR;
    UCHAR *pucMsgAddr = NULL_PTR;
    ULONG  ulSendLen;
    ULONG  ulMsgLen;

    /* 只有激活态的连接才允许发送缓存消息 */
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

/* 发送缓存的所有消息 */
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

/* 连接的消息发送入口函数 */
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

    /* 定位发送消息的连接，根据pstLocation寻找连接 */
    ulRet = CONN_FindConnection(&stLocation, &ulSendConnID);
    if (ulRet != SUCCESS)
    {
        CONN_AllocConnection(&stLocation, &ulSendConnID);
    }

    /* 路径MTU和消息长度的差小于200不能使用非流控的传输协议传送，这里添加 */

    /* 在该连接上发送消息的话，就激活一下该连接 */
    CONN_ActiveConnection(ulSendConnID);

    /* 在连接上发送消息 */
    CONN_SendInConnection(pucTextMsg,
                          ulMsgLen,
                          ulSendConnID,
                         &stLocation);
    return SUCCESS;
}
