#include "components.h"
#include "constants.h"
#include "entities.h"
#include "systems.h"

#include <dbg.h>
#include <flecs.h>
#include <fmt/format.h>
#include <raylib.h>

#include <cmath>
#include <iostream>

int main()
{
    dbg(42, "hello world", false);

    flecs::world world;

    create_ball(&world);
    create_paddle(&world);
    create_bricks(&world);

    flecs::entity ball{world.lookup("Ball")};
    flecs::system paddle_movement_system{add_paddle_movement_system(&world)};
    flecs::system ball_with_wall_collision_system{
        add_ball_with_wall_collision_system(&world)};
    flecs::system ball_with_paddle_collision_system{
        add_ball_with_paddle_collision_system(&world, &ball)};
    flecs::system ball_with_brick_collision_system{
        add_ball_with_brick_collision_system(&world, &ball)};

    InitWindow(constants::kWindowWidth,
               constants::kWindowHeight,
               constants::kWindowTitle.data());
    SetTargetFPS(constants::kTargetFramerate);

    while (!WindowShouldClose())
    {
        const float frame_time{GetFrameTime()};

        // update
        update_velocity_entities(&world, frame_time);

        // handle paddle movement
        paddle_movement_system.run();

        // handle ball with wall collisions
        ball_with_wall_collision_system.run();
        ball_with_brick_collision_system.run();
        ball_with_paddle_collision_system.run();

        BeginDrawing();

        ClearBackground(BLACK);

        render_position_entities(&world);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
