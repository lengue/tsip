/*******************************************************************************
����    : �������Ϣ����(WIN32ר��)
������  : �ƴ�ƽ
��������: 200.01.10
�޸ļ�¼:
*******************************************************************************/
/* �ⲿ����ģ��ͷ�ļ� */
#include "common\common.h"
#include "abnf\abnf.h"
#include "ubuf\ubuf.h"
#include "sip\uri.h"
#include "sip\sip.h"


/* ��ģ������ṩ�ĳ����ͽṹͷ�ļ� */

/* ��ģ���ڲ�ʹ�õĳ����ͽṹͷ�ļ� */

/* ��ģ���ڲ���������ͷ�ļ� */


/* ��ģ��ȫ�ֱ�������ͷ�ļ� */


/* ����һ���Ի� */
ULONG SIP_Dlg_CreateDialog(UBUF_HEADER_S * pstUbufSipMsg, ULONG *pulDlgIndex)
{
    /* ����һ���Ի����ƿ� */
    /* �����Ϣ����Call-ID,��ʹ�ø�Call-ID�������Զ����� */
    /* �����Ϣ����From tag,��ʹ�ø�From tag�������Զ����� */
    /* �����Ϣ����Cseq,��ʹ�ø�Cseq�������Զ����� */

    return SUCCESS;
}

ULONG SIP_Dlg_ReleaseDialog(ULONG *pulDlgIndex)
{
    return SUCCESS;
}

/* ����һ���Ի� */
ULONG SIP_Dlg_SendRequest(ULONG ulDlgIndex, UBUF_HEADER_S * pstUbufSipMsg)
{
 //   ULONG ulTxnID;

    /* �����Ϣ��û��Call-ID,���նԻ����ƿ��е�CallID��� */
    /* �����Ϣ��û��From tag,���նԻ����ƿ��е�From tag��� */
    /* �����Ϣ��û��Cseq,���նԻ����ƿ��е�Cseq��� */
    /* �����Ϣ��û��MaxForwards,����� */

    /* ����Э��������ڽ�����URI */

    /* ������������ɹ���������������Ϣ���� */

    return SUCCESS;
}

/* ����һ���Ի� */
ULONG SIP_Dlg_SendResponse(ULONG ulDlgIndex, UBUF_HEADER_S * pstUbufSipMsg)
{
    /* �����Ϣ��û��Call-ID,���նԻ����ƿ��е�CallID��� */
    /* �����Ϣ��û��From tag,���նԻ����ƿ��е�From tag��� */
    /* �����Ϣ��û��Cseq,���նԻ����ƿ��е�Cseq��� */
    /* �����Ϣ��û��MaxForwards,����� */

    /* �ҵ����ڽ�����URI */
    /* ������������ɹ���������������Ϣ���� */
    /* û�н����ɹ�����ڷ��Ͷ��� */
    return SUCCESS;
}

ULONG SIP_Dlg_AllocDialogCB(ULONG *pulDialogCBID)
{
    return SUCCESS;
}

