/*******************************************************************************
����    : TIMERģ��ȫ�ֱ�������
������  : �ƴ�ƽ
��������: 2009.07.19
�޸ļ�¼:
*******************************************************************************/
/* �ⲿ����ģ��ͷ�ļ� */
#include "common\common.h"

/* ��ģ������ṩ�ĳ����ͽṹͷ�ļ� */
#include "timer\timer_const.h"
#include "timer\timer_type.h"

/* ��ģ���ڲ�ʹ�õĳ����ͽṹͷ�ļ� */
#include "timer_const.inc"
#include "timer_type.inc"

/* ��ģ���ڲ���������ͷ�ļ� */

/* ��ģ��ȫ�ֱ�������ͷ�ļ� */
#include "timer_var.inc"

TIMER_S     *g_pstTimerCB = NULL_PTR;
COMM_QUEUE_S g_stTimerQueue;