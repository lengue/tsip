/*******************************************************************************
����    : SIPЭ��ջ����㹦�ܺ���
������  : �ƴ�ƽ
��������: 2009.01.10
�޸ļ�¼:
*******************************************************************************/
ULONG SIP_Txn_AllocTxn(ULONG  ulCoreID, ULONG *pulTxnID);
ULONG SIP_Txn_FreeTxn(ULONG ulTxnID);
ULONG SIP_Txn_Match(UBUF_HEADER_S *pstUbufSipMsg, ULONG *pulTxnID);
ULONG SIP_Txn_MakeKey(void *pCompared);
ULONG SIP_Txn_Compare(void *pCompared, ULONG ulPara);
ULONG SIP_Txn_CompareResponse(UBUF_HEADER_S *pstUbufResponse,
                              UBUF_HEADER_S *pstUbufTxnInitMsg);
ULONG SIP_Txn_CompareRequest(UBUF_HEADER_S *pstUbufRequest,
                             UBUF_HEADER_S *pstUbufTxnInitMsg);
ULONG SIP_Txn_CompareSendBy(UBUF_HEADER_S *pstUbufRequest,
                            UBUF_HEADER_S *pstUbufTxnInitMsg);
ULONG SIP_Txn_GenerateBranch(ULONG  *pulTxnID);