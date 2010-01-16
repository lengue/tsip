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
    BOOL                 bCoreFlag;   /* �Ƿ�ƥ����Ĺ��� */
    ULONG                ulIndex;      /* �������� */
    struct tagABNF_GRAMMAR_NODE_S *pstChild;     /* ��һ�����ӹ��� */
    struct tagABNF_GRAMMAR_NODE_S *pstNextNode;  /* ˳�����һ���ӹ��� */
}ABNF_GRAMMAR_NODE_S;

/* Ԫ�ؽṹ */
typedef struct tagABNF_ELEMENT_S
{
    /* �ظ����� */
    ULONG ulMinRepeat;
    ULONG ulMaxRepeat;

    /* ����˳����߿�ѡ������� */
    struct tagABNF_ELEMENT_S *pNextElement;

    /* ��ʶ���� */
    ABNF_ELEMENT_TYPE_E eType;
    union
    {
        /* �����ַ����� */
        struct
        {
            UCHAR ucMinValue; /* ��Сֵ */
            UCHAR ucMaxValue; /* ���ֵ�������ַ�ʱ������Сֵ */
        }stChar;

        /* ������� */
        struct
        {
            BOOL  bCoreFlag;  /* ��ʶ�Ƿ��Ǻ��Ĺ��� */
            ULONG ulIndex;    /* �������� */
        }stRule;

        /* �����ַ������� */
        struct
        {
            BOOL   bSensitive; /* �Ƿ��Сд���� */
            UCHAR *pucString;  /* �ַ���ָ�� */
        }stString;

        /* ��ϵĵ�һ����Ԫ�أ����������к���� */
        struct tagABNF_ELEMENT_S *pstFirstChild;
    }u;
}ABNF_ELEMENT_S;

/* ��������ṹ */
typedef struct tagABNF_RULE_S
{
    UCHAR             aucName[ABNF_MAX_RULE_NAME_LEN + 1];   /* �������� */
    ULONG             ulAppRuleIndex;                        /* ӳ���Ӧ�ù��� */
    ABNF_ELEMENT_S   *pstElements;
}ABNF_RULE_S;

/* ����ṹ */
typedef struct tagABNF_RULE_LIST_S
{
    ULONG        ulRuleNum; /* ������Ŀ */
    ABNF_RULE_S *pstRules;  /* �������� */
}ABNF_RULE_LIST_S;

typedef ULONG (*ABNF_PARSE_FUNC)(ABNF_GRAMMAR_NODE_S *pstNode, UCHAR *pucText, void **ppStruct);

typedef struct tagABNF_APP_RULE_S
{
    ABNF_APP_RULE_HEADER
    ABNF_PARSE_FUNC pfnParseFunc;
}ABNF_APP_RULE_S;