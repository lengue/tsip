/*******************************************************************************
����    : �����ṹ����
������  : �ƴ�ƽ
��������: 200.01.10
�޸ļ�¼:
*******************************************************************************/
/* ������ӿ��ƿ���Ŀ */
#define CONN_MAX_CONN 100

/* ���ӵĽ��ܺͷ��ͻ��� */
#define CONN_SEND_BUFFER 10000
#define CONN_RECV_BUFFER 10000

typedef enum tagCONN_STATE_E
{
    CONN_STATE_DEACTIVE,
    CONN_STATE_ACTIVE,
    CONN_STATE_BUTT
}CONN_STATE_E;