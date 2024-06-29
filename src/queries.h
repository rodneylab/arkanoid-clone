#ifndef SRC_QUERIES_H
#define SRC_QUERIES_H

#include "resources.h"

#include <flecs.h>
#include <flecs/addons/cpp/mixins/query/impl.hpp>

const flecs::query<GameState> get_game_state_update_query(
    const flecs::world &world)
{
    return world.query_builder<GameState>().term_at(1).singleton().build();
}

#endif
