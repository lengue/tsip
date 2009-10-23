/*******************************************************************************
sip_cp_slstring.c
业务逻辑表字符串与枚举值对应关系及其获取函数
*******************************************************************************/
#include <stdio.h>
#include <libxml/parser.h>

#include "vsp.h"
#include "sip_cp_const.inc"
#include "sip_cp_type.inc"
#include "sip_cp_slstring.inc"

UCHAR *g_apucEventName[SIPCP_EVENT_BUTT] =
{
    /*物理事件*/
    "OffHook",       //SIPCP_EVENT_OFFHOOK
    "OnHook",        //SIPCP_EVENT_ONHOOK
    "Hook",          //SIPCP_EVENT_HOOK

    /*单个拨号事件*/
    "Soc0",          //SIPCP_EVENT_SOC0
    "Soc1",          //SIPCP_EVENT_SOC1
    "Soc2",          //SIPCP_EVENT_SOC2
    "Soc3",          //SIPCP_EVENT_SOC3
    "Soc4",          //SIPCP_EVENT_SOC4
    "Soc5",          //SIPCP_EVENT_SOC5
    "Soc6",          //SIPCP_EVENT_SOC6
    "Soc7",          //SIPCP_EVENT_SOC7
    "Soc8",          //SIPCP_EVENT_SOC8
    "Soc9",          //SIPCP_EVENT_SOC9
    "Soc*",          //SIPCP_EVENT_SOCE
    "Soc#",          //SIPCP_EVENT_SOCF
    "SocX",          //SIPCP_EVENT_SOCX

    /*数图事件*/
    "SccNumber",     //SIPCP_EVENT_SCC
    "NormalNumber",   //SIPCP_EVENT_INIT
    "RecvNumberFail",//SIPCP_EVENT_DIAL_FAIL

    /*内部事件*/
    "ServiceTimeout",//SIPCP_EVENT_SRV_TIMEOUT
    "SignalComplete",//SIPCP_EVENT_SIGNAL_COMP

    /*网络事件*/
    "PeerAlert",     //SIPCP_EVENT_LEG_PEER_ALERT
    "CreateSucc",    //SIPCP_EVENT_LEG_CREATE_SUCCESS
    "ReleaseInd",    //SIPCP_EVENT_LEG_RELEASE
    "NegoSucc",      //SIPCP_EVENT_LEG_NEGO_SUCCESS
    "NegoFail",      //SIPCP_EVENT_LEG_NEGO_FAIL
    "ReferSucc",     //SIPCP_EVENT_LEG_REFER_SUCCESS
    "ReferFail",     //SIPCP_EVENT_LEG_REFER_FAIL
    "CreateInd"      //SIPCP_EVENT_LEG_CREATE_IND
};

UCHAR *g_apucSignalType[SIPCP_SIGNAL_TYPE_BUTT]=
{
    /*提示音信号*/
    "DialTone",       //SIPCP_SIGNAL_TONE_DIAL,
    "CWtone",         //SIPCP_SIGNAL_TONE_CW,
    "RingBackTone",   //SIPCP_SIGNAL_TONE_RINGBACK,

    /*线路信号*/
    "Ring",           //SIPCP_SIGNAL_LINE_RING,

    /*复合信号*/
    "CID1",           //SIPCP_SIGNAL_CID1,
    "CID2",           //SIPCP_SIGNAL_CID2,
    "AOCE",           //SIPCP_SIGNAL_AOCE,
    "WaitHook"        //SIPCP_SIGNAL_WAITHOOK,
};

UCHAR *g_apucPlayType[SIPCP_SIGNAL_PLAY_BUTT]=
{
    "LocalPlay",  //SIPCP_SIGNAL_PLAY_L,
    "LocalMix",   //SIPCP_SIGNAL_PLAY_L_MIX,
    "RemotePlay", //SIPCP_SIGNAL_PLAY_R,
    "RemoteMix"   //SIPCP_SIGNAL_PLAY_R_MIX,
};

UCHAR *g_apucSID[SIPCP_SERVICE_ID_BUTT]=
{
    "NORMAL",   //SIPCP_SERVICE_ID_NORMAL
    "MCID",     //SIPCP_SERVICE_ID_MCID
    "CW",       //SIPCP_SERVICE_ID_CW
    "HOLD",     //SIPCP_SERVICE_ID_HOLD
    "CONF",     //SIPCP_SERVICE_ID_CONF
    "TPTY",      //SIPCP_SERVICE_ID_3PTY
    "ALL"      //SIPCP_SERVICE_ID_ALL
};

