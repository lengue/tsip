#ifndef SIP_CP_CONST_H
#define SIP_CP_CONST_H

/*�¼�������Ŀ*/
#define SIPCP_EVENT_PARA_NUM 2
/*ҵ���߼�ACTION������Ŀ*/
#define SIPCP_ACTION_PARA_NUM 5
/*ҵ���߼�ACTION��Ŀ*/
#define SIPCP_ACTION_NUM 10
/*���ӿ���*/
#define SIPCP_MAX_INTERFACE_NUM 8
/*MDU���������Ϣ��*/
#define SIPCP_MAX_MDU_NUM  10
/*SPCB�е�����ȸ���*/
#define SIPCP_LEG_NUM_IN_SPCB 3

/*SPCB�е����۲��¼�����*/
#define SIPCP_MAX_OEVENT_NUM 10

/*ҵ���߼��汾��ʶ����*/
#define SIPCP_SL_VERSION_LEN 20

/*SPCB�е�������Ӹ���*/
#define SIPCP_CONN_NUM_IN_SPCB SIPCP_LEG_NUM_IN_SPCB

/*���ҵ���߼���*/
#define SIPCP_MAX_SL_ITEMS 1024

typedef enum tagSIPCP_MDU_TYPE_E
{
    /* ����CP->CC */
    SIPCP_MDU_CREAT_LEG_CMD  ,
    SIPCP_MDU_RELEASE_LEG_CMD,/*�ͷ��ȣ��ͷ������Ҫͨ��*/
    SIPCP_MDU_CLEAR_LEG_CMD  ,  /*�ͷ��ȣ��ͷ��������ͨ��*/
    SIPCP_MDU_REFER_LEG_CMD  ,
    SIPCP_MDU_HOLD_LEG_CMD   ,
    SIPCP_MDU_RESUME_LEG_CMD ,
    SIPCP_MDU_REFRESH_LEG_CMD,

    /* ��ӦCP->CC */
    SIPCP_MDU_CREAT_LEG_ACK  ,
    SIPCP_MDU_REFER_LEG_ACK  ,
    SIPCP_MDU_REFRESH_LEG_ACK,

    /* ָʾCP->CC */
    SIPCP_MDU_CREAT_LEG_IND,          /* �յ�INVITE���ȴ���ָʾ */
    SIPCP_MDU_RELEASE_LEG_IND,        /* ���ͷ�ָʾ */
    SIPCP_MDU_SESSION_REFRESH_IND,    /* �Ựˢ��ָʾ������ý��Э�� */
    SIPCP_MDU_SESSION_END_IND,        /* �Ự�ս�ָʾ�����ڱ�ʾ�Ự�սᣬ���ǻ����ڶ��ĵ���� */

    /* ȷ��CP->CC */
    SIPCP_MDU_CREAT_LEG_CFM,

    /* �¼�֪ͨ(�û��¼��粦�ţ������¼��練���������)��˫�� */
    SIPCP_MDU_DIAL_NUM_NTY,

    SIPCP_MDU_MSG_TYPE_BUTT
}SIPCP_MDU_TYPE_E;

/* ������ö�� */
typedef enum tagSIPCP_LEG_TYPE_E
{
    SIPCP_LEG_TYPE_CONTROL,          /* ҵ������� */
    SIPCP_LEG_TYPE_CALL,             /* ��ͨ������ */
    SIPCP_LEG_TYPE_ALL,              /* �����ȣ������Ȳ��� */
    SIPCP_LEG_TYPE_ANY,              /* �κ��ȣ�������ƥ�� */
    SIPCP_LEG_TYPE_BUTT
}SIPCP_LEG_TYPE_E;

/*������������ö��*/
typedef enum tagSIPCP_CLEG_TYPE_E
{
    SIPCP_CLEG_TYPE_CURR,           /* ��ǰ������ */
    SIPCP_CLEG_TYPE_N_CURR,         /* �ǵ�ǰ������ */
    SIPCP_CLEG_TYPE_ALL,            /* ���к����ȣ������Ȳ��� */
    SIPCP_CLEG_TYPE_ANY,            /* �κκ����ȣ�������ƥ�� */
    SIPCP_CLEG_TYPE_BUTT
}SIPCP_CLEG_TYPE_E;

