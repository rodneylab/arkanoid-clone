#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

constexpr float kBALL_RADIUS{10.F};

struct Ball
{
    Ball(float x_coordinate, float y_coordinate);

    sf::CircleShape shape;
};
