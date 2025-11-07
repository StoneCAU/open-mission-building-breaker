cmake_minimum_required(VERSION 3.30)
project(open_mission_building_breaker)

set(CMAKE_CXX_STANDARD 17)

add_executable(open_mission_building_breaker src/main.cpp
        src/game/Game.h
        src/game/Game.cpp
        src/ui/InputHandler.h
        src/ui/InputHandler.cpp
        src/ui/UIRenderer.h
        src/ui/UIRenderer.cpp
        src/game/GameSession.cpp
        src/game/GameSession.h
        src/game/Player.cpp
        src/game/Player.h
        src/game/GameConfig.h
        src/game/Building.cpp
        src/game/Building.h
        src/game/BuildingManager.cpp
        src/game/BuildingManager.h)

add_compile_options(/utf-8)