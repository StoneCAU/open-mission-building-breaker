cmake_minimum_required(VERSION 3.22)
project(open_mission_building_breaker)

set(CMAKE_CXX_STANDARD 17)

# SDL2 경로 설정
set(SDL2_DIR "${CMAKE_SOURCE_DIR}/libs/SDL2")
set(SDL2_IMAGE_DIR "${CMAKE_SOURCE_DIR}/libs/SDL2_image")
set(SDL2_MIXER_DIR "${CMAKE_SOURCE_DIR}/libs/SDL2_mixer")
set(SDL2_TTF_DIR "${CMAKE_SOURCE_DIR}/libs/SDL2_ttf")

# 소스 파일들
set(SOURCES
        src/main.cpp

        # Core - Game
        src/core/game/Game.cpp
        src/core/game/GameSession.cpp
        src/core/game/GameStats.cpp
        src/core/game/ScoreManager.cpp

        # Core - Player
        src/core/player/Player.cpp
        src/core/player/PlayerMovement.cpp
        src/core/player/PlayerAction.cpp
        src/core/player/PlayerCollision.cpp
        src/core/player/PlayerAttachment.cpp

        # Core - Building
        src/core/building/Building.cpp
        src/core/building/BuildingManager.cpp
        src/core/building/Floor.cpp

        # Platform - Console
        src/platform/console/ConsoleRenderer.cpp
        src/platform/console/ConsoleInputHandler.cpp
        src/core/ui/UIMessage.cpp

        # Platform - SDL (새로 추가된 파일들)
        src/platform/sdl/base/SDLRenderer.cpp
        src/platform/sdl/base/SDLInputHandler.cpp
        src/platform/sdl/assets/AssetManager.cpp
        src/platform/sdl/rendering/MenuRenderer.cpp
        src/platform/sdl/rendering/GameRenderer.cpp
        src/platform/sdl/rendering/GameOverRenderer.cpp
        src/platform/sdl/rendering/PlayerAnimationRenderer.cpp
        src/platform/sdl/rendering/PlayerAnimationRenderer.h
        src/platform/sdl/rendering/HUDRenderer.cpp
        src/platform/sdl/rendering/HUDRenderer.h
        src/platform/sdl/rendering/BuildingRenderer.cpp
        src/platform/sdl/rendering/BuildingRenderer.h
        src/platform/sdl/assets/SoundManager.cpp
        src/platform/sdl/assets/SoundManager.h
        src/platform/sdl/rendering/UltimateEffectRenderer.cpp
        src/platform/sdl/rendering/UltimateEffectRenderer.h
        src/platform/sdl/assets/AssetConfig.h
)

if(USE_SDL)
    add_compile_definitions(USE_SDL)
endif()

add_executable(${PROJECT_NAME} ${SOURCES})

# SDL2 설정 (USE_SDL일 때만)
if(USE_SDL)
    # Include 디렉토리
    target_include_directories(${PROJECT_NAME} PRIVATE
            ${SDL2_DIR}/include
            ${SDL2_DIR}/include/SDL2
            ${SDL2_IMAGE_DIR}/include
            ${SDL2_IMAGE_DIR}/include/SDL2
            ${SDL2_MIXER_DIR}/include
            ${SDL2_MIXER_DIR}/include/SDL2
            ${SDL2_TTF_DIR}/include
            ${SDL2_TTF_DIR}/include/SDL2
    )

    # 링크 디렉토리
    target_link_directories(${PROJECT_NAME} PRIVATE
            ${SDL2_DIR}/lib
            ${SDL2_IMAGE_DIR}/lib
            ${SDL2_MIXER_DIR}/lib
            ${SDL2_TTF_DIR}/lib
    )

    # MinGW용 UTF-8 설정 (수정됨)
    if(MINGW)
        target_compile_options(${PROJECT_NAME} PRIVATE -finput-charset=UTF-8 -fexec-charset=UTF-8)
    endif()

    # 라이브러리 링크
    target_link_libraries(${PROJECT_NAME}
            mingw32
            SDL2main
            SDL2
            SDL2_image
            SDL2_mixer
            SDL2_ttf
    )

    # DLL 복사
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL2_DIR}/bin/SDL2.dll"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL2_IMAGE_DIR}/bin/SDL2_image.dll"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL2_MIXER_DIR}/bin/SDL2_mixer.dll"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL2_TTF_DIR}/bin/SDL2_ttf.dll"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()

# SDL2 링크 (USE_SDL 옵션과 관계없이 항상 링크)
if(USE_SDL)
    target_link_libraries(${PROJECT_NAME}
            mingw32
            SDL2main
            SDL2
            SDL2_image
            SDL2_mixer
            SDL2_ttf
    )

    # DLL 복사 (실행 시 필요)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL2_DIR}/bin/SDL2.dll"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL2_IMAGE_DIR}/bin/SDL2_image.dll"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL2_MIXER_DIR}/bin/SDL2_mixer.dll"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL2_TTF_DIR}/bin/SDL2_ttf.dll"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>

            # assets 폴더 전체 복사 (추가됨)
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/assets"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    )
endif()
