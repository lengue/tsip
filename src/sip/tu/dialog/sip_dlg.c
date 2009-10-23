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


/* 本模块对外提供的常量和结构头文件 */

/* 本模块内部使用的常量和结构头文件 */

/* 本模块内部函数声明头文件 */


/* 本模块全局变量声明头文件 */


/* 创建一个对话 */
ULONG SIP_Dlg_CreateDialog(UBUF_HEADER_S * pstUbufSipMsg, ULONG *pulDlgIndex)
{
    /* 申请一个对话控制块 */
    /* 如果消息中有Call-ID,则使用该Call-ID，否则自动生成 */
    /* 如果消息中有From tag,则使用该From tag，否则自动生成 */
    /* 如果消息中有Cseq,则使用该Cseq，否则自动生成 */

    return SUCCESS;
}

ULONG SIP_Dlg_ReleaseDialog(ULONG *pulDlgIndex)
{
    return SUCCESS;
}

/* 创建一个对话 */
ULONG SIP_Dlg_SendRequest(ULONG ulDlgIndex, UBUF_HEADER_S * pstUbufSipMsg)
{
 //   ULONG ulTxnID;

    /* 如果消息中没有Call-ID,按照对话控制块中的CallID添加 */
    /* 如果消息中没有From tag,按照对话控制块中的From tag添加 */
    /* 如果消息中没有Cseq,按照对话控制块中的Cseq添加 */
    /* 如果消息中没有MaxForwards,则添加 */

    /* 按照协议查找用于解析的URI */

    /* 如果立即解析成功，这立即启动消息发送 */

    return SUCCESS;
}

/* 创建一个对话 */
ULONG SIP_Dlg_SendResponse(ULONG ulDlgIndex, UBUF_HEADER_S * pstUbufSipMsg)
{
    /* 如果消息中没有Call-ID,按照对话控制块中的CallID添加 */
    /* 如果消息中没有From tag,按照对话控制块中的From tag添加 */
    /* 如果消息中没有Cseq,按照对话控制块中的Cseq添加 */
    /* 如果消息中没有MaxForwards,则添加 */

    /* 找到用于解析的URI */
    /* 如果立即解析成功，这立即启动消息发送 */
    /* 没有解析成功则挂在发送队列 */
    return SUCCESS;
}

ULONG SIP_Dlg_AllocDialogCB(ULONG *pulDialogCBID)
{
    return SUCCESS;
}

