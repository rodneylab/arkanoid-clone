#include "entities.h"

#include "components.h"
#include "constants.h"

#include <flecs.h>
#include <raylib.h>

void create_ball(flecs::world *world)
{
    auto ball(world->entity("Ball"));
    ball.add<Ball>();
    ball.set<CircleComponent>(CircleComponent(constants::kBallRadius, RED));

    Position position(static_cast<float>(constants::kWindowWidth) / 2,
                      static_cast<float>(constants::kWindowHeight) / 2);
    ball.set<Position>(position);

    ball.set<CollisionBox>(
        CollisionBox(constants::kBallRadius, constants::kBallRadius));
    ball.set<Velocity>(
        Velocity(-constants::kBallVelocity, -constants::kBallVelocity));
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
                RectangleComponent(constants::kBrickWidth,
                                   constants::kBrickHeight,
                                   YELLOW));
            brick.set<Position>(Position(
                static_cast<float>((row + 1) * (constants::kBrickWidth + 3) +
                                   constants::kBricksInsetX),
                static_cast<float>((column + 2) *
                                   (constants::kBrickHeight + 3))));
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
    paddle.set<RectangleComponent>(RectangleComponent{constants::kPaddleWidth,
                                                      constants::kPaddleHeight,
                                                      RED});
    paddle.set<Position>(Position{
        static_cast<float>(constants::kWindowWidth) / 2,
        static_cast<float>(constants::kWindowHeight) -
            constants::kPaddleInsetBottom,
    });
    // NOLINTBEGIN(readability-magic-numbers)
    paddle.set<CollisionBox>(CollisionBox{0.5F * constants::kPaddleWidth,
                                          0.5F * constants::kPaddleHeight});
    // NOLINTEND(readability-magic-numbers)
    paddle.set<Velocity>(Velocity{0.F, 0.F});
}
