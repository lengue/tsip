#ifndef SIP_CP_VAR_INC
#define SIP_CP_VAR_INC

/*ҵ���߼�������*/
extern SIPCP_SL_FIND_S g_ausSlIndexTbl[SIPCP_MAX_INTERFACE_NUM][SIPCP_SERVICE_ID_BUTT][SIPCP_LEG_STATE_BUTT][SIPCP_LEGS_STATE_BUTT];

/*ҵ���߼���*/
extern SIPCP_SL_ITEM_S *g_pstSlTbl;

/*ҵ���߼��洢��*/
extern SIPCP_SL_STORE_S *g_pstSlStore;

/*ҵ���߼�ӳ���*/
extern SIPCP_SL_MAPPING_S *g_pstSlMapping;

extern USHORT g_usCurrSlTbl;
extern USHORT g_usCurrSlItem;
extern UCHAR  g_usCurrAction;

/*ҵ����ƿ�*/
extern SIPCP_SPCB_S    *g_pstSPCB;
extern SIPCP_SPCB_S    *g_pstSPCBBak;

/*MDU��Ϣ���й���*/
extern UCHAR            g_ucMduMsgNum;
extern SIPCP_MDU_MSG_S *g_pstMduQueue;

/*ҵ���߼�ִ��ʱ�Ļ�������*/
extern ULONG ulCurSpID;
extern ULONG ulTmpConnID;
extern ULONG ulTmpConfID;

#endif