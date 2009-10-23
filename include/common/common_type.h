/*******************************************************************************
����    : COMMONģ������ṩ������
������  : �ƴ�ƽ
��������: 200.01.10
�޸ļ�¼:
*******************************************************************************/

/* ȫ���������� */
typedef unsigned char   UCHAR;
typedef unsigned short  USHORT;
//typedef unsigned int    ULONG;
typedef unsigned int    HTIMER;
#if !WIN32
typedef UCHAR           BOOL;
#endif

/* ͨ�õĶ��й���ṹ */
typedef struct tagCOMM_QUEUE_S
{
    ULONG ulHead;
    ULONG ulTail;
    ULONG ulNum;
}COMM_QUEUE_S;

/* ���ƿ�ͨ��ͷ������ʹ��ͨ�ù���Ŀ��ƿ鶼����������ͷ�����ұ�����ڿ��ƿ���
ǰ�� */
#define COMM_CB_HEADER    \
                                UCHAR  ucIsUsed;\
                                ULONG  ulNext;

/* ͨ�õĿ��ƿ��ʼ�� */
#define COMM_CB_INIT(ulNum_, ulSize_, pstCBAddr_, stQueue_) \
{\
    ULONG  ulLoop;\
    (pstCBAddr_) = malloc((ulNum_) * (ulSize_));\
    if((pstCBAddr_) != NULL_PTR)\
    {\
        for (ulLoop = 0; ulLoop < (ulNum_); ulLoop++)\
        {\
            (pstCBAddr_)[ulLoop].ucIsUsed = FALSE;\
            (pstCBAddr_)[ulLoop].ulNext   = ulLoop + 1;\
        }\
        \
        (pstCBAddr_)[(ulNum_) - 1].ulNext   = NULL_ULONG;\
        \
        (stQueue_).ulNum = (ulNum_);\
        (stQueue_).ulHead = 0;\
        (stQueue_).ulTail = (ulNum_) - 1;\
    }\
}

/* ͨ�õĿ��ƿ���� */
#define COMM_CB_ALLOC(pstCBAddr_, stQueue_, ulID_) \
{\
    if ((stQueue_).ulNum == 0)\
    {\
        (ulID_) = NULL_ULONG;\
    }\
    else\
    {\
        (ulID_) = (stQueue_).ulHead;\
\
        (stQueue_).ulNum--;\
        if ((stQueue_).ulNum == 0)\
        {\
            (stQueue_).ulHead = NULL_ULONG;\
            (stQueue_).ulTail = NULL_ULONG;\
        }\
        else\
        {\
            (stQueue_).ulHead = (pstCBAddr_)[(ulID_)].ulNext;\
        }\
\
        (pstCBAddr_)[(ulID_)].ucIsUsed = TRUE;\
        (pstCBAddr_)[(ulID_)].ulNext   = NULL_ULONG;\
    }\
}

/* ͨ�õĿ��ƿ��ͷ� */
#define COMM_CB_FREE(pstCBAddr_, stQueue_, ulID_) \
{\
    if ((pstCBAddr_)[(ulID_)].ucIsUsed == TRUE)\
    {\
        (stQueue_).ulNum++;\
        if ((stQueue_).ulNum == 1)\
        {\
            (stQueue_).ulHead = (ulID_);\
            (stQueue_).ulTail = (ulID_);\
        }\
        else\
        {\
            (pstCBAddr_)[(stQueue_).ulTail].ulNext = (ulID_);\
            (stQueue_).ulTail = (ulID_);\
        }\
\
        (pstCBAddr_)[(ulID_)].ucIsUsed = FALSE;\
        (pstCBAddr_)[(ulID_)].ulNext   = NULL_USHORT;\
    }\
}