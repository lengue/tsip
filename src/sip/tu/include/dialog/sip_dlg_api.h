extern ULONG SIP_Dlg_Init();
extern ULONG SIP_Dlg_AllocDialog(UCHAR *pucCallID, 
                                  UCHAR *pucLocalTag, 
                                  UCHAR *pucRemoteTag,
                                  ULONG *pulDlgIndex);
extern ULONG SIP_Dlg_ReleaseDialog(ULONG ulDlgIndex);
extern ULONG SIP_Dlg_UpdateRemoteSeq(ULONG ulDlgID, ULONG ulRemoteSeq);
extern ULONG SIP_Dlg_UpdateLocalSeq(ULONG ulDlgID, ULONG ulLocalSeq);
extern ULONG SIP_Dlg_UpdateLocalURI(ULONG ulDlgID, URI_S *pstLocalURI);
extern ULONG SIP_Dlg_UpdateRemoteURI(ULONG ulDlgID, URI_S *pstRemoteURI);
extern ULONG SIP_Dlg_UpdateRouteSet(ULONG ulDlgID, SIP_ROUTE_PARAM_S *pstRouteSet);
extern ULONG SIP_Dlg_UpdateRemoteTarget(ULONG ulDlgID, URI_S *pstRemoteTarget);
extern ULONG SIP_Dlg_UpdateSecureFlag(ULONG ulDlgID, UCHAR ucSecureFlag);
extern ULONG SIP_Dlg_UpdateSessionState(ULONG ulDlgID, SIP_DLG_SESSION_STATE_E eState);
extern ULONG SIP_Dlg_GetSessionState(ULONG ulDlgID, SIP_DLG_SESSION_STATE_E *peState);
extern ULONG SIP_Dlg_GenerateRequest(ULONG ulDlgIndex, UBUF_HEADER_S *pstUbufSipMsg);
extern ULONG SIP_Dlg_CanDialogRelease(ULONG ulDlgID);
extern ULONG SIP_Dlg_IDMap(ULONG ulDlgID, ULONG ulAppDlgID);