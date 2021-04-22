# IC service Cluster API

## Overview

This repository has 3 main elements:

* Cluster API library code: ./src ./include
* Test app for client: ./test/client_test

## How to build

```bash
# ipc library is required by cluster_api, so please check ipc library and headers are installed in the system.
$ ./buildtest.sh
```

## How to run

* For client_test

```bash
$ ./buildtest.sh -d
$ cd cmakeBuild/test/client_test
$ ./client_test


     CUnit - A Unit testing framework for C - Version 2.1-2
             http://cunit.sourceforge.net/


***************** CUNIT CONSOLE - MAIN MENU ******************************
(R)un  (S)elect  (L)ist  (A)ctivate  (F)ailures  (O)ptions  (H)elp  (Q)uit
Enter command: ★(r)to run all test automatically ★(q)to terminate
```

# Source configuration

- cluster_api/
    - include/
        - Set of external public headers
    - src/
        - cluster_api Library of implementation source
    - test/
        - client_test/
            - Cluster API Test environment of implementation source

# Build Configuration

When ./buildtest.sh run, Build shows as follow:

- cluster_api/
    - cmakeBuild/
        - src/
            - The library of Cluster API as follows:
                ```bash
                libcluster_api.so -> libcluster_api.so.1
                libcluster_api.so.1 -> libcluster_api.so.1.0.0
                libcluster_api.so.1.0.0
                ```
        - test/
            - client_test/
                - Cluster API Test running files
                    ```bash
                    client_test
                    dummy_server
                    ```

# Cluster API Implementation Content

- API function which becomes I/F for application Implemented according to IC-Service_API_rev0.4.docx.
- The main implementation of the process is to manage the data sent from the IC-Service server. When calling API from the application, extract only the relevant data from the managed data.
- In addition to IC-Service_API_rev0.4.docx, the following two APIs have also been implemented：
    - bool clusterInit(void)
        - Initialization process for using the Cluster API.
        - This API connects to the IC-Service server, assuming that the server has already started.
        - Return true bool value if processing successfully.
    - bool clusterTerm(void)
        - Termination　Cluster API Process.
        - Used to disconnect from the IC-Service server.
        - Return true bool value if processing successfully.
- Applications that include cluster_api.h and link with libcluster_api.so can call every API.

# client_test Test content

Implement operation tests for every API of Cluster API. The configuration is as follows：

- The test configures two executable files; client_test and dummy_server：
    - client_test: Cluster API Test Body
    - dummy_server: Mock IC-Service server used for Cluster API testing
- When executing the test, only execute client_test.
- dummy_server is automatically started within client_test.
- The wait time for dummy_server to start and for dummy_server to send is 10 msec. 
  (Since it is asynchronous with another process, it does not work as intended without waiting all time)
- client_test is an automated test program implemented using CUnit. All item tests and unit item tests are both possible according to CUnit specifications.
- ※ The test may rarely fail due to the timing of inter-process communication. 
(Test all 156 items 20 times, return 1 item FAIL)

Implementation of items test is as follows：

1. InitAndTerm (5 Items)
    - Verify that clusterInit() and clusterTerm() work as intended：
        - Verify the operation when calling clusterInit(), and clusterTerm() sequentially、in the state of the IC-Service server exists (normal system).
        - Verify that an error occurs due to an incorrect API call order:
            - Call clusterInit() 2 times.
            - Call clusterInit() then, call clusterTerm() 2 times.
            - Without calling clusterInit(), call clusterTerm().
        - Verify an error occurs when calling clusterInit()、in the state of the IC-Service server does not exist.
2. Telltale (52 Items)
    - Verify every API can obtain intended value as described in 3-4 Telltale of IC-Service_API_rev0.4.docx:
        - Each API Implemented one pattern at a time.
3. ShiftPosition (2 Items)
    - Verify every API can obtain intended value as described in 3-5 ShiftPosition of IC-Service_API_rev0.4.docx: 
        - Each API Implemented one pattern at a time.
4. Speed (2 Items)
    -  Verify every API can obtain intended value as described in 3-6 Speed of IC-Service_API_rev0.4.docx: 
        - Each API Implemented one pattern at a time.
5. Tacho (1 Item)
    - Verify every API can obtain intended value as described in 3-7 Tacho of IC-Service_API_rev0.4.docx: 
        - Each API Implemented one pattern at a time.
6. TripComputer (20 Items)
    -  Verify every API can obtain intended value as described in 3-8 TripComputer of IC-Service_API_rev0.4.docx:
        - Each API Implemented one pattern at a time.
7. RegisterAndNotify (74 Items)
    - Verify registerIcHmi() in 3-9 Register/Notify of IC-Service_API_rev0.4.docx works as intended:
        - In the 1st argument arg_1, if IC_HMI_TT_ALL test assigned:
            - Verify that the callback function is called whenever any of the 34 signals changed as described in 3-2-1 TellTale.
            - Verify that the callback function is not called if none of the 34 signals have changed as described in 3-2-1 TellTale.
            - Verify that the callback function is not called even if other signals than 34 signals have changed as described in 3-2-1 TellTale.
        -  In the 1st argument arg_1, If 0 assigned: Verify the callback function is not called even any of the 34 signals changed as described in 3-2-1 TellTale.
        - In the 1st argument arg_1, if other flags than the following flags 3-2-1-1～3-2-1-34 are set: Verify the callback function is not called when any of the 34 signals changed as described in 3-2-1 TellTale.
        - In the 2nd argument addr, if NULL assigned: Verify that an error occurs.
        - In the 1st argument arg_1, if 3-2-1-1～3-2-1-34 are assigned set values. The following two procedures are implemented:
            - Verify that the callback function is called only when the signal corresponding to the assigned flag is changed.
            - Verify that the callback function is not called; even the other 33 signals changed.
        - In the 1st argument arg_1, if one flag from 3-2-1-1～3-2-1-34 is assigned clear value. The following two procedures are implemented:
            - Verify that the callback function is not called even if the signal corresponding to the cleared flag changed.
            - Verify that there is no callback notification for an incorrectly cleared flag; even the other 33 signals changed. 
            (Callback notification for other signal changes occur)
