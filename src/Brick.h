#include "Rectangle.h"
#include "constants.h"

#include <SFML/Graphics.hpp>

struct Brick : public Rectangle
{
public:
    bool destroyed{false};

    Brick(float mPositionX, float mPositionY);
};
