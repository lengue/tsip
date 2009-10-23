/*******************************************************************************
����    : ���ϻ���������ܺ���
������  : �ƴ�ƽ
��������: 2009.04.21
�޸ļ�¼:
*******************************************************************************/
/* �ⲿ����ģ��ͷ�ļ� */
#include "..\include\common\common.h"

/* ��ģ������ṩ�ĳ����ͽṹͷ�ļ� */
#include "..\include\ubuf\ubuf.h"

/* ��ģ���ڲ�ʹ�õĳ����ͽṹͷ�ļ� */
#include "ubuf_const.inc"
#include "ubuf_type.inc"

/* ��ģ���ڲ���������ͷ�ļ� */
#include "ubuf.inc"

/* ��ģ��ȫ�ֱ�������ͷ�ļ� */

/* �������ϻ��� */
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

/* ��һ�����ϻ���������
pstComponent ��������ڸ�UBUF�е�UBUF_PTR���͵ĵ�ַ
��������ʵ�ʵ�ַ���������
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

    /* �����һƬ��ʼ��� */
    pstTemp = pstUBuf;
    while (pstTemp->pNext != NULL_PTR)
    {
        pstTemp = pstTemp->pNext;
    }

    /* ��Ƭ����������Ƭ */
    if (ulComponentLen > (pstTemp->ulSegmentLen - pstTemp->ulCurrentLen))
    {
        /* ������������ */
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

/* �ͷ�����UBUF */
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

/* ��UBUF�е�ָ��ת��Ϊʵ��ָ�� */
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

/* ��UBUF��¡һ�� */
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
