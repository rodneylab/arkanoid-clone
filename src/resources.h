#ifndef SRC_RESOURCES_H
#define SRC_RESOURCES_H

enum class GameMode
{
    TITLE,
    PLAYING
};

struct GameState
{
    GameMode game_mode{GameMode::TITLE};
};

#endif
