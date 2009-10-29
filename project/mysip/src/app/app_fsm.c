/*******************************************************************************
功能    : 应用模块程序
创建人  : 唐春平
创建日期: 2009.07.19
修改记录:
*******************************************************************************/
/* 外部依赖模块头文件 */
#include "common\common.h"
#include "ubuf\ubuf.h"
#include "sip\uri.h"
#include "sip\sip.h"
#include "timer\timer.h"

/* 本模块对外提供的常量和结构头文件 */
#include "app\app.h"

/* 本模块内部使用的常量和结构头文件 */
#include "app_const.inc"
#include "app_type.inc"

/* 本模块内部函数声明头文件 */
#include "app.inc"
#include "app_fsm.inc"

/* 本模块全局变量声明头文件 */
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
    SIP_MSG_S     *pstSipMsg     = NULL_PTR;
    ULONG          ulRet;
    SIP_HEADER_FROM_S *pstFrom = NULL_PTR;
    SIP_HEADER_TO_S   *pstTo   = NULL_PTR;
    ULONG ulRuleIndex;
    ULONG ulStackRef1 = NULL_ULONG;
    ULONG ulStackRef2 = NULL_ULONG;

    /* 输入请求URI */
    printf("\r\nTarget URI:");
    scanf("%s",aucURI);

    /* 构造初始INVITE */
    pstSipMsgUBuf = UBUF_AllocUBuf(SIP_MAX_UBUF_MSG_LEN);
    if (pstSipMsgUBuf == NULL_PTR)
    {
        return FAIL;
    }

    pstSipMsg = UBUF_AddComponent(pstSipMsgUBuf, sizeof(SIP_MSG_S));
    memset(pstSipMsg, 0, sizeof(SIP_MSG_S));

    /* 添加方法 */
    pstSipMsg->eMsgType                          = SIP_MSG_TYPE_REQUEST;
    pstSipMsg->uStartLine.stRequstLine.eMethod   = SIP_METHOD_INVITE;
    pstSipMsg->uStartLine.stRequstLine.ucVersion = 2;

    /* 添加Request URI */
    SIP_GetRuleIndex("addr-spec", &ulRuleIndex);
    ulRet = SIP_Decode(ulRuleIndex,
                      aucURI,
                     (ULONG)strlen(aucURI),
                      pstSipMsgUBuf,
                     &pstSipMsg->uStartLine.stRequstLine.pstRequestURI);
    if (ulRet != SUCCESS)
    {
        return FAIL;
    }

    /* 添加To头域 */
    pstTo = UBUF_AddComponent(pstSipMsgUBuf, sizeof(SIP_HEADER_TO_S));
    memset(pstTo, 0, sizeof(SIP_HEADER_TO_S));
    pstSipMsg->apstHeaders[SIP_HEADER_TO] = (SIP_HEADER_S *)pstTo;
    pstTo->stHeader.pstNext = NULL_PTR;
    pstTo->stNameAddr.bName = FALSE;
    ulRet = SIP_Decode(ulRuleIndex,
                              aucURI,
                             (ULONG)strlen(aucURI),
                              pstSipMsgUBuf,
                             &pstTo->stNameAddr.pstUri);
    if (ulRet != SUCCESS)
    {
        return FAIL;
    }

    /* 添加From头域 */
    pstFrom = UBUF_AddComponent(pstSipMsgUBuf, sizeof(SIP_HEADER_FROM_S));
    memset(pstFrom, 0, sizeof(SIP_HEADER_FROM_S));
    pstSipMsg->apstHeaders[SIP_HEADER_FROM] = (SIP_HEADER_S *)pstFrom;
    pstFrom->stHeader.pstNext = NULL_PTR;
    pstFrom->stNameAddr.bName = FALSE;
    ulRet = SIP_Decode(ulRuleIndex,
                              g_pucAppPublicID,
                             (ULONG)strlen(g_pucAppPublicID),
                              pstSipMsgUBuf,
                             &pstFrom->stNameAddr.pstUri);
    if (ulRet != SUCCESS)
    {
        return FAIL;
    }

    APP_SendDownMsg(NULL_ULONG,
                    0,
                    NULL_ULONG,
                    NULL_ULONG,
                    pstSipMsgUBuf);

    /* 状态迁为APP_STATE_WAIT_REMOTE_ANSWER */
    g_eAppState = APP_STATE_WAIT_REMOTE_ANSWER;
    return SUCCESS;
}

ULONG APP_Fsm_WaitRemoteAnswer_RemoteAnswerProc()
{
    /* 发送ACK */
    /* 状态迁为APP_STATE_ACTIVE */
    return SUCCESS;
}

ULONG APP_Fsm_Active_RemoteReleaseProc()
{
    /* 发送BYE的200 */
    /* 状态迁为APP_STATE_WAIT_CLEAR */
    return SUCCESS;
}

ULONG APP_Fsm_Active_OnhokProc()
{
    /* 发送BYE */
    /* 状态迁为APP_STATE_WAIT_CLEAR */
    return SUCCESS;
}

ULONG APP_Fsm_WaitClear_OnhookProc()
{
    /* 状态迁为APP_STATE_IDLE */
    return SUCCESS;
}

ULONG APP_Fsm_Idle_IncommingCallProc()
{
    UBUF_HEADER_S *pstSipMsgUBuf = NULL_PTR;
    SIP_MSG_S     *pstSipMsg     = NULL_PTR;

    printf("\r\nAlert........................");

    /* 发送180响应 */
    pstSipMsgUBuf = UBUF_AllocUBuf(SIP_MAX_UBUF_MSG_LEN);
    if (pstSipMsgUBuf == NULL_PTR)
    {
        return FAIL;
    }

    pstSipMsg = UBUF_AddComponent(pstSipMsgUBuf, sizeof(SIP_MSG_S));
    memset(pstSipMsg, 0, sizeof(SIP_MSG_S));

    /* 添加方法 */
    pstSipMsg->eMsgType                            = SIP_MSG_TYPE_RESPONSE;
    pstSipMsg->uStartLine.stStatusLine.ucVersion   = 2;
    pstSipMsg->uStartLine.stStatusLine.eStatusCode = SIP_STATUS_CODE_180;

    APP_SendDownMsg(NULL_ULONG,
                    NULL_ULONG,
                    NULL_ULONG,
                    g_ulAppTxnID,
                    pstSipMsgUBuf);

    /* 状态迁为APP_STATE_WAIT_LOCAL_ANSWER */
    g_eAppState = APP_STATE_WAIT_LOCAL_ANSWER;
    return SUCCESS;
}

ULONG APP_Fsm_WaitLocalAnswer_OffhookProc()
{
    /* 发送200响应 */
    /* 状态迁为APP_STATE_ACTIVE */
    return SUCCESS;
}

ULONG APP_Fsm_Busy_IncommingCallProc()
{
    /* 发送486响应 */
    return SUCCESS;
}
