/*******************************************************************************
����    : ϵͳ��������
������  : �ƴ�ƽ
��������: 200.01.10
�޸ļ�¼:
*******************************************************************************/

/* ϵͳģ��IDö��ֵ */
typedef enum tagSYS_MODULE_ID_E
{
    SYS_MODULE_BEGIN,

    SYS_MODULE_APP = SYS_MODULE_BEGIN, /*��һ���Զ����ģ�����SYS_MODULE_BEGIN*/
    SYS_MODULE_TIMER,   /* ��ʱ��ģ�� */
    SYS_MODULE_SIP,   /* Э��ջ */

    SYS_MODULE_BUTT
}SYS_MODULE_ID_E;