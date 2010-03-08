/*******************************************************************************
功能    : 应用模块内部常量定义
创建人  : 唐春平
创建日期: 2009.07.19
修改记录:
*******************************************************************************/
typedef enum tagAPP_STATE_E
{
    APP_STATE_IDLE,
    APP_STATE_WAIT_LOCAL_ANSWER,
    APP_STATE_WAIT_REMOTE_ANSWER,
    APP_STATE_ACTIVE,
    APP_STATE_WAIT_LOCAL_CLEAR,
    APP_STATE_WAIT_REMOTE_CLEAR,
    APP_STATE_BUTT
}APP_STATE_E;

typedef enum tagAPP_EVENT_E
{
    APP_EVENT_OFFHOOK,
    APP_EVENT_ONHOK,
    APP_EVENT_FLASHOOK,
    APP_EVENT_INCOMING_CALL,
    APP_EVENT_REMOTE_RING,
    APP_EVENT_REMOTE_ANSWER,
    APP_EVENT_REMOTE_RELEASE,
    APP_EVENT_BUTT
}APP_EVENT_E;