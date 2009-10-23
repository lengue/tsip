extern ULONG SIP_Locate_Server(UBUF_HEADER_S *pstUbuf,
                               UBUF_PTR       upUri,
                               SIP_LOCATION_RESULT_S   *pstResult,
                               SIP_LOCATION_RESULT_PROC pfnProc,
                               ULONG ulPara);
extern ULONG SIP_Locate_FindNextHop(UBUF_HEADER_S *pstUbufSipMsg, UBUF_PTR *pupUri);