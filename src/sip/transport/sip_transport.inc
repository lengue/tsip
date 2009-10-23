/*******************************************************************************
功能    : 传输层全局变量声明
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/
ULONG SIP_Txp_Init();
ULONG SIP_Txp_RecvDownMsg(UBUF_HEADER_S  *pstSipMsgUbuf,
                          SIP_LOCATION_S *pstPeerLocation);
ULONG SIP_Txp_RecvUpMsg(SIP_LOCATION_S *pstPeerLocation,
                        UBUF_HEADER_S  *pstSipMsgUbuf);
ULONG SIP_Txp_ReceiveRequest(SIP_LOCATION_S *pstPeerLocation,
                             UBUF_HEADER_S *pstUbufSipMsg);
ULONG SIP_Txp_ReceiveReponse(SIP_LOCATION_S *pstPeerLocation,
                             UBUF_HEADER_S *pstUbufSipMsg);
ULONG SIP_Txp_AddViaHeaderInRequest(UBUF_HEADER_S *pstSipMsgUbuf,
                                    SIP_LOCATION_S *pstPeerLocation);