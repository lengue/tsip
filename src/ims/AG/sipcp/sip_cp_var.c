#include <stdio.h>
#include "vsp.h"
#include "sip_cp_const.inc"
#include "sip_cp_type.inc"

/*ҵ���߼�������*/
SIPCP_SL_FIND_S g_ausSlIndexTbl[SIPCP_MAX_INTERFACE_NUM][SIPCP_SERVICE_ID_BUTT][SIPCP_LEG_STATE_BUTT][SIPCP_LEGS_STATE_BUTT];

/*ҵ���߼���*/
SIPCP_SL_ITEM_S *g_pstSlTbl;

/*ҵ���߼��洢��*/
SIPCP_SL_STORE_S *g_pstSlStore;

/*ҵ���߼�ӳ���*/
SIPCP_SL_MAPPING_S *g_pstSlMapping;

USHORT g_usCurrSlTbl;
USHORT g_usCurrSlItem;
UCHAR  g_usCurrAction;

/*ҵ����ƿ�*/
SIPCP_SPCB_S    *g_pstSPCB;
SIPCP_SPCB_S    *g_pstSPCBBak;

/*MDU��Ϣ���й���*/
UCHAR          g_ucMduMsgNum = 0;
SIPCP_MDU_MSG_S *g_pstMduQueue;

/*ҵ���߼�ִ��ʱ�Ļ�������*/
ULONG ulCurSpID;
ULONG ulTmpConnID;
ULONG ulTmpConfID;