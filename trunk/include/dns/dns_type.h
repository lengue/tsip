typedef struct tagDNS_A_RR_S
{
    ULONG ulIpAddr;
}DNS_A_RR_S;

typedef struct tagDNS_SRV_RR_S
{
    ULONG ulIpAddr;
}DNS_SRV_RR_S;

typedef struct tagDNS_NAPTR_RR_S
{
    ULONG ulIpAddr;
}DNS_NAPTR_RR_S;

typedef struct tagDNS_RESULT_S
{
    DNS_TYPE_E eType;
    union
    {
        DNS_A_RR_S stARR;
        DNS_SRV_RR_S stSrvRR;
        DNS_SRV_RR_S stNaptrRR;
    }u;
}DNS_RESULT_S;

typedef ULONG (*DNS_FN_RESULT_NOTIFY)(UCHAR *pucDomainName,
                                      ULONG ulPara,
                                      DNS_RESULT_S *pstResult);