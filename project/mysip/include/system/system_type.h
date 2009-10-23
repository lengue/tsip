/* ģ���ʼ������ */
typedef ULONG SYS_MODULE_INIT_PROC();

/* ģ����Ϣ������ */
typedef void SYS_MODULE_MSG_PROC(ULONG ulSrcMID, void *pstMsg);

typedef struct tagSYS_MODULE_INFO_S
{
    SYS_MODULE_INIT_PROC *pfnInitProc;
    SYS_MODULE_MSG_PROC  *pfnMsgProc;
}SYS_MODULE_INFO_S;

typedef struct tagSYS_MSG_S
{
    ULONG  ulSrcModuleID;   /* Դģ�� */
    ULONG  ulDstModuleID;   /* Դģ�� */
    ULONG  ulLength;        /* ��Ϣ���� */
    void  *pMsg;            /* ��Ϣָ�� */
    struct tagSYS_MSG_S  *pstNext;
}SYS_MSG_S;

typedef struct tagSYS_MODULE_CB_S
{
    HANDLE     hThread;    /* ��Ϣ�����߳�ID */
    HANDLE     hSemaphore; /* ��Ϣ������Դ�ű� */
    HANDLE     hMutex;     /* ��Ϣ���в������� */
    SYS_MSG_S *pstMsgQueue;
    SYS_MODULE_MSG_PROC  *pfnMsgProc;
}SYS_MODULE_CB_S;

