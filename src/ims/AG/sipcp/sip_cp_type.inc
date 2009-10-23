/*事件结构*/
#ifndef SIP_CP_TYPE_H
#define SIP_CP_TYPE_H

typedef struct tagSIPCP_EVENT_S
{
    SIPCP_EVENT_E  eEvent;                          /*触发事件*/
    SIPCP_EVENT_E  eFailEvent;                      /*业务逻辑触发失败的转化事件*/
    UCHAR        aucParaList[SIPCP_EVENT_PARA_NUM]; /*事件参数，不同事件参数含义不同*/
}SIPCP_EVENT_S;

/*行动结构*/
typedef struct tagSIPCP_ACTION_S
{
    SIPCP_ACTION_E eAction;
    UCHAR          aucParaList[SIPCP_ACTION_PARA_NUM];
}SIPCP_ACTION_S;

/*业务逻辑表核心结构*/
typedef struct tagSIPCP_SL_CORE_S
{
    SIPCP_EVENT_S    stEvent;                     /*触发事件*/
    ULONG            ulCondition;                 /*入口条件，参考SIP_CONDITION_E*/
    USHORT           usFlagMask;                  /*特殊处理标记掩码，参考SIP_FLAG_E*/
    SIPCP_ACTION_S     astAction[SIPCP_ACTION_NUM]; /*行动列表*/
    SIPCP_SERVICE_ID_E eNextHSID;                 /*下一归属业务逻辑*/
    USHORT           usSIDMask;                   /*业务允许掩码*/
}SIPCP_SL_CORE_S;

/*业务逻辑查找项结构*/
typedef struct tagSIPCP_SL_ITEM_S
{
    SIPCP_SL_CORE_S   stSlCore;                  /* 业务逻辑表项 */
    USHORT            usNextItem;                /* 下一逻辑项索引 */
}SIPCP_SL_ITEM_S;

/*业务逻辑查找表结构*/
typedef struct tagSIPCP_SL_FIND_S
{
    USHORT usFirstIndex;  /* 第一条业务逻辑表项 */
    USHORT usLastIndex;   /* 最后一条业务逻辑表项 */
}SIPCP_SL_FIND_S;

/*业务逻辑存储表结构，存储数据库*/
typedef struct tagSIPCP_SL_STORE_S
{
    SIPCP_SERVICE_ID_E eHSID;             /*归属业务ID*/
    SIPCP_LEG_STATE_E  eOLS;              /*控制腿状态*/
    SIPCP_LEGS_STATE_E eCLS;              /*呼叫腿状态*/
    SIPCP_SL_CORE_S    stSlCore;          /*业务逻辑项*/
}SIPCP_SL_STORE_S;

/*业务逻辑映射存储表结构，存储数据库*/
typedef struct tagSIPCP_SL_MAPPING_S
{
    UCHAR  ucIsUsed;                   /*使用标记*/
    UCHAR  aucVersion[SIPCP_SL_VERSION_LEN];  /*归属业务ID*/
    USHORT usFirstIndex;               /*第一条业务逻辑表项在这个业务逻辑表中的为位置*/
    USHORT ucTotal;                    /*表项总数*/
}SIPCP_SL_MAPPING_S;

/*refer命令参数*/
typedef struct tagSIPCP_REFER_PARA_S
{
    UCHAR ucReferType;
    ULONG ulLegID;
}SIPCP_REFER_PARA_S;

typedef struct tagSIPCP_CREATE_PARA_S
{
    UCHAR ucReferType;
    ULONG ulLegID;
}SIPCP_CREATE_PARA_S;

/*MDU消息结构*/
typedef struct tagSIPCP_MDU_MSG_S
{
    ULONG ulSpID;
    UCHAR ucSeq;
    ULONG ulEmID;
    UCHAR ucBchn;
    ULONG ulLegID;
    SIPCP_MDU_TYPE_E    eMsgType;    /*SIPCP_MDU_MSG_TYPE_E*/
    ULONG               ulCauseCode; /*内部响应码*/
    struct union
    {
        SIPCP_REFER_PARA_S  stReferPara;
        SIPCP_CREATE_PARA_S stCreatePara;
    }u;                              /*不同MDU消息私有参数*/
}SIPCP_MDU_MSG_S;

/*数图管理结构*/
typedef struct tagSIPCP_DMM_MNT_S
{
    ULONG ulDmmIndex;
    UCHAR ucDialMode;
}SIPCP_DMM_MNT_S;

/*信号管理结构*/
typedef struct tagSIPCP_SIGNAL_MNT_S
{
    SIPCP_SIGNAL_TYPE_E eSignalID;
}SIPCP_SIGNAL_MNT_S;

/*连接管理结构*/
typedef struct tagSIPCP_CONN_MNT_S
{
    ULONG ulConnID;                    /*连接ID*/
    UCHAR ucRefLegSeq;                 /*关联腿序号*/
    UCHAR ucLocalPlay;                 /*本端放音标记*/
    UCHAR ucRemotePlay;                /*远端放音标记*/
    UCHAR ucMix;                       /*混音标记*/
}SIPCP_CONN_MNT_S;

