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
#include "system\system.h"
#include "connect\connect.h"
#include "timer\timer.h"
#include "app\app.h"

/* 本模块对外提供的常量和结构头文件 */

/* 本模块内部使用的常量和结构头文件 */
#include "sip_adapt_const.inc"
#include "sip_adapt_type.inc"

/* 本模块内部函数声明头文件 */
#include "sip_adapt.inc"

/* 本模块全局变量声明头文件 */
#include "sip_adapt_var.inc"

ULONG SIP_ADPT_Init()
{
    ULONG ulRet;
    SIP_ADAPT_CFG_S stCfg;

    g_pucSipAdptBuffer = malloc(SIP_MAX_TEXT_MSG_LEN);

    /* 构建SIP协议栈配置数据 */
    ulRet = SIP_ADPT_BuildConfigStruct(&stCfg);
    if (ulRet == SUCCESS)
    {
        /* 配置SIP协议栈 */
        ulRet = SIP_Config(&stCfg.stSipCfg);

        /* 配置连接 */
        ulRet |= CONN_Config(&stCfg.stConnCfg);
    }

    /* 释放配置数据 */
    SIP_ADPT_FreeConfigStruct(&stCfg);

    return ulRet;
}

ULONG SIP_APDT_MsgProc(ULONG ulModuleID, void* pMsg)
{
    SIP_ADPT_CONN_MSG_S  *pstConnMsg  = NULL_PTR;
    TIMER_MSG_S *pstTimerMsg = NULL_PTR;
    APP_MSG_S   *pstAppMsg   = NULL_PTR;

    switch(ulModuleID)
    {
        case SYS_MODULE_CONN:
            pstConnMsg =(SIP_ADPT_CONN_MSG_S *)pMsg;
            SIP_RecvUpMsg(&pstConnMsg->stLocation, pstConnMsg->pstMsgUbuf);
            break;

        case SYS_MODULE_TIMER:
            pstTimerMsg = (TIMER_MSG_S *)pMsg;
            SIP_TimeoutProc(pstTimerMsg->ulName, pstTimerMsg->ulPara);
            break;

        case SYS_MODULE_APP:
            pstAppMsg = (APP_MSG_S *)pMsg;
            SIP_RecvDownMsg(pstAppMsg->ulAppRef1,
                            pstAppMsg->ulAppRef2,
                            pstAppMsg->ulStackRef1,
                            pstAppMsg->ulStackRef2,
                            pstAppMsg->pstUbufSipMsg);
            break;

        default:
            break;
    }

    return SUCCESS;
}

ULONG SIP_ADPT_BuildConfigStruct(SIP_ADAPT_CFG_S *g_pstCfg)
{
    xmlDocPtr doc;
    xmlNodePtr pstNodeRoot  = NULL_PTR;
    xmlNodePtr pstNode      = NULL_PTR;
    xmlChar   *szKey        = NULL_PTR;
    ULONG      ulRet        = NULL_PTR;
    ULONG      ulLoop;
    SIP_ADAPT_CFG_LAYER_E eCfgLayer;
    UCHAR      aucCfgFlag[SIP_ADAPT_CFG_LAYER_BUTT];

    doc = xmlReadFile("config.xml","UTF-8",XML_PARSE_RECOVER|XML_PARSE_NOBLANKS);
    if (NULL == doc)
    {
        printf("Document not parsed successfully. \n");
        return FAIL;
    }

    pstNodeRoot = xmlDocGetRootElement(doc);
    if (NULL_PTR == pstNodeRoot)
    {
        return FAIL;
    }

    /* 文档根元素为config节点 */
    if (xmlStrcmp(pstNodeRoot->name, BAD_CAST "config"))
    {
        printf("\r\nroot node err at line%d",pstNode->line);
        return FAIL;
    }

    for (ulLoop = 0; ulLoop < SIP_ADAPT_CFG_LAYER_BUTT; ulLoop++)
    {
        aucCfgFlag[ulLoop] = FALSE;
    }

    /* 查找transport节点 */
    pstNode = pstNodeRoot->children;
    while (pstNode != NULL_PTR)
    {
        eCfgLayer = SIP_ADAPT_CFG_LAYER_BUTT;
        if (!xmlStrcmp(pstNode->name, BAD_CAST "tu"))
        {
            eCfgLayer = SIP_ADAPT_CFG_LAYER_TU;
        }
        else if (!xmlStrcmp(pstNode->name, BAD_CAST "transaction"))
        {
            eCfgLayer = SIP_ADAPT_CFG_LAYER_TXN;
        }
        else if (!xmlStrcmp(pstNode->name, BAD_CAST "transport"))
        {
            eCfgLayer = SIP_ADAPT_CFG_LAYER_TXP;
        }
        else if (!xmlStrcmp(pstNode->name, BAD_CAST "syntax"))
        {
            eCfgLayer = SIP_ADAPT_CFG_LAYER_SYNTAX;
        }
        else if (!xmlStrcmp(pstNode->name, BAD_CAST "conn"))
        {
            eCfgLayer = SIP_ADAPT_CFG_LAYER_CONN;
        }

        if (eCfgLayer < SIP_ADAPT_CFG_LAYER_BUTT)
        {
            if (aucCfgFlag[eCfgLayer] == TRUE)
            {
                return FAIL;
            }
            else
            {
                aucCfgFlag[eCfgLayer] = TRUE;
                ulRet = g_apfnSipAdaptCfgParse[eCfgLayer](g_pstCfg, doc, pstNode);
                if(ulRet != SUCCESS)
                {
                    return ulRet;
                }
            }
        }

        pstNode = pstNode->next;
    }

    /* 有一样未配置都是错误的 */
    for (ulLoop = 0; ulLoop < SIP_ADAPT_CFG_LAYER_BUTT; ulLoop++)
    {
        if(aucCfgFlag[ulLoop] == FALSE)
        {
            return FAIL;
        }
    }

    /* 挂函数钩子 */
    g_pstCfg->stSipCfg.stShellCfg.pfnStartTimer     = SIP_ADPT_StartTimer;
    g_pstCfg->stSipCfg.stShellCfg.pfnStopTimer      = SIP_ADPT_StopTimer;
    g_pstCfg->stSipCfg.stShellCfg.pfnSendUpMsg      = SIP_ADPT_SendUpMsg;
    g_pstCfg->stSipCfg.stShellCfg.pfnSendDownMsg    = SIP_ADPT_SendDownMsg;
    g_pstCfg->stSipCfg.stShellCfg.pfnGenerateRandom = SYS_GeneralRandomString;
    return SUCCESS;
}


