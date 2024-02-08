#ifndef SRC_GAME_H
#define SRC_GAME_H

#include "Entity.h"
#include "Manager.h"
#include "constants.h"
#include "utility.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <chrono>
#include <string>
#include <variant>

using FrameTime = float;

struct Game
{
    enum ArkanoidGroup : std::size_t
    {
        GPaddle,
        GBrick,
        GBall
    };

    sf::RenderWindow window{{constants::kWindowWidth, constants::kWindowHeight},
                            "Arkanoid Clone"};
    FrameTime last_frame_time{0.F};
    FrameTime current_slice{0.F};
    bool running{false};
    Manager _manager;

    Entity &create_ball();
    Entity &create_paddle();
    Entity &create_brick(const sf::Vector2f &position);

    Game();
    void CreateGameComponents();

    void run();
    void input_phase();
    void update_phase();
    void draw_phase();
    void render(const sf::Drawable &drawable);
};

#endif
