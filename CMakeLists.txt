cmake_minimum_required(VERSION 3.8)
project(Five)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
pkg_check_modules(SDL2_MIXER REQUIRED SDL2_mixer)
pkg_check_modules(SDL2_NET REQUIRED SDL2_net)

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
target_include_directories(Five PUBLIC
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
        ${SDL2_MIXER_INCLUDE_DIRS}
        ${SDL2_NET_INCLUDE_DIRS})
target_compile_options(Five PUBLIC
        ${SDL2_CFLAGS}
        ${SDL2_IMAGE_CFLAGS}
        ${SDL2_TTF_CFLAGS}
        ${SDL2_MIXER_CFLAGS}
        ${SDL2_NET_CFLAGS})
target_link_libraries(Five
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
        ${SDL2_MIXER_LIBRARIES}
        ${SDL2_NET_LIBRARIES})

add_executable(Five_AI
        Five_AI/Five_AI.h
        Five_AI/main.cpp)
target_include_directories(Five_AI PUBLIC
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
        ${SDL2_NET_INCLUDE_DIRS})
target_compile_options(Five_AI PUBLIC
        ${SDL2_CFLAGS}
        ${SDL2_TTF_CFLAGS}
        ${SDL2_NET_CFLAGS})
target_link_libraries(Five_AI
        ${SDL2_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
        ${SDL2_NET_LIBRARIES})
