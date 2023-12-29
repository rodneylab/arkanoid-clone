find_package(Catch2 3)
if(NOT Catch2_FOUND)
    cpmaddpackage("gh:catchorg/Catch2#v3.5.0")
endif()

add_executable(Catch_tests_run BallTests.cpp PaddleTests.cpp)

target_link_libraries(
    Catch_tests_run
    PRIVATE Ball_lib
            Brick_lib
            Paddle_lib
            arkanoid_compiler_flags)
target_link_libraries(Catch_tests_run PRIVATE Catch2::Catch2WithMain)
target_include_directories(Catch_tests_run PUBLIC "${PROJECT_SOURCE_DIR}/src")

include(Catch)
catch_discover_tests(Catch_tests_run)
