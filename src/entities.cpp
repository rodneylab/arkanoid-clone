#include "entities.h"

#include "components.h"
#include "constants.h"

#include <flecs.h>
#include <fmt/core.h>
#include <raylib.h>

void create_ball(flecs::world *world)
{
    // Randomly choose ball x direction
    const float ball_x_velocity{
        constants::kBallVelocity *
        (-1.F + static_cast<float>(GetRandomValue(0, 1)) * 2.F)};

    world->entity("Ball")
        .add<Ball>()
        .set<CircleComponent>(CircleComponent(constants::kBallRadius, RED))
        .set<Position>(
            Position(static_cast<float>(constants::kWindowWidth) / 2,
                     static_cast<float>(constants::kWindowHeight) / 2))
        .set<CollisionBox>(
            CollisionBox(constants::kBallRadius, constants::kBallRadius))
        .set<Velocity>(Velocity(ball_x_velocity, -constants::kBallVelocity));
}

void create_bricks(flecs::world *world)
{
    constexpr float padded_brick_width{constants::kBrickWidth + 3};
    constexpr float padded_brick_height{constants::kBrickHeight + 3};
    // NOLINTBEGIN(readability-magic-numbers)
    constexpr float collision_box_half_width{0.5F * constants::kBrickWidth};
    constexpr float collision_box_half_height{0.5F * constants::kBrickHeight};
    // NOLINTEND(readability-magic-numbers)

    flecs::entity BrickEntity =
        world->prefab("Brick")
            .set<RectangleComponent>(RectangleComponent(constants::kBrickWidth,
                                                        constants::kBrickHeight,
                                                        YELLOW))
            .set<CollisionBox>(CollisionBox{collision_box_half_width,
                                            collision_box_half_height});

    for (int column{0}; column < constants::kBrickRows; ++column)
    {
        for (int row{0}; row < constants::kBrickColumns; ++row)
        {
            const std::string name{
                fmt::format("Brick_{:02}_{:02}", column + 1, row + 1)};
            world->entity(name.c_str())
                .is_a(BrickEntity)
                .add<Brick>()
                .set<Position>(Position(
                    (static_cast<float>(row + 1) * padded_brick_width +
                     constants::kBricksInsetX),
                    (static_cast<float>(column + 2) * padded_brick_height)));
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

void create_ball_with_paddle_collision_sound(flecs::world *world)
{
    Sound paddle_ball_collision_sound =
        LoadSound(ASSETS_PATH "ArkanoidSFX6.wav");
    auto sound_entity(world->entity("BallPaddleCollisionSound"));
    sound_entity.set<Audible>(Audible{paddle_ball_collision_sound});
}

void destroy_ball_with_paddle_collision_sound(flecs::world *world)
{
    const Sound paddle_ball_collision_sound =
        world->lookup("BallPaddleCollisionSound").get<Audible>()->sound;
    UnloadSound(paddle_ball_collision_sound);
}
