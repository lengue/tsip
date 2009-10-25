/*******************************************************************************
功能    : SIP协议栈对外提供的函数
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/
extern ULONG SIP_Config(SIP_CFG_S *pstCfg);
extern ULONG SIP_TimeoutProc(ULONG ulName, ULONG ulPara);
extern ULONG SIP_GetRuleIndex(UCHAR *pucRuleName, ULONG *pulRuleIndex);
extern ULONG SIP_Code(ULONG  ulRuleIndex,
                       void  *pstStruct,
                       UCHAR *pucBuffer,
                       ULONG  ulBufferLen,
                       ULONG *pulMsgLen);
extern ULONG SIP_Decode(ULONG  ulRuleIndex,
                         UCHAR *pucMsgString,
                         ULONG  ulMsgLen,
                         UBUF_HEADER_S *pstUBuf,
                         void **ppstStruct);
extern ULONG SIP_Clone(ULONG          ulRuleIndex,
                       void          *pSrcStruct,
                       UBUF_HEADER_S *pstDstUbuf,
                       void         **ppDstStruct);
extern ULONG SIP_RecvUpMsg(SIP_LOCATION_S *pstPeerLocation, UBUF_HEADER_S *pstUbufSipMsg);
extern ULONG SIP_RecvDownMsg(ULONG ulAppRef1,
                             ULONG ulAppRef2,
                             ULONG *pulStackRef1,
                             ULONG *pulStackRef2,
                             UBUF_HEADER_S *pstUbufSipMsg);