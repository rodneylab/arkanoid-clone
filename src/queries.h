#ifndef SRC_QUERIES_H
#define SRC_QUERIES_H

#include "components.h"
#include "resources.h"

#include <flecs.h>
#include <flecs/addons/cpp/mixins/query/impl.hpp>

flecs::query<const GameState> get_game_state_query(const flecs::world &world)
{
    return world.query_builder<const GameState>()
        .term_at(1)
        .singleton()
        .cache_kind(flecs::QueryCacheAuto)
        .build();
}

flecs::query<GameState> get_game_state_update_query(const flecs::world &world)
{
    return world.query_builder<GameState>().term_at(1).singleton().build();
}

flecs::query<Position, const Velocity>
get_velocity_entity_position_update_query(const flecs::world &world)
{
    return world.query_builder<Position, const Velocity>()
        .cache_kind(flecs::QueryCacheAuto)
        .build();
}

flecs::query<const Wall, const AxisAlignedOneWayCollider>
get_wall_collider_query(const flecs::world &world)
{
    return world.query_builder<const Wall, const AxisAlignedOneWayCollider>()
        .build();
}

#endif
