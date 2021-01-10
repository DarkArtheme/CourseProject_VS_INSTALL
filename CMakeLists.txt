cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME SFML_Sandbox)

project(${PROJECT_NAME})
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W1")
set(CMAKE_CXX_STANDARD 17)


find_package(Doxygen)
option(BUILD_DOCUMENTATION "Create and install documentation (requires Doxygen)" ${DOXYGEN_FOUND})

if(DOXYGEN_FOUND)

	set(DOXYGEN_IN ${CMAKE_SOURCE_DIR}/doc/Doxyfile.in)
    set(DOXYGEN_OUT ${PROJECT_BINARY_DIR}/doc/Doxyfile)
	
	configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")
	add_custom_target(
		docs ALL
		COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
		COMMENT "Generating API documentation with Doxygen"
		VERBATIM
		WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/doc
	)

endif()


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










