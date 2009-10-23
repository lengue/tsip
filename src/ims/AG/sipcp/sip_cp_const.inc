#ifndef SIP_CP_CONST_H
#define SIP_CP_CONST_H

/*事件参数数目*/
#define SIPCP_EVENT_PARA_NUM 2
/*业务逻辑ACTION参数数目*/
#define SIPCP_ACTION_PARA_NUM 5
/*业务逻辑ACTION数目*/
#define SIPCP_ACTION_NUM 10
/*最大接口数*/
#define SIPCP_MAX_INTERFACE_NUM 8
/*MDU队列最大消息数*/
#define SIPCP_MAX_MDU_NUM  10
/*SPCB中的最大腿个数*/
#define SIPCP_LEG_NUM_IN_SPCB 3

/*SPCB中的最大观察事件个数*/
#define SIPCP_MAX_OEVENT_NUM 10

/*业务逻辑版本标识长度*/
#define SIPCP_SL_VERSION_LEN 20

/*SPCB中的最大连接个数*/
#define SIPCP_CONN_NUM_IN_SPCB SIPCP_LEG_NUM_IN_SPCB

/*最大业务逻辑项*/
#define SIPCP_MAX_SL_ITEMS 1024

typedef enum tagSIPCP_MDU_TYPE_E
{
    /* 命令CP->CC */
    SIPCP_MDU_CREAT_LEG_CMD  ,
    SIPCP_MDU_RELEASE_LEG_CMD,/*释放腿，释放完成需要通报*/
    SIPCP_MDU_CLEAR_LEG_CMD  ,  /*释放腿，释放完成无需通报*/
    SIPCP_MDU_REFER_LEG_CMD  ,
    SIPCP_MDU_HOLD_LEG_CMD   ,
    SIPCP_MDU_RESUME_LEG_CMD ,
    SIPCP_MDU_REFRESH_LEG_CMD,

    /* 响应CP->CC */
    SIPCP_MDU_CREAT_LEG_ACK  ,
    SIPCP_MDU_REFER_LEG_ACK  ,
    SIPCP_MDU_REFRESH_LEG_ACK,

    /* 指示CP->CC */
    SIPCP_MDU_CREAT_LEG_IND,          /* 收到INVITE，腿创建指示 */
    SIPCP_MDU_RELEASE_LEG_IND,        /* 腿释放指示 */
    SIPCP_MDU_SESSION_REFRESH_IND,    /* 会话刷新指示，用于媒体协商 */
    SIPCP_MDU_SESSION_END_IND,        /* 会话终结指示，用于表示会话终结，但是还存在订阅的情况 */

    /* 确认CP->CC */
    SIPCP_MDU_CREAT_LEG_CFM,

    /* 事件通知(用户事件如拨号，网络事件如反极、脉冲等)，双向 */
    SIPCP_MDU_DIAL_NUM_NTY,

    SIPCP_MDU_MSG_TYPE_BUTT
}SIPCP_MDU_TYPE_E;

/* 腿类型枚举 */
typedef enum tagSIPCP_LEG_TYPE_E
{
    SIPCP_LEG_TYPE_CONTROL,          /* 业务控制腿 */
    SIPCP_LEG_TYPE_CALL,             /* 普通呼叫腿 */
    SIPCP_LEG_TYPE_ALL,              /* 所有腿，用于腿操作 */
    SIPCP_LEG_TYPE_ANY,              /* 任何腿，用于腿匹配 */
    SIPCP_LEG_TYPE_BUTT
}SIPCP_LEG_TYPE_E;

/*呼叫腿子类型枚举*/
typedef enum tagSIPCP_CLEG_TYPE_E
{
    SIPCP_CLEG_TYPE_CURR,           /* 当前呼叫腿 */
    SIPCP_CLEG_TYPE_N_CURR,         /* 非当前呼叫腿 */
    SIPCP_CLEG_TYPE_ALL,            /* 所有呼叫腿，用于腿操作 */
    SIPCP_CLEG_TYPE_ANY,            /* 任何呼叫腿，用于腿匹配 */
    SIPCP_CLEG_TYPE_BUTT
}SIPCP_CLEG_TYPE_E;

