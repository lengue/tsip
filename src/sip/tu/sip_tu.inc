ULONG SIP_TU_Init(SIP_TU_CFG_S *pstCfg);
ULONG SIP_TU_RecvUpMsg(ULONG ulCoreID,
                       ULONG ulTxnID,
                       SIP_LOCATION_S *pstPeerAddr,
                       UBUF_HEADER_S  *pstUbufSipMsg);
ULONG SIP_TU_RecvDownMsg(ULONG ulAppRef1,
                         ULONG ulAppRef2,
                         ULONG *pulStackRef1,
                         ULONG *pulStackRef2,
                         UBUF_HEADER_S  *pstUbufSipMsg);
ULONG SIP_TU_MapRef1(ULONG ulStackRef1, ULONG ulAppRef1);
ULONG SIP_TU_MapRef2(ULONG ulStackRef1, ULONG ulAppRef1);