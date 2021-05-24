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

#include <string.h>
#include <stdbool.h>

#include <CUnit/CUnit.h>

#include <cluster_api.h>

#include "client_test_common.h"

typedef struct {
    IC_HMI_ON_OFF value;
    bool isCallbacked;
    const unsigned long long flag;
    const char *name;
} CLUSTER_TEST_CB_STORE_TABLE_S;

#define CLUSTER_TEST_DECLARE_STORE_UNIT(flag) \
    {IC_HMI_OFF, false, flag, #flag}

static CLUSTER_TEST_CB_STORE_TABLE_S g_cbStoreTable[] = {
    CLUSTER_TEST_DECLARE_STORE_UNIT(IC_HMI_TT_TURN_R),
    CLUSTER_TEST_DECLARE_STORE_UNIT(IC_HMI_TT_TURN_L),
    CLUSTER_TEST_DECLARE_STORE_UNIT(IC_HMI_TT_BRAKE),
    CLUSTER_TEST_DECLARE_STORE_UNIT(IC_HMI_TT_SEATBELT),
    CLUSTER_TEST_DECLARE_STORE_UNIT(IC_HMI_TT_HIGHBEAM),
    CLUSTER_TEST_DECLARE_STORE_UNIT(IC_HMI_TT_DOOR),
    CLUSTER_TEST_DECLARE_STORE_UNIT(IC_HMI_TT_EPS),
    CLUSTER_TEST_DECLARE_STORE_UNIT(IC_HMI_TT_SRS_AIRBAG),
    CLUSTER_TEST_DECLARE_STORE_UNIT(IC_HMI_TT_ABS),
    CLUSTER_TEST_DECLARE_STORE_UNIT(IC_HMI_TT_LOW_BATTERY),
    CLUSTER_TEST_DECLARE_STORE_UNIT(IC_HMI_TT_OIL_PRESS),
    CLUSTER_TEST_DECLARE_STORE_UNIT(IC_HMI_TT_ENGINE),
    CLUSTER_TEST_DECLARE_STORE_UNIT(IC_HMI_TT_FUEL),
    CLUSTER_TEST_DECLARE_STORE_UNIT(IC_HMI_TT_IMMOBI),
    CLUSTER_TEST_DECLARE_STORE_UNIT(IC_HMI_TT_TM_FAIL),
    CLUSTER_TEST_DECLARE_STORE_UNIT(IC_HMI_TT_ESP_ACT),
    CLUSTER_TEST_DECLARE_STORE_UNIT(IC_HMI_TT_ESP_OFF),
    CLUSTER_TEST_DECLARE_STORE_UNIT(IC_HMI_TT_ADAPTING_LIGHTING),
    CLUSTER_TEST_DECLARE_STORE_UNIT(IC_HMI_TT_AUTO_STOP),
    CLUSTER_TEST_DECLARE_STORE_UNIT(IC_HMI_TT_AUTO_STOP_FAIL),
    CLUSTER_TEST_DECLARE_STORE_UNIT(IC_HMI_TT_PARKING_LIGHTS),
    CLUSTER_TEST_DECLARE_STORE_UNIT(IC_HMI_TT_FRONT_FOG),
    CLUSTER_TEST_DECLARE_STORE_UNIT(IC_HMI_TT_EXTERIOR_LIGHT_FAULT),
    CLUSTER_TEST_DECLARE_STORE_UNIT(IC_HMI_TT_ACC_FAIL),
    CLUSTER_TEST_DECLARE_STORE_UNIT(IC_HMI_TT_LDW_OFF),
    CLUSTER_TEST_DECLARE_STORE_UNIT(IC_HMI_TT_HILL_DESCENT),
    CLUSTER_TEST_DECLARE_STORE_UNIT(IC_HMI_TT_AUTO_HI_BEAM_GREEN),
    CLUSTER_TEST_DECLARE_STORE_UNIT(IC_HMI_TT_AUTO_HI_BEAM_AMBER),
    CLUSTER_TEST_DECLARE_STORE_UNIT(IC_HMI_TT_LDW_OPERATE),
    CLUSTER_TEST_DECLARE_STORE_UNIT(IC_HMI_TT_GENERAL_WARN),
    CLUSTER_TEST_DECLARE_STORE_UNIT(IC_HMI_TT_SPORTS_MODE),
    CLUSTER_TEST_DECLARE_STORE_UNIT(IC_HMI_TT_DRIVING_POWER_MODE),
    CLUSTER_TEST_DECLARE_STORE_UNIT(IC_HMI_TT_HOT_TEMP),
    CLUSTER_TEST_DECLARE_STORE_UNIT(IC_HMI_TT_LOW_TEMP)
};

static bool g_isCalledCallback = false;

#define CLUSTER_TEST_STORE_TABLE_SIZE (sizeof(g_cbStoreTable)/sizeof(g_cbStoreTable[0]))

static void cleanStoreTable(void)
{
    int i;

    for (i = 0; i < CLUSTER_TEST_STORE_TABLE_SIZE; i++) {
        g_cbStoreTable[i].value = 0;
        g_cbStoreTable[i].isCallbacked = false;
    }
}

static bool checkValueFromStoreTable(const unsigned long long flag,
                                     const bool expectedIsCallbacked,
                                     const IC_HMI_ON_OFF expectedOnOff)
{
    bool ret = true;
    int i;

    for (i = 0; i < CLUSTER_TEST_STORE_TABLE_SIZE; i++) {
        if (flag == g_cbStoreTable[i].flag) {
            if (g_cbStoreTable[i].isCallbacked != expectedIsCallbacked) {
                ret = false;
                fprintf(stderr, "%s:%d [%s] isCallbacked == %d is false\n", __FILE__, __LINE__, g_cbStoreTable[i].name, expectedIsCallbacked);
            }

            if (expectedIsCallbacked == true && g_cbStoreTable[i].value != expectedOnOff) {
                ret = false;
                fprintf(stderr, "%s:%d [%s] value == %d is false\n", __FILE__, __LINE__, g_cbStoreTable[i].name, expectedOnOff);
            }
            return ret;
        }
    }

    return false;
}

static void sendDataAllSet(IPC_DATA_IC_SERVICE_S *pSendData, IC_HMI_ON_OFF onOff)
{
    pSendData->turnR = onOff;
    pSendData->turnL = onOff;
    pSendData->brake = onOff;
    pSendData->seatbelt = onOff;
    pSendData->highbeam = onOff;
    pSendData->door = onOff;
    pSendData->eps = onOff;
    pSendData->srsAirbag = onOff;
    pSendData->abs = onOff;
    pSendData->lowBattery = onOff;
    pSendData->oilPress = onOff;
    pSendData->engine = onOff;
    pSendData->fuel = onOff;
    pSendData->immobi = onOff;
    pSendData->tmFail = onOff;
    pSendData->espAct = onOff;
    pSendData->espOff = onOff;
    pSendData->adaptingLighting = onOff;
    pSendData->autoStop = onOff;
    pSendData->autoStopFail = onOff;
    pSendData->parkingLights = onOff;
    pSendData->frontFog = onOff;
    pSendData->exteriorLightFault = onOff;
    pSendData->accFail = onOff;
    pSendData->ldwOff = onOff;
    pSendData->hillDescent = onOff;
    pSendData->autoHiBeamGreen = onOff;
    pSendData->autoHiBeamAmber = onOff;
    pSendData->ldwOperate = onOff;
    pSendData->generalWarn = onOff;
    pSendData->sportsMode = onOff;
    pSendData->drivingPowerMode = onOff;
    pSendData->hotTemp = onOff;
    pSendData->lowTemp = onOff;
}

static void notifyIcHmi(unsigned long long arg_1, IC_HMI_ON_OFF arg_2)
{
    int i;

    g_isCalledCallback = true;

    for (i = 0; i < sizeof(g_cbStoreTable) / sizeof(g_cbStoreTable[0]); i++) {
        if ((arg_1 & g_cbStoreTable[i].flag) == g_cbStoreTable[i].flag) {
            g_cbStoreTable[i].value = arg_2;
            g_cbStoreTable[i].isCallbacked = true;
        }
    }
}

static void TestregisterIcHmi_ALL_DifferentData() {
    unsigned long long arg_1;
    IPC_DATA_IC_SERVICE_S sendData;
    int i;

    CU_ASSERT(clusterInit() == true);

    cleanStoreTable();
    g_isCalledCallback = false;

    arg_1 = IC_HMI_TT_ALL;
    CU_ASSERT(registerIcHmi(arg_1, notifyIcHmi) == true);

    CU_ASSERT(g_isCalledCallback == false);

    memset(&sendData, 0x00, sizeof(sendData));
    sendDataAllSet(&sendData, IC_HMI_ON);
    requestSendData(&sendData);

    CU_ASSERT(g_isCalledCallback == true);
    for (i = 0; i < CLUSTER_TEST_STORE_TABLE_SIZE; i++) {
        CU_ASSERT(checkValueFromStoreTable(g_cbStoreTable[i].flag, true, IC_HMI_ON) == true);
    }

    g_isCalledCallback = false;
    sendDataAllSet(&sendData, IC_HMI_OFF);
    requestSendData(&sendData);

    CU_ASSERT(g_isCalledCallback == true);
    for (i = 0; i < CLUSTER_TEST_STORE_TABLE_SIZE; i++) {
        CU_ASSERT(checkValueFromStoreTable(g_cbStoreTable[i].flag, true, IC_HMI_OFF) == true);
    }

    CU_ASSERT(clusterTerm() == true);
}

static void TestregisterIcHmi_ALL_SameData() {
    unsigned long long arg_1;
    IPC_DATA_IC_SERVICE_S sendData;

    CU_ASSERT(clusterInit() == true);

    cleanStoreTable();
    g_isCalledCallback = false;

    arg_1 = IC_HMI_TT_ALL;
    CU_ASSERT(registerIcHmi(arg_1, notifyIcHmi) == true);

    CU_ASSERT(g_isCalledCallback == false);

    // Send the data "different" from initial value.
    memset(&sendData, 0x00, sizeof(sendData));
    sendDataAllSet(&sendData, IC_HMI_OFF);
    requestSendData(&sendData);

    CU_ASSERT(g_isCalledCallback == false);

    g_isCalledCallback = false;
    sendDataAllSet(&sendData, IC_HMI_OFF);
    requestSendData(&sendData);

    CU_ASSERT(g_isCalledCallback == false);

    CU_ASSERT(clusterTerm() == true);
}

static void TestregisterIcHmi_NullFunc() {
    unsigned long long arg_1;

    CU_ASSERT(clusterInit() == true);

    arg_1 = IC_HMI_TT_ALL;
    CU_ASSERT(registerIcHmi(arg_1, NULL) == false);

    CU_ASSERT(clusterTerm() == true);
}

static void TestregisterIcHmi_ALL_OFF() {
    unsigned long long arg_1;
    IPC_DATA_IC_SERVICE_S sendData;

    CU_ASSERT(clusterInit() == true);

    cleanStoreTable();
    g_isCalledCallback = false;

    arg_1 = 0ULL;
    CU_ASSERT(registerIcHmi(arg_1, notifyIcHmi) == true);

    CU_ASSERT(g_isCalledCallback == false);

    memset(&sendData, 0x00, sizeof(sendData));
    sendDataAllSet(&sendData, IC_HMI_ON);
    requestSendData(&sendData);

    CU_ASSERT(g_isCalledCallback == false);

    g_isCalledCallback = false;
    sendDataAllSet(&sendData, IC_HMI_OFF);
    requestSendData(&sendData);

    CU_ASSERT(g_isCalledCallback == false);

    CU_ASSERT(clusterTerm() == true);
}

static void TestregisterIcHmi_ALL_UnrelatedParams(void)
{
    unsigned long long arg_1;
    IPC_DATA_IC_SERVICE_S sendData;

    CU_ASSERT(clusterInit() == true);

    cleanStoreTable();
    g_isCalledCallback = false;

    arg_1 = IC_HMI_TT_ALL;
    CU_ASSERT(registerIcHmi(arg_1, notifyIcHmi) == true);

    CU_ASSERT(g_isCalledCallback == false);

    memset(&sendData, 0x01, sizeof(sendData));
    sendDataAllSet(&sendData, IC_HMI_OFF);
    requestSendData(&sendData);

    CU_ASSERT(g_isCalledCallback == false);

    g_isCalledCallback = false;
    memset(&sendData, 0x00, sizeof(sendData));
    sendDataAllSet(&sendData, IC_HMI_OFF);
    requestSendData(&sendData);

    CU_ASSERT(g_isCalledCallback == false);

    CU_ASSERT(clusterTerm() == true);
}

static void TestregisterIcHmi_Set_UnusedFlag(void)
{
    unsigned long long arg_1;
    IPC_DATA_IC_SERVICE_S sendData;
    int i;

    CU_ASSERT(clusterInit() == true);

    cleanStoreTable();
    g_isCalledCallback = false;

    arg_1 = IC_HMI_TT_ALL;
    for (i = 0; i < CLUSTER_TEST_STORE_TABLE_SIZE; i++) {
        arg_1 &= ~(g_cbStoreTable[i].flag);
    }
    CU_ASSERT(registerIcHmi(arg_1, notifyIcHmi) == true);

    CU_ASSERT(g_isCalledCallback == false);

    memset(&sendData, 0x00, sizeof(sendData));
    sendDataAllSet(&sendData, IC_HMI_ON);
    requestSendData(&sendData);

    CU_ASSERT(g_isCalledCallback == false);

    g_isCalledCallback = false;
    sendDataAllSet(&sendData, IC_HMI_OFF);
    requestSendData(&sendData);

    CU_ASSERT(g_isCalledCallback == false);

    CU_ASSERT(clusterTerm() == true);
}

#define REGISTER_NOTIFY_TEST_FUNC(flag, param) \
    static void TestregisterIcHmi_ON_ ##flag(void) \
    { \
        unsigned long long arg_1; \
        IPC_DATA_IC_SERVICE_S sendData; \
 \
        CU_ASSERT(clusterInit() == true); \
 \
        cleanStoreTable(); \
        g_isCalledCallback = false; \
 \
        arg_1 = IC_HMI_ ##flag; \
        CU_ASSERT(registerIcHmi(arg_1, notifyIcHmi) == true); \
 \
        CU_ASSERT(g_isCalledCallback == false); \
 \
        memset(&sendData, 0x00, sizeof(sendData)); \
        sendDataAllSet(&sendData, IC_HMI_ON); \
        sendData.param = IC_HMI_OFF; \
        requestSendData(&sendData); \
 \
        CU_ASSERT(g_isCalledCallback == false); \
 \
        g_isCalledCallback = false; \
        sendData.param = IC_HMI_ON; \
        requestSendData(&sendData); \
 \
        CU_ASSERT(g_isCalledCallback == true); \
        CU_ASSERT(checkValueFromStoreTable(IC_HMI_ ##flag, true, IC_HMI_ON) == true); \
 \
        g_isCalledCallback = false; \
        sendData.param = IC_HMI_OFF; \
        requestSendData(&sendData); \
 \
        CU_ASSERT(g_isCalledCallback == true); \
        CU_ASSERT(checkValueFromStoreTable(IC_HMI_ ##flag, true, IC_HMI_OFF) == true); \
 \
        CU_ASSERT(clusterTerm() == true); \
    } \
    static void TestregisterIcHmi_OFF_ ##flag(void) \
    { \
        unsigned long long arg_1; \
        IPC_DATA_IC_SERVICE_S sendData; \
 \
        CU_ASSERT(clusterInit() == true); \
 \
        cleanStoreTable(); \
        g_isCalledCallback = false; \
 \
        arg_1 = IC_HMI_TT_ALL & ~IC_HMI_ ##flag; \
        CU_ASSERT(registerIcHmi(arg_1, notifyIcHmi) == true); \
 \
        CU_ASSERT(g_isCalledCallback == false); \
 \
        memset(&sendData, 0x00, sizeof(sendData)); \
        sendDataAllSet(&sendData, IC_HMI_OFF); \
        sendData.param = IC_HMI_ON; \
        requestSendData(&sendData); \
 \
        CU_ASSERT(g_isCalledCallback == false); \
 \
        g_isCalledCallback = false; \
        sendData.param = IC_HMI_OFF; \
        requestSendData(&sendData); \
 \
        CU_ASSERT(g_isCalledCallback == false); \
 \
        g_isCalledCallback = false; \
        sendDataAllSet(&sendData, IC_HMI_ON); \
        requestSendData(&sendData); \
 \
        CU_ASSERT(g_isCalledCallback == true); \
        CU_ASSERT(checkValueFromStoreTable(IC_HMI_ ##flag, false, IC_HMI_OFF) == true); \
 \
        CU_ASSERT(clusterTerm() == true); \
    }

#define REGISTER_NOTIFY_ADD_TEST(testSuite, flag) \
    CU_add_test(testSuite, "registerIcHmi_ON_" #flag, TestregisterIcHmi_ON_ ##flag); \
    CU_add_test(testSuite, "registerIcHmi_OFF_" #flag, TestregisterIcHmi_OFF_ ##flag)

REGISTER_NOTIFY_TEST_FUNC(TT_TURN_R, turnR)
REGISTER_NOTIFY_TEST_FUNC(TT_TURN_L, turnL)
REGISTER_NOTIFY_TEST_FUNC(TT_BRAKE, brake)
REGISTER_NOTIFY_TEST_FUNC(TT_SEATBELT, seatbelt)
REGISTER_NOTIFY_TEST_FUNC(TT_HIGHBEAM, highbeam)
REGISTER_NOTIFY_TEST_FUNC(TT_DOOR, door)
REGISTER_NOTIFY_TEST_FUNC(TT_EPS, eps)
REGISTER_NOTIFY_TEST_FUNC(TT_SRS_AIRBAG, srsAirbag)
REGISTER_NOTIFY_TEST_FUNC(TT_ABS, abs)
REGISTER_NOTIFY_TEST_FUNC(TT_LOW_BATTERY, lowBattery)
REGISTER_NOTIFY_TEST_FUNC(TT_OIL_PRESS, oilPress)
REGISTER_NOTIFY_TEST_FUNC(TT_ENGINE, engine)
REGISTER_NOTIFY_TEST_FUNC(TT_FUEL, fuel)
REGISTER_NOTIFY_TEST_FUNC(TT_IMMOBI, immobi)
REGISTER_NOTIFY_TEST_FUNC(TT_TM_FAIL, tmFail)
REGISTER_NOTIFY_TEST_FUNC(TT_ESP_ACT, espAct)
REGISTER_NOTIFY_TEST_FUNC(TT_ESP_OFF, espOff)
REGISTER_NOTIFY_TEST_FUNC(TT_ADAPTING_LIGHTING, adaptingLighting)
REGISTER_NOTIFY_TEST_FUNC(TT_AUTO_STOP, autoStop)
REGISTER_NOTIFY_TEST_FUNC(TT_AUTO_STOP_FAIL, autoStopFail)
REGISTER_NOTIFY_TEST_FUNC(TT_PARKING_LIGHTS, parkingLights)
REGISTER_NOTIFY_TEST_FUNC(TT_FRONT_FOG, frontFog)
REGISTER_NOTIFY_TEST_FUNC(TT_EXTERIOR_LIGHT_FAULT, exteriorLightFault)
REGISTER_NOTIFY_TEST_FUNC(TT_ACC_FAIL, accFail)
REGISTER_NOTIFY_TEST_FUNC(TT_LDW_OFF, ldwOff)
REGISTER_NOTIFY_TEST_FUNC(TT_HILL_DESCENT, hillDescent)
REGISTER_NOTIFY_TEST_FUNC(TT_AUTO_HI_BEAM_GREEN, autoHiBeamGreen)
REGISTER_NOTIFY_TEST_FUNC(TT_AUTO_HI_BEAM_AMBER, autoHiBeamAmber)
REGISTER_NOTIFY_TEST_FUNC(TT_LDW_OPERATE, ldwOperate)
REGISTER_NOTIFY_TEST_FUNC(TT_GENERAL_WARN, generalWarn)
REGISTER_NOTIFY_TEST_FUNC(TT_SPORTS_MODE, sportsMode)
REGISTER_NOTIFY_TEST_FUNC(TT_DRIVING_POWER_MODE, drivingPowerMode)
REGISTER_NOTIFY_TEST_FUNC(TT_HOT_TEMP, hotTemp)
REGISTER_NOTIFY_TEST_FUNC(TT_LOW_TEMP, lowTemp)

void SetTestSuiteRegisterAndNotify(void)
{
    CU_pSuite testSuite;

    testSuite = CU_add_suite("RegisterAndNotify", NULL, NULL);
    CU_add_test(testSuite, "registerIcHmi_ALL_DifferentData", TestregisterIcHmi_ALL_DifferentData);
    CU_add_test(testSuite, "registerIcHmi_ALL_SameData", TestregisterIcHmi_ALL_SameData);
    CU_add_test(testSuite, "registerIcHmi_ALL_UnrelatedParams", TestregisterIcHmi_ALL_UnrelatedParams);
    CU_add_test(testSuite, "registerIcHmi_ALL_OFF", TestregisterIcHmi_ALL_OFF);
    CU_add_test(testSuite, "registerIcHmi_Set_UnusedFlag", TestregisterIcHmi_Set_UnusedFlag);
    CU_add_test(testSuite, "registerIcHmi_NullFunc", TestregisterIcHmi_NullFunc);
    REGISTER_NOTIFY_ADD_TEST(testSuite, TT_TURN_R);
    REGISTER_NOTIFY_ADD_TEST(testSuite, TT_TURN_L);
    REGISTER_NOTIFY_ADD_TEST(testSuite, TT_BRAKE);
    REGISTER_NOTIFY_ADD_TEST(testSuite, TT_SEATBELT);
    REGISTER_NOTIFY_ADD_TEST(testSuite, TT_HIGHBEAM);
    REGISTER_NOTIFY_ADD_TEST(testSuite, TT_DOOR);
    REGISTER_NOTIFY_ADD_TEST(testSuite, TT_EPS);
    REGISTER_NOTIFY_ADD_TEST(testSuite, TT_SRS_AIRBAG);
    REGISTER_NOTIFY_ADD_TEST(testSuite, TT_ABS);
    REGISTER_NOTIFY_ADD_TEST(testSuite, TT_LOW_BATTERY);
    REGISTER_NOTIFY_ADD_TEST(testSuite, TT_OIL_PRESS);
    REGISTER_NOTIFY_ADD_TEST(testSuite, TT_ENGINE);
    REGISTER_NOTIFY_ADD_TEST(testSuite, TT_FUEL);
    REGISTER_NOTIFY_ADD_TEST(testSuite, TT_IMMOBI);
    REGISTER_NOTIFY_ADD_TEST(testSuite, TT_TM_FAIL);
    REGISTER_NOTIFY_ADD_TEST(testSuite, TT_ESP_ACT);
    REGISTER_NOTIFY_ADD_TEST(testSuite, TT_ESP_OFF);
    REGISTER_NOTIFY_ADD_TEST(testSuite, TT_ADAPTING_LIGHTING);
    REGISTER_NOTIFY_ADD_TEST(testSuite, TT_AUTO_STOP);
    REGISTER_NOTIFY_ADD_TEST(testSuite, TT_AUTO_STOP_FAIL);
    REGISTER_NOTIFY_ADD_TEST(testSuite, TT_PARKING_LIGHTS);
    REGISTER_NOTIFY_ADD_TEST(testSuite, TT_FRONT_FOG);
    REGISTER_NOTIFY_ADD_TEST(testSuite, TT_EXTERIOR_LIGHT_FAULT);
    REGISTER_NOTIFY_ADD_TEST(testSuite, TT_ACC_FAIL);
    REGISTER_NOTIFY_ADD_TEST(testSuite, TT_LDW_OFF);
    REGISTER_NOTIFY_ADD_TEST(testSuite, TT_HILL_DESCENT);
    REGISTER_NOTIFY_ADD_TEST(testSuite, TT_AUTO_HI_BEAM_GREEN);
    REGISTER_NOTIFY_ADD_TEST(testSuite, TT_AUTO_HI_BEAM_AMBER);
    REGISTER_NOTIFY_ADD_TEST(testSuite, TT_LDW_OPERATE);
    REGISTER_NOTIFY_ADD_TEST(testSuite, TT_GENERAL_WARN);
    REGISTER_NOTIFY_ADD_TEST(testSuite, TT_SPORTS_MODE);
    REGISTER_NOTIFY_ADD_TEST(testSuite, TT_DRIVING_POWER_MODE);
    REGISTER_NOTIFY_ADD_TEST(testSuite, TT_HOT_TEMP);
    REGISTER_NOTIFY_ADD_TEST(testSuite, TT_LOW_TEMP);
}
