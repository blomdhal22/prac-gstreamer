#
# Platform and architecture setup
#

# Set warnings as errors flag
option(WARNINGS_AS_ERRORS "Treat all warnings as errors" OFF)
if(WARNINGS_AS_ERRORS)
  set(WARN_AS_ERROR_FLAGS "-Werror")
endif()

# Get upper case system name
string(TOUPPER ${CMAKE_SYSTEM_NAME} SYSTEM_NAME_UPPER)

# Determine architecture (32/64 bit)
set(X64 OFF)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(X64 ON)
endif()

set(DEFAULT_PROJECT_OPTIONS
    C_STANDARD 11
    CXX_STANDARD 11
)

#
# Include directories
#
set(DEFAULT_INCLUDE_DIRECTORIES)

#
# Libraries
#
set(DEFAULT_LIBRARIES)

#
# Compile definitions
#
set(DEFAULT_COMPILE_DEFINITIONS
	SYSTEM_${SYSTEM_NAME_UPPER}
)

#
# Compile options
#

set(DEFAULT_COMPILE_OPTIONS)

# GCC and Clang compiler options
if (CMAKE_C_COMPILER_ID MATCHES "GNU" OR CMAKE_C_COMPILER_ID MATCHES "Clang")
	set(DEFAULT_COMPILE_OPTIONS ${DEFAULT_COMPILE_OPTIONS}
        -g
        -Wall
        -Wno-missing-braces
        -Wno-unused-function
        ${WARN_AS_ERROR_FLAGS}
    )
    set (CMAKE_C_FLAGS "-std=gnu11 ${CMAKE_C_FLAGS}")
    add_definitions(-DHAVE_PTHREADS)
endif ()

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(DEFAULT_COMPILE_OPTIONS ${DEFAULT_COMPILE_OPTIONS}
        -g
        -Wall
        -Wno-missing-braces
        -Wno-unused-function
        ${WARN_AS_ERROR_FLAGS}
    )

    set (CMAKE_CXX_FLAGS "-std=gnu++11 ${CMAKE_CXX_FLAGS}")
    add_definitions(-DHAVE_PTHREADS)
endif ()

# Prevent "no matching function for call to 'operator delete'" error
# https://github.com/pybind/pybind11/issues/1604
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(DEFAULT_COMPILE_OPTIONS ${DEFAULT_COMPILE_OPTIONS}
        -fsized-deallocation
    )
endif ()

#
# Linker options
#

set(DEFAULT_LINKER_OPTIONS)

# Use pthreads on mingw and linux
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(DEFAULT_LINKER_OPTIONS
        -pthread
    )
endif()

# Code coverage - Debug only
# NOTE: Code coverage results with an optimized (non-Debug) build may be misleading
if (CMAKE_BUILD_TYPE MATCHES Debug AND (CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
    set(DEFAULT_COMPILE_OPTIONS ${DEFAULT_COMPILE_OPTIONS}
        -g
        -O0
        -fprofile-arcs
        -ftest-coverage
    )

    set(DEFAULT_LINKER_OPTIONS ${DEFAULT_LINKER_OPTIONS}
        -fprofile-arcs
        -ftest-coverage
    )
endif()

add_definitions(-DHAVE_SYS_UIO_H)