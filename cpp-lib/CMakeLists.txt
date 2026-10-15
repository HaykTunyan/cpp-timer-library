
cmake_minimum_required(VERSION 3.16)
project(TimerLib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


option(BUILD_SHARED_LIBS "Build shared library instead of static" OFF)


add_library(timer src/timer.cpp)


target_include_directories(timer PUBLIC include)


# Build unit tests
option(BUILD_TESTS "Build unit tests" ON)

if(BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE 
    )
    FetchContent_MakeAvailable(googletest)

    enable_testing()
    add_executable(timer_tests tests/timer_tests.cpp)
    target_link_libraries(timer_tests PRIVATE timer GTest::gtest_main)

    include(GoogleTest)
    gtest_discover_tests(timer_tests)
endif()