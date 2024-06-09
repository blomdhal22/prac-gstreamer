cmake_minimum_required(VERSION 2.8.9)

# pkg-config
find_package(PkgConfig)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)
pkg_check_modules(GSTREAMER_APP REQUIRED gstreamer-app-1.0)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/liblogging)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/myapp-00)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/myapp-01)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/prac-01-init)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/prac-02-element)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/prac-03-bin)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/prac-04-bus)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/prac-05-pad)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/prac-06-first-app)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/prac-07-meta)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/prac-08-buffering)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/prac-09-thread1)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/prac-10-thread2)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/prac-11-autoplugging)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/prac-12-manipulation)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/prac-13-manipulation2)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/prac-14-manipulation3)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/prac-15-manipulation4)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/prac-16-manipulation5)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/prac-17-manipulation6)


add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/prac-webcam1)


add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tutorial1)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tutorial2)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tutorial3)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tutorial4)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tutorial5)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tutorial6)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tutorial7)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tutorial8)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tutorial9)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tutorial12)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tutorial13)