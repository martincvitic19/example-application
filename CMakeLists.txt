# Copyright (c) 2021 Nordic Semiconductor ASA
# SPDX-License-Identifier: Apache-2.0
#
# This CMake file is picked by the Zephyr build system because it is defined
# as the module CMake entry point (see zephyr/module.yml).


# This is needed so that custom driver classes using system calls are taken into
# account
zephyr_syscall_include_directories(include)

zephyr_include_directories(include)

add_subdirectory(drivers)
add_subdirectory(lib)

# Add Google Test and Google Mock
find_package(GTest REQUIRED)
find_package(GMock REQUIRED)

# Include directories
include_directories(${GTEST_INCLUDE_DIRS} ${GMOCK_INCLUDE_DIRS})

# Add tests to build
target_link_libraries(non_trivial_button_led_app gtest gmock)