#include "systems.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <cstdint>

constexpr float kFloatEps{0.001F};
constexpr float kFloatAbsMargin{0.000001F};

TEST_CASE("It computes collision box top", "[top]")
{
    // arrange
    const Position position{
        30,
        40,
    };
    const CollisionBox collision_box{5, 5};

    // act
    const float ball_top{top(position, collision_box)};

    // assert
    constexpr float expected{35.F};
    REQUIRE_THAT(ball_top,
                 Catch::Matchers::WithinRel(expected, kFloatEps) ||
                     Catch::Matchers::WithinAbs(expected, kFloatAbsMargin));
}

TEST_CASE("It computes collision box right", "[right]")
{
    // arrange
    const Position position{
        30,
        40,
    };
    const CollisionBox collision_box{5, 5};

    // act
    const float ball_right{right(position, collision_box)};

    // assert
    constexpr float expected{35.F};
    REQUIRE_THAT(ball_right,
                 Catch::Matchers::WithinRel(expected, kFloatEps) ||
                     Catch::Matchers::WithinAbs(expected, kFloatAbsMargin));
}

TEST_CASE("It computes collision box bottom", "[bottom]")
{
    // arrange
    const Position position{
        30,
        40,
    };
    const CollisionBox collision_box{5, 5};

    // act
    const float ball_bottom{bottom(position, collision_box)};

    // assert
    constexpr float expected{45.F};
    REQUIRE_THAT(ball_bottom,
                 Catch::Matchers::WithinRel(expected, kFloatEps) ||
                     Catch::Matchers::WithinAbs(expected, kFloatAbsMargin));
}

TEST_CASE("It computes collision box left", "[left]")
{
    // arrange
    const Position position{
        30,
        40,
    };
    const CollisionBox collision_box{5, 5};

    // act
    const float ball_left{left(position, collision_box)};

    // assert
    constexpr float expected{25.F};
    REQUIRE_THAT(ball_left,
                 Catch::Matchers::WithinRel(expected, kFloatEps) ||
                     Catch::Matchers::WithinAbs(expected, kFloatAbsMargin));
}
