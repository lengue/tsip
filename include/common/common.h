/*******************************************************************************
����    : COMMONģ��Ψһ��Ҫ������ͷ�ļ�
������  : �ƴ�ƽ
��������: 200.01.10
�޸ļ�¼:
*******************************************************************************/
#define DEBUG_ABNF 0
#define DEBUG_ABNF_CORE 0
/*����ϵͳ*/
#define WIN32      1

#if WIN32
#define _WIN32_WINNT 0x0500

/* ��׼ͷ�ļ� */
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
