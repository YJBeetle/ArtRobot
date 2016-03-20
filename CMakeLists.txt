project(Art_robot)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)

#OS X
include_directories(/opt/local/include/)
include_directories(/opt/local/include/cairo/)
link_directories(/opt/local/lib/)

#Win
#set(CMAKE_LEGACY_CYGWIN_WIN32 0) # Remove when CMake >= 2.8.4 is required
#include_directories(c:/opt/cygwin/root/usr/include/cairo)
#include_directories(/usr/include/cairo)
#include_directories(librsvg-2.0)
include_directories(c:/opt/msys64/mingw32/include/cairo)
include_directories(c:/opt/msys64/mingw32/include/librsvg-2.0)
include_directories(c:/opt/msys64/mingw32/include/glib-2.0)
include_directories(c:/opt/msys64/mingw32/lib/glib-2.0/include)
include_directories(c:/opt/msys64/mingw32/include/gdk-pixbuf-2.0)
#link_directories(c:/opt/msys64/mingw32/lib/)

add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(Art_robot cairo)
target_link_libraries(Art_robot rsvg-2)