/* 释放配置结构 */
ULONG SIP_ADPT_FreeConfigStruct(SIP_ADAPT_CFG_S *g_pstCfg)
{
    ULONG ulLoop;

    for(ulLoop = 0; ulLoop < SIP_ADAPT_CFG_LAYER_BUTT; ulLoop++)
    {
        g_apfnSipAdaptCfgFree[ulLoop](g_pstCfg);
    }

    return SUCCESS;
}


ULONG SIP_ADPT_BuildTuConfig(SIP_ADAPT_CFG_S *g_pstCfg,
                             xmlDocPtr doc,
                             xmlNodePtr pstNode)
{
    xmlNodePtr pstTempNode;
    xmlChar   *szKey;

    /* 解析CORE TYPE */
    pstTempNode = pstNode->children;
    while(pstTempNode != NULL_PTR)
    {
        if (!xmlStrcmp(pstTempNode->name, BAD_CAST "core"))
        {
            szKey = xmlNodeListGetString(doc, pstTempNode->children, 1);
            if (!xmlStrcmp(szKey, BAD_CAST "UA"))
            {
                g_pstCfg->stSipCfg.stTuCfg.eType = SIP_CORE_UA;
            }
            else if (!xmlStrcmp(szKey, BAD_CAST "REGISTAR"))
            {
                g_pstCfg->stSipCfg.stTuCfg.eType = SIP_CORE_REGISTAR;
            }
            else if (!xmlStrcmp(szKey, BAD_CAST "STATEFULL_PROXY"))
            {
                g_pstCfg->stSipCfg.stTuCfg.eType = SIP_CORE_STATEFULL_PROXY;
            }
            else if (!xmlStrcmp(szKey, BAD_CAST "STATELESS_PROXY"))
            {
                g_pstCfg->stSipCfg.stTuCfg.eType = SIP_CORE_STATELESS_PROXY;
            }
            else
            {
                xmlFree(szKey);
                return FAIL;
            }

            xmlFree(szKey);
            return SUCCESS;
        }

        pstTempNode = pstTempNode->next;
    }

    return FAIL;
}


ULONG SIP_ADPT_FreeTuConfig(SIP_ADAPT_CFG_S *g_pstCfg)
{
    return SUCCESS;
}
ULONG SIP_ADPT_BuildTransactionConfig(SIP_ADAPT_CFG_S *g_pstCfg,
                                      xmlDocPtr doc,
                                      xmlNodePtr pstNode)
{
    xmlNodePtr pstTempNode;
    xmlChar   *szKey;

    /* 解析T1 */
    pstTempNode = pstNode->children;
    while(pstTempNode != NULL_PTR)
    {
        if (!xmlStrcmp(pstTempNode->name, BAD_CAST "T1"))
        {
            szKey = xmlNodeListGetString(doc, pstTempNode->children, 1);
            g_pstCfg->stSipCfg.stTxnCfg.ulTimerT1 = (ULONG)atol(szKey);
            xmlFree(szKey);
            return SUCCESS;
        }

        pstTempNode = pstTempNode->next;
    }

    return FAIL;
}


