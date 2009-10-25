/*******************************************************************************
功能    : SIP协议栈传输层功能函数
创建人  : 唐春平
创建日期: 2009.01.10
修改记录:
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
