cmake_minimum_required(VERSION 3.4)

set(CMAKE_CXX_STANDARD 17)

# set(CMAKE_EXE_LINKER_FLAGS "-static -v")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTIMER=1")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")

project(ArtRobotRender)

add_subdirectory("ArtRobot")

include(FetchContent)
FetchContent_Declare(json
        # GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
        GIT_TAG v3.11.2)
FetchContent_GetProperties(json)
if (NOT json_POPULATED)
    FetchContent_Populate(json)
    add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif ()

# Project
set(SRC_LIST
        src/main.cpp
        src/Args.cpp
        )
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} PRIVATE ArtRobot)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

configure_file(template/demo/demo.json demo.json COPYONLY)
configure_file(template/demo/bg-veins.svg bg-veins.svg COPYONLY)
configure_file(template/ThrowIt/ThrowIt.json ThrowIt.json COPYONLY)
configure_file(template/ThrowIt/p.png p.png COPYONLY)
configure_file(template/ThrowIt/pp.jpg pp.jpg COPYONLY)
configure_file(template/SaySticker/SaySticker.json SaySticker.json COPYONLY)
configure_file(template/SaySticker/SaySticker_userPhotoMask.png SaySticker_userPhotoMask.png COPYONLY)
configure_file(template/SaySticker/SaySticker_messageX.png SaySticker_messageX.png COPYONLY)
configure_file(template/SaySticker/SaySticker2.json SaySticker2.json COPYONLY)
configure_file(template/SaySticker/SaySticker2_userPhotoMask.png SaySticker2_userPhotoMask.png COPYONLY)
configure_file(template/SaySticker/SaySticker2_messageX.png SaySticker2_messageX.png COPYONLY)
