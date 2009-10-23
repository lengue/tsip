/*******************************************************************************
功能    : 系统常量定义
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/

/* 系统模块ID枚举值 */
typedef enum tagSYS_MODULE_ID_E
{
    SYS_MODULE_BEGIN,

    SYS_MODULE_APP = SYS_MODULE_BEGIN, /*第一个自定义的模块等于SYS_MODULE_BEGIN*/
    SYS_MODULE_TIMER,   /* 定时器模块 */
    SYS_MODULE_SIP,   /* 协议栈 */

    SYS_MODULE_BUTT
}SYS_MODULE_ID_E;