typedef struct tagSIP_UAC_REQUEST_CB_S
{
    ULONG ulAppRef1;                            /* ��¼��Ӧ�òο�1 */
    ULONG ulAppRef2;                            /* ��¼��Ӧ�òο�2 */
    ULONG ulDialogID;                           /* ��¼�ĶԻ�ID */
    UBUF_HEADER_S *pstSipMsgUbuf;               /* UAC���͵�������Ϣ */
    ULONG ulLoactionID;                         /* ��λ����ID�������ͷ� */
    SIP_LOCATION_RESULT_S stLocations;          /* λ���б� */
    UCHAR ucCurrLocation;                       /* ��ǰʹ�õ�λ�� */
    ULONG ulTxnID;                              /* �������� */
}SIP_UAC_REQUEST_CB_S;

typedef struct tagSIP_UAC_TARGET_S
{
    SIP_LOCATION_S stLocation;                  /* Ŀ��λ�� */
    ULONG          ulTxnID;                     /* �������� */
}SIP_UAC_TARGET_S;

typedef struct tagSIP_UAC_CB_S
{
    COMM_CB_HEADER
    ULONG ulAppDlgID;                           /* Ӧ�ò�Ի�ID */
    ULONG ulAppTxnID;                           /* Ӧ�ò�����ID*/
    struct
    {
        ULONG ulDlgID;                          /* ���ɵĶԻ�ID */
        ULONG ulTxnID;                          /* ���ɵ�����ID */
    }astFork[SIP_LOCATION_MAX_NUMBER];
    ULONG ulLocationID;                         /* ��λ����ID�������ͷ� */
    SIP_LOCATION_RESULT_S stLocations;          /* λ���б� */
    UBUF_HEADER_S        *pstSipMsgUbuf;        /* �����͵�������Ϣ */
}SIP_UAC_CB_S;

typedef struct tagSIP_UAS_CB_S
{
    COMM_CB_HEADER
    ULONG ulAppDlgID;                         /* Ӧ�ò�Ի�ID */
    ULONG ulAppTxnID;                         /* Ӧ�ò�����ID*/    
    ULONG ulDlgID;                            /* ��¼�ĶԻ�ID */
    ULONG ulTxnID;                            /* ��¼������ID */
}SIP_UAS_CB_S;