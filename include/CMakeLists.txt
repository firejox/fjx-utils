function(target_phs target)
    list(TRANSFORM ARGN PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/fjx-utils/)
    set_target_properties(${target} PROPERTIES PUBLIC_HEADER "${ARGN}")
endfunction()

if (IS_SUBPROJECT)
    target_include_directories(${PROJECT_NAME}.utils-objects
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
    target_include_directories(${PROJECT_NAME}.utils-shared
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
    target_include_directories(${PROJECT_NAME}.utils
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
else()
    target_include_directories(utils-objects
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
    target_include_directories(utils-shared
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
    target_include_directories(utils
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

    target_phs(utils-objects
        utils.h
        list.h
        bstree.h
        avl_tree.h)
endif()