typedef enum tagSIPCP_ACTION_E
{
    /*命令下发*/
    SIPCP_ACTION_CREATE_LEG,
    SIPCP_ACTION_CONFIRM_LEG,
    SIPCP_ACTION_RELEASE_LEG,
    SIPCP_ACTION_REFER_LEG,
    SIPCP_ACTION_HOLD_LEG,
    SIPCP_ACTION_RESUME_LEG,

    SIPCP_ACTION_DEACTIVE_LEG,
    SIPCP_ACTION_ACTIVE_LEG,

    /* 后续ACTION认为任何时候都不会失败 */
    SIPCP_ACTION_START_TIMER,
    SIPCP_ACTION_STOP_TIMER,

    /*这两个ACTION必须放在最后，防止引起混乱*/
    SIPCP_ACTION_START_SIGNAL,
    SIPCP_ACTION_SET_CURRENT_LEG,
    SIPCP_ACTION_BUTT
}SIPCP_ACTION_E;

/* 腿的创建方式 */
typedef enum tagSIPCP_CREATE_METHOD_E
{
    /*命令下发*/
    SIPCP_CREATE_METHOD_OFFHOOK,  /* 摘机直接上报 */
    SIPCP_CREATE_METHOD_OVERLAP,  /* 重叠发号 */
    SIPCP_CREATE_METHOD_HOTLINE,  /* 热线URI地址 */
    SIPCP_CREATE_METHOD_CONF,     /* 会议URI地址 */
    SIPCP_CREATE_METHOD_USER,     /* 根据普通号码产生URI地址 */
    SIPCP_CREATE_METHOD_EME,      /* 根据紧急号码产生URI地址 */
    SIPCP_CREATE_METHOD_SCC,      /* 根据SCC号码产生URI地址 */
    SIPCP_CREATE_METHOD_BUTT
}SIPCP_CREATE_METHOD_E;

/* 腿的激活模式 */
typedef enum tagSIPCP_ACTIVE_MODE_E
{
    /*命令下发*/
    SIPCP_ACTIVE_MODE_PREEMPT,    /* 资源抢占 */
    SIPCP_ACTIVE_MODE_MIX,        /* 混音方式 */
    SIPCP_ACTIVE_MODE_R_TONE,     /* 远端放音 */
    SIPCP_ACTIVE_MODE_BUTT
}SIPCP_ACTIVE_MODE_E;

/* 信号类型 */
typedef enum tagSIPCP_SIGNAL_TYPE_E
{
    /*提示音信号*/
    SIPCP_SIGNAL_TONE_DIAL,
    SIPCP_SIGNAL_TONE_CW,
    SIPCP_SIGNAL_TONE_RINGBACK,

    /*线路信号*/
    SIPCP_SIGNAL_LINE_RING,

    /*复合信号*/
    SIPCP_SIGNAL_MULT_CID1,
    SIPCP_SIGNAL_MULT_CID2,
    SIPCP_SIGNAL_MULT_AOCE,
    SIPCP_SIGNAL_MULT_WAITHOOK,

    SIPCP_SIGNAL_TYPE_BUTT
}SIPCP_SIGNAL_TYPE_E;

/* 信号播放方式 */
typedef enum tagSIPCP_SIGNAL_PLAY_E
{
    SIPCP_SIGNAL_PLAY_L,     /*本地放音*/
    SIPCP_SIGNAL_PLAY_L_MIX, /*本地混音*/
    SIPCP_SIGNAL_PLAY_R,     /*远端放音*/
    SIPCP_SIGNAL_PLAY_R_MIX, /*远端混音*/
    SIPCP_SIGNAL_PLAY_BUTT
}SIPCP_SIGNAL_PLAY_E;


