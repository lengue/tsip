/*******************************************************************************
功能    : ABNF模块功能函数
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/
/* 外部依赖模块头文件 */
#include "common\common.h"
#include "s_hash\s_hash.h"

/* 本模块对外提供的常量和结构头文件 */


/* 本模块内部使用的常量和结构头文件 */
#include "s_hash_type.inc"

/* 本模块内部函数声明头文件 */
#include "s_hash.inc"

/* 本模块全局变量声明头文件 */

/* 创建一张Hash表 */
void *HASH_CreateTable(ULONG ulNumber,
                       FN_HASH_MAKE_KEY  pfnMakeKey,
                       FN_HASH_COMPARE   pfnCompare)
{
    HASH_TABLE_S *pstHashTable = NULL_PTR;

    /* 构造一个ulNum长度的数组 */
    pstHashTable = malloc(sizeof(HASH_TABLE_S) + ulNumber * sizeof(HASH_NODE_S));

    /* 记录函数参数 */
    pstHashTable->ulNumber   = ulNumber;
    pstHashTable->pfnCompare = pfnCompare;
    pstHashTable->pfnMakeKey = pfnMakeKey;

    /* 初始化元素数组 */
    memset(pstHashTable->apstNode, 0x0, ulNumber * sizeof(HASH_NODE_S *));

    /* 返回结构 */
    return pstHashTable;
}

/* 销毁hash表 */
ULONG HASH_DestroyTable(void *pstHash)
{
    ULONG ulLoop;
    HASH_TABLE_S *pstHashTable = NULL_PTR;

    pstHashTable = (HASH_TABLE_S *)pstHash;
    for (ulLoop = 0; ulLoop < pstHashTable->ulNumber; ulLoop++)
    {
        if (pstHashTable->apstNode[ulLoop] != NULL_PTR)
        {
            HASH_DestroyNode(pstHashTable->apstNode[ulLoop]);
        }
    }

    free(pstHashTable);
    return SUCCESS;
}

/*销毁一个节点*/
ULONG HASH_DestroyNode(HASH_NODE_S  *pstHashNode)
{
    if (pstHashNode == NULL_PTR)
    {
        return SUCCESS;
    }

    if (pstHashNode->pstNext != NULL_PTR)
    {
        HASH_DestroyNode(pstHashNode->pstNext);
    }

    free(pstHashNode);
    return SUCCESS;
}

/* 在HASH表中查找 */
ULONG HASH_Find(void *pstHash, void *pCompared)
{
    ULONG ulKey;
    ULONG ulRet = FAIL;
    HASH_TABLE_S *pstHashTable = NULL_PTR;
    HASH_NODE_S  *pstHashNode  = NULL_PTR;

    pstHashTable = (HASH_TABLE_S *)pstHash;

    /* 生成key */
    ulKey = pstHashTable->pfnMakeKey(pCompared);
    ulKey = ulKey % pstHashTable->ulNumber;

    pstHashNode = pstHashTable->apstNode[ulKey];
    while(pstHashNode != NULL_PTR)
    {
        /* 进行比较 */
        ulRet = pstHashTable->pfnCompare(pCompared, pstHashNode->ulPara);
        if(ulRet == SUCCESS)
        {
            return pstHashNode->ulPara;
        }
        else
        {
            pstHashNode = pstHashNode->pstNext;
        }
    }

    return NULL_ULONG;
}

/* 向HASH表中插入一个元素 */
void *HASH_AddNode(void *pstHash, void *pCompared, ULONG ulPara)
{
    ULONG ulKey;
    HASH_TABLE_S *pstHashTable = NULL_PTR;
    HASH_NODE_S  **ppstHashNode  = NULL_PTR;

    pstHashTable = (HASH_TABLE_S *)pstHash;

    /* 生成key */
    ulKey = pstHashTable->pfnMakeKey(pCompared);
    ulKey = ulKey % pstHashTable->ulNumber;

    ppstHashNode = &pstHashTable->apstNode[ulKey];
    while(*ppstHashNode != NULL_PTR)
    {
        ppstHashNode = &(*ppstHashNode)->pstNext;
    }

    *ppstHashNode = malloc(sizeof(HASH_NODE_S));
    (*ppstHashNode)->ulPara   = ulPara;
    (*ppstHashNode)->ppstHead = &pstHashTable->apstNode[ulKey];
    (*ppstHashNode)->pstNext  = NULL_PTR;

    return *ppstHashNode;
}

/* 从HASH表中删除一个元素 */
ULONG HASH_DeleteNode(void *pstNode)
{
    HASH_NODE_S  *pstHashNode   = NULL_PTR;
    HASH_NODE_S  **ppstNodeTmp  = NULL_PTR;

    pstHashNode = (HASH_NODE_S *)pstNode;
    ppstNodeTmp = pstHashNode->ppstHead;

    /* 找到并删除元素 */
    while ((*ppstNodeTmp != NULL_PTR) && (*ppstNodeTmp != pstHashNode))
    {
        ppstNodeTmp = &(*ppstNodeTmp)->pstNext;
    }
    
    if (*ppstNodeTmp == NULL_PTR)
    {
        return FAIL;
    }
    
    *ppstNodeTmp = pstHashNode->pstNext;
    free(pstHashNode);
    return SUCCESS;
}

