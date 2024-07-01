#ifndef SRC_COMPONENTS_H
#define SRC_COMPONENTS_H

/*! \file src/components.h
 * \brief Declarations for ECS component classes and structs
 */

#include <raylib.h>

struct Audible
{
    Sound sound;
};

/**
 * \brief Ball ECS tag.
 */
struct Ball
{
};

/**
 * \brief Brick ECS tag.
 */
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

/**
 * \brief Class enum used to specify active side for axis-aligned one-way colliders
 */
enum class CollisionSide
{
    TOP,
    RIGHT,
    BOTTOM,
    LEFT
};

/**
 * \brief One-sided collider suitable for walls and platforms
 */
struct AxisAlignedOneWayCollider
{
    AxisAlignedOneWayCollider()
        : collision_side(CollisionSide::BOTTOM), displacement(0.F)
    {
    }

    /**
     * \brief Constructor initialisng collider with displacement and collision side values
     *
     * \param[in] displacement_value displacement of active collision side from left of window, for left and right colliders, displacement from top of window fortop and bottom colliders.
     * \param[in] collision_side_value either top, right, bottom or left depending on active collision side. For example,
     * LEFT indicates collider will only register collisions from an object moving right, hitting the left side
     */
    AxisAlignedOneWayCollider(const float displacement_value,
                              const CollisionSide collision_side_value)
        : collision_side(collision_side_value), displacement(displacement_value)
    {
    }

    // e.g LEFT indicates collider will only register collisions from an object moving right, hitting the left side
    CollisionSide collision_side;

    // for left or right collider, displacement of colliding side in pixels from left of window, for top or bottom collider, displacement of colliding side from top of screen
    float displacement;
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

/**
 * \brief Paddle ECS tag.
 */
struct Paddle
{
};

/**
 * \brief Wall ECS tag.
 */
struct Wall
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

    RectangleComponent(const float width_value,
                       const float height_value,
                       const Color colour_value)
        : width{width_value}, height{height_value}, colour{colour_value}
    {
    }

    RectangleComponent(const RectangleComponent &other) = default;
    RectangleComponent(RectangleComponent &&other) = default;
    RectangleComponent &operator=(const RectangleComponent &other) = default;
    RectangleComponent &operator=(RectangleComponent &&other) = default;
    ~RectangleComponent() = default;

    float width;
    float height;
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
