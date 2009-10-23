/*******************************************************************************
����    : Ӧ��ģ�����
������  : �ƴ�ƽ
��������: 2009.07.19
�޸ļ�¼:
*******************************************************************************/
/* �ⲿ����ģ��ͷ�ļ� */
#include "common\common.h"
#include "ubuf\ubuf.h"
#include "sip\uri.h"
#include "sip\sip.h"

/* ��ģ������ṩ�ĳ����ͽṹͷ�ļ� */
#include "app\app.h"

/* ��ģ���ڲ�ʹ�õĳ����ͽṹͷ�ļ� */
#include "app_const.inc"
#include "app_type.inc"

/* ��ģ���ڲ���������ͷ�ļ� */
#include "app_fsm.inc"

/* ��ģ��ȫ�ֱ�������ͷ�ļ� */
#include "app_var.inc"

ULONG APP_FsmInitProc()
{
    g_eAppState = APP_STATE_IDLE;
    memset(g_apfnAppFsm, 0, sizeof(APP_FN_FSM_PROC)*APP_STATE_BUTT*APP_EVENT_BUTT);
    g_apfnAppFsm[APP_STATE_IDLE][APP_EVENT_OFFHOOK] = APP_Fsm_Idle_OffhookProc;
    g_apfnAppFsm[APP_STATE_IDLE][APP_EVENT_INCOMING_CALL] = APP_Fsm_Idle_IncommingCallProc;
    g_apfnAppFsm[APP_STATE_WAIT_LOCAL_ANSWER][APP_EVENT_OFFHOOK] = APP_Fsm_WaitLocalAnswer_OffhookProc;
    g_apfnAppFsm[APP_STATE_WAIT_LOCAL_ANSWER][APP_EVENT_INCOMING_CALL] = APP_Fsm_Busy_IncommingCallProc;
    g_apfnAppFsm[APP_STATE_WAIT_REMOTE_ANSWER][APP_EVENT_REMOTE_ANSWER] = APP_Fsm_WaitRemoteAnswer_RemoteAnswerProc;
    g_apfnAppFsm[APP_STATE_WAIT_REMOTE_ANSWER][APP_EVENT_INCOMING_CALL] = APP_Fsm_Busy_IncommingCallProc;
    g_apfnAppFsm[APP_STATE_ACTIVE][APP_EVENT_ONHOK]          = APP_Fsm_Active_OnhokProc;
    g_apfnAppFsm[APP_STATE_ACTIVE][APP_EVENT_REMOTE_RELEASE] = APP_Fsm_Active_RemoteReleaseProc;
    g_apfnAppFsm[APP_STATE_ACTIVE][APP_EVENT_INCOMING_CALL] = APP_Fsm_Busy_IncommingCallProc;
    g_apfnAppFsm[APP_STATE_WAIT_CLEAR][APP_EVENT_ONHOK] = APP_Fsm_WaitClear_OnhookProc;
    g_apfnAppFsm[APP_STATE_WAIT_CLEAR][APP_EVENT_INCOMING_CALL] = APP_Fsm_Busy_IncommingCallProc;

    return SUCCESS;
}

ULONG APP_FsmProc(APP_EVENT_E eEvent)
{
    if (g_apfnAppFsm[g_eAppState][eEvent] == NULL_PTR)
    {
        return SUCCESS;
    }

    return g_apfnAppFsm[g_eAppState][eEvent]();
}

