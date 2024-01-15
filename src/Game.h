#include "Ball.h"
#include "Brick.h"
#include "Paddle.h"
#include "constants.h"

#include <SFML/Window.hpp>

#include <chrono>
#include <string>
#include <variant>

template <class T1, class T2>
bool is_intersecting(const T1 &mObjectA, const T2 &mObjectB);
void handle_collision(const Paddle &mPaddle, Ball &mBall);
void handle_collision(Brick &mBrick, Ball &mBall);


struct Game
{
    sf::RenderWindow window{{constants::kWindowWidth, constants::kWindowHeight},
                            "Arkanoid Clone"};
    FrameTime last_frame_time{0.F};
    FrameTime current_slice{0.F};
    bool running{false};

    Ball ball{static_cast<int>(constants::kWindowWidth / 2),
              static_cast<int>(constants::kWindowHeight / 2)};
    Paddle paddle{static_cast<int>(constants::kWindowWidth / 2),
                  constants::kWindowHeight - constants::kPaddleInsetBottom};
    std::vector<Brick> bricks;

    Game();

    void run();
    void input_phase();
    void update_phase();
    void draw_phase();
};
