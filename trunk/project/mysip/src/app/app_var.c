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

/* 本模块对外提供的常量和结构头文件 */
#include "app\app.h"

/* 本模块内部使用的常量和结构头文件 */
#include "app_const.inc"
#include "app_type.inc"

/* 本模块内部函数声明头文件 */

/* 本模块全局变量声明头文件 */
#include "app_var.inc"

APP_FN_FSM_PROC g_apfnAppFsm[APP_STATE_BUTT][APP_EVENT_BUTT];
APP_STATE_E g_eAppState;
UCHAR g_ucRemoteAlertFlag = FALSE;
UCHAR *g_pucAppPublicID = "sip:tangchunping@huawei.com";
ULONG g_ulStackDlgID;
ULONG g_ulStackTxnID;