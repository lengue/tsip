/*******************************************************************************
功能    : 系统机制函数实现
          主要是消息机制和系统初始化机制
创建人  : 唐春平
创建日期: 2009.07.19
修改记录:
*******************************************************************************/
/* 外部依赖模块头文件 */
#include "common\common.h"

/* 本模块对外提供的常量和结构头文件 */
#include "system\system.h"

/* 本模块内部使用的常量和结构头文件 */

/* 本模块内部函数声明头文件 */

/* 本模块全局变量声明头文件 */
#include "system_var.inc"

/* 模块间消息发送 */
ULONG SYS_SendMsg(ULONG  ulSrcModuleID,
                  ULONG  ulDstModuleID,
                  void  *pstMsg,
                  ULONG  ulLength)
{
    SYS_MSG_S *pstSysMsg;
    SYS_MSG_S **ppstSysMsg;
    LONG      lPrevCount;

    /* 占用消息队列处理代码 */
    WaitForSingleObject(g_pstMsgCB[ulDstModuleID].hMutex, INFINITE);

    /*申请消息空间*/
    pstSysMsg = malloc(sizeof(SYS_MSG_S));
    pstSysMsg->ulSrcModuleID = ulSrcModuleID;
    pstSysMsg->ulDstModuleID = ulDstModuleID;
    pstSysMsg->pMsg    = malloc(ulLength);
    pstSysMsg->pstNext = NULL_PTR;
    memcpy(pstSysMsg->pMsg, pstMsg, ulLength);

    ppstSysMsg = &g_pstMsgCB[ulDstModuleID].pstMsgQueue;
    while (*ppstSysMsg != NULL_PTR)
    {
        ppstSysMsg = &(*ppstSysMsg)->pstNext;
    }

    *ppstSysMsg = pstSysMsg;

    /* 释放消息队列处理代码 */
    ReleaseMutex(g_pstMsgCB[ulDstModuleID].hMutex);

    /*对g_astMsgCB[ulDstModuleID].ulLight V操作*/
    ReleaseSemaphore(g_pstMsgCB[ulDstModuleID].hSemaphore, 1, &lPrevCount);

    return SUCCESS;
}

/* 模块消息处理任务 */
DWORD WINAPI SYS_ModuleMsgProcTask(PVOID pvParam)
{
    ULONG ulModuleID;
    SYS_MSG_S *pstMsg;
    SYS_MODULE_MSG_PROC *pfnMsgProc = NULL_PTR;

    ulModuleID = (ULONG)pvParam;

    while(1)
    {
        /* 等待消息队列中的消息 */
        WaitForSingleObject(g_pstMsgCB[ulModuleID].hSemaphore, INFINITE);

        /* 占用消息队列处理代码 */
        WaitForSingleObject(g_pstMsgCB[ulModuleID].hMutex, INFINITE);

        /* 取出当前的消息 */
        pstMsg = g_pstMsgCB[ulModuleID].pstMsgQueue;
        g_pstMsgCB[ulModuleID].pstMsgQueue = pstMsg->pstNext;
        g_pstMsgCB[ulModuleID].pfnMsgProc(pstMsg->ulSrcModuleID,
                                          pstMsg->pMsg);
        free(pstMsg->pMsg);
        free(pstMsg);

        /* 释放消息队列处理代码 */
        ReleaseMutex(g_pstMsgCB[ulModuleID].hMutex);
    }

    return SUCCESS;
}

/* 模块初始化 */
ULONG SYS_InitProc()
{
    ULONG ulLoop;
    ULONG ulRet;
    DWORD dwThreadID;

    g_pstMsgCB = (SYS_MODULE_CB_S *)malloc(SYS_MODULE_BUTT * sizeof(SYS_MODULE_CB_S));
    memset(g_pstMsgCB, 0, SYS_MODULE_BUTT * sizeof(SYS_MODULE_CB_S));

    /* 各模块初始化 */
    for(ulLoop = 0; ulLoop < SYS_MODULE_BUTT; ulLoop++)
    {
        if (g_astModuleCfg[ulLoop].pfnInitProc != NULL_PTR)
        {
            ulRet = g_astModuleCfg[ulLoop].pfnInitProc();
            if (ulRet != SUCCESS)
            {
                return ulRet;
            }
        }
    }

    /* 各模块消息处理任务 */
    for(ulLoop = 0; ulLoop < SYS_MODULE_BUTT; ulLoop++)
    {
        if (g_astModuleCfg[ulLoop].pfnMsgProc == NULL)
        {
            /* 没有消息处理函数不启动 */
            continue;
        }
        g_pstMsgCB[ulLoop].pfnMsgProc = g_astModuleCfg[ulLoop].pfnMsgProc;

        /* 每个消息队列最多100个消息未处理 */
        g_pstMsgCB[ulLoop].hSemaphore = CreateSemaphore(NULL,0,100,NULL);

        /* 每个消息队列最多100个消息未处理 */
        g_pstMsgCB[ulLoop].hMutex     = CreateMutex(NULL, FALSE, NULL);

        /*对每个需要消息处理的模块建立消息处理线程*/
        g_pstMsgCB[ulLoop].hThread    = CreateThread(NULL,
                                                     0,
                                                     SYS_ModuleMsgProcTask,
                                                    (PVOID)ulLoop,
                                                     0,
                                                    &dwThreadID);
        CloseHandle(g_pstMsgCB[ulLoop].hThread);
    }

    return SUCCESS;
}

int SYS_CopyFile2Buffer(FILE *pfFile, char *pBuffer, int iBufferLen)
{
    char c;
    int iCount = 0;

    c = getc(pfFile);
    while(c != EOF)
    {
        if(iCount >= iBufferLen)
        {
            break;
        }

        pBuffer[iCount] = c;
        iCount++;
        c = getc(pfFile);
    }

    return iCount;
}

UCHAR *SYS_GetFileContent(UCHAR *pucFileName, ULONG *pulLen)
{
    FILE *pFile;
    char *pBuffer = NULL;
    int  iCount;

    pFile = fopen(pucFileName, "rb");
    if (pFile == NULL)
    {
        printf("open file %s failed", pucFileName);
        return NULL_PTR;
    }
    else
    {
        pBuffer = malloc(100000);
        iCount = SYS_CopyFile2Buffer(pFile, pBuffer, 100000);
        pBuffer[iCount] = '\0';
    }

    *pulLen = iCount;
    fclose(pFile);
    return pBuffer;
}

void SYS_FreeFileContent(UCHAR *pucText)
{
    if (pucText == NULL_PTR)
    {
        return;
    }

    free(pucText);
    return;
}

/* 产生随机字符串 */
ULONG g_ulSeedSet = FALSE;
ULONG SYS_GeneralRandomString(CHAR *pcString, ULONG ulSize)
{
    ULONG ulRandom;
    ULONG ulSeed;
    LARGE_INTEGER lCount;

    if ((pcString == NULL_PTR) || (ulSize == 0))
    {
        return FAIL;
    }

    if(FALSE == g_ulSeedSet)
    {
        g_ulSeedSet = TRUE;
        QueryPerformanceCounter (&lCount);
        ulSeed = lCount.LowPart + lCount.HighPart;
        srand(ulSeed);
    }

    ulRandom = rand();
    sprintf(pcString, "%x", ulRandom);
    return SUCCESS;
}