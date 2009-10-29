extern ULONG SIP_SendDownMsg(UBUF_HEADER_S  *pstSipMsgUbuf,
                             SIP_LOCATION_S *pstPeerLocation);
extern ULONG SIP_StartTimer(ULONG ulName,
                     ULONG ulLen,
                     ULONG ulPara,
                     SIP_TIMER_TYPE_E eType,
                     ULONG *pulHandle);
extern ULONG SIP_StopTimer(ULONG ulHandle);
extern ULONG SIP_GenerateRandomString(CHAR *pcBuffer, ULONG ulBufferLen);
extern ULONG SIP_SendUpMsg(ULONG ulStackRef1,
                           ULONG ulStackRef2,
                           ULONG ulAppRef1,
                           ULONG ulAppRef2,
                           UBUF_HEADER_S *pstUbufSipMsg);