# CMakeLists.txt 전체 수정본

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
        src/core/game/Game.cpp
        src/core/game/GameSession.cpp
        src/core/game/GameStats.cpp
        src/core/game/ScoreManager.cpp
        src/core/player/Player.cpp
        src/core/player/PlayerMovement.cpp
        src/core/player/PlayerAction.cpp
        src/core/player/PlayerCollision.cpp
        src/core/player/PlayerAttachment.cpp
        src/core/building/Building.cpp
        src/core/building/BuildingManager.cpp
        src/core/building/Floor.cpp
        src/platform/console/ConsoleRenderer.cpp
        src/platform/console/ConsoleInputHandler.cpp
        src/platform/console/UIMessage.cpp
)

# SDL 사용 시 소스 추가
if(USE_SDL)
    list(APPEND SOURCES
            src/platform/sdl/SDLRenderer.cpp
            src/platform/sdl/SDLInputHandler.cpp
    )
    add_compile_definitions(USE_SDL)
endif()

add_executable(${PROJECT_NAME} ${SOURCES})

# SDL2 설정 (USE_SDL일 때만)
if(USE_SDL)
    # Include 디렉토리 - target별로 설정
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
