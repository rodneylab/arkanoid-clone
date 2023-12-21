#include <SFML/Graphics.hpp>

#include "constants.h"

struct Brick
{
public:
    Brick(float mPositionX, float mPositionY);

    float x() const;
    float y() const;
    float left() const;
    float right() const;
    float top() const;
    float bottom() const;

    bool destroyed{false};
    sf::RectangleShape shape;
};
