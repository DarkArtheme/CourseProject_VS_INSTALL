
set(SOURCE_FILES BaseEnemy.h BaseEnemy.cpp
        BaseTower.h BaseTower.cpp
        Button.h Button.cpp
        ContextMenu.h ContextMenu.cpp
        GridMaker.h GridMaker.cpp
        Loader.h Loader.cpp
        main.cpp
        TmxLevel.h TmxLevel.cpp
        LevelMaker.cpp LevelMaker.h )

#target_link_libraries(${PROJECT_NAME} tinyxml2)

#if (CMAKE_BUILD_TYPE STREQUAL "Release")
#    add_executable(${PROJECT_NAME} WIN32 ${SOURCE_FILES})
#else ()
#    add_executable(${PROJECT_NAME} ${SOURCE_FILES})
#endif ()


set(SFML_ROOT ${CMAKE_SOURCE_DIR}/libs/SFML-2.4.2)

set(SFML_STATIC_LIBRARIES FALSE)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/libs/SFML-2.4.2/cmake/Modules")
find_package(SFML REQUIRED COMPONENTS audio graphics window system)

include_directories(${SFML_INCLUDE_DIR})
add_executable (${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES} tinyxml2)




