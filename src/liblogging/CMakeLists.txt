# CMake version
cmake_minimum_required(VERSION 3.6.3)

# Target name
set(target logging)

# Define
set(root_dir ${PROJECT_ROOT_DIR})

# Includes
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${root_dir}/include
)

# Sources
file(GLOB_RECURSE public_headers
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

file(GLOB_RECURSE sources
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
    )

# Build library
add_library(${target} SHARED ${sources})

# Project options
set_target_properties(${target}
    PROPERTIES
    ${DEFAULT_PROJECT_OPTIONS}
    #PUBLIC_HEADER "${public_headers}"
    C_STANDARD 11
    CXX_STANDARD 11
)

# Compile options
target_compile_options(${target}
    PRIVATE

    PUBLIC
    ${DEFAULT_COMPILE_OPTIONS}

    INTERFACE
)

target_link_libraries(${target}
    PRIVATE

    PUBLIC
    ${DEFAULT_LINKER_OPTIONS}
    ${DEFAULT_LIBRARIES}

    INTERFACE
)

# Install
install(
    TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_PATH}/bin
    LIBRARY DESTINATION ${INSTALL_PATH}/lib
    ARCHIVE DESTINATION ${INSTALL_PATH}/lib/static
    )

# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
