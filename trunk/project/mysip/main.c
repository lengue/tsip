/*******************************************************************************
功能    : 程序主函数
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/
/* 外部依赖模块头文件 */
#include "common\common.h"
#include "system\system.h"
#include "ubuf\ubuf.h"
#include "sip\uri.h"
#include "sip\sip.h"
#include "app\app.h"

/* 本模块对外提供的常量和结构头文件 */
/* 本模块内部使用的常量和结构头文件 */
/* 本模块内部函数声明头文件 */
/* 本模块全局变量声明头文件 */

#include "main.h"

int main(int argc, char* argv[])
{
    ULONG ulRet;

    ulRet = SYS_InitProc();
    if (ulRet != SUCCESS)
    {
        printf("\r\nSYS_InitProc Failed");
        return ulRet;
    }

    return APP_StartUserInterface();
}
