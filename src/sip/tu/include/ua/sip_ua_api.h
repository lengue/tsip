extern ULONG SIP_UA_RecvDownMsg(ULONG ulAppRef1,
                                ULONG ulAppRef2,
                                ULONG ulStackRef1,
                                ULONG ulStackRef2,
                                UBUF_HEADER_S *pstUbufSipMsg);
extern ULONG SIP_UAS_SendResponse(IN  ULONG ulTxnID,
                                  OUT ULONG ulDlgID,
                                  UBUF_HEADER_S * pstUbufSipMsg);
extern ULONG SIP_UA_Init();
extern ULONG SIP_UA_RecvUpMsg(ULONG ulCoreID,
                              SIP_LOCATION_S *pstPeerAddr,
                              UBUF_HEADER_S *pstUbufSipMsg);