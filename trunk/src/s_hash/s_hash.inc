/*******************************************************************************
����    : HASHģ��
������  : �ƴ�ƽ
��������: 200.01.10
�޸ļ�¼:
*******************************************************************************/
void *HASH_CreateTable(ULONG ulNumber,
                       FN_HASH_MAKE_KEY  pfnMakeKey,
                       FN_HASH_COMPARE   pfnCompare);
ULONG HASH_DestroyTable(void *pstHash);
ULONG HASH_DestroyNode(HASH_NODE_S  *pstHashNode);
ULONG HASH_Find(void *pstHash, void *pCompared);
void *HASH_AddNode(void *pstHash, void *pCompared, ULONG ulPara);
ULONG HASH_DeleteNode(void *pstNode);