#ifndef SIP_CP_SLSTRING_INC
#define SIP_CP_SLSTRING_INC

extern UCHAR *g_apucEventName[SIPCP_EVENT_BUTT];

extern UCHAR *g_apucSignalType[SIPCP_SIGNAL_TYPE_BUTT];

extern UCHAR *g_apucPlayType[SIPCP_SIGNAL_PLAY_BUTT];

extern UCHAR *g_apucSID[SIPCP_SERVICE_ID_BUTT];

extern UCHAR *g_apucLegType[SIPCP_LEG_TYPE_BUTT];

extern UCHAR *g_apucCLegType[SIPCP_CLEG_TYPE_BUTT];

extern UCHAR *g_apucLegState[SIPCP_LEG_STATE_BUTT];

extern UCHAR *g_apucLegsState[SIPCP_LEGS_STATE_BUTT];

extern UCHAR *g_apucCondition[SIPCP_CONDITION_BUTT];

extern UCHAR *g_apucSpecFlag[SIPCP_FLAG_BUTT];

extern UCHAR *g_apucAction[SIPCP_ACTION_BUTT];

extern UCHAR *g_apucCreateMethod[SIPCP_CREATE_METHOD_BUTT];

extern UCHAR *g_apucActiveMode[SIPCP_ACTIVE_MODE_BUTT];

extern UCHAR *g_apucTimerName[SIPCP_TIMER_BUTT];

extern UCHAR SIPCP_GetEnumFromString(UCHAR *pucString,UCHAR **ppucSource,UCHAR ucMaxEnum);
extern UCHAR* SIPCP_GetStringFromEnum(UCHAR ucEnum,UCHAR **ppucSource);
extern ULONG SIPCP_GetEventEnumFromString(UCHAR *pucString,UCHAR *pucEnum);
extern ULONG SIPCP_GetEventStringFromEnum(UCHAR ucEnum, UCHAR **ppucString);
extern ULONG SIPCP_GetSignalTypeEnumFromString(UCHAR *pucString,UCHAR *pucEnum);
extern ULONG SIPCP_GetSignalTypeStringFromEnum(UCHAR ucEnum, UCHAR **ppucString);
extern ULONG SIPCP_GetPlayTypeEnumFromString(UCHAR *pucString,UCHAR *pucEnum);
extern ULONG SIPCP_GetPlayTypeStringFromEnum(UCHAR ucEnum, UCHAR **ppucString);
extern ULONG SIPCP_GetSIDEnumFromString(UCHAR *pucString,UCHAR *pucEnum);
extern ULONG SIPCP_GetSIDStringFromEnum(UCHAR ucEnum, UCHAR **ppucString);
extern ULONG SIPCP_GetLegTypeEnumFromString(UCHAR *pucString,UCHAR *pucEnum);
extern ULONG SIPCP_GetLegTypeStringFromEnum(UCHAR ucEnum, UCHAR **ppucString);
extern ULONG SIPCP_GetCLegTypeEnumFromString(UCHAR *pucString,UCHAR *pucEnum);
extern ULONG SIPCP_GetCLegTypeStringFromEnum(UCHAR ucEnum, UCHAR **ppucString);
extern ULONG SIPCP_GetLegStateEnumFromString(UCHAR *pucString,UCHAR *pucEnum);
extern ULONG SIPCP_GetLegStateStringFromEnum(UCHAR ucEnum, UCHAR **ppucString);
extern ULONG SIPCP_GetLegsStateEnumFromString(UCHAR *pucString,UCHAR *pucEnum);
extern ULONG SIPCP_GetLegsStateStringFromEnum(UCHAR ucEnum, UCHAR **ppucString);
extern ULONG SIPCP_GetConditionEnumFromString(UCHAR *pucString,UCHAR *pucEnum);
extern ULONG SIPCP_GetConditionStringFromEnum(UCHAR ucEnum, UCHAR **ppucString);
extern ULONG SIPCP_GetSpecFlagEnumFromString(UCHAR *pucString,UCHAR *pucEnum);
extern ULONG SIPCP_GetSpecFlagStringFromEnum(UCHAR ucEnum, UCHAR **ppucString);
extern ULONG SIPCP_GetActionEnumFromString(UCHAR *pucString,UCHAR *pucEnum);
extern ULONG SIPCP_GetActionStringFromEnum(UCHAR ucEnum, UCHAR **ppucString);
extern ULONG SIPCP_GetCreateMethodEnumFromString(UCHAR *pucString,UCHAR *pucEnum);
extern ULONG SIPCP_GetCreateMethodStringFromEnum(UCHAR ucEnum, UCHAR **ppucString);
extern ULONG SIPCP_GetActiveModeEnumFromString(UCHAR *pucString,UCHAR *pucEnum);
extern ULONG SIPCP_GetActiveModeStringFromEnum(UCHAR ucEnum, UCHAR **ppucString);
extern ULONG SIPCP_GetTimerNameEnumFromString(UCHAR *pucString,UCHAR *pucEnum);
extern ULONG SIPCP_GetTimerNameStringFromEnum(UCHAR ucEnum, UCHAR **ppucString);
#endif
