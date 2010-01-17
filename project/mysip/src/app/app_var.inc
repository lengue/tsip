/*******************************************************************************
功能    : 应用模块程序
创建人  : 唐春平
创建日期: 2009.07.19
修改记录:
*******************************************************************************/
extern APP_FN_FSM_PROC g_apfnAppFsm[APP_STATE_BUTT][APP_EVENT_BUTT];
extern APP_STATE_E g_eAppState;
extern UCHAR g_ucRemoteAlertFlag;
extern UCHAR *g_pucAppPublicID;
extern ULONG g_ulStackDlgID;
extern ULONG g_ulStackTxnID;