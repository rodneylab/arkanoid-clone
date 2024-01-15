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
