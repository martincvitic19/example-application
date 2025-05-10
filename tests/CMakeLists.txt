# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.20)

# Set the project name and version
project(non_trivial_button_led_app VERSION 1.0)

# Specify the directories for source files
set(SOURCES
    test_gpio.cpp  # Corrected source file name
    mock_read/mock_read.cpp
    # Add more source files if needed
)

# Define the executable for test_gpio target
add_executable(test_gpio ${SOURCES})

# Add the mock_read directory
target_include_directories(test_gpio PRIVATE
    ${CMAKE_SOURCE_DIR}/tests/mock_read
    ${CMAKE_SOURCE_DIR}/tests/mock_zephyr
    /usr/src/googletest/googletest/include  # GoogleTest include directory
    /usr/src/googletest/googlemock/include  # GoogleMock include directory
)

# Add the mock_read.cpp implementation
target_sources(test_gpio PRIVATE
    ${CMAKE_SOURCE_DIR}/mock_read/mock_read.cpp
)

# Specify where the GoogleTest and GoogleMock libraries are located
link_directories(
    /usr/src/googletest/googletest/lib   # GoogleTest library directory
    /usr/src/googletest/googlemock/lib   # GoogleMock library directory
)

# Link GoogleTest, GoogleMock, and pthread
target_link_libraries(test_gpio
    gtest
    gtest_main
    gmock
    gmock_main
    pthread
)

# Add the test so it runs with ctest
add_test(NAME test_gpio COMMAND test_gpio)
