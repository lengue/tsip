#include <libxml/parser.h>
#include <stdio.h>
#include <string.h>
#include "vsp.h"
#include "sip_cp_const.inc"
#include "sip_cp_type.inc"
#include "sip_cp_func.inc"
#include "sip_cp_slstring.inc"
#include "sip_cp_slparse.inc"
#include "sip_cp_var.inc"

/*xml解析*/
ULONG SIPCP_ParseSL(xmlDocPtr doc)
{
    xmlNodePtr pstNodeSL    = NULL_PTR;
    xmlNodePtr pstNodeHSID  = NULL_PTR;
    xmlChar   *szKey        = NULL_PTR;
    ULONG      ulRet        = NULL_PTR;

    //确定文档根元素
    pstNodeSL = xmlDocGetRootElement(doc);
    if (NULL_PTR == pstNodeSL)
    {
        return FAIL;
    }

    if (xmlStrcmp(pstNodeSL->name, BAD_CAST "ServiceLogic"))
    {
        printf("\r\nroot node err at line%d",pstNodeSL->line);
        return FAIL;
    }

    szKey = xmlGetProp(pstNodeSL, BAD_CAST"version");
    //查找带有属性version的节点
    if (szKey == NULL_PTR)
    {
        printf("\r\nServiceLogic node has no version attr");
        return FAIL;
    }
    else
    {
        printf("\r\nService version:%s",szKey);
        g_pstSlMapping[g_usCurrSlTbl].ucIsUsed = TRUE;
        g_pstSlMapping[g_usCurrSlTbl].ucTotal = 0;
        g_pstSlMapping[g_usCurrSlTbl].usFirstIndex = g_usCurrSlItem;
        strncpy(g_pstSlMapping[g_usCurrSlTbl].aucVersion,
                szKey,
                SIPCP_SL_VERSION_LEN - 1);
    }

    pstNodeHSID = pstNodeSL->children;
    while (pstNodeHSID!= NULL_PTR)
    {
        if(xmlStrcmp(pstNodeHSID->name, BAD_CAST "HomeSID"))
        {
            printf("\r\nServiceLogic err node %s at line %d",
                    pstNodeHSID->name,
                    pstNodeHSID->line);
            return FAIL;
        }

        ulRet = SIPCP_ParseHSID(pstNodeHSID);
        if(ulRet != SUCCESS)
        {
            return ulRet;
        }
        pstNodeHSID = pstNodeHSID->next;
    }

    return SUCCESS;
}


ULONG SIPCP_ParseHSID(xmlNodePtr pstNodeHSID)
{
    xmlNodePtr pstNodeLegState;
    xmlChar   *szKey        = NULL_PTR;
    ULONG      ulRet        = NULL_PTR;
    UCHAR      ucEnum;

    szKey = xmlGetProp(pstNodeHSID, BAD_CAST"name");
    if (szKey == NULL_PTR)
    {
        printf("\r\nHomeSID node has no name attr at line%d",pstNodeHSID->line);
        return FAIL;
    }

    ulRet = SIPCP_GetSIDEnumFromString(szKey, &ucEnum);
    if (ulRet != SUCCESS)
    {
        printf("\r\nHomeSID node has no name value %s at line%d",szKey,pstNodeHSID->line);
        return ulRet;
    }
    else
    {
        printf("\r\n  HomeSID name:%s",szKey);
        g_pstSlStore[g_usCurrSlItem].eHSID = ucEnum;
    }

    pstNodeLegState = pstNodeHSID->children;
    while (pstNodeLegState != NULL_PTR)
    {
        if(xmlStrcmp(pstNodeLegState->name, BAD_CAST "LegState"))
        {
            printf("\r\nHomeSID err node %s at line %d",
                    pstNodeLegState->name,
                    pstNodeLegState->line);
            return FAIL;
        }

        ulRet = SIPCP_ParseLegState(pstNodeLegState);
        if(ulRet != SUCCESS)
        {
            return ulRet;
        }

        pstNodeLegState = pstNodeLegState->next;
    }

    return SUCCESS;
}


ULONG SIPCP_ParseLegState(xmlNodePtr pstNodeLegState)
{
    xmlNodePtr pstNodeEvent;
    xmlChar   *szOLS        = NULL_PTR;
    xmlChar   *szICLS       = NULL_PTR;
    ULONG      ulRet        = NULL_PTR;
    UCHAR      ucOLS;
    UCHAR      ucICLS;

    szOLS = xmlGetProp(pstNodeLegState, BAD_CAST"OLS");
    if (szOLS == NULL_PTR)
    {
        printf("\r\nLegState node has no OLS attr at line%d",pstNodeLegState->line);
        return FAIL;
    }

    szICLS = xmlGetProp(pstNodeLegState, BAD_CAST"ICLS");
    if (szICLS == NULL_PTR)
    {
        printf("\r\nLegState node has no ICLS attr at line%d",pstNodeLegState->line);
        return FAIL;
    }

    ulRet = SIPCP_GetLegStateEnumFromString(szOLS, &ucOLS);
    if (ulRet != SUCCESS)
    {
        printf("\r\nHomeSID node has err OLS value %s at line%d",szOLS,pstNodeLegState->line);
        return ulRet;
    }

    ulRet = SIPCP_GetLegsStateEnumFromString(szICLS, &ucICLS);
    if (ulRet != SUCCESS)
    {
        printf("\r\nHomeSID node has err ICLS value %s at line%d",szICLS,pstNodeLegState->line);
        return ulRet;
    }

    g_pstSlStore[g_usCurrSlItem].eOLS = ucOLS;
    g_pstSlStore[g_usCurrSlItem].eCLS = ucICLS;
    printf("\r\n    LegState OLS:%s ICLS:%s",szOLS,szICLS);

    pstNodeEvent = pstNodeLegState->children;
    while (pstNodeEvent!= NULL_PTR)
    {
        if (xmlStrcmp(pstNodeEvent->name, BAD_CAST "Event"))
        {
            printf("\r\nHomeSID err node %s at line %d",
                    pstNodeEvent->name,
                    pstNodeEvent->line);
            return FAIL;
        }

        ulRet = SIPCP_ParseEvent(pstNodeEvent);
        if(ulRet != SUCCESS)
        {
            return ulRet;
        }

        g_usCurrSlItem++;
        g_pstSlMapping[g_usCurrSlTbl].ucTotal++;
        g_pstSlStore[g_usCurrSlItem].eHSID = g_pstSlStore[g_usCurrSlItem-1].eHSID;
        g_pstSlStore[g_usCurrSlItem].eOLS = ucOLS;
        g_pstSlStore[g_usCurrSlItem].eCLS = ucICLS;
        pstNodeEvent = pstNodeEvent->next;
    }

    return SUCCESS;
}

