#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "constants.h"

struct Ball
{
    Ball(float x_coordinate, float y_coordinate);

    void update();

    sf::CircleShape shape;
    sf::Vector2f velocity{-constants::kBallVelocity, -constants::kBallVelocity};
};
