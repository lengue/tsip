/*******************************************************************************
功能    : ABNF模块功能函数
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/
typedef struct tagHASH_NODE_S
{
    ULONG ulPara;
    ULONG ulKey;
    struct tagHASH_NODE_S **ppstHead;
    struct tagHASH_NODE_S  *pstNext; /*后一个节点*/
}HASH_NODE_S;

typedef struct tagHASH_TABLE_S
{
    FN_HASH_MAKE_KEY pfnMakeKey;
    FN_HASH_COMPARE  pfnCompare;
    ULONG            ulNumber;
    HASH_NODE_S     *apstNode[0];
}HASH_TABLE_S;