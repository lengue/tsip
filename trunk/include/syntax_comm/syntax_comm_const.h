/* ����ר�ú�*/
/* �ӻ�ȡ�ַ���ֵ�� */
#define SYNTAX_GET_STRING(pucDest, pstNode, pucString, pstUbuf, ppStruct) \
{ \
    *(ppStruct) = UBUF_AddComponent(pstUbuf, (pstNode)->ulSize + 1);\
    (pucDest) = *(ppStruct);\
    strncpy((pucDest), pucString + (pstNode)->ulOffset, (pstNode)->ulSize); \
    (pucDest)[(pstNode)->ulSize] = '\0'; \
}

/* ��ȡʮ�������ֺ� */
#define SYNTAX_GET_DEC_NUM(Number, pucChar) \
{\
    Number = 0;\
    while ((*pucChar >= '0') && (*pucChar <= '9'))\
    {\
        Number = Number*10 + *pucChar - '0';\
        pucChar++;\
    }\
}

/* �жϻ�ȡ����ռ� */
#define SYNTAX_GET_COMPONET_PTR(pstComponet, Type, pstUbuf, ppStruct)\
{\
    if (*ppStruct == NULL_PTR)\
    {\
        *ppStruct = UBUF_AddComponent(pstUbuf, sizeof(Type));\
        memset(*ppStruct, 0, sizeof(Type));\
    }\
\
    pstComponet = *ppStruct;\
}

/* ����ר�ú� */
/* ����Ϣ���������һ����ͨ�ַ���ֵ */
#define SYNTAX_ADD_STRING(pstBuffer, pucString) \
{\
    strncpy(pstBuffer->pucBuffer + pstBuffer->ulCurrentLen,\
            (pucString), \
            strlen(pucString)); \
    pstBuffer->ulCurrentLen += (ULONG)strlen(pucString); \
}

/* ����Ϣ���������һ��10�������� */
#define SYNTAX_ADD_DEC_NUM(pstBuffer, Number) \
{\
    itoa(Number, pstBuffer->pucBuffer + pstBuffer->ulCurrentLen, 10);\
    pstBuffer->ulCurrentLen += (ULONG)strlen(pstBuffer->pucBuffer + pstBuffer->ulCurrentLen); \
}

/* ��¡ר�ú�*/
#define SYNTAX_CLONE_STRING(pstDstUbuf, pucDstStr, pucSrcStr) \
{ \
    ULONG ulLen = (ULONG)strlen(pucSrcStr);\
    (pucDstStr) = UBUF_AddComponent(pstDstUbuf, ulLen + 1); \
    strncpy((pucDstStr), (pucSrcStr), ulLen); \
    (pucDstStr)[ulLen] = '\0'; \
}

