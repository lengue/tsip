#ifndef SIP_CP_FUNC_INC
#define SIP_CP_FUNC_INC

extern ULONG SIPCP_GetServiceMaskFromID(UCHAR ucEnum,ULONG *pulMask);

extern ULONG SIPCP_GetServiceIDFromMask(ULONG ulMask,UCHAR *pucEnum);

extern ULONG SIPCP_GetConditionMaskFromEnum(UCHAR ucEnum,ULONG *pulMask);

extern ULONG SIPCP_GetConditionIDFromMask(ULONG ulMask,UCHAR *pucEnum);

extern ULONG SIPCP_GetSpecFlagMaskFromID(UCHAR ucEnum,ULONG *pulMask);

extern ULONG SIPCP_GetSpecFlagIDFromMask(ULONG ulMask,UCHAR *pucEnum);

extern ULONG VOS_MemCpy(UCHAR *pucDisStr,UCHAR *pucSource,ULONG ulSize);

extern ULONG SIP_StartDmm();

extern ULONG SIP_SendMduMsg(SIPCP_MDU_MSG_S *pMduMsg);

#endif