ULONG SIPCP_ParseEvent(xmlNodePtr pstNodeEvent)
{
	xmlNodePtr pstNodeSlItem;
	xmlChar   *szKey      = NULL_PTR;
    ULONG      ulRet        = NULL_ULONG;
    UCHAR      ucEnum;

    szKey = xmlGetProp(pstNodeEvent, BAD_CAST"name");
    if (szKey == NULL_PTR)
    {
        printf("\r\nEvent node has no name attr at line%d",pstNodeEvent->line);
        return FAIL;
    }

    ulRet = SIPCP_GetEventEnumFromString(szKey, &ucEnum);
    if (ulRet != SUCCESS)
    {
        printf("\r\nEvent node has err name value %s at line%d",szKey,pstNodeEvent->line);
        return ulRet;
    }

    /*解析事件节点*/
    ulRet = SIPCP_ParseEventNode(ucEnum,pstNodeEvent);
    if (ulRet != SUCCESS)
    {
        return ulRet;
    }

    printf("\r\n      Event:%s",szKey);

    /*解析业务逻辑项节点*/
    pstNodeSlItem = pstNodeEvent->children;
    while(pstNodeSlItem != NULL_PTR)
    {
        ulRet = SIPCP_ParseSLItem(pstNodeSlItem);
        if (ulRet != SUCCESS)
        {
            return ulRet;
        }

        pstNodeSlItem = pstNodeSlItem->next;
    }

    return SUCCESS;
}


ULONG SIPCP_ParseEventNode(SIPCP_EVENT_E eEvent,xmlNodePtr pstNodeEvent)
{
    xmlChar *szKey;
    UCHAR    ucEnum;
    ULONG    ulRet  = NULL_ULONG;

    switch(eEvent)
    {
        /*没有参数的事件*/
        case SIPCP_EVENT_OFFHOOK:
        case SIPCP_EVENT_ONHOOK:
        case SIPCP_EVENT_HOOK:
        case SIPCP_EVENT_SOC0:
        case SIPCP_EVENT_SOC1:
        case SIPCP_EVENT_SOC2:
        case SIPCP_EVENT_SOC3:
        case SIPCP_EVENT_SOC4:
        case SIPCP_EVENT_SOC5:
        case SIPCP_EVENT_SOC6:
        case SIPCP_EVENT_SOC7:
        case SIPCP_EVENT_SOC8:
        case SIPCP_EVENT_SOC9:
        case SIPCP_EVENT_SOCE:
        case SIPCP_EVENT_SOCF:
        case SIPCP_EVENT_SOCX:
        case SIPCP_EVENT_SCC:
        case SIPCP_EVENT_INIT:
        case SIPCP_EVENT_DIAL_FAIL:
            break;

        /*定时器*/
        case SIPCP_EVENT_SRV_TIMEOUT:
            szKey = xmlGetProp(pstNodeEvent, BAD_CAST"TimerName");
            if (szKey == NULL_PTR)
            {
                printf("\r\nTimerName attribute missed at line%d",pstNodeEvent->line);
                return FAIL;
            }

            ulRet = SIPCP_GetTimerNameEnumFromString(szKey, &ucEnum);
            if (ulRet != SUCCESS)
            {
                printf("\r\nTimerName value error at line%d",pstNodeEvent->line);
                return ulRet;
            }
            g_pstSlStore[g_usCurrSlItem].stSlCore.stEvent.aucParaList[0] = ucEnum;
            break;

        case SIPCP_EVENT_SIGNAL_COMP:
            szKey = xmlGetProp(pstNodeEvent, BAD_CAST"SignalType");
            if (szKey == NULL_PTR)
            {
                printf("\r\nSignalType attribute missed at line%d",pstNodeEvent->line);
                return FAIL;
            }

            ulRet = SIPCP_GetSignalTypeEnumFromString(szKey, &ucEnum);
            if (ulRet != SUCCESS)
            {
                printf("\r\nSignalType value error at line%d",pstNodeEvent->line);
                return ulRet;
            }

            g_pstSlStore[g_usCurrSlItem].stSlCore.stEvent.aucParaList[0] = ucEnum;
            break;

        /*网络事件*/
        case SIPCP_EVENT_LEG_PEER_ALERT:
        case SIPCP_EVENT_LEG_CREATE_SUCCESS:
        case SIPCP_EVENT_LEG_RELEASE:
        case SIPCP_EVENT_LEG_NEGO_SUCCESS:
        case SIPCP_EVENT_LEG_NEGO_FAIL:
        case SIPCP_EVENT_LEG_REFER_SUCCESS:
        case SIPCP_EVENT_LEG_REFER_FAIL:
        case SIPCP_EVENT_LEG_CREATE_IND:
            szKey = xmlGetProp(pstNodeEvent, BAD_CAST"LegType");
            if (szKey == NULL_PTR)
            {
                printf("\r\nLegType attribute missed at line%d",pstNodeEvent->line);
                return FAIL;
            }

            ulRet = SIPCP_GetLegTypeEnumFromString(szKey, &ucEnum);
            if (ulRet != SUCCESS)
            {
                printf("\r\nLegType value error at line%d",pstNodeEvent->line);
                return ulRet;
            }

            g_pstSlStore[g_usCurrSlItem].stSlCore.stEvent.aucParaList[0] = ucEnum;

            szKey = xmlGetProp(pstNodeEvent, BAD_CAST"CLegType");
            if (szKey == NULL_PTR)
            {
                printf("\r\nCLegType attribute missed at line%d",pstNodeEvent->line);
                return FAIL;
            }

            ulRet = SIPCP_GetCLegTypeEnumFromString(szKey, &ucEnum);
            if (ulRet != SUCCESS)
            {
                printf("\r\nCLegType value error at line%d",pstNodeEvent->line);
                return ulRet;
            }

            g_pstSlStore[g_usCurrSlItem].stSlCore.stEvent.aucParaList[1] = ucEnum;
            break;

        default:
            printf("\r\nEvent node has err enum value %d at line%d",
                   eEvent,
                   pstNodeEvent->line);
            return FAIL;
    }

    szKey = xmlGetProp(pstNodeEvent, BAD_CAST"FailToDo");
    if (szKey != NULL_PTR)
    {
        ulRet = SIPCP_GetEventEnumFromString(szKey, &ucEnum);
        if(ulRet != SUCCESS)
        {
            printf("\r\nFailToDo value error at line%d",pstNodeEvent->line);
            return FAIL;
        }
        g_pstSlStore[g_usCurrSlItem].stSlCore.stEvent.eFailEvent = eEvent;
    }
    g_pstSlStore[g_usCurrSlItem].stSlCore.stEvent.eEvent = eEvent;
    return SUCCESS;
}


