cmake_minimum_required(VERSION 3.18)

set(DEVKITPRO "$ENV{DEVKITPRO}")

set(CMAKE_C_COMPILER "${DEVKITPRO}/devkitARM/bin/arm-none-eabi-gcc")
set(TOOLCHAIN_FILE "${DEVKITPRO}/cmake/GBA.cmake")
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${DEVKITPRO}/cmake")
project(GBA_Game C)

set(GBA_ROOT "${DEVKITPRO}/libgba")
set(DKP_GBA_PLATFORM_LIBRARY "libgba")

find_program(GBA_GBAFIX_EXE gbafix
    PATHS "${DEVKITPRO}/tools/bin"
    REQUIRED
)
include("${DEVKITPRO}/cmake/Platform/NintendoGBA.cmake")
include(FetchContent)

include_directories("${CMAKE_SOURCE_DIR}/src")
include_directories("${CMAKE_SOURCE_DIR}/vendor")

set(CMAKE_C_FLAGS "${GBA_ARCH_SETTINGS} ${GBA_COMMON_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${GBA_LINKER_FLAGS}")

file(GLOB_RECURSE SOURCES "src/*.c")
add_executable(${PROJECT_NAME} ${SOURCES})

gba_create_rom(${PROJECT_NAME}
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.gba"
    TITLE "Meu Jogo GBA"
    GAMECODE "MEJG"
    MAKERCODE "ABCD"
    VERSION "1.0"
    DEBUG "1"
)

# Lua
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/vendor/lua/lua")
    FetchContent_Declare(
        lib_lua
        GIT_REPOSITORY https://github.com/lua/lua
        GIT_TAG v5.4.7
        SOURCE_DIR "${CMAKE_SOURCE_DIR}/vendor/lua"
        SUBBUILD_DIR "${CMAKE_BINARY_DIR}/CMakeFiles/_deps/lua-subbuild"
        BINARY_DIR "${CMAKE_BINARY_DIR}/CMakeFiles/_deps/lua-build"
    )
    FetchContent_MakeAvailable(lib_lua)
    execute_process(
        COMMAND make CC=cc lua MYCFLAGS="-std=c99" MYLIBS="" 
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/vendor/lua"
    )
endif()
file(GLOB lua_files "${CMAKE_SOURCE_DIR}/vendor/lua/*.c")
list(REMOVE_ITEM lua_files "${CMAKE_SOURCE_DIR}/vendor/lua/lua.c")
list(REMOVE_ITEM lua_files "${CMAKE_SOURCE_DIR}/vendor/lua/onelua.c")
add_library(lua-static STATIC "${lua_files}")
find_program(LUAC lua PATHS "${CMAKE_SOURCE_DIR}/vendor/lua" REQUIRED NO_DEFAULT_PATH)

# Engine
set(ENGINE_HEADER "${CMAKE_SOURCE_DIR}/vendor/engine/bytecode.h")
if(NOT EXISTS ${ENGINE_HEADER})
    FetchContent_Declare(
        lib_engine
        GIT_REPOSITORY https://github.com/gamelly/gly-engine
        GIT_TAG develop
        SOURCE_DIR "${CMAKE_SOURCE_DIR}/vendor/engine"
        SUBBUILD_DIR "${CMAKE_BINARY_DIR}/CMakeFiles/_deps/engine-subbuild"
        BINARY_DIR "${CMAKE_BINARY_DIR}/CMakeFiles/_deps/engine-build"
    )
    FetchContent_MakeAvailable(lib_engine)
    file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/vendor/engine/dist")
    execute_process(
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/vendor/engine"
        COMMAND ${LUAC} src/cli/main.lua build --core native --bundler
    )
    execute_process(
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/vendor/engine"
        COMMAND ${LUAC} src/cli/main.lua fs-xxd-i dist/main.lua bytecode.h --const --name engine_bytecode_lua
        COMMAND_ERROR_IS_FATAL
    )
endif()

# Game
string(LENGTH "${GAME}" GAME_LENGTH)
set(GAME_HEADER "${CMAKE_SOURCE_DIR}/vendor/game/bytecode.h")
if(DEFINED GAME AND GAME_LENGTH)
    get_filename_component(GAME "${GAME}" ABSOLUTE)
    file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/vendor/game")
    execute_process(
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/vendor/engine"
        COMMAND ${LUAC} src/cli/main.lua fs-xxd-i ${GAME} ${GAME_HEADER} --const --name game_bytecode_lua
        COMMAND_ERROR_IS_FATAL
    )
endif()

target_link_libraries(${PROJECT_NAME} ${GBA_STANDARD_LIBRARIES})
target_link_libraries(${PROJECT_NAME} lua-static)
target_link_libraries(${PROJECT_NAME} m)
