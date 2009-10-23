/* 域名解析控制块 */
typedef struct tagDNS_CB_S
{
    UCHAR      ucIsUsed;                               /* 控制块是否在使用 */
    UCHAR      aucDomainName[DNS_MAX_DOMAIN_NAME_LEN]; /* 待解析域名 */
    DNS_TYPE_E eType;                                  /* 域名类型   */
    ULONG      ulReqList;                              /* 请求列表   */
    ULONG      ulNext;                                 /* 下一控制块索引 */
}DNS_CB_S;


/* 域名请求记录结构，便于异步操作 */
typedef struct tagDNS_REQ_S
{
    UCHAR                  ucIsUsed;   /* 控制块是否在使用 */
    DNS_FN_RESULT_NOTIFY   pfnNotify;  /* 解析结果通知函数 */
    ULONG                  ulPara;     /* 解析请求参数 */
    ULONG                  ulNext;     /* 下一控制块索引 */
}DNS_REQ_S;
