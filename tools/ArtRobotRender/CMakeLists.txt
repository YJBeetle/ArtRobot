set(ARTROBOT_RENDER_SOURCES
        src/main.cpp
        src/Args.cpp
        )

add_executable(ArtRobotRender ${ARTROBOT_RENDER_SOURCES})
target_compile_definitions(ArtRobotRender PRIVATE TIMER=1)
target_link_libraries(ArtRobotRender PRIVATE ArtRobotJson)

configure_file(template/demo/demo.json demo.json COPYONLY)
configure_file(template/demo/bg-veins.svg bg-veins.svg COPYONLY)
configure_file(template/ThrowIt/ThrowIt.json ThrowIt.json COPYONLY)
configure_file(template/ThrowIt/p.png p.png COPYONLY)
configure_file(template/ThrowIt/pp.jpg pp.jpg COPYONLY)
configure_file(template/SaySticker/SaySticker.json SaySticker.json COPYONLY)
configure_file(template/SaySticker/SaySticker_messageX.png SaySticker_messageX.png COPYONLY)
configure_file(template/SaySticker/SaySticker2.json SaySticker2.json COPYONLY)
configure_file(template/SaySticker/SaySticker2_messageX.png SaySticker2_messageX.png COPYONLY)

install(TARGETS ArtRobotRender RUNTIME DESTINATION bin)
