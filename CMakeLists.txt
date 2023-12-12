cmake_minimum_required(VERSION 3.16)
project(Araknoid LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

find_package(
    SFML 2.6
    COMPONENTS network
               audio
               graphics
               window
               system
    REQUIRED)
if(NOT SFML_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
    FetchContent_MakeAvailable(SFML)
endif()

add_library(Ball src/Ball.cpp src/Ball.h)
target_link_libraries(
    Ball
    sfml-network
    sfml-audio
    sfml-graphics
    sfml-window
    sfml-system)
target_compile_features(Ball PRIVATE cxx_std_17)

add_executable(Araknoid src/araknoid.cpp)
target_include_directories(Araknoid PUBLIC "${PROJECT_BINARY_DIR}")
target_link_libraries(Araknoid PRIVATE Ball)
target_compile_features(Araknoid PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET Araknoid
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD
        COMMAND
            ${CMAKE_COMMAND} -E copy
            ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll
            $<TARGET_FILE_DIR:Araknoid>
        VERBATIM)
endif()

install(TARGETS Araknoid)
