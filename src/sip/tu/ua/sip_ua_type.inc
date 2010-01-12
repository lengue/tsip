typedef struct tagSIP_UAC_REQUEST_CB_S
{
    ULONG ulAppRef1;                            /* 记录的应用参考1 */
    ULONG ulAppRef2;                            /* 记录的应用参考2 */
    ULONG ulDialogID;                           /* 记录的对话ID */
    UBUF_HEADER_S *pstSipMsgUbuf;               /* UAC发送的请求消息 */
    ULONG ulLoactionID;                         /* 定位请求ID，用于释放 */
    SIP_LOCATION_RESULT_S stLocations;          /* 位置列表 */
    UCHAR ucCurrLocation;                       /* 当前使用的位置 */
    ULONG ulTxnID;                              /* 关联事务 */
}SIP_UAC_REQUEST_CB_S;

typedef struct tagSIP_UAC_TARGET_S
{
    SIP_LOCATION_S stLocation;                  /* 目标位置 */
    ULONG          ulTxnID;                     /* 关联事务 */
}SIP_UAC_TARGET_S;

typedef struct tagSIP_UAC_CB_S
{
    COMM_CB_HEADER
    ULONG ulAppDlgID;                           /* 应用层对话ID */
    ULONG ulAppTxnID;                           /* 应用层事物ID*/
    struct
    {
        ULONG ulDlgID;                          /* 生成的对话ID */
        ULONG ulTxnID;                          /* 生成的事务ID */
    }astFork[SIP_LOCATION_MAX_NUMBER];
    ULONG ulLocationID;                         /* 定位请求ID，用于释放 */
    SIP_LOCATION_RESULT_S stLocations;          /* 位置列表 */
    UBUF_HEADER_S        *pstSipMsgUbuf;        /* 待发送的请求消息 */
}SIP_UAC_CB_S;

typedef struct tagSIP_UAS_CB_S
{
    COMM_CB_HEADER
    ULONG ulAppDlgID;                         /* 应用层对话ID */
    ULONG ulAppTxnID;                         /* 应用层事物ID*/    
    ULONG ulDlgID;                            /* 记录的对话ID */
    ULONG ulTxnID;                            /* 记录的事务ID */
}SIP_UAS_CB_S;