/*******************************************************************************
功能    : abnf模块对外提供的结构
创建人  : 唐春平
创建日期: 200.01.10
修改记录:
*******************************************************************************/
/* 文法节点结构 */
typedef struct tagABNF_GRAMMAR_NODE_S
{
    ULONG                ulOffset;     /* 节点偏移 */
    ULONG                ulSize;       /* 节点长度 */
    UCHAR                ucRuleListIndex;   /* 是否匹配核心规则 */
    ULONG                ulIndex;      /* 规则索引 */
    struct tagABNF_GRAMMAR_NODE_S *pstChild;     /* 第一个子子规则 */
    struct tagABNF_GRAMMAR_NODE_S *pstNextNode;  /* 顺序的下一条子规则 */
}ABNF_GRAMMAR_NODE_S;