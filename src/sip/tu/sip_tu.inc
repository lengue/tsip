ULONG SIP_TU_Init(SIP_TU_CFG_S *pstCfg);
ULONG SIP_TU_RecvDownMsg(ULONG ulAppRef1,
                         ULONG ulAppRef2,
                         ULONG *pulStackRef1,
                         ULONG *pulStackRef2,
                         UBUF_HEADER_S  *pstUbufSipMsg);