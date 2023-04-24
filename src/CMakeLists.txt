set(LIB_SOURCE avl_tree.c bstree.c)

if (IS_SUBPROJECT)
    add_library(${PROJECT_NAME}.utils-objects OBJECT ${LIB_SOURCE})
    add_library(fjx-libs::utils-objects ALIAS ${PROJECT_NAME}.utils-objects)
    target_link_libraries(${PROJECT_NAME}.utils-objects
        PRIVATE ${PROJECT_NAME}.utils-compl)

    add_library(${PROJECT_NAME}.utils STATIC)
    add_library(fjx-libs::utils ALIAS ${PROJECT_NAME}.utils)

    target_link_libraries(${PROJECT_NAME}.utils
        PRIVATE ${PROJECT_NAME}.utils-objects
        PRIVATE ${PROJECT_NAME}.utils-compl)

    add_library(${PROJECT_NAME}.utils-shared SHARED)
    add_library(fjx-libs::utils-shared ALIAS ${PROJECT_NAME}.utils-shared)

    target_link_libraries(${PROJECT_NAME}.utils-shared
        PRIVATE ${PROJECT_NAME}.utils-objects
        PRIVATE ${PROJECT_NAME}.utils-compl)

    set_target_properties(${PROJECT_NAME}.utils
        PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME})

    set_target_properties(${PROJECT_NAME}.utils-shared
        PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME})
else()
    add_library(utils-objects OBJECT ${LIB_SOURCE})
    target_link_libraries(utils-objects
        PRIVATE utils-compl)

    add_library(utils STATIC)
    add_library(fjx-libs::utils ALIAS utils)

    target_link_libraries(utils
        PRIVATE utils-objects
        PRIVATE utils-compl)

    add_library(utils-shared SHARED)
    add_library(fjx-libs::utils-shared ALIAS utils-shared)

    target_link_libraries(utils-shared
        PRIVATE utils-objects
        PRIVATE utils-compl)

    set_target_properties(utils
        PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME})

    set_target_properties(utils-shared
        PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME})
endif()

