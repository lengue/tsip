/*******************************************************************************
����    : SIPЭ��ջ����㹦�ܺ���
������  : �ƴ�ƽ
��������: 2009.01.10
�޸ļ�¼:
*******************************************************************************/
/* ������������: �������ã�����ΪIP���IP�����򲻰�IP����дViaͷ�� */
//UCHAR *g_pucSipTxpHost;

/* �������������մ���Э�����У���¼��Э�������Ӷ��� */
extern ULONG           g_aulConnTransport[CONN_TRANSPORT_PROTOCOL_BUTT];

/* ������Ӷ��� */
extern CONN_LISTEN_S  *g_pstConnListen;
extern CONN_FUNC_MSG_PROC g_pfnConnMsgProc;

/* ���ӿ��ж����Լ����ӿ��ƿ� */
extern COMM_QUEUE_S    g_stConnCBQueue;
extern CONN_CB_S      *g_pstConnCB;