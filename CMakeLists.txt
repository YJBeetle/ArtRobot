cmake_minimum_required(VERSION 3.4)

set (CMAKE_CXX_STANDARD 11)

# set(CMAKE_EXE_LINKER_FLAGS "-static -v")
# set(CMAKE_CXX_FLAGS "-g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTIMER=1")

project(Art_robot)

# sources
aux_source_directory(./src/ SRC_LIST)

# libs
include(FindPkgConfig)

## cairo
pkg_check_modules(CAIRO cairo)
if (NOT CAIRO_FOUND)
message(FATAL_ERROR "Cairo not found!")
endif ()
message(STATUS "Cairo library status:")
message(STATUS "    version: ${CAIRO_VERSION}")
message(STATUS "    include path: ${CAIRO_INCLUDE_DIRS}")
message(STATUS "    lib path: ${CAIRO_LIBDIR}")
message(STATUS "    libraries: ${CAIRO_LIBRARIES}")
include_directories(${CAIRO_INCLUDE_DIRS})
link_directories(${CAIRO_LIBDIR})

## Freetype
find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})

## rsvg
pkg_check_modules(RSVG librsvg-2.0)
if (NOT RSVG_FOUND)
message(FATAL_ERROR "rsvg not found!")
endif ()
message(STATUS "rsvg library status:")
message(STATUS "    version: ${RSVG_VERSION}")
message(STATUS "    include path: ${RSVG_INCLUDE_DIRS}")
message(STATUS "    lib path: ${RSVG_LIBDIR}")
message(STATUS "    libraries: ${RSVG_LIBRARIES}")
include_directories(${RSVG_INCLUDE_DIRS})
link_directories(${RSVG_LIBDIR})

set(LIB_LIST
    ${CAIRO_LIBRARIES}
    ${FREETYPE_LIBRARIES}
    # ${RSVG_LIBRARIES}
    rsvg-2
)

# building project
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${LIB_LIST})

configure_file(res/fonts/Lantinghei.ttc Lantinghei.ttc COPYONLY)
configure_file(template/demo/data.json data.json COPYONLY)
configure_file(template/demo/bg-veins.svg bg-veins.svg COPYONLY)
