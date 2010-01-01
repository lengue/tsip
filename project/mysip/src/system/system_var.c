/**********************************************************************
功能    : COMMON通用函数实现
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
**********************************************************************/
/* 外部依赖模块头文件 */
#include "common\common.h"
#include "ubuf\ubuf.h"
#include "sip\uri.h"
#include "sip\sip.h"
#include "app\app.h"
#include "timer\timer.h"
#include "sipadapt\sipadapt.h"

/* 本模块对外提供的常量和结构头文件 */
#include "system\system.h"

/* 本模块内部使用的常量和结构头文件 */

/* 本模块内部函数声明头文件 */

/* 本模块全局变量声明头文件 */
#include "system_var.inc"

SYS_MODULE_CB_S   *g_pstMsgCB = NULL_PTR;
SYS_MODULE_INFO_S g_astModuleCfg[SYS_MODULE_BUTT] =
{
    {APP_InitProc,          APP_MsgProc},
    {TIMER_Init,            NULL_PTR},
    {SIP_ADPT_Init,         SIP_APDT_MsgProc}
};