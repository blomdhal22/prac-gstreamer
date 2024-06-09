# CMake version
cmake_minimum_required(VERSION 2.8.9)

# for X86 build
set(TARGET_X86_DESK_TOP ON)

# Include cmake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Declare project
project(prac_gstreamer)

# Set output directories
set(DEFAULT_CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

set(OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/out)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_PATH}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_PATH}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_PATH}/static)
set(HEADER_OUTPUT_PATH ${OUTPUT_PATH}/include)
set(INSTALL_PATH ${OUTPUT_PATH}/install/${CMAKE_BUILD_TYPE})
set(PREBUILT_BIN_PATH ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt-bin)

# Set enable output of compile commands during generation
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Includes
include_directories(include)

# Compile options
include(cmake/compileOptions.cmake)

# Build type - Release by default
set(CMAKE_BUILD_TYPE Debug)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Overrides
set(CMAKE_MACOSX_RPATH ON)

get_filename_component(PROJECT_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}" ABSOLUTE)

# Project modules
add_subdirectory(src)
# add_subdirectory(tests)

# Code coverage - Debug only
# NOTE: Code coverage results with an optimized (non-Debug) build may be misleading
option(BUILD_COVERAGE "Build code coverage" OFF)
if (CMAKE_BUILD_TYPE MATCHES Debug AND CMAKE_COMPILER_IS_GNUCXX AND BUILD_COVERAGE)
    include(CodeCoverage)
    setup_target_for_coverage(${PROJECT_NAME}_coverage UnitTests coverage)
endif()

message(STATUS "OUTPUT_PATH=${OUTPUT_PATH}")
# message(STATUS "HEADER_OUTPUT_PATH=${HEADER_OUTPUT_PATH}")
message(STATUS "CMAKE_C_COMPILER=${CMAKE_C_COMPILER}")
message(STATUS "CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
# message(STATUS "BUILD_TEST=${BUILD_TEST}")
# message(STATUS "OPX_HEADER_PATH=${OPX_HEADER_PATH}")
# message(STATUS "OPX_LIBRARY_HEADER_PATH=${OPX_LIBRARY_HEADER_PATH}")
message(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "CMAKE_LIBRARY_OUTPUT_DIRECTORY=${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
message(STATUS "CMAKE_ARCHIVE_OUTPUT_DIRECTORY=${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
# message(STATUS "INSTALL_PATH=${INSTALL_PATH}")
message(STATUS "PREBUILT_BIN_PATH=${PREBUILT_BIN_PATH}")
message(STATUS "CMAKE_SYSTEM_PROCESSOR=${CMAKE_SYSTEM_PROCESSOR}")