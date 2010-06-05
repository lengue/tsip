/*******************************************************************************
功能    : SIP协议栈传输层功能函数
创建人  : 唐春平
创建日期: 2009.01.10
修改记录:
*******************************************************************************/
/* 外部依赖模块头文件 */
#include "common\common.h"
#include "abnf\abnf.h"
#include "ubuf\ubuf.h"
#include "uri\uri.h"
#include "sip\sip.h"
#include "s_hash\s_hash.h"

#include "..\include\shell\sip_shell.h"
#include "..\include\transport\sip_transport.h"
#include "..\include\tu\sip_tu.h"

/* 本模块对外提供的常量和结构头文件 */
#include "..\include\transaction\sip_transaction.h"

/* 本模块内部使用的常量和结构头文件 */
#include "sip_txn_const.inc"
#include "sip_txn_type.inc"

/* 本模块内部函数声明头文件 */
#include "sip_txn_fsm.inc"
#include "sip_txn_mngt.inc"

/* 本模块全局变量声明头文件 */
#include "sip_txn_var.inc"

ULONG SIP_Txn_FsmInit()
{
    SIP_TXN_TYPE_E eType;
    SIP_TXN_STATE_E eState;

    /* 先全部置空 */
    memset(g_astSipTxnFsm,
           0x0,
           SIP_TXN_TYPE_BUTT
         * SIP_TXN_STATE_TERMINATED
         * SIP_TXN_EVENT_BUTT
         * sizeof(SIP_TXN_FSM_S));

    /* INVITE客户端事务状态机
                                   |INVITE from TU
                 Timer A fires     |INVITE sent
                 Reset A,          V                      Timer B fires
                 INVITE sent +-----------+                or Transport Err.
                   +---------|           |---------------+inform TU
                   |         |  Calling  |               |
                   +-------->|           |-------------->|
                             +-----------+ 2xx           |
                                |  |       2xx to TU     |
                                |  |1xx                  |
        300-699 +---------------+  |1xx to TU            |
       ACK sent |                  |                     |
    resp. to TU |  1xx             V                     |
                |  1xx to TU  -----------+               |
                |  +---------|           |               |
                |  |         |Proceeding |-------------->|
                |  +-------->|           | 2xx           |
                |            +-----------+ 2xx to TU     |
                |       300-699    |                     |
                |       ACK sent,  |                     |
                |       resp. to TU|                     |
                |                  |                     |      NOTE:
                |  300-699         V                     |
                |  ACK sent  +-----------+Transport Err. |  transitions
                |  +---------|           |Inform TU      |  labeled with
                |  |         | Completed |-------------->|  the event
                |  +-------->|           |               |  over the action
                |            +-----------+               |  to take
                |              ^   |                     |
                |              |   | Timer D fires       |
                +--------------+   | -                   |
                                   |                     |
                                   V                     |
                             +-----------+               |
                             |           |               |
                             | Terminated|<--------------+
                             |           |
                             +-----------+
    */
    eType = SIP_TXN_TYPE_INVITE_CLIENT;

    eState = SIP_TXN_STATE_INIT;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_REQUEST].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_REQUEST].astActions[0].pfnProc = SIP_Txn_ActSendMsg;
    /*不可靠才重传*/
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_REQUEST].astActions[1].pfnProc = SIP_Txn_ActStartTimer;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_REQUEST].astActions[1].ulPara  = SIP_TXN_TIMER_A;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_REQUEST].astActions[2].pfnProc = SIP_Txn_ActStartTimer;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_REQUEST].astActions[2].ulPara  = SIP_TXN_TIMER_B;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_REQUEST].eNextState = SIP_TXN_STATE_CALLING;

    eState = SIP_TXN_STATE_CALLING;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TIMER_A_FIRE].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TIMER_A_FIRE].astActions[0].pfnProc = SIP_Txn_ActReSendRequest;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TIMER_A_FIRE].astActions[1].pfnProc = SIP_Txn_ActStartTimer;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TIMER_A_FIRE].astActions[1].ulPara  = SIP_TXN_TIMER_A;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TIMER_A_FIRE].eNextState = SIP_TXN_STATE_CALLING;

    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TIMER_B_FIRE].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TIMER_B_FIRE].astActions[0].pfnProc = SIP_Txn_ActInformTU;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TIMER_B_FIRE].eNextState = SIP_TXN_STATE_TERMINATED;

    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TXP_ERR].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TXP_ERR].astActions[0].pfnProc = SIP_Txn_ActInformTU;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TXP_ERR].eNextState = SIP_TXN_STATE_TERMINATED;

    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_1XX_RESPONSE].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_1XX_RESPONSE].astActions[0].pfnProc = SIP_Txn_ActPassMsg;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_1XX_RESPONSE].astActions[1].pfnProc = SIP_Txn_ActStopTimer;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_1XX_RESPONSE].astActions[1].ulPara  = SIP_TXN_TIMER_A;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_1XX_RESPONSE].astActions[2].pfnProc = SIP_Txn_ActStopTimer;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_1XX_RESPONSE].astActions[2].ulPara  = SIP_TXN_TIMER_B;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_1XX_RESPONSE].eNextState = SIP_TXN_STATE_PROCEEDING;

    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_2XX_RESPONSE].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_2XX_RESPONSE].astActions[0].pfnProc = SIP_Txn_ActPassMsg;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_2XX_RESPONSE].eNextState = SIP_TXN_STATE_TERMINATED;

    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_3456XX_RESPONSE].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_3456XX_RESPONSE].astActions[0].pfnProc = SIP_Txn_ActPassMsg;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_3456XX_RESPONSE].astActions[1].pfnProc = SIP_Txn_ActSendAck;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_3456XX_RESPONSE].astActions[2].pfnProc = SIP_Txn_ActStopTimer;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_3456XX_RESPONSE].astActions[2].ulPara  = SIP_TXN_TIMER_A;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_3456XX_RESPONSE].astActions[3].pfnProc = SIP_Txn_ActStopTimer;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_3456XX_RESPONSE].astActions[3].ulPara  = SIP_TXN_TIMER_B;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_3456XX_RESPONSE].eNextState = SIP_TXN_STATE_COMPLETED;

    eState = SIP_TXN_STATE_PROCEEDING;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_1XX_RESPONSE].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_1XX_RESPONSE].astActions[0].pfnProc = SIP_Txn_ActPassMsg;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_1XX_RESPONSE].eNextState = SIP_TXN_STATE_PROCEEDING;

    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_2XX_RESPONSE].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_2XX_RESPONSE].astActions[0].pfnProc = SIP_Txn_ActPassMsg;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_2XX_RESPONSE].eNextState = SIP_TXN_STATE_TERMINATED;

    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_3456XX_RESPONSE].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_3456XX_RESPONSE].astActions[0].pfnProc = SIP_Txn_ActPassMsg;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_3456XX_RESPONSE].astActions[1].pfnProc = SIP_Txn_ActSendAck;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_3456XX_RESPONSE].astActions[2].pfnProc = SIP_Txn_ActStartTimer;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_3456XX_RESPONSE].astActions[2].ulPara  = SIP_TXN_TIMER_D;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_3456XX_RESPONSE].eNextState = SIP_TXN_STATE_COMPLETED;

    eState = SIP_TXN_STATE_COMPLETED;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_3456XX_RESPONSE].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_3456XX_RESPONSE].astActions[0].pfnProc = SIP_Txn_ActSendAck;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_3456XX_RESPONSE].eNextState = SIP_TXN_STATE_COMPLETED;

    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TXP_ERR].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TXP_ERR].astActions[0].pfnProc = SIP_Txn_ActInformTU;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TXP_ERR].eNextState = SIP_TXN_STATE_TERMINATED;

    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TIMER_D_FIRE].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TIMER_D_FIRE].eNextState = SIP_TXN_STATE_TERMINATED;

    /* 非INVITE客户端事务状态机
                                   |Request from TU
                                   |send request
               Timer E             V
               send request  +-----------+
                   +---------|           |-------------------+
                   |         |  Trying   |  Timer F          |
                   +-------->|           |  or Transport Err.|
                             +-----------+  inform TU        |
                200-699         |  |                         |
                resp. to TU     |  |1xx                      |
                +---------------+  |resp. to TU              |
                |                  |                         |
                |   Timer E        V       Timer F           |
                |   send req +-----------+ or Transport Err. |
                |  +---------|           | inform TU         |
                |  |         |Proceeding |------------------>|
                |  +-------->|           |-----+             |
                |            +-----------+     |1xx          |
                |              |      ^        |resp to TU   |
                | 200-699      |      +--------+             |
                | resp. to TU  |                             |
                |              |                             |
                |              V                             |
                |            +-----------+                   |
                |            |           |                   |
                |            | Completed |                   |
                |            |           |                   |
                |            +-----------+                   |
                |              ^   |                         |
                |              |   | Timer K                 |
                +--------------+   | -                       |
                                   |                         |
                                   V                         |
             NOTE:           +-----------+                   |
                             |           |                   |
         transitions         | Terminated|<------------------+
         labeled with        |           |
         the event           +-----------+
         over the action
         to take
    */
    eType = SIP_TXN_TYPE_NON_INVITE_CLIENT;

    eState = SIP_TXN_STATE_INIT;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_REQUEST].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_REQUEST].astActions[0].pfnProc = SIP_Txn_ActSendMsg;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_REQUEST].astActions[1].pfnProc = SIP_Txn_ActStartTimer;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_REQUEST].astActions[1].ulPara  = SIP_TXN_TIMER_E;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_REQUEST].astActions[2].pfnProc = SIP_Txn_ActStartTimer;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_REQUEST].astActions[2].ulPara  = SIP_TXN_TIMER_F;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_REQUEST].eNextState = SIP_TXN_STATE_TRYING;

    eState = SIP_TXN_STATE_TRYING;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TIMER_E_FIRE].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TIMER_E_FIRE].astActions[0].pfnProc = SIP_Txn_ActReSendRequest;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TIMER_E_FIRE].astActions[1].pfnProc = SIP_Txn_ActStartTimer;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TIMER_E_FIRE].astActions[1].ulPara  = SIP_TXN_TIMER_E;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TIMER_E_FIRE].eNextState = SIP_TXN_STATE_CALLING;

    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TIMER_F_FIRE].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TIMER_F_FIRE].astActions[0].pfnProc = SIP_Txn_ActInformTU;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TIMER_F_FIRE].eNextState = SIP_TXN_STATE_TERMINATED;

    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TXP_ERR].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TXP_ERR].astActions[0].pfnProc = SIP_Txn_ActInformTU;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TXP_ERR].eNextState = SIP_TXN_STATE_TERMINATED;

    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_1XX_RESPONSE].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_1XX_RESPONSE].astActions[0].pfnProc = SIP_Txn_ActPassMsg;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_1XX_RESPONSE].astActions[1].pfnProc = SIP_Txn_ActStopTimer;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_1XX_RESPONSE].astActions[1].ulPara  = SIP_TXN_TIMER_E;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_1XX_RESPONSE].astActions[2].pfnProc = SIP_Txn_ActStopTimer;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_1XX_RESPONSE].astActions[2].ulPara  = SIP_TXN_TIMER_F;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_1XX_RESPONSE].eNextState = SIP_TXN_STATE_PROCEEDING;

    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_2XX_RESPONSE].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_2XX_RESPONSE].astActions[0].pfnProc = SIP_Txn_ActPassMsg;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_2XX_RESPONSE].astActions[1].pfnProc = SIP_Txn_ActStopTimer;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_2XX_RESPONSE].astActions[1].ulPara  = SIP_TXN_TIMER_K;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_2XX_RESPONSE].eNextState = SIP_TXN_STATE_COMPLETED;

    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_3456XX_RESPONSE].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_3456XX_RESPONSE].astActions[0].pfnProc = SIP_Txn_ActPassMsg;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_3456XX_RESPONSE].astActions[1].pfnProc = SIP_Txn_ActStopTimer;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_3456XX_RESPONSE].astActions[1].ulPara  = SIP_TXN_TIMER_K;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_3456XX_RESPONSE].eNextState = SIP_TXN_STATE_COMPLETED;

    eState = SIP_TXN_STATE_PROCEEDING;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TIMER_E_FIRE].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TIMER_E_FIRE].astActions[0].pfnProc = SIP_Txn_ActReSendRequest;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TIMER_E_FIRE].astActions[1].pfnProc = SIP_Txn_ActStartTimer;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TIMER_E_FIRE].astActions[1].ulPara  = SIP_TXN_TIMER_E;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TIMER_E_FIRE].eNextState = SIP_TXN_STATE_CALLING;

    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TIMER_F_FIRE].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TIMER_F_FIRE].astActions[0].pfnProc = SIP_Txn_ActInformTU;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TIMER_F_FIRE].eNextState = SIP_TXN_STATE_TERMINATED;

    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TXP_ERR].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TXP_ERR].astActions[0].pfnProc = SIP_Txn_ActInformTU;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TXP_ERR].eNextState = SIP_TXN_STATE_TERMINATED;

    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_1XX_RESPONSE].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_1XX_RESPONSE].astActions[0].pfnProc = SIP_Txn_ActPassMsg;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_1XX_RESPONSE].eNextState = SIP_TXN_STATE_PROCEEDING;

    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_2XX_RESPONSE].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_2XX_RESPONSE].astActions[0].pfnProc = SIP_Txn_ActPassMsg;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_2XX_RESPONSE].astActions[1].pfnProc = SIP_Txn_ActStopTimer;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_2XX_RESPONSE].astActions[1].ulPara  = SIP_TXN_TIMER_E;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_2XX_RESPONSE].astActions[2].pfnProc = SIP_Txn_ActStopTimer;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_2XX_RESPONSE].astActions[2].ulPara  = SIP_TXN_TIMER_F;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_2XX_RESPONSE].astActions[3].pfnProc = SIP_Txn_ActStartTimer;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_2XX_RESPONSE].astActions[3].ulPara  = SIP_TXN_TIMER_K;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_2XX_RESPONSE].eNextState = SIP_TXN_STATE_COMPLETED;

    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_3456XX_RESPONSE].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_3456XX_RESPONSE].astActions[0].pfnProc = SIP_Txn_ActPassMsg;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_3456XX_RESPONSE].astActions[1].pfnProc = SIP_Txn_ActStopTimer;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_3456XX_RESPONSE].astActions[1].ulPara  = SIP_TXN_TIMER_E;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_3456XX_RESPONSE].astActions[2].pfnProc = SIP_Txn_ActStopTimer;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_3456XX_RESPONSE].astActions[2].ulPara  = SIP_TXN_TIMER_F;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_3456XX_RESPONSE].astActions[3].pfnProc = SIP_Txn_ActStartTimer;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_3456XX_RESPONSE].astActions[3].ulPara  = SIP_TXN_TIMER_K;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_3456XX_RESPONSE].eNextState = SIP_TXN_STATE_COMPLETED;

    eState = SIP_TXN_STATE_COMPLETED;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TIMER_K_FIRE].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TIMER_K_FIRE].eNextState = SIP_TXN_STATE_TERMINATED;


    /* INVITE服务器端事务状态机
                               |INVITE
                               |pass INV to TU
            INVITE             V send 100 if TU won't in 200ms
            send response+-----------+
                +--------|           |--------+101-199 from TU
                |        | Proceeding|        |send response
                +------->|           |<-------+
                         |           |          Transport Err.
                         |           |          Inform TU
                         |           |--------------->+
                         +-----------+                |
            300-699 from TU |     |2xx from TU        |
            send response   |     |send response      |
                            |     +------------------>+
                            |                         |
            INVITE          V          Timer G fires  |
            send response+-----------+ send response  |
                +--------|           |--------+       |
                |        | Completed |        |       |
                +------->|           |<-------+       |
                         +-----------+                |
                            |     |                   |
                        ACK |     |                   |
                        -   |     +------------------>+
                            |        Timer H fires    |
                            V        or Transport Err.|
                         +-----------+  Inform TU     |
                         |           |                |
                         | Confirmed |                |
                         |           |                |
                         +-----------+                |
                               |                      |
                               |Timer I fires         |
                               |-                     |
                               |                      |
                               V                      |
                         +-----------+                |
                         |           |                |
                         | Terminated|<---------------+
                         |           |
                         +-----------+
    */
    eType = SIP_TXN_TYPE_INVITE_SERVER;

    eState = SIP_TXN_STATE_INIT;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_REQUEST].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_REQUEST].astActions[0].pfnProc = SIP_Txn_ActPassMsg;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_REQUEST].eNextState = SIP_TXN_STATE_PROCEEDING;

    eState = SIP_TXN_STATE_PROCEEDING;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_REQUEST].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_REQUEST].astActions[0].pfnProc = SIP_Txn_ActReSendResponse;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_REQUEST].eNextState = SIP_TXN_STATE_PROCEEDING;

    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_1XX_RESPONSE].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_1XX_RESPONSE].astActions[0].pfnProc = SIP_Txn_ActSendMsg;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_1XX_RESPONSE].eNextState = SIP_TXN_STATE_PROCEEDING;

    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_2XX_RESPONSE].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_2XX_RESPONSE].astActions[0].pfnProc = SIP_Txn_ActSendMsg;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_2XX_RESPONSE].eNextState = SIP_TXN_STATE_TERMINATED;

    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_3456XX_RESPONSE].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_3456XX_RESPONSE].astActions[0].pfnProc = SIP_Txn_ActSendMsg;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_3456XX_RESPONSE].astActions[1].pfnProc = SIP_Txn_ActStartTimer;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_3456XX_RESPONSE].astActions[1].ulPara  = SIP_TXN_TIMER_G;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_3456XX_RESPONSE].astActions[2].pfnProc = SIP_Txn_ActStartTimer;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_3456XX_RESPONSE].astActions[2].ulPara  = SIP_TXN_TIMER_H;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_3456XX_RESPONSE].eNextState = SIP_TXN_STATE_COMPLETED;

    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TXP_ERR].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TXP_ERR].astActions[0].pfnProc = SIP_Txn_ActInformTU;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TXP_ERR].eNextState = SIP_TXN_STATE_TERMINATED;

    eState = SIP_TXN_STATE_COMPLETED;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_REQUEST].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_REQUEST].astActions[0].pfnProc = SIP_Txn_ActReSendResponse;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_REQUEST].eNextState = SIP_TXN_STATE_COMPLETED;

    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TIMER_G_FIRE].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TIMER_G_FIRE].astActions[0].pfnProc = SIP_Txn_ActReSendResponse;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TIMER_G_FIRE].eNextState = SIP_TXN_STATE_COMPLETED;

    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TIMER_H_FIRE].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TIMER_H_FIRE].astActions[0].pfnProc = SIP_Txn_ActInformTU;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TIMER_H_FIRE].eNextState = SIP_TXN_STATE_TERMINATED;

    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TXP_ERR].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TXP_ERR].astActions[0].pfnProc = SIP_Txn_ActInformTU;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TXP_ERR].eNextState = SIP_TXN_STATE_TERMINATED;

    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_ACK].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_ACK].astActions[0].pfnProc = SIP_Txn_ActStopTimer;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_ACK].astActions[0].ulPara  = SIP_TXN_TIMER_G;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_ACK].astActions[1].pfnProc = SIP_Txn_ActStopTimer;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_ACK].astActions[1].ulPara  = SIP_TXN_TIMER_H;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_ACK].astActions[2].pfnProc = SIP_Txn_ActStartTimer;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_ACK].astActions[2].ulPara  = SIP_TXN_TIMER_I;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_ACK].eNextState = SIP_TXN_STATE_CONFIRMED;

    eState = SIP_TXN_STATE_CONFIRMED;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TIMER_I_FIRE].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TIMER_I_FIRE].eNextState = SIP_TXN_STATE_TERMINATED;

    /* 非INVITE服务器端事务状态机
                                  |Request received
                                  |pass to TU
                                  V
                            +-----------+
                            |           |
                            | Trying    |-------------+
                            |           |             |
                            +-----------+             |200-699 from TU
                                  |                   |send response
                                  |1xx from TU        |
                                  |send response      |
                                  |                   |
               Request            V      1xx from TU  |
               send response+-----------+send response|
                   +--------|           |--------+    |
                   |        | Proceeding|        |    |
                   +------->|           |<-------+    |
            +<--------------|           |             |
            |Trnsprt Err    +-----------+             |
            |Inform TU            |                   |
            |                     |                   |
            |                     |200-699 from TU    |
            |                     |send response      |
            |  Request            V                   |
            |  send response+-----------+             |
            |      +--------|           |             |
            |      |        | Completed |<------------+
            |      +------->|           |
            +<--------------|           |
            |Trnsprt Err    +-----------+
            |Inform TU            |
            |                     |Timer J fires
            |                     |-
            |                     |
            |                     V
            |               +-----------+
            |               |           |
            +-------------->| Terminated|
                            |           |
                            +-----------+
    */
    eType = SIP_TXN_TYPE_NON_INVITE_SERVER;

    eState = SIP_TXN_STATE_INIT;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_REQUEST].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_REQUEST].astActions[0].pfnProc = SIP_Txn_ActPassMsg;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_REQUEST].eNextState = SIP_TXN_STATE_TRYING;

    eState = SIP_TXN_STATE_TRYING;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_1XX_RESPONSE].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_1XX_RESPONSE].astActions[0].pfnProc = SIP_Txn_ActSendMsg;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_1XX_RESPONSE].eNextState = SIP_TXN_STATE_PROCEEDING;

    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_2XX_RESPONSE].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_2XX_RESPONSE].astActions[0].pfnProc = SIP_Txn_ActSendMsg;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_2XX_RESPONSE].astActions[1].pfnProc = SIP_Txn_ActStartTimer;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_2XX_RESPONSE].astActions[1].ulPara  = SIP_TXN_TIMER_J;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_2XX_RESPONSE].eNextState = SIP_TXN_STATE_COMPLETED;

    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_3456XX_RESPONSE].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_3456XX_RESPONSE].astActions[0].pfnProc = SIP_Txn_ActSendMsg;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_3456XX_RESPONSE].astActions[1].pfnProc = SIP_Txn_ActStartTimer;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_3456XX_RESPONSE].astActions[1].ulPara  = SIP_TXN_TIMER_J;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_3456XX_RESPONSE].eNextState = SIP_TXN_STATE_COMPLETED;

    eState = SIP_TXN_STATE_PROCEEDING;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_REQUEST].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_REQUEST].astActions[0].pfnProc = SIP_Txn_ActReSendResponse;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_REQUEST].eNextState = SIP_TXN_STATE_PROCEEDING;

    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_1XX_RESPONSE].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_1XX_RESPONSE].astActions[0].pfnProc = SIP_Txn_ActSendMsg;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_1XX_RESPONSE].eNextState = SIP_TXN_STATE_PROCEEDING;

    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_2XX_RESPONSE].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_2XX_RESPONSE].astActions[0].pfnProc = SIP_Txn_ActSendMsg;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_2XX_RESPONSE].astActions[1].pfnProc = SIP_Txn_ActStartTimer;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_2XX_RESPONSE].astActions[1].ulPara  = SIP_TXN_TIMER_J;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_2XX_RESPONSE].eNextState = SIP_TXN_STATE_COMPLETED;

    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_3456XX_RESPONSE].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_3456XX_RESPONSE].astActions[0].pfnProc = SIP_Txn_ActSendMsg;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_3456XX_RESPONSE].astActions[1].pfnProc = SIP_Txn_ActStartTimer;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_3456XX_RESPONSE].astActions[1].ulPara  = SIP_TXN_TIMER_J;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_SEND_3456XX_RESPONSE].eNextState = SIP_TXN_STATE_COMPLETED;

    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TXP_ERR].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TXP_ERR].astActions[0].pfnProc = SIP_Txn_ActInformTU;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TXP_ERR].eNextState = SIP_TXN_STATE_TERMINATED;

    eState = SIP_TXN_STATE_COMPLETED;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_REQUEST].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_REQUEST].astActions[0].pfnProc = SIP_Txn_ActReSendResponse;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_RECV_REQUEST].eNextState = SIP_TXN_STATE_COMPLETED;

    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TIMER_J_FIRE].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TIMER_J_FIRE].eNextState = SIP_TXN_STATE_TERMINATED;

    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TXP_ERR].ulValid = TRUE;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TXP_ERR].astActions[0].pfnProc = SIP_Txn_ActInformTU;
    g_astSipTxnFsm[eType][eState][SIP_TXN_EVENT_TXP_ERR].eNextState = SIP_TXN_STATE_COMPLETED;

    return SUCCESS;
}

