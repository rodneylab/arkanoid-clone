#include "constants.h"
#include "entities.h"
#include "queries.h"
#include "resources.h"
#include "systems.h"

#include <flecs/addons/cpp/flecs.hpp>
#include <flecs/addons/cpp/mixins/system/impl.hpp>
#include <flecs/addons/cpp/world.hpp>
#include <fmt/core.h>

#undef near
#undef far

#include <raylib.h>

#include <ctime>

/*
 * HUD font is pixelated to mimic font used in arcade games in the 1980s.  SDF
 * is used for HUD font to be able to scale it without default smoothing being
 * applied.  Smoothing reduces the pixelation, which is a design feature of the
 * font.
 */
void initialise_sdf_hud_font(Font *sdf_hud_font, Shader *shader)
{
    // Load font into memory
    int fileSize = 0;
    unsigned char *fileData =
        LoadFileData(ASSETS_PATH "press-start-2p-v15-latin-regular.ttf",
                     &fileSize);

    // Generate default font from TTF
    constexpr int kFontBaseSize{16};
    constexpr int kFontGlyphs{95};
    Font fontDefault{};
    fontDefault.baseSize = kFontBaseSize;
    fontDefault.glyphCount = kFontGlyphs;

    // Load font data from in-memory font
    fontDefault.glyphs = LoadFontData(fileData,
                                      fileSize,
                                      kFontBaseSize,
                                      nullptr,
                                      kFontGlyphs,
                                      FONT_DEFAULT);
    constexpr int kFontAtlasPadding{4};
    constexpr int kBasicPackingAlgorihm{0};
    Image atlas = GenImageFontAtlas(fontDefault.glyphs,
                                    &fontDefault.recs,
                                    kFontGlyphs,
                                    kFontBaseSize,
                                    kFontAtlasPadding,
                                    kBasicPackingAlgorihm);
    UnloadImage(atlas);

    // Generate SDF font from TTF font
    *sdf_hud_font = Font{};
    sdf_hud_font->baseSize = kFontBaseSize;
    sdf_hud_font->glyphCount = kFontGlyphs;
    constexpr int kFontPadding{0};
    sdf_hud_font->glyphs = LoadFontData(fileData,
                                        fileSize,
                                        kFontBaseSize,
                                        nullptr,
                                        kFontPadding,
                                        FONT_SDF);
    constexpr int kImageFontSize{10};
    constexpr int kSkylinePackingAlgorihm{1};
    atlas = GenImageFontAtlas(sdf_hud_font->glyphs,
                              &sdf_hud_font->recs,
                              kFontGlyphs,
                              kImageFontSize,
                              kFontPadding,
                              kSkylinePackingAlgorihm);
    sdf_hud_font->texture = LoadTextureFromImage(atlas);
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
}

int main()
{
    flecs::world world;
    world.set<GameState>({});

    // Seed the Raylib RNG used in the create_ball method
    SetRandomSeed((unsigned int)std::time(nullptr));
    create_ball(&world);
    create_paddle(&world);
    create_bricks(&world);

    InitWindow(constants::kWindowWidth,
               constants::kWindowHeight,
               constants::kWindowTitle.data());
    InitAudioDevice();

    Font sdf_hud_font{};
    Shader shader{};
    initialise_sdf_hud_font(&sdf_hud_font, &shader);

    const Font arkanoid_font{LoadFont(ASSETS_PATH "Arka_solid.ttf")};

    create_ball_with_brick_collision_sound(&world);
    create_ball_with_paddle_collision_sound(&world);

    flecs::entity ball{world.lookup("Ball")};
    const flecs::system paddle_movement_system{
        add_paddle_movement_system(&world)};
    const flecs::system ball_with_wall_collision_system{
        add_ball_with_wall_collision_system(&world)};
    const flecs::system ball_with_paddle_collision_system{
        add_ball_with_paddle_collision_system(&world, &ball)};
    const flecs::system ball_with_brick_collision_system{
        add_ball_with_brick_collision_system(&world, &ball)};

    const auto game_state_update_query = get_game_state_update_query(world);

    SetTargetFPS(constants::kTargetFramerate);
    SetTextureFilter(sdf_hud_font.texture, TEXTURE_FILTER_POINT);

    const GameState *game_state{world.get<GameState>()};

    while (!WindowShouldClose())
    {
        const float frame_time{GetFrameTime()};

        handle_game_state_input_system(game_state_update_query);

        switch (game_state->game_mode)
        {
        case GameMode::TITLE:
            BeginDrawing();

            ClearBackground(BLACK);

            BeginShaderMode(shader);
            render_hud(sdf_hud_font);
            render_instructions(sdf_hud_font);
            EndShaderMode();

            render_title(arkanoid_font);

            EndDrawing();
            break;

        case GameMode::PLAYING:
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
            break;
        }
    }

    destroy_ball_with_brick_collision_sound(&world);
    destroy_ball_with_paddle_collision_sound(&world);

    UnloadFont(arkanoid_font);
    UnloadFont(sdf_hud_font);

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
