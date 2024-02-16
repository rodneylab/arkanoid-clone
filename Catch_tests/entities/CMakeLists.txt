add_executable(EntityTests ball.cpp bricks.cpp)

target_link_libraries(EntityTests PRIVATE Entities_lib Systems_lib
                                          arkanoid_compiler_flags)

target_link_libraries(EntityTests PRIVATE Catch2::Catch2WithMain)
target_include_directories(EntityTests PUBLIC "${PROJECT_SOURCE_DIR}/src")
