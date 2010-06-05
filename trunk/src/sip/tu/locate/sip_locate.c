/*******************************************************************************
����    : ���ڶ�λ�����������Ӧ��Ŀ���ַ(rfc3263)
������  : �ƴ�ƽ
��������: 200.01.10
�޸ļ�¼:
*******************************************************************************/
/* �ⲿ����ģ��ͷ�ļ� */
#include "common\common.h"
#include "abnf\abnf.h"
#include "ubuf\ubuf.h"
#include "uri\uri.h"
#include "sip\sip.h"


/* ��ģ������ṩ�ĳ����ͽṹͷ�ļ� */
#include "..\include\locate\sip_locate.h"

/* ��ģ���ڲ�ʹ�õĳ����ͽṹͷ�ļ� */

/* ��ģ���ڲ���������ͷ�ļ� */


/* ��ģ��ȫ�ֱ�������ͷ�ļ� */


/* ���ڶ�λ�����Ŀ��λ�� */
ULONG SIP_Locate_Server(URI_S *pstUri,
                        SIP_LOCATION_RESULT_S   *pstResult,
                        SIP_LOCATION_RESULT_PROC pfnProc,
                        ULONG ulPara)
{
//    ULONG ulRet;
    SIP_TRANSPORT_PROTOCOL_E eProtocol = SIP_TRANSPORT_PROTOCOL_BUTT;
    UCHAR *pucAddr = NULL_PTR;
    USHORT usPort;
    URI_PARAMETER_S *pstUriPara = NULL_PTR;

    if ((pstUri->eUriType != URI_TYPE_SIP)||(pstUri->eUriType != URI_TYPE_SIP))
    {
        return FAIL;
    }

    /* ѡ����Э�� */
    /* ����protocol���� */
    pstUriPara = pstUri->u.stSipUri.pstParameters;
    while (pstUriPara != NULL_PTR)
    {
        if (pstUriPara->eParaName == URI_PARAMETER_TRANSPORT)
        {
            eProtocol = pstUriPara->ucParaValue;
            break;
        }

        pstUriPara = pstUriPara->pstNext;
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
                /*�������ȼ����ô���Э��*/
            }
            else
            {
                ulRet = Dns_Lookup(DNS_TYPE_SRV);
                if(ulRet == SUCCESS)
                {
                    /*�������ȼ����ô���Э��*/
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


    /* ѡ��IP��ַ�Ͷ˿ں� */
    if (pstUri->u.stSipUri.stHostPort.stHost.eHostType != URI_HOST_DOMAIN)
    {
        pucAddr = pstUri->u.stSipUri.stHostPort.stHost.pucAddrStr;
        if(pstUri->u.stSipUri.stHostPort.usPort != NULL_USHORT)
        {
            usPort = pstUri->u.stSipUri.stHostPort.usPort;
        }
        else
        {
            /* ʹ��Э��ȱʡ�Ķ˿� */
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
        /*��ʼû���������NAPTR*/
        if (pstSipUri->ppstParameters[URI_PARAMETER_TRANSPORT]== NULL_PTR)
        {
            Dns_Lookup(DNS_TYPE_NAPTR);
        }

        /* ���Ų�SRV */
        Dns_Lookup(DNS_TYPE_SRV);

        /*�ٽ��Ų�A/AAAA*/
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

/* ���ڶ�λ��Ӧ��Ŀ��λ�� */
ULONG SIP_Locate_Client(URI_HOST_PORT_S  *pstSendBy,
                        SIP_LOCATION_RESULT_S   *pstResult,
                        SIP_LOCATION_RESULT_PROC pfnProc,
                        ULONG ulPara)
{
    #if 0
    if(/*send-by�ֶ���һ���ɿ�����Э��*/)
    {
        /* ʹ��ԭ�������ӷ��� */
    }
    else if (/*����maddr����*/)
    {
        /* ʹ��maddr���������ĵ�ַ���� */
    }
    else if(/* ���ɿ��ĵ�������Э�� */ && /*��received����*/)
    {

    }

    /* ʹ��rfc3263�ķ�ʽ���� */
    /* ���͸���Ϣ */
    #endif
    return SUCCESS;
}

/* ���ڶ�λ��Ӧ�ı���Ŀ��λ�� */
ULONG SIP_Locate_RedundancyClient(URI_HOST_PORT_S  *pstSendBy,
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
            /* ʹ��Э��ȱʡ�Ķ˿� */
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
        /* ���ݴ���Э�鹹��SRV RR */
        Dns_Lookup(DNS_TYPE_SRV);
    }
    #endif

    return SUCCESS;
}

/* ������һ����ͬʱ������RequestURI��·�ɼ� */
ULONG SIP_Locate_FindNextHop(UBUF_HEADER_S *pstUbufSipMsg, URI_S **ppstUri)
{
    SIP_MSG_S          *pstSipMsg = NULL_PTR;
    SIP_HEADER_ROUTE_S *pstRoute  = NULL_PTR;
    SIP_ROUTE_PARAM_S  *pstRouteParam = NULL_PTR;
    URI_S              *pstRequestURI = NULL_PTR;
    SIP_ROUTE_PARAM_S **ppstTemp = NULL_PTR;

    pstSipMsg = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUbufSipMsg);
    if (pstSipMsg->apstHeaders[SIP_HEADER_ROUTE] == NULL_PTR)
    {
        /* ���·�ɼ�Ϊ�գ���һ��URIΪRequestURI */
        *ppstUri = pstSipMsg->uStartLine.stRequstLine.pstRequestURI;
        return SUCCESS;
    }

    /* ���·�ɼ�Ϊ�գ���һ��URIΪ·�ɼ��ĵ�һ��·�ɵ�ַ */
    pstRoute      = (SIP_HEADER_ROUTE_S *)pstSipMsg->apstHeaders[SIP_HEADER_ROUTE];
    pstRouteParam = pstRoute->pstRouteParam;

    *ppstUri = pstRouteParam->stNameAddr.pstUri;

    /* ���·�ɼ���˵�URIΪ�ϸ�·�����������URIǨ�Ƶ�RequestURI�С�ԭ��
    ��RequestURI����Route��� */
    if (pstRouteParam->ucLr == FALSE)
    {
        pstRequestURI = pstSipMsg->uStartLine.stRequstLine.pstRequestURI;
        pstSipMsg->uStartLine.stRequstLine.pstRequestURI
                                         = pstRouteParam->stNameAddr.pstUri;
        pstRoute->pstRouteParam = pstRouteParam->pstNext;
        ppstTemp = &pstRoute->pstRouteParam;
        while (*ppstTemp != NULL_PTR)
        {
            ppstTemp = &(*ppstTemp)->pstNext;
        }

        *ppstTemp = UBUF_AddComponent(pstUbufSipMsg, sizeof(SIP_ROUTE_PARAM_S));
        memset(pstRouteParam, 0x0, sizeof(SIP_ROUTE_PARAM_S));
        pstRouteParam->stNameAddr.bName   = FALSE;
        pstRouteParam->stNameAddr.pstUri = pstRequestURI;
        pstRouteParam->ucLr = FALSE;
    }

    return SUCCESS;
}
