cmake_minimum_required(VERSION 3.8)
project(Five)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

find_package(SDL2)

include_directories(${SDL2_INCLUDE_DIRS})

add_executable(Five
        Five/action.h
        Five/chess.h
        Five/chess.cpp
        Five/Game.h
        Five/Game.cpp
        Five/handleDraw.h
        Five/handleEvent.h
        Five/main.cpp
        Five/res.h
        Five/timer.h)

add_executable(Five_AI
        Five_AI/Five_AI.h
        Five_AI/main.cpp)

target_link_libraries(Five SDL2 SDL2_image SDL2_ttf SDL2_mixer SDL2_net)
target_link_libraries(Five_AI SDL2 SDL2_ttf SDL2_net)
