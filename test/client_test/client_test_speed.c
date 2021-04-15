#include <CUnit/CUnit.h>

#include <cluster_api.h>

#include "client_test_common.h"

#define SPEED_TEST_FUNC(func, param, value) \
    static void Test ##func() { \
        IPC_DATA_IC_SERVICE_S sendData; \
        CU_ASSERT(clusterInit() == true); \
        memset(&sendData, 0x00, sizeof(sendData)); \
        sendData.param = value; \
        requestSendData(&sendData); \
        CU_ASSERT(func() == value); \
        CU_ASSERT(clusterTerm() == true); \
    }

#define SPEED_ADD_TEST(testSuite, func) \
    CU_add_test(testSuite, #func, Test ##func)

SPEED_TEST_FUNC(getSpAnalogVal, spAnalogVal, 0x00007530)
SPEED_TEST_FUNC(getSpAnaDigUnitVal, spAnaDigUnitVal, IC_HMI_SP_MPH)

void SetTestSuiteSpeed(void)
{
    CU_pSuite testSuite;

    testSuite = CU_add_suite("Speed", NULL, NULL);
    SPEED_ADD_TEST(testSuite, getSpAnalogVal);
    SPEED_ADD_TEST(testSuite, getSpAnaDigUnitVal);
}

