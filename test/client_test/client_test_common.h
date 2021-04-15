#ifndef CLIENT_TEST_COMMON_H
#define CLIENT_TEST_COMMON_H

#include <unistd.h>
#include <ipc.h>

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
