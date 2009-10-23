ULONG SIP_SendDownMsg(UBUF_HEADER_S  *pstSipMsgUbuf,
                      SIP_LOCATION_S *pstPeerLocation);
ULONG SIP_StartTimer(ULONG ulName,
                     ULONG ulLen,
                     ULONG ulPara,
                     TIMER_TYPE_E eType,
                     ULONG *pulHandle);
ULONG SIP_StopTimer(ULONG ulHandle);
ULONG SIP_GenerateRandomString(CHAR *pcBuffer, ULONG ulBufferLen);