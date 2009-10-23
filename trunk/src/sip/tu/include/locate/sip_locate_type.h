typedef struct tagSIP_LOCATION_RESULT_S
{
    ULONG ulNumber;
    SIP_LOCATION_S astLocations[SIP_LOCATION_MAX_NUMBER];
}SIP_LOCATION_RESULT_S;

typedef ULONG (*SIP_LOCATION_RESULT_PROC)(SIP_LOCATION_RESULT_S *pstResult,
                                          ULONG ulPara);