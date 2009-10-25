ULONG SIP_UAC_Init();
ULONG SIP_UAC_AllocCB(ULONG *pulUacID);
VOID SIP_UAC_FreeCB(ULONG ulUacID);
ULONG SIP_UAC_SendRequest(ULONG ulAppID,
                          ULONG ulDlgID,
                          UBUF_HEADER_S * pstUbufSipMsg);
ULONG SIP_UAC_ProcessingResponse(ULONG ulCoreID,
                                 UBUF_HEADER_S *pstUbufSipMsg);
ULONG SIP_UAC_LocateResult(SIP_LOCATION_RESULT_S *pstResult,
                           ULONG ulPara);
ULONG SIP_UAC_GenerateRequest(UBUF_HEADER_S * pstUbufSipMsg);
ULONG SIP_UAC_AddViaHeader(UBUF_HEADER_S *pstUbufSipMsg);