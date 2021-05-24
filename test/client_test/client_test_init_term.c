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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <CUnit/CUnit.h>

#include <cluster_api.h>

#include "client_test_common.h"

// OK case. Calling the APIs of Init and Term while the server is already started.
static void TestInitAndTermOK(void)
{
    CU_ASSERT(clusterInit() == true);
    CU_ASSERT(clusterTerm() == true);
}

// NG case. Call the API of Init function twice.
static void TestInitAndTermNG1(void)
{
    CU_ASSERT(clusterInit() == true);
    CU_ASSERT(clusterInit() == false);
    CU_ASSERT(clusterTerm() == true);
}

// NG case. Call the API of Term function twice.
static void TestInitAndTermNG2(void)
{
    CU_ASSERT(clusterInit() == true);
    CU_ASSERT(clusterTerm() == true);
    CU_ASSERT(clusterTerm() == false);
}

// NG case. Call the API of Term only.
static void TestInitAndTermNG3(void)
{
    CU_ASSERT(clusterTerm() == false);
}

// NG case. Server is not ready.
static void TestInitAndTermNG4(void)
{
    pid_t child;

    stopDummyServer();
    CU_ASSERT(clusterInit() == false);
    CU_ASSERT(clusterTerm() == false);

    if ((child = fork()) < 0) {
        fprintf(stderr, "%s:%d Failed to fork() : %d\n", __FILE__, __LINE__, child);
        exit(EXIT_FAILURE);
    }

    startDummyServer(child);
    connectToDummyServer();
}

void SetTestSuiteInitAndTerm(void)
{
    CU_pSuite testSuite;

    testSuite = CU_add_suite("InitAndTerm", NULL, NULL);
    CU_add_test(testSuite, "InitAndTerm_OK", TestInitAndTermOK);
    CU_add_test(testSuite, "InitAndTerm_NG_1", TestInitAndTermNG1);
    CU_add_test(testSuite, "InitAndTerm_NG_2", TestInitAndTermNG2);
    CU_add_test(testSuite, "InitAndTerm_NG_3", TestInitAndTermNG3);
    CU_add_test(testSuite, "InitAndTerm_NG_4", TestInitAndTermNG4);
}

