cmake_minimum_required(VERSION 3.13)

set(IS_SUBPROJECT FALSE)
if (DEFINED PROJECT_NAME)
    set(IS_SUBPROJECT TRUE)
endif()

project(fjx-utils C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 23)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_C_FLAGS "-Wall -Wextra")
set(CMAKE_C_FLAGS_DEBUG "-O0 -ggdb")
set(CMAKE_C_FLAGS_RELEASE "-O2")

add_subdirectory(src)
add_subdirectory(include)

if (NOT IS_SUBPROJECT)
    add_subdirectory(tests)
    get_directory_property(TEST_EXEC_LIST DIRECTORY tests DEFINITION TEST_EXEC_LIST)

    set(CMAKE_CTEST_ARGUMENTS "--output-on-failure")
    enable_testing()

    foreach (test_name IN LISTS TEST_EXEC_LIST)
        add_test(NAME "${test_name}" COMMAND ${CMAKE_COMMAND}
            --build ${CMAKE_BINARY_DIR}
            --target ${test_name})
        add_custom_target("${test_name}" $<TARGET_FILE:build.${test_name}>)
        add_dependencies(${test_name} "build.${test_name}")
    endforeach()

    add_library(fjx-libs::utils ALIAS ${PROJECT_NAME})

    install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include/fjx-utils)
    install(EXPORT ${PROJECT_NAME}
        FILE utils.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/fjx-libs)

    set(CPACK_PACKAGE_NAME fjx-utils)
    set(CPACK_PACKAGE_CONTACT "Firejox firejox@gmail.com")
    set(CPACK_GENERATOR DEB)
    include(CPack)
endif()