ULONG SIPCP_ParseSLItem(xmlNodePtr pstNodeSLItem)
{
    ULONG ulRet;

    if (!xmlStrcmp(pstNodeSLItem->name, BAD_CAST "ConditionList"))
    {
        ulRet = SIPCP_ParseConditionList(pstNodeSLItem);
    }
    else if (!xmlStrcmp(pstNodeSLItem->name, BAD_CAST "SpecificFlagList"))
    {
        ulRet = SIPCP_ParseSpecificFlagList(pstNodeSLItem);
    }
    else if (!xmlStrcmp(pstNodeSLItem->name, BAD_CAST "ActionList"))
    {
        ulRet = SIPCP_ParseActionList(pstNodeSLItem);
    }
    else if (!xmlStrcmp(pstNodeSLItem->name, BAD_CAST "ServiceSupportList"))
    {
        ulRet = SIPCP_ParseServiceSupportList(pstNodeSLItem);
    }
    else if (!xmlStrcmp(pstNodeSLItem->name, BAD_CAST "NextHSID"))
    {
        ulRet = SIPCP_ParseNextHSID(pstNodeSLItem);
    }
    else
    {
        printf("\r\nError SLItem:%s line%d",
                pstNodeSLItem->name,
                pstNodeSLItem->line);
        return FAIL;
    }

    return ulRet;
}

ULONG SIPCP_ParseConditionList(xmlNodePtr pstNodeConditionList)
{
	xmlNodePtr pstNodeCondition;
	xmlChar   *szKey      = NULL_PTR;
    ULONG      ulRet        = NULL_ULONG;
    ULONG      ulMask;
    UCHAR      ucEnum;

    pstNodeCondition = pstNodeConditionList->children;
    while (pstNodeCondition != NULL_PTR)
    {
        if(xmlStrcmp(pstNodeCondition->name, BAD_CAST "Condition"))
        {
            printf("\r\nCondition name error at line%d",pstNodeCondition->line);
            return FAIL;
        }

        szKey = xmlGetProp(pstNodeCondition, BAD_CAST"name");
        if (szKey == NULL_PTR)
        {
            printf("\r\nname attr missed at line%d",pstNodeCondition->line);
            return FAIL;
        }

        ulRet = SIPCP_GetConditionEnumFromString(szKey, &ucEnum);
        if (ulRet != SUCCESS)
        {
            printf("\r\nname attr value err at line%d",pstNodeCondition->line);
            return FAIL;
        }

        ulRet = SIPCP_GetConditionMaskFromEnum(ucEnum, &ulMask);
        if (ulRet != SUCCESS)
        {
            printf("\r\nname attr value err at line%d",pstNodeCondition->line);
            return FAIL;
        }

        g_pstSlStore[g_usCurrSlItem].stSlCore.ulCondition |= ulMask;
        pstNodeCondition = pstNodeCondition->next;
    }

    return SUCCESS;
}