ULONG SIP_Txn_FsmProc(ULONG ulTxnID, SIP_TXN_EVENT_E eEvent, UBUF_HEADER_S *pstUbufSipMsg)
{
    SIP_TXN_TYPE_E  eType;
    SIP_TXN_STATE_E eState;
    ULONG           ulLoop;
    FN_SIP_TXN_ACTION pfnAction = NULL_PTR;

    eType  = g_pstSipTxnCB[ulTxnID].eType;
    eState = g_pstSipTxnCB[ulTxnID].eState;
    if (g_astSipTxnFsm[eType][eState][eEvent].ulValid != TRUE)
    {
        return FAIL;
    }

    for (ulLoop = 0; ulLoop < SIP_TXN_MAX_FSM_ACTION; ulLoop++)
    {

        pfnAction = g_astSipTxnFsm[eType][eState][eEvent].astActions[ulLoop].pfnProc;
        if (pfnAction != NULL_PTR)
        {
            pfnAction(ulTxnID, 
                      g_astSipTxnFsm[eType][eState][eEvent].astActions[ulLoop].ulPara, 
                      pstUbufSipMsg);
        }
    }

    if (g_astSipTxnFsm[eType][eState][eEvent].eNextState == SIP_TXN_STATE_TERMINATED)
    {
        /* 释放该事务 */
        SIP_Txn_FreeTxn(ulTxnID);
    }
    else
    {
        g_pstSipTxnCB[ulTxnID].eState = g_astSipTxnFsm[eType][eState][eEvent].eNextState;
    }

    return SUCCESS;
}

