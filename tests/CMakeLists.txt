include(FetchContent)

FetchContent_Declare(
    check
    GIT_REPOSITORY https://github.com/libcheck/check.git
    GIT_TAG     0.15.2
)
FetchContent_GetProperties(check)
if (NOT check_POPULATED)
    FetchContent_Populate(check)

    add_subdirectory(${check_SOURCE_DIR} ${check_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

set(TEST_EXEC_LIST "")
macro(add_test_executable exec_name)
    add_executable("build.${exec_name}" EXCLUDE_FROM_ALL ${ARGN})
    set_target_properties("build.${exec_name}"
        PROPERTIES
            OUTPUT_NAME ${exec_name})
    target_link_libraries("build.${exec_name}" ${PROJECT_NAME} Check::check)
    list(APPEND TEST_EXEC_LIST ${exec_name})
endmacro()

add_library(main_suite EXCLUDE_FROM_ALL OBJECT main_suite.c)
target_link_libraries(main_suite Check::check)

add_test_executable(list_test list_test.c $<TARGET_OBJECTS:main_suite>)
add_test_executable(avl_test avl_test.c $<TARGET_OBJECTS:main_suite>)

