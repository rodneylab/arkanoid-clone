#include "Ball.h"

#include <SFML/Graphics.hpp>
#include <catch2/catch_test_macros.hpp>

// NOLINTNEXTLINE(misc-use-anonymous-namespace)
TEST_CASE("BallCorrectlyInitialised", "[BallTests]")
{
    // NOLINTBEGIN(readability-magic-numbers)
    // NOLINTBEGIN(cppcoreguidelines-avoid-do-while)
    const Ball ball{256, 512};

    const sf::Color ball_colour{ball.shape.getFillColor()};
    const sf::Vector2 ball_origin{ball.shape.getOrigin()};
    const float ball_radius{ball.shape.getRadius()};

    CHECK(ball_colour == sf::Color::Red);
    CHECK(ball_origin.x == 10.F);
    CHECK(ball_origin.y == 10.F);
    CHECK(ball.x() == 256.F);
    CHECK(ball.y() == 512.F);
    CHECK(ball_radius == 10.F);
    // NOLINTEND(cppcoreguidelines-avoid-do-while)
    // NOLINTEND(readability-magic-numbers)
}

// NOLINTNEXTLINE(misc-use-anonymous-namespace)
TEST_CASE("BallEdgeMethodsReturnExpectedValues", "[BallTests]")
{
    // NOLINTBEGIN(readability-magic-numbers)
    // NOLINTBEGIN(cppcoreguidelines-avoid-do-while)
    constexpr float ball_radius{10.F};
    constexpr unsigned int ball_x{256};
    constexpr unsigned int ball_y{512};
    const Ball ball{ball_x, ball_y};

    CHECK(ball.top() == ball_y - ball_radius);
    CHECK(ball.right() == ball_x + ball_radius);
    CHECK(ball.bottom() == ball_y + ball_radius);
    CHECK(ball.left() == ball_x - ball_radius);
    // NOLINTEND(cppcoreguidelines-avoid-do-while)
    // NOLINTEND(readability-magic-numbers)
}

// NOLINTNEXTLINE(misc-use-anonymous-namespace)
TEST_CASE("UpdateChangesVelocitDirectionWhenBallMovesOffScreen", "[BallTests]")
{
    // NOLINTBEGIN(readability-magic-numbers)
    // NOLINTBEGIN(cppcoreguidelines-avoid-do-while)

    // arrange
    constexpr unsigned int ball_x{10};
    constexpr unsigned int ball_y{10};
    Ball ball{ball_x, ball_y};
    ball.velocity = sf::Vector2{-20.F, -20.F};

    // act
    ball.update();

    // assert
    CHECK(ball.velocity.x > 0);
    CHECK(ball.velocity.y > 0);

    // arrange
    ball = Ball{1000, 1000};
    ball.velocity = sf::Vector2{20.F, 20.F};

    // act
    ball.update();

    // assert
    CHECK(ball.velocity.x < 0);
    CHECK(ball.velocity.y < 0);

    // NOLINTEND(cppcoreguidelines-avoid-do-while)
    // NOLINTEND(readability-magic-numbers)
}
