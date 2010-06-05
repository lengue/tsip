/*******************************************************************************
功能    : COMMON模块唯一需要包含的头文件
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/
#define DEBUG_ABNF 0
#define DEBUG_ABNF_CORE 0
/*操作系统*/
#define WIN32      1

#if WIN32
#define _WIN32_WINNT 0x0500

/* 标准头文件 */
#include <stdio.h>
#include <stdlib.h>
//#include <Winsock2.h>
#include <windows.h>
#include "libxml\parser.h"
#else
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#endif

#include "common_const.h"
#include "common_type.h"