UCHAR *g_apucLegType[SIPCP_LEG_TYPE_BUTT]=
{
    "ControlLeg",       /* 业务控制腿 */
    "CallLeg",          /* 普通呼叫腿 */
    "All",              /* 所有腿，用于腿操作 */
    "Any"               /* 任何腿，用于腿匹配 */
};

UCHAR *g_apucCLegType[SIPCP_CLEG_TYPE_BUTT]=
{
    "CurrentLeg",     /* 当前呼叫腿 */
    "NonCurrentLeg",  /* 非当前呼叫腿 */
    "All",            /* 所有呼叫腿，用于腿操作 */
    "Any",            /* 任何呼叫腿，用于腿匹配 */
};

UCHAR *g_apucLegState[SIPCP_LEG_STATE_BUTT]=
{
    "N",        //SIPCP_LEG_STATE_N,
    "W",        //SIPCP_LEG_STATE_W,
    "A",        //SIPCP_LEG_STATE_A,
    "D",        //SIPCP_LEG_STATE_D,
    "H",        //SIPCP_LEG_STATE_H,
    "any"       //SIPCP_LEG_STATE_ANY
};

UCHAR *g_apucLegsState[SIPCP_LEGS_STATE_BUTT]=
{
    "N",//SIPCP_LEGS_STATE_N,
    "W",//SIPCP_LEGS_STATE_W,
    "A",//SIPCP_LEGS_STATE_A,
    "D",//SIPCP_LEGS_STATE_D,
    "H",//SIPCP_LEGS_STATE_H,
    "AW",//SIPCP_LEGS_STATE_AW,
    "HH",//SIPCP_LEGS_STATE_HH,
    "AA",//SIPCP_LEGS_STATE_AA,
    "AH",//SIPCP_LEGS_STATE_AH,
    "any"//SIPCP_LEGS_STATE_ANY,
};

UCHAR *g_apucCondition[SIPCP_CONDITION_BUTT]=
{
    "Caller", //SIPCP_CONDITION_CALLER
    "Callee", //SIPCP_CONDITION_CALLEE
    "OffHook", //SIPCP_CONDITION_OFFHOOK
    "OnHook", //SIPCP_CONDITION_ONHOOK
    "PstnPort", //SIPCP_CONDITION_PSTNPORT
    "IsdnPort", //SIPCP_CONDITION_ISDNPORT
    "PraPort", //SIPCP_CONDITION_PRAPORT
    "Non-Nego", //SIPCP_CONDITION_NONEGO
    "Profile20" //SIPCP_CONDITION_PROFILE20
//    SIPCP_CONDITION_CALLEE_FIRST = 0x04,
};

UCHAR *g_apucSpecFlag[SIPCP_FLAG_BUTT]=
{
    "KeepActive", //SIPCP_FLAG_KA
};

UCHAR *g_apucAction[SIPCP_ACTION_BUTT]=
{
    /*命令下发*/
    "CreateLeg",  //SIPCP_ACTION_CREATE_LEG,
    "ConfirmLeg",  //SIPCP_ACTION_CFM_LEG,
    "ReleaseLeg",  //SIPCP_ACTION_RELEASE_LEG,
    "ReferLeg",  //SIPCP_ACTION_REFER_LEG,
    "HoldLeg",  //SIPCP_ACTION_HOLD_LEG,
    "ResumeLeg",  //SIPCP_ACTION_RESUME_LEG,

    "DeactiveLeg",  //SIPCP_ACTION_DEACTIVE_LEG,
    "ActiveLeg",  //SIPCP_ACTION_ACTIVE_LEG,

    /* 后续ACTION认为任何时候都不会失败 */
    "StartTimer",  //SIPCP_ACTION_START_SRV_TIMER,
    "StopTimer",  //SIPCP_ACTION_STOP_SRV_TIMER,

    /*这两个ACTION必须放在最后，防止引起混乱*/
    "StartSignal",  //SIPCP_ACTION_START_SIGNAL,
    "SetCurrentLeg"  //SIPCP_ACTION_SET_CURRENT_LEG,
};

UCHAR *g_apucCreateMethod[SIPCP_CREATE_METHOD_BUTT]=
{
    "OffHook",  //SIPCP_CREATE_METHOD_OFFHOOK,
    "Overlap",  //SIPCP_CREATE_METHOD_OVERLAP,
    "Hotline",  //SIPCP_CREATE_METHOD_HOTLINE,
    "Conference",  //SIPCP_CREATE_METHOD_CONF,
    "UserNumber",  //SIPCP_CREATE_METHOD_USER,
    "EmeNumber",  //SIPCP_CREATE_METHOD_EME,
    "SccNumber"  //SIPCP_CREATE_METHOD_SCC,
};

