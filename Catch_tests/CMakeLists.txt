find_package(Catch2 3)
if(NOT Catch2_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.5.0)
    FetchContent_MakeAvailable(Catch2)
endif()

add_executable(Catch_tests_run BallTests.cpp PaddleTests.cpp)

target_link_libraries(Catch_tests_run PRIVATE Game_lib arkanoid_compiler_flags)
target_link_libraries(Catch_tests_run PRIVATE Catch2::Catch2WithMain)
target_include_directories(Catch_tests_run PUBLIC "${PROJECT_SOURCE_DIR}/src")

include(Catch)
catch_discover_tests(Catch_tests_run)
