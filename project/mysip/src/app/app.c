/*******************************************************************************
功能    : 应用模块程序
创建人  : 唐春平
创建日期: 2009.07.19
修改记录:
*******************************************************************************/
/* 外部依赖模块头文件 */
#include "common\common.h"
#include "system\system.h"
#include "timer\timer.h"
#include "ubuf\ubuf.h"
#include "sip\uri.h"
#include "sip\sip.h"

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

/* 启动用户界面 */
ULONG APP_StartUserInterface()
{
    ULONG  ulOperation;
    APP_EVENT_E eEvent;

    #define APP_OPERATION_OFFHOOK 0
    #define APP_OPERATION_ONHOOK  1

    while(1)
    {
        printf("\r\nUser Operation(0:Offhok 1:Onhook):");
        scanf("%d",&ulOperation);
        switch(ulOperation)
        {
            case APP_OPERATION_OFFHOOK:
                eEvent = APP_EVENT_OFFHOOK;
                APP_FsmProc(eEvent);
                break;

            case APP_OPERATION_ONHOOK:
                eEvent = APP_EVENT_ONHOK;
                APP_FsmProc(eEvent);
                break;

            default:
                break;
        }
    }

    return SUCCESS;
}

ULONG APP_InitProc()
{
    APP_FsmInitProc();
    return SUCCESS;
}

ULONG APP_MsgProc(ULONG ulModuleID, void* pMsg)
{
    ULONG ulRet = SUCCESS;
    APP_MSG_S  *pstAppMsg = NULL_PTR;

    switch(ulModuleID)
    {
        case SYS_MODULE_TIMER:
            ulRet = APP_TimerProc(pMsg);
            break;

        case SYS_MODULE_SIP:
            pstAppMsg = (APP_MSG_S *)pMsg;
            ulRet = APP_RecvUpMsg(pstAppMsg->ulStackRef1,
                                  pstAppMsg->ulStackRef2,
                                  pstAppMsg->ulAppRef1,
                                  pstAppMsg->ulAppRef2,
                                  pstAppMsg->pstUbufSipMsg);
            break;

        default:
            break;
    }

    return ulRet;
}

ULONG APP_RecvUpMsg(ULONG ulStackRef1,
                    ULONG ulStackRef2,
                    ULONG ulAppRef1,
                    ULONG ulAppRef2,
                    UBUF_HEADER_S * pstUbufSipMsg)
{
    SIP_MSG_S *pstSipMsg = NULL_PTR;

    pstSipMsg = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUbufSipMsg);
    switch (pstSipMsg->eMsgType)
    {
        case SIP_MSG_TYPE_REQUEST:
            APP_SipRequestProc(ulStackRef1, ulStackRef2, pstUbufSipMsg);
            g_ulStackTxnID = ulStackRef2;
            break;

        case SIP_MSG_TYPE_RESPONSE:
            APP_SipResponseProc(ulStackRef1, ulStackRef2, pstUbufSipMsg);
            g_ulStackDlgID = ulStackRef1;
            break;

        default:
            return FAIL;
    }

    /* 网络侧消息的终点，释放消息 */
    UBUF_FreeBuffer(pstUbufSipMsg);
    return SUCCESS;
}

ULONG APP_SipRequestProc(ULONG ulDlgID,
                         ULONG ulTxnID,
                         UBUF_HEADER_S *pstUbufSipMsg)
{
    SIP_MSG_S *pstSipMsg = NULL_PTR;
    APP_EVENT_E eEvent;

    pstSipMsg = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUbufSipMsg);
    switch(pstSipMsg->uStartLine.stRequstLine.eMethod)
    {
        case SIP_METHOD_INVITE:
            eEvent = APP_EVENT_INCOMING_CALL;
            break;

        case SIP_METHOD_BYE:
            eEvent = APP_EVENT_REMOTE_RELEASE;
            break;

        default:
            return FAIL;
    }

    APP_FsmProc(eEvent);
    return SUCCESS;
}

ULONG APP_SipResponseProc(ULONG ulDlgID,
                          ULONG ulReqID,
                          UBUF_HEADER_S *pstUbufSipMsg)
{
    SIP_MSG_S *pstSipMsg = NULL_PTR;
    APP_EVENT_E eEvent;

    pstSipMsg = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUbufSipMsg);
    switch (pstSipMsg->uStartLine.stStatusLine.eStatusCode)
    {
        case SIP_STATUS_CODE_180:
            eEvent = APP_EVENT_REMOTE_RING;
            break;

        case SIP_STATUS_CODE_200:
            eEvent = APP_EVENT_REMOTE_ANSWER;
            break;

        default:
            return FAIL;
    }

    APP_FsmProc(eEvent);
    return SUCCESS;
}

/* 应用层定时器超时 */
ULONG APP_TimerProc(TIMER_MSG_S *pstTimerMsg)
{
    printf("\r\n timer out,name=%d,para=%d",
            pstTimerMsg->ulName,
            pstTimerMsg->ulPara);
    return SUCCESS;
}

ULONG APP_SendDownMsg(ULONG ulAppRef1,
                      ULONG ulAppRef2,
                      ULONG ulStackRef1,
                      ULONG ulStackRef2,
                      UBUF_HEADER_S *pstUbufSipMsg)
{
    APP_MSG_S stAppMsg;

    stAppMsg.ulAppRef1 = ulAppRef1;
    stAppMsg.ulAppRef2 = ulAppRef2;
    stAppMsg.ulStackRef1 = ulStackRef1;
    stAppMsg.ulStackRef2 = ulStackRef2;
    stAppMsg.pstUbufSipMsg = pstUbufSipMsg;

    return SYS_SendMsg(SYS_MODULE_APP,
                       SYS_MODULE_SIP,
                      &stAppMsg,
                       sizeof(APP_MSG_S));
}

/*参考号映射*/
ULONG APP_MapRef1(ULONG ulAppDlgID, ULONG ulStackRef1)
{
    g_ulStackDlgID = ulStackRef1;
    return SUCCESS;
}

/*参考号映射*/
ULONG APP_MapRef2(ULONG ulAppTxnID, ULONG ulStackRef2)
{
    g_ulStackTxnID = ulStackRef2;
    return SUCCESS;
}