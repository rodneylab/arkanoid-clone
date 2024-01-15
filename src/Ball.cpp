#include "Ball.h"

Ball::Ball(float x_coordinate, float y_coordinate)
    : velocity{-constants::kBallVelocity, -constants::kBallVelocity}
{
    shape.setPosition(x_coordinate, y_coordinate);
    shape.setRadius(constants::kBallRadius);
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin(constants::kBallRadius, constants::kBallRadius);
}

void Ball::update(FrameTime frame_time)
{
    shape.move(velocity * frame_time);

    if (left() < 0)
    {
        velocity.x = constants::kBallVelocity;
    }
    else if (right() > constants::kWindowWidth)
    {
        velocity.x = -constants::kBallVelocity;
    }

    if (top() < 0)
    {
        velocity.y = constants::kBallVelocity;
    }
    else if (bottom() > constants::kWindowHeight)
    {
        velocity.y = -constants::kBallVelocity;
    }
}

float Ball::x() const noexcept
{
    return shape.getPosition().x;
}

float Ball::y() const noexcept
{
    return shape.getPosition().y;
}

float Ball::left() const noexcept
{
    return x() - constants::kBallRadius;
}

float Ball::right() const noexcept
{
    return x() + constants::kBallRadius;
}

float Ball::top() const noexcept
{
    return y() - constants::kBallRadius;
}

float Ball::bottom() const noexcept
{
    return y() + constants::kBallRadius;
}
