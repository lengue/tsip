/*******************************************************************************
功能    : 统一缓存(Union Buffer)组件对外提供类型声明
创建人  : 唐春平
创建日期: 2009.04.21
修改记录:
*******************************************************************************/
/*
统一缓存类似于MBUF
|--------------|      +----->|--------------|
|  本片长度    |      |      |  本片长度    |
|  消息总长度  |      |      |   *无效*     |
| 本片有效长度 |      |      | 本片有效长度 |
|  下一片指针  |------+      |  下一片指针  |
|--------------|             |--------------|
|   缓存内容   |             |   缓存内容   |
|   ........   |             |   ........   |
|--------------|             |--------------|
*/

typedef struct tagUBUF_HEADER_S
{
    /* 本片总长度 */
    ULONG ulSegmentLen;
    /* 消息总长度，只有头片有效 */
    ULONG ulMsgLen;
    /* 当前片使用长度 */
    ULONG ulCurrentLen;
    struct tagUBUF_HEADER_S *pNext;
}UBUF_HEADER_S;