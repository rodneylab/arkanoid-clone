#ifndef SRC_COMPONENTS_H
#define SRC_COMPONENTS_H

#include <raylib.h>

struct Brick
{
};

struct CircleComponent
{
    Color colour;
    float radius;
};

struct CollisionBox
{
    float half_width;
    float half_height;
};

struct Paddle
{
};

struct Position
{
    Position() : centre{}
    {
    }

    explicit Position(const Vector2 &position_vector) : centre{position_vector}
    {
    }

    Vector2 centre;
};

struct RectangleComponent
{
    int width;
    int height;
    Color colour;
};

struct Velocity
{
    Velocity() : values{.x = 0.F, .y = 0.F}
    {
    }

    explicit Velocity(const Vector2 &other_values) : values{other_values}
    {
    }

    Vector2 values;
};

#endif
