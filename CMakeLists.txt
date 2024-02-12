cmake_minimum_required(VERSION 3.16)
project(RaylibGame LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)
add_library(arkanoid_compiler_flags INTERFACE)
target_compile_features(arkanoid_compiler_flags INTERFACE cxx_std_17)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(
    arkanoid_compiler_flags
    INTERFACE
        "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-pedantic-errors;-Werror;-Wall;-Weffc++;-Wextra;-Wconversion;-Wsign-conversion>>"
        "$<${msvc_cxx}:$<BUILD_INTERFACE:-W4>>")

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake")
option(RUN_TESTS "Enable tests" ON)
include(coverage)
add_coverage_target("Catch_tests/*")

include(cmake/CPM.cmake)
cpmaddpackage("gh:sharkdp/dbg-macro#fb9976f410f8b29105818b20278cd0be0e853fe8"
)# v0.5.1
cpmaddpackage("gh:SanderMertens/flecs#198607d10ab8f8fc44540043271d6e3be019250b"
)# v3.2.11
cpmaddpackage("gh:fmtlib/fmt#e69e5f977d458f2650bb346dadf2ad30c5320281") # 10.x
cpmaddpackage("gh:raysan5/raylib#ae50bfa2cc569c0f8d5bc4315d39db64005b1b0"
)# v5.0
cpmaddpackage("gh:gabime/spdlog#7c02e204c92545f869e2f04edaab1f19fe8b19fd"
)# v1.13.0

add_library(Entities_lib STATIC "${PROJECT_SOURCE_DIR}/src/entities.cpp")
target_link_libraries(
    Entities_lib
    flecs::flecs_static
    raylib
    arkanoid_compiler_flags)

add_library(Systems_lib STATIC "${PROJECT_SOURCE_DIR}/src/systems.cpp")
target_link_libraries(
    Systems_lib
    flecs::flecs_static
    raylib
    arkanoid_compiler_flags)

add_executable(Arkanoid src/main.cpp)
target_link_libraries(
    Arkanoid
    PUBLIC Entities_lib
           Systems_lib
           dbg_macro
           flecs::flecs_static
           fmt
           raylib
           arkanoid_compiler_flags
           spdlog::spdlog_header_only)
target_include_directories(Arkanoid PUBLIC "${PROJECT_BINARY_DIR}")

target_compile_definitions(
    Arkanoid PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")

option(RUN_UNIT_TESTS "Run Catch2 unit tests" ON)
if(RUN_UNIT_TESTS)
    enable_testing()
    add_subdirectory(Catch_tests)
endif()