/* 业务ID */
typedef enum tagSIPCP_SERVICE_ID_E
{
      SIPCP_SERVICE_ID_NORMAL,   /* NORMAL */
      SIPCP_SERVICE_ID_MCID  ,   /* MCID */
      SIPCP_SERVICE_ID_CW    ,   /* CW */
      SIPCP_SERVICE_ID_HOLD  ,   /* HOLD */
      SIPCP_SERVICE_ID_CONF  ,   /* CONF */
      SIPCP_SERVICE_ID_3PTY  ,   /* 3PTY */
      SIPCP_SERVICE_ID_ALL  ,    /* ALL */
      SIPCP_SERVICE_ID_BUTT
}SIPCP_SERVICE_ID_E;

/* 业务掩码 */
typedef enum tagSIPCP_SERVICE_MASK_E
{
    SIPCP_SERVICE_MASK_NORMAL   = 0x01,           /* NORMAL */
    SIPCP_SERVICE_MASK_MCID     = 0x02,           /* MCID */
    SIPCP_SERVICE_MASK_CW       = 0x04,           /* CW */
    SIPCP_SERVICE_MASK_HOLD     = 0x08,           /* HOLD */
    SIPCP_SERVICE_MASK_CONF     = 0x10,           /* CONF */
    SIPCP_SERVICE_MASK_3PTY     = 0x20,           /* 3PTY */
    SIPCP_SERVICE_MASK_ALL      = 0xffffffff,     /* 所有业务 */
}SIPCP_SERVICE_MASK_E;

/* 腿状态 */
typedef enum tagSIPCP_LEG_STATE_E
{
    SIPCP_LEG_STATE_N,
    SIPCP_LEG_STATE_W,
    SIPCP_LEG_STATE_A,
    SIPCP_LEG_STATE_D,/*考虑删除该状态，使用是否协商标记来做处理*/
    SIPCP_LEG_STATE_H,
    SIPCP_LEG_STATE_ANY,
    SIPCP_LEG_STATE_BUTT
}SIPCP_LEG_STATE_E;

/* 综合腿状态 */
typedef enum tagSIPCP_LEGS_STATE_E
{
    SIPCP_LEGS_STATE_N,
    SIPCP_LEGS_STATE_W,
    SIPCP_LEGS_STATE_A,
    SIPCP_LEGS_STATE_D,
    SIPCP_LEGS_STATE_H,
    SIPCP_LEGS_STATE_AW,
    SIPCP_LEGS_STATE_HH,
    SIPCP_LEGS_STATE_AA,
    SIPCP_LEGS_STATE_AH,
    SIPCP_LEGS_STATE_ANY,
    SIPCP_LEGS_STATE_BUTT
}SIPCP_LEGS_STATE_E;

/* 事件枚举 */
typedef enum tagSIPCP_EVENT_E
{
    /*物理事件*/
    SIPCP_EVENT_OFFHOOK,
    SIPCP_EVENT_ONHOOK,
    SIPCP_EVENT_HOOK,

    /*单个拨号事件*/
    SIPCP_EVENT_SOC0,
    SIPCP_EVENT_SOC1,
    SIPCP_EVENT_SOC2,
    SIPCP_EVENT_SOC3,
    SIPCP_EVENT_SOC4,
    SIPCP_EVENT_SOC5,
    SIPCP_EVENT_SOC6,
    SIPCP_EVENT_SOC7,
    SIPCP_EVENT_SOC8,
    SIPCP_EVENT_SOC9,
    SIPCP_EVENT_SOCE,
    SIPCP_EVENT_SOCF,
    SIPCP_EVENT_SOCX, /*任意单个拨号事件*/

    /*数图事件*/
    SIPCP_EVENT_SCC,
    SIPCP_EVENT_INIT,
    SIPCP_EVENT_DIAL_FAIL,

    /*内部事件*/
    SIPCP_EVENT_SRV_TIMEOUT,
    SIPCP_EVENT_SIGNAL_COMP,

    /*网络事件*/
    SIPCP_EVENT_LEG_PEER_ALERT,
    SIPCP_EVENT_LEG_CREATE_SUCCESS,
    SIPCP_EVENT_LEG_RELEASE,
    SIPCP_EVENT_LEG_NEGO_SUCCESS,
    SIPCP_EVENT_LEG_NEGO_FAIL,
    SIPCP_EVENT_LEG_REFER_SUCCESS,
    SIPCP_EVENT_LEG_REFER_FAIL,
    SIPCP_EVENT_LEG_CREATE_IND,
    SIPCP_EVENT_BUTT
}SIPCP_EVENT_E;

