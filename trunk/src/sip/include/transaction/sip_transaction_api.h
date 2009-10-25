extern ULONG SIP_Txn_AllocTxn(ULONG  ulCoreID, ULONG *pulTxnID);
extern ULONG SIP_Txn_RecvDownMsg(ULONG ulTxnID,
                                 UBUF_HEADER_S  *pstUbufSipMsg,
                                 SIP_LOCATION_S *pstPeerLocation);
extern ULONG SIP_Txn_Init(SIP_TXN_CFG_S *pstCfg);
extern ULONG SIP_Txn_Match(UBUF_HEADER_S *pstUbufSipMsg, ULONG *pulTxnID);
extern ULONG SIP_Txn_RecvUpMsg(ULONG ulTxnID,
                               UBUF_HEADER_S *pstUbufSipMsg,
                               SIP_LOCATION_S *pstPeerLocation);
extern ULONG SIP_Txn_TimerProc(ULONG ulTxnID, SIP_TXN_TIMER_NAME_E eTimer);