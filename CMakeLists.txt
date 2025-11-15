cmake_minimum_required(VERSION 3.30)
project(open_mission_building_breaker)

set(CMAKE_CXX_STANDARD 17)

add_executable(open_mission_building_breaker src/main.cpp
        src/core/game/Game.h
        src/core/game/Game.cpp
        src/ui/InputHandler.h
        src/ui/InputHandler.cpp
        src/ui/UIRenderer.h
        src/ui/UIRenderer.cpp
        src/core/game/GameSession.cpp
        src/core/game/GameSession.h
        src/core/player/Player.cpp
        src/core/player/Player.h
        src/core/game/GameConfig.h
        src/core/building/Building.cpp
        src/core/building/Building.h
        src/core/building/BuildingManager.cpp
        src/core/building/BuildingManager.h
        src/core/player/CollisionResult.h
        src/core/player/PlayerMovement.cpp
        src/core/player/PlayerMovement.h
        src/core/player/PlayerAction.cpp
        src/core/player/PlayerAction.h
        src/core/player/PlayerCollision.cpp
        src/core/player/PlayerCollision.h
        src/core/player/PlayerActionType.h
        src/ui/UIMessage.h
        src/ui/UIMessage.cpp
        src/core/building/Floor.cpp
        src/core/building/Floor.h
        src/core/game/GameOverDisplayData.h
        src/core/game/GameStats.h
        src/core/game/GameStats.cpp
)

add_compile_options(/utf-8)