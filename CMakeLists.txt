cmake_minimum_required(VERSION 3.13)

project(fjx-utils C)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 11)

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

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(DIRECTORY include/fjx-utils DESTINATION include)
