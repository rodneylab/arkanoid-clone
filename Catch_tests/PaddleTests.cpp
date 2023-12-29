#include "Paddle.h"

#include <SFML/Graphics.hpp>
#include <catch2/catch_test_macros.hpp>

// NOLINTNEXTLINE(misc-use-anonymous-namespace)
TEST_CASE("PaddleCorrectlyInitialised", "[PaddleTests]")
{
    // NOLINTBEGIN(readability-magic-numbers)
    // NOLINTBEGIN(cppcoreguidelines-avoid-do-while)
    const Paddle paddle{256, 512};

    const sf::Color paddle_colour{paddle.shape.getFillColor()};
    const sf::Vector2 paddle_origin{paddle.shape.getOrigin()};
    const float paddle_width{paddle.shape.getSize().x};
    const float paddle_height{paddle.shape.getSize().y};

    CHECK(paddle_colour == sf::Color::Red);
    CHECK(paddle_origin.x == 30.F);
    CHECK(paddle_origin.y == 10.F);
    CHECK(paddle.x() == 256.F);
    CHECK(paddle.y() == 512.F);
    CHECK(paddle_width == 60.F);
    CHECK(paddle_height == 20.F);
    // NOLINTEND(cppcoreguidelines-avoid-do-while)
    // NOLINTEND(readability-magic-numbers)
}
//
// NOLINTNEXTLINE(misc-use-anonymous-namespace)
TEST_CASE("PaddleEdgeMethodsReturnExpectedValues", "[PaddleTests]")
{
    // NOLINTBEGIN(readability-magic-numbers)
    // NOLINTBEGIN(cppcoreguidelines-avoid-do-while)
    constexpr float paddle_width{60.F};
    constexpr float paddle_height{20.F};
    constexpr unsigned int paddle_x{256};
    constexpr unsigned int paddle_y{512};
    const Paddle paddle{paddle_x, paddle_y};

    CHECK(paddle.top() == paddle_y - (paddle_height / 2));
    CHECK(paddle.right() == paddle_x + (paddle_width / 2));
    CHECK(paddle.bottom() == paddle_y + (paddle_height / 2));
    CHECK(paddle.left() == paddle_x - (paddle_width / 2));
    // NOLINTEND(cppcoreguidelines-avoid-do-while)
    // NOLINTEND(readability-magic-numbers)
}
