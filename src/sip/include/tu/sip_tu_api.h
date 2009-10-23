extern ULONG SIP_TU_Init(SIP_TU_CFG_S *pstCfg);
extern ULONG SIP_TU_RecvUpMsg(ULONG ulTxnID,
                       SIP_LOCATION_S *pstPeerAddr,
                       UBUF_HEADER_S *pstUbufSipMsg);