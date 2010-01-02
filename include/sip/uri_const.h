/*******************************************************************************
����    : URIͨ�ó�������
������  : �ƴ�ƽ
��������: 200.01.10
�޸ļ�¼:
*******************************************************************************/
#define URI_MAX_LEN 1000

typedef enum tagURI_TYPE_E
{
    URI_TYPE_SIP,
    URI_TYPE_SIPS,
    URI_TYPE_TEL,
    URI_TYPE_BUTT
}URI_TYPE_E;

typedef enum tagURI_PARAMETER_E
{
    URI_PARAMETER_TRANSPORT,
    URI_PARAMETER_USER,
    URI_PARAMETER_METHOD,
    URI_PARAMETER_TTL,
    URI_PARAMETER_MADDR,
    URI_PARAMETER_LR,
    URI_PARAMETER_BUTT
}URI_PARAMETER_E;

typedef enum tagURI_HOST_E
{
    URI_HOST_DOMAIN,
    URI_HOST_IPV4,
    URI_HOST_IPV6,
    URI_HOST_BUTT
}URI_HOST_E;