UCHAR *g_apucActiveMode[SIPCP_ACTIVE_MODE_BUTT]=
{
    "Preempt", //SIPCP_ACTIVE_MODE_PREEMPT,
    "Mix", //SIPCP_ACTIVE_MODE_MIX,
    "PlayRemoteTone" //SIPCP_ACTIVE_MODE_R_TONE,
};

UCHAR *g_apucTimerName[SIPCP_TIMER_BUTT]=
{
    "Hotline", //SIPCP_TIMER_HOTLINE,
    "CallWait", //SIPCP_TIMER_CW,
    "BackRing"  //SIPCP_TIMER_BACKRING,
};

/*根据字符串获取枚举*/
UCHAR SIPCP_GetEnumFromString(UCHAR *pucString,UCHAR **ppucSource,UCHAR ucMaxEnum)
{
    UCHAR ucLoop;
    for (ucLoop = 0; ucLoop < ucMaxEnum; ucLoop++)
    {
        if(!xmlStrcmp(BAD_CAST(pucString), BAD_CAST(ppucSource[ucLoop])))
        {
            return ucLoop;
        }
    }

    return NULL_UCHAR;
}

/*根据枚举获取字符串*/
UCHAR* SIPCP_GetStringFromEnum(UCHAR ucEnum,UCHAR **ppucSource)
{
    return ppucSource[ucEnum];
}

ULONG SIPCP_GetEventEnumFromString(UCHAR *pucString,UCHAR *pucEnum)
{
    UCHAR ucEnum;
    ucEnum = SIPCP_GetEnumFromString(pucString, g_apucEventName, SIPCP_EVENT_BUTT);
    if(ucEnum < SIPCP_EVENT_BUTT)
    {
        *pucEnum = ucEnum;
        return SUCCESS;
    }
    return FAIL;
}

ULONG SIPCP_GetEventStringFromEnum(UCHAR ucEnum, UCHAR **ppucString)
{
    if(ucEnum >= SIPCP_EVENT_BUTT)
    {
        return FAIL;
    }

    *ppucString = SIPCP_GetStringFromEnum(ucEnum, g_apucEventName);
    return SUCCESS;
}

ULONG SIPCP_GetSignalTypeEnumFromString(UCHAR *pucString,UCHAR *pucEnum)
{
    UCHAR ucEnum;
    ucEnum = SIPCP_GetEnumFromString(pucString, g_apucSignalType, SIPCP_SIGNAL_TYPE_BUTT);
    if(ucEnum < SIPCP_SIGNAL_TYPE_BUTT)
    {
        *pucEnum = ucEnum;
        return SUCCESS;
    }
    return FAIL;
}

ULONG SIPCP_GetSignalTypeStringFromEnum(UCHAR ucEnum, UCHAR **ppucString)
{
    if(ucEnum >= SIPCP_SIGNAL_TYPE_BUTT)
    {
        return FAIL;
    }

    *ppucString = SIPCP_GetStringFromEnum(ucEnum, g_apucSignalType);
    return SUCCESS;
}

ULONG SIPCP_GetPlayTypeEnumFromString(UCHAR *pucString,UCHAR *pucEnum)
{
    UCHAR ucEnum;
    ucEnum = SIPCP_GetEnumFromString(pucString, g_apucPlayType, SIPCP_SIGNAL_PLAY_BUTT);
    if(ucEnum < SIPCP_SIGNAL_PLAY_BUTT)
    {
        *pucEnum = ucEnum;
        return SUCCESS;
    }
    return FAIL;
}

ULONG SIPCP_GetPlayTypeStringFromEnum(UCHAR ucEnum, UCHAR **ppucString)
{
    if(ucEnum >= SIPCP_SIGNAL_PLAY_BUTT)
    {
        return FAIL;
    }

    *ppucString = SIPCP_GetStringFromEnum(ucEnum, g_apucPlayType);
    return SUCCESS;
}

ULONG SIPCP_GetSIDEnumFromString(UCHAR *pucString,UCHAR *pucEnum)
{
    UCHAR ucEnum;
    ucEnum = SIPCP_GetEnumFromString(pucString, g_apucSID, SIPCP_SERVICE_ID_BUTT);
    if(ucEnum < SIPCP_SERVICE_ID_BUTT)
    {
        *pucEnum = (SIPCP_SERVICE_ID_E)ucEnum;
        return SUCCESS;
    }
    return FAIL;
}