/* 向网络侧发送消息 */
ULONG SIP_Txn_ActSendMsg(ULONG ulTxnID, ULONG ulPara, UBUF_HEADER_S *pstUbufSipMsg)
{
    SIP_TXN_CB_S *pstSipTxnCB = NULL_PTR;

    pstSipTxnCB = &g_pstSipTxnCB[ulTxnID];

    /* 发送新的请求 */
    SIP_Txp_RecvDownMsg(pstUbufSipMsg,
                       &pstSipTxnCB->stPeer);

    return SUCCESS;
}

/* 向网络侧重新发送请求消息 */
ULONG SIP_Txn_ActReSendRequest(ULONG ulTxnID, ULONG ulPara, UBUF_HEADER_S *pstUbufSipMsg)
{
    SIP_TXN_CB_S *pstSipTxnCB = NULL_PTR;
    SIP_MSG_S    *pstSipMsg = NULL_PTR;
    SIP_MSG_S    *pstSipMsgTemp = NULL_PTR;
    UBUF_HEADER_S *pstUbufBuffer = NULL_PTR;
    ULONG ulRuleIndex;
    
    pstSipTxnCB = &g_pstSipTxnCB[ulTxnID];

    pstSipMsg = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstSipTxnCB->pstUbufRequest);
    pstUbufBuffer = UBUF_AllocUBuf(SIP_MAX_UBUF_MSG_LEN);
    SIP_GetRuleIndex("SIP-message", &ulRuleIndex);
    SIP_Clone(ulRuleIndex, pstSipMsg, pstUbufBuffer, &pstSipMsgTemp);

    /* 发送新的请求 */
    SIP_Txp_RecvDownMsg(pstUbufBuffer,
                       &pstSipTxnCB->stPeer);

    return SUCCESS;
}

