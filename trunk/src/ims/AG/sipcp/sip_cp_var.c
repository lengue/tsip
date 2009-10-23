#include <stdio.h>
#include "vsp.h"
#include "sip_cp_const.inc"
#include "sip_cp_type.inc"

/*业务逻辑索引表*/
SIPCP_SL_FIND_S g_ausSlIndexTbl[SIPCP_MAX_INTERFACE_NUM][SIPCP_SERVICE_ID_BUTT][SIPCP_LEG_STATE_BUTT][SIPCP_LEGS_STATE_BUTT];

/*业务逻辑表*/
SIPCP_SL_ITEM_S *g_pstSlTbl;

/*业务逻辑存储表*/
SIPCP_SL_STORE_S *g_pstSlStore;

/*业务逻辑映射表*/
SIPCP_SL_MAPPING_S *g_pstSlMapping;

USHORT g_usCurrSlTbl;
USHORT g_usCurrSlItem;
UCHAR  g_usCurrAction;

/*业务控制块*/
SIPCP_SPCB_S    *g_pstSPCB;
SIPCP_SPCB_S    *g_pstSPCBBak;

/*MDU消息队列管理*/
UCHAR          g_ucMduMsgNum = 0;
SIPCP_MDU_MSG_S *g_pstMduQueue;

/*业务逻辑执行时的缓存数据*/
ULONG ulCurSpID;
ULONG ulTmpConnID;
ULONG ulTmpConfID;