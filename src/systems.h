#ifndef SRC_SYSTEMS_H
#define SRC_SYSTEMS_H

/*! \file src/systems.h
 * \brief Declarations for ECS system functions
 */

#include "components.h"
#include "resources.h"

#include <flecs.h>

flecs::system add_paddle_movement_system(flecs::world *world);
flecs::system add_ball_with_wall_collision_system(
    flecs::world *world,
    const flecs::query<const Wall, const AxisAlignedOneWayCollider>
        &wall_collider_query);
flecs::system add_ball_with_paddle_collision_system(flecs::world *world,
                                                    flecs::entity *ball);
flecs::system add_ball_with_brick_collision_system(flecs::world *world,
                                                   flecs::entity *ball);

void update_timer_system(const flecs::query<GameState> &game_state_update_query,
                         float delta_time);

void handle_game_state_input_system(
    const flecs::query<GameState> &game_state_update_query);
void handle_round_title_playing_transition_system(
    const flecs::query<GameState> &game_state_update_query);
void update_velocity_entities(flecs::world *world, float frame_time);
void render_hud(const flecs::query<const GameState> &game_state_query,
                const Font &hud_font);
void render_instructions(const Font &hud_font);
void render_position_entities(flecs::world *world);
void render_title(const Font &title_font);
void render_round_title(const flecs::query<const GameState> &game_state_query,
                        const Font &hud_font);
void render_side_panel(const flecs::query<const GameState> &game_state_query,
                       const Font &hud_font);

float top(const Position &position, const CollisionBox &collision_box);
float right(const Position &position, const CollisionBox &collision_box);
float bottom(const Position &position, const CollisionBox &collision_box);
float left(const Position &position, const CollisionBox &collision_box);

bool is_intersecting(const Position &position_1,
                     const CollisionBox &collision_box_1,
                     const Position &position_2,
                     const CollisionBox &collision_box_2);

#endif
