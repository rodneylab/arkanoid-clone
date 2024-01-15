#include "Paddle.h"
#include "constants.h"

Paddle::Paddle(float initial_x, float initial_y)
{
    shape.setPosition(initial_x, initial_y);
    shape.setSize({constants::kPaddleWidth, constants::kPaddleHeight});
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin(constants::kPaddleWidth / static_cast<float>(2),
                    constants::kPaddleHeight / static_cast<float>(2));
}
void Paddle::update(FrameTime frame_time)
{
    shape.move(velocity * frame_time);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && left() > 0)
    {
        velocity.x = -constants::kPaddleVelocity;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) &&
             right() < constants::kWindowWidth)
    {
        velocity.x = constants::kPaddleVelocity;
    }
    else
    {
        velocity.x = 0;
    }
}
