/*******************************************************************************
����    : �������Ϣ����(WIN32ר��)
������  : �ƴ�ƽ
��������: 200.01.10
�޸ļ�¼:
*******************************************************************************/
/* �ⲿ����ģ��ͷ�ļ� */
#include "common\common.h"
#include "abnf\abnf.h"
#include "ubuf\ubuf.h"
#include "sip\uri.h"
#include "sip\sip.h"

/* ��ģ������ṩ�ĳ����ͽṹͷ�ļ� */

/* ��ģ���ڲ�ʹ�õĳ����ͽṹͷ�ļ� */

/* ��ģ���ڲ���������ͷ�ļ� */

/* ��ģ��ȫ�ֱ�������ͷ�ļ� */

/* �Ի�ģ���ʼ�� */
ULONG SIP_Dlg_Init()
{
    COMM_CB_INIT(SIP_DLG_MAX_CB, 
                 sizeof(SIP_DLG_CB_S), 
                 g_pstSipDlgCB, 
                 g_stSipDlgCBQueue);

    g_pstSipDlgHash = HASH_CreateTable(SIP_DLG_MAX_CB,
                                       SIP_Dlg_MakeKey,
                                       SIP_Dlg_Compare);

    return SUCCESS;
}
    
/* ����һ���Ի� */
ULONG SIP_Dlg_AllocDialog(UCHAR *pucCallID, 
                          UCHAR *pucLocalTag, 
                          UCHAR *pucRemoteTag,
                          ULONG *pulDlgIndex)
{
    ULONG ulDlgID;
    SIP_DLG_CB_S *pstSipDlgCB = NULL_PTR;
    SIP_DIALOG_ID_S *pstDialogID = NULL_PTR;
    
    /* ����һ���Ի����ƿ� */
    COMM_CB_ALLOC(g_pstSipDlgCB, g_stSipDlgCBQueue, ulDlgID);

    pstSipDlgCB = &g_pstSipDlgCB[ulDlgID];
    pstSipDlgCB->pstDialogID = UBUF_AllocUBuf(SIP_DLG_UBUF_LEN);
    pstDialogID = UBUF_GET_MSG_PTR(pstSipDlgCB->pstDialogID);
    
    /* ��¼�Ի���Ϣ*/
    UBUF_CLONE_STRING(pucCallID, 
                      pstSipDlgCB->pstDialogID,
                      pstDialogID->pucCallID);

    UBUF_CLONE_STRING(pucLocalTag, 
                      pstSipDlgCB->pstDialogID,
                      pstDialogID->pucCallID);

    UBUF_CLONE_STRING(pucRemoteTag, 
                      pstSipDlgCB->pstDialogID,
                      pstDialogID->pucCallID);

    /*���öԻ����뵽HASH������*/
    pstSipDlgCB->pstHashNode = HASH_AddNode(g_pstSipDlgHash, pstDialogID, ulDlgID);
    
    *pulDlgIndex = ulDlgID;
    return SUCCESS;
}

/* �ͷ�һ���Ի� */
ULONG SIP_Dlg_ReleaseDialog(ULONG ulDlgIndex)
{
    SIP_DLG_CB_S *pstSipDlgCB = NULL_PTR;

    pstSipDlgCB = &g_pstSipDlgCB[ulDlgIndex];
    
    HASH_DeleteNode(pstSipDlgCB->pstHashNode);
    
    UBUF_FreeBuffer(pstSipDlgCB->pstDialogID);
    UBUF_FreeBuffer(pstSipDlgCB->pstLocalURI);
    UBUF_FreeBuffer(pstSipDlgCB->pstRemoteURI);
    UBUF_FreeBuffer(pstSipDlgCB->pstRemoteTarget);
    UBUF_FreeBuffer(pstSipDlgCB->pastRouteSet);
    
    return SUCCESS;
}

ULONG SIP_Dlg_UpdateRemoteSeq(ULONG ulDlgID, ULONG ulRemoteSeq)
{
    SIP_DLG_CB_S *pstSipDlgCB = NULL_PTR;

    pstSipDlgCB = &g_pstSipDlgCB[ulDlgID];
    pstSipDlgCB->ulRemoteSeq = ulRemoteSeq;

    return SUCCESS;
}