ULONG SIPCP_GetSIDStringFromEnum(UCHAR ucEnum, UCHAR **ppucString)
{
    if(ucEnum >= SIPCP_SERVICE_ID_BUTT)
    {
        return FAIL;
    }

    *ppucString = SIPCP_GetStringFromEnum(ucEnum, g_apucSID);
    return SUCCESS;
}

ULONG SIPCP_GetLegTypeEnumFromString(UCHAR *pucString,UCHAR *pucEnum)
{
    UCHAR ucEnum;
    ucEnum = SIPCP_GetEnumFromString(pucString, g_apucLegType, SIPCP_LEG_TYPE_BUTT);
    if(ucEnum < SIPCP_LEG_TYPE_BUTT)
    {
        *pucEnum = (SIPCP_LEG_TYPE_E)ucEnum;
        return SUCCESS;
    }
    return FAIL;
}

ULONG SIPCP_GetLegTypeStringFromEnum(UCHAR ucEnum, UCHAR **ppucString)
{
    if(ucEnum >= SIPCP_LEG_TYPE_BUTT)
    {
        return FAIL;
    }

    *ppucString = SIPCP_GetStringFromEnum(ucEnum, g_apucLegType);
    return SUCCESS;
}

ULONG SIPCP_GetCLegTypeEnumFromString(UCHAR *pucString,UCHAR *pucEnum)
{
    UCHAR ucEnum;
    ucEnum = SIPCP_GetEnumFromString(pucString, g_apucCLegType, SIPCP_CLEG_TYPE_BUTT);
    if(ucEnum < SIPCP_CLEG_TYPE_BUTT)
    {
        *pucEnum = (SIPCP_CLEG_TYPE_E)ucEnum;
        return SUCCESS;
    }
    return FAIL;
}

ULONG SIPCP_GetCLegTypeStringFromEnum(UCHAR ucEnum, UCHAR **ppucString)
{
    if(ucEnum >= SIPCP_CLEG_TYPE_BUTT)
    {
        return FAIL;
    }

    *ppucString = SIPCP_GetStringFromEnum(ucEnum, g_apucCLegType);
    return SUCCESS;
}

ULONG SIPCP_GetLegStateEnumFromString(UCHAR *pucString,UCHAR *pucEnum)
{
    UCHAR ucEnum;
    ucEnum = SIPCP_GetEnumFromString(pucString, g_apucLegState, SIPCP_LEG_STATE_BUTT);
    if(ucEnum < SIPCP_LEG_STATE_BUTT)
    {
        *pucEnum = (SIPCP_LEG_STATE_E)ucEnum;
        return SUCCESS;
    }
    return FAIL;
}

ULONG SIPCP_GetLegStateStringFromEnum(UCHAR ucEnum, UCHAR **ppucString)
{
    if(ucEnum >= SIPCP_LEG_STATE_BUTT)
    {
        return FAIL;
    }

    *ppucString = SIPCP_GetStringFromEnum(ucEnum, g_apucLegState);
    return SUCCESS;
}

ULONG SIPCP_GetLegsStateEnumFromString(UCHAR *pucString,UCHAR *pucEnum)
{
    UCHAR ucEnum;
    ucEnum = SIPCP_GetEnumFromString(pucString, g_apucLegsState, SIPCP_LEGS_STATE_BUTT);
    if(ucEnum < SIPCP_LEGS_STATE_BUTT)
    {
        *pucEnum = (SIPCP_LEGS_STATE_E)ucEnum;
        return SUCCESS;
    }
    return FAIL;
}

ULONG SIPCP_GetLegsStateStringFromEnum(UCHAR ucEnum, UCHAR **ppucString)
{
    if(ucEnum >= SIPCP_LEGS_STATE_BUTT)
    {
        return FAIL;
    }

    *ppucString = SIPCP_GetStringFromEnum(ucEnum, g_apucLegsState);
    return SUCCESS;
}

ULONG SIPCP_GetConditionEnumFromString(UCHAR *pucString,UCHAR *pucEnum)
{
    UCHAR ucEnum;
    ucEnum = SIPCP_GetEnumFromString(pucString, g_apucCondition, SIPCP_CONDITION_BUTT);
    if(ucEnum < SIPCP_CONDITION_BUTT)
    {
        *pucEnum = (SIPCP_CONDITION_E)ucEnum;
        return SUCCESS;
    }
    return FAIL;
}

