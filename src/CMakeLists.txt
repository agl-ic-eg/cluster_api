# Copyright (c) 2021, Nippon Seiki Co., Ltd.
# SPDX-License-Identifier: Apache-2.0

# Define project Targets
add_library(${TARGET_NAME} SHARED
    cluster_api.c
)

pkg_check_modules(IPC REQUIRED cluster_ipc)

# Include directories
target_include_directories(${TARGET_NAME} PRIVATE
    ./
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    $<BUILD_INTERFACE:${IPC_INCLUDE_DIRS}>
    $<INSTALL_INTERFACE:/include>
)

# find thread library
find_package(Threads REQUIRED)
target_link_libraries(${TARGET_NAME} INTERFACE
    Threads::Threads
    ${IPC_LIBRARIES})

set_target_properties(${TARGET_NAME}
    PROPERTIES
    VERSION ${CLUSTER_API_LIBRARY_VERSION}
    SOVERSION 1
)

set(CLUSTER_API_HEADERS
    ../include/cluster_api.h ../include/cluster_api_registernotify.h
    ../include/cluster_api_shiftposition.h
    ../include/cluster_api_speed.h
    ../include/cluster_api_tacho.h
    ../include/cluster_api_telltale.h
    ../include/cluster_api_tripcomputer.h
    )

# make install
install(
    TARGETS ${TARGET_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
)

install (
    FILES ${CLUSTER_API_HEADERS} DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR}
)
