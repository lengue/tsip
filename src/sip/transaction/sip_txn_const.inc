/*******************************************************************************
功能    : SIP协议栈传输层功能函数
创建人  : 唐春平
创建日期: 2009.01.10
修改记录:
*******************************************************************************/
#define SIP_TXN_MAX_FSM_ACTION 5
#define SIP_TXN_MAX_FSM_TIMER  2
#define SIP_TXN_CB_NUM 100

/* 事务事件 */
typedef enum tagSIP_TXN_EVENT_E
{
    SIP_TXN_EVENT_SEND_REQUEST,             /* TU发送请求 */
    SIP_TXN_EVENT_SEND_1XX_RESPONSE,        /* TU发送1XX响应 */
    SIP_TXN_EVENT_SEND_2XX_RESPONSE,        /* TU发送2XX响应 */
    SIP_TXN_EVENT_SEND_3456XX_RESPONSE,     /* TU发送3XX-6XX响应 */
    SIP_TXN_EVENT_RECV_REQUEST,             /* 收到网络侧非ACK请求 */
    SIP_TXN_EVENT_RECV_ACK,                 /* 收到网络侧ACK请求 */
    SIP_TXN_EVENT_RECV_1XX_RESPONSE,        /* 收到网络侧1XX响应 */
    SIP_TXN_EVENT_RECV_2XX_RESPONSE,        /* 收到网络侧2XX响应 */
    SIP_TXN_EVENT_RECV_3456XX_RESPONSE,     /* 收到网络侧3XX-6XX响应 */
    SIP_TXN_EVENT_TIMER_A_FIRE,              /* 定时器A超时 */
    SIP_TXN_EVENT_TIMER_B_FIRE,              /* 定时器B超时 */
    SIP_TXN_EVENT_TIMER_D_FIRE,              /* 定时器D超时 */
    SIP_TXN_EVENT_TIMER_E_FIRE,              /* 定时器E超时 */
    SIP_TXN_EVENT_TIMER_F_FIRE,              /* 定时器F超时 */
    SIP_TXN_EVENT_TIMER_K_FIRE,              /* 定时器K超时 */
    SIP_TXN_EVENT_TIMER_G_FIRE,              /* 定时器G超时 */
    SIP_TXN_EVENT_TIMER_H_FIRE,              /* 定时器H超时 */
    SIP_TXN_EVENT_TIMER_I_FIRE,              /* 定时器I超时 */
    SIP_TXN_EVENT_TIMER_J_FIRE,              /* 定时器J超时 */
    SIP_TXN_EVENT_TXP_ERR,                  /* 传输层错误 */

    SIP_TXN_EVENT_BUTT
}SIP_TXN_EVENT_E;

/* 事务状态，几种类型事务的状态复用 */
typedef enum tagSIP_TXN_STATE_E
{
    /* 初始状态 */
    SIP_TXN_STATE_INIT,

    SIP_TXN_STATE_TRYING,
    SIP_TXN_STATE_CALLING,
    SIP_TXN_STATE_PROCEEDING,
    SIP_TXN_STATE_COMPLETED,
    SIP_TXN_STATE_CONFIRMED,
    SIP_TXN_STATE_TERMINATED
}SIP_TXN_STATE_E;

/* 事务类型 */
typedef enum tagSIP_TXN_TYPE_E
{
    SIP_TXN_TYPE_INVITE_CLIENT,     /* INVTE客户端事务 */
    SIP_TXN_TYPE_NON_INVITE_CLIENT, /* 非INVTE客户端事务 */
    SIP_TXN_TYPE_INVITE_SERVER,     /* INVTE服务器端事务 */
    SIP_TXN_TYPE_NON_INVITE_SERVER, /* 非INVTE服务器端事务 */

    SIP_TXN_TYPE_BUTT
}SIP_TXN_TYPE_E;