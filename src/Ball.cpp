#include "Ball.h"

Ball::Ball(float x_coordinate, float y_coordinate)
    : velocity{-constants::kBallVelocity, -constants::kBallVelocity}
{
    shape.setPosition(x_coordinate, y_coordinate);
    shape.setRadius(constants::kBallRadius);
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin(constants::kBallRadius, constants::kBallRadius);
}

void Ball::update()
{
    shape.move(velocity);

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

float Ball::x() const
{
    return shape.getPosition().x;
}

float Ball::y() const
{
    return shape.getPosition().y;
}

float Ball::left() const
{
    return x() - constants::kBallRadius;
}

float Ball::right() const
{
    return x() + constants::kBallRadius;
}

float Ball::top() const
{
    return y() - constants::kBallRadius;
}

float Ball::bottom() const
{

    return y() + constants::kBallRadius;
}
