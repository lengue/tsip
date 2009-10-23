/*******************************************************************************
功能    : 用于定位发送请求和相应的目标地址(rfc3263)
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/
/* 外部依赖模块头文件 */
#include "common\common.h"
#include "abnf\abnf.h"
#include "ubuf\ubuf.h"
#include "sip\uri.h"
#include "sip\sip.h"


/* 本模块对外提供的常量和结构头文件 */
#include "..\include\locate\sip_locate.h"

/* 本模块内部使用的常量和结构头文件 */

/* 本模块内部函数声明头文件 */


/* 本模块全局变量声明头文件 */


/* 用于定位请求的目的位置 */
ULONG SIP_Locate_Server(UBUF_HEADER_S *pstUbuf,
                        UBUF_PTR       upUri,
                        SIP_LOCATION_RESULT_S   *pstResult,
                        SIP_LOCATION_RESULT_PROC pfnProc,
                        ULONG ulPara)
{
//    ULONG ulRet;
    URI_S *pstUri = NULL_PTR;
    SIP_TRANSPORT_PROTOCOL_E eProtocol = SIP_TRANSPORT_PROTOCOL_BUTT;
    UCHAR *pucAddr = NULL_PTR;
    USHORT usPort;
    UBUF_PTR upPara;
    URI_PARAMETER_S *pstUriPara = NULL_PTR;

    pstUri = UBUF_UBufPtr2Ptr(pstUbuf, upUri);
    if ((pstUri->eUriType != URI_TYPE_SIP)||(pstUri->eUriType != URI_TYPE_SIP))
    {
        return FAIL;
    }

    /* 选择传输协议 */
    /* 查找protocol参数 */
    upPara = pstUri->u.stSipUri.upstParameters;
    while (upPara != UBUF_NULL_PTR)
    {
        pstUriPara = UBUF_UBufPtr2Ptr(pstUbuf, upPara);
        if (pstUriPara->eParaName == URI_PARAMETER_TRANSPORT)
        {
            eProtocol = pstUriPara->ucParaValue;
            break;
        }
        upPara = pstUriPara->upstNext;
    }

    if (eProtocol == SIP_TRANSPORT_PROTOCOL_BUTT)
    {
        if ((pstUri->u.stSipUri.stHostPort.stHost.eHostType != URI_HOST_DOMAIN)
               ||(pstUri->u.stSipUri.stHostPort.usPort != NULL_USHORT))
        {
            if (pstUri->eUriType == URI_TYPE_SIP)
            {
                eProtocol = SIP_TRANSPORT_PROTOCOL_UDP;
            }
            else
            {
                eProtocol = SIP_TRANSPORT_PROTOCOL_TCP;
            }
        }
        else
        {
            #if 0
            ulRet = Dns_Lookup(DNS_TYPE_NAPTR);
            if(ulRet == SUCCESS)
            {
                /*根据优先级采用传输协议*/
            }
            else
            {
                ulRet = Dns_Lookup(DNS_TYPE_SRV);
                if(ulRet == SUCCESS)
                {
                    /*根据优先级采用传输协议*/
                }
                else if (pstSipUri->ucSecure == FALSE)
                {
                    pstTransAddr->ucProtocolType = SIP_TRANSPORT_PROTOCOL_UDP;
                }
                else
                {
                    pstTransAddr->ucProtocolType = SIP_TRANSPORT_PROTOCOL_TCP;
                }
            }
            #endif
        }
    }


    /* 选择IP地址和端口号 */
    if (pstUri->u.stSipUri.stHostPort.stHost.eHostType != URI_HOST_DOMAIN)
    {
        pucAddr = UBUF_UBufPtr2Ptr(pstUbuf, pstUri->u.stSipUri.stHostPort.stHost.upucAddrStr);
        if(pstUri->u.stSipUri.stHostPort.usPort != NULL_USHORT)
        {
            usPort = pstUri->u.stSipUri.stHostPort.usPort;
        }
        else
        {
            /* 使用协议缺省的端口 */
            if (pstUri->eUriType == URI_TYPE_SIP)
            {
                usPort = 5060;
            }
            else
            {
                usPort = 5061;
            }
        }
    }
    else if (pstUri->u.stSipUri.stHostPort.usPort != NULL_USHORT)
    {
        #if 0
        if()
        {
            Dns_Lookup(DNS_TYPE_A);
        }
        else
        {
            Dns_Lookup(DNS_TYPE_AAAA);
        }

        pstTransAddr->usPort = pstSipUri->stHostPort.usPort;
        #endif
    }
    else
    {
        #if 0
        /*开始没查则这里查NAPTR*/
        if (pstSipUri->ppstParameters[URI_PARAMETER_TRANSPORT]== NULL_PTR)
        {
            Dns_Lookup(DNS_TYPE_NAPTR);
        }

        /* 接着查SRV */
        Dns_Lookup(DNS_TYPE_SRV);

        /*再接着查A/AAAA*/
        if()
        {
            Dns_Lookup(DNS_TYPE_A);
        }
        else
        {
            Dns_Lookup(DNS_TYPE_AAAA);
        }
        #endif
    }

    pstResult->ulNumber = 1;
    pstResult->astLocations[0].eProtocol = eProtocol;
    strcpy(pstResult->astLocations[0].aucIPStr, pucAddr);
    pstResult->astLocations[0].usPort    = usPort;
    return SUCCESS;
}

