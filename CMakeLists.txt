project(Art_robot)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)

include_directories(/opt/local/include/cairo/)
link_directories(/opt/local/lib/)

add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(Art_robot cairo)
