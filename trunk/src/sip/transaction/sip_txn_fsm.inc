/*******************************************************************************
����    : SIPЭ��ջ����㹦�ܺ���
������  : �ƴ�ƽ
��������: 2009.01.10
�޸ļ�¼:
*******************************************************************************/
ULONG SIP_Txn_FsmInit();
ULONG SIP_Txn_FsmProc(ULONG ulTxnID, SIP_TXN_EVENT_E eEvent);
ULONG SIP_Txn_ActSendMsg(ULONG ulTxnID, ULONG ulPara);
ULONG SIP_Txn_ActPassMsg(ULONG ulTxnID, ULONG ulPara);
ULONG SIP_Txn_ActInformTU(ULONG ulTxnID, ULONG ulPara);
ULONG SIP_Txn_ActSendAck(ULONG ulTxnID, ULONG ulPara);
ULONG SIP_Txn_ActStartTimer(ULONG ulTxnID, ULONG ulPara);
ULONG SIP_Txn_ActStopTimer(ULONG ulTxnID, ULONG ulPara);
ULONG SIP_Txn_StartTimerA(ULONG ulTxnID);
ULONG SIP_Txn_StartTimerB(ULONG ulTxnID);
ULONG SIP_Txn_LookupTimer(ULONG ulTxnID, SIP_TXN_TIMER_NAME_E eName, UCHAR *pucSeq);
ULONG SIP_Txn_AllocTimer(ULONG ulTxnID, SIP_TXN_TIMER_NAME_E eName, UCHAR *pucSeq);
ULONG SIP_Txn_StopTimer(ULONG ulTxnID, SIP_TXN_TIMER_NAME_E eName);