/*资源管理结构*/
typedef struct tagSIPCP_RESOURCE_MNT_S
{
    ULONG ulConfID;                                 /*混音资源ID*/
    UCHAR ucConnNum;                                /*连接数目*/
    UCHAR ucResvConn;                               /*保留连接序号，不能释放*/
    SIPCP_CONN_MNT_S astConn[SIPCP_CONN_NUM_IN_SPCB]; /*连接数组*/
}SIPCP_RESOURCE_MNT_S;

/*流属性结构*/
typedef struct tagSIPCP_MEDIA_ATTR_S
{
    ULONG ulTmp;
}SIPCP_MEDIA_ATTR_S;

/*******************************************************************************
腿管理结构中的流属性表示的是腿已经协商好的流属性，
腿控制结构中的流属性表示的是腿正在协商的流属性
连接里面(RTP里面记录的是打开通道使用的流属性)
*******************************************************************************/
/*业务腿管理结构*/
typedef struct tagSIPCP_LEG_MNT_S
{
    ULONG              ulLegID;         /*腿控制块索引,NULL_ULONG为无效值*/
    SIPCP_LEG_STATE_E  eState;          /*腿状态*/
    SIPCP_MEDIA_ATTR_S stMediaAttr;     /*腿当前的流属性*/
    UCHAR              ucNegotiate;     /*是否正在协商*/
    UCHAR              ucConnSeq;       /*关联连接序号*/
    UCHAR              bFocus :1;       /*Focus标记*/
}SIPCP_LEG_MNT_S;

typedef struct tagSIPCP_OBSERVE_EVENT_S
{
    USHORT         ulSlIndex;                         /*业务逻辑表索引*/
    SIPCP_EVENT_E  eEvent;                            /*触发事件*/
    UCHAR          aucParaList[SIPCP_EVENT_PARA_NUM]; /*事件参数，不同事件参数含义不同*/
}SIPCP_OBSERVE_EVENT_S;

/*业务逻辑管理结构*/
typedef struct tagSIPCP_SERVICE_MNT_S
{
    SIPCP_SERVICE_ID_E eHomeSID;                      /*归属业务ID*/
    SIPCP_LEG_STATE_E  eOLS;                          /*缓存控制腿状态*/
    SIPCP_LEGS_STATE_E eICLS;                         /*缓存综合呼叫腿状态*/
    SIPCP_SERVICE_ID_E eCurSID;                       /*当前业务ID*/
    UCHAR              ucCurLegSeq;                   /*当前腿索引*/
    UCHAR              ucCtlLegSeq;                   /*控制腿索引*/
    UCHAR              ucValidLegNum;                 /*有效腿数目*/
    UCHAR              ucAllocedLegNum;               /*占用腿数目*/
    SIPCP_LEG_MNT_S    astLeg[SIPCP_LEG_NUM_IN_SPCB]; /*腿记录*/
    HTIMER             hSrvTimer;                     /*业务定时器句柄*/
    SIPCP_TIMER_E      eSrvTimerName;                 /*业务定时器名字*/
    ULONG              ulSrvTimerLen;                 /*业务定时器时长*/
    SIPCP_OBSERVE_EVENT_S astOEvent[SIPCP_MAX_OEVENT_NUM]; /*腿记录*/
}SIPCP_SERVICE_MNT_S;

/*业务逻辑管理结构*/
typedef struct tagSIPCP_SERVICE_RIGHT_S
{
    UCHAR  ulSrvPriority[SIPCP_SERVICE_ID_BUTT];  /*业务优先级*/
    ULONG  ulSrvMask;                             /*业务支持掩码*/
    ULONG  ulHotlineLen;                          /*热线时长*/
}SIPCP_SERVICE_RIGHT_S;

/*业务控制块*/
typedef struct tagSIPCP_SPCB_S
{
    UCHAR ucUsed;

    /*基本属性BasicAttr*/
    ULONG ulEmID;
    UCHAR ucBsn;
    ULONG ulVagNo;

    /*动态属性DynamicAttr*/
    UCHAR bWaitRelease :1;/*等待释放标记*/
    UCHAR bDiscFlag    :1;/*摘挂机标志*/
    UCHAR bReversePol  :1;/*反极性标记*/
    UCHAR bAoceReady   :1;/*AOCE发送标记*/

    /*扩展属性ExternAttr*/
    UCHAR *pstAoceStr;

    /*数图子控制块*/
    SIPCP_DMM_MNT_S stDmmCtr;

    /*信号子控制块*/
    SIPCP_SIGNAL_MNT_S stSignal;

    /*资源子控制块*/
    SIPCP_RESOURCE_MNT_S stResource;

    /*业务逻辑子控制块*/
    SIPCP_SERVICE_MNT_S  stService;

    /*业务权限缓存*/
    SIPCP_SERVICE_RIGHT_S stSrvRight;

    USHORT usNextIndex;
}SIPCP_SPCB_S;

#else
#endif