ULONG SIP_Dlg_UpdateLocalSeq(ULONG ulDlgID, ULONG ulLocalSeq)
{
    SIP_DLG_CB_S *pstSipDlgCB = NULL_PTR;

    pstSipDlgCB = &g_pstSipDlgCB[ulDlgID];
    pstSipDlgCB->ulLocalSeq = ulLocalSeq;

    return SUCCESS;
}

ULONG SIP_Dlg_UpdateLocalURI(ULONG ulDlgID, URI_S *pstLocalURI)
{
    SIP_DLG_CB_S *pstSipDlgCB = NULL_PTR;
    ULONG         ulRuleIndex;
    URI_S        *pstUri = NULL_PTR;
    
    pstSipDlgCB = &g_pstSipDlgCB[ulDlgID];
    if (pstSipDlgCB->pstLocalURI != NULL_PTR)
    {
        UBUF_FreeBuffer(pstSipDlgCB->pstLocalURI);
    }

    pstSipDlgCB->pstLocalURI = UBUF_AllocUBuf(URI_MAX_LEN);

    SIP_GetRuleIndex("addr-spec", &ulRuleIndex);
    SIP_Clone(ulRuleIndex, pstLocalURI, pstSipDlgCB->pstLocalURI, &pstUri);

    return SUCCESS;
}

ULONG SIP_Dlg_UpdateRemoteURI(ULONG ulDlgID, URI_S *pstRemoteURI)
{
    SIP_DLG_CB_S *pstSipDlgCB = NULL_PTR;
    ULONG         ulRuleIndex;
    URI_S        *pstUri = NULL_PTR;
    
    pstSipDlgCB = &g_pstSipDlgCB[ulDlgID];
    if (pstSipDlgCB->pstRemoteURI != NULL_PTR)
    {
        UBUF_FreeBuffer(pstSipDlgCB->pstRemoteURI);
    }

    pstSipDlgCB->pstRemoteURI = UBUF_AllocUBuf(URI_MAX_LEN);

    SIP_GetRuleIndex("addr-spec", &ulRuleIndex);
    SIP_Clone(ulRuleIndex, pstRemoteURI, pstSipDlgCB->pstRemoteURI, &pstUri);

    return SUCCESS;
}

ULONG SIP_Dlg_UpdateRouteSet(ULONG ulDlgID, SIP_ROUTE_PARAM_S *pstRouteSet)
{
    SIP_DLG_CB_S *pstSipDlgCB = NULL_PTR;
    ULONG         ulRuleIndex;
    SIP_ROUTE_PARAM_S *pstRoute = NULL_PTR;
    
    pstSipDlgCB = &g_pstSipDlgCB[ulDlgID];
    if (pstSipDlgCB->pastRouteSet != NULL_PTR)
    {
        UBUF_FreeBuffer(pstSipDlgCB->pastRouteSet);
    }

    pstSipDlgCB->pastRouteSet = UBUF_AllocUBuf(URI_MAX_LEN);

    SIP_GetRuleIndex("route-param", &ulRuleIndex);
    SIP_Clone(ulRuleIndex, pstRouteSet, pstSipDlgCB->pastRouteSet, &pstRoute);

    return SUCCESS;
}

ULONG SIP_Dlg_UpdateRemoteTarget(ULONG ulDlgID, URI_S *pstRemoteTarget)
{
    SIP_DLG_CB_S *pstSipDlgCB = NULL_PTR;
    ULONG         ulRuleIndex;
    URI_S        *pstUri = NULL_PTR;
    
    pstSipDlgCB = &g_pstSipDlgCB[ulDlgID];
    if (pstSipDlgCB->pstRemoteTarget != NULL_PTR)
    {
        UBUF_FreeBuffer(pstSipDlgCB->pstRemoteTarget);
    }

    pstSipDlgCB->pstRemoteTarget = UBUF_AllocUBuf(URI_MAX_LEN);

    SIP_GetRuleIndex("addr-spec", &ulRuleIndex);
    SIP_Clone(ulRuleIndex, pstRemoteTarget, pstSipDlgCB->pstRemoteTarget, &pstUri);

    return SUCCESS;
}

