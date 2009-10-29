/*******************************************************************************
����    : �������Ϣ����(WIN32ר��)
������  : �ƴ�ƽ
��������: 200.01.10
�޸ļ�¼:
*******************************************************************************/
/* �ⲿ����ģ��ͷ�ļ� */
#include "common\common.h"
#include "ubuf\ubuf.h"
#include "sip\uri.h"
#include "sip\sip.h"

#include "..\include\syntax\sip_syntax.h"
#include "..\include\transport\sip_transport.h"
#include "..\include\transaction\sip_transaction.h"
#include "..\include\tu\sip_tu.h"
#include "..\include\shell\sip_shell.h"

/* ��ģ������ṩ�ĳ����ͽṹͷ�ļ� */

/* ��ģ���ڲ�ʹ�õĳ����ͽṹͷ�ļ� */
#include "sip_shell_const.inc"
#include "sip_shell_type.inc"

/* ��ģ���ڲ���������ͷ�ļ� */
#include "sip_shell_cfg.inc"

/* ��ģ��ȫ�ֱ�������ͷ�ļ� */
#include "sip_shell_var.inc"

/* Э��ջ��ʱ����ʱ */
ULONG SIP_TimeoutProc(ULONG ulName, ULONG ulPara)
{
    /* �������ַַ���ʱ�� */
    SIP_Txn_TimerProc(ulPara, ulName);

    return SUCCESS;
}

/* ����ABNF��������ȡ��Ӧ���ڲ��������� */
ULONG SIP_GetRuleIndex(UCHAR *pucRuleName, ULONG *pulRuleIndex)
{
    return SIP_Syntax_GetRuleIndex(pucRuleName, pulRuleIndex);
}

/* Э��ջ���뺯�� */
ULONG SIP_Code(ULONG  ulRuleIndex,
               void  *pstStruct,
               UCHAR *pucBuffer,
               ULONG  ulBufferLen,
               ULONG *pulMsgLen)
{
    return SIP_Syntax_Code(ulRuleIndex, pstStruct, pucBuffer, ulBufferLen, pulMsgLen);
}

/* Э��ջ���뺯�� */
ULONG SIP_Decode(ULONG  ulRuleIndex,
                 UCHAR *pucMsgString,
                 ULONG  ulMsgLen,
                 UBUF_HEADER_S *pstUBuf,
                 void **ppstStruct)
{
    return SIP_Syntax_Decode(ulRuleIndex, pucMsgString, ulMsgLen, pstUBuf, ppstStruct);
}

/* Э��ջ��¡���� */
ULONG SIP_Clone(ULONG          ulRuleIndex,
                void          *pSrcStruct,
                UBUF_HEADER_S *pstDstUbuf,
                void         **ppDstStruct)
{
    return SIP_Syntax_Clone(ulRuleIndex, pSrcStruct, pstDstUbuf, ppDstStruct);
}

/* Э��ջ�յ�������ϱ���Ϣ */
ULONG SIP_RecvUpMsg(SIP_LOCATION_S *pstPeerLocation, UBUF_HEADER_S *pstUbufSipMsg)
{
    return SIP_Txp_RecvUpMsg(pstPeerLocation,
                             pstUbufSipMsg);
}

/* Э��ջ�յ�Ӧ�ò��·���Ϣ */
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


