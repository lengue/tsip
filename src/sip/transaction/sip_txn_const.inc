/*******************************************************************************
����    : SIPЭ��ջ����㹦�ܺ���
������  : �ƴ�ƽ
��������: 2009.01.10
�޸ļ�¼:
*******************************************************************************/
#define SIP_TXN_MAX_FSM_ACTION 5
#define SIP_TXN_MAX_FSM_TIMER  2
#define SIP_TXN_CB_NUM 100

/* �����¼� */
typedef enum tagSIP_TXN_EVENT_E
{
    SIP_TXN_EVENT_SEND_REQUEST,             /* TU�������� */
    SIP_TXN_EVENT_SEND_1XX_RESPONSE,        /* TU����1XX��Ӧ */
    SIP_TXN_EVENT_SEND_2XX_RESPONSE,        /* TU����2XX��Ӧ */
    SIP_TXN_EVENT_SEND_3456XX_RESPONSE,     /* TU����3XX-6XX��Ӧ */
    SIP_TXN_EVENT_RECV_REQUEST,             /* �յ�������ACK���� */
    SIP_TXN_EVENT_RECV_ACK,                 /* �յ������ACK���� */
    SIP_TXN_EVENT_RECV_1XX_RESPONSE,        /* �յ������1XX��Ӧ */
    SIP_TXN_EVENT_RECV_2XX_RESPONSE,        /* �յ������2XX��Ӧ */
    SIP_TXN_EVENT_RECV_3456XX_RESPONSE,     /* �յ������3XX-6XX��Ӧ */
    SIP_TXN_EVENT_TIMER_A_FIRE,              /* ��ʱ��A��ʱ */
    SIP_TXN_EVENT_TIMER_B_FIRE,              /* ��ʱ��B��ʱ */
    SIP_TXN_EVENT_TIMER_D_FIRE,              /* ��ʱ��D��ʱ */
    SIP_TXN_EVENT_TIMER_E_FIRE,              /* ��ʱ��E��ʱ */
    SIP_TXN_EVENT_TIMER_F_FIRE,              /* ��ʱ��F��ʱ */
    SIP_TXN_EVENT_TIMER_K_FIRE,              /* ��ʱ��K��ʱ */
    SIP_TXN_EVENT_TIMER_G_FIRE,              /* ��ʱ��G��ʱ */
    SIP_TXN_EVENT_TIMER_H_FIRE,              /* ��ʱ��H��ʱ */
    SIP_TXN_EVENT_TIMER_I_FIRE,              /* ��ʱ��I��ʱ */
    SIP_TXN_EVENT_TIMER_J_FIRE,              /* ��ʱ��J��ʱ */
    SIP_TXN_EVENT_TXP_ERR,                  /* �������� */

    SIP_TXN_EVENT_BUTT
}SIP_TXN_EVENT_E;

/* ����״̬���������������״̬���� */
typedef enum tagSIP_TXN_STATE_E
{
    /* ��ʼ״̬ */
    SIP_TXN_STATE_INIT,

    SIP_TXN_STATE_TRYING,
    SIP_TXN_STATE_CALLING,
    SIP_TXN_STATE_PROCEEDING,
    SIP_TXN_STATE_COMPLETED,
    SIP_TXN_STATE_CONFIRMED,
    SIP_TXN_STATE_TERMINATED
}SIP_TXN_STATE_E;

/* �������� */
typedef enum tagSIP_TXN_TYPE_E
{
    SIP_TXN_TYPE_INVITE_CLIENT,     /* INVTE�ͻ������� */
    SIP_TXN_TYPE_NON_INVITE_CLIENT, /* ��INVTE�ͻ������� */
    SIP_TXN_TYPE_INVITE_SERVER,     /* INVTE������������ */
    SIP_TXN_TYPE_NON_INVITE_SERVER, /* ��INVTE������������ */

    SIP_TXN_TYPE_BUTT
}SIP_TXN_TYPE_E;