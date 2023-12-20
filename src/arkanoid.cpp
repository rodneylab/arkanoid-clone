#include <SFML/Window.hpp>

#include <variant>

#include "Ball.h"
#include "Paddle.h"

template <class T1, class T2>
bool is_intersecting(const T1 &mObjectA, const T2 &mObjectB)
{
    return mObjectA.right() >= mObjectB.left() &&
           mObjectA.left() <= mObjectB.right() &&
           mObjectA.bottom() >= mObjectB.top() &&
           mObjectA.top() <= mObjectB.bottom();
}

void handle_collision(const Paddle &mPaddle, Ball &mBall)
{
    if (!is_intersecting(mPaddle, mBall))
    {
        return;
    }

    mBall.velocity.y = -constants::kBallVelocity;

    if (mBall.x() < mPaddle.x())
    {
        mBall.velocity.x = -constants::kBallVelocity;
    }
    else
    {
        mBall.velocity.x = constants::kBallVelocity;
    }
}

int main()
{
    Ball ball{static_cast<int>(constants::kWindowWidth / 2),
              static_cast<int>(constants::kWindowHeight / 2)};
    Paddle paddle{static_cast<int>(constants::kWindowWidth / 2),
                  constants::kWindowHeight - constants::kPaddleInsetBottom};
    sf::RenderWindow window{{constants::kWindowWidth, constants::kWindowHeight},
                            "Arkanoid Clone"};
    window.setFramerateLimit(constants::kFramerateLimit);

    while (window.isOpen())
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::KeyPressed:
                // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
                if (event.key.scancode == sf::Keyboard::Scan::Escape)
                {
                    window.close();
                }
                break;

            default:
                break;
            }
        }


        window.clear(sf::Color::Black);

        ball.update();
        paddle.update();

        handle_collision(paddle, ball);

        window.draw(ball.shape);
        window.draw(paddle.shape);
        window.display();
    }
    return 0;
}