ULONG APP_Fsm_Idle_OffhookProc()
{
    UCHAR aucURI[200];
    UBUF_HEADER_S *pstSipMsgUBuf = NULL_PTR;
    UBUF_PTR       upComponent   = UBUF_NULL_PTR;
    SIP_MSG_S     *pstSipMsg     = NULL_PTR;
    ULONG          ulRet;
    SIP_HEADER_S  *pstHeader = NULL_PTR;
    SIP_HEADER_FROM_S *pstFrom = NULL_PTR;
    SIP_HEADER_TO_S   *pstTo   = NULL_PTR;

    /* ��������URI */
    printf("\r\nTarget URI:");
    scanf("%s",aucURI);

    /* �����ʼINVITE */
    pstSipMsgUBuf = UBUF_AllocUBuf(SIP_MAX_UBUF_MSG_LEN);
    if (pstSipMsgUBuf == NULL_PTR)
    {
        return FAIL;
    }

    pstSipMsg = UBUF_AddComponent(pstSipMsgUBuf, sizeof(SIP_MSG_S), &upComponent);
    memset(pstSipMsg, 0xff, sizeof(SIP_MSG_S));

    /* ��ӷ��� */
    pstSipMsg->eMsgType                          = SIP_MSG_TYPE_REQUEST;
    pstSipMsg->uStartLine.stRequstLine.eMethod   = SIP_METHOD_INVITE;
    pstSipMsg->uStartLine.stRequstLine.ucVersion = 2;

    /* ���Request URI */
    ulRet = SIP_Syntax_UriDecode(aucURI,
                                 (ULONG)strlen(aucURI),
                                 pstSipMsgUBuf,
                                &pstSipMsg->uStartLine.stRequstLine.upRequestURI);
    if (ulRet != SUCCESS)
    {
        return FAIL;
    }

    /* ���Toͷ�� */
    pstHeader = UBUF_AddComponent(pstSipMsgUBuf,
                                  sizeof(SIP_HEADER_S)+sizeof(SIP_HEADER_TO_S),
                                 &pstSipMsg->aupstHeaders[SIP_HEADER_TO]);
    pstHeader->upstNext = UBUF_NULL_PTR;

    pstTo =  (SIP_HEADER_TO_S *)pstHeader->pstSpec;
    memset(pstTo, 0xff, sizeof(SIP_HEADER_TO_S));

    pstTo->stNameAddr.bName = FALSE;
    ulRet = SIP_Syntax_UriDecode(aucURI,
                                 (ULONG)strlen(aucURI),
                                 pstSipMsgUBuf,
                                &pstTo->stNameAddr.upstUri);
    if (ulRet != SUCCESS)
    {
        return FAIL;
    }

    /* ���Fromͷ�� */
    pstHeader = UBUF_AddComponent(pstSipMsgUBuf,
                                  sizeof(SIP_HEADER_S)+sizeof(SIP_HEADER_FROM_S),
                                 &pstSipMsg->aupstHeaders[SIP_HEADER_FROM]);
    pstHeader->upstNext = UBUF_NULL_PTR;

    pstFrom =  (SIP_HEADER_FROM_S *)pstHeader->pstSpec;
    memset(pstFrom, 0xff, sizeof(SIP_HEADER_FROM_S));

    pstFrom->stNameAddr.bName = FALSE;
    ulRet = SIP_Syntax_UriDecode(g_pucAppPublicID,
                                 (ULONG)strlen(g_pucAppPublicID),
                                 pstSipMsgUBuf,
                                &pstFrom->stNameAddr.upstUri);
    if (ulRet != SUCCESS)
    {
        return FAIL;
    }

    SIP_UAC_SendRequest(0, NULL_ULONG, pstSipMsgUBuf);

    /* ״̬ǨΪAPP_STATE_WAIT_REMOTE_ANSWER */
    g_eAppState = APP_STATE_WAIT_REMOTE_ANSWER;
    return SUCCESS;
}

ULONG APP_Fsm_WaitRemoteAnswer_RemoteAnswerProc()
{
    /* ����ACK */
    /* ״̬ǨΪAPP_STATE_ACTIVE */
    return SUCCESS;
}

ULONG APP_Fsm_Active_RemoteReleaseProc()
{
    /* ����BYE��200 */
    /* ״̬ǨΪAPP_STATE_WAIT_CLEAR */
    return SUCCESS;
}

ULONG APP_Fsm_Active_OnhokProc()
{
    /* ����BYE */
    /* ״̬ǨΪAPP_STATE_WAIT_CLEAR */
    return SUCCESS;
}

ULONG APP_Fsm_WaitClear_OnhookProc()
{
    /* ״̬ǨΪAPP_STATE_IDLE */
    return SUCCESS;
}

ULONG APP_Fsm_Idle_IncommingCallProc()
{
    UBUF_HEADER_S *pstSipMsgUBuf = NULL_PTR;
    UBUF_PTR       upComponent   = UBUF_NULL_PTR;
    SIP_MSG_S     *pstSipMsg     = NULL_PTR;
    ULONG          ulRet;
    ULONG          ulDlgID;

    printf("\r\nAlert........................");

    /* ����180��Ӧ */
    pstSipMsgUBuf = UBUF_AllocUBuf(SIP_MAX_UBUF_MSG_LEN);
    if (pstSipMsgUBuf == NULL_PTR)
    {
        return FAIL;
    }

    pstSipMsg = UBUF_AddComponent(pstSipMsgUBuf, sizeof(SIP_MSG_S), &upComponent);
    memset(pstSipMsg, 0xff, sizeof(SIP_MSG_S));

    /* ��ӷ��� */
    pstSipMsg->eMsgType                            = SIP_MSG_TYPE_RESPONSE;
    pstSipMsg->uStartLine.stStatusLine.ucVersion   = 2;
    pstSipMsg->uStartLine.stStatusLine.eStatusCode = SIP_STATUS_CODE_180;

    SIP_UAS_SendResponse(g_ulAppTxnID,
                        &ulDlgID,
                         pstSipMsgUBuf);

    /* ״̬ǨΪAPP_STATE_WAIT_LOCAL_ANSWER */
    g_eAppState = APP_STATE_WAIT_LOCAL_ANSWER;
    return SUCCESS;
}

ULONG APP_Fsm_WaitLocalAnswer_OffhookProc()
{
    /* ����200��Ӧ */
    /* ״̬ǨΪAPP_STATE_ACTIVE */
    return SUCCESS;
}

ULONG APP_Fsm_Busy_IncommingCallProc()
{
    /* ����486��Ӧ */
    return SUCCESS;
}
