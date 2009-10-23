/*******************************************************************************
����    : URIͨ�ýṹ����
������  : �ƴ�ƽ
��������: 200.01.10
�޸ļ�¼:
*******************************************************************************/

typedef struct tagURI_PARAMETER_S
{
    URI_PARAMETER_E eParaName;
    UCHAR           ucParaValue;
    UBUF_PTR        upstNext;     /* URI_PARAMETER_S */
}URI_PARAMETER_S;

typedef struct tagURI_USER_INFO_S
{
    UBUF_PTR upucUserInfo;/*UCHAR*/
    UBUF_PTR upucPassword;/*UCHAR*/
}URI_USER_INFO_S;

typedef struct tagURI_HOST_S
{
    URI_HOST_E eHostType;

    /* ��ַ�ַ���������UCHAR */
    UBUF_PTR   upucAddrStr;
}URI_HOST_S;

typedef struct tagURI_HOST_PORT_S
{
    URI_HOST_S stHost;
    USHORT     usPort;
}URI_HOST_PORT_S;

/*SIPS��SIP URI*/
typedef struct tagSIP_URI_S
{
    UBUF_PTR         upstUserInfo;/*URI_USER_INFO_S*/
    URI_HOST_PORT_S  stHostPort;
    UBUF_PTR         upstParameters;/*URI_PARAMETER_S*/
    UCHAR            ucHeaders;
}SIP_URI_S;

typedef struct tagTEL_URI_S
{
    UBUF_PTR         upucTelNo;/*UCHAR*/
}TEL_URI_S;

typedef struct tagURI_S
{
    URI_TYPE_E eUriType;
    union
    {
        SIP_URI_S stSipUri;
        TEL_URI_S stTelUri;
    }u;
}URI_S;