ULONG SIPCP_ParseSpecificFlagList(xmlNodePtr pstNodeSpecificFlagList)
{
	xmlNodePtr pstNodeSpecFlag;
	xmlChar   *szKey      = NULL_PTR;
    ULONG      ulRet        = NULL_ULONG;
    ULONG      ulMask;
    UCHAR      ucEnum;

    pstNodeSpecFlag = pstNodeSpecificFlagList->children;
    while (pstNodeSpecFlag != NULL_PTR)
    {
        if(xmlStrcmp(pstNodeSpecFlag->name, BAD_CAST"Flag"))
        {
            printf("\r\nerror Flag node at line%d",pstNodeSpecFlag->line);
            return FAIL;
        }

        szKey = xmlGetProp(pstNodeSpecFlag, BAD_CAST"name");
        if (szKey == NULL_PTR)
        {
            printf("\r\nname attr missed at line%d",pstNodeSpecFlag->line);
            return FAIL;
        }

        ulRet = SIPCP_GetSpecFlagEnumFromString(szKey, &ucEnum);
        if (ulRet != SUCCESS)
        {
            printf("\r\nname attr value errr at line%d",pstNodeSpecFlag->line);
            return FAIL;
        }

        ulRet = SIPCP_GetSpecFlagMaskFromID(ucEnum, &ulMask);
        if (ulRet != SUCCESS)
        {
            printf("\r\nname attr value errr at line%d",pstNodeSpecFlag->line);
            return FAIL;
        }

        g_pstSlStore[g_usCurrSlItem].stSlCore.usFlagMask |= ulMask;
        pstNodeSpecFlag = pstNodeSpecFlag->next;
    }

    return SUCCESS;
}

ULONG SIPCP_ParseServiceSupportList(xmlNodePtr pstNodeServiceSupportList)
{
	xmlNodePtr pstNodeSupport;
	xmlChar   *szKey      = NULL_PTR;
    ULONG      ulRet      = NULL_ULONG;
    ULONG      ulMask;
    UCHAR      ucEnum;

    pstNodeSupport = pstNodeServiceSupportList->children;
    while (pstNodeSupport != NULL_PTR)
    {
        if(xmlStrcmp(pstNodeSupport->name, BAD_CAST"SID"))
        {
            printf("\r\nerror SID node at line%d",pstNodeSupport->line);
            return FAIL;
        }

        szKey = xmlGetProp(pstNodeSupport, BAD_CAST"name");
        if (szKey == NULL_PTR)
        {
            printf("\r\nname attr missed at line%d",pstNodeSupport->line);
            return FAIL;
        }

        ulRet = SIPCP_GetSIDEnumFromString(szKey, &ucEnum);
        if (ulRet != SUCCESS)
        {
            printf("\r\nname attr value error at line%d",pstNodeSupport->line);
            return FAIL;
        }

        ulRet = SIPCP_GetServiceMaskFromID(ucEnum, &ulMask);
        if (ulRet != SUCCESS)
        {
            printf("\r\nname attr value error at line%d",pstNodeSupport->line);
            return FAIL;
        }

        g_pstSlStore[g_usCurrSlItem].stSlCore.usSIDMask |= ulMask;
        pstNodeSupport = pstNodeSupport->next;
    }

    return SUCCESS;
}

ULONG SIPCP_ParseNextHSID(xmlNodePtr pstNodeNextHSID)
{
	xmlChar   *szKey      = NULL_PTR;
    ULONG      ulRet      = NULL_ULONG;
    UCHAR      ucEnum;

    szKey = xmlGetProp(pstNodeNextHSID, BAD_CAST"name");
    if (szKey == NULL_PTR)
    {
        printf("\r\nname attr missed at line%d",pstNodeNextHSID->line);
        return FAIL;
    }

    ulRet = SIPCP_GetSIDEnumFromString(szKey, &ucEnum);
    if (ulRet != SUCCESS)
    {
        printf("\r\nname attr value error at line%d",pstNodeNextHSID->line);
        return FAIL;
    }

    g_pstSlStore[g_usCurrSlItem].stSlCore.eNextHSID = ucEnum;
    return SUCCESS;
}


ULONG SIPCP_ParseActionList(xmlNodePtr pstNodeActionList)
{
	xmlNodePtr pstNodeAction;
    ULONG      ulRet        = NULL_ULONG;
    UCHAR      ucEnum;

    g_usCurrAction = 0;

    pstNodeAction = pstNodeActionList->children;
    while (pstNodeAction != NULL_PTR)
    {
        ulRet = SIPCP_GetActionEnumFromString(pstNodeAction->name, &ucEnum);
        if (ulRet != SUCCESS)
        {
            printf("\r\nerror Action node at line%d",pstNodeAction->line);
            return FAIL;
        }

        ulRet = SIPCP_ParseAction(ucEnum, pstNodeAction);
        if (ulRet != SUCCESS)
        {
            return FAIL;
        }

        g_usCurrAction++;
        pstNodeAction = pstNodeAction->next;
    }

    return SUCCESS;
}

