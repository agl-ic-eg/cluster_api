# Copyright (c) 2021, Nippon Seiki Co., Ltd.
# SPDX-License-Identifier: Apache-2.0

project(cluster_api C)

cmake_minimum_required(VERSION 2.8)

set(CLUSTER_API_LIBRARY_VERSION "1.0.0")

# Name of library API
set(TARGET_NAME cluster_api)

find_package(PkgConfig)

# Build option for DEBUG
option(DEBUG_BUILD "This is debug build." OFF)

# set gcc flags
set(CMAKE_C_FLAGS "-Wall -O2")

if(DEBUG_BUILD)
    add_definitions(-DDEBUG)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
    set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/installdir/usr CACHE PATH "..." FORCE)
endif()

include(GNUInstallDirs)

# Subdirectories
add_subdirectory(src)
if(DEBUG_BUILD)
    add_subdirectory(test/client_test)
endif()

configure_file(cluster_api.pc.in cluster_api.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/cluster_api.pc
    DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

