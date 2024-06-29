#include "systems.h"

#include "components.h"
#include "constants.h"
#include "resources.h"

#include <flecs/addons/cpp/flecs.hpp>
#include <flecs/addons/cpp/mixins/query/impl.hpp>
#include <flecs/addons/cpp/mixins/system/impl.hpp>
#include <flecs/addons/cpp/world.hpp>
#include <fmt/core.h>

#undef near
#undef far
#include <raylib.h>

#include <cmath>
#include <string>

void handle_game_state_input_system(
    const flecs::query<GameState> &game_state_update_query)
{
    game_state_update_query.each([](GameState &game_state) {
        if (game_state.game_mode == GameMode::TITLE && IsKeyDown(KEY_SPACE))
        {
            game_state.timer.reset();
            game_state.game_mode = GameMode::ROUND_TITLE;
        }
    });
}

void handle_round_title_playing_transition_system(
    const flecs::query<GameState> &game_state_update_query)
{
    game_state_update_query.each([](GameState &game_state) {
        constexpr float kWaitTime{3.F};
        if (game_state.timer.time > kWaitTime)
        {
            game_state.game_mode = GameMode::PLAYING;
        }
    });
}

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
            [ball, world](const Paddle /* paddle */,
                          const CollisionBox &collision_box,
                          const Position &position) {
                if (!is_intersecting(*ball->get<Position>(),
                                     *ball->get<CollisionBox>(),
                                     position,
                                     collision_box))
                {
                    return;
                }

                if (IsAudioDeviceReady())
                {
                    const Sound sound{world->lookup("BallPaddleCollisionSound")
                                          .get<Audible>()
                                          ->sound};
                    PlaySound(sound);
                }

                const float new_velocity_y{-ball->get<Velocity>()->values.y};

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
            [ball, world](flecs::entity entity,
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

                if (IsAudioDeviceReady())
                {
                    const Sound sound{world->lookup("BallBrickCollisionSound")
                                          .get<Audible>()
                                          ->sound};
                    PlaySound(sound);
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

void update_timer_system(const flecs::query<GameState> &game_state_update_query,
                         const float delta_time)
{
    game_state_update_query.each([&delta_time](GameState &game_state) {
        game_state.timer.increment(delta_time);
    });
}

void render_hud(const Font &hud_font)
{
    constexpr int kPlayerTextPositionX{50};
    constexpr int kTextPositionY{10};
    constexpr int kScoreTextOffsetX{33};
    constexpr int kScoreTextPositionY{27};
    constexpr float kTextFontSize{16.F};
    constexpr float kTextFontSpacing{1.F};
    DrawTextEx(hud_font,
               "1UP",
               Vector2{kPlayerTextPositionX, kTextPositionY},
               kTextFontSize,
               kTextFontSpacing,
               RED);
    DrawTextEx(
        hud_font,
        "00",
        Vector2{kPlayerTextPositionX + kScoreTextOffsetX, kScoreTextPositionY},
        kTextFontSize,
        kTextFontSpacing,
        WHITE);

    constexpr int kHiScoreTextPositionX{190};
    DrawTextEx(hud_font,
               "HIGH SCORE",
               Vector2{kHiScoreTextPositionX, kTextPositionY},
               kTextFontSize,
               kTextFontSpacing,
               RED);
    constexpr int kHiScoreTextOffsetX{51};
    DrawTextEx(hud_font,
               "50000",
               Vector2{kHiScoreTextPositionX + kHiScoreTextOffsetX,
                       kScoreTextPositionY},
               kTextFontSize,
               kTextFontSpacing,
               WHITE);
}

void render_instructions(const Font &hud_font)
{
    constexpr int kTextPositionY{300};
    constexpr int kFontSize{18};
    constexpr float kFontSpacing{1.F};
    const std::string text{"PRESS SPACE TO START"};
    const Vector2 text_measurements{
        MeasureTextEx(hud_font, text.data(), kFontSize, kFontSpacing)};

    DrawTextEx(hud_font,
               text.data(),
               Vector2{0.5F * static_cast<float>(constants::kWindowWidth -
                                                 text_measurements.x),
                       kTextPositionY},
               kFontSize,
               kFontSpacing,
               WHITE);
}

void render_title(const Font &title_font)
{
    constexpr int kTextPositionY{110};
    constexpr int kFontSize{72};
    constexpr float kFontSpacing{1.F};
    const std::string text{"ARKANOID"};
    const Vector2 text_measurements{
        MeasureTextEx(title_font, text.data(), kFontSize, kFontSpacing)};

    DrawTextEx(title_font,
               text.data(),
               Vector2{0.5F * static_cast<float>(constants::kWindowWidth -
                                                 text_measurements.x),
                       kTextPositionY},
               kFontSize,
               kFontSpacing,
               DARKBLUE);
}

void render_round_title(const flecs::query<const GameState> &game_state_query,
                        const Font &hud_font)
{
    game_state_query.each([&hud_font](const GameState &game_state) {
        constexpr int kFontSize{18};
        constexpr float kFontSpacing{1.F};
        const std::string text{fmt::format("ROUND {}", game_state.round)};
        const Vector2 text_measurements{
            MeasureTextEx(hud_font, text.data(), kFontSize, kFontSpacing)};

        DrawTextEx(hud_font,
                   text.data(),
                   Vector2{0.5F * static_cast<float>(constants::kWindowWidth -
                                                     text_measurements.x),
                           0.5F * static_cast<float>(constants::kWindowHeight -
                                                     text_measurements.y)},
                   kFontSize,
                   kFontSpacing,
                   WHITE);
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
            DrawRectangle(static_cast<int>(std::round(position.centre.x) -
                                           0.5 * rectangle.width),
                          static_cast<int>(std::round(position.centre.y) -
                                           0.5 * rectangle.height),
                          static_cast<int>(std::round(rectangle.width)),
                          static_cast<int>(std::round(rectangle.height)),
                          rectangle.colour);
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
