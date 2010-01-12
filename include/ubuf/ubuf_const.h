#define UBUF_GET_MSG_PTR(pstUbuf) ((UCHAR *)(pstUbuf) + sizeof(UBUF_HEADER_S))

#define UBUF_CLONE_STRING(pucSrcString, pstDstUbuf, pucDstString) \
{\
    ULONG ulLen = (ULONG)strlen(pucSrcString);\
    pucDstString = UBUF_AddComponent(pstDstUbuf, ulLen+1);\
    strcpy(pucDstString, pucSrcString);\
    pucDstString[ulLen] = '\0';\
}
