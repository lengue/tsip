/*******************************************************************************
功能    : 传输层全局变量声明
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/
/* 外部依赖模块头文件 */
#include "common\common.h"
#include "ubuf\ubuf.h"
#include "sip\uri.h"
#include "sip\sip.h"

/* 本模块对外提供的常量和结构头文件 */
#include "connect\connect.h"

/* 本模块内部使用的常量和结构头文件 */
#include "connect_const.inc"
#include "connect_type.inc"

/* 本模块内部函数声明头文件 */
/* 本模块全局变量声明头文件 */


/* 本端主机配置: 必须配置，配置为IP则绑定IP，否则不绑定IP，填写Via头域 */
//UCHAR *g_pucSipTxpHost;

/* 传输层管理，按照传输协议排列，记录该协议下连接队列 */
ULONG                 g_aulConnTransport[SIP_TRANSPORT_PROTOCOL_BUTT];

/* 监控连接队列 */
CONN_LISTEN_S     *g_pstConnListen;
CONN_FUNC_MSG_PROC g_pfnConnMsgProc = NULL_PTR;

/* 连接空闲队列以及连接控制块 */
COMM_QUEUE_S           g_stConnCBQueue;
CONN_CB_S            *g_pstConnCB;