/* 向网络侧重新发送响应消息 */
ULONG SIP_Txn_ActReSendResponse(ULONG ulTxnID, ULONG ulPara, UBUF_HEADER_S *pstUbufSipMsg)
{
    SIP_TXN_CB_S *pstSipTxnCB = NULL_PTR;
    SIP_MSG_S    *pstSipMsg = NULL_PTR;
    SIP_MSG_S    *pstSipMsgTemp = NULL_PTR;
    UBUF_HEADER_S *pstUbufBuffer = NULL_PTR;
    ULONG ulRuleIndex;

    pstSipTxnCB = &g_pstSipTxnCB[ulTxnID];

    pstSipMsg = (SIP_MSG_S *)UBUF_GET_MSG_PTR(pstSipTxnCB->pstUbufResponse);
    pstUbufBuffer = UBUF_AllocUBuf(SIP_MAX_UBUF_MSG_LEN);
    SIP_GetRuleIndex("SIP-message", &ulRuleIndex);
    SIP_Clone(ulRuleIndex, pstSipMsg, pstUbufBuffer, &pstSipMsgTemp);

    /* 发送新的请求 */
    SIP_Txp_RecvDownMsg(pstUbufBuffer,
                       &pstSipTxnCB->stPeer);

    return SUCCESS;
}

