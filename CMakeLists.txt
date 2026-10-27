cmake_minimum_required(VERSION 3.31)
project(GTKMMCustomEntry)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MAJOR_VERSION 1)
set(CMAKE_MINOR_VERSION 0)
set(CMAKE_PATCH_VERSION 0)
set(CMAKE_TWEAK_VERSION 0)
set(PROJECT_VERSION "${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}.${CMAKE_TWEAK_VERSION}")

configure_file(README.md ${CMAKE_CURRENT_BINARY_DIR}/README.md COPYONLY)
configure_file(inc/GTKMMCustomEntry.h.in ${CMAKE_CURRENT_SOURCE_DIR}/inc/GTKMMCustomEntry.h)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM4 REQUIRED gtkmm-4.0)

add_library(GTKMMCustomEntry src/GTKMMCustomEntry.cpp)

target_include_directories(GTKMMCustomEntry PUBLIC ${GTKMM4_INCLUDE_DIRS} inc/)
target_link_libraries(GTKMMCustomEntry PUBLIC ${GTKMM4_LIBRARIES})

IF (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    message(STATUS "Release")
    target_link_options(${PROJECT_NAME} PRIVATE -Os -O3 -s)
    target_compile_options(${PROJECT_NAME} PRIVATE -Os -O3 -s)
ENDIF ()

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
        ARCHIVE DESTINATION lib)
install(DIRECTORY inc/
        DESTINATION include/GTKMMCustomEntry
        FILES_MATCHING PATTERN "*.h")

include(packaging.cmake)