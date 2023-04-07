cmake_minimum_required(VERSION 3.13)

project(fjx-utils C)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 11)

include(CheckCSourceCompiles)
include(CheckCSourceRuns)
include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckLibraryExists)
include(CheckSymbolExists)
include(CheckTypeSize)

# Check headers
set(INCLUDES "")
macro(ck_check_include_file header var)
    check_include_files("${INCLUDES};${header}" ${var})
    if(${var})
        set(INCLUDES ${INCLUDES} ${header})
    endif(${var})
endmacro(ck_check_include_file)

ck_check_include_file("stdlib.h" HAVE_STDLIB_H)

# Check for integer types
# (The following are used in check.h. Regardless if they are used in
# the project, they will need to be checked in order to use Check).
check_type_size(intmax_t INTMAX_T)
check_type_size(uintmax_t UINTMAX_T)

check_type_size(pid_t PID_T)
if (NOT HAVE_PID_T)
    if (WIN32)
        set(pid_t "int")
    else (WIN32)
        MESSAGE(FATAL_ERROR "pid_t doesn't exist on this platform?")
    endif(WIN32)
endif(NOT HAVE_PID_T)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_C_FLAGS "-Wall -Wextra")
set(CMAKE_C_FLAGS_DEBUG "-O0 -ggdb")
set(CMAKE_C_FLAGS_RELEASE "-O2")

add_subdirectory(src)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_subdirectory(tests)
get_directory_property(TEST_EXEC_LIST DIRECTORY tests DEFINITION TEST_EXEC_LIST)

set(CMAKE_CTEST_ARGUMENTS "--output-on-failure")
enable_testing()

foreach (test_name IN LISTS TEST_EXEC_LIST)
    add_test(NAME "${test_name}" COMMAND ${CMAKE_COMMAND}
        --build ${CMAKE_BINARY_DIR}
        --target "${test_name}.test")
    add_custom_target("${test_name}.test" $<TARGET_FILE:${test_name}>)
    add_dependencies("${test_name}.test" ${test_name})
endforeach()
