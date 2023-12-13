#include <SFML/Graphics.hpp>
#include <catch2/catch_test_macros.hpp>

#include "Ball.h"

// NOLINTNEXTLINE(misc-use-anonymous-namespace)
TEST_CASE("BallCorrectlyInitialised", "[BallTests]")
{
    // NOLINTBEGIN(readability-magic-numbers)
    // NOLINTBEGIN(cppcoreguidelines-avoid-do-while)
    const Ball ball{256, 512};

    const sf::Vector2 ball_origin{ball.shape.getOrigin()};
    const sf::Vector2 ball_position{ball.shape.getPosition()};
    const sf::Color ball_colour{ball.shape.getFillColor()};
    const float ball_radius{ball.shape.getRadius()};

    CHECK(ball_origin.x == 10.F);
    CHECK(ball_origin.y == 10.F);
    CHECK(ball_position.x == 256);
    CHECK(ball_position.y == 512);
    CHECK(ball_colour == sf::Color::Red);
    CHECK(ball_radius == 10.F);
    // NOLINTEND(cppcoreguidelines-avoid-do-while)
    // NOLINTEND(readability-magic-numbers)
}
