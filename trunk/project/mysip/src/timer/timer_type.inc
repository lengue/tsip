typedef struct tagTIMER_S
{
    UCHAR ucIsUsed;
    ULONG ulModuleID;
    ULONG ulName;
    TIMER_TYPE_E eType;
    ULONG ulLen;
    ULONG ulPara;
    HANDLE  hTimer;
    ULONG ulNext;
}TIMER_S;