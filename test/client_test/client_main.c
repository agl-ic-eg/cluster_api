// Test program for IC-service IPC inplementation
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <CUnit/CUnit.h>
#include <CUnit/Console.h>

#include <ipc.h>
#include <cluster_api.h>

#include "client_test_common.h"
#include "client_test_suites.h"

int main(int argc, char *argv[])
{
    pid_t child;

    setenv(IPC_ENV_DOMAIN_SOCKET_PATH, IPC_TEST_DOMAIN_PATH, 1);

    setExeFilePath(argv[0]);
    if ((child = fork()) < 0) {
        fprintf(stderr, "client_main:%d Failed to fork() : %d\n", __LINE__, child);
        exit(EXIT_FAILURE);
    }

    startDummyServer(child);
    connectToDummyServer();

    CU_initialize_registry();

    SetTestSuiteInitAndTerm();
    SetTestSuiteTelltale();
    SetTestSuiteShiftPosition();
    SetTestSuiteSpeed();
    SetTestSuiteTacho();
    SetTestSuiteTripComputer();
    SetTestSuiteRegisterAndNotify();

    CU_console_run_tests();

    CU_cleanup_registry();

    stopDummyServer();

    return 0;
}
