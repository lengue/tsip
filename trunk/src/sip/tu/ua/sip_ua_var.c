/*******************************************************************************
功能    : 传输层消息管理(WIN32专用)
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/
/* 外部依赖模块头文件 */
#include "common\common.h"
#include "ubuf\ubuf.h"
#include "sip\uri.h"
#include "sip\sip.h"

#include "..\include\locate\sip_locate.h"

/* 本模块对外提供的常量和结构头文件 */
#include "sip_ua_const.inc"
#include "sip_ua_type.inc"

/* 本模块内部使用的常量和结构头文件 */


/* 本模块内部函数声明头文件 */
#include "sip_uac.inc"
#include "sip_uas.inc"

/* 本模块全局变量声明头文件 */
SIP_UAC_CB_S   *g_pstSipUacCB = NULL_PTR;
COMM_QUEUE_S   g_stSipUacCBQueue;
SIP_UAS_CB_S   *g_pstSipUasCB = NULL_PTR;
COMM_QUEUE_S   g_stSipUasCBQueue;