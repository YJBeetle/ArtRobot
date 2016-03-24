project(Art_robot)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)

#OS X
include_directories(/opt/local/include/)
include_directories(/opt/local/include/cairo/)
link_directories(/opt/local/lib/)

#Win MSYS2
include_directories(c:/opt/msys64/mingw64/include/cairo)
include_directories(c:/opt/msys64/mingw64/include/librsvg-2.0)
include_directories(c:/opt/msys64/mingw64/include/glib-2.0)
include_directories(c:/opt/msys64/mingw64/lib/glib-2.0/include)
include_directories(c:/opt/msys64/mingw64/include/gdk-pixbuf-2.0)
include_directories(c:/opt/msys64/mingw64/include/json-glib-1.0)

add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(Art_robot cairo)
target_link_libraries(Art_robot rsvg-2)
target_link_libraries(Art_robot json-glib-1.0)