ULONG SIPCP_GetConditionStringFromEnum(UCHAR ucEnum, UCHAR **ppucString)
{
    if(ucEnum >= SIPCP_CONDITION_BUTT)
    {
        return FAIL;
    }

    *ppucString = SIPCP_GetStringFromEnum(ucEnum, g_apucCondition);
    return SUCCESS;
}

ULONG SIPCP_GetSpecFlagEnumFromString(UCHAR *pucString,UCHAR *pucEnum)
{
    UCHAR ucEnum;
    ucEnum = SIPCP_GetEnumFromString(pucString, g_apucSpecFlag, SIPCP_FLAG_BUTT);
    if(ucEnum < SIPCP_FLAG_BUTT)
    {
        *pucEnum = (SIPCP_FLAG_E)ucEnum;
        return SUCCESS;
    }
    return FAIL;
}

ULONG SIPCP_GetSpecFlagStringFromEnum(UCHAR ucEnum, UCHAR **ppucString)
{
    if(ucEnum >= SIPCP_FLAG_BUTT)
    {
        return FAIL;
    }

    *ppucString = SIPCP_GetStringFromEnum(ucEnum, g_apucSpecFlag);
    return SUCCESS;
}

ULONG SIPCP_GetActionEnumFromString(UCHAR *pucString,UCHAR *pucEnum)
{
    UCHAR ucEnum;
    ucEnum = SIPCP_GetEnumFromString(pucString, g_apucAction, SIPCP_ACTION_BUTT);
    if(ucEnum < SIPCP_ACTION_BUTT)
    {
        *pucEnum = (SIPCP_ACTION_E)ucEnum;
        return SUCCESS;
    }
    return FAIL;
}

ULONG SIPCP_GetActionStringFromEnum(UCHAR ucEnum, UCHAR **ppucString)
{
    if(ucEnum >= SIPCP_ACTION_BUTT)
    {
        return FAIL;
    }

    *ppucString = SIPCP_GetStringFromEnum(ucEnum, g_apucAction);
    return SUCCESS;
}

ULONG SIPCP_GetCreateMethodEnumFromString(UCHAR *pucString,UCHAR *pucEnum)
{
    UCHAR ucEnum;
    ucEnum = SIPCP_GetEnumFromString(pucString, g_apucCreateMethod, SIPCP_CREATE_METHOD_BUTT);
    if(ucEnum < SIPCP_CREATE_METHOD_BUTT)
    {
        *pucEnum = (SIPCP_CREATE_METHOD_E)ucEnum;
        return SUCCESS;
    }
    return FAIL;
}

ULONG SIPCP_GetCreateMethodStringFromEnum(UCHAR ucEnum, UCHAR **ppucString)
{
    if(ucEnum >= SIPCP_CREATE_METHOD_BUTT)
    {
        return FAIL;
    }

    *ppucString = SIPCP_GetStringFromEnum(ucEnum, g_apucCreateMethod);
    return SUCCESS;
}

ULONG SIPCP_GetActiveModeEnumFromString(UCHAR *pucString,UCHAR *pucEnum)
{
    UCHAR ucEnum;
    ucEnum = SIPCP_GetEnumFromString(pucString, g_apucActiveMode, SIPCP_ACTIVE_MODE_BUTT);
    if(ucEnum < SIPCP_ACTIVE_MODE_BUTT)
    {
        *pucEnum = (SIPCP_ACTIVE_MODE_E)ucEnum;
        return SUCCESS;
    }
    return FAIL;
}

ULONG SIPCP_GetActiveModeStringFromEnum(UCHAR ucEnum, UCHAR **ppucString)
{
    if(ucEnum >= SIPCP_ACTIVE_MODE_BUTT)
    {
        return FAIL;
    }

    *ppucString = SIPCP_GetStringFromEnum(ucEnum, g_apucActiveMode);
    return SUCCESS;
}

ULONG SIPCP_GetTimerNameEnumFromString(UCHAR *pucString,UCHAR *pucEnum)
{
    UCHAR ucEnum;
    ucEnum = SIPCP_GetEnumFromString(pucString, g_apucTimerName, SIPCP_TIMER_BUTT);
    if(ucEnum < SIPCP_TIMER_BUTT)
    {
        *pucEnum = (SIPCP_TIMER_E)ucEnum;
        return SUCCESS;
    }
    return FAIL;
}

ULONG SIPCP_GetTimerNameStringFromEnum(UCHAR ucEnum, UCHAR **ppucString)
{
    if(ucEnum >= SIPCP_TIMER_BUTT)
    {
        return FAIL;
    }

    *ppucString = SIPCP_GetStringFromEnum(ucEnum, g_apucTimerName);
    return SUCCESS;
}