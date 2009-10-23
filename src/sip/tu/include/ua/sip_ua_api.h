extern ULONG SIP_UA_RecvDownMsg(ULONG  ulAppRef1,
                         ULONG  ulAppRef2,
                         ULONG *pulSipRef1,
                         ULONG *pulSipRef2,
                         UBUF_HEADER_S * pstUbufSipMsg);
extern ULONG SIP_UAS_SendResponse(IN  ULONG ulTxnID,
                                  OUT ULONG *pulDlgID,
                                  UBUF_HEADER_S * pstUbufSipMsg);
extern ULONG SIP_UA_Init();
extern ULONG SIP_UA_RecvUpMsgFromTxn();
extern ULONG SIP_UA_RecvUpMsgFromTxp(ULONG ulTxnID, UBUF_HEADER_S * pstUbufSipMsg);
