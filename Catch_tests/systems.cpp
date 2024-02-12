#include "systems.h"

#include <catch2/catch_test_macros.hpp>

#include <cstdint>

TEST_CASE("It computes collision box top", "[top]")
{
    // arrange
    const Position position{Vector2{
        .x = 30,
        .y = 40,
    }};
    const CollisionBox collision_box{5, 5};

    // act
    const float ball_top{top(position, collision_box)};

    // assert
    CHECK(ball_top == 35);
}

TEST_CASE("It computes collision box right", "[right]")
{
    // arrange
    const Position position{Vector2{
        .x = 30,
        .y = 40,
    }};
    const CollisionBox collision_box{5, 5};

    // act
    const float ball_right{right(position, collision_box)};

    // assert
    CHECK(ball_right == 35);
}

TEST_CASE("It computes collision box bottom", "[bottom]")
{
    // arrange
    const Position position{Vector2{
        .x = 30,
        .y = 40,
    }};
    const CollisionBox collision_box{5, 5};

    // act
    const float ball_bottom{bottom(position, collision_box)};

    // assert
    CHECK(ball_bottom == 45);
}

TEST_CASE("It computes collision box left", "[left]")
{
    // arrange
    const Position position{Vector2{
        .x = 30,
        .y = 40,
    }};
    const CollisionBox collision_box{5, 5};

    // act
    const float ball_left{left(position, collision_box)};

    // assert
    CHECK(ball_left == 25);
}
