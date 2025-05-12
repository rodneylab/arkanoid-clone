cmake_minimum_required(VERSION 3.16)
project(
    Arkanoid
    VERSION 0.0.1
    DESCRIPTION "Arkanoid clone built in C++ with raylib and flecs."
    LANGUAGES CXX )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)
add_library(arkanoid_compiler_flags INTERFACE)
target_compile_features(arkanoid_compiler_flags INTERFACE cxx_std_17)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

if(DEFINED EMSCRIPTEN)
    set(CMAKE_EXE_LINKER_FLAGS
        "${CMAKE_EXE_LINKER_FLAGS} -s -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1 -s ALLOW_MEMORY_GROWTH=1 -s STACK_SIZE=1mb -s EXPORTED_RUNTIME_METHODS=cwrap"
    )
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(
    arkanoid_compiler_flags
    INTERFACE
        "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-pedantic-errors;-Werror;-Wall;-Weffc++;-Wextra;-Wconversion;-Wsign-conversion>>"
        "$<${msvc_cxx}:$<BUILD_INTERFACE:-W4>>")

if(NOT DEFINED EMSCRIPTEN)
    include(cmake/StaticAnalysers.cmake)
    enable_clang_tidy()

    set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake")
    option(RUN_TESTS "Enable tests" ON)
    include(coverage)
    add_coverage_target("Catch_tests/*")
endif()

include(cmake/CPM.cmake)
cpmaddpackage("gh:sharkdp/dbg-macro#fb9976f410f8b29105818b20278cd0be0e853fe8"
)# v0.5.1
cpmaddpackage("gh:SanderMertens/flecs#eeb9eaa64243536c5d5c19279c9c305a52d0b9b3"
)# v4.0.0
cpmaddpackage("gh:fmtlib/fmt#0c9fce2ffefecfdce794e1859584e25877b7b592") # 11.0.2
cpmaddpackage("gh:raysan5/raylib#ae50bfa2cc569c0f8d5bc4315d39db64005b1b0"
)# v5.0
cpmaddpackage("gh:nlohmann/json#9cca280a4d0ccf0c08f47a99aa71d1b0e52f8d03"
)# v3.11.3
message(STATUS "Include spdlog")
cpmaddpackage("gh:gabime/spdlog#27cb4c76708608465c413f6d0e6b8d99a4d84302"
)# v1.14.1

add_library(Components_lib STATIC "${PROJECT_SOURCE_DIR}/src/components.cpp")
set_target_properties(Components_lib PROPERTIES CXX_CLANG_TIDY
                                                "${CLANG_TIDY_COMMAND}")
target_link_libraries(
    Components_lib
    fmt::fmt
    raylib
    spdlog::spdlog_header_only
    arkanoid_compiler_flags)
target_compile_definitions(Components_lib PRIVATE SPDLOG_FMT_EXTERNAL)

add_library(FileSystem_lib STATIC "${PROJECT_SOURCE_DIR}/src/file_system.cpp")
set_target_properties(FileSystem_lib PROPERTIES CXX_CLANG_TIDY
                                                "${CLANG_TIDY_COMMAND}")
target_link_libraries(
    FileSystem_lib
    Components_lib
    nlohmann_json::nlohmann_json
    arkanoid_compiler_flags)
target_compile_definitions(
    FileSystem_lib PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")
target_compile_definitions(FileSystem_lib PRIVATE SPDLOG_FMT_EXTERNAL)

add_library(Entities_lib STATIC "${PROJECT_SOURCE_DIR}/src/entities.cpp")
set_target_properties(Entities_lib PROPERTIES CXX_CLANG_TIDY
                                              "${CLANG_TIDY_COMMAND}")
target_link_libraries(
    Entities_lib
    FileSystem_lib
    flecs::flecs_static
    arkanoid_compiler_flags)
target_compile_definitions(
    Entities_lib PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")

add_library(Systems_lib STATIC "${PROJECT_SOURCE_DIR}/src/systems.cpp")
set_target_properties(Systems_lib PROPERTIES CXX_CLANG_TIDY
                                             "${CLANG_TIDY_COMMAND}")
target_link_libraries(
    Systems_lib
    Components_lib
    Entities_lib
    fmt::fmt
    raylib
    arkanoid_compiler_flags)
target_compile_definitions(Systems_lib PRIVATE SPDLOG_FMT_EXTERNAL)

add_executable(Arkanoid src/main.cpp)
set_target_properties(Arkanoid PROPERTIES CXX_CLANG_TIDY
                                          "${CLANG_TIDY_COMMAND}")
target_link_libraries(
    Arkanoid
    PUBLIC Components_lib
           Entities_lib
           Systems_lib
           dbg_macro
           flecs::flecs_static
           arkanoid_compiler_flags
           spdlog::spdlog_header_only)
target_include_directories(Arkanoid PUBLIC "${PROJECT_BINARY_DIR}")
target_compile_definitions(
    Arkanoid PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")
target_compile_definitions(Arkanoid PRIVATE SPDLOG_FMT_EXTERNAL)

option(RUN_UNIT_TESTS "Run Catch2 unit tests" ON)
if(RUN_UNIT_TESTS)
    enable_testing()
    add_subdirectory(Catch_tests)
endif()

option(CREATE_DOCUMENTATION "Create documentation" ON)
if(CREATE_DOCUMENTATION)
    set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
    add_subdirectory(docs)
endif()