typedef enum tagSIPCP_ACTION_E
{
    /*�����·�*/
    SIPCP_ACTION_CREATE_LEG,
    SIPCP_ACTION_CONFIRM_LEG,
    SIPCP_ACTION_RELEASE_LEG,
    SIPCP_ACTION_REFER_LEG,
    SIPCP_ACTION_HOLD_LEG,
    SIPCP_ACTION_RESUME_LEG,

    SIPCP_ACTION_DEACTIVE_LEG,
    SIPCP_ACTION_ACTIVE_LEG,

    /* ����ACTION��Ϊ�κ�ʱ�򶼲���ʧ�� */
    SIPCP_ACTION_START_TIMER,
    SIPCP_ACTION_STOP_TIMER,

    /*������ACTION���������󣬷�ֹ�������*/
    SIPCP_ACTION_START_SIGNAL,
    SIPCP_ACTION_SET_CURRENT_LEG,
    SIPCP_ACTION_BUTT
}SIPCP_ACTION_E;

/* �ȵĴ�����ʽ */
typedef enum tagSIPCP_CREATE_METHOD_E
{
    /*�����·�*/
    SIPCP_CREATE_METHOD_OFFHOOK,  /* ժ��ֱ���ϱ� */
    SIPCP_CREATE_METHOD_OVERLAP,  /* �ص����� */
    SIPCP_CREATE_METHOD_HOTLINE,  /* ����URI��ַ */
    SIPCP_CREATE_METHOD_CONF,     /* ����URI��ַ */
    SIPCP_CREATE_METHOD_USER,     /* ������ͨ�������URI��ַ */
    SIPCP_CREATE_METHOD_EME,      /* ���ݽ����������URI��ַ */
    SIPCP_CREATE_METHOD_SCC,      /* ����SCC�������URI��ַ */
    SIPCP_CREATE_METHOD_BUTT
}SIPCP_CREATE_METHOD_E;

/* �ȵļ���ģʽ */
typedef enum tagSIPCP_ACTIVE_MODE_E
{
    /*�����·�*/
    SIPCP_ACTIVE_MODE_PREEMPT,    /* ��Դ��ռ */
    SIPCP_ACTIVE_MODE_MIX,        /* ������ʽ */
    SIPCP_ACTIVE_MODE_R_TONE,     /* Զ�˷��� */
    SIPCP_ACTIVE_MODE_BUTT
}SIPCP_ACTIVE_MODE_E;

/* �ź����� */
typedef enum tagSIPCP_SIGNAL_TYPE_E
{
    /*��ʾ���ź�*/
    SIPCP_SIGNAL_TONE_DIAL,
    SIPCP_SIGNAL_TONE_CW,
    SIPCP_SIGNAL_TONE_RINGBACK,

    /*��·�ź�*/
    SIPCP_SIGNAL_LINE_RING,

    /*�����ź�*/
    SIPCP_SIGNAL_MULT_CID1,
    SIPCP_SIGNAL_MULT_CID2,
    SIPCP_SIGNAL_MULT_AOCE,
    SIPCP_SIGNAL_MULT_WAITHOOK,

    SIPCP_SIGNAL_TYPE_BUTT
}SIPCP_SIGNAL_TYPE_E;

/* �źŲ��ŷ�ʽ */
typedef enum tagSIPCP_SIGNAL_PLAY_E
{
    SIPCP_SIGNAL_PLAY_L,     /*���ط���*/
    SIPCP_SIGNAL_PLAY_L_MIX, /*���ػ���*/
    SIPCP_SIGNAL_PLAY_R,     /*Զ�˷���*/
    SIPCP_SIGNAL_PLAY_R_MIX, /*Զ�˻���*/
    SIPCP_SIGNAL_PLAY_BUTT
}SIPCP_SIGNAL_PLAY_E;


/* ҵ��ID */
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

/* ҵ������ */
typedef enum tagSIPCP_SERVICE_MASK_E
{
    SIPCP_SERVICE_MASK_NORMAL   = 0x01,           /* NORMAL */
    SIPCP_SERVICE_MASK_MCID     = 0x02,           /* MCID */
    SIPCP_SERVICE_MASK_CW       = 0x04,           /* CW */
    SIPCP_SERVICE_MASK_HOLD     = 0x08,           /* HOLD */
    SIPCP_SERVICE_MASK_CONF     = 0x10,           /* CONF */
    SIPCP_SERVICE_MASK_3PTY     = 0x20,           /* 3PTY */
    SIPCP_SERVICE_MASK_ALL      = 0xffffffff,     /* ����ҵ�� */
}SIPCP_SERVICE_MASK_E;

