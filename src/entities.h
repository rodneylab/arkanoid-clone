#ifndef SRC_ENTITIES_H
#define SRC_ENTITIES_H

#include <flecs.h>
#include <raylib.h>

void create_ball(flecs::world *world);
void create_bricks(flecs::world *world);
void create_paddle(flecs::world *world);
void create_ball_with_brick_collision_sound(flecs::world *world);
void destroy_ball_with_brick_collision_sound(flecs::world *world);
void create_ball_with_paddle_collision_sound(flecs::world *world);
void destroy_ball_with_paddle_collision_sound(flecs::world *world);

#endif
