/*******************************************************************************
����    : TIMERģ�鹦�ܺ���
������  : �ƴ�ƽ
��������: 2009.07.19
�޸ļ�¼:
*******************************************************************************/
/* �ⲿ����ģ��ͷ�ļ� */
#include "common\common.h"
#include "system\system.h"

/* ��ģ������ṩ�ĳ����ͽṹͷ�ļ� */
#include "timer\timer.h"

/* ��ģ���ڲ�ʹ�õĳ����ͽṹͷ�ļ� */
#include "timer_const.inc"
#include "timer_type.inc"

/* ��ģ���ڲ���������ͷ�ļ� */
#include "timer.inc"

/* ��ģ��ȫ�ֱ�������ͷ�ļ� */
#include "timer_var.inc"

/* TIMERģ���ʼ������ */
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

/* ����һ����ʱ�� */
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

    /* ����һ����ʱ�����ƿ� */
    ulRet = TIMER_AllocCB(&ulTimerID);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }

    /* ��¼��ʱ������ */
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

    /* ���ò���ϵͳ����������ʱ�� */
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
