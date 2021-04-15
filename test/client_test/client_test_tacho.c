#include <CUnit/CUnit.h>

#include <cluster_api.h>

#include "client_test_common.h"

#define TACHO_TEST_FUNC(func, param, value) \
    static void Test ##func() { \
        IPC_DATA_IC_SERVICE_S sendData; \
        CU_ASSERT(clusterInit() == true); \
        memset(&sendData, 0x00, sizeof(sendData)); \
        sendData.param = value; \
        requestSendData(&sendData); \
        CU_ASSERT(func() == value); \
        CU_ASSERT(clusterTerm() == true); \
    }

#define TACHO_ADD_TEST(testSuite, func) \
    CU_add_test(testSuite, #func, Test ##func)

TACHO_TEST_FUNC(getTaAnalogVal, taAnalogVal, 0x00004E20)

void SetTestSuiteTacho(void)
{
    CU_pSuite testSuite;

    testSuite = CU_add_suite("Tacho", NULL, NULL);
    TACHO_ADD_TEST(testSuite, getTaAnalogVal);
}

