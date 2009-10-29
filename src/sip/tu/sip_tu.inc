ULONG SIP_TU_Init(SIP_TU_CFG_S *pstCfg);
ULONG SIP_TU_RecvDownMsg(ULONG ulAppRef1,
                         ULONG ulAppRef2,
                         ULONG ulStackRef1,
                         ULONG ulStackRef2,
                         UBUF_HEADER_S  *pstUbufSipMsg);