ULONG SIPCP_ParseAction(SIPCP_ACTION_E eAction,xmlNodePtr pstNodeAction)
{
    ULONG ulRet = NULL_ULONG;

    g_pstSlStore[g_usCurrSlItem].stSlCore.astAction[g_usCurrAction].eAction = eAction;

    switch(eAction)
    {
        case SIPCP_ACTION_CREATE_LEG:
            ulRet = SIPCP_ParseActionPara_CreateLeg(pstNodeAction);
            break;

        case SIPCP_ACTION_CONFIRM_LEG:
            ulRet = SIPCP_ParseActionPara_ConfirmLeg(pstNodeAction);
            break;

        case SIPCP_ACTION_RELEASE_LEG:
            ulRet = SIPCP_ParseActionPara_ReleaseLeg(pstNodeAction);
            break;

        case SIPCP_ACTION_REFER_LEG:
            ulRet = SIPCP_ParseActionPara_ReferLeg(pstNodeAction);
            break;

        case SIPCP_ACTION_HOLD_LEG:
            ulRet = SIPCP_ParseActionPara_HoldLeg(pstNodeAction);
            break;

        case SIPCP_ACTION_RESUME_LEG:
            ulRet = SIPCP_ParseActionPara_ResumeLeg(pstNodeAction);
            break;

        case SIPCP_ACTION_DEACTIVE_LEG:
            ulRet = SIPCP_ParseActionPara_DeactiveLeg(pstNodeAction);
            break;

        case SIPCP_ACTION_ACTIVE_LEG:
            ulRet = SIPCP_ParseActionPara_ActiveLeg(pstNodeAction);
            break;

        case SIPCP_ACTION_START_TIMER:
            ulRet = SIPCP_ParseActionPara_StartTimer(pstNodeAction);
            break;

        case SIPCP_ACTION_STOP_TIMER:
            ulRet = SIPCP_ParseActionPara_StopTimer(pstNodeAction);
            break;

        case SIPCP_ACTION_START_SIGNAL:
            ulRet = SIPCP_ParseActionPara_StartSignal(pstNodeAction);
            break;

        case SIPCP_ACTION_SET_CURRENT_LEG:
            ulRet = SIPCP_ParseActionPara_SetCurrentLeg(pstNodeAction);
            break;

        default:
            printf("\r\nerror Action node at line%d",pstNodeAction->line);
            return FAIL;
    }

    return ulRet;
}
/**/
ULONG SIPCP_ParseActionPara_CreateLeg(xmlNodePtr pstNodeAction)
{
	xmlChar   *szKey      = NULL_PTR;
    ULONG      ulRet      = NULL_ULONG;
    UCHAR      ucEnum;

    szKey = xmlGetProp(pstNodeAction, BAD_CAST"LegType");
    if (szKey == NULL_PTR)
    {
        printf("\r\nLegType attr missed at line%d",pstNodeAction->line);
        return FAIL;
    }

    ulRet = SIPCP_GetLegTypeEnumFromString(szKey, &ucEnum);
    if (ulRet != SUCCESS)
    {
        printf("\r\nLegType attr value at line%d",pstNodeAction->line);
        return FAIL;
    }

    g_pstSlStore[g_usCurrSlItem].stSlCore.astAction[g_usCurrAction].aucParaList[0]
        = ucEnum;

    szKey = xmlGetProp(pstNodeAction, BAD_CAST"CreateMethod");
    if (szKey == NULL_PTR)
    {
        printf("\r\nCreateMethod attr missed at line%d",pstNodeAction->line);
        return FAIL;
    }

    ulRet = SIPCP_GetCreateMethodEnumFromString(szKey, &ucEnum);
    if (ulRet != SUCCESS)
    {
        printf("\r\nCreateMethod attr value at line%d",pstNodeAction->line);
        return FAIL;
    }

    g_pstSlStore[g_usCurrSlItem].stSlCore.astAction[g_usCurrAction].aucParaList[1]
        = ucEnum;
    return SUCCESS;
}

/**/
ULONG SIPCP_ParseActionPara_ConfirmLeg(xmlNodePtr pstNodeAction)
{
	xmlChar   *szKey      = NULL_PTR;
    ULONG      ulRet      = NULL_ULONG;
    UCHAR      ucEnum;

    szKey = xmlGetProp(pstNodeAction, BAD_CAST"LegType");
    if (szKey == NULL_PTR)
    {
        printf("\r\nLegType attr missed at line%d",pstNodeAction->line);
        return FAIL;
    }

    ulRet = SIPCP_GetLegTypeEnumFromString(szKey, &ucEnum);
    if (ulRet != SUCCESS)
    {
        printf("\r\nLegType attr value at line%d",pstNodeAction->line);
        return FAIL;
    }

    g_pstSlStore[g_usCurrSlItem].stSlCore.astAction[g_usCurrAction].aucParaList[0]
        = ucEnum;

    szKey = xmlGetProp(pstNodeAction, BAD_CAST"CLegType");
    if (szKey == NULL_PTR)
    {
        printf("\r\nCLegType attr missed at line%d",pstNodeAction->line);
        return FAIL;
    }

    ulRet = SIPCP_GetCLegTypeEnumFromString(szKey, &ucEnum);
    if (ulRet != SUCCESS)
    {
        printf("\r\nCLegType attr value at line%d",pstNodeAction->line);
        return FAIL;
    }

    g_pstSlStore[g_usCurrSlItem].stSlCore.astAction[g_usCurrAction].aucParaList[1]
        = ucEnum;

    szKey = xmlGetProp(pstNodeAction, BAD_CAST"Reason");
    if (szKey == NULL_PTR)
    {
        printf("\r\nReason attr missed at line%d",pstNodeAction->line);
        return FAIL;
    }

    g_pstSlStore[g_usCurrSlItem].stSlCore.astAction[g_usCurrAction].aucParaList[2]
        = ucEnum;
    return SUCCESS;
}


