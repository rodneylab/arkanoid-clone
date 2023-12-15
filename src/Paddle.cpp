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
void Paddle::update()
{
    shape.move(velocity);

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

float Paddle::x() const
{
    return shape.getPosition().x;
}

float Paddle::y() const
{
    return shape.getPosition().y;
}

float Paddle::left() const
{
    return x() - (shape.getSize().x / static_cast<float>(2));
}

float Paddle::right() const
{
    return x() + (shape.getSize().x / static_cast<float>(2));
}

float Paddle::top() const
{
    return y() - (shape.getSize().y / static_cast<float>(2));
}

float Paddle::bottom() const
{
    return y() + (shape.getSize().y / static_cast<float>(2));
}
