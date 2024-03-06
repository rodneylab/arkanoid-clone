find_package(Catch2 3)
if(NOT Catch2_FOUND)
    cpmaddpackage("gh:catchorg/Catch2#8ac8190e494a381072c89f5e161b92a08d98b37b"
    )# v3.5.3
endif()

list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
add_subdirectory(components)
add_subdirectory(entities)

add_executable(Catch_tests_run systems.cpp)

target_link_libraries(Catch_tests_run PRIVATE Entities_lib Systems_lib
                                              arkanoid_compiler_flags)
target_link_libraries(Catch_tests_run PRIVATE Catch2::Catch2WithMain)
target_include_directories(Catch_tests_run PUBLIC "${PROJECT_SOURCE_DIR}/src")

include(Catch)
catch_discover_tests(Catch_tests_run)