ULONG SIPCP_ParseActionPara_ReleaseLeg(xmlNodePtr pstNodeAction)
{
	xmlChar   *szKey      = NULL_PTR;
    ULONG      ulRet      = NULL_ULONG;
    UCHAR      ucEnum;

    szKey = xmlGetProp(pstNodeAction, BAD_CAST"LegType");
    if (szKey == NULL_PTR)
    {
        printf("\r\nLegType attr missed at line%d",pstNodeAction->line);
        return FAIL;
    }

    ulRet = SIPCP_GetLegTypeEnumFromString(szKey, &ucEnum);
    if (ulRet != SUCCESS)
    {
        printf("\r\nLegType attr value at line%d",pstNodeAction->line);
        return FAIL;
    }

    g_pstSlStore[g_usCurrSlItem].stSlCore.astAction[g_usCurrAction].aucParaList[0]
        = ucEnum;

    szKey = xmlGetProp(pstNodeAction, BAD_CAST"CLegType");
    if (szKey == NULL_PTR)
    {
        printf("\r\nCLegType attr missed at line%d",pstNodeAction->line);
        return FAIL;
    }

    ulRet = SIPCP_GetCLegTypeEnumFromString(szKey, &ucEnum);
    if (ulRet != SUCCESS)
    {
        printf("\r\nCLegType attr value at line%d",pstNodeAction->line);
        return FAIL;
    }

    g_pstSlStore[g_usCurrSlItem].stSlCore.astAction[g_usCurrAction].aucParaList[1]
        = ucEnum;

    szKey = xmlGetProp(pstNodeAction, BAD_CAST"Reason");
    if (szKey == NULL_PTR)
    {
        printf("\r\nReason attr missed at line%d",pstNodeAction->line);
        return FAIL;
    }

    g_pstSlStore[g_usCurrSlItem].stSlCore.astAction[g_usCurrAction].aucParaList[2]
        = ucEnum;

    return SUCCESS;
}

ULONG SIPCP_ParseActionPara_ReferLeg(xmlNodePtr pstNodeAction)
{
	xmlChar   *szKey      = NULL_PTR;
    ULONG      ulRet      = NULL_ULONG;
    UCHAR      ucEnum;

    szKey = xmlGetProp(pstNodeAction, BAD_CAST"ByLegType");
    if (szKey == NULL_PTR)
    {
        printf("\r\nByLegType attr missed at line%d",pstNodeAction->line);
        return FAIL;
    }

    ulRet = SIPCP_GetLegTypeEnumFromString(szKey, &ucEnum);
    if (ulRet != SUCCESS)
    {
        printf("\r\nByLegType attr value at line%d",pstNodeAction->line);
        return FAIL;
    }

    g_pstSlStore[g_usCurrSlItem].stSlCore.astAction[g_usCurrAction].aucParaList[0]
        = ucEnum;

    szKey = xmlGetProp(pstNodeAction, BAD_CAST"ByCLegType");
    if (szKey == NULL_PTR)
    {
        printf("\r\nByCLegType attr missed at line%d",pstNodeAction->line);
        return FAIL;
    }

    ulRet = SIPCP_GetCLegTypeEnumFromString(szKey, &ucEnum);
    if (ulRet != SUCCESS)
    {
        printf("\r\nByCLegType attr value at line%d",pstNodeAction->line);
        return FAIL;
    }

    g_pstSlStore[g_usCurrSlItem].stSlCore.astAction[g_usCurrAction].aucParaList[1]
        = ucEnum;

    szKey = xmlGetProp(pstNodeAction, BAD_CAST"ToLegType");
    if (szKey == NULL_PTR)
    {
        printf("\r\nToLegType attr missed at line%d",pstNodeAction->line);
        return FAIL;
    }

    ulRet = SIPCP_GetLegTypeEnumFromString(szKey, &ucEnum);
    if (ulRet != SUCCESS)
    {
        printf("\r\nToLegType attr value at line%d",pstNodeAction->line);
        return FAIL;
    }

    g_pstSlStore[g_usCurrSlItem].stSlCore.astAction[g_usCurrAction].aucParaList[2]
        = ucEnum;

    szKey = xmlGetProp(pstNodeAction, BAD_CAST"ToCLegType");
    if (szKey == NULL_PTR)
    {
        printf("\r\nToCLegType attr missed at line%d",pstNodeAction->line);
        return FAIL;
    }

    ulRet = SIPCP_GetCLegTypeEnumFromString(szKey, &ucEnum);
    if (ulRet != SUCCESS)
    {
        printf("\r\nToCLegType attr value at line%d",pstNodeAction->line);
        return FAIL;
    }

    g_pstSlStore[g_usCurrSlItem].stSlCore.astAction[g_usCurrAction].aucParaList[3]
        = ucEnum;

    return SUCCESS;
}

ULONG SIPCP_ParseActionPara_HoldLeg(xmlNodePtr pstNodeAction)
{
	xmlChar   *szKey      = NULL_PTR;
    ULONG      ulRet      = NULL_ULONG;
    UCHAR      ucEnum;

    szKey = xmlGetProp(pstNodeAction, BAD_CAST"LegType");
    if (szKey == NULL_PTR)
    {
        printf("\r\nLegType attr missed at line%d",pstNodeAction->line);
        return FAIL;
    }

    ulRet = SIPCP_GetLegTypeEnumFromString(szKey, &ucEnum);
    if (ulRet != SUCCESS)
    {
        printf("\r\nLegType attr value at line%d",pstNodeAction->line);
        return FAIL;
    }

    g_pstSlStore[g_usCurrSlItem].stSlCore.astAction[g_usCurrAction].aucParaList[0]
        = ucEnum;

    szKey = xmlGetProp(pstNodeAction, BAD_CAST"CLegType");
    if (szKey == NULL_PTR)
    {
        printf("\r\nCLegType attr missed at line%d",pstNodeAction->line);
        return FAIL;
    }

    ulRet = SIPCP_GetCLegTypeEnumFromString(szKey, &ucEnum);
    if (ulRet != SUCCESS)
    {
        printf("\r\nCLegType attr value at line%d",pstNodeAction->line);
        return FAIL;
    }

    g_pstSlStore[g_usCurrSlItem].stSlCore.astAction[g_usCurrAction].aucParaList[1]
        = ucEnum;

    return SUCCESS;
}

