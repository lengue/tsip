#ifndef SIP_CP_VAR_INC
#define SIP_CP_VAR_INC

/*业务逻辑索引表*/
extern SIPCP_SL_FIND_S g_ausSlIndexTbl[SIPCP_MAX_INTERFACE_NUM][SIPCP_SERVICE_ID_BUTT][SIPCP_LEG_STATE_BUTT][SIPCP_LEGS_STATE_BUTT];

/*业务逻辑表*/
extern SIPCP_SL_ITEM_S *g_pstSlTbl;

/*业务逻辑存储表*/
extern SIPCP_SL_STORE_S *g_pstSlStore;

/*业务逻辑映射表*/
extern SIPCP_SL_MAPPING_S *g_pstSlMapping;

extern USHORT g_usCurrSlTbl;
extern USHORT g_usCurrSlItem;
extern UCHAR  g_usCurrAction;

/*业务控制块*/
extern SIPCP_SPCB_S    *g_pstSPCB;
extern SIPCP_SPCB_S    *g_pstSPCBBak;

/*MDU消息队列管理*/
extern UCHAR            g_ucMduMsgNum;
extern SIPCP_MDU_MSG_S *g_pstMduQueue;

/*业务逻辑执行时的缓存数据*/
extern ULONG ulCurSpID;
extern ULONG ulTmpConnID;
extern ULONG ulTmpConfID;

#endif