project(Art_robot)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)

#OS X port
include_directories(/opt/local/include)
include_directories(/opt/local/include/cairo)
include_directories(/opt/local/include/freetype2)
include_directories(/opt/local/include/librsvg-2.0)
include_directories(/opt/local/include/glib-2.0)
include_directories(/opt/local/lib/glib-2.0/include)
include_directories(/opt/local/include/gdk-pixbuf-2.0)
include_directories(/opt/local/include/json-glib-1.0)
link_directories(/opt/local/lib/)

#OS X brew
include_directories(/usr/local/opt/cairo/include/cairo)
include_directories(/usr/local/opt/freetype/include/freetype2)
include_directories(/usr/local/opt/librsvg/include/librsvg-2.0)
include_directories(/usr/local/opt/glib/include/glib-2.0)
include_directories(/usr/local/opt/glib/lib/glib-2.0/include)
include_directories(/usr/local/opt/gdk-pixbuf/include/gdk-pixbuf-2.0)
include_directories(/usr/local/opt/json-glib/include/json-glib-1.0)
link_directories(/usr/local/lib)

#Win MSYS2
include_directories(c:/opt/msys64/mingw64/include/cairo)
include_directories(c:/opt/msys64/mingw64/include/freetype2)
include_directories(c:/opt/msys64/mingw64/include/librsvg-2.0)
include_directories(c:/opt/msys64/mingw64/include/glib-2.0)
include_directories(c:/opt/msys64/mingw64/lib/glib-2.0/include)
include_directories(c:/opt/msys64/mingw64/include/gdk-pixbuf-2.0)
include_directories(c:/opt/msys64/mingw64/include/json-glib-1.0)

#Linux
include_directories(/usr/include/cairo)
include_directories(/usr/include/freetype2)
include_directories(/usr/include/librsvg-2.0)
include_directories(/usr/include/glib-2.0)
include_directories(/usr/include/gdk-pixbuf-2.0)
include_directories(/usr/include/json-glib-1.0)
include_directories(/usr/lib/x86_64-linux-gnu/glib-2.0/include)
link_directories(/usr/lib/)

add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} cairo)
target_link_libraries(${PROJECT_NAME} freetype)
target_link_libraries(${PROJECT_NAME} rsvg-2)
target_link_libraries(${PROJECT_NAME} json-glib-1.0)

#set(CMAKE_EXE_LINKER_FLAGS "-static -v")
