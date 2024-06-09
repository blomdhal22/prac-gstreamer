# Target name
set(target myapp-00)
set(CMAKE_CXX_STANDARD 11)

# Define
set(root_dir ${PROJECT_ROOT_DIR})

# Includes
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${root_dir}/include
    ${root_dir}/libs/include
    ${GSTREAMER_INCLUDE_DIRS}
    ${GSTREAMER_APP_INCLUDE_DIRS}
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

target_link_directories(${target}
    PRIVATE

    PUBLIC
    ${GSTREAMER_LIBRARY_DIRS}
    ${GSTREAMER_APP_LIBRARY_DIRS}

    INTERFACE
)

target_link_libraries(${target}
    PRIVATE

    PUBLIC
    ${DEFAULT_LINKER_OPTIONS}
    ${DEFAULT_LIBRARIES}

    pthread
    logging

    gstreamer-1.0
    gobject-2.0 
    glib-2.0

    gstpbutils-1.0 
    gstvideo-1.0
    
    gstaudio-1.0
    gstbase-1.0


    ${GSTREAMER_LIBRARIES}
    ${GSTREAMER_APP_LIBRARIES}

    INTERFACE
)

# Install
install(TARGETS ${target} DESTINATION bin)
