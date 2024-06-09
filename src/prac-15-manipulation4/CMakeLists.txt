# Target name
set(target prac-15-manipulation4)
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

    /usr/include/gtk-3.0
    /usr/include/at-spi2-atk/2.0
    /usr/include/at-spi-2.0
    /usr/include/dbus-1.0
    /usr/lib/x86_64-linux-gnu/dbus-1.0/include
    /usr/include/gtk-3.0
    /usr/include/gio-unix-2.0
    /usr/include/cairo
    /usr/include/pango-1.0
    /usr/include/harfbuzz
    /usr/include/pango-1.0
    /usr/include/fribidi
    /usr/include/harfbuzz
    /usr/include/atk-1.0
    /usr/include/cairo
    /usr/include/pixman-1
    /usr/include/uuid
    /usr/include/freetype2
    /usr/include/gdk-pixbuf-2.0
    /usr/include/libpng16
    /usr/include/x86_64-linux-gnu
    /usr/include/libmount
    /usr/include/blkid
    /usr/include/glib-2.0
    /usr/lib/x86_64-linux-gnu/glib-2.0/include
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

    gtk-3
    gdk-3
    pangocairo-1.0
    pango-1.0
    harfbuzz
    atk-1.0
    cairo-gobject 
    cairo 
    gdk_pixbuf-2.0 
    gio-2.0 
    gobject-2.0 
    glib-2.0

    INTERFACE
)

# Install
install(TARGETS ${target} DESTINATION bin)