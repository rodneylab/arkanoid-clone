#include "Rectangle.h"
#include "constants.h"

#include <SFML/Graphics.hpp>

struct Paddle : public Rectangle
{
    sf::Vector2f velocity;

    Paddle(float initial_x, float initial_y);
    void update(FrameTime frame_time);
};
