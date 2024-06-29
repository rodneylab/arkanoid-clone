#ifndef SRC_RESOURCES_H
#define SRC_RESOURCES_H

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
};

#endif
