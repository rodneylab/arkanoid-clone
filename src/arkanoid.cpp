#include "Ball.h"
#include "Brick.h"
#include "Paddle.h"

#include <SFML/Window.hpp>

#include <variant>

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

void handle_collision(Brick &mBrick, Ball &mBall)
{
    if (!is_intersecting(mBall, mBrick))
    {
        return;
    }

    mBrick.destroyed = true;

    const float overlap_left{mBall.right() - mBrick.left()};
    const float overlap_right{mBrick.right() - mBall.left()};
    const float overlap_top{mBall.bottom() - mBrick.top()};
    const float overlap_bottom{mBrick.bottom() - mBall.top()};

    const bool ball_from_left(std::abs(overlap_left) < std::abs(overlap_right));
    const bool ball_from_top(std::abs(overlap_top) < std::abs(overlap_bottom));

    const float min_overlap_x{ball_from_left ? overlap_left : overlap_right};
    const float min_overlap_y{ball_from_top ? overlap_top : overlap_bottom};

    if (std::abs(min_overlap_x) < std::abs(min_overlap_y))
    {
        mBall.velocity.x = ball_from_left ? -constants::kBallVelocity
                                          : constants::kBallVelocity;
    }
    else
    {
        mBall.velocity.y = ball_from_top ? -constants::kBallVelocity
                                         : constants::kBallVelocity;
    }
}

int main()
{
    Ball ball{static_cast<int>(constants::kWindowWidth / 2),
              static_cast<int>(constants::kWindowHeight / 2)};
    Paddle paddle{static_cast<int>(constants::kWindowWidth / 2),
                  constants::kWindowHeight - constants::kPaddleInsetBottom};

    std::vector<Brick> bricks;
    for (int iX{0}; iX < constants::kCountBlocksX; ++iX)
    {
        for (int iY{0}; iY < constants::kCountBlocksY; ++iY)
        {
            bricks.emplace_back(
                static_cast<float>(iX + 1) * (constants::kBlockWidth + 3) +
                    constants::kBlocksInsetX,
                static_cast<float>(iY + 2) * (constants::kBlockHeight + 3));
        }
    }

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
        for (auto &brick : bricks)
        {
            handle_collision(brick, ball);
        }

        bricks.erase(
            // remove_if moves satisfying elements to the end of the vector and
            // returns an iterator to the first (moved) satisfying element
            remove_if(begin(bricks),
                      end(bricks),
                      [](const Brick &mBrick) { return mBrick.destroyed; }),
            end(bricks));

        window.draw(ball.shape);
        window.draw(paddle.shape);
        for (auto &brick : bricks)
        {
            window.draw(brick.shape);
        }
        window.display();
    }
    return 0;
}