ULONG SIPCP_ParseActionPara_ResumeLeg(xmlNodePtr pstNodeAction)
{
	xmlChar   *szKey      = NULL_PTR;
    ULONG      ulRet      = NULL_ULONG;
    UCHAR      ucEnum;

    szKey = xmlGetProp(pstNodeAction, BAD_CAST"LegType");
    if (szKey == NULL_PTR)
    {
        printf("\r\nLegType attr missed at line%d",pstNodeAction->line);
        return FAIL;
    }

    ulRet = SIPCP_GetLegTypeEnumFromString(szKey, &ucEnum);
    if (ulRet != SUCCESS)
    {
        printf("\r\nLegType attr value at line%d",pstNodeAction->line);
        return FAIL;
    }

    g_pstSlStore[g_usCurrSlItem].stSlCore.astAction[g_usCurrAction].aucParaList[0]
        = ucEnum;

    szKey = xmlGetProp(pstNodeAction, BAD_CAST"CLegType");
    if (szKey == NULL_PTR)
    {
        printf("\r\nCLegType attr missed at line%d",pstNodeAction->line);
        return FAIL;
    }

    ulRet = SIPCP_GetCLegTypeEnumFromString(szKey, &ucEnum);
    if (ulRet != SUCCESS)
    {
        printf("\r\nCLegType attr value at line%d",pstNodeAction->line);
        return FAIL;
    }

    g_pstSlStore[g_usCurrSlItem].stSlCore.astAction[g_usCurrAction].aucParaList[1]
        = ucEnum;

    return SUCCESS;
}

ULONG SIPCP_ParseActionPara_SetCurrentLeg(xmlNodePtr pstNodeAction)
{
	xmlChar   *szKey      = NULL_PTR;
    ULONG      ulRet      = NULL_ULONG;
    UCHAR      ucEnum;

    szKey = xmlGetProp(pstNodeAction, BAD_CAST"CLegType");
    if (szKey == NULL_PTR)
    {
        printf("\r\nCLegType attr missed at line%d",pstNodeAction->line);
        return FAIL;
    }

    ulRet = SIPCP_GetCLegTypeEnumFromString(szKey, &ucEnum);
    if (ulRet != SUCCESS)
    {
        printf("\r\nCLegType attr value at line%d",pstNodeAction->line);
        return FAIL;
    }

    g_pstSlStore[g_usCurrSlItem].stSlCore.astAction[g_usCurrAction].aucParaList[0]
        = ucEnum;

    return SUCCESS;
}


ULONG SIPCP_ParseActionPara_ActiveLeg(xmlNodePtr pstNodeAction)
{
    xmlChar   *szKey      = NULL_PTR;
    ULONG      ulRet      = NULL_ULONG;
    UCHAR      ucEnum;

    szKey = xmlGetProp(pstNodeAction, BAD_CAST"LegType");
    if (szKey == NULL_PTR)
    {
        printf("\r\nLegType attr missed at line%d",pstNodeAction->line);
        return FAIL;
    }

    ulRet = SIPCP_GetLegTypeEnumFromString(szKey, &ucEnum);
    if (ulRet != SUCCESS)
    {
        printf("\r\nLegType attr value at line%d",pstNodeAction->line);
        return FAIL;
    }

    g_pstSlStore[g_usCurrSlItem].stSlCore.astAction[g_usCurrAction].aucParaList[0]
        = ucEnum;

    szKey = xmlGetProp(pstNodeAction, BAD_CAST"CLegType");
    if (szKey == NULL_PTR)
    {
        printf("\r\nCLegType attr missed at line%d",pstNodeAction->line);
        return FAIL;
    }

    ulRet = SIPCP_GetCLegTypeEnumFromString(szKey, &ucEnum);
    if (ulRet != SUCCESS)
    {
        printf("\r\nCLegType attr value at line%d",pstNodeAction->line);
        return FAIL;
    }

    g_pstSlStore[g_usCurrSlItem].stSlCore.astAction[g_usCurrAction].aucParaList[1]
        = ucEnum;

    szKey = xmlGetProp(pstNodeAction, BAD_CAST"ActiveMode");
    if (szKey == NULL_PTR)
    {
        printf("\r\nActiveMode attr missed at line%d",pstNodeAction->line);
        return FAIL;
    }

    ulRet = SIPCP_GetActiveModeEnumFromString(szKey, &ucEnum);
    if (ulRet != SUCCESS)
    {
        printf("\r\nActiveMode attr value at line%d",pstNodeAction->line);
        return FAIL;
    }

    g_pstSlStore[g_usCurrSlItem].stSlCore.astAction[g_usCurrAction].aucParaList[2]
        = ucEnum;

    return SUCCESS;
}

