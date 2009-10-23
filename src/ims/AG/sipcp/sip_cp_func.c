#include <libxml/parser.h>
#include <stdio.h>
#include "vsp.h"
#include "sip_cp_const.inc"
#include "sip_cp_type.inc"
#include "sip_cp_func.inc"
/*******************************************************************************

*******************************************************************************/
ULONG SIPCP_GetServiceMaskFromID(SIPCP_SERVICE_ID_E eEnum,ULONG *pulMask)
{
    return SUCCESS;
}

/*******************************************************************************

*******************************************************************************/
ULONG SIPCP_GetServiceIDFromMask(ULONG ulMask,UCHAR *pucEnum)
{
    return SUCCESS;
}

/*******************************************************************************

*******************************************************************************/
ULONG SIPCP_GetConditionMaskFromEnum(UCHAR ucEnum,ULONG *pulMask)
{
    return SUCCESS;
}

/*******************************************************************************

*******************************************************************************/
ULONG SIPCP_GetConditionIDFromMask(ULONG ulMask,UCHAR *pucEnum)
{
    return SUCCESS;
}

/*******************************************************************************

*******************************************************************************/
ULONG SIPCP_GetSpecFlagMaskFromID(UCHAR ucEnum,ULONG *pulMask)
{
    return SUCCESS;
}

/*******************************************************************************

*******************************************************************************/
ULONG SIPCP_GetSpecFlagIDFromMask(ULONG ulMask,UCHAR *pucEnum)
{
    return SUCCESS;
}

/*******************************************************************************
Ë¢ÐÂ×ÛºÏºô½ÐÍÈ×´Ì¬
*******************************************************************************/
ULONG SIPCP_StartDmm()
{
    return SUCCESS;
}

ULONG SIPCP_StopDmm()
{
    return SUCCESS;
}

/*******************************************************************************
Ë¢ÐÂ×ÛºÏºô½ÐÍÈ×´Ì¬
*******************************************************************************/
ULONG SIPCP_SendMduMsg(SIPCP_MDU_MSG_S *pMduMsg)
{
    return SUCCESS;
}

/*******************************************************************************
Ë¢ÐÂ×ÛºÏºô½ÐÍÈ×´Ì¬
*******************************************************************************/
ULONG VOS_MemCpy(UCHAR *pucDisStr,UCHAR *pucSource,ULONG ulSize)
{
    return SUCCESS;
}

/*******************************************************************************
Ë¢ÐÂ×ÛºÏºô½ÐÍÈ×´Ì¬
*******************************************************************************/
ULONG SIPCP_CheckSlItem(UCHAR *pucDisStr,UCHAR *pucSource,ULONG ulSize)
{
    return SUCCESS;
}

/*******************************************************************************
Ë¢ÐÂ×ÛºÏºô½ÐÍÈ×´Ì¬
*******************************************************************************/
ULONG SIPCP_BuildSlTbl(ULONG ulVagNo)
{
    USHORT usLoop;
    USHORT usFirst;
    USHORT usLast;
    SIPCP_SL_FIND_S *pstFindTbl;

    usFirst = g_pstSlMapping[ulVagNo].usFirstIndex;
    usLast  = g_pstSlMapping[ulVagNo].usFirstIndex
             + g_pstSlMapping[ulVagNo].ucTotal
             - 1;

    for (usLoop = usFirst; usLoop <= usLast; usLoop++)
    {
        VOS_MemCpy(&g_pstSlTbl[usLoop - usFirst].stSlCore,
                   &g_pstSlStore[usLoop].stSlCore,
                   sizeof(SIPCP_SL_CORE_S));
        g_pstSlTbl[usLoop - usFirst].usNextItem = NULL_USHORT;

        pstFindTbl = &g_ausSlIndexTbl[ulVagNo]
                                     [g_pstSlStore[usLoop].eHSID]
                                     [g_pstSlStore[usLoop].eOLS]
                                     [g_pstSlStore[usLoop].eCLS];
        if (pstFindTbl->usFirstIndex == NULL_USHORT)
        {
            pstFindTbl->usFirstIndex = usLoop - usFirst;
            pstFindTbl->usLastIndex  = usLoop - usFirst;
        }
        else
        {
            g_pstSlTbl[pstFindTbl->usLastIndex].usNextItem = usLoop - usFirst;
            pstFindTbl->usLastIndex = usLoop - usFirst;
        }
    }

    return SUCCESS;
}