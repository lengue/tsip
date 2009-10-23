/*******************************************************************************
����    : abnfģ������ṩ�ĳ���
������  : �ƴ�ƽ
��������: 200.01.10
�޸ļ�¼:
*******************************************************************************/
/* ÿ������Ļ������Ԫ�ص����� */
typedef enum tagABNF_ELEMENT_TYPE_E
{
    ABNF_ELEMENT_DATA_CHAR,   /* �����ַ����� */
    ABNF_ELEMENT_DATA_STRING, /* �����ַ������� */
    ABNF_ELEMENT_RULE,        /* �������� */
    ABNF_ELEMENT_SEQUENCE,    /* ˳����� */
    ABNF_ELEMENT_OPTIONAL,    /* ��ѡ��� */
    ABNF_ELEMENT_BUTT
}ABNF_ELEMENT_TYPE_E;

/* ���������ֵ */
#define ABNF_MAX_RULE_NAME_LEN 31

#define ABNF_APP_RULE_HEADER \
    UCHAR aucName[ABNF_MAX_RULE_NAME_LEN + 1];   /* ABNF�������� */\
    ULONG ulRuleIndex;                           /* ABNF������������ */