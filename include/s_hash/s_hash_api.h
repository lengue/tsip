/*******************************************************************************
����    : HASHģ��
������  : �ƴ�ƽ
��������: 200.01.10
�޸ļ�¼:
*******************************************************************************/
extern void *HASH_CreateTable(ULONG ulNumber,
                       FN_HASH_MAKE_KEY  pfnMakeKey,
                       FN_HASH_COMPARE   pfnCompare);
extern ULONG HASH_DestroyTable(void *pstHash);
extern ULONG HASH_Find(void *pstHash, void *pCompared);
extern ULONG HASH_AddNode(void *pstHash, void *pCompared, ULONG ulPara);
extern ULONG HASH_DeleteNode(void *pstHash, void *pCompared, ULONG ulPara);

