/*******************************************************************************
功能    : 传输层结构定义
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/
/* 最大连接控制块数目 */
#define CONN_MAX_CONN 100

/* 连接的接受和发送缓存 */
#define CONN_SEND_BUFFER 10000
#define CONN_RECV_BUFFER 10000

typedef enum tagCONN_STATE_E
{
    CONN_STATE_DEACTIVE,
    CONN_STATE_ACTIVE,
    CONN_STATE_BUTT
}CONN_STATE_E;