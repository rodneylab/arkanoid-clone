add_executable(ComponentTests rectangle.cpp)

target_link_libraries(ComponentTests PRIVATE fmt::fmt raylib arkanoid_compiler_flags)
target_link_libraries(ComponentTests PRIVATE Catch2::Catch2WithMain)
target_include_directories(ComponentTests PUBLIC "${PROJECT_SOURCE_DIR}/src")

include(Catch)
catch_discover_tests(ComponentTests)
