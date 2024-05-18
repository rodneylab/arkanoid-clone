message(STATUS "Include Catch2")
cpmaddpackage("gh:catchorg/Catch2#4e8d92bf02f7d1c8006a0e7a5ecabd8e62d98502"
)# v3.6.0

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
