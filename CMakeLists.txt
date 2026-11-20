cmake_minimum_required(VERSION 3.16)
project(gmap LANGUAGES CXX)

option(GMAP_SOLUTION "Build with the local instructor solution" OFF)

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS src/gmap_viewer.html)
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/src/gmap_viewer.html" VIEWER_HTML)
configure_file(src/gmap_viewer.hpp.in gmap_viewer.hpp @ONLY)

add_executable(gmap src/gmap_helper.cpp src/gmap_main.cpp src/gmap_display.cpp)
target_compile_features(gmap PRIVATE cxx_std_11)
target_include_directories(gmap PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
if(GMAP_SOLUTION)
    target_compile_definitions(gmap PRIVATE GMAP_SOLUTION)
endif()
