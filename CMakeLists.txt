cmake_minimum_required(VERSION 3.16)
project(gmap LANGUAGES CXX)

option(GMAP_SOLUTION "Build with the instructor solution" OFF)

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS src/gmap_viewer.html)
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/src/gmap_viewer.html" VIEWER_HTML)
configure_file(src/gmap_viewer.hpp.in gmap_viewer.hpp @ONLY)

add_executable(gmap src/gmap_helper.cpp src/gmap_main.cpp src/gmap_display.cpp)
target_compile_features(gmap PRIVATE cxx_std_11)
target_include_directories(gmap PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
if(GMAP_SOLUTION)
    set(GMAP_SOLUTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../gmap_exercise-solutions/gmap_solution.cpp"
        CACHE FILEPATH "Path to the instructor solution source")
    if(NOT EXISTS "${GMAP_SOLUTION_FILE}" OR IS_DIRECTORY "${GMAP_SOLUTION_FILE}")
        message(FATAL_ERROR
            "Solution file not found: ${GMAP_SOLUTION_FILE}\n"
            "Clone gmap_exercise-solutions next to the exercise or set -DGMAP_SOLUTION_FILE=/path/to/gmap_solution.cpp.")
    endif()
    target_sources(gmap PRIVATE "${GMAP_SOLUTION_FILE}")
    target_include_directories(gmap PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
else()
    target_sources(gmap PRIVATE src/gmap.cpp)
endif()
