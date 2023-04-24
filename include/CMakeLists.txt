function(target_phs target)
    list(TRANSFORM ARGN PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/fjx-utils/)
    set_target_properties(${target} PROPERTIES PUBLIC_HEADER "${ARGN}")
endfunction()

if (IS_SUBPROJECT)
    add_library(${PROJECT_NAME}.utils-headers INTERFACE)
    add_library(fjx-libs::utils-headers ALIAS ${PROJECT_NAME}.utils-headers)
    target_include_directories(${PROJECT_NAME}.utils-headers
        INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

    target_link_libraries(${PROJECT_NAME}.utils-objects
        PRIVATE     fjx-libs::utils-headers)
    target_link_libraries(${PROJECT_NAME}.utils
        INTERFACE   fjx-libs::utils-headers)
    target_link_libraries(${PROJECT_NAME}.utils-shared
        INTERFACE   fjx-libs::utils-headers)
else()
    add_library(utils-headers INTERFACE)
    add_library(fjx-libs::utils-headers ALIAS utils-headers)
    target_include_directories(utils-headers
        INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        INTERFACE $<INSTALL_INTERFACE:include>)
    target_phs(utils-headers
        utils.h
        list.h
        bstree.h
        avl_tree.h)

    target_link_libraries(utils-objects PRIVATE     fjx-libs::utils-headers)
    target_link_libraries(utils         INTERFACE   fjx-libs::utils-headers)
    target_link_libraries(utils-shared  INTERFACE   fjx-libs::utils-headers)
endif()
