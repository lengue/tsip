typedef struct tagAPP_MSG_S
{
    ULONG ulAppRef1;
    ULONG ulAppRef2;
    ULONG ulStackRef1;
    ULONG ulStackRef2;
    UBUF_HEADER_S *pstUbufSipMsg;
}APP_MSG_S;