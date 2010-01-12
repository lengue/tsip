/*******************************************************************************
功能    : 传输层消息管理(WIN32专用)
创建人  : 唐春平
创建日期: 200.01.10
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
#include "..\include\dialog\sip_dlg.h"

/* 本模块内部使用的常量和结构头文件 */
#include "sip_dlg_const.inc"
#include "sip_dlg_type.inc"

/* 本模块内部函数声明头文件 */

/* 本模块全局变量声明头文件 */
#include "sip_dlg_var.inc"

SIP_DLG_CB_S *g_pstSipDlgCB   = NULL_PTR;
COMM_QUEUE_S  g_stSipDlgCBQueue;
void         *g_pstSipDlgHash = NULL_PTR;