/* 向TU侧发送消息 */
ULONG SIP_Txn_ActPassMsg(ULONG ulTxnID, ULONG ulPara, UBUF_HEADER_S *pstUbufSipMsg)
{
    SIP_TXN_CB_S *pstSipTxnCB = NULL_PTR;

    pstSipTxnCB = &g_pstSipTxnCB[ulTxnID];

    /* 向TU通报消息 */
    SIP_TU_RecvUpMsg(pstSipTxnCB->ulCoreID,
                     ulTxnID,
                    &pstSipTxnCB->stPeer,
                     pstUbufSipMsg);

    return SUCCESS;
}

/* 向TU侧发送事件指示 */
ULONG SIP_Txn_ActInformTU(ULONG ulTxnID, ULONG ulPara, UBUF_HEADER_S *pstUbufSipMsg)
{
    return SUCCESS;
}

/* 发送ACK请求 */
ULONG SIP_Txn_ActSendAck(ULONG ulTxnID, ULONG ulPara, UBUF_HEADER_S *pstUbufSipMsg)
{
    return SUCCESS;
}

/* 重启定时器A */
ULONG SIP_Txn_ActStartTimer(ULONG ulTxnID, ULONG ulPara, UBUF_HEADER_S *pstUbufSipMsg)
{
    SIP_TXN_TIMER_NAME_E eName;

    eName = ulPara;
    switch (eName)
    {
        case SIP_TXN_TIMER_A:
            SIP_Txn_StartTimerA(ulTxnID);
            break;

        case SIP_TXN_TIMER_B:
            SIP_Txn_StartTimerB(ulTxnID);
            break;

        default:
            break;
    }

    return SUCCESS;
}


