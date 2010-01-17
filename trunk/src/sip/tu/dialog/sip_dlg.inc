ULONG SIP_Dlg_Init();
ULONG SIP_Dlg_AllocDialog(UCHAR *pucCallID, 
                          UCHAR *pucLocalTag, 
                          UCHAR *pucRemoteTag,
                          ULONG *pulDlgIndex);
ULONG SIP_Dlg_ReleaseDialog(ULONG ulDlgIndex);
ULONG SIP_Dlg_UpdateRemoteSeq(ULONG ulDlgID, ULONG ulRemoteSeq);
ULONG SIP_Dlg_UpdateLocalSeq(ULONG ulDlgID, ULONG ulLocalSeq);
ULONG SIP_Dlg_UpdateLocalURI(ULONG ulDlgID, URI_S *pstLocalURI);
ULONG SIP_Dlg_UpdateRemoteURI(ULONG ulDlgID, URI_S *pstRemoteURI);
ULONG SIP_Dlg_UpdateRouteSet(ULONG ulDlgID, SIP_ROUTE_PARAM_S *pstRouteSet);
ULONG SIP_Dlg_UpdateRemoteTarget(ULONG ulDlgID, URI_S *pstRemoteTarget);
ULONG SIP_Dlg_UpdateSecureFlag(ULONG ulDlgID, UCHAR ucSecureFlag);
ULONG SIP_Dlg_GenerateRequest(ULONG ulDlgIndex, UBUF_HEADER_S *pstUbufSipMsg);
ULONG SIP_Dlg_MakeKey(void *pCompared);
ULONG SIP_Dlg_Compare(void *pCompared, ULONG ulPara);
ULONG SIP_Dlg_MatchDialog(UBUF_HEADER_S *pstUbufSipMsg, ULONG *pulDlgIndex);
ULONG SIP_Dlg_IDMap(ULONG ulDlgID, ULONG ulAppDlgID);

