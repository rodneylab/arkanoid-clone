#ifndef SRC_RECTANGLE_H
#define SRC_RECTANGLE_H

#include <SFML/Graphics.hpp>

struct Rectangle
{
    sf::RectangleShape shape;

    float x() const noexcept;
    float y() const noexcept;
    float left() const noexcept;
    float right() const noexcept;
    float top() const noexcept;
    float bottom() const noexcept;
};

#endif
