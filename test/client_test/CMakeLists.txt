# Copyright (c) 2021, Nippon Seiki Co., Ltd.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 2.8)

pkg_check_modules(IPC REQUIRED cluster_ipc)

# use the variable <RESULTNAME>_INCLUDE_DIRS to get include paths
include_directories(
    ${IPC_INCLUDE_DIRS}
    ../../include
    )

# Define project Targets
set(CLIENT_TEST_BINARY_NAME client_test)
add_executable(${CLIENT_TEST_BINARY_NAME}
    client_main.c
    client_test_common.c
    client_test_init_term.c
    client_test_telltale.c
    client_test_shiftposition.c
    client_test_speed.c
    client_test_tacho.c
    client_test_tripcomputer.c
    client_test_register_notify.c
    )

# use the variable <RESULTNAME>_LIBRARIES to get library paths
target_link_libraries(${CLIENT_TEST_BINARY_NAME}
    ${IPC_LIBRARIES}
    ${TARGET_NAME}
    pthread
    cunit
    )

# Define project Targets
set(DUMMY_SERVER_BINARY_NAME dummy_server)
add_executable(${DUMMY_SERVER_BINARY_NAME} dummy_server.c)

target_link_libraries(${DUMMY_SERVER_BINARY_NAME}
    ${IPC_LIBRARIES}
    pthread
    )

