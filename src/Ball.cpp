#include "Ball.h"

Ball::Ball(float x_coordinate, float y_coordinate)
{
    shape.setPosition(x_coordinate, y_coordinate);
    shape.setRadius(kBALL_RADIUS);
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin(kBALL_RADIUS, kBALL_RADIUS);
}
