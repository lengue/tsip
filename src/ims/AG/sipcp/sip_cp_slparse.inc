#ifndef SIP_CP_SLPARSE_INC
#define SIP_CP_SLPARSE_INC

extern ULONG SIPCP_ParseSL(xmlDocPtr doc);

extern ULONG SIPCP_ParseHSID(xmlNodePtr pstNodeHSID);

extern ULONG SIPCP_ParseLegState(xmlNodePtr pstNodeLegState);

extern ULONG SIPCP_ParseEvent(xmlNodePtr pstNodeEvent);

extern ULONG SIPCP_ParseEventNode(SIPCP_EVENT_E eEvent,xmlNodePtr pstNodeEvent);

extern ULONG SIPCP_ParseSLItem(xmlNodePtr pstNodeSLItem);

extern ULONG SIPCP_ParseConditionList(xmlNodePtr pstNodeConditionList);

extern ULONG SIPCP_ParseSpecificFlagList(xmlNodePtr pstNodeSpecificFlagList);

extern ULONG SIPCP_ParseActionList(xmlNodePtr pstNodeActionList);

extern ULONG SIPCP_ParseServiceSupportList(xmlNodePtr pstNodeServiceSupportList);

extern ULONG SIPCP_ParseNextHSID(xmlNodePtr pstNodeNextHSID);

extern ULONG SIPCP_ParseActionList(xmlNodePtr pstNodeActionList);

extern ULONG SIPCP_ParseAction(SIPCP_ACTION_E eAction,xmlNodePtr pstNodeAction);

extern ULONG SIPCP_ParseActionPara_CreateLeg(xmlNodePtr pstNodeAction);

extern ULONG SIPCP_ParseActionPara_ConfirmLeg(xmlNodePtr pstNodeAction);

extern ULONG SIPCP_ParseActionPara_ReleaseLeg(xmlNodePtr pstNodeAction);

extern ULONG SIPCP_ParseActionPara_ReferLeg(xmlNodePtr pstNodeAction);

extern ULONG SIPCP_ParseActionPara_HoldLeg(xmlNodePtr pstNodeAction);

extern ULONG SIPCP_ParseActionPara_ResumeLeg(xmlNodePtr pstNodeAction);

extern ULONG SIPCP_ParseActionPara_SetCurrentLeg(xmlNodePtr pstNodeAction);

extern ULONG SIPCP_ParseActionPara_ActiveLeg(xmlNodePtr pstNodeAction);

extern ULONG SIPCP_ParseActionPara_DeactiveLeg(xmlNodePtr pstNodeAction);

extern ULONG SIPCP_ParseActionPara_StartSignal(xmlNodePtr pstNodeAction);

extern ULONG SIPCP_ParseActionPara_StartTimer(xmlNodePtr pstNodeAction);

extern ULONG SIPCP_ParseActionPara_StopTimer(xmlNodePtr pstNodeAction);

#else
#endif