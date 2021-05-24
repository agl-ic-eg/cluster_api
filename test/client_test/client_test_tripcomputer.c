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

#define TRIPCOMPUTER_TEST_FUNC(func, param, value) \
    static void Test ##func() { \
        IPC_DATA_IC_SERVICE_S sendData; \
        CU_ASSERT(clusterInit() == true); \
        memset(&sendData, 0x00, sizeof(sendData)); \
        sendData.param = value; \
        requestSendData(&sendData); \
        CU_ASSERT(func() == value); \
        CU_ASSERT(clusterTerm() == true); \
    }

#define TRIPCOMPUTER_ADD_TEST(testSuite, func) \
    CU_add_test(testSuite, #func, Test ##func)

TRIPCOMPUTER_TEST_FUNC(getTrcomTripAVal, trcomTripAVal, 0x0001869F)
TRIPCOMPUTER_TEST_FUNC(getTrcomTripBVal, trcomTripBVal, 0x0001869F)
TRIPCOMPUTER_TEST_FUNC(getTrcomOdoVal, trcomOdoVal, 0x000F423F)
TRIPCOMPUTER_TEST_FUNC(getTrcomUnitVal, trcomUnitVal, IC_HMI_TRCOM_MILE)
TRIPCOMPUTER_TEST_FUNC(getAvgSpeedAVal, avgSpeedAVal, 0x012C)
TRIPCOMPUTER_TEST_FUNC(getAvgSpeedBVal, avgSpeedBVal, 0x012C)
TRIPCOMPUTER_TEST_FUNC(getHourAVal, hourAVal, 0x03E7)
TRIPCOMPUTER_TEST_FUNC(getHourBVal, hourBVal, 0x03E7)
TRIPCOMPUTER_TEST_FUNC(getMinuteAVal, minuteAVal, 0x3B)
TRIPCOMPUTER_TEST_FUNC(getMinuteBVal, minuteBVal, 0x3B)
TRIPCOMPUTER_TEST_FUNC(getSecondAVal, secondAVal, 0x3B)
TRIPCOMPUTER_TEST_FUNC(getSecondBVal, secondBVal, 0x3B)
TRIPCOMPUTER_TEST_FUNC(getOTempVal, oTempVal, (signed short)0xFFD8)
TRIPCOMPUTER_TEST_FUNC(getOTempUnitVal, oTempUnitVal, IC_HMI_OTEMP_UNIT_K)
TRIPCOMPUTER_TEST_FUNC(getCruRangeVal, cruRangeVal, 0x064A)
TRIPCOMPUTER_TEST_FUNC(getAvgFuelAVal, avgFuelAVal, 0x07CF)
TRIPCOMPUTER_TEST_FUNC(getAvgFuelBVal, avgFuelBVal, 0x07CF)
TRIPCOMPUTER_TEST_FUNC(getInsFuelAVal, insFuelAVal, 0x07CF)
TRIPCOMPUTER_TEST_FUNC(getInsFuelBVal, insFuelBVal, 0x07CF)
TRIPCOMPUTER_TEST_FUNC(getFuelEconomyUnitVal, fuelEconomyUnitVal, IC_HMI_FUEL_KWH_100KM)

void SetTestSuiteTripComputer(void)
{
    CU_pSuite testSuite;

    testSuite = CU_add_suite("TripComputer", NULL, NULL);
    TRIPCOMPUTER_ADD_TEST(testSuite, getTrcomTripAVal);
    TRIPCOMPUTER_ADD_TEST(testSuite, getTrcomTripBVal);
    TRIPCOMPUTER_ADD_TEST(testSuite, getTrcomOdoVal);
    TRIPCOMPUTER_ADD_TEST(testSuite, getTrcomUnitVal);
    TRIPCOMPUTER_ADD_TEST(testSuite, getAvgSpeedAVal);
    TRIPCOMPUTER_ADD_TEST(testSuite, getAvgSpeedBVal);
    TRIPCOMPUTER_ADD_TEST(testSuite, getHourAVal);
    TRIPCOMPUTER_ADD_TEST(testSuite, getHourBVal);
    TRIPCOMPUTER_ADD_TEST(testSuite, getMinuteAVal);
    TRIPCOMPUTER_ADD_TEST(testSuite, getMinuteBVal);
    TRIPCOMPUTER_ADD_TEST(testSuite, getSecondAVal);
    TRIPCOMPUTER_ADD_TEST(testSuite, getSecondBVal);
    TRIPCOMPUTER_ADD_TEST(testSuite, getOTempVal);
    TRIPCOMPUTER_ADD_TEST(testSuite, getOTempUnitVal);
    TRIPCOMPUTER_ADD_TEST(testSuite, getCruRangeVal);
    TRIPCOMPUTER_ADD_TEST(testSuite, getAvgFuelAVal);
    TRIPCOMPUTER_ADD_TEST(testSuite, getAvgFuelBVal);
    TRIPCOMPUTER_ADD_TEST(testSuite, getInsFuelAVal);
    TRIPCOMPUTER_ADD_TEST(testSuite, getInsFuelBVal);
    TRIPCOMPUTER_ADD_TEST(testSuite, getFuelEconomyUnitVal);
}

