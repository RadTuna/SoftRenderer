cmake_minimum_required(VERSION 3.1)

project(SoftRendererPlayer)

################################################################################
# Source files
################################################################################
file(GLOB COMMON_SOURCE_FILES
	${PROJECT_SOURCE_DIR}/*.cpp
	${PROJECT_SOURCE_DIR}/*.h
	${PROJECT_SOURCE_DIR}/WindowsPrivate/*.cpp
	${PROJECT_SOURCE_DIR}/WindowsPrivate/*.h
	)
		
file(GLOB WINDOWS_RESOURCE_FILES
	${PROJECT_SOURCE_DIR}/WindowsPrivate/Res/*.h
	${PROJECT_SOURCE_DIR}/WindowsPrivate/Res/*.rc
	${PROJECT_SOURCE_DIR}/WindowsPrivate/Res/*.ico
	)

set_source_files_properties(${COMMON_SOURCE_FILES} PROPERTIES HEADER_FILE_ONLY FALSE)
set_source_files_properties(${WINDOWS_RESOURCE_FILES} PROPERTIES HEADER_FILE_ONLY FALSE)

set(ALL_FILES
    ${COMMON_SOURCE_FILES}
    ${WINDOWS_RESOURCE_FILES})

foreach(FILE ${ALL_FILES})
    # make rel path
	file(RELATIVE_PATH REL_PATH "${PROJECT_SOURCE_DIR}" "${FILE}")

    # get the base path
    get_filename_component(BASE_PATH "${REL_PATH}" PATH)

    # changes /'s to \\'s
    string(REPLACE "/" "\\" GROUP "${BASE_PATH}")

    source_group("${GROUP}" FILES "${FILE}")
endforeach()


################################################################################
# Include directory
################################################################################	
include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/WindowsPrivate
    ${RUNTIME_MODULE_DIR}/Math/Public
    ${RUNTIME_MODULE_DIR}/Engine/Public
    ${RUNTIME_MODULE_DIR}/Renderer/Public
    ${RUNTIME_MODULE_DIR}/Renderer/WindowsPublic
	)


################################################################################
# Precompiled Headers
################################################################################
set(ARG_PCH_HEADER Precompiled.h)
set(ARG_PCH_SOURCE Precompiled.cpp)
foreach (FILE ${ALL_FILES})
  file(RELATIVE_PATH REL_PATH "${PROJECT_SOURCE_DIR}" "${FILE}")
  if (REL_PATH MATCHES ".*\\.(c|cpp)$")
	if (REL_PATH STREQUAL ${ARG_PCH_SOURCE})
	  set_source_files_properties(${FILE} PROPERTIES COMPILE_FLAGS "/Yc\"${ARG_PCH_HEADER}\"")
	else()
	  set_source_files_properties(${FILE} PROPERTIES COMPILE_FLAGS "/Yu\"${ARG_PCH_HEADER}\"")
	endif()
  endif()
endforeach(FILE)

################################################################################
# Excutable Output Setting
################################################################################
add_executable(${PROJECT_NAME} WIN32 ${ALL_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES
	PREFIX ""
	OUTPUT_NAME ${PROJECT_NAME}
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Output/Windows/Binary
	VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Resource
)
	
################################################################################
# Import Library
################################################################################
target_link_libraries(${PROJECT_NAME} MathModule RendererModule EngineModule)


