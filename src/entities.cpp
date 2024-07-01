#include "entities.h"

#include "components.h"
#include "constants.h"

#include <flecs/addons/cpp/entity.hpp>
#include <flecs/addons/cpp/world.hpp>

// Windows workarounds for CloseWindow / ShowCursor errors

#if defined(_WIN32)
#define NOGDI  // All GDI defines and routines
#define NOUSER // All USER defines and routines
#endif

#include <fmt/core.h>

#undef near
#undef far
#include <raylib.h>

#include <string>

constexpr float kPaddleTop{static_cast<float>(constants::kWindowHeight) -
                           0.5 * constants::kPaddleHeight -
                           constants::kPaddleInsetBottom};

constexpr float kBallInitialPositionTop{kPaddleTop -
                                        2.F * constants::kBallRadius};

void create_ball(flecs::world *world)
{
    // Randomly choose ball x direction
    const float ball_x_velocity{
        constants::kBallVelocity *
        (-1.F + static_cast<float>(GetRandomValue(0, 1)) * 2.F)};

    world->entity("Ball")
        .add<Ball>()
        .set<CircleComponent>(CircleComponent(constants::kBallRadius, RED))
        .set<Position>(Position(static_cast<float>(constants::kWallWidth) +
                                    0.5F * constants::kBricksWidth,
                                kBallInitialPositionTop))
        .set<CollisionBox>(
            CollisionBox(constants::kBallRadius, 0.5F * constants::kBallRadius))
        .set<Velocity>(
            Velocity(ball_x_velocity, -0.5F * constants::kBallVelocity));
}

void create_bricks(flecs::world *world)
{
    constexpr float padded_brick_width{constants::kBrickWidth +
                                       constants::kBrickPadding};
    constexpr float padded_brick_height{constants::kBrickHeight +
                                        constants::kBrickPadding};
    constexpr float collision_box_half_width{0.5F * constants::kBrickWidth};
    constexpr float collision_box_half_height{0.5F * constants::kBrickHeight};

    const flecs::entity BrickEntity{
        world->prefab("Brick")
            .set<RectangleComponent>(RectangleComponent(constants::kBrickWidth,
                                                        constants::kBrickHeight,
                                                        YELLOW))
            .set<CollisionBox>(CollisionBox{collision_box_half_width,
                                            collision_box_half_height})};
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
                    static_cast<float>(row) * padded_brick_width +
                        constants::kBricksInsetX +
                        0.5F * constants::kBrickWidth,
                    static_cast<float>(column) * padded_brick_height +
                        constants::kBricksInsetY + 0.5F * padded_brick_height));
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
    paddle.set<Position>(Position{static_cast<float>(constants::kWallWidth) +
                                      0.5F * constants::kBricksWidth,
                                  kPaddleTop});
    paddle.set<CollisionBox>(CollisionBox{0.5F * constants::kPaddleWidth,
                                          0.5F * constants::kPaddleHeight});
    paddle.set<Velocity>(Velocity{0.F, 0.F});
}

void create_walls(flecs::world *world)
{
    auto top_wall(world->entity("TopWall"));
    top_wall.add<Wall>();
    top_wall.set<AxisAlignedOneWayCollider>(
        AxisAlignedOneWayCollider(constants::kBricksInsetX,
                                  CollisionSide::BOTTOM));

    auto left_wall(world->entity("LeftWall"));
    left_wall.add<Wall>();
    left_wall.set<AxisAlignedOneWayCollider>(
        AxisAlignedOneWayCollider(constants::kBricksInsetX,
                                  CollisionSide::RIGHT));

    constexpr float kRightWallLeftSideDisplacement{constants::kWallWidth +
                                                   constants::kBricksWidth};

    auto right_wall(world->entity("RightWall"));
    right_wall.add<Wall>();
    right_wall.set<AxisAlignedOneWayCollider>(
        AxisAlignedOneWayCollider(kRightWallLeftSideDisplacement,
                                  CollisionSide::LEFT));
}

void create_ball_with_brick_collision_sound(flecs::world *world)
{
    const Sound ball_brick_collision_sound{
        LoadSound(ASSETS_PATH "ArkanoidSFX7.wav")};
    auto sound_entity(world->entity("BallBrickCollisionSound"));
    sound_entity.set<Audible>(Audible{ball_brick_collision_sound});
}

void destroy_ball_with_brick_collision_sound(flecs::world *world)
{
    const Sound ball_brick_collision_sound{
        world->lookup("BallBrickCollisionSound").get<Audible>()->sound};
    UnloadSound(ball_brick_collision_sound);
}

void create_ball_with_paddle_collision_sound(flecs::world *world)
{
    const Sound paddle_ball_collision_sound{
        LoadSound(ASSETS_PATH "ArkanoidSFX6.wav")};
    auto sound_entity(world->entity("BallPaddleCollisionSound"));
    sound_entity.set<Audible>(Audible{paddle_ball_collision_sound});
}

void destroy_ball_with_paddle_collision_sound(flecs::world *world)
{
    const Sound paddle_ball_collision_sound =
        world->lookup("BallPaddleCollisionSound").get<Audible>()->sound;
    UnloadSound(paddle_ball_collision_sound);
}
