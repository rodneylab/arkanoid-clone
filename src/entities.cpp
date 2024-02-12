#include "entities.h"

#include "components.h"
#include "constants.h"

#include <flecs.h>

void create_ball(flecs::world *world)
{
    auto ball(world->entity("Ball"));
    ball.set<CircleComponent>(
        CircleComponent{.colour = RED, .radius = constants::kBallRadius});
    ball.set<Position>(Position{Vector2{
        .x = static_cast<float>(constants::kWindowWidth) / 2,
        .y = static_cast<float>(constants::kWindowHeight) / 2,
    }});
    ball.set<CollisionBox>(
        CollisionBox{constants::kBallRadius, constants::kBallRadius});
    ball.set<Velocity>(Velocity{Vector2{.x = -constants::kBallVelocity,
                                        .y = -constants::kBallVelocity}});
}

void create_bricks(flecs::world *world)
{

    for (int column{0}; column < constants::kBrickRows; ++column)
    {
        for (int row{0}; row < constants::kBrickColumns; ++row)
        {
            auto brick = world->entity();
            brick.add<Brick>();
            brick.set<RectangleComponent>(
                RectangleComponent{.width = constants::kBrickWidth,
                                   .height = constants::kBrickHeight,
                                   .colour = YELLOW});
            brick.set<Position>(Position{
                Vector2{.x = static_cast<float>(
                            (row + 1) * (constants::kBrickWidth + 3) +
                            constants::kBricksInsetX),
                        .y = static_cast<float>(
                            (column + 2) * (constants::kBrickHeight + 3))}});
            // NOLINTBEGIN(readability-magic-numbers)
            brick.set<CollisionBox>(
                CollisionBox{0.5F * constants::kBrickWidth,
                             0.5F * constants::kBrickHeight});
            // NOLINTEND(readability-magic-numbers)
        }
    }
}

void create_paddle(flecs::world *world)
{
    auto paddle(world->entity("Paddle"));
    paddle.add<Paddle>();
    paddle.set<RectangleComponent>(
        RectangleComponent{.width = constants::kPaddleWidth,
                           .height = constants::kPaddleHeight,
                           .colour = RED});
    paddle.set<Position>(Position{Vector2{
        .x = static_cast<float>(constants::kWindowWidth) / 2,
        .y = static_cast<float>(constants::kWindowHeight) -
             constants::kPaddleInsetBottom,
    }});
    // NOLINTBEGIN(readability-magic-numbers)
    paddle.set<CollisionBox>(CollisionBox{0.5F * constants::kPaddleWidth,
                                          0.5F * constants::kPaddleHeight});
    // NOLINTEND(readability-magic-numbers)
    paddle.set<Velocity>(Velocity{Vector2{.x = 0.F, .y = 0.F}});
}
