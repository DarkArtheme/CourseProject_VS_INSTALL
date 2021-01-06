cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME SFML_Sandbox)

project(${PROJECT_NAME})
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W1")
set(CMAKE_CXX_STANDARD 17)


#set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}")


include_directories(prj)
include_directories(libs)

set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}")
install(FILES 
	${CMAKE_SOURCE_DIR}/libs/SFML_VS/bin/openal32.dll
	${CMAKE_SOURCE_DIR}/libs/SFML_VS/bin/sfml-audio-d-2.dll
	${CMAKE_SOURCE_DIR}/libs/SFML_VS/bin/sfml-graphics-d-2.dll
	${CMAKE_SOURCE_DIR}/libs/SFML_VS/bin/sfml-system-d-2.dll
	${CMAKE_SOURCE_DIR}/libs/SFML_VS/bin/sfml-window-d-2.dll 
	CONFIGURATIONS Debug 
	DESTINATION ${PROJECT_BINARY_DIR}/prj/Debug)
install(FILES 
	${CMAKE_SOURCE_DIR}/libs/SFML_VS/bin/openal32.dll
	${CMAKE_SOURCE_DIR}/libs/SFML_VS/bin/sfml-audio-d-2.dll
	${CMAKE_SOURCE_DIR}/libs/SFML_VS/bin/sfml-graphics-d-2.dll
	${CMAKE_SOURCE_DIR}/libs/SFML_VS/bin/sfml-system-d-2.dll
	${CMAKE_SOURCE_DIR}/libs/SFML_VS/bin/sfml-window-d-2.dll 
	CONFIGURATIONS Debug 
	DESTINATION ${PROJECT_BINARY_DIR}/prj)
	
install(FILES 
	${CMAKE_SOURCE_DIR}/libs/SFML_VS/bin/openal32.dll
	${CMAKE_SOURCE_DIR}/libs/SFML_VS/bin/sfml-audio-2.dll
	${CMAKE_SOURCE_DIR}/libs/SFML_VS/bin/sfml-graphics-2.dll
	${CMAKE_SOURCE_DIR}/libs/SFML_VS/bin/sfml-system-2.dll
	${CMAKE_SOURCE_DIR}/libs/SFML_VS/bin/sfml-window-2.dll 
	CONFIGURATIONS Release 
	DESTINATION ${PROJECT_BINARY_DIR}/prj/Release)
install(FILES 
	${CMAKE_SOURCE_DIR}/libs/SFML_VS/bin/openal32.dll
	${CMAKE_SOURCE_DIR}/libs/SFML_VS/bin/sfml-audio-2.dll
	${CMAKE_SOURCE_DIR}/libs/SFML_VS/bin/sfml-graphics-2.dll
	${CMAKE_SOURCE_DIR}/libs/SFML_VS/bin/sfml-system-2.dll
	${CMAKE_SOURCE_DIR}/libs/SFML_VS/bin/sfml-window-2.dll 
	CONFIGURATIONS Release 
	DESTINATION ${PROJECT_BINARY_DIR}/prj)
	
	
install(DIRECTORY ${CMAKE_SOURCE_DIR}/src CONFIGURATIONS Debug
	DESTINATION ${PROJECT_BINARY_DIR}/prj/Debug)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/src CONFIGURATIONS Debug
	DESTINATION ${PROJECT_BINARY_DIR}/prj)
	
install(DIRECTORY ${CMAKE_SOURCE_DIR}/src CONFIGURATIONS Release
	DESTINATION ${PROJECT_BINARY_DIR}/prj/Release)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/src CONFIGURATIONS Release
	DESTINATION ${PROJECT_BINARY_DIR}/prj)

add_subdirectory(prj)
add_subdirectory(libs)