/* 业务定时器类型 */
typedef enum tagSIPCP_TIMER_E
{
    SIPCP_TIMER_HOTLINE,  /*热线定时器*/
    SIPCP_TIMER_CW,       /*呼等定时器*/
    SIPCP_TIMER_BACKRING, /*回振定时器*/
    SIPCP_TIMER_BUTT
}SIPCP_TIMER_E;

/* 特殊处理标记，枚举形式，与掩码对应 */
typedef enum tagSIPCP_FLAG_E
{
    SIPCP_FLAG_KA,      /* 信号保持激活 */
    SIPCP_FLAG_KT,      /* 保持定时器 */
    SIPCP_FLAG_BUTT
}SIPCP_FLAG_E;

/* 特殊处理标记，掩码形式 */
typedef enum tagSIPCP_FLAG_MASK_E
{
    SIPCP_FLAG_MASK_NULL = 0,    /* 无特殊标记 */
    SIPCP_FLAG_MASK_KA   = 0x01, /* 信号保持激活 */
}SIPCP_FLAG_MASK_E;


/* 入口条件标记，枚举形式，与掩码对应 */
typedef enum tagSIPCP_CONDITION_E
{
    SIPCP_CONDITION_CALLER    , /* 主叫 */
    SIPCP_CONDITION_CALLEE    , /* 被叫 */
    SIPCP_CONDITION_OFFHOOK   , /* 摘机 */
    SIPCP_CONDITION_ONHOOK    , /* 挂机 */
    SIPCP_CONDITION_PSTNPORT  , /* PSTN端口 */
    SIPCP_CONDITION_ISDNPORT  , /* ISDN端口 */
    SIPCP_CONDITION_PRAPORT   , /* PRA端口 */
    SIPCP_CONDITION_NONEGO    , /* 非协商 */
    SIPCP_CONDITION_PROFILE20 , /* profile20未设置 */
//    SIPCP_CONDITION_CALLEE_FIRST = 0x04, /* 被叫优先 */
    SIPCP_CONDITION_BUTT
}SIPCP_CONDITION_E;

/* 入口条件标记，掩码形式 */
typedef enum tagSIPCP_CONDITION_MASK_E
{
    SIPCP_CONDITION_MASK_NULL         = 0,    /* 无条件 */
    SIPCP_CONDITION_MASK_CALLER       = 0x01, /* 主叫 */
    SIPCP_CONDITION_MASK_CALLEE       = 0x02, /* 被叫 */
    SIPCP_CONDITION_MASK_OFFHOOK      = 0x04, /* 摘机 */
    SIPCP_CONDITION_MASK_ONHOOK       = 0x08, /* 挂机 */
    SIPCP_CONDITION_MASK_PSTNPORT     = 0x10, /* PSTN端口 */
    SIPCP_CONDITION_MASK_ISDNPORT     = 0x20, /* ISDN端口 */
    SIPCP_CONDITION_MASK_PRAPORT      = 0x40, /* PRA端口 */
    SIPCP_CONDITION_MASK_NONEGO       = 0x80, /* 非协商 */
    SIPCP_CONDITION_MASK_PROFILE20    = 0x100, /* profile20未设置 */
//    SIPCP_CONDITION_MASK_CALLEE_FIRST = 0x04, /* 被叫优先 */
}SIPCP_CONDITION_MASK_E;

#else
#endif