/*******************************************************************************
����    : ABNFģ�鹦�ܺ���
������  : �ƴ�ƽ
��������: 200.01.10
�޸ļ�¼:
*******************************************************************************/
/* �ⲿ����ģ��ͷ�ļ� */
#include "common\common.h"
#include "s_hash\s_hash.h"

/* ��ģ������ṩ�ĳ����ͽṹͷ�ļ� */


/* ��ģ���ڲ�ʹ�õĳ����ͽṹͷ�ļ� */
#include "s_hash_type.inc"

/* ��ģ���ڲ���������ͷ�ļ� */
#include "s_hash.inc"

/* ��ģ��ȫ�ֱ�������ͷ�ļ� */

/* ����һ��Hash�� */
void *HASH_CreateTable(ULONG ulNumber,
                       FN_HASH_MAKE_KEY  pfnMakeKey,
                       FN_HASH_COMPARE   pfnCompare)
{
    HASH_TABLE_S *pstHashTable = NULL_PTR;

    /* ����һ��ulNum���ȵ����� */
    pstHashTable = malloc(sizeof(HASH_TABLE_S) + ulNumber * sizeof(HASH_NODE_S));

    /* ��¼�������� */
    pstHashTable->ulNumber   = ulNumber;
    pstHashTable->pfnCompare = pfnCompare;
    pstHashTable->pfnMakeKey = pfnMakeKey;

    /* ��ʼ��Ԫ������ */
    memset(pstHashTable->apstNode, 0x0, ulNumber * sizeof(HASH_NODE_S *));

    /* ���ؽṹ */
    return pstHashTable;
}

/* ����hash�� */
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

/*����һ���ڵ�*/
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

/* ��HASH���в��� */
ULONG HASH_Find(void *pstHash, void *pCompared)
{
    ULONG ulKey;
    ULONG ulRet = FAIL;
    HASH_TABLE_S *pstHashTable = NULL_PTR;
    HASH_NODE_S  *pstHashNode  = NULL_PTR;

    pstHashTable = (HASH_TABLE_S *)pstHash;

    /* ����key */
    ulKey = pstHashTable->pfnMakeKey(pCompared);
    ulKey = ulKey % pstHashTable->ulNumber;

    pstHashNode = pstHashTable->apstNode[ulKey];
    while(pstHashNode != NULL_PTR)
    {
        /* ���бȽ� */
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

/* ��HASH���в���һ��Ԫ�� */
void *HASH_AddNode(void *pstHash, void *pCompared, ULONG ulPara)
{
    ULONG ulKey;
    HASH_TABLE_S *pstHashTable = NULL_PTR;
    HASH_NODE_S  **ppstHashNode  = NULL_PTR;

    pstHashTable = (HASH_TABLE_S *)pstHash;

    /* ����key */
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

/* ��HASH����ɾ��һ��Ԫ�� */
ULONG HASH_DeleteNode(void *pstNode)
{
    HASH_NODE_S  *pstHashNode   = NULL_PTR;
    HASH_NODE_S  **ppstNodeTmp  = NULL_PTR;

    pstHashNode = (HASH_NODE_S *)pstNode;
    ppstNodeTmp = pstHashNode->ppstHead;

    /* �ҵ���ɾ��Ԫ�� */
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

