#include "Game.h"

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

Game::Game()
{
    window.setFramerateLimit(constants::kFramerateLimit);

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
}

void Game::run()
{
    running = true;
    while (window.isOpen())
    {
        auto time_point1(std::chrono::high_resolution_clock::now());

        window.clear(sf::Color::Black);
        input_phase();
        update_phase();
        draw_phase();

        auto time_point2(std::chrono::high_resolution_clock::now());
        auto elapsed_time(time_point2 - time_point1);
        const FrameTime frame_time{
            std::chrono::duration_cast<
                std::chrono::duration<float, std::milli>>(elapsed_time)
                .count()};
        last_frame_time = frame_time;
        constexpr float kMillisecondsPerSecond{1000.F};
        auto frame_time_seconds(frame_time / kMillisecondsPerSecond);
        auto fps(1.F / frame_time_seconds);

        window.setTitle("FT: " + std::to_string(frame_time) +
                        "\tFPS: " + std::to_string(fps));
    }
}

void Game::input_phase()
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
}

void Game::update_phase()
{
    current_slice += last_frame_time;

    for (; current_slice >= constants::kFrameTimeSlice;
         current_slice -= constants::kFrameTimeSlice)
    {
        ball.update(constants::kFrameTimestep);
        paddle.update(constants::kFrameTimestep);

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
    }
}

void Game::draw_phase()
{
    window.draw(ball.shape);
    window.draw(paddle.shape);
    for (auto &brick : bricks)
    {
        window.draw(brick.shape);
    }
    window.display();
}
