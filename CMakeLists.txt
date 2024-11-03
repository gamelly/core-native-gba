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
    TITLE "Gly Engine"
    GAMECODE "GLYE"
    MAKERCODE "GLYE"
    VERSION "1.0"
    DEBUG "0"
)

# Zig
set(ZIG_VERSION "0.13.0")
set(ZIG_DOWNLOAD "http://ziglang.org/download")
set(ZIG_PATH "${CMAKE_SOURCE_DIR}/vendor/zig/zig")
string(TOLOWER ${CMAKE_HOST_SYSTEM_NAME} CMAKE_HOST_SYSTEM_NAME_LOWER)
if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
    set(ZIG_DOWNLOAD "${ZIG_DOWNLOAD}/${ZIG_VERSION}/zig-macos-${CMAKE_HOST_SYSTEM_PROCESSOR}-${ZIG_VERSION}.tar.xz")
elseif(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
    set(ZIG_DOWNLOAD "${ZIG_DOWNLOAD}/${ZIG_VERSION}/zig-linux-${CMAKE_HOST_SYSTEM_PROCESSOR}-${ZIG_VERSION}.tar.xz")
elseif(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    set(ZIG_DOWNLOAD "${ZIG_DOWNLOAD}/${ZIG_VERSION}/zig-windows-${CMAKE_HOST_SYSTEM_PROCESSOR}-${ZIG_VERSION}.zip")
endif()
if(NOT EXISTS ${ZIG_PATH})
    FetchContent_Populate(
        compiler_zig
        URL "${ZIG_DOWNLOAD}"
        SOURCE_DIR "${CMAKE_SOURCE_DIR}/vendor/zig"
        SUBBUILD_DIR "${CMAKE_SOURCE_DIR}/vendor/_deps/zig-subbuild"
        BINARY_DIR "${CMAKE_SOURCE_DIR}/vendor/_deps/zig-build"
    )
endif()
find_program(ZIGC zig PATHS "${CMAKE_SOURCE_DIR}/vendor/zig" REQUIRED NO_DEFAULT_PATH)

# Font
set(FONT_DOWNLOAD "https://github.com/gamelly/gly-type/archive/refs/heads/main.zip")
if(NOT EXISTS "${LUA_PATH}/font")
    FetchContent_Populate(
        lib_font
        URL "${FONT_DOWNLOAD}"
        SOURCE_DIR "${CMAKE_SOURCE_DIR}/vendor/font"
        SUBBUILD_DIR "${CMAKE_SOURCE_DIR}/vendor/_deps/font-subbuild"
        BINARY_DIR "${CMAKE_SOURCE_DIR}/vendor/_deps/font-build"
    )
endif()

# Lua
set(LUA_VERSION "5.4.7")
set(LUA_DOWNLOAD "https://github.com/lua/lua/archive/refs/tags/v${LUA_VERSION}.tar.gz")
set(LUA_PATH "${CMAKE_SOURCE_DIR}/vendor/lua")
set(LUA_OPTMIZE FALSE)
if(NOT EXISTS "${LUA_PATH}/lua")
    set(LUA_OPTMIZE TRUE)
    FetchContent_Populate(
        compiler_lua
        URL "${LUA_DOWNLOAD}"
        SOURCE_DIR "${CMAKE_SOURCE_DIR}/vendor/lua"
        SUBBUILD_DIR "${CMAKE_SOURCE_DIR}/vendor/_deps/lua-subbuild"
        BINARY_DIR "${CMAKE_SOURCE_DIR}/vendor/_deps/lua-build"
    )
    execute_process(
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/vendor/lua"
        COMMAND ${ZIGC} cc -DMAKE_LUA onelua.c -o lua
            -target ${CMAKE_HOST_SYSTEM_PROCESSOR}-${CMAKE_HOST_SYSTEM_NAME_LOWER}
    )
endif()
file(GLOB lua_files "${CMAKE_SOURCE_DIR}/vendor/lua/*.c")
list(REMOVE_ITEM lua_files "${CMAKE_SOURCE_DIR}/vendor/lua/lua.c")
list(REMOVE_ITEM lua_files "${CMAKE_SOURCE_DIR}/vendor/lua/onelua.c")
add_library(lua-static STATIC "${lua_files}")
find_program(LUAC lua PATHS "${CMAKE_SOURCE_DIR}/vendor/lua" REQUIRED NO_DEFAULT_PATH)

# Engine
set(ENGINE_DIR "${CMAKE_SOURCE_DIR}/vendor/engine")
set(ENGINE_VERSION "0.0.10")
set(ENGINE_DOWNLOAD "https://github.com/gamelly/gly-engine/archive/refs/tags/${ENGINE_VERSION}.tar.gz")
set(ENGINE_HEADER "${ENGINE_DIR}/bytecode.h")
if(NOT EXISTS ${ENGINE_HEADER})
    FetchContent_Declare(
        lib_engine
        URL "${ENGINE_DOWNLOAD}"
        SOURCE_DIR ${ENGINE_DIR}
        SUBBUILD_DIR "${CMAKE_SOURCE_DIR}/vendor/_deps/engine-subbuild"
        BINARY_DIR "${CMAKE_SOURCE_DIR}/vendor/_deps/engine-build"
    )
    FetchContent_MakeAvailable(lib_engine)
    file(MAKE_DIRECTORY "${ENGINE_DIR}/lite")
    file(MAKE_DIRECTORY "${ENGINE_DIR}/full")
    file(MAKE_DIRECTORY "${ENGINE_DIR}/examples/template")
    execute_process(
        WORKING_DIRECTORY ${ENGINE_DIR}
        COMMAND ${LUAC} src/cli/main.lua build --core lite --bundler --dist ${ENGINE_DIR}/lite/
    )
    execute_process(
        WORKING_DIRECTORY ${ENGINE_DIR}
        COMMAND ${LUAC} src/cli/main.lua build --core native --bundler --dist ${ENGINE_DIR}/native/
    )
    execute_process(
        WORKING_DIRECTORY ${ENGINE_DIR}
        COMMAND ${LUAC} src/cli/main.lua tool-template-fill ${ENGINE_DIR}/examples/template/game.lua 204800
    )
    set(ENGINE "@native")
endif()
if(DEFINED ENGINE)
    if (ENGINE MATCHES "^\\@")
        string(SUBSTRING "${ENGINE}" 1 -1 ENGINE)
        set(ENGINE "${ENGINE_DIR}/${ENGINE}/main.lua")
    endif()
    if(NOT EXISTS ${ENGINE})
        message(FATAL_ERROR "engine not found: ${ENGINE}")
    endif()
    get_filename_component(ENGINE "${ENGINE}" ABSOLUTE)
    execute_process(
        WORKING_DIRECTORY ${ENGINE_DIR}
        COMMAND ${LUAC} src/cli/main.lua fs-xxd-i ${ENGINE} ${ENGINE_HEADER} --const --name engine_bytecode_lua
        COMMAND_ERROR_IS_FATAL ANY
    )
endif()

# Game
string(LENGTH "${GAME}" GAME_LENGTH)
set(GAME_HEADER "${CMAKE_SOURCE_DIR}/vendor/game/bytecode.h")
if (DEFINED GAME AND GAME_LENGTH AND GAME MATCHES "^\\@")
    string(SUBSTRING "${GAME}" 1 -1 GAME)
    set(GAME "${ENGINE_DIR}/examples/${GAME}/game.lua")
endif()
if ((NOT DEFINED GAME AND NOT EXISTS ${GAME_HEADER}) OR (DEFINED GAME AND NOT GAME_LENGTH))
    set(GAME "${ENGINE_DIR}/examples/template/game.lua")
    set(GAME_LENGTH 1)
endif()
if(DEFINED GAME AND GAME_LENGTH)
    if(NOT EXISTS ${GAME})
        message(FATAL_ERROR "game not found: ${GAME}")
    endif()
    get_filename_component(GAME "${GAME}" ABSOLUTE)
    file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/vendor/game")
    execute_process(
        WORKING_DIRECTORY ${ENGINE_DIR}
        COMMAND ${LUAC} src/cli/main.lua fs-xxd-i ${GAME} ${GAME_HEADER} --const --name game_bytecode_lua
        COMMAND_ERROR_IS_FATAL ANY
    )
endif()

# Lua perfomance
if(LUA_OPTMIZE)
    execute_process(
        WORKING_DIRECTORY ${ENGINE_DIR}
        COMMAND ${LUAC} src/cli/main.lua fs-luaconf ${LUA_PATH}/luaconf.h --32bits
    )
endif()

target_link_libraries(${PROJECT_NAME} lua-static)
target_link_libraries(${PROJECT_NAME} m)
