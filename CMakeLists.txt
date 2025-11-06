cmake_minimum_required(VERSION 3.30)
project(open_mission_building_breaker)

set(CMAKE_CXX_STANDARD 17)

add_executable(open_mission_building_breaker src/main.cpp
        src/game/Game.h
        src/game/Game.cpp
        src/ui/InputHandler.h
        src/ui/InputHandler.cpp
        src/ui/UIRenderer.h
        src/ui/UIRenderer.cpp)

add_compile_options(/utf-8)