/*******************************************************************************
功能    : 传输层消息管理(WIN32专用)
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/
ULONG SIP_UAS_Init();
ULONG SIP_UAS_AllocCB(ULONG *pulUasID);
VOID SIP_UAS_FreeCB(ULONG ulUasID);
ULONG SIP_UAS_ProcessingRequest(ULONG ulTxnID,
                                SIP_LOCATION_S *pstPeerAddr,
                                UBUF_HEADER_S  *pstUbufSipMsg);
ULONG SIP_UAS_SendResponse(ULONG ulAppDlgID,
                           ULONG ulAppTxnID,
                           ULONG *pulDlgID,
                           ULONG ulUasID,
                           UBUF_HEADER_S * pstUbufSipMsg);
ULONG SIP_UAS_MethodInspection(UBUF_HEADER_S *pstSipMsgUbuf);
ULONG SIP_UAS_HeaderInspection(UBUF_HEADER_S *pstSipMsgUbuf);
ULONG SIP_UAS_ContentProcessing(UBUF_HEADER_S *pstSipMsgUbuf);
ULONG SIP_UAS_ApplyingExtensions(UBUF_HEADER_S *pstSipMsgUbuf);
ULONG SIP_UAS_ProcessingRequest(ULONG ulCoreID,
                                SIP_LOCATION_S *pstPeerAddr,
                                UBUF_HEADER_S  *pstUbufSipMsg);
ULONG SIP_UAS_GenerateResponse(UBUF_HEADER_S *pstUbufRequest, UBUF_HEADER_S *pstUbufResponse);