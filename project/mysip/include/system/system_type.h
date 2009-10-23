/* 模块初始化函数 */
typedef ULONG SYS_MODULE_INIT_PROC();

/* 模块消息处理函数 */
typedef void SYS_MODULE_MSG_PROC(ULONG ulSrcMID, void *pstMsg);

typedef struct tagSYS_MODULE_INFO_S
{
    SYS_MODULE_INIT_PROC *pfnInitProc;
    SYS_MODULE_MSG_PROC  *pfnMsgProc;
}SYS_MODULE_INFO_S;

typedef struct tagSYS_MSG_S
{
    ULONG  ulSrcModuleID;   /* 源模块 */
    ULONG  ulDstModuleID;   /* 源模块 */
    ULONG  ulLength;        /* 消息长度 */
    void  *pMsg;            /* 消息指针 */
    struct tagSYS_MSG_S  *pstNext;
}SYS_MSG_S;

typedef struct tagSYS_MODULE_CB_S
{
    HANDLE     hThread;    /* 消息处理线程ID */
    HANDLE     hSemaphore; /* 消息队列资源信标 */
    HANDLE     hMutex;     /* 消息队列操作互斥 */
    SYS_MSG_S *pstMsgQueue;
    SYS_MODULE_MSG_PROC  *pfnMsgProc;
}SYS_MODULE_CB_S;

