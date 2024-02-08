#ifndef SRC_COMPONENT_H
#define SRC_COMPONENT_H

#include "Entity.h"
#include "constants.h"
#include "utility.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

struct Component;
class Entity;
class Game;

using ComponentArray = std::array<Component *, kMaxComponents>;

template <typename T>
inline ComponentID get_component_type_id() noexcept
{
    static_assert(std::is_base_of<Component, T>::value,
                  "T must inherit from Component");

    static ComponentID const type_id{internal::get_unique_component_id()};

    return type_id;
}

struct Component
{
    Entity *entity{nullptr};

    virtual void init();
    virtual void update(float frame_time);
    virtual void draw();

    Component() = default;
    Component(const Component &other);
    Component(const Component &&other) noexcept;
    Component &operator=(const Component &other);
    Component &operator=(Component &&other) noexcept;
    virtual ~Component() = default;
};

struct PositionComponent : Component
{
    sf::Vector2f _position;

    PositionComponent() = default;
    explicit PositionComponent(const sf::Vector2f &position);

    [[nodiscard]] float x() const noexcept;
    [[nodiscard]] float y() const noexcept;
};


struct PhysicsComponent : Component
{
    PositionComponent *_position{nullptr};
    sf::Vector2f _velocity;
    sf::Vector2f _half_size;

    std::function<void(const sf::Vector2f &)> on_out_of_bounds;

    explicit PhysicsComponent(const sf::Vector2f &half_size);

    void init() override;
    void update(float frame_time) override;

    [[nodiscard]] float x() const noexcept;
    [[nodiscard]] float y() const noexcept;
    [[nodiscard]] float left() const noexcept;
    [[nodiscard]] float right() const noexcept;
    [[nodiscard]] float top() const noexcept;
    [[nodiscard]] float bottom() const noexcept;
};


struct CircleComponent : Component
{
    Game *_game{nullptr};
    PositionComponent *_position{nullptr};
    sf::CircleShape _shape;
    float _radius;

    CircleComponent(Game *game, float radius);
    CircleComponent(const CircleComponent &other) = default;
    ~CircleComponent() override = default;
    void init() override;
    void update(float frame_time) override;
    void draw() override;
};

struct RectangleComponent : Component
{
    Game *_game{nullptr};
    PositionComponent *_position{nullptr};
    sf::RectangleShape _shape;
    sf::Color _colour;
    sf::Vector2f _size;

    RectangleComponent(Game *game, const sf::Vector2f &half_size);
    RectangleComponent(Game *game,
                       sf::Color colour,
                       const sf::Vector2f &half_size);

    void init() override;
    void update(float frame_time) override;
    void draw() override;
};

struct PaddleControlComponent : Component
{
    PhysicsComponent *_physics_component{nullptr};

    void init() override;
    void update(FrameTime frame_time) override;
};

template <typename T1, typename T2>
bool is_intersecting(const T1 &object_a, const T2 &object_b) noexcept;

void handle_paddle_ball_collision(const Entity &paddle, Entity &ball) noexcept;
void handle_brick_ball_collision(Entity &brick, Entity &ball);

#endif
