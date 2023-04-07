find_package(Check REQUIRED)

find_program(DEBIAN "dpkg")
if (DEBIAN)
    find_package(Threads REQUIRED)
    set(CHECK_LIBRARIES ${CHECK_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} m subunit)
endif()

set(TEST_EXEC_LIST "")
macro(add_test_executable exec_name)
    add_executable(${exec_name} EXCLUDE_FROM_ALL ${ARGN})
    target_include_directories(${exec_name}
        PUBLIC ${CHECK_INCLUDE_DIR}
        PUBLIC ${PROJECT_SOURCE_DIR}/include
    )
    target_link_libraries(${exec_name} ${PROJECT_NAME} ${CHECK_LIBRARIES})
    list(APPEND TEST_EXEC_LIST ${exec_name})
endmacro()

add_library(main_suite OBJECT main_suite.c)

add_test_executable(list_test list_test.c $<TARGET_OBJECTS:main_suite>)
add_test_executable(avl_test avl_test.c $<TARGET_OBJECTS:main_suite>)

