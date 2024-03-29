/*******************************************************************************
功能    : 传输层全局变量声明
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/
ULONG SIP_ADPT_Init();
ULONG SIP_APDT_MsgProc(ULONG ulModuleID, void* pMsg);
ULONG SIP_ADPT_BuildConfigStruct(SIP_ADAPT_CFG_S *g_pstCfg);
ULONG SIP_ADPT_FreeConfigStruct(SIP_ADAPT_CFG_S *g_pstCfg);
ULONG SIP_ADPT_BuildTuConfig(SIP_ADAPT_CFG_S *g_pstCfg,
                             xmlDocPtr doc,
                             xmlNodePtr pstNode);
ULONG SIP_ADPT_FreeTuConfig(SIP_ADAPT_CFG_S *g_pstCfg);
ULONG SIP_ADPT_BuildTransactionConfig(SIP_ADAPT_CFG_S *g_pstCfg,
                                      xmlDocPtr doc,
                                      xmlNodePtr pstNode);
ULONG SIP_ADPT_FreeTransactionConfig(SIP_ADAPT_CFG_S *g_pstCfg);
ULONG SIP_ADPT_BuildTransportConfig(SIP_ADAPT_CFG_S *g_pstCfg,
                                    xmlDocPtr doc,
                                    xmlNodePtr pstNode);
ULONG SIP_ADPT_FreeTransportConfig(SIP_ADAPT_CFG_S *g_pstCfg);
ULONG SIP_ADPT_BuildSyntaxtConfig(SIP_ADAPT_CFG_S *g_pstCfg,
                                  xmlDocPtr doc,
                                  xmlNodePtr pstNode);
ULONG SIP_ADPT_FreeSyntaxConfig(SIP_ADAPT_CFG_S *g_pstCfg);
ULONG SIP_ADPT_BuildConnConfig(SIP_ADAPT_CFG_S *g_pstCfg,
                               xmlDocPtr doc,
                               xmlNodePtr pstNode);
ULONG SIP_ADPT_FreeConnConfig(SIP_ADAPT_CFG_S *g_pstCfg);
ULONG SIP_ADPT_StartTimer(ULONG ulName,
                          ULONG ulLen,
                          ULONG ulPara,
                          SIP_TIMER_TYPE_E eType,
                          ULONG *pulHandle);
ULONG SIP_ADPT_RecvUpMsg(CONN_LOCATION_S *pstLocation,
                         UCHAR *pucMsg,
                         ULONG ulMsgLen);
ULONG SIP_ADPT_SendUpMsg(ULONG ulStackRef1,
                         ULONG ulStackRef2,
                         ULONG AppRef1,
                         ULONG AppRef2,
                         UBUF_HEADER_S *pstUbufSipMsg);
ULONG SIP_ADPT_SendDownMsg(UBUF_HEADER_S  *pstSipMsgUbuf,
                           SIP_LOCATION_S *pstPeerLocation);
VOID SIP_ADPT_StopTimer(ULONG ulHandle);

ULONG SIP_ADPT_LocationSip2Conn(SIP_LOCATION_S  *pstSipLocation, 
                                CONN_LOCATION_S *pstConnLocation);
ULONG SIP_ADPT_LocationConn2Sip(CONN_LOCATION_S *pstConnLocation, 
                                SIP_LOCATION_S  *pstSipLocation);

