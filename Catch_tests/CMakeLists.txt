cpmaddpackage("gh:catchorg/Catch2#05e10dfccc28c7f973727c54f850237d07d5e10f"
)# v3.5.2

#add_executable(Catch_tests_run BallTests.cpp PaddleTests.cpp)
add_executable(Catch_tests_run ComponentTests.cpp)

target_link_libraries(Catch_tests_run PRIVATE Game_lib arkanoid_compiler_flags)
target_link_libraries(Catch_tests_run PRIVATE Catch2::Catch2WithMain)
target_include_directories(Catch_tests_run PUBLIC "${PROJECT_SOURCE_DIR}/src")

list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(Catch_tests_run)
