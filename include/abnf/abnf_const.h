/*******************************************************************************
����    : abnfģ������ṩ�ĳ���
������  : �ƴ�ƽ
��������: 200.01.10
�޸ļ�¼:
*******************************************************************************/
/* ���������ֵ */
#define ABNF_MAX_RULE_NAME_LEN 31

#define ABNF_APP_RULE_HEADER \
    UCHAR aucName[ABNF_MAX_RULE_NAME_LEN + 1];   /* ABNF�������� */\
    ULONG ulRuleIndex;                           /* ABNF������������ */

/* ����Ƿ�ƥ��Ӧ�ù��� */
#define ABNF_RULE_MATCH(pstNode, _ucRuleListIndex, pstAppRule, ulAppRuleIndex) \
 ((pstNode->ucRuleListIndex == _ucRuleListIndex) \
&&(pstNode->ulIndex == pstAppRule[ulAppRuleIndex].ulRuleIndex))

#define ABNF_GET_RULE_INDEX(pstAppRule, ulAppRuleIndex) pstAppRule[ulAppRuleIndex].ulRuleIndex

#define ABNF_MAX_DEPEND_RULE_NUM 10