/* 重启定时器A */
ULONG SIP_Txn_ActStopTimer(ULONG ulTxnID, ULONG ulPara, UBUF_HEADER_S *pstUbufSipMsg)
{
    SIP_TXN_TIMER_NAME_E eName;

    eName = ulPara;
    SIP_Txn_StopTimer(ulTxnID, eName);

    return SUCCESS;
}

ULONG SIP_Txn_StartTimerA(ULONG ulTxnID)
{
    UCHAR ucSeq;
    ULONG ulTimerLen;
    
    ulTimerLen = 1 << g_pstSipTxnCB[ulTxnID].ucReSendNum;

    /* 分配A定时器 */
    SIP_Txn_AllocTimer(ulTxnID, SIP_TXN_TIMER_A, &ucSeq);

    SIP_StartTimer(SIP_TXN_TIMER_A,
                   ulTimerLen * g_ulSipTxnT1,
                   ulTxnID,
                   SIP_TIMER_NO_LOOP,
                  &g_pstSipTxnCB[ulTxnID].astTimers[ucSeq].hTimerID);

    g_pstSipTxnCB[ulTxnID].ucReSendNum++;

    return SUCCESS;
}

ULONG SIP_Txn_StartTimerB(ULONG ulTxnID)
{
    UCHAR ucSeq;

    /* 分配A定时器 */
    SIP_Txn_AllocTimer(ulTxnID, SIP_TXN_TIMER_B, &ucSeq);

    SIP_StartTimer(SIP_TXN_TIMER_B,
                   64 * g_ulSipTxnT1,
                   ulTxnID,
                   SIP_TIMER_NO_LOOP,
                  &g_pstSipTxnCB[ulTxnID].astTimers[ucSeq].hTimerID);

    return SUCCESS;
}

