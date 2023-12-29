cmake_minimum_required(VERSION 3.16)
project(Arkanoid LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)
add_library(arkanoid_compiler_flags INTERFACE)
target_compile_features(arkanoid_compiler_flags INTERFACE cxx_std_17)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake")
option(RUN_TESTS "Enable tests" ON)
include(coverage)
add_coverage_target("*/Catch_tests/*")

include(cmake/CPM.cmake)

find_package(
    SFML 2.6
    COMPONENTS network
               audio
               graphics
               window
               system)
if(NOT SFML_FOUND)
    cpmaddpackage("gh:SFML/SFML#2.6.x")
endif()

add_library(Ball_lib STATIC "${PROJECT_SOURCE_DIR}/src/Ball.cpp")
target_link_libraries(
    Ball_lib
    sfml-network
    sfml-audio
    sfml-graphics
    sfml-window
    sfml-system
    arkanoid_compiler_flags)

add_library(Brick_lib STATIC "${PROJECT_SOURCE_DIR}/src/Brick.cpp")
target_link_libraries(
    Brick_lib
    sfml-network
    sfml-audio
    sfml-graphics
    sfml-window
    sfml-system
    arkanoid_compiler_flags)

add_library(Paddle_lib STATIC "${PROJECT_SOURCE_DIR}/src/Paddle.cpp")
target_link_libraries(
    Paddle_lib
    sfml-network
    sfml-audio
    sfml-graphics
    sfml-window
    sfml-system
    arkanoid_compiler_flags)

add_executable(Arkanoid src/arkanoid.cpp)
target_link_libraries(
    Arkanoid
    PUBLIC Ball_lib
           Brick_lib
           Paddle_lib
           arkanoid_compiler_flags)
target_include_directories(Arkanoid PUBLIC "${PROJECT_BINARY_DIR}")

if(WIN32)
    add_custom_command(
        TARGET Arkanoid
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD
        COMMAND
            ${CMAKE_COMMAND} -E copy
            ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll
            $<TARGET_FILE_DIR:Arkanoid>
        VERBATIM)
endif()

install(TARGETS Arkanoid)

option(RUN_UNIT_TESTS "Run Catch2 unit tests" ON)
if(RUN_UNIT_TESTS)
    enable_testing()
    add_subdirectory(Catch_tests)
endif()
