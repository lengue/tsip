/* �������ϻ��� */
extern UBUF_HEADER_S* UBUF_AllocUBuf(ULONG ulLen);

/* ��һ�����ϻ��������� */
extern VOID* UBUF_AddComponent(UBUF_HEADER_S *pstUBuf, ULONG ulComponentLen);

/* �ͷ�����UBUF */
extern void UBUF_FreeBuffer(UBUF_HEADER_S *pstUBuf);