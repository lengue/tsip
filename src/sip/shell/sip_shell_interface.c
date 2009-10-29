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

#include "..\include\syntax\sip_syntax.h"
#include "..\include\transport\sip_transport.h"
#include "..\include\transaction\sip_transaction.h"
#include "..\include\tu\sip_tu.h"
#include "..\include\shell\sip_shell.h"

/* 本模块对外提供的常量和结构头文件 */

/* 本模块内部使用的常量和结构头文件 */
#include "sip_shell_const.inc"
#include "sip_shell_type.inc"

/* 本模块内部函数声明头文件 */
#include "sip_shell_cfg.inc"

/* 本模块全局变量声明头文件 */
#include "sip_shell_var.inc"

/* 协议栈定时器超时 */
ULONG SIP_TimeoutProc(ULONG ulName, ULONG ulPara)
{
    /* 根据名字分发定时器 */
    SIP_Txn_TimerProc(ulPara, ulName);

    return SUCCESS;
}

/* 根据ABNF规则名获取对应的内部规则索引 */
ULONG SIP_GetRuleIndex(UCHAR *pucRuleName, ULONG *pulRuleIndex)
{
    return SIP_Syntax_GetRuleIndex(pucRuleName, pulRuleIndex);
}

/* 协议栈编码函数 */
ULONG SIP_Code(ULONG  ulRuleIndex,
               void  *pstStruct,
               UCHAR *pucBuffer,
               ULONG  ulBufferLen,
               ULONG *pulMsgLen)
{
    return SIP_Syntax_Code(ulRuleIndex, pstStruct, pucBuffer, ulBufferLen, pulMsgLen);
}

/* 协议栈解码函数 */
ULONG SIP_Decode(ULONG  ulRuleIndex,
                 UCHAR *pucMsgString,
                 ULONG  ulMsgLen,
                 UBUF_HEADER_S *pstUBuf,
                 void **ppstStruct)
{
    return SIP_Syntax_Decode(ulRuleIndex, pucMsgString, ulMsgLen, pstUBuf, ppstStruct);
}

/* 协议栈克隆函数 */
ULONG SIP_Clone(ULONG          ulRuleIndex,
                void          *pSrcStruct,
                UBUF_HEADER_S *pstDstUbuf,
                void         **ppDstStruct)
{
    return SIP_Syntax_Clone(ulRuleIndex, pSrcStruct, pstDstUbuf, ppDstStruct);
}

/* 协议栈收到网络层上报消息 */
ULONG SIP_RecvUpMsg(SIP_LOCATION_S *pstPeerLocation, UBUF_HEADER_S *pstUbufSipMsg)
{
    return SIP_Txp_RecvUpMsg(pstPeerLocation,
                             pstUbufSipMsg);
}

/* 协议栈收到应用层下发消息 */
ULONG SIP_RecvDownMsg(ULONG  ulAppRef1,
                      ULONG  ulAppRef2,
                      ULONG  ulStackRef1,
                      ULONG  ulStackRef2,
                      UBUF_HEADER_S * pstUbufSipMsg)
{
    return SIP_TU_RecvDownMsg(ulAppRef1,
                              ulAppRef2,
                              ulStackRef1,
                              ulStackRef2,
                              pstUbufSipMsg);
}


