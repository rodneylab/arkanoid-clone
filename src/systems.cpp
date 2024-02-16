#include "systems.h"

#include "components.h"
#include "constants.h"

#include <flecs.h>
#include <raylib.h>

#include <cmath>

flecs::system add_paddle_movement_system(flecs::world *world)
{
    auto paddle_movement_system =
        world->system<Paddle, Position, CollisionBox>().each(
            [](flecs::entity entity,
               const Paddle & /* paddle */,
               const Position &position,
               const CollisionBox &collision_box

            ) {
                if (IsKeyDown(KEY_LEFT) && left(position, collision_box) > 0)
                {
                    entity.set<Velocity>(
                        Velocity{-constants::kPaddleVelocity, 0.F});
                }
                else if (IsKeyDown(KEY_RIGHT) &&
                         right(position, collision_box) <
                             constants::kWindowWidth)
                {
                    entity.set<Velocity>(
                        Velocity{constants::kPaddleVelocity, 0.F});
                }
                else
                {
                    entity.set<Velocity>(Velocity{0.F, 0.F});
                }
            });

    return paddle_movement_system;
}

flecs::system add_ball_with_wall_collision_system(flecs::world *world)
{
    flecs::system add_ball_with_wall_collision_system =
        world->system<Ball, Position, Velocity, CollisionBox>().each(
            [](const Ball & /* ball */,
               const Position &position,
               Velocity &velocity,
               const CollisionBox &collision_box) {
                if (left(position, collision_box) < 0 ||
                    right(position, collision_box) > constants::kWindowWidth)
                {
                    velocity.values.x = -velocity.values.x;
                }

                if (top(position, collision_box) < 0 ||
                    bottom(position, collision_box) > constants::kWindowHeight)
                {
                    velocity.values.y = -velocity.values.y;
                }
            });
    return add_ball_with_wall_collision_system;
}

flecs::system add_ball_with_paddle_collision_system(flecs::world *world,
                                                    flecs::entity *ball)
{
    flecs::system ball_with_paddle_collision_system =
        world->system<Paddle, CollisionBox, Position>().each(
            [ball](const Paddle /* paddle */,
                   const CollisionBox &collision_box,
                   const Position &position) {
                if (!is_intersecting(*ball->get<Position>(),
                                     *ball->get<CollisionBox>(),
                                     position,
                                     collision_box))
                {
                    return;
                }

                float new_velocity_y{-ball->get<Velocity>()->values.y};

                if (ball->get<Position>()->centre.x < position.centre.x)
                {
                    ball->set<Velocity>(
                        Velocity{-constants::kBallVelocity, new_velocity_y});
                }
                else
                {
                    ball->set<Velocity>(
                        Velocity{constants::kBallVelocity, new_velocity_y});
                }
            });

    return ball_with_paddle_collision_system;
}

flecs::system add_ball_with_brick_collision_system(flecs::world *world,
                                                   flecs::entity *ball)
{
    flecs::system ball_with_brick_collision_system =
        world->system<Brick, Position, CollisionBox>().each(
            [ball](flecs::entity entity,
                   Brick /* brick */,
                   Position position,
                   CollisionBox collision_box) {
                if (!is_intersecting(*ball->get<Position>(),
                                     *ball->get<CollisionBox>(),
                                     position,
                                     collision_box))
                {
                    return;
                }
                const float overlap_left{
                    right(*ball->get<Position>(), *ball->get<CollisionBox>()) -
                    left(position, collision_box)};
                const float overlap_right{
                    right(position, collision_box) -
                    left(*ball->get<Position>(), *ball->get<CollisionBox>())};
                const float overlap_top{
                    bottom(*ball->get<Position>(), *ball->get<CollisionBox>()) -
                    top(position, collision_box)};
                const float overlap_bottom{
                    bottom(position, collision_box) -
                    top(*ball->get<Position>(), *ball->get<CollisionBox>())};

                const bool ball_from_left{std::abs(overlap_left) <
                                          std::abs(overlap_right)};
                const bool ball_from_top{std::abs(overlap_top) <
                                         std::abs(overlap_bottom)};

                const float min_overlap_x{ball_from_left ? overlap_left
                                                         : overlap_right};
                const float min_overlap_y{ball_from_top ? overlap_top
                                                        : overlap_bottom};

                const Velocity *current_ball_velocity{ball->get<Velocity>()};
                const Vector2 current_ball_velocity_values{
                    current_ball_velocity->values};
                if (std::abs(min_overlap_x) < std::abs(min_overlap_y))
                {
                    ball->set<Velocity>(
                        Velocity{-current_ball_velocity_values.x,
                                 current_ball_velocity_values.y});
                }
                else
                {
                    ball->set<Velocity>(
                        Velocity{current_ball_velocity_values.x,
                                 -current_ball_velocity_values.y});
                }
                entity.destruct();
            });

    return ball_with_brick_collision_system;
}

void update_velocity_entities(flecs::world *world, const float /* frame_time */)
{
    world->each([](Position &position, const Velocity &velocity) {
        position.centre.x += velocity.values.x;
        position.centre.y += velocity.values.y;
    });
}

void render_position_entities(flecs::world *world)
{
    world->each([](const CircleComponent &circle, const Position &position) {
        DrawCircle(static_cast<int>(std::round(position.centre.x)),
                   static_cast<int>(std::round(position.centre.y)),
                   circle.radius,
                   circle.colour);
    });
    world->each(
        [](const RectangleComponent &rectangle, const Position &position) {
            // NOLINTBEGIN(readability-magic-numbers)
            DrawRectangle(static_cast<int>(std::round(position.centre.x) -
                                           0.5 * rectangle.width),
                          static_cast<int>(std::round(position.centre.y) -
                                           0.5 * rectangle.height),
                          static_cast<int>(std::round(rectangle.width)),
                          static_cast<int>(std::round(rectangle.height)),
                          rectangle.colour);
            // NOLINTEND(readability-magic-numbers)
        });
}

float top(const Position &position, const CollisionBox &collision_box)
{
    return position.centre.y - collision_box.half_height;
}

float right(const Position &position, const CollisionBox &collision_box)
{
    return position.centre.x + collision_box.half_width;
}

float bottom(const Position &position, const CollisionBox &collision_box)
{
    return position.centre.y + collision_box.half_height;
}

float left(const Position &position, const CollisionBox &collision_box)
{
    return position.centre.x - collision_box.half_width;
}

bool is_intersecting(const Position &position_1,
                     const CollisionBox &collision_box_1,
                     const Position &position_2,
                     const CollisionBox &collision_box_2)
{
    return right(position_1, collision_box_1) >=
               left(position_2, collision_box_2) &&
           left(position_1, collision_box_1) <=
               right(position_2, collision_box_2) &&
           bottom(position_1, collision_box_1) >=
               top(position_2, collision_box_2) &&
           top(position_1, collision_box_1) <=
               bottom(position_2, collision_box_2);
}
