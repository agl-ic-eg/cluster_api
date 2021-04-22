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
Enter command: ★(r)to run all test automatically ★(q)to End
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
- In addition to IC-Service_API_rev0.4.docx, the following two APIs have been additionally implemented：
    - bool clusterInit(void)
        - Initialization process for using the Cluster API.
        - This API connects to the server for IC-Service, so it is assumed that the server has already started.
        - Return a bool value with true if the operation was successful.
    - bool clusterTerm(void)
        - Termination　Cluster API Process.
        - Used to disconnect from the IC-Service server.
        - Return a bool value with true if the operation was successful。
- Apps can include cluster_api.h and link with libcluster_api.so to call various APIs.

# client_test Test content

Perform operation tests for various APIs of Cluster API. The configuration is as follows：

- The test consists of two executable files, client_test and dummy_server：
    - client_test : Cluster API Test Body
    - dummy_server : Mock IC-Service server for Cluster API testing
- When running The test, only should run client_test.
- dummy_server is automatically started within client_test.
- The wait time for dummy_server to start and for dummy_server to send is 10 msec. 
  (Since it is asynchronous with another process, it does not work as intended if there is no wait)
- client_test is an automatic test program implemented using CUnit, All item tests and unit item test are both possible according to CUnit specifications.
- ※In rare cases, a test may fail due to timing of inter-process communication.  
(Test all 156 items 20 times, returnt 1 item FAIL)

The test items to be implemented are as follows：

1. InitAndTerm (5 Items)
    - Verify that clusterInit () and clusterTerm () work as intended：
        - Confirm the operation when calling clusterInit() and clusterTerm() sequentially when the IC-Service server exists (normal system).
        - Confirm that an error occurs due to an incorrect API call order:
            - Call clusterInit() 2 times.
            - Call clusterInit() then, clusterTerm() 2 times.
            - Without calling clusterInit(), call clusterTerm().
        - Confirm an error occurs when calling clusterInit() when the IC-Service server does not exist.
2. Telltale (52 Items)
    - Confirm each APIs can obtain intended value as described in 3-4 Telltale of IC-Service_API_rev0.4.docx:
        - Each API Implemented one pattern at a time.
3. ShiftPosition (2 Items)
    - Confirm each APIs can obtain intended value as described in 3-5 ShiftPosition of IC-Service_API_rev0.4.docx: 
        - Each API Implemented one pattern at a time.
4. Speed (2 Items)
    -  Confirm each APIs can obtain intended value as described in 3-6 Speed of IC-Service_API_rev0.4.docx: 
        - Each API Implemented one pattern at a time.
5. Tacho (1 Item)
    - Confirm each APIs can obtain intended value as described in 3-7 Tacho of IC-Service_API_rev0.4.docx: 
        - Each API Implemented one pattern at a time.
6. TripComputer (20 Items)
    -  Confirm each APIs can obtain intended value as described in 3-8 TripComputer of IC-Service_API_rev0.4.docx:
        - Each API Implemented one pattern at a time.
7. RegisterAndNotify (74 Items)
    - Confirm registerIcHmi() in 3-9 Register/Notify of IC-Service_API_rev0.4.docx works as intended:
        - In the 1st argument arg_1, IC_HMI_TT_ALL Test when specified:
            - Confirm that the callback function is called whenever any of the 34 signals changed as described in 3-2-1 TellTale.
            - Confirm that the callback function is not called if none of the 34 signals have changed as described in 3-2-1 TellTale.
            - Confirm that the callback function is not called even if other signal than 34 signals have changed as described in 3-2-1 TellTale.
        - In the 1st argument arg_1, If 0 is specified, confirm that the callback function is not called even if any of the 34 signals changed as described in 3-2-1 TellTale.
        - In the 1st argument arg_1, if flags other than following flags 3-2-1-1～3-2-1-34 are set、Confirm that the callback function is not called when any of the 34 signals changed as described in 3-2-1 TellTale.
        - In the 2nd argument addr, if NULL is specified: Confirm that an error occurs.
        - In the 1st argument arg_1, if 3-2-1-1～3-2-1-34 are specified set values: The following two procedures are implemented:
            - Confirm that the callback function is called only when the signal corresponding to the specified flag changed.
            - Confirm that the callback function is not called, even the other 33 signals changed.
        - In the 1st argument arg_1, if one flag from 3-2-1-1～3-2-1-34 are specified clear value, The following two procedures are implemented:
            - Verify that the callback function is not called even if the signal corresponding to the cleared flag changes.
            - Verify that there is no callback notification for an incorrectly cleared flag, even the other 33 signals changed. 
            (Callback notification for other signal changes occur)
