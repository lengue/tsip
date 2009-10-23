/*******************************************************************************
����    : ϵͳ���ƺ���ʵ��
          ��Ҫ����Ϣ���ƺ�ϵͳ��ʼ������
������  : �ƴ�ƽ
��������: 2009.07.19
�޸ļ�¼:
*******************************************************************************/
/* �ⲿ����ģ��ͷ�ļ� */
#include "common\common.h"

/* ��ģ������ṩ�ĳ����ͽṹͷ�ļ� */
#include "system\system.h"

/* ��ģ���ڲ�ʹ�õĳ����ͽṹͷ�ļ� */

/* ��ģ���ڲ���������ͷ�ļ� */

/* ��ģ��ȫ�ֱ�������ͷ�ļ� */
#include "system_var.inc"

/* ģ�����Ϣ���� */
ULONG SYS_SendMsg(ULONG  ulSrcModuleID,
                  ULONG  ulDstModuleID,
                  void  *pstMsg,
                  ULONG  ulLength)
{
    SYS_MSG_S *pstSysMsg;
    SYS_MSG_S **ppstSysMsg;
    LONG      lPrevCount;

    /* ռ����Ϣ���д������ */
    WaitForSingleObject(g_pstMsgCB[ulDstModuleID].hMutex, INFINITE);

    /*������Ϣ�ռ�*/
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

    /* �ͷ���Ϣ���д������ */
    ReleaseMutex(g_pstMsgCB[ulDstModuleID].hMutex);

    /*��g_astMsgCB[ulDstModuleID].ulLight V����*/
    ReleaseSemaphore(g_pstMsgCB[ulDstModuleID].hSemaphore, 1, &lPrevCount);

    return SUCCESS;
}

/* ģ����Ϣ�������� */
DWORD WINAPI SYS_ModuleMsgProcTask(PVOID pvParam)
{
    ULONG ulModuleID;
    SYS_MSG_S *pstMsg;
    SYS_MODULE_MSG_PROC *pfnMsgProc = NULL_PTR;

    ulModuleID = (ULONG)pvParam;

    while(1)
    {
        /* �ȴ���Ϣ�����е���Ϣ */
        WaitForSingleObject(g_pstMsgCB[ulModuleID].hSemaphore, INFINITE);

        /* ռ����Ϣ���д������ */
        WaitForSingleObject(g_pstMsgCB[ulModuleID].hMutex, INFINITE);

        /* ȡ����ǰ����Ϣ */
        pstMsg = g_pstMsgCB[ulModuleID].pstMsgQueue;
        g_pstMsgCB[ulModuleID].pstMsgQueue = pstMsg->pstNext;
        g_pstMsgCB[ulModuleID].pfnMsgProc(pstMsg->ulSrcModuleID,
                                          pstMsg->pMsg);
        free(pstMsg->pMsg);
        free(pstMsg);

        /* �ͷ���Ϣ���д������ */
        ReleaseMutex(g_pstMsgCB[ulModuleID].hMutex);
    }

    return SUCCESS;
}

/* ģ���ʼ�� */
ULONG SYS_InitProc()
{
    ULONG ulLoop;
    ULONG ulRet;
    DWORD dwThreadID;

    g_pstMsgCB = (SYS_MODULE_CB_S *)malloc(SYS_MODULE_BUTT * sizeof(SYS_MODULE_CB_S));
    memset(g_pstMsgCB, 0, SYS_MODULE_BUTT * sizeof(SYS_MODULE_CB_S));

    /* ��ģ���ʼ�� */
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

    /* ��ģ����Ϣ�������� */
    for(ulLoop = 0; ulLoop < SYS_MODULE_BUTT; ulLoop++)
    {
        if (g_astModuleCfg[ulLoop].pfnMsgProc == NULL)
        {
            /* û����Ϣ������������ */
            continue;
        }
        g_pstMsgCB[ulLoop].pfnMsgProc = g_astModuleCfg[ulLoop].pfnMsgProc;

        /* ÿ����Ϣ�������100����Ϣδ���� */
        g_pstMsgCB[ulLoop].hSemaphore = CreateSemaphore(NULL,0,100,NULL);

        /* ÿ����Ϣ�������100����Ϣδ���� */
        g_pstMsgCB[ulLoop].hMutex     = CreateMutex(NULL, FALSE, NULL);

        /*��ÿ����Ҫ��Ϣ�����ģ�齨����Ϣ�����߳�*/
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

/* ��������ַ��� */
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