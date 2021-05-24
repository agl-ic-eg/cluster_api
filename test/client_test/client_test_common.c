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
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <cluster_api.h>

#include "client_test_common.h"

#define CLUSTER_TEST_WAIT_TIME (10000)

static int g_dummyServerFd = -1;
static char g_dummyServerExeFilePath[256];

void setExeFilePath(const char *exeFilePath)
{
    char *pCh;

    // dummy_server in the same path as client_test.
    strcpy(g_dummyServerExeFilePath, exeFilePath);
    pCh = strrchr(g_dummyServerExeFilePath, '/');
    pCh = pCh != NULL ? pCh + 1: g_dummyServerExeFilePath;
    strcpy(pCh, "dummy_server");
}

void startDummyServer(pid_t child)
{
    if (child == 0) {
        int rc;
        char *argv[2] = {NULL, NULL};

        argv[0] = g_dummyServerExeFilePath;

        rc = execv(argv[0], argv);
        if (rc < 0) {
            ERRNO_LOG(execv);
        }
    }
    else {
        usleep(CLUSTER_TEST_WAIT_TIME);
    }
}

void stopDummyServer(void)
{
    if (g_dummyServerFd >= 0) {
        shutdown(g_dummyServerFd, SHUT_RDWR);
        close(g_dummyServerFd);
    }
    unlink(CLUSTER_TEST_SENDDATA_FILE);
    sleep(1);
}

void connectToDummyServer(void)
{
    int rc;
    int fd;
    struct sockaddr_un unixAddr;
    int len;

    fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0) {
        ERRNO_LOG(socket);
        return;
    }
    unixAddr.sun_family = AF_UNIX;
    strcpy(unixAddr.sun_path, CLUSTER_TEST_DOMAIN_PATH);
    len = sizeof(unixAddr.sun_family)+strlen(unixAddr.sun_path);
    rc = connect(fd, (struct sockaddr *)&unixAddr, len);
    if (rc != 0) {
        ERRNO_LOG(connect);
        shutdown(fd, SHUT_RDWR);
        close(fd);
        return;
    }

    g_dummyServerFd = fd;
}

void requestSendData(IPC_DATA_IC_SERVICE_S *pSendData)
{
    FILE *fp = NULL;
    int dummy = 1;
    int size;

    fp = fopen(CLUSTER_TEST_SENDDATA_FILE, "wb");
    if (fp != NULL) {
        size = fwrite(pSendData, 1, sizeof(IPC_DATA_IC_SERVICE_S), fp);
        if (size < sizeof(IPC_DATA_IC_SERVICE_S)) {
            ERRNO_LOG(fwrite);
        }
        fclose(fp);

        size = write(g_dummyServerFd, &dummy, sizeof(dummy));
        if (size < 0) {
            ERRNO_LOG(write);
        }
        usleep(CLUSTER_TEST_WAIT_TIME);
    }
}

