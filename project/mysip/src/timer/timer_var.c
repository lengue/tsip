/*******************************************************************************
功能    : TIMER模块全局变量声明
创建人  : 唐春平
创建日期: 2009.07.19
修改记录:
*******************************************************************************/
/* 外部依赖模块头文件 */
#include "common\common.h"

/* 本模块对外提供的常量和结构头文件 */
#include "timer\timer_const.h"
#include "timer\timer_type.h"

/* 本模块内部使用的常量和结构头文件 */
#include "timer_const.inc"
#include "timer_type.inc"

/* 本模块内部函数声明头文件 */

/* 本模块全局变量声明头文件 */
#include "timer_var.inc"

TIMER_S     *g_pstTimerCB = NULL_PTR;
COMM_QUEUE_S g_stTimerQueue;