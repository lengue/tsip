/*******************************************************************************
����    : ABNFģ�鹦�ܺ���
������  : �ƴ�ƽ
��������: 200.01.10
�޸ļ�¼:
*******************************************************************************/
typedef struct tagHASH_NODE_S
{
    ULONG ulPara;
    struct tagHASH_NODE_S *pstNext;
}HASH_NODE_S;

typedef struct tagHASH_TABLE_S
{
    FN_HASH_MAKE_KEY pfnMakeKey;
    FN_HASH_COMPARE  pfnCompare;
    ULONG            ulNumber;
    HASH_NODE_S     *apstNode[0];
}HASH_TABLE_S;