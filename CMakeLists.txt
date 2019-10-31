##########################################################
## CMake Setting for SoftRenderer 
##########################################################

cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CMakeDependentOption)
set(MAIN_PROJECT_NAME SoftRenderer)

project(${MAIN_PROJECT_NAME})

set(RUNTIME_MODULE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source/Runtime)

if(NOT WIN32)
	message(FATAL_ERROR "This project only supports windows environment currently.")
endif()

if(NOT MSVC)
	message(FATAL_ERROR "This project only supports MSVC project currently.")
endif()

################################################################################
# Sub projects
################################################################################

add_subdirectory(Source/Runtime/Math)
add_subdirectory(Source/Runtime/Renderer)
add_subdirectory(Source/Runtime/Engine)
add_subdirectory(Source/Player)

set(CMAKE_SUPPRESS_REGENERATION true)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT SoftRendererPlayer)

