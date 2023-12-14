#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "constants.h"

struct Ball
{
    Ball(float x_coordinate, float y_coordinate);

    void update();

    float x() const;
    float y() const;
    float left() const;
    float right() const;
    float top() const;
    float bottom() const;

    sf::CircleShape shape;
    sf::Vector2f velocity;
};
