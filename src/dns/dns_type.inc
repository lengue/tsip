/* �����������ƿ� */
typedef struct tagDNS_CB_S
{
    UCHAR      ucIsUsed;                               /* ���ƿ��Ƿ���ʹ�� */
    UCHAR      aucDomainName[DNS_MAX_DOMAIN_NAME_LEN]; /* ���������� */
    DNS_TYPE_E eType;                                  /* ��������   */
    ULONG      ulReqList;                              /* �����б�   */
    ULONG      ulNext;                                 /* ��һ���ƿ����� */
}DNS_CB_S;


/* ���������¼�ṹ�������첽���� */
typedef struct tagDNS_REQ_S
{
    UCHAR                  ucIsUsed;   /* ���ƿ��Ƿ���ʹ�� */
    DNS_FN_RESULT_NOTIFY   pfnNotify;  /* �������֪ͨ���� */
    ULONG                  ulPara;     /* ����������� */
    ULONG                  ulNext;     /* ��һ���ƿ����� */
}DNS_REQ_S;