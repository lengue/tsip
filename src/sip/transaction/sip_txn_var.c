/*******************************************************************************
功能    : SIP协议栈传输层功能函数
创建人  : 唐春平
创建日期: 2009.01.10
修改记录:
*******************************************************************************/
/* 外部依赖模块头文件 */
#include "common\common.h"
#include "abnf\abnf.h"
#include "ubuf\ubuf.h"
#include "sip\uri.h"
#include "sip\sip.h"
#include "s_hash\s_hash.h"

/* 本模块对外提供的常量和结构头文件 */
#include "..\include\transaction\sip_transaction.h"

/* 本模块内部使用的常量和结构头文件 */
#include "sip_txn_const.inc"
#include "sip_txn_type.inc"

/* 本模块内部函数声明头文件 */

/* 本模块全局变量声明头文件 */
#include "sip_txn_var.inc"

/* 连接空闲队列以及连接控制块 */
/* 所有的事务控制块 */
SIP_TXN_CB_S  *g_pstSipTxnCB = NULL_PTR;
COMM_QUEUE_S   g_stSipTxnCBQueue;
void          *g_pstSipTxnCBHash     = NULL_PTR;

SIP_TXN_FSM_S g_astSipTxnFsm[SIP_TXN_TYPE_BUTT][SIP_TXN_STATE_TERMINATED][SIP_TXN_EVENT_BUTT];

ULONG g_ulSipTxnT1 = 500;