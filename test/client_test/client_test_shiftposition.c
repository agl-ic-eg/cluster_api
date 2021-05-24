/*
 * Copyright (c) 2021, Nippon Seiki Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
