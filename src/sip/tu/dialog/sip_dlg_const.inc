/* SIPӦ�ã�Ŀǰ�лỰ�Ͷ������֣��Ự��INVITE������BYE�ͷţ�������SUBSCRIBE����
REFER��������ص�NOTIFY�ͷ� */
typedef enum tagSIP_APPLICATION_TYPE_E
{
    SIP_APPLICATION_SESSION,
    SIP_APPLICATION_SUBSCRIPTION,
    SIP_APPLICATION_BUTT
}SIP_APPLICATION_TYPE_E;

/* ý��Э��״̬,����session��early-session���ֲ������ͣ�����Ľ��ܺͷ��Ͷ��ǶԱ�
�˶��� */
typedef enum tagSIP_NEGO_STATE_E
{
    SIP_NEGO_STATE_NULL,
    SIP_NEGO_STATE_RECV_OFFER,
    SIP_NEGO_STATE_SEND_OFFER,
    SIP_NEGO_STATE_COMPLETE,
    SIP_NEGO_STATE_BUTT
}SIP_NEGO_STATE_E;
