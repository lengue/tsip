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
#include "s_hash\s_hash.h"

/* ��ģ������ṩ�ĳ����ͽṹͷ�ļ� */
#include "..\include\dialog\sip_dlg.h"

/* ��ģ���ڲ�ʹ�õĳ����ͽṹͷ�ļ� */
#include "sip_dlg_const.inc"
#include "sip_dlg_type.inc"

/* ��ģ���ڲ���������ͷ�ļ� */
#include "sip_dlg.inc"

/* ��ģ��ȫ�ֱ�������ͷ�ļ� */
#include "sip_dlg_var.inc"

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

ULONG SIP_Dlg_InitDialog(ULONG ulDlgID)
{
    SIP_DLG_CB_S *pstSipDlgCB = NULL_PTR;

    pstSipDlgCB = &g_pstSipDlgCB[ulDlgID];

    pstSipDlgCB->eSessionState = SIP_DLG_SESSION_STATE_BUTT;
    pstSipDlgCB->pstDialogID = NULL_PTR;
    pstSipDlgCB->pstLocalURI = NULL_PTR;
    pstSipDlgCB->pstRemoteURI = NULL_PTR;
    pstSipDlgCB->pstRemoteTarget = NULL_PTR;
    pstSipDlgCB->pastRouteSet = NULL_PTR;
    pstSipDlgCB->ulLocalSeq = NULL_ULONG;
    pstSipDlgCB->ulRemoteSeq = NULL_ULONG;
    pstSipDlgCB->ucSecureFlag = FALSE;
    pstSipDlgCB->pstHashNode = NULL_PTR;

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

    SIP_Dlg_InitDialog(ulDlgID);

    pstSipDlgCB = &g_pstSipDlgCB[ulDlgID];
    pstSipDlgCB->pstDialogID = UBUF_AllocUBuf(SIP_DLG_UBUF_LEN);
    pstDialogID = (SIP_DIALOG_ID_S *)UBUF_AddComponent(pstSipDlgCB->pstDialogID, sizeof(SIP_DIALOG_ID_S));
    pstDialogID->pucCallID    = NULL_PTR;
    pstDialogID->pucLocalTag  = NULL_PTR;
    pstDialogID->pucRemoteTag = NULL_PTR;
    
    /* ��¼�Ի���Ϣ*/
    UBUF_CLONE_STRING(pucCallID, 
                      pstSipDlgCB->pstDialogID,
                      pstDialogID->pucCallID);

    UBUF_CLONE_STRING(pucLocalTag, 
                      pstSipDlgCB->pstDialogID,
                      pstDialogID->pucLocalTag);

    UBUF_CLONE_STRING(pucRemoteTag, 
                      pstSipDlgCB->pstDialogID,
                      pstDialogID->pucRemoteTag);

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

ULONG SIP_Dlg_UpdateSessionState(ULONG ulDlgID, SIP_DLG_SESSION_STATE_E eState)
{
    SIP_DLG_CB_S *pstSipDlgCB = NULL_PTR;

    pstSipDlgCB = &g_pstSipDlgCB[ulDlgID];
    pstSipDlgCB->eSessionState = eState;

    return SUCCESS;
}

ULONG SIP_Dlg_GetSessionState(ULONG ulDlgID, SIP_DLG_SESSION_STATE_E *peState)
{
    SIP_DLG_CB_S *pstSipDlgCB = NULL_PTR;

    pstSipDlgCB = &g_pstSipDlgCB[ulDlgID];
    *peState = pstSipDlgCB->eSessionState;

    return SUCCESS;
}

/* ����һ���Ի� */
ULONG SIP_Dlg_GenerateRequest(ULONG ulDlgIndex, UBUF_HEADER_S *pstUbufSipMsg)
{
    ULONG ulRuleIndex;
    SIP_DLG_CB_S         *pstSipDlgCB     = NULL_PTR;
    SIP_DIALOG_ID_S      *pstDialogID     = NULL_PTR;
    SIP_MSG_S            *pstSipMsg       = NULL_PTR;
    SIP_HEADER_CALL_ID_S *pstHeaderCallID = NULL_PTR;
    SIP_HEADER_FROM_S    *pstHeaderFrom   = NULL_PTR;
    SIP_HEADER_TO_S      *pstHeaderTo     = NULL_PTR;
    SIP_HEADER_CSEQ_S    *pstHeaderCseq   = NULL_PTR;
    URI_S                *pstUri          = NULL_PTR;
    
    pstSipDlgCB = &g_pstSipDlgCB[ulDlgIndex];
    pstDialogID = (SIP_DIALOG_ID_S *)UBUF_GET_MSG_PTR(pstSipDlgCB->pstDialogID);
    pstSipMsg = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstUbufSipMsg);
    
    SIP_GetRuleIndex("addr-spec", &ulRuleIndex);

    /* Request URI���*/
    pstUri = (URI_S *)UBUF_GET_MSG_PTR(pstSipDlgCB->pstRemoteTarget);
    SIP_Clone(ulRuleIndex,
              pstUri,
              pstUbufSipMsg,
             &pstSipMsg->uStartLine.stRequstLine.pstRequestURI);
    
    /* CallID��� */
    pstHeaderCallID = UBUF_AddComponent(pstUbufSipMsg, sizeof(SIP_HEADER_CALL_ID_S));
    memset(pstHeaderCallID, 0, sizeof(SIP_HEADER_CALL_ID_S));
    pstSipMsg->apstHeaders[SIP_HEADER_CALL_ID] = (SIP_HEADER_S *)pstHeaderCallID;
    pstHeaderCallID->stHeader.pstNext = NULL_PTR;

    UBUF_CLONE_STRING(pstDialogID->pucCallID, 
                      pstUbufSipMsg,
                      pstHeaderCallID->pucCallID);
        
    /* Fromͷ����� */
    pstHeaderFrom = UBUF_AddComponent(pstUbufSipMsg, sizeof(SIP_HEADER_FROM_S));
    memset(pstHeaderFrom, 0, sizeof(SIP_HEADER_FROM_S));
    pstSipMsg->apstHeaders[SIP_HEADER_FROM] = (SIP_HEADER_S *)pstHeaderFrom;
    pstHeaderFrom->stHeader.pstNext = NULL_PTR;

    /*tag*/
    UBUF_CLONE_STRING(pstDialogID->pucLocalTag, 
                      pstUbufSipMsg,
                      pstHeaderFrom->pucTag);
    /*uri*/
    pstHeaderFrom->stNameAddr.bName = TRUE;
    pstHeaderFrom->stNameAddr.pucName = NULL_PTR;
    
    pstUri = (URI_S *)UBUF_GET_MSG_PTR(pstSipDlgCB->pstLocalURI);
    SIP_Clone(ulRuleIndex,
              pstUri,
              pstUbufSipMsg,
             &pstHeaderFrom->stNameAddr.pstUri);
    
    /* Toͷ�����*/
    pstHeaderTo= UBUF_AddComponent(pstUbufSipMsg, sizeof(SIP_HEADER_TO_S));
    memset(pstHeaderTo, 0, sizeof(SIP_HEADER_TO_S));
    pstSipMsg->apstHeaders[SIP_HEADER_TO] = (SIP_HEADER_S *)pstHeaderTo;
    pstHeaderTo->stHeader.pstNext = NULL_PTR;
    
    /*tag*/
    UBUF_CLONE_STRING(pstDialogID->pucRemoteTag, 
                      pstUbufSipMsg,
                      pstHeaderTo->pucTag);

    /*uri*/
    pstHeaderTo->stNameAddr.bName = TRUE;
    pstHeaderTo->stNameAddr.pucName = NULL_PTR;
    pstUri = (URI_S *)UBUF_GET_MSG_PTR(pstSipDlgCB->pstRemoteURI);
    SIP_Clone(ulRuleIndex,
              pstUri,
              pstUbufSipMsg,
             &pstHeaderTo->stNameAddr.pstUri);
    
    /* Cseq��� */
    pstHeaderCseq = UBUF_AddComponent(pstUbufSipMsg, sizeof(SIP_HEADER_CSEQ_S));
    memset(pstHeaderCseq, 0, sizeof(SIP_HEADER_CSEQ_S));
    pstSipMsg->apstHeaders[SIP_HEADER_CSEQ] = (SIP_HEADER_S *)pstHeaderCseq;
    pstHeaderCseq->stHeader.pstNext = NULL_PTR;

    if (pstSipDlgCB->ulLocalSeq == NULL_ULONG)
    {
        pstSipDlgCB->ulLocalSeq = 0;
    }
    else
    {
        pstSipDlgCB->ulLocalSeq++;
    }
    pstHeaderCseq->eMethod = pstSipMsg->uStartLine.stRequstLine.eMethod;
    pstHeaderCseq->ulSeq = pstSipDlgCB->ulLocalSeq;

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
    SIP_DLG_CB_S     *pstSipDlgCB    = NULL_PTR;
    SIP_DIALOG_ID_S  *pstDialogID    = NULL_PTR;
    SIP_DIALOG_ID_S  *pstCmpDialogID = NULL_PTR;

    pstCmpDialogID = (SIP_DIALOG_ID_S *)pCompared;

    pstSipDlgCB   = &g_pstSipDlgCB[ulPara];
    pstDialogID   = (SIP_DIALOG_ID_S *)UBUF_GET_MSG_PTR(pstSipDlgCB->pstDialogID);
    
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
    
    ulDlgID = HASH_Find(g_pstSipDlgHash, &stDialogID);
    if (ulDlgID == NULL_ULONG)
    {
        return FAIL;
    }
    
    *pulDlgIndex = ulDlgID;
    return SUCCESS;
}

/*  �ж϶Ի��Ƿ����ɾ�� */
ULONG SIP_Dlg_CanDialogRelease(ULONG ulDlgID)
{
    SIP_DLG_CB_S *pstSipDlgCB = NULL_PTR;

    pstSipDlgCB = &g_pstSipDlgCB[ulDlgID];

    if(pstSipDlgCB->eSessionState == SIP_DLG_SESSION_STATE_BUTT)
    {
        return SUCCESS;        
    }

    return FAIL;
}

/* ӳ��Э��ջ�Ի�ID��Ӧ�ò�ID */
ULONG SIP_Dlg_IDMap(ULONG ulDlgID, ULONG ulAppDlgID)
{
    SIP_DLG_CB_S *pstSipDlgCB = NULL_PTR;
    
    pstSipDlgCB = &g_pstSipDlgCB[ulDlgID];
    pstSipDlgCB->ulAppDlgID = ulAppDlgID;

    return SUCCESS;
}
