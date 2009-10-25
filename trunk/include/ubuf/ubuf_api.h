/* 申请联合缓存 */
extern UBUF_HEADER_S* UBUF_AllocUBuf(ULONG ulLen);

/* 向一块联合缓存添加组件 */
extern VOID* UBUF_AddComponent(UBUF_HEADER_S *pstUBuf, ULONG ulComponentLen);

/* 释放联合UBUF */
extern void UBUF_FreeBuffer(UBUF_HEADER_S *pstUBuf);