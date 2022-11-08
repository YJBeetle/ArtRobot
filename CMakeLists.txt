cmake_minimum_required(VERSION 3.13)

set (CMAKE_CXX_STANDARD 17)

option(UsePango "Use Pango" ON)
option(UseRsvg "Use Rsvg" ON)
option(UseJpeg "Use Jpeg" ON)
option(UseOpenCV "Use OpenCV" ON)
option(UseWebP "Use WebP" ON)

include(cmake/FindLibrary.cmake)

# Project
project(ArtRobot)

file(GLOB_RECURSE SRC_LIST
    ./src/*.cpp
    ./src/*.hpp
)

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
target_link_libraries(${PROJECT_NAME} PUBLIC ${WEBP_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${JPEG_INCLUDE_DIR})
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

# Test
add_executable(Test test/Test.cpp)
target_link_libraries(Test PUBLIC ${PROJECT_NAME})

# TestImage
add_executable(TestImage test/TestImage.cpp)
target_link_libraries(TestImage PUBLIC ${PROJECT_NAME})
configure_file(test/img.jpg img.jpg COPYONLY)
configure_file(test/img-CMYK.jpg img-CMYK.jpg COPYONLY)
configure_file(test/img-Gray.jpg img-Gray.jpg COPYONLY)
configure_file(test/img.png img.png COPYONLY)
configure_file(test/mask.png mask.png COPYONLY)
