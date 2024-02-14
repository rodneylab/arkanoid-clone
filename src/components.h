#ifndef SRC_COMPONENTS_H
#define SRC_COMPONENTS_H

#include <raylib.h>

struct Ball
{
};

struct Brick
{
};

struct CircleComponent
{
    CircleComponent() : radius{}, colour{BLACK}
    {
    }

    CircleComponent(const float radius_value, const Color colour_value)
        : radius(radius_value), colour(colour_value)
    {
    }

    float radius;
    Color colour;
};

struct CollisionBox
{
    CollisionBox() : half_width{}, half_height{}
    {
    }

    CollisionBox(const float half_width_value, const float half_height_value)
        : half_width{half_width_value}, half_height{half_height_value}
    {
    }

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

    Position(const float centre_x, const float centre_y) : centre{}
    {
        centre.x = centre_x;
        centre.y = centre_y;
    }

    Vector2 centre;
};

struct RectangleComponent
{
    RectangleComponent() : width{}, height{}, colour{BLACK}
    {
    }

    RectangleComponent(const int width_value,
                       const int height_value,
                       const Color colour_value)
        : width{width_value}, height{height_value}, colour{colour_value}
    {
    }

    int width;
    int height;
    Color colour;
};

struct Velocity
{
    Velocity() : values{}
    {
    }

    explicit Velocity(const Vector2 &other_values) : values{other_values}
    {
    }

    Velocity(const float velocity_x, const float velocity_y) : values{}
    {
        values.x = velocity_x;
        values.y = velocity_y;
    }

    Vector2 values;
};

#endif
