extern ULONG SIP_UA_Init();
extern ULONG SIP_UA_RecvDownMsg(ULONG ulAppRef1,
                                ULONG ulAppRef2,
                                ULONG *pulStackRef1,
                                ULONG *pulStackRef2,
                                UBUF_HEADER_S *pstUbufSipMsg);
extern ULONG SIP_UA_RecvUpMsg(ULONG ulCoreID,
                              ULONG ulTxnID,
                              SIP_LOCATION_S *pstPeerAddr,
                              UBUF_HEADER_S  *pstUbufSipMsg);

extern ULONG SIP_UA_MapRef1(ULONG ulStackRef1, ULONG ulAppRef1);
extern ULONG SIP_UA_MapRef2(ULONG ulStackRef1, ULONG ulAppRef1);