/* 查找定时器 */
ULONG SIP_Txn_LookupTimer(ULONG ulTxnID, SIP_TXN_TIMER_NAME_E eName, UCHAR *pucSeq)
{
    UCHAR ucLoop;

    for (ucLoop = 0; ucLoop < SIP_TXN_MAX_FSM_TIMER; ucLoop++)
    {
        if(g_pstSipTxnCB[ulTxnID].astTimers[ucLoop].eTimerName == eName)
        {
            *pucSeq = ucLoop;
            return SUCCESS;
        }
    }

    return FAIL;
}

/* 分配定时器 */
ULONG SIP_Txn_AllocTimer(ULONG ulTxnID, SIP_TXN_TIMER_NAME_E eName, UCHAR *pucSeq)
{
    UCHAR ucLoop;

    for (ucLoop = 0; ucLoop < SIP_TXN_MAX_FSM_TIMER; ucLoop++)
    {
        if(g_pstSipTxnCB[ulTxnID].astTimers[ucLoop].eTimerName == NULL_ULONG)
        {
            g_pstSipTxnCB[ulTxnID].astTimers[ucLoop].eTimerName = eName;
            *pucSeq = ucLoop;
            return SUCCESS;
        }
    }

    return FAIL;
}

/* 分配定时器 */
ULONG SIP_Txn_StopTimer(ULONG ulTxnID, SIP_TXN_TIMER_NAME_E eName)
{
    UCHAR ucLoop;

    for (ucLoop = 0; ucLoop < SIP_TXN_MAX_FSM_TIMER; ucLoop++)
    {
        if(g_pstSipTxnCB[ulTxnID].astTimers[ucLoop].eTimerName == eName)
        {
            SIP_StopTimer(g_pstSipTxnCB[ulTxnID].astTimers[ucLoop].hTimerID);
            g_pstSipTxnCB[ulTxnID].astTimers[ucLoop].hTimerID   = NULL_ULONG;
            g_pstSipTxnCB[ulTxnID].astTimers[ucLoop].eTimerName = NULL_ULONG;

            return SUCCESS;
        }
    }

    return FAIL;
}
