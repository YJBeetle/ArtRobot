cmake_minimum_required(VERSION 3.13)

set (CMAKE_CXX_STANDARD 11)

# set(CMAKE_EXE_LINKER_FLAGS "-static -v")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTIMER=1")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")

project(ArtRobot)

if(CMAKE_SYSTEM_NAME MATCHES "Emscripten")
    set(WASM TRUE)
    add_compile_definitions("WASM")
endif()

# libs
include(FindPkgConfig)

## pixman
pkg_check_modules(PIXMAN pixman-1)
if (NOT PIXMAN_FOUND)
    message(FATAL_ERROR "pixman not found!")
endif ()
message(STATUS "pixman library status:")
message(STATUS "    version: ${PIXMAN_VERSION}")
message(STATUS "    include path: ${PIXMAN_INCLUDE_DIRS}")
message(STATUS "    lib path: ${PIXMAN_LIBDIR}")
message(STATUS "    libraries: ${PIXMAN_LIBRARIES}")

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

## pango
pkg_check_modules(PANGO pangocairo)
if (NOT PANGO_FOUND)
    message(FATAL_ERROR "Pango not found!")
endif ()
message(STATUS "Pango library status:")
message(STATUS "    version: ${PANGO_VERSION}")
message(STATUS "    include path: ${PANGO_INCLUDE_DIRS}")
message(STATUS "    lib path: ${PANGO_LIBDIR}")
message(STATUS "    libs path: ${PANGO_LIBRARY_DIRS}")
message(STATUS "    libraries: ${PANGO_LIBRARIES}")

## rsvg
pkg_check_modules(RSVG librsvg-2.0)
if (NOT RSVG_FOUND)
    message(FATAL_ERROR "rsvg not found!")
endif ()
message(STATUS "rsvg library status:")
message(STATUS "    version: ${RSVG_VERSION}")
message(STATUS "    include path: ${RSVG_INCLUDE_DIRS}")
message(STATUS "    lib path: ${RSVG_LIBDIR}")
message(STATUS "    libs path: ${RSVG_LIBRARY_DIRS}")
message(STATUS "    libraries: ${RSVG_LIBRARIES}")

## Jpeg
find_package(JPEG REQUIRED)
message(STATUS "Jpeg library status:")
message(STATUS "    version: ${JPEG_VERSION}")
message(STATUS "    include path: ${JPEG_INCLUDE_DIR}")
message(STATUS "    libraries: ${JPEG_LIBRARIES}")

# Find OpenCV
find_package(OpenCV REQUIRED)
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "    libraries: ${OpenCV_LIBS}")

## libwebp
pkg_check_modules(WEBP libwebp)
if (NOT WEBP_FOUND)
    message(FATAL_ERROR "libwebp not found!")
endif ()
message(STATUS "libwebp library status:")
message(STATUS "    version: ${WEBP_VERSION}")
message(STATUS "    include path: ${WEBP_INCLUDE_DIRS}")
message(STATUS "    lib path: ${WEBP_LIBDIR}")
message(STATUS "    libraries: ${WEBP_LIBRARIES}")

# Project
file(GLOB_RECURSE SRC_LIST
    ./src/*.cpp
    ./src/*.hpp
)
if(WASM)
    add_library(${PROJECT_NAME} STATIC ${SRC_LIST})
else()
    add_library(${PROJECT_NAME} SHARED ${SRC_LIST})
endif()
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PUBLIC ${PIXMAN_INCLUDE_DIRS})
target_link_directories(${PROJECT_NAME} PUBLIC ${PIXMAN_LIBDIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${PIXMAN_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CAIRO_INCLUDE_DIRS})
target_link_directories(${PROJECT_NAME} PUBLIC ${CAIRO_LIBDIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${CAIRO_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${PANGO_INCLUDE_DIRS})
target_link_directories(${PROJECT_NAME} PUBLIC ${PANGO_LIBDIR})
target_link_directories(${PROJECT_NAME} PUBLIC ${PANGO_LIBRARY_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${PANGO_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${RSVG_INCLUDE_DIRS})
target_link_directories(${PROJECT_NAME} PUBLIC ${RSVG_LIBDIR})
target_link_directories(${PROJECT_NAME} PUBLIC ${RSVG_LIBRARY_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${RSVG_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${JPEG_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${JPEG_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${OpenCV_LIBS})
target_include_directories(${PROJECT_NAME} PUBLIC ${WEBP_INCLUDE_DIRS})
target_link_directories(${PROJECT_NAME} PUBLIC ${WEBP_LIBDIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${WEBP_LIBRARIES})

# Headers file
file(GLOB_RECURSE HEADERS
    ./src/*.hpp
)
foreach(HEADER ${HEADERS})
    file(RELATIVE_PATH HEADER ${CMAKE_CURRENT_SOURCE_DIR}/src ${HEADER})
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/${HEADER}" "${CMAKE_CURRENT_BINARY_DIR}/include/ArtRobot/${HEADER}" COPYONLY)
endforeach()
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/include/")

# install
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/include/" DESTINATION include)

# test
add_executable(TestImage test/TestImage.cpp)
target_link_libraries(TestImage PUBLIC ${PROJECT_NAME})
configure_file(test/img.jpg img.jpg COPYONLY)
configure_file(test/mask.png mask.png COPYONLY)
