#include "components.h"
#include "constants.h"
#include "entities.h"
#include "systems.h"

#include <flecs.h>
#include <fmt/format.h>
#include <raylib.h>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>

#include <cmath>
#include <iostream>

int main()
{
    flecs::world world;

    // Seed the Raylib RNG used in the create_ball method
    SetRandomSeed((unsigned int)time(nullptr));
    create_ball(&world);
    create_paddle(&world);
    create_bricks(&world);

    InitWindow(constants::kWindowWidth,
               constants::kWindowHeight,
               constants::kWindowTitle.data());
    InitAudioDevice();
    //Sound paddle_ball_collision_sound =
    //  LoadSound(ASSETS_PATH "ArkanoidSFX6.wav");

    create_ball_with_paddle_collision_sound(&world);

    flecs::entity ball{world.lookup("Ball")};
    flecs::system paddle_movement_system{add_paddle_movement_system(&world)};
    flecs::system ball_with_wall_collision_system{
        add_ball_with_wall_collision_system(&world)};
    flecs::system ball_with_paddle_collision_system{
        add_ball_with_paddle_collision_system(&world, &ball)};
    flecs::system ball_with_brick_collision_system{
        add_ball_with_brick_collision_system(&world, &ball)};

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

    destroy_ball_with_paddle_collision_sound(&world);
    //UnloadSound(paddle_ball_collision_sound);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
