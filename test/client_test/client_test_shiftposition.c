#include <CUnit/CUnit.h>

#include <cluster_api.h>

#include "client_test_common.h"

#define SHIFTPOSITION_TEST_FUNC(func, param, value) \
    static void Test ##func() { \
        IPC_DATA_IC_SERVICE_S sendData; \
        CU_ASSERT(clusterInit() == true); \
        memset(&sendData, 0x00, sizeof(sendData)); \
        sendData.param = value; \
        requestSendData(&sendData); \
        CU_ASSERT(func() == value); \
        CU_ASSERT(clusterTerm() == true); \
    }

#define SHIFTPOSITION_ADD_TEST(testSuite, func) \
    CU_add_test(testSuite, #func, Test ##func)

SHIFTPOSITION_TEST_FUNC(getGearAtVal, gearAtVal, IC_HMI_AT_UNUSED)
SHIFTPOSITION_TEST_FUNC(getGearMtVal, gearMtVal, IC_HMI_MT_EIGHTTH)

void SetTestSuiteShiftPosition(void)
{
    CU_pSuite testSuite;

    testSuite = CU_add_suite("ShiftPosition", NULL, NULL);
    SHIFTPOSITION_ADD_TEST(testSuite, getGearAtVal);
    SHIFTPOSITION_ADD_TEST(testSuite, getGearMtVal);
}
