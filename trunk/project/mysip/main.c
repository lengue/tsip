/*******************************************************************************
����    : ����������
������  : �ƴ�ƽ
��������: 200.01.10
�޸ļ�¼:
*******************************************************************************/
/* �ⲿ����ģ��ͷ�ļ� */
#include "common\common.h"
#include "system\system.h"
#include "ubuf\ubuf.h"
#include "sip\uri.h"
#include "sip\sip.h"
#include "app\app.h"

/* ��ģ������ṩ�ĳ����ͽṹͷ�ļ� */
/* ��ģ���ڲ�ʹ�õĳ����ͽṹͷ�ļ� */
/* ��ģ���ڲ���������ͷ�ļ� */
/* ��ģ��ȫ�ֱ�������ͷ�ļ� */

#include "main.h"

int main(int argc, char* argv[])
{
    ULONG ulRet;

    ulRet = SYS_InitProc();
    if (ulRet != SUCCESS)
    {
        printf("\r\nSYS_InitProc Failed");
        return ulRet;
    }

    return APP_StartUserInterface();
}
