#include <SFML/Graphics.hpp>

struct Paddle
{
    Paddle(float initial_x, float initial_y);
    void update();
    float x() const;
    float y() const;
    float left() const;
    float right() const;
    float top() const;
    float bottom() const;

    sf::RectangleShape shape;
    sf::Vector2f velocity;
};
