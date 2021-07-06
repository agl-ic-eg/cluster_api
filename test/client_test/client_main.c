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

// Test program for IC-service IPC inplementation
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <CUnit/CUnit.h>
#include <CUnit/Console.h>

#include <cluster_ipc.h>
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
