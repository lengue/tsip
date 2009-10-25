extern ULONG SIP_Locate_Server(URI_S *pstUri,
                               SIP_LOCATION_RESULT_S   *pstResult,
                               SIP_LOCATION_RESULT_PROC pfnProc,
                               ULONG ulPara);
extern ULONG SIP_Locate_FindNextHop(UBUF_HEADER_S *pstUbufSipMsg, URI_S **ppstUri);
