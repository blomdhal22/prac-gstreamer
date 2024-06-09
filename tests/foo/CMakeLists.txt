# Target name
set(target test-foo)

# Define
set(root_dir ${CMAKE_CURRENT_SOURCE_DIR}/../../)

if (CMAKE_SYSTEM_PROCESSOR MATCHES "^x86" OR true)
    link_directories(${root_dir}/libs/android)
endif()

# Includes
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${root_dir}/include
    ${root_dir}/libs/include
)

# Sources
file(GLOB sources
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

# Build executable
add_executable(${target} ${sources})

# Project options
set_target_properties(${target}
    PROPERTIES
    ${DEFAULT_PROJECT_OPTIONS}
    C_STANDARD   11
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

    pthread
    aiddca
    log

    INTERFACE
)

# Install
install(TARGETS ${target} DESTINATION bin)