ULONG SIP_Dlg_UpdateSecureFlag(ULONG ulDlgID, UCHAR ucSecureFlag)
{
    SIP_DLG_CB_S *pstSipDlgCB = NULL_PTR;

    pstSipDlgCB = &g_pstSipDlgCB[ulDlgID];
    pstSipDlgCB->ucSecureFlag = ucSecureFlag;

    return SUCCESS;
}

/* ����һ���Ի� */
ULONG SIP_Dlg_GenerateRequest(ULONG ulDlgIndex, UBUF_HEADER_S *pstUbufSipMsg)
{
    ULONG ulLen;
    ULONG ulRuleIndex;
    SIP_DLG_CB_S *pstSipDlgCB = NULL_PTR;
    SIP_DIALOG_S *pstDialog = NULL_PTR;
    SIP_MSG_S    *pstSipMsg = NULL_PTR;
    SIP_HEADER_CALL_ID_S *pstHeaderCallID = NULL_PTR;
    SIP_HEADER_FROM_S    *pstHeaderFrom = NULL_PTR;
    SIP_HEADER_TO_S      *pstHeaderTo = NULL_PTR;
    SIP_HEADER_CSEQ_S    *pstHeaderCseq = NULL_PTR;
    
    pstSipDlgCB = &g_pstSipDlgCB[ulDlgIndex];
    pstDialog = (SIP_DIALOG_S *)UBUF_GET_MSG_PTR(pstSipDlgCB->pstDialog);
    pstSipMsg = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUbufSipMsg);
    SIP_Syntax_GetRuleIndex("addr-spec", &ulRuleIndex);
        
    /* CallID��� */
    pstHeaderCallID = UBUF_AddComponent(pstUbufSipMsg, sizeof(SIP_HEADER_CALL_ID_S));
    pstSipMsg->apstHeaders[SIP_HEADER_CALL_ID] = pstHeaderCallID;
    pstHeaderCallID->stHeader.pstNext = NULL_PTR;

    UBUF_CLONE_STRING(pstDialog->pucCallID, 
                      pstUbufSipMsg,
                      pstHeaderCallID->pucCallID);
        
    /* Fromͷ����� */
    pstHeaderFrom = UBUF_AddComponent(pstUbufSipMsg, sizeof(SIP_HEADER_FROM_S));
    pstSipMsg->apstHeaders[SIP_HEADER_FROM] = pstHeaderFrom;
    pstHeaderFrom->stHeader.pstNext = NULL_PTR;

    /*tag*/
    UBUF_CLONE_STRING(pstDialog->pucLocalTag, 
                      pstUbufSipMsg,
                      pstHeaderFrom->pucTag);
    /*uri*/
    pstHeaderFrom->stNameAddr.bName = FALSE;
    pstHeaderFrom->stNameAddr.pucName = NULL_PTR;
    SIP_Syntax_Clone(ulRuleIndex,
                     pstDialog->pstLocalURI,
                     pstUbufSipMsg,
                    &pstHeaderFrom->stNameAddr.pstUri);
    
    /* Toͷ�����*/
    pstHeaderTo= UBUF_AddComponent(pstUbufSipMsg, sizeof(SIP_HEADER_TO_S));
    pstSipMsg->apstHeaders[SIP_HEADER_TO] = pstHeaderTo;
    pstHeaderTo->stHeader.pstNext = NULL_PTR;
    
    /*tag*/
    UBUF_CLONE_STRING(pstDialog->pucRemoteTag, 
                      pstUbufSipMsg,
                      pstHeaderTo->pucTag);

    /*uri*/
    pstHeaderTo->stNameAddr.bName = FALSE;
    pstHeaderTo->stNameAddr.pucName = NULL_PTR;
    SIP_Syntax_Clone(ulRuleIndex,
                     pstDialog->pstRemoteURI,
                     pstUbufSipMsg,
                    &pstHeaderTo->stNameAddr.pstUri);
    
    /* Cseq��� */
    pstHeaderCseq = UBUF_AddComponent(pstUbufSipMsg, sizeof(SIP_HEADER_CSEQ_S));
    pstSipMsg->apstHeaders[SIP_HEADER_CSEQ] = pstHeaderCseq;
    pstHeaderCseq->stHeader.pstNext = NULL_PTR;

    if (pstDialog->ulLocalSeq == NULL_ULONG)
    {
        pstDialog->ulLocalSeq = 0;
    }
    else
    {
        pstDialog->ulLocalSeq++;
    }
    pstHeaderCseq->eMethod = pstSipMsg->uStartLine.stRequstLine.eMethod;
    pstHeaderCseq->ulSeq = pstDialog->ulLocalSeq;

    /* ��·�����ͷ�� */

    return SUCCESS;
}

