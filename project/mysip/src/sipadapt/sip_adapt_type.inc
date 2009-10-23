typedef struct tagSIP_ADPT_CONN_MSG_S
{
    SIP_LOCATION_S  stLocation;
    UBUF_HEADER_S  *pstMsgUbuf;
}SIP_ADPT_CONN_MSG_S;

typedef struct tagSIP_ADPT_TIMER_MSG_S
{
    ULONG ulName;
    ULONG ulPara;
}SIP_ADPT_TIMER_MSG_S;

typedef struct tagSIP_ADAPT_CFG_S
{
    SIP_CFG_S  stSipCfg;
    CONN_CFG_S stConnCfg;
}SIP_ADAPT_CFG_S;

typedef ULONG (*FUNC_SIP_ADAPT_PARSE)(SIP_ADAPT_CFG_S *g_pstCfg,
                                      xmlDocPtr doc,
                                      xmlNodePtr pstNode);
typedef ULONG (*FUNC_SIP_ADAPT_FREE)(SIP_ADAPT_CFG_S *g_pstCfg);