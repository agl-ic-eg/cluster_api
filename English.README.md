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
        - cluster_api Library implementation source
    - test/
        - client_test/
            - Cluster API Test environment implementation source

# Product Configuration

Product when executing ./buildtest.sh shows as follow:

- cluster_api/
    - cmakeBuild/
        - src/
            - The library of Cluster API as follow:
                ```bash
                libcluster_api.so -> libcluster_api.so.1
                libcluster_api.so.1 -> libcluster_api.so.1.0.0
                libcluster_api.so.1.0.0
                ```
        - test/
            - client_test/
                - Cluster API Test executing file
                    ```bash
                    client_test
                    dummy_server
                    ```

# Cluster API Implementation Content

- I/F API functions for apps were implemented as shown in IC-Service_API_rev0.4.docx.
- The main implementation process is to manage the data sent from the server for the IC-Service, and when the API is called from the application, only the relevant data is extracted from the managed data.
- In addition to IC-Service_API_rev0.4.docx, the following two APIs have been additionally implemented：
    - bool clusterInit(void)
        - Initialization process for using the Cluster API.
        - This API connects to the server for IC-Service, so it is assumed that the server has already started.
        - Return a bool value with true if the operation was successful.
    - bool clusterTerm(void)
        - Termination　Cluster API Process。
        - IC-Service用サーバとの接続を切断するために使用。
        - 処理成功時をtrueとしたbool値を返す。
- アプリはcluster_api.hをincludeし、libcluster_api.soとリンクすることで、各種APIを呼び出すことが可能となる。

# client_test テスト内容

Cluster APIの各種APIに対する動作テストを実施する。構成は以下の通り。

- テストは実行ファイルclient_testとdummy_serverの2つで構成される。
    - client_test : Cluster APIテスト本体
    - dummy_server : Cluster APIテストに使用する疑似IC-Serviceサーバ
- テスト実行時はclient_testだけ実行すれば良い。
- dummy_serverはclient_test内から自動で起動される。
- dummy_serverの起動待ち、dummy_serverからの送信待ちについては10msecとしている。  
  (別プロセスで非同期であることから、全く待ちがないと意図通りに動作しない)
- client_testはCUnitを用いて実装した自動テストプログラムである。CUnitの仕様通り全項目テスト、単体項目テスト、いずれも可能である。
- ※プロセス間通信のタイミングの関係上、稀にテストがFAILすることがある。  
(テスト全156項目を20周繰り返して1項目FAILする程度)

実施するテスト項目については以下の通り。

1. InitAndTerm (5項目)
    - clusterInit()とclusterTerm()が意図通り動作することを確認する。
        - IC-Service用サーバが存在する状態でclusterInit()とclusterTerm()を順に呼ぶ時の動作を確認する(正常系)
        - APIの呼び出し順誤りでエラーになることを確認する。
            - clusterInit()2連続呼び出し。
            - clusterInit()後、clusterTerm()を2連続呼び出し。
            - clusterInit()を呼ばずにclusterTerm()を呼び出し。
        - IC-Service用サーバが存在しない状態でのclusterInit()呼び出しでエラーになることを確認する。
2. Telltale (52項目)
    - IC-Service_API_rev0.4.docx の 3-4 Telltale に記載されている各種APIで意図通りの値が取得できることを確認する。
        - 各API 1パターンずつ実施。
3. ShiftPosition (2項目)
    - IC-Service_API_rev0.4.docx の 3-5 ShiftPosition に記載されている各種APIで意図通りの値が取得できることを確認する。
        - 各API 1パターンずつ実施。
4. Speed (2項目)
    - IC-Service_API_rev0.4.docx の 3-6 Speed に記載されている各種APIで意図通りの値が取得できることを確認する。
        - 各API 1パターンずつ実施。
5. Tacho (1項目)
    - IC-Service_API_rev0.4.docx の 3-7 Tacho に記載されている各種APIで意図通りの値が取得できることを確認する。
        - 各API 1パターンずつ実施。
6. TripComputer (20項目)
    - IC-Service_API_rev0.4.docx の 3-8 TripComputer に記載されている各種APIで意図通りの値が取得できることを確認する。
        - 各API 1パターンずつ実施。
7. RegisterAndNotify (74項目)
    - IC-Service_API_rev0.4.docx の 3-9 Register/Notify にある registerIcHmi()の動作が意図通りであることを確認する。
        - 第1引数 arg_1 に IC_HMI_TT_ALL を指定した時のテスト。
            - 3-2-1 TellTaleに記載された34種のどのシグナルが変化してもコールバック関数が呼ばれることを確認する。
            - 3-2-1 TellTaleに記載された34種いずれのシグナルも変化していなければコールバック関数が呼ばれないことを確認する。
            - 3-2-1 TellTaleに記載された34種のシグナル以外が変化してもコールバック関数が呼ばれないことを確認する。
        - 第1引数 arg_1 に 0 を指定した場合に、3-2-1 TellTaleに記載された34種のどのシグナルが変化してもコールバック関数が呼ばれないことを確認する。
        - 第1引数 arg_1 に 3-2-1-1～3-2-1-34 に示されたフラグ以外全てをセットした場合に、3-2-1 TellTaleに記載された34種のどのシグナルが変化してもコールバック関数が呼ばれないことを確認する。
        - 第2引数 addr にNULLが指定された場合にエラーとなることを確認する。
        - 第1引数 arg_1 に 3-2-1-1～3-2-1-34 のいずれか1つをセットした値を指定した場合について以下の2つを実施する。
            - 指定したフラグに該当するシグナルが変化した時だけコールバック関数が呼ばれることを確認する。
            - 他の33種のシグナルが変化してもコールバック関数が呼ばれないことを確認する。
        - 第1引数 arg_1 に 3-2-1-1～3-2-1-34 のいずれか1つのフラグだけクリアした値を指定した場合について以下の2つを実施する。
            - クリアしたフラグに該当するシグナルが変化しても、コールバック関数が呼ばれないことを確認する。
            - 他の33種のシグナルが変化しても、誤ってクリアしたフラグに関するコールバック通知が行われないことを確認する。  
            (他のシグナル変化へのコールバック通知は行われること)
