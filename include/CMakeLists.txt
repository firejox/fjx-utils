function(target_phs target)
    list(TRANSFORM ARGN PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/fjx-utils/)
    set_target_properties(${target} PROPERTIES PUBLIC_HEADER "${ARGN}")
endfunction()

target_phs(${PROJECT_NAME}
    utils.h
    list.h
    bstree.h
    avl_tree.h)

target_include_directories(${PROJECT_NAME}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