/* ��״̬ */
typedef enum tagSIPCP_LEG_STATE_E
{
    SIPCP_LEG_STATE_N,
    SIPCP_LEG_STATE_W,
    SIPCP_LEG_STATE_A,
    SIPCP_LEG_STATE_D,/*����ɾ����״̬��ʹ���Ƿ�Э�̱����������*/
    SIPCP_LEG_STATE_H,
    SIPCP_LEG_STATE_ANY,
    SIPCP_LEG_STATE_BUTT
}SIPCP_LEG_STATE_E;

/* �ۺ���״̬ */
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

/* �¼�ö�� */
typedef enum tagSIPCP_EVENT_E
{
    /*�����¼�*/
    SIPCP_EVENT_OFFHOOK,
    SIPCP_EVENT_ONHOOK,
    SIPCP_EVENT_HOOK,

    /*���������¼�*/
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
    SIPCP_EVENT_SOCX, /*���ⵥ�������¼�*/

    /*��ͼ�¼�*/
    SIPCP_EVENT_SCC,
    SIPCP_EVENT_INIT,
    SIPCP_EVENT_DIAL_FAIL,

    /*�ڲ��¼�*/
    SIPCP_EVENT_SRV_TIMEOUT,
    SIPCP_EVENT_SIGNAL_COMP,

    /*�����¼�*/
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

/* ҵ��ʱ������ */
typedef enum tagSIPCP_TIMER_E
{
    SIPCP_TIMER_HOTLINE,  /*���߶�ʱ��*/
    SIPCP_TIMER_CW,       /*���ȶ�ʱ��*/
    SIPCP_TIMER_BACKRING, /*����ʱ��*/
    SIPCP_TIMER_BUTT
}SIPCP_TIMER_E;

/* ���⴦����ǣ�ö����ʽ���������Ӧ */
typedef enum tagSIPCP_FLAG_E
{
    SIPCP_FLAG_KA,      /* �źű��ּ��� */
    SIPCP_FLAG_KT,      /* ���ֶ�ʱ�� */
    SIPCP_FLAG_BUTT
}SIPCP_FLAG_E;

/* ���⴦����ǣ�������ʽ */
typedef enum tagSIPCP_FLAG_MASK_E
{
    SIPCP_FLAG_MASK_NULL = 0,    /* �������� */
    SIPCP_FLAG_MASK_KA   = 0x01, /* �źű��ּ��� */
}SIPCP_FLAG_MASK_E;


/* ���������ǣ�ö����ʽ���������Ӧ */
typedef enum tagSIPCP_CONDITION_E
{
    SIPCP_CONDITION_CALLER    , /* ���� */
    SIPCP_CONDITION_CALLEE    , /* ���� */
    SIPCP_CONDITION_OFFHOOK   , /* ժ�� */
    SIPCP_CONDITION_ONHOOK    , /* �һ� */
    SIPCP_CONDITION_PSTNPORT  , /* PSTN�˿� */
    SIPCP_CONDITION_ISDNPORT  , /* ISDN�˿� */
    SIPCP_CONDITION_PRAPORT   , /* PRA�˿� */
    SIPCP_CONDITION_NONEGO    , /* ��Э�� */
    SIPCP_CONDITION_PROFILE20 , /* profile20δ���� */
//    SIPCP_CONDITION_CALLEE_FIRST = 0x04, /* �������� */
    SIPCP_CONDITION_BUTT
}SIPCP_CONDITION_E;

/* ���������ǣ�������ʽ */
typedef enum tagSIPCP_CONDITION_MASK_E
{
    SIPCP_CONDITION_MASK_NULL         = 0,    /* ������ */
    SIPCP_CONDITION_MASK_CALLER       = 0x01, /* ���� */
    SIPCP_CONDITION_MASK_CALLEE       = 0x02, /* ���� */
    SIPCP_CONDITION_MASK_OFFHOOK      = 0x04, /* ժ�� */
    SIPCP_CONDITION_MASK_ONHOOK       = 0x08, /* �һ� */
    SIPCP_CONDITION_MASK_PSTNPORT     = 0x10, /* PSTN�˿� */
    SIPCP_CONDITION_MASK_ISDNPORT     = 0x20, /* ISDN�˿� */
    SIPCP_CONDITION_MASK_PRAPORT      = 0x40, /* PRA�˿� */
    SIPCP_CONDITION_MASK_NONEGO       = 0x80, /* ��Э�� */
    SIPCP_CONDITION_MASK_PROFILE20    = 0x100, /* profile20δ���� */
//    SIPCP_CONDITION_MASK_CALLEE_FIRST = 0x04, /* �������� */
}SIPCP_CONDITION_MASK_E;

#else
#endif