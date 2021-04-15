#include <CUnit/CUnit.h>

#include <cluster_api.h>

#include "client_test_common.h"

#define TELLTALE_TEST_FUNC(func, param) \
    static void Test ##func() { \
        IPC_DATA_IC_SERVICE_S sendData; \
        CU_ASSERT(clusterInit() == true); \
        memset(&sendData, 0x00, sizeof(sendData)); \
        sendData.param = IC_HMI_ON; \
        requestSendData(&sendData); \
        CU_ASSERT(func() == IC_HMI_ON); \
        CU_ASSERT(clusterTerm() == true); \
    }

#define TELLTALE_ADD_TEST(testSuite, func) \
    CU_add_test(testSuite, #func, Test ##func)

TELLTALE_TEST_FUNC(getTurnR, turnR)
TELLTALE_TEST_FUNC(getTurnL, turnL)
TELLTALE_TEST_FUNC(getBrake, brake)
TELLTALE_TEST_FUNC(getSeatbelt, seatbelt)
TELLTALE_TEST_FUNC(getFrontRightSeatbelt, frontRightSeatbelt)
TELLTALE_TEST_FUNC(getFrontCenterSeatbelt, frontCenterSeatbelt)
TELLTALE_TEST_FUNC(getFrontLeftSeatbelt, frontLeftSeatbelt)
TELLTALE_TEST_FUNC(getMid1RightSeatbelt, mid1RightSeatbelt)
TELLTALE_TEST_FUNC(getMid1CenterSeatbelt, mid1CenterSeatbelt)
TELLTALE_TEST_FUNC(getMid1LeftSeatbelt, mid1LeftSeatbelt)
TELLTALE_TEST_FUNC(getMid2RightSeatbelt, mid2RightSeatbelt)
TELLTALE_TEST_FUNC(getMid2CenterSeatbelt, mid2CenterSeatbelt)
TELLTALE_TEST_FUNC(getMid2LeftSeatbelt, mid2LeftSeatbelt)
TELLTALE_TEST_FUNC(getRearRightSeatbelt, rearRightSeatbelt)
TELLTALE_TEST_FUNC(getRearCenterSeatbelt, rearCenterSeatbelt)
TELLTALE_TEST_FUNC(getRearLeftSeatbelt, rearLeftSeatbelt)
TELLTALE_TEST_FUNC(getHighbeam, highbeam)
TELLTALE_TEST_FUNC(getDoor, door)
TELLTALE_TEST_FUNC(getFrontRightDoor, frontRightDoor)
TELLTALE_TEST_FUNC(getFrontLeftDoor, frontLeftDoor)
TELLTALE_TEST_FUNC(getRearRightDoor, rearRightDoor)
TELLTALE_TEST_FUNC(getRearLeftDoor, rearLeftDoor)
TELLTALE_TEST_FUNC(getTrunkDoor, trunkDoor)
TELLTALE_TEST_FUNC(getHoodDoor, hoodDoor)
TELLTALE_TEST_FUNC(getEps, eps)
TELLTALE_TEST_FUNC(getSrsAirbag, srsAirbag)
TELLTALE_TEST_FUNC(getAbs, abs)
TELLTALE_TEST_FUNC(getLowBattery, lowBattery)
TELLTALE_TEST_FUNC(getOilPress, oilPress)
TELLTALE_TEST_FUNC(getEngine, engine)
TELLTALE_TEST_FUNC(getFuel, fuel)
TELLTALE_TEST_FUNC(getImmobi, immobi)
TELLTALE_TEST_FUNC(getTMFail, tmFail)
TELLTALE_TEST_FUNC(getEspAct, espAct)
TELLTALE_TEST_FUNC(getEspOff, espOff)
TELLTALE_TEST_FUNC(getAdaptingLighting, adaptingLighting)
TELLTALE_TEST_FUNC(getAutoStop, autoStop)
TELLTALE_TEST_FUNC(getAutoStopFail, autoStopFail)
TELLTALE_TEST_FUNC(getParkingLights, parkingLights)
TELLTALE_TEST_FUNC(getFrontFog, frontFog)
TELLTALE_TEST_FUNC(getExteriorLightFault, exteriorLightFault)
TELLTALE_TEST_FUNC(getAccFail, accFail)
TELLTALE_TEST_FUNC(getLdwOff, ldwOff)
TELLTALE_TEST_FUNC(getHillDescent, hillDescent)
TELLTALE_TEST_FUNC(getAutoHiBeamGreen, autoHiBeamGreen)
TELLTALE_TEST_FUNC(getAutoHiBeamAmber, autoHiBeamAmber)
TELLTALE_TEST_FUNC(getSportsMode, sportsMode)
TELLTALE_TEST_FUNC(getLdwOperate, ldwOperate)
TELLTALE_TEST_FUNC(getGeneralWarn, generalWarn)
TELLTALE_TEST_FUNC(getDriverPowerMode, drivingPowerMode)
TELLTALE_TEST_FUNC(getHotTemp, hotTemp)
TELLTALE_TEST_FUNC(getLowTemp, lowTemp)

void SetTestSuiteTelltale(void)
{
    CU_pSuite testSuite;

    testSuite = CU_add_suite("Telltale", NULL, NULL);
    TELLTALE_ADD_TEST(testSuite, getTurnR);
    TELLTALE_ADD_TEST(testSuite, getTurnL);
    TELLTALE_ADD_TEST(testSuite, getBrake);
    TELLTALE_ADD_TEST(testSuite, getSeatbelt);
    TELLTALE_ADD_TEST(testSuite, getFrontRightSeatbelt);
    TELLTALE_ADD_TEST(testSuite, getFrontCenterSeatbelt);
    TELLTALE_ADD_TEST(testSuite, getFrontLeftSeatbelt);
    TELLTALE_ADD_TEST(testSuite, getMid1RightSeatbelt);
    TELLTALE_ADD_TEST(testSuite, getMid1CenterSeatbelt);
    TELLTALE_ADD_TEST(testSuite, getMid1LeftSeatbelt);
    TELLTALE_ADD_TEST(testSuite, getMid2RightSeatbelt);
    TELLTALE_ADD_TEST(testSuite, getMid2CenterSeatbelt);
    TELLTALE_ADD_TEST(testSuite, getMid2LeftSeatbelt);
    TELLTALE_ADD_TEST(testSuite, getRearRightSeatbelt);
    TELLTALE_ADD_TEST(testSuite, getRearCenterSeatbelt);
    TELLTALE_ADD_TEST(testSuite, getRearLeftSeatbelt);
    TELLTALE_ADD_TEST(testSuite, getHighbeam);
    TELLTALE_ADD_TEST(testSuite, getDoor);
    TELLTALE_ADD_TEST(testSuite, getFrontRightDoor);
    TELLTALE_ADD_TEST(testSuite, getFrontLeftDoor);
    TELLTALE_ADD_TEST(testSuite, getRearRightDoor);
    TELLTALE_ADD_TEST(testSuite, getRearLeftDoor);
    TELLTALE_ADD_TEST(testSuite, getTrunkDoor);
    TELLTALE_ADD_TEST(testSuite, getHoodDoor);
    TELLTALE_ADD_TEST(testSuite, getEps);
    TELLTALE_ADD_TEST(testSuite, getSrsAirbag);
    TELLTALE_ADD_TEST(testSuite, getAbs);
    TELLTALE_ADD_TEST(testSuite, getLowBattery);
    TELLTALE_ADD_TEST(testSuite, getOilPress);
    TELLTALE_ADD_TEST(testSuite, getEngine);
    TELLTALE_ADD_TEST(testSuite, getFuel);
    TELLTALE_ADD_TEST(testSuite, getImmobi);
    TELLTALE_ADD_TEST(testSuite, getTMFail);
    TELLTALE_ADD_TEST(testSuite, getEspAct);
    TELLTALE_ADD_TEST(testSuite, getEspOff);
    TELLTALE_ADD_TEST(testSuite, getAdaptingLighting);
    TELLTALE_ADD_TEST(testSuite, getAutoStop);
    TELLTALE_ADD_TEST(testSuite, getAutoStopFail);
    TELLTALE_ADD_TEST(testSuite, getParkingLights);
    TELLTALE_ADD_TEST(testSuite, getFrontFog);
    TELLTALE_ADD_TEST(testSuite, getExteriorLightFault);
    TELLTALE_ADD_TEST(testSuite, getAccFail);
    TELLTALE_ADD_TEST(testSuite, getLdwOff);
    TELLTALE_ADD_TEST(testSuite, getHillDescent);
    TELLTALE_ADD_TEST(testSuite, getAutoHiBeamGreen);
    TELLTALE_ADD_TEST(testSuite, getAutoHiBeamAmber);
    TELLTALE_ADD_TEST(testSuite, getSportsMode);
    TELLTALE_ADD_TEST(testSuite, getLdwOperate);
    TELLTALE_ADD_TEST(testSuite, getGeneralWarn);
    TELLTALE_ADD_TEST(testSuite, getDriverPowerMode);
    TELLTALE_ADD_TEST(testSuite, getHotTemp);
    TELLTALE_ADD_TEST(testSuite, getLowTemp);
}

