#include "Game.h"

int main()
{
    Game game{};
    game.CreateGameComponents();
    game.run();

    return 0;
}
