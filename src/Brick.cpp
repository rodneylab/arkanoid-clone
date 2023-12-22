#include "Brick.h"
#include "constants.h"

Brick::Brick(float mPositionX, float mPositionY)
{
    shape.setPosition(mPositionX, mPositionY);
    shape.setSize({constants::kBlockWidth, constants::kBlockHeight});
    shape.setFillColor(sf::Color::Yellow);
    shape.setOrigin(constants::kBlockWidth / static_cast<float>(2),
                    constants::kBlockHeight / static_cast<float>(2));
}

float Brick::x() const
{
    return shape.getPosition().x;
}

float Brick::y() const
{
    return shape.getPosition().y;
}

float Brick::left() const
{
    return x() - constants::kBlockWidth / static_cast<float>(2);
}

float Brick::right() const
{
    return x() + constants::kBlockWidth / static_cast<float>(2);
}
float Brick::top() const
{
    return y() - constants::kBlockHeight / static_cast<float>(2);
}
float Brick::bottom() const
{
    return y() + constants::kBlockHeight / static_cast<float>(2);
}
