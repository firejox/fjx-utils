cmake_minimum_required(VERSION 3.21)

set(IS_SUBPROJECT FALSE)
if (DEFINED PROJECT_NAME)
    set(IS_SUBPROJECT TRUE)
endif()

project(fjx-utils C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 23)

if (IS_SUBPROJECT)
    add_library(${PROJECT_NAME}.utils-compl-release INTERFACE)
    target_compile_options(${PROJECT_NAME}.utils-compl-release
        INTERFACE "$<$<OR:$<C_COMPILER_ID:GNU>,$<C_COMPILER_ID:Clang>>:-O3>")

    add_library(${PROJECT_NAME}.utils-compl INTERFACE)
    target_compile_options(${PROJECT_NAME}.utils-compl
        INTERFACE "$<$<OR:$<C_COMPILER_ID:GNU>,$<C_COMPILER_ID:Clang>>:-Wall;-Wextra>")

    target_link_libraries(${PROJECT_NAME}.utils-compl
        INTERFACE ${PROJECT_NAME}.utils-compl-release)
else()
    if (NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Release)
    endif()

    add_library(utils-compl-debug INTERFACE)
    add_library(fjx-libs::utils-compl-debug ALIAS utils-compl-debug)
    target_compile_options(utils-compl-debug
        INTERFACE "$<$<OR:$<C_COMPILER_ID:GNU>,$<C_COMPILER_ID:Clang>>:-O0;-ggdb>")

    add_library(utils-compl-release INTERFACE)
    add_library(fjx-libs::utils-compl-release ALIAS utils-compl-release)
    target_compile_options(utils-compl-release
        INTERFACE "$<$<OR:$<C_COMPILER_ID:GNU>,$<C_COMPILER_ID:Clang>>:-O3>")

    add_library(utils-compl INTERFACE)
    add_library(fjx-libs::utils-compl ALIAS utils-compl)
    target_compile_options(utils-compl
        INTERFACE "$<$<OR:$<C_COMPILER_ID:GNU>,$<C_COMPILER_ID:Clang>>:-Wall;-Wextra>")

    target_link_libraries(utils-compl
        INTERFACE debug fjx-libs::utils-compl-debug
        INTERFACE optimized fjx-libs::utils-compl-release)
endif()

add_subdirectory(src)
add_subdirectory(include)

if (NOT IS_SUBPROJECT)
    add_subdirectory(tests EXCLUDE_FROM_ALL)
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

    install(TARGETS
            utils
            utils-shared
            utils-objects
            utils-compl
            utils-compl-debug
            utils-compl-release
            utils-headers
        EXPORT utils-targets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        PUBLIC_HEADER DESTINATION include/fjx-utils)
    install(EXPORT utils-targets
        NAMESPACE "fjx-libs::"
        FILE utils.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/fjx-libs)
    export(EXPORT utils-targets
        NAMESPACE "fjx-libs::"
        FILE utils.cmake)

    set(CPACK_PACKAGE_NAME fjx-utils)
    set(CPACK_PACKAGE_CONTACT "Firejox firejox@gmail.com")
    set(CPACK_GENERATOR DEB)
    include(CPack)
endif()
