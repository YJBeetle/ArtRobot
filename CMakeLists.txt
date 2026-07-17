cmake_minimum_required(VERSION 3.13)

project(ArtRobot LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

option(UsePango "Use Pango" ON)
option(UseRsvg "Use Rsvg" ON)
option(UseJpeg "Use Jpeg" ON)
option(UseOpenCV "Use OpenCV" ON)
option(UseWebP "Use WebP" ON)
option(UseJsonTemplate "Build the optional ArtRobotJson template parser" OFF)
option(BuildTest "Build Test" ON)

if (CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    if (NOT DEFINED ENV{EMSDK})
        message(FATAL_ERROR "EMSDK must be set when building ArtRobot for Emscripten")
    endif ()
    set(ARTROBOT_EMSCRIPTEN_SYSROOT
            "$ENV{EMSDK}/upstream/emscripten/cache/sysroot")
    set(ENV{PKG_CONFIG_LIBDIR}
            "${ARTROBOT_EMSCRIPTEN_SYSROOT}/lib/pkgconfig:${ARTROBOT_EMSCRIPTEN_SYSROOT}/share/pkgconfig")
    set(ENV{PKG_CONFIG_SYSROOT_DIR} "${ARTROBOT_EMSCRIPTEN_SYSROOT}")
endif ()

include(cmake/FindLibrary.cmake)

file(GLOB_RECURSE SRC_LIST
        ./src/*.cpp
        ./src/*.hpp
        )
list(FILTER SRC_LIST EXCLUDE REGEX "/src/Json/")

add_library(${PROJECT_NAME} STATIC ${SRC_LIST})

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
target_include_directories(${PROJECT_NAME} PUBLIC ${WEBP_INCLUDE_DIRS})
target_link_directories(${PROJECT_NAME} PUBLIC ${WEBP_LIBDIR})
if (WASM)
    target_link_libraries(${PROJECT_NAME} PUBLIC ${WEBP_STATIC_LIBRARIES})
else ()
    target_link_libraries(${PROJECT_NAME} PUBLIC ${WEBP_LIBRARIES})
endif ()
target_include_directories(${PROJECT_NAME} PUBLIC ${JPEG_INCLUDE_DIRS})
target_link_directories(${PROJECT_NAME} PUBLIC ${JPEG_LIBDIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${JPEG_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${OpenCV_LIBS})

# Features
configure_file("src/Features.hpp.in" "${CMAKE_CURRENT_BINARY_DIR}/include/ArtRobot/Features.hpp" @ONLY)

# Headers file
file(GLOB_RECURSE HEADERS
        ./src/*.hpp
        )
foreach (HEADER ${HEADERS})
    file(RELATIVE_PATH HEADER ${CMAKE_CURRENT_SOURCE_DIR}/src ${HEADER})
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/${HEADER}" "${CMAKE_CURRENT_BINARY_DIR}/include/ArtRobot/${HEADER}" COPYONLY)
endforeach ()
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/include/")

if (UseJsonTemplate)
    if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/json/CMakeLists.txt")
        message(FATAL_ERROR
                "ArtRobotJson requires 3rdParty/json; run: git submodule update --init --recursive")
    endif ()

    add_subdirectory(3rdParty/json EXCLUDE_FROM_ALL)
    add_library(ArtRobotJson STATIC src/Json/Template.cpp)
    add_library(ArtRobot::Json ALIAS ArtRobotJson)
    target_link_libraries(ArtRobotJson PUBLIC ${PROJECT_NAME})
    target_link_libraries(ArtRobotJson PRIVATE nlohmann_json::nlohmann_json)
    target_include_directories(ArtRobotJson PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/include/")
endif ()

# install
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        )
if (UseJsonTemplate)
    install(TARGETS ArtRobotJson
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib
            )
endif ()
install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/include/" DESTINATION include)

if (BuildTest)
    enable_testing()

    # TestComponent
    add_executable(TestComponent test/TestComponent.cpp)
    target_link_libraries(TestComponent PUBLIC ${PROJECT_NAME})
    add_test(NAME Component COMMAND TestComponent)

    # TestImage
    add_executable(TestImage test/TestImage.cpp)
    target_link_libraries(TestImage PUBLIC ${PROJECT_NAME})
    add_test(NAME Image COMMAND TestImage)

    # TestRenderer
    add_executable(TestRenderer test/TestRenderer.cpp)
    target_link_libraries(TestRenderer PUBLIC ${PROJECT_NAME})
    add_test(NAME Renderer COMMAND TestRenderer)
    configure_file(test/img.jpg img.jpg COPYONLY)
    configure_file(test/img-CMYK.jpg img-CMYK.jpg COPYONLY)
    configure_file(test/img-Gray.jpg img-Gray.jpg COPYONLY)
    configure_file(test/img.png img.png COPYONLY)
    configure_file(test/mask.png mask.png COPYONLY)

    if (UseJsonTemplate)
        add_executable(TestJsonTemplate test/TestJsonTemplate.cpp)
        target_link_libraries(TestJsonTemplate PUBLIC ArtRobotJson)
        add_test(NAME JsonTemplate COMMAND TestJsonTemplate)
    endif ()
endif ()
