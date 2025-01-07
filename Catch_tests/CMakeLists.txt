message(STATUS "Include Catch2")
cpmaddpackage("gh:catchorg/Catch2#914aeecfe23b1e16af6ea675a4fb5dbd5a5b8d0a"
)# v3.8.0

list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
add_subdirectory(components)
add_subdirectory(entities)

add_executable(Catch_tests_run systems.cpp)

target_link_libraries(
    Catch_tests_run
    PRIVATE Components_lib
            Entities_lib
            Systems_lib
            arkanoid_compiler_flags)
target_link_libraries(Catch_tests_run PRIVATE Catch2::Catch2WithMain)
target_include_directories(Catch_tests_run PUBLIC "${PROJECT_SOURCE_DIR}/src")

include(Catch)
catch_discover_tests(Catch_tests_run)
