/*******************************************************************************
功能    : 联合缓存组件功能函数
创建人  : 唐春平
创建日期: 2009.04.21
修改记录:
*******************************************************************************/
/* 外部依赖模块头文件 */
#include "..\include\common\common.h"

/* 本模块对外提供的常量和结构头文件 */
#include "..\include\ubuf\ubuf.h"

/* 本模块内部使用的常量和结构头文件 */
#include "ubuf_const.inc"
#include "ubuf_type.inc"

/* 本模块内部函数声明头文件 */
#include "ubuf.inc"

/* 本模块全局变量声明头文件 */

/* 申请联合缓存 */
UBUF_HEADER_S* UBUF_AllocUBuf(ULONG ulLen)
{
    UBUF_HEADER_S *pstUBuf = NULL_PTR;

    pstUBuf = (UBUF_HEADER_S *)malloc(sizeof(UBUF_HEADER_S) + ulLen);
    if (pstUBuf == NULL_PTR)
    {
        return NULL_PTR;
    }

    pstUBuf->ulSegmentLen = ulLen;
    pstUBuf->ulMsgLen     = 0;
    pstUBuf->ulCurrentLen = 0;
    pstUBuf->pNext        = NULL_PTR;

    return pstUBuf;
}

/* 向一块联合缓存添加组件
pstComponent 返回组件在该UBUF中的UBUF_PTR类型的地址
函数返回实际地址，方便操作
*/
VOID* UBUF_AddComponent(UBUF_HEADER_S *pstUBuf,
                        ULONG ulComponentLen,
                        UBUF_PTR *pupComponent)
{
    UBUF_HEADER_S *pstTemp = NULL_PTR;
    ULONG          ulTemp;

    if (ulComponentLen > pstUBuf->ulSegmentLen)
    {
        printf("Component Len is too large");
        return NULL_PTR;
    }

    /* 从最后一片开始添加 */
    pstTemp = pstUBuf;
    while (pstTemp->pNext != NULL_PTR)
    {
        pstTemp = pstTemp->pNext;
    }

    /* 本片够长申请新片 */
    if (ulComponentLen > (pstTemp->ulSegmentLen - pstTemp->ulCurrentLen))
    {
        /* 不够长往下走 */
        pstTemp->pNext = UBUF_AllocUBuf(pstTemp->ulSegmentLen);
        if (pstTemp->pNext == NULL_PTR)
        {
            return NULL_PTR;
        }

        pstTemp = pstTemp->pNext;
    }

    *pupComponent          = pstUBuf->ulMsgLen;
    pstUBuf->ulMsgLen     += ulComponentLen;
    ulTemp = pstTemp->ulCurrentLen;
    pstTemp->ulCurrentLen += ulComponentLen;
    return (UCHAR *)pstTemp + sizeof(UBUF_HEADER_S) + ulTemp;
}

/* 释放联合UBUF */
void UBUF_FreeBuffer(UBUF_HEADER_S *pstUBuf)
{
    if (pstUBuf == NULL_PTR)
    {
        return;
    }

    if (pstUBuf->pNext != NULL_PTR)
    {
        UBUF_FreeBuffer(pstUBuf->pNext);
    }

    free(pstUBuf);
    return;
}

/* 将UBUF中的指针转化为实际指针 */
VOID* UBUF_UBufPtr2Ptr(UBUF_HEADER_S *pstUBuf, UBUF_PTR pUBufPtr)
{
    ULONG ulTemp;
    UBUF_HEADER_S *pstTemp = NULL_PTR;

    if (pstUBuf == NULL_PTR)
    {
        return NULL_PTR;
    }

    ulTemp  = pUBufPtr;
    pstTemp = pstUBuf;
    while (ulTemp > pstTemp->ulCurrentLen)
    {
        if (pstTemp->pNext == NULL_PTR)
        {
            return NULL_PTR;
        }

        ulTemp -= pstTemp->ulCurrentLen;
        pstTemp = pstTemp->pNext;
    }

    return (UCHAR *)pstTemp + sizeof(UBUF_HEADER_S) + ulTemp;
}

/* 将UBUF克隆一份 */
UBUF_HEADER_S* UBUF_UBufClone(UBUF_HEADER_S *pstSrcUBuf)
{
    UBUF_HEADER_S *pstDstUBuf = NULL_PTR;
    UBUF_HEADER_S *pstSrcTemp = NULL_PTR;
    UBUF_HEADER_S **ppstDstTemp = NULL_PTR;

    ppstDstTemp = &pstDstUBuf;
    pstSrcTemp  =  pstSrcUBuf;
    while (pstSrcTemp != NULL_PTR)
    {
        *ppstDstTemp = UBUF_AllocUBuf(pstSrcUBuf->ulSegmentLen);
        if (*ppstDstTemp = NULL_PTR)
        {
            UBUF_FreeBuffer(pstDstUBuf);
            return NULL_PTR;
        }

        memcpy(*ppstDstTemp, pstSrcTemp, sizeof(UBUF_HEADER_S) + pstSrcUBuf->ulSegmentLen);
        ppstDstTemp = &(*ppstDstTemp)->pNext;
        pstSrcTemp  = pstSrcTemp->pNext;
    }

    return pstDstUBuf;
}