/* keyֵ���ɺ��� */
ULONG SIP_Dlg_MakeKey(void *pCompared)
{
    UCHAR *pucChar;
    ULONG  ulKey = 0;
    SIP_DIALOG_ID_S *pstDialogID = NULL_PTR;

    pstDialogID = (SIP_DIALOG_ID_S *)pCompared;
    pucChar     = pstDialogID->pucCallID;
    while(*pucChar != 0)
    {
        ulKey += *pucChar;
        pucChar++;
    }

    return ulKey;
}

/* �Ի��ȽϺ��� */
ULONG SIP_Dlg_Compare(void *pCompared, ULONG ulPara)
{
    ULONG ulRet;
    SIP_DLG_CB_S     *pstSipDlgCB    = NULL_PTR;
    SIP_DIALOG_ID_S  *pstDialogID    = NULL_PTR;
    SIP_DIALOG_ID_S  *pstCmpDialogID = NULL_PTR;

    pstCmpDialogID = (SIP_DIALOG_ID_S *)pCompared;

    pstSipDlgCB   = &g_pstSipDlgCB[ulPara];
    pstDialogID   = UBUF_GET_MSG_PTR(pstSipDlgCB->pstDialogID);
    
    if (strcmp(pstDialogID->pucCallID, pstCmpDialogID->pucCallID) != 0)
    {
        return FAIL;
    }

    if (strcmp(pstDialogID->pucRemoteTag, pstCmpDialogID->pucRemoteTag) != 0)
    {
        return FAIL;
    }

    if (strcmp(pstDialogID->pucLocalTag, pstCmpDialogID->pucLocalTag) != 0)
    {
        return FAIL;
    }

    return SUCCESS;
}


/* ƥ��Ի� */
ULONG SIP_Dlg_MatchDialog(UBUF_HEADER_S *pstUbufSipMsg, ULONG *pulDlgIndex)
{
    ULONG                 ulDlgID;
    SIP_DIALOG_ID_S       stDialogID;
    SIP_MSG_S            *pstSipMsg       = NULL_PTR;
    SIP_HEADER_CALL_ID_S *pstHeaderCallID = NULL_PTR;
    SIP_HEADER_FROM_S    *pstHeaderFrom   = NULL_PTR;
    SIP_HEADER_TO_S      *pstHeaderTo     = NULL_PTR;

    pstSipMsg       = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUbufSipMsg);
    
    pstHeaderCallID = (SIP_HEADER_CALL_ID_S *)pstSipMsg->apstHeaders[SIP_HEADER_CALL_ID];
    pstHeaderFrom   = (SIP_HEADER_FROM_S *)pstSipMsg->apstHeaders[SIP_HEADER_FROM];
    pstHeaderTo     = (SIP_HEADER_TO_S *)pstSipMsg->apstHeaders[SIP_HEADER_TO];
    
    stDialogID.pucCallID    = pstHeaderCallID->pucCallID;
    stDialogID.pucLocalTag  = pstHeaderTo->pucTag;
    stDialogID.pucRemoteTag = pstHeaderFrom->pucTag;
    
    ulDlgID = HASH_Find(SIP_DLG_MAX_CB, &stDialogID);
    if (ulDlgID == NULL_ULONG)
    {
        return FAIL;
    }
    
    *pulDlgIndex = ulDlgID;
    return SUCCESS;
}

