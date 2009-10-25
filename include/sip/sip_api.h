/*******************************************************************************
功能    : SIP协议栈对外提供的函数
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/
extern ULONG SIP_TimeoutProc(ULONG ulName, ULONG ulPara);
extern ULONG SIP_Config(SIP_CFG_S *pstCfg);
extern ULONG SIP_UAC_SendRequest(ULONG ulAppID,
                                 ULONG ulDlgID,
                                 UBUF_HEADER_S * pstUbufSipMsg);
extern ULONG SIP_UAS_SendResponse(IN  ULONG ulTxnID,
                                   OUT ULONG *pulDlgID,
                                   UBUF_HEADER_S * pstUbufSipMsg);
extern ULONG SIP_Txp_RecvUpMsg(SIP_LOCATION_S *pstPeerLocation,
                                UBUF_HEADER_S  *pstSipMsgUbuf);