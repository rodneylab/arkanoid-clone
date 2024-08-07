#ifndef SRC_RESOURCES_H
#define SRC_RESOURCES_H

#include "constants.h"
#include "file_system.h"

#include <map>

enum class GameMode
{
    TITLE,
    ROUND_TITLE,
    PLAYING
};

struct Timer
{
    float time{0.F};

    void reset()
    {
        time = 0.F;
    }

    void increment(const float delta)
    {
        time += delta;
    }
};

struct GameState
{
    GameMode game_mode{GameMode::TITLE};
    int round{1};
    Timer timer{};
    int score{0};
    int high_score{constants::kDefaultHighScore};
};

#endif
