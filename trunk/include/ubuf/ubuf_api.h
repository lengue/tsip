/* �������ϻ��� */
extern UBUF_HEADER_S* UBUF_AllocUBuf(ULONG ulLen);
/* ��һ�����ϻ��������� */
extern VOID* UBUF_AddComponent(UBUF_HEADER_S *pstUBuf,
                        ULONG ulComponentLen,
                        UBUF_PTR *pupComponent);
/* �ͷ�����UBUF */
extern void UBUF_FreeBuffer(UBUF_HEADER_S *pstUBuf);

/* ��UBUF�е�ָ��ת��Ϊʵ��ָ�� */
extern VOID* UBUF_UBufPtr2Ptr(UBUF_HEADER_S *pstUBuf, UBUF_PTR pUBufPtr);

/* ��UBUF��¡һ�� */
extern UBUF_HEADER_S* UBUF_UBufClone(UBUF_HEADER_S *pstSrcUBuf);