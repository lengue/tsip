/* 发起域名解析 */
ULONG DNS_Resolve(UCHAR     *pucName,
                  DNS_TYPE_E eDnsType,
                  DNS_RESULT_S *pstResult,
                  DNS_FN_RESULT_NOTIFY pfnNotify,
                  ULONG      ulPara)
{
    /***************************************************************************
    分为几步处理:
    1. 查找控域名解析制块中是否存在同一域名和类型的控制块，如果存在，则申请新的
       请求控制块，并将该控制块挂在该域名解析控制块下面就行了，否则继续处理
    2. 根据类型直接调用操作系统函数获取高速缓中的结果，如果获取成功，直接填充
       pstResult，返回OK；否则继续处理
    3. 申请新的域名解析控制块和新的请求控制块，将请求控制块挂入域名解析控制块，
       启动定时器等待结果，返回等待异步解析
    ***************************************************************************/
    return SUCCESS;
}

/* 请求终止域名解析 */
ULONG DNS_CancelResolve(UCHAR     *pucName,
                        DNS_TYPE_E eDnsType,
                        DNS_FN_RESULT_NOTIFY pfnNotify,
                        ULONG      ulPara)
{
    /***************************************************************************
    分为几步处理:
    1. 查找控域名解析制块中是否存在同一域名和类型的控制块，如果不存在，则直接返
       回OK
    2. 查找请求队列中是否存在pfnNotify和ulPara一致的请求控制块，如果存在则释放该
       请求控制块
    3. 返回OK
    ***************************************************************************/
    return SUCCESS;
}

