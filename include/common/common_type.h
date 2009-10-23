/*******************************************************************************
功能    : COMMON模块对外提供的类型
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/

/* 全局数据类型 */
typedef unsigned char   UCHAR;
typedef unsigned short  USHORT;
//typedef unsigned int    ULONG;
typedef unsigned int    HTIMER;
#if !WIN32
typedef UCHAR           BOOL;
#endif

/* 通用的队列管理结构 */
typedef struct tagCOMM_QUEUE_S
{
    ULONG ulHead;
    ULONG ulTail;
    ULONG ulNum;
}COMM_QUEUE_S;

/* 控制块通用头，所有使用通用管理的控制块都必须包含这个头，并且必须放在控制块最
前面 */
#define COMM_CB_HEADER    \
                                UCHAR  ucIsUsed;\
                                ULONG  ulNext;

/* 通用的控制块初始化 */
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

/* 通用的控制块分配 */
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

/* 通用的控制块释放 */
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