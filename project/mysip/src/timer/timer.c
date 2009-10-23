/*******************************************************************************
功能    : TIMER模块功能函数
创建人  : 唐春平
创建日期: 2009.07.19
修改记录:
*******************************************************************************/
/* 外部依赖模块头文件 */
#include "common\common.h"
#include "system\system.h"

/* 本模块对外提供的常量和结构头文件 */
#include "timer\timer.h"

/* 本模块内部使用的常量和结构头文件 */
#include "timer_const.inc"
#include "timer_type.inc"

/* 本模块内部函数声明头文件 */
#include "timer.inc"

/* 本模块全局变量声明头文件 */
#include "timer_var.inc"

/* TIMER模块初始化函数 */
ULONG TIMER_Init()
{
    ULONG ulLoop;

    g_pstTimerCB = malloc(TIMER_CB_NUM * sizeof(TIMER_S));
    if (g_pstTimerCB == NULL_PTR)
    {
        return FAIL;
    }

    for (ulLoop = 0; ulLoop < TIMER_CB_NUM; ulLoop++)
    {
        g_pstTimerCB[ulLoop].ucIsUsed = FALSE;
        g_pstTimerCB[ulLoop].ulNext = ulLoop + 1;
    }

    g_pstTimerCB[TIMER_CB_NUM - 1].ulNext = NULL_ULONG;

    g_stTimerQueue.ulHead = 0;
    g_stTimerQueue.ulTail = TIMER_CB_NUM - 1;
    g_stTimerQueue.ulNum = TIMER_CB_NUM;

    return SUCCESS;
}

ULONG TIMER_AllocCB(ULONG *pulTimerID)
{
    ULONG ulID;
    if (g_stTimerQueue.ulNum == 0)
    {
        return FAIL;
    }

    ulID = g_stTimerQueue.ulHead;

    g_stTimerQueue.ulNum--;
    if(g_stTimerQueue.ulNum == 0)
    {
        g_stTimerQueue.ulHead = NULL_ULONG;
        g_stTimerQueue.ulTail = NULL_ULONG;
    }
    else
    {
        g_stTimerQueue.ulHead = g_pstTimerCB[ulID].ulNext;
    }

    g_pstTimerCB[ulID].ucIsUsed = TRUE;
    g_pstTimerCB[ulID].ulName   = NULL_ULONG;
    g_pstTimerCB[ulID].ulLen    = NULL_ULONG;
    g_pstTimerCB[ulID].ulPara   = NULL_ULONG;
    g_pstTimerCB[ulID].hTimer   = INVALID_HANDLE_VALUE;
    g_pstTimerCB[ulID].ulNext   = FALSE;

    *pulTimerID = ulID;
    return SUCCESS;
}

VOID TIMER_ReleaseCB(ULONG ulTimerID)
{
    ULONG ulID;

    if ((ulTimerID >= TIMER_CB_NUM)
      ||(g_pstTimerCB[ulTimerID].ucIsUsed == FALSE))
    {
        return;
    }

    g_stTimerQueue.ulNum++;
    if(g_stTimerQueue.ulNum == 1)
    {
        g_stTimerQueue.ulHead = ulTimerID;
        g_stTimerQueue.ulTail = ulTimerID;
    }
    else
    {
        ulID = g_stTimerQueue.ulTail;
        g_pstTimerCB[ulID].ulPara = ulTimerID;
        g_stTimerQueue.ulTail = ulTimerID;
    }

    return;
}

/* 启动一个定时器 */
ULONG TIMER_Start(ULONG ulModuleID,
                  ULONG ulName,
                  ULONG ulLen,
                  ULONG ulPara,
                  TIMER_TYPE_E eType,
                  ULONG *pulHandle)
{
    ULONG     ulRet;
    ULONG     ulTimerID;
    HANDLE    hTimerQTimer;
    ULONG     ulPeriod;

    /* 分配一个定时器控制块 */
    ulRet = TIMER_AllocCB(&ulTimerID);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }

    /* 记录定时器参数 */
    g_pstTimerCB[ulTimerID].ulModuleID = ulModuleID;
    g_pstTimerCB[ulTimerID].ulName = ulName;
    g_pstTimerCB[ulTimerID].eType  = eType;
    g_pstTimerCB[ulTimerID].ulLen  = ulLen;
    g_pstTimerCB[ulTimerID].ulPara = ulPara;

    if (eType == TIMER_TYPE_LOOP)
    {
        ulPeriod = ulLen;
    }
    else
    {
        ulPeriod = 0;
    }

    /* 调用操作系统函数启动定时器 */
    CreateTimerQueueTimer(&hTimerQTimer,
                          NULL,
                          TIMER_TimeoutProc,
                          (PVOID)ulTimerID,
                          ulLen,
                          ulPeriod,
                          0);
    if (hTimerQTimer == INVALID_HANDLE_VALUE)
    {
        TIMER_ReleaseCB(ulTimerID);
        return FAIL;
    }

    g_pstTimerCB[ulTimerID].hTimer = hTimerQTimer;
    *pulHandle = ulTimerID;

    return SUCCESS;
}

VOID CALLBACK TIMER_TimeoutProc(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
{
    ULONG ulTimerID;
    TIMER_MSG_S stMsg;

    ulTimerID = (ULONG)lpParameter;

    stMsg.ulName = g_pstTimerCB[ulTimerID].ulName;
    stMsg.ulPara = g_pstTimerCB[ulTimerID].ulPara;

    SYS_SendMsg(SYS_MODULE_TIMER,
                g_pstTimerCB[ulTimerID].ulModuleID,
                &stMsg,
                sizeof(TIMER_MSG_S));

    if (g_pstTimerCB[ulTimerID].eType == TIMER_TYPE_NOLOOP)
    {
        TIMER_Stop(ulTimerID);
    }

    return;
}

VOID TIMER_Stop(ULONG ulHandle)
{
    if ((ulHandle >= TIMER_CB_NUM)
      ||(g_pstTimerCB[ulHandle].ucIsUsed == FALSE))
    {
        return;
    }

    DeleteTimerQueueTimer(NULL, g_pstTimerCB[ulHandle].hTimer, NULL);
    TIMER_ReleaseCB(ulHandle);
    return;
}
