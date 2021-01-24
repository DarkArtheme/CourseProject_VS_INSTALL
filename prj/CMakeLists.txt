set(SOURCE_FILES BaseEnemy.h BaseEnemy.cpp
        BaseTower.h BaseTower.cpp
        Button.h Button.cpp
        ContextMenu.h ContextMenu.cpp
        GridMaker.h GridMaker.cpp
        Loader.h Loader.cpp
        main.cpp
        TmxLevel.h TmxLevel.cpp
        LevelMaker.cpp LevelMaker.h )


set(SFML_STATIC_LIBRARIES FALSE)

set(CMAKE_MODULE_PATH "${SFML_DIR}/cmake/Modules")

find_package(SFML REQUIRED COMPONENTS audio graphics window system)

include_directories(${SFML_INCLUDE_DIR})
add_executable (${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES} tinyxml2)




