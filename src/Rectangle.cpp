#include "Rectangle.h"

float Rectangle::x() const noexcept
{
    return shape.getPosition().x;
}

float Rectangle::y() const noexcept
{
    return shape.getPosition().y;
}

float Rectangle::left() const noexcept
{
    return x() - shape.getSize().x / static_cast<float>(2);
}

float Rectangle::right() const noexcept
{
    return x() + shape.getSize().x / static_cast<float>(2);
}

float Rectangle::top() const noexcept
{
    return y() - shape.getSize().y / static_cast<float>(2);
}

float Rectangle::bottom() const noexcept
{
    return y() + shape.getSize().y / static_cast<float>(2);
}
