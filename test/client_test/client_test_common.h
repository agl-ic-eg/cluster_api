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

#ifndef CLIENT_TEST_COMMON_H
#define CLIENT_TEST_COMMON_H

#include <unistd.h>
#include <cluster_ipc.h>

#define IPC_TEST_DOMAIN_PATH "./"

#define CLUSTER_TEST_DOMAIN_PATH "cluster_test_domain"
#define CLUSTER_TEST_SENDDATA_FILE "cluster_test.dat"

#define ERRNO_LOG(func) \
  do { \
      perror(#func "()"); \
      fprintf(stderr, "%s:%d Failed to %s()\n", __FILE__, __LINE__, #func); \
  }while(0)

void setExeFilePath(const char *exeFilePath);
void startDummyServer(pid_t child);
void stopDummyServer(void);
void connectToDummyServer(void);
void requestSendData(IPC_DATA_IC_SERVICE_S *pSendData);

#endif // CLIENT_TEST_COMMON_H
