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

#include "client_test_common.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/epoll.h>

#include <cluster_ipc.h>

#define CLUSTER_TEST_EPOLL_WAIT_NUM (2)

int createEpoll(int waitNum)
{
    int fd = -1;

    fd = epoll_create(waitNum);
    if (fd < 0) {
        ERRNO_LOG(epoll_create);
        return -1;
    }

    return fd;
}

int createSocket(int epollFd, int *pOutTestFd)
{
    int rc;
    int fd = -1;
    int testFd = -1;
    struct sockaddr_un unixAddr;
    int len;
    struct epoll_event epollEv;

    fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0) {
        ERRNO_LOG(socket);
        return -1;
    }

    unixAddr.sun_family = AF_UNIX;
    strcpy(unixAddr.sun_path, CLUSTER_TEST_DOMAIN_PATH);
    len = sizeof(unixAddr.sun_family)+strlen(unixAddr.sun_path);
    rc = bind(fd, (struct sockaddr *)&unixAddr, len);
    if (rc < 0) {
        ERRNO_LOG(bind);
        goto err;
    }

    rc = listen(fd, 1);
    if (rc < 0) {
        ERRNO_LOG(listen);
        goto err;
    }

    testFd = accept(fd, (struct sockaddr*)&unixAddr, (socklen_t *)&len);
    if (testFd < 0) {
        ERRNO_LOG(accept);
        goto err;
    }

    memset(&epollEv, 0, sizeof(epollEv));
    epollEv.events = EPOLLIN;
    epollEv.data.fd = fd;
    epoll_ctl(epollFd, EPOLL_CTL_ADD, epollEv.data.fd, &epollEv);

    epollEv.events = EPOLLIN | EPOLLRDHUP;
    epollEv.data.fd = testFd;
    epoll_ctl(epollFd, EPOLL_CTL_ADD, epollEv.data.fd, &epollEv);

    *pOutTestFd = testFd;

    return fd;
err:
    shutdown(fd, SHUT_RDWR);
    close(fd);
    unlink(CLUSTER_TEST_DOMAIN_PATH);
    return -1;
}

void sendMessageLoop(void)
{
    int epollFd = -1;
    int socketFd = -1;
    int testFd = -1;
    int fdNum;
    int i;
    struct epoll_event epEvents[CLUSTER_TEST_EPOLL_WAIT_NUM];
    bool isRunning = true;
    int dummyData;
    FILE *fp;
    IPC_DATA_IC_SERVICE_S sendData;
    IPC_RET_E rc;
    int size;

    epollFd = createEpoll(CLUSTER_TEST_EPOLL_WAIT_NUM);
    if (epollFd < 0) {
        goto end;
    }

    socketFd = createSocket(epollFd, &testFd);
    if (socketFd < 0) {
        goto end;
    }

    while(isRunning == true) {
        fdNum = epoll_wait(epollFd, epEvents, CLUSTER_TEST_EPOLL_WAIT_NUM, -1);

        for (i = 0; i < fdNum; i++) {
            if (epEvents[i].events & EPOLLRDHUP) {
                isRunning = false;
                break;
            }
            else if (epEvents[i].events & EPOLLIN) {
                size = read(epEvents[i].data.fd, &dummyData, sizeof(dummyData));
                if (size < 0) {
                    ERRNO_LOG(read);
                }
                fp = fopen(CLUSTER_TEST_SENDDATA_FILE, "rb");
                if (fp != NULL) {
                    size = fread(&sendData, 1, sizeof(sendData), fp);
                    if (size < sizeof(sendData) && ferror(fp)) {
                        ERRNO_LOG(fread);
                    }
                    fclose(fp);
                    rc = ipcSendMessage(IPC_USAGE_TYPE_IC_SERVICE, &sendData, sizeof(sendData));
                    if (rc != IPC_RET_OK) {
                        fprintf(stderr, "dummy_server:%d Failed to ipcSendMessage() : %d\n", __LINE__, rc);
                    }
                }
            }
        }
    }

end:
    if (testFd >= 0) {
        shutdown(testFd, SHUT_RDWR);
        close(testFd);
    }

    if (socketFd >= 0) {
        shutdown(socketFd, SHUT_RDWR);
        close(socketFd);
        unlink(CLUSTER_TEST_DOMAIN_PATH);
    }

    if (epollFd >= 0) {
        close(epollFd);
    }
}

int main(void)
{
    IPC_RET_E ret;

    setenv(IPC_ENV_DOMAIN_SOCKET_PATH, IPC_TEST_DOMAIN_PATH, 1);

    ret = ipcServerStart(IPC_USAGE_TYPE_IC_SERVICE);
    if (ret != IPC_RET_OK) {
        fprintf(stderr, "dummy_server:%d Failed to ipcServerStart() : %d\n", __LINE__, ret);
        exit(EXIT_FAILURE);
    }

    sendMessageLoop();

    ret = ipcServerStop(IPC_USAGE_TYPE_IC_SERVICE);
    if (ret != IPC_RET_OK) {
        fprintf(stderr, "dummy_server:%d Failed to ipcServerStop() : %d\n", __LINE__, ret);
        exit(EXIT_FAILURE);
    }

    return 0;
}

