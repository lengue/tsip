/*******************************************************************************
����    : abnfģ������ṩ�Ľṹ
������  : �ƴ�ƽ
��������: 200.01.10
�޸ļ�¼:
*******************************************************************************/
/* �ķ��ڵ�ṹ */
typedef struct tagABNF_GRAMMAR_NODE_S
{
    ULONG                ulOffset;     /* �ڵ�ƫ�� */
    ULONG                ulSize;       /* �ڵ㳤�� */
    UCHAR                ucRuleListIndex;   /* �Ƿ�ƥ����Ĺ��� */
    ULONG                ulIndex;      /* �������� */
    struct tagABNF_GRAMMAR_NODE_S *pstChild;     /* ��һ�����ӹ��� */
    struct tagABNF_GRAMMAR_NODE_S *pstNextNode;  /* ˳�����һ���ӹ��� */
}ABNF_GRAMMAR_NODE_S;