ULONG SIP_ADPT_FreeTransactionConfig(SIP_ADAPT_CFG_S *g_pstCfg)
{
    return SUCCESS;
}


ULONG SIP_ADPT_BuildTransportConfig(SIP_ADAPT_CFG_S *g_pstCfg,
                                    xmlDocPtr doc,
                                    xmlNodePtr pstNode)
{
    xmlNodePtr pstTempNode;
    xmlChar   *szKey;

    /* 查找sendby节点 */
    pstTempNode = pstNode->children;
    while (pstTempNode != NULL_PTR)
    {
        if (!xmlStrcmp(pstTempNode->name, BAD_CAST "sendby"))
        {
            szKey = xmlNodeListGetString(doc, pstTempNode->children, 1);
            g_pstCfg->stSipCfg.stTxpCfg.pucSendby = malloc(SIP_ADAPT_SENDBY_LEN+1);
            strncpy(g_pstCfg->stSipCfg.stTxpCfg.pucSendby, szKey, SIP_ADAPT_SENDBY_LEN);
            xmlFree(szKey);
            return SUCCESS;
        }

        pstTempNode = pstTempNode->next;
    }

    return FAIL;
}


ULONG SIP_ADPT_FreeTransportConfig(SIP_ADAPT_CFG_S *g_pstCfg)
{
    free(g_pstCfg->stSipCfg.stTxpCfg.pucSendby);
    return SUCCESS;
}


ULONG SIP_ADPT_BuildSyntaxtConfig(SIP_ADAPT_CFG_S *g_pstCfg,
                                  xmlDocPtr doc,
                                  xmlNodePtr pstNode)
{
    xmlNodePtr pstTempNode;
    xmlChar   *szKey;
    ULONG      ulLen;

    /* 查找sendby节点 */
    pstTempNode = pstNode->children;
    while (pstTempNode != NULL_PTR)
    {
        if (!xmlStrcmp(pstTempNode->name, BAD_CAST "dir"))
        {
            szKey = xmlNodeListGetString(doc, pstTempNode->children, 1);
            g_pstCfg->stSipCfg.stSyntaxCfg.pucAbnfDes
                                         = SYS_GetFileContent(szKey, &ulLen);
            g_pstCfg->stSipCfg.stSyntaxCfg.ulLen= ulLen;
            return SUCCESS;
        }

        pstTempNode = pstTempNode->next;
    }

    return FAIL;
}

ULONG SIP_ADPT_FreeSyntaxConfig(SIP_ADAPT_CFG_S *g_pstCfg)
{
    SYS_FreeFileContent(g_pstCfg->stSipCfg.stSyntaxCfg.pucAbnfDes);
    return SUCCESS;
}


ULONG SIP_ADPT_BuildConnConfig(SIP_ADAPT_CFG_S *g_pstCfg,
                               xmlDocPtr doc,
                               xmlNodePtr pstNode)
{
    xmlNodePtr pstTempNode;
    UCHAR      ucListenNum = 0;
    xmlChar   *szKey;

    /* 查找listen节点 */
    pstTempNode = pstNode->children;
    while (pstTempNode != NULL_PTR)
    {
        if (xmlStrcmp(pstTempNode->name, BAD_CAST "item"))
        {
            pstTempNode = pstTempNode->next;
            continue;
        }

        szKey = xmlGetProp(pstTempNode, BAD_CAST"protocol");
        if(!xmlStrcmp(BAD_CAST(szKey), BAD_CAST "TCP"))
        {
            g_pstCfg->stConnCfg.stListen[ucListenNum].eProtocol
                = SIP_TRANSPORT_PROTOCOL_TCP;
        }
        else if(!xmlStrcmp(BAD_CAST(szKey), BAD_CAST "UDP"))
        {
            g_pstCfg->stConnCfg.stListen[ucListenNum].eProtocol
                = SIP_TRANSPORT_PROTOCOL_UDP;
        }
        else
        {
            printf("\r\nprotocol not support");
            return FAIL;
        }

        szKey = xmlGetProp(pstTempNode, BAD_CAST"addr");
        if(!xmlStrcmp(BAD_CAST(szKey), BAD_CAST "*"))
        {
            g_pstCfg->stConnCfg.stListen[ucListenNum].ulIpAddr
                = NULL_ULONG;
        }
        else
        {
            g_pstCfg->stConnCfg.stListen[ucListenNum].ulIpAddr = inet_addr(szKey);
        }

        szKey = xmlGetProp(pstTempNode, BAD_CAST"port");
        g_pstCfg->stConnCfg.stListen[ucListenNum].usPort = (USHORT)strtod(szKey, NULL_PTR);

        ucListenNum++;
        if (ucListenNum == SIP_TXP_MAX_LISTEN_NUM)
        {
            break;
        }

        pstTempNode = pstTempNode->next;
    }

    if (ucListenNum == 0)
    {
        return FAIL;
    }

    g_pstCfg->stConnCfg.ucListenNum = ucListenNum;
    g_pstCfg->stConnCfg.pfnMsgProc = SIP_ADPT_RecvUpMsg;
    return SUCCESS;
}

