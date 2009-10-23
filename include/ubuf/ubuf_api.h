/* 申请联合缓存 */
extern UBUF_HEADER_S* UBUF_AllocUBuf(ULONG ulLen);
/* 向一块联合缓存添加组件 */
extern VOID* UBUF_AddComponent(UBUF_HEADER_S *pstUBuf,
                        ULONG ulComponentLen,
                        UBUF_PTR *pupComponent);
/* 释放联合UBUF */
extern void UBUF_FreeBuffer(UBUF_HEADER_S *pstUBuf);

/* 将UBUF中的指针转化为实际指针 */
extern VOID* UBUF_UBufPtr2Ptr(UBUF_HEADER_S *pstUBuf, UBUF_PTR pUBufPtr);

/* 将UBUF克隆一份 */
extern UBUF_HEADER_S* UBUF_UBufClone(UBUF_HEADER_S *pstSrcUBuf);