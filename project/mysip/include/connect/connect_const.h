/* ��������Ŀ */
#define CONN_MAX_LISTEN_NUM 5

typedef enum tagCONN_TRANSPORT_PROTOCOL_E
{
    CONN_TRANSPORT_PROTOCOL_TCP,
    CONN_TRANSPORT_PROTOCOL_TLS,
    CONN_TRANSPORT_PROTOCOL_UDP,
    CONN_TRANSPORT_PROTOCOL_SCTP,
    CONN_TRANSPORT_PROTOCOL_BUTT
}CONN_TRANSPORT_PROTOCOL_E;