ULONG SIP_ADPT_FreeConnConfig(SIP_ADAPT_CFG_S *g_pstCfg)
{
    return SUCCESS;
}

/* 接受网络消息 */
ULONG SIP_ADPT_RecvUpMsg(SIP_LOCATION_S *pstLocation,
                         UCHAR *pucMsg,
                         ULONG ulMsgLen)
{
    ULONG ulRet;
    SIP_ADPT_CONN_MSG_S stConnMsg;
    UBUF_HEADER_S      *pstUBuf = NULL_PTR;
    ULONG ulRuleIndex;
    SIP_MSG_S *pstSipMsg = NULL_PTR;

    printf("\r\nRecv Sip Msg:\r\n%s",pucMsg);
    pstUBuf = UBUF_AllocUBuf(SIP_MAX_UBUF_MSG_LEN);
    SIP_GetRuleIndex("SIP-message", &ulRuleIndex);
    ulRet = SIP_Decode(ulRuleIndex,
                       pucMsg,
                       ulMsgLen,
                       pstUBuf,
                      &pstSipMsg);
    if (ulRet != SUCCESS)
    {
        UBUF_FreeBuffer(pstUBuf);
        return ulRet;
    }

    memcpy(&stConnMsg.stLocation, pstLocation, sizeof(SIP_LOCATION_S));
    stConnMsg.pstMsgUbuf = pstUBuf;

    return SYS_SendMsg(SYS_MODULE_CONN,
                       SYS_MODULE_SIP,
                      &stConnMsg,
                       sizeof(SIP_ADPT_CONN_MSG_S));
}

/* 发送用户消息 */
ULONG SIP_ADPT_SendUpMsg(ULONG ulStackRef1,
                         ULONG ulStackRef2,
                         ULONG AppRef1,
                         ULONG AppRef2,
                         UBUF_HEADER_S *pstUbufSipMsg)
{
    APP_MSG_S stAppMsg;

    stAppMsg.ulAppRef1 = AppRef1;
    stAppMsg.ulAppRef2 = AppRef1;
    stAppMsg.ulStackRef1 = ulStackRef1;
    stAppMsg.ulStackRef2 = ulStackRef2;
    stAppMsg.pstUbufSipMsg = pstUbufSipMsg;

    return SYS_SendMsg(SYS_MODULE_SIP,
                       SYS_MODULE_APP,
                      &stAppMsg,
                       sizeof(APP_MSG_S));
}

/* 发送网络消息 */
ULONG SIP_ADPT_SendDownMsg(UBUF_HEADER_S  *pstSipMsgUbuf,
                           SIP_LOCATION_S *pstPeerLocation)
{
    ULONG ulMsgLen;
    ULONG ulRuleIndex;
    SIP_MSG_S *pstSipMsg = NULL_PTR;

    pstSipMsg = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstSipMsgUbuf);
    SIP_GetRuleIndex("SIP-message", &ulRuleIndex);
    SIP_Code(ulRuleIndex,
                    pstSipMsg,
                    g_pucSipAdptBuffer,
                    SIP_MAX_TEXT_MSG_LEN,
                    &ulMsgLen);
    g_pucSipAdptBuffer[ulMsgLen] = '\0';
    printf("\r\nSend Sip Msg:\r\n%s",g_pucSipAdptBuffer);
    return CONN_SendMsg(g_pucSipAdptBuffer, ulMsgLen, pstPeerLocation);
}

ULONG SIP_ADPT_StartTimer(ULONG ulName,
                          ULONG ulLen,
                          ULONG ulPara,
                          SIP_TIMER_TYPE_E eType,
                          ULONG *pulHandle)
{
    return TIMER_Start(SYS_MODULE_SIP, ulName, ulLen, ulPara, eType, pulHandle);
}

VOID SIP_ADPT_StopTimer(ULONG ulHandle)
{
    TIMER_Stop(ulHandle);
    return;
}