ULONG SIPCP_ParseActionPara_DeactiveLeg(xmlNodePtr pstNodeAction)
{
	xmlChar   *szKey      = NULL_PTR;
    ULONG      ulRet      = NULL_ULONG;
    UCHAR      ucEnum;

    szKey = xmlGetProp(pstNodeAction, BAD_CAST"LegType");
    if (szKey == NULL_PTR)
    {
        printf("\r\nLegType attr missed at line%d",pstNodeAction->line);
        return FAIL;
    }

    ulRet = SIPCP_GetLegTypeEnumFromString(szKey, &ucEnum);
    if (ulRet != SUCCESS)
    {
        printf("\r\nLegType attr value at line%d",pstNodeAction->line);
        return FAIL;
    }

    g_pstSlStore[g_usCurrSlItem].stSlCore.astAction[g_usCurrAction].aucParaList[0]
        = ucEnum;

    szKey = xmlGetProp(pstNodeAction, BAD_CAST"CLegType");
    if (szKey == NULL_PTR)
    {
        printf("\r\nCLegType attr missed at line%d",pstNodeAction->line);
        return FAIL;
    }

    ulRet = SIPCP_GetCLegTypeEnumFromString(szKey, &ucEnum);
    if (ulRet != SUCCESS)
    {
        printf("\r\nCLegType attr value at line%d",pstNodeAction->line);
        return FAIL;
    }

    g_pstSlStore[g_usCurrSlItem].stSlCore.astAction[g_usCurrAction].aucParaList[1]
        = ucEnum;

    return SUCCESS;
}

ULONG SIPCP_ParseActionPara_StartSignal(xmlNodePtr pstNodeAction)
{
	xmlChar   *szKey      = NULL_PTR;
    ULONG      ulRet      = NULL_ULONG;
    UCHAR      ucEnum;

    szKey = xmlGetProp(pstNodeAction, BAD_CAST"SignalType");
    if (szKey == NULL_PTR)
    {
        printf("\r\nSignalType attr missed at line%d",pstNodeAction->line);
        return FAIL;
    }

    ulRet = SIPCP_GetSignalTypeEnumFromString(szKey, &ucEnum);
    if (ulRet != SUCCESS)
    {
        printf("\r\nSignalType attr value at line%d",pstNodeAction->line);
        return FAIL;
    }

    g_pstSlStore[g_usCurrSlItem].stSlCore.astAction[g_usCurrAction].aucParaList[0]
        = ucEnum;

    szKey = xmlGetProp(pstNodeAction, BAD_CAST"PlayType");
    if (szKey == NULL_PTR)
    {
        printf("\r\nPlayType attr missed at line%d",pstNodeAction->line);
        return FAIL;
    }

    ulRet = SIPCP_GetPlayTypeEnumFromString(szKey, &ucEnum);
    if (ulRet != SUCCESS)
    {
        printf("\r\nPlayType attr value at line%d",pstNodeAction->line);
        return FAIL;
    }

    g_pstSlStore[g_usCurrSlItem].stSlCore.astAction[g_usCurrAction].aucParaList[1]
        = ucEnum;

    szKey = xmlGetProp(pstNodeAction, BAD_CAST"LegType");
    if (szKey != NULL_PTR)
    {
        ulRet = SIPCP_GetLegTypeEnumFromString(szKey, &ucEnum);
        if (ulRet != SUCCESS)
        {
            printf("\r\nLegType attr value at line%d",pstNodeAction->line);
            return FAIL;
        }

        g_pstSlStore[g_usCurrSlItem].stSlCore.astAction[g_usCurrAction].aucParaList[2]
            = ucEnum;

        szKey = xmlGetProp(pstNodeAction, BAD_CAST"CLegType");
        if (szKey == NULL_PTR)
        {
            printf("\r\nCLegType attr missed at line%d",pstNodeAction->line);
            return FAIL;
        }

        ulRet = SIPCP_GetCLegTypeEnumFromString(szKey, &ucEnum);
        if (ulRet != SUCCESS)
        {
            printf("\r\nCLegType attr value at line%d",pstNodeAction->line);
            return FAIL;
        }

        g_pstSlStore[g_usCurrSlItem].stSlCore.astAction[g_usCurrAction].aucParaList[3]
            = ucEnum;
    }
    ucEnum = ucEnum;
    return SUCCESS;
}

ULONG SIPCP_ParseActionPara_StartTimer(xmlNodePtr pstNodeAction)
{
    xmlChar   *szKey      = NULL_PTR;
    ULONG      ulRet      = NULL_ULONG;
    UCHAR      ucEnum;

    szKey = xmlGetProp(pstNodeAction, BAD_CAST"TimerName");
    if (szKey == NULL_PTR)
    {
        printf("\r\nTimerName attr missed at line%d",pstNodeAction->line);
        return FAIL;
    }

    ulRet = SIPCP_GetTimerNameEnumFromString(szKey, &ucEnum);
    if (ulRet != SUCCESS)
    {
        printf("\r\nTimerName attr value at line%d",pstNodeAction->line);
        return FAIL;
    }

    g_pstSlStore[g_usCurrSlItem].stSlCore.astAction[g_usCurrAction].aucParaList[0]
        = ucEnum;

    return SUCCESS;
}


ULONG SIPCP_ParseActionPara_StopTimer(xmlNodePtr pstNodeAction)
{
	xmlChar   *szKey      = NULL_PTR;
    ULONG      ulRet      = NULL_ULONG;
    UCHAR      ucEnum;

    szKey = xmlGetProp(pstNodeAction, BAD_CAST"TimerName");
    if (szKey == NULL_PTR)
    {
        printf("\r\nTimerName attr missed at line%d",pstNodeAction->line);
        return FAIL;
    }

    ulRet = SIPCP_GetTimerNameEnumFromString(szKey, &ucEnum);
    if (ulRet != SUCCESS)
    {
        printf("\r\nTimerName attr value at line%d",pstNodeAction->line);
        return FAIL;
    }

    g_pstSlStore[g_usCurrSlItem].stSlCore.astAction[g_usCurrAction].aucParaList[0]
        = ucEnum;

    return SUCCESS;
}