/* 用于定位响应的目的位置 */
ULONG SIP_Locate_Client(UBUF_HEADER_S *pstUbuf,
                                UBUF_PTR       upSendBy,
                                SIP_LOCATION_RESULT_S   *pstResult,
                                SIP_LOCATION_RESULT_PROC pfnProc,
                                ULONG ulPara)
{
    #if 0
    if(/*send-by字段是一个可靠传输协议*/)
    {
        /* 使用原来的连接发送 */
    }
    else if (/*存在maddr参数*/)
    {
        /* 使用maddr参数描述的地址发送 */
    }
    else if(/* 不可靠的单播传输协议 */ && /*有received参数*/)
    {

    }

    /* 使用rfc3263的方式查找 */
    /* 发送该消息 */
    #endif
    return SUCCESS;
}

/* 用于定位响应的备份目的位置 */
ULONG SIP_Locate_RedundancyClient(UBUF_HEADER_S *pstUbuf,
                                  UBUF_PTR       upSendBy,
                                  SIP_LOCATION_RESULT_S   *pstResult,
                                  SIP_LOCATION_RESULT_PROC pfnProc,
                                  ULONG ulPara)
{
    #if 0
    pstTransAddr->ucProtocolType = pstViaHeader->ucProtocolType;
    if(pstViaHeader->stSendBy.eHostType != HOST_DOMAIN)
    {
        pstTransAddr->ulIpAddr = pstViaHeader->stSendBy.uHost.ucIPV4;
        if(pstViaHeader->stSendBy.usPort != NULL_USHORT)
        {
            pstTransAddr->usPort = pstViaHeader->stSendBy.usPort;
        }
        else
        {
            /* 使用协议缺省的端口 */
        }
    }
    else if(pstViaHeader->stSendBy.usPort != NULL_USHORT)
    {
        if()
        {
            Dns_Lookup(DNS_TYPE_A);
        }
        else
        {
            Dns_Lookup(DNS_TYPE_AAAA);
        }
    }
    else
    {
        /* 根据传输协议构造SRV RR */
        Dns_Lookup(DNS_TYPE_SRV);
    }
    #endif

    return SUCCESS;
}

/* 发现下一跳，同时会整理RequestURI和路由集 */
ULONG SIP_Locate_FindNextHop(UBUF_HEADER_S *pstUbufSipMsg, UBUF_PTR *pupUri)
{
    SIP_MSG_S          *pstSipMsg = NULL_PTR;
    SIP_HEADER_S       *pstHeader = NULL_PTR;
    SIP_HEADER_ROUTE_S *pstRoute  = NULL_PTR;
    SIP_ROUTE_PARAM_S  *pstRouteParam = NULL_PTR;
    UBUF_PTR            upRequestUri;
    UBUF_PTR           *pupTemp;

    pstSipMsg = UBUF_UBufPtr2Ptr(pstUbufSipMsg, 0);
    if (pstSipMsg->aupstHeaders[SIP_HEADER_ROUTE] == UBUF_NULL_PTR)
    {
        /* 如果路由集为空，下一跳URI为RequestURI */
        *pupUri = pstSipMsg->uStartLine.stRequstLine.upRequestURI;
        return SUCCESS;
    }

    /* 如果路由集为空，下一跳URI为路由集的第一个路由地址 */
    pstHeader = UBUF_UBufPtr2Ptr(pstUbufSipMsg, pstSipMsg->aupstHeaders[SIP_HEADER_ROUTE]);
    pstRoute = (SIP_HEADER_ROUTE_S *)pstHeader->pstSpec;
    pstRouteParam = UBUF_UBufPtr2Ptr(pstUbufSipMsg, pstRoute->upstRouteParam);

    *pupUri = pstRouteParam->stNameAddr.upstUri;

    /* 如果路由集最顶端的URI为严格路由器，则将最顶端URI迁移到RequestURI中。原来
    的RequestURI放在Route最后 */
    if (pstRouteParam->ucLr == FALSE)
    {
        upRequestUri = pstSipMsg->uStartLine.stRequstLine.upRequestURI;
        pstSipMsg->uStartLine.stRequstLine.upRequestURI
                                         = pstRouteParam->stNameAddr.upstUri;
        pstRoute->upstRouteParam = pstRouteParam->upstNext;
        pupTemp = &pstRoute->upstRouteParam;
        while (*pupTemp != UBUF_NULL_PTR)
        {
            pstRouteParam = UBUF_UBufPtr2Ptr(pstUbufSipMsg, *pupTemp);
            pupTemp = &pstRouteParam->upstNext;
        }

        pstRouteParam = UBUF_AddComponent(pstUbufSipMsg,
                                          sizeof(SIP_ROUTE_PARAM_S),
                                          pupTemp);
        memset(pstRouteParam, 0xff, sizeof(SIP_ROUTE_PARAM_S));
        pstRouteParam->stNameAddr.bName   = FALSE;
        pstRouteParam->stNameAddr.upstUri = upRequestUri;
        pstRouteParam->ucLr = FALSE;
    }

    return SUCCESS;
}
