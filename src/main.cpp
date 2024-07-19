#include "constants.h"
#include "entities.h"
#include "queries.h"
#include "resources.h"
#include "systems.h"

#include <flecs/addons/cpp/flecs.hpp>
#include <flecs/addons/cpp/mixins/system/impl.hpp>
#include <flecs/addons/cpp/world.hpp>

// Windows workarounds for CloseWindow / ShowCursor errors

#if defined(_WIN32)
#define NOGDI  // All GDI defines and routines
#define NOUSER // All USER defines and routines
#endif

#include <fmt/core.h>
#include <spdlog/cfg/env.h>

#undef near
#undef far

#include <raylib.h>

#include <ctime>
#include <string_view>

void initialise_sdf_font(const std::string_view &font_path,
                         Font *sdf_font,
                         Shader *shader)
{
    // Load font into memory
    int fileSize{0};
    unsigned char *fileData = LoadFileData(font_path.data(), &fileSize);

    // Generate default font from TTF
    constexpr int kFontBaseSize{27};
    constexpr int kFontGlyphs{95};

    // Generate SDF font from TTF font
    *sdf_font = Font{};
    sdf_font->baseSize = kFontBaseSize;
    sdf_font->glyphCount = kFontGlyphs;
    constexpr int kFontPadding{0};
    sdf_font->glyphs = LoadFontData(fileData,
                                    fileSize,
                                    kFontBaseSize,
                                    nullptr,
                                    kFontPadding,
                                    FONT_SDF);
    constexpr int kImageFontSize{10};
    constexpr int kSkylinePackingAlgorithm{1};
    const Image atlas = GenImageFontAtlas(sdf_font->glyphs,
                                          &sdf_font->recs,
                                          kFontGlyphs,
                                          kImageFontSize,
                                          kFontPadding,
                                          kSkylinePackingAlgorithm);
    sdf_font->texture = LoadTextureFromImage(atlas);
    UnloadImage(atlas);

    // Free memory
    UnloadFileData(fileData);

    // Load shader for SDF
#if defined(PLATFORM_DESKTOP)
    constexpr int kGLSLVersion{330};
#else // PLATFORM_ANDROID, PLATFORM_WEB
    constexpr int kGLSLVersion{100};
#endif
    *shader = Shader{LoadShader(
        nullptr,
        TextFormat( // NOLINT [cppcoreguidelines-pro-type-vararg]
            ASSETS_PATH "%s",
            fmt::format("shaders/glsl{}/sdf.fs", kGLSLVersion).c_str()))};

    SetTextureFilter(sdf_font->texture, TEXTURE_FILTER_BILINEAR);
}

int main()
{
    spdlog::cfg::
        load_env_levels(); // use `export SPDLOG_LEVEL=trace` to set log level from command line

    flecs::world world;
    world.set<GameState>({});

    // Seed the Raylib RNG used in the create_ball method
    SetRandomSeed((unsigned int)std::time(nullptr));
    create_ball(&world);
    create_paddle(&world);
    create_bricks(&world);
    create_walls(&world);

    InitWindow(constants::kWindowWidth,
               constants::kWindowHeight,
               constants::kWindowTitle.data());
    InitAudioDevice();

    Font sdf_instruction_font{};
    Shader shader{};
    initialise_sdf_font(
        std::string_view{ASSETS_PATH "jetbrains-mono-v18-latin-regular.ttf"},
        &sdf_instruction_font,
        &shader);

    const Font hud_font{
        LoadFont(ASSETS_PATH "press-start-2p-v15-latin-regular.ttf")};

    constexpr int kFontBaseSize{27};
    const Font instruction_font{
        LoadFontEx(ASSETS_PATH "jetbrains-mono-v18-latin-regular.ttf",
                   kFontBaseSize,
                   nullptr,
                   0)};

    constexpr int kArkanoidFontBaseSize{72};
    const Font arkanoid_font{LoadFontEx(ASSETS_PATH "Arka_solid.ttf",
                                        kArkanoidFontBaseSize,
                                        nullptr,
                                        0)};

    create_ball_with_brick_collision_sound(&world);
    create_ball_with_paddle_collision_sound(&world);

    flecs::entity ball{world.lookup("Ball")};

    const auto game_state_query = get_game_state_query(world);
    const auto game_state_update_query = get_game_state_update_query(world);
    const auto velocity_entity_position_update_query =
        get_velocity_entity_position_update_query(world);
    const auto wall_collider_query = get_wall_collider_query(world);

    const flecs::system paddle_movement_system{
        add_paddle_movement_system(&world)};
    const flecs::system ball_with_wall_collision_system{
        add_ball_with_wall_collision_system(&world, wall_collider_query)};
    const flecs::system ball_with_paddle_collision_system{
        add_ball_with_paddle_collision_system(&world, &ball)};
    const flecs::system ball_with_brick_collision_system{
        add_ball_with_brick_collision_system(game_state_update_query,
                                             &world,
                                             &ball)};
    const flecs::system render_circle_position_entities_system{
        add_render_circle_position_entities_system(&world)};
    const flecs::system render_rectangle_position_entities_system{
        add_render_rectangle_position_entities_system(&world)};

    SetTargetFPS(constants::kTargetFramerate);

    const GameState *game_state{world.get<GameState>()};

    double previous_time{GetTime()};
    double current_time{0.0};
    double update_draw_time{0.0};
    float delta_time{0.F};

    while (!WindowShouldClose())
    {
        const float frame_time{GetFrameTime()};
        update_timer_system(game_state_update_query, delta_time);

        handle_game_state_input_system(game_state_update_query);

        switch (game_state->game_mode)
        {
        case GameMode::TITLE:
            BeginDrawing();

            ClearBackground(BLACK);

            BeginShaderMode(shader);
            render_hud(game_state_query, hud_font);
            render_instructions(sdf_instruction_font);
            EndShaderMode();

            render_title(arkanoid_font);

            EndDrawing();
            break;

        case GameMode::ROUND_TITLE:
            handle_round_title_playing_transition_system(
                game_state_update_query);
            BeginDrawing();

            ClearBackground(BLACK);

            BeginShaderMode(shader);
            render_hud(game_state_query, hud_font);
            render_round_title(game_state_query, hud_font);
            EndShaderMode();

            EndDrawing();

            break;

        case GameMode::PLAYING:
            // update
            update_velocity_entities(velocity_entity_position_update_query,
                                     frame_time);

            // handle paddle movement
            paddle_movement_system.run();

            // handle ball with wall collisions
            ball_with_wall_collision_system.run();
            ball_with_brick_collision_system.run();
            ball_with_paddle_collision_system.run();

            BeginDrawing();

            ClearBackground(BLACK);

            render_circle_position_entities_system.run();
            render_rectangle_position_entities_system.run();

            BeginShaderMode(shader);
            render_side_panel(game_state_query, hud_font);
            EndShaderMode();

            EndDrawing();
            break;
        }

        current_time = GetTime();
        update_draw_time = current_time - previous_time;
        delta_time = static_cast<float>(update_draw_time);
        previous_time = current_time;
    }

    destroy_ball_with_brick_collision_sound(&world);
    destroy_ball_with_paddle_collision_sound(&world);

    UnloadFont(arkanoid_font);
    UnloadFont(hud_font);
    UnloadFont(instruction_font);
    UnloadFont(sdf_instruction_font);

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
