/*******************************************************************************
����    : SIP���������ṩAPI
������  : �ƴ�ƽ
��������: 200.01.10
�޸ļ�¼:
*******************************************************************************/
extern ULONG SIP_Txp_RecvUpMsg(SIP_LOCATION_S *pstPeerLocation,
                                UBUF_HEADER_S  *pstSipMsgUbuf);
extern ULONG SIP_Txp_RecvDownMsg(UBUF_HEADER_S  *pstSipMsgUbuf,
                                  SIP_LOCATION_S *pstPeerLocation);
extern ULONG SIP_Txp_Init();