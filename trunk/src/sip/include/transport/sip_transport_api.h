/*******************************************************************************
功能    : SIP传输层对外提供API
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/
extern ULONG SIP_Txp_RecvUpMsg(SIP_LOCATION_S *pstPeerLocation,
                                UBUF_HEADER_S  *pstSipMsgUbuf);
extern ULONG SIP_Txp_RecvDownMsg(UBUF_HEADER_S  *pstSipMsgUbuf,
                                  SIP_LOCATION_S *pstPeerLocation);
extern ULONG SIP_Txp_Init();