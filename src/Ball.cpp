#include "Ball.h"

Ball::Ball(float x_coordinate, float y_coordinate)
{
    shape.setPosition(x_coordinate, y_coordinate);
    shape.setRadius(constants::kBallRadius);
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin(constants::kBallRadius, constants::kBallRadius);
}

void Ball::update()
{
    shape.move(velocity);
}
