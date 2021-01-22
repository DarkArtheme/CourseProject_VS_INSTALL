cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME SFML_Sandbox)

project(${PROJECT_NAME})
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W1")
set(CMAKE_CXX_STANDARD 17)


find_package(Doxygen)
option(BUILD_DOCUMENTATION "Create and install documentation (requires Doxygen)" ${DOXYGEN_FOUND})

if(DOXYGEN_FOUND)

	set(DOXYGEN ${CMAKE_SOURCE_DIR}/doc/Doxyfile.in)
	
    message("Doxygen build started")
	
	add_custom_target(
		docs ALL
		COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN}
		COMMENT "Generating API documentation with Doxygen"
		VERBATIM
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/doc
	)

endif()


include_directories(prj)
include_directories(libs)


set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}")

install(FILES 
	${CMAKE_SOURCE_DIR}/libs/SFML-2.4.2/bin/openal32.dll
	${CMAKE_SOURCE_DIR}/libs/SFML-2.4.2/bin/sfml-audio-2.dll
	${CMAKE_SOURCE_DIR}/libs/SFML-2.4.2/bin/sfml-graphics-2.dll
	${CMAKE_SOURCE_DIR}/libs/SFML-2.4.2/bin/sfml-system-2.dll
	${CMAKE_SOURCE_DIR}/libs/SFML-2.4.2/bin/sfml-window-2.dll 
	${CMAKE_SOURCE_DIR}/libs/SFML-2.4.2/bin/sfml-audio-d-2.dll
	${CMAKE_SOURCE_DIR}/libs/SFML-2.4.2/bin/sfml-graphics-d-2.dll
	${CMAKE_SOURCE_DIR}/libs/SFML-2.4.2/bin/sfml-system-d-2.dll
	${CMAKE_SOURCE_DIR}/libs/SFML-2.4.2/bin/sfml-window-d-2.dll 
	DESTINATION ${PROJECT_BINARY_DIR}/game)
	
	

install(DIRECTORY ${CMAKE_SOURCE_DIR}/src 
	DESTINATION ${PROJECT_BINARY_DIR}/game)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/doc DESTINATION ${PROJECT_BINARY_DIR})

add_subdirectory(prj)
add_subdirectory(libs)

install(TARGETS ${PROJECT_NAME} DESTINATION ${PROJECT_BINARY_DIR}/game)










