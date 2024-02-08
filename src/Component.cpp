#include "Component.h"
#include "Game.h"
#include "constants.h"

void Component::init()
{
}

void Component::update(float frame_time)
{
}

void Component::draw()
{
}

PositionComponent::PositionComponent(const sf::Vector2f &position)
    : _position(position)
{
}

float PositionComponent::x() const noexcept
{
    return _position.x;
}

float PositionComponent::y() const noexcept
{
    return _position.y;
}

PhysicsComponent::PhysicsComponent(const sf::Vector2f &half_size)
    : _half_size(half_size), _velocity({0.F, 0.F})
{
}

void PhysicsComponent::init()
{
    _position = &entity->get_component<PositionComponent>();
}

void PhysicsComponent::update(float frame_time)
{
    _position->_position += _velocity * frame_time;

    if (on_out_of_bounds == nullptr)
    {
        return;
    }

    if (left() < 0)
    {
        on_out_of_bounds(sf::Vector2f{1.F, 0.F});
    }
    else if (right() > constants::kWindowWidth)
    {
        on_out_of_bounds(sf::Vector2f{-1.F, 0.F});
    }

    if (top() < 0)
    {
        on_out_of_bounds(sf::Vector2f{0.F, 1.F});
    }
    else if (bottom() > constants::kWindowHeight)
    {
        on_out_of_bounds(sf::Vector2f{0.F, -1.F});
    }
}

[[nodiscard]] float PhysicsComponent::x() const noexcept
{
    return _position->x();
}
[[nodiscard]] float PhysicsComponent::y() const noexcept
{
    return _position->y();
}

[[nodiscard]] float PhysicsComponent::left() const noexcept
{
    return x() - _half_size.x;
}

[[nodiscard]] float PhysicsComponent::right() const noexcept
{
    return x() + _half_size.x;
}
[[nodiscard]] float PhysicsComponent::top() const noexcept
{
    return y() - _half_size.y;
}

[[nodiscard]] float PhysicsComponent::bottom() const noexcept
{
    return y() + _half_size.y;
}

CircleComponent::CircleComponent(Game *game, float radius)
    : _game(game), _radius(radius)
{
}

//CircleComponent::CircleComponent(const CircleComponent &other) = default;


void CircleComponent::init()
{
    _position = &entity->get_component<PositionComponent>();

    _shape.setRadius(_radius);
    _shape.setFillColor(sf::Color::Red);
    _shape.setOrigin(_radius, _radius);
}

void CircleComponent::update(float frame_time)
{
    _shape.setPosition(_position->_position);
}

void CircleComponent::draw()
{
    _game->render(_shape);
}

RectangleComponent::RectangleComponent(Game *game,
                                       const sf::Vector2f &half_size)
    : RectangleComponent(game, sf::Color::Red, half_size)
{
}

RectangleComponent::RectangleComponent(Game *game,
                                       sf::Color colour,
                                       const sf::Vector2f &half_size)
    : _game(game), _colour(colour), _size{half_size * static_cast<float>(2)}
{
}

void RectangleComponent::init()
{
    _position = &entity->get_component<PositionComponent>();

    _shape.setSize(_size);
    _shape.setFillColor(_colour);
    _shape.setOrigin(_size.x / static_cast<float>(2),
                     _size.y / static_cast<float>(2));
}

void RectangleComponent::update(float frame_time)
{
    _shape.setPosition(_position->_position);
}

void RectangleComponent::draw()
{
    _game->render(_shape);
}

void PaddleControlComponent::init()
{
    _physics_component = &entity->get_component<PhysicsComponent>();
}

void PaddleControlComponent::update(FrameTime frame_time)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) &&
        _physics_component->left() > 0)
    {
        _physics_component->_velocity.x = -constants::kPaddleVelocity;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) &&
             _physics_component->right() < constants::kWindowWidth)
    {
        _physics_component->_velocity.x = constants::kPaddleVelocity;
    }
    else
    {
        _physics_component->_velocity.x = 0.0;
    }
}

void handle_paddle_ball_collision(const Entity &paddle, Entity &ball) noexcept
{
    auto &paddle_component(paddle.get_component<PhysicsComponent>());
    auto &ball_component(ball.get_component<PhysicsComponent>());

    if (!is_intersecting(paddle_component, ball_component))
    {
        return;
    }

    ball_component._velocity.y = -constants::kBallVelocity;

    if (ball_component.x() < paddle_component.x())
    {
        ball_component._velocity.x = -constants::kBallVelocity;
    }
    else
    {
        ball_component._velocity.x = constants::kBallVelocity;
    }
}

void handle_brick_ball_collision(Entity &brick, Entity &ball)
{
    auto &brick_physics_component(brick.get_component<PhysicsComponent>());
    auto &ball_physics_component(ball.get_component<PhysicsComponent>());


    if (!is_intersecting(ball_physics_component, brick_physics_component))
    {
        return;
    }

    brick.destroy();

    const float overlap_left{ball_physics_component.right() -
                             brick_physics_component.left()};
    const float overlap_right{brick_physics_component.right() -
                              ball_physics_component.left()};
    const float overlap_top{ball_physics_component.bottom() -
                            brick_physics_component.top()};
    const float overlap_bottom{brick_physics_component.bottom() -
                               ball_physics_component.top()};

    const bool ball_from_left(std::abs(overlap_left) < std::abs(overlap_right));
    const bool ball_from_top(std::abs(overlap_top) < std::abs(overlap_bottom));

    const float min_overlap_x{ball_from_left ? overlap_left : overlap_right};
    const float min_overlap_y{ball_from_top ? overlap_top : overlap_bottom};

    if (std::abs(min_overlap_x) < std::abs(min_overlap_y))
    {
        ball_physics_component._velocity.x = ball_from_left
                                                 ? -constants::kBallVelocity
                                                 : constants::kBallVelocity;
    }
    else
    {
        ball_physics_component._velocity.y = ball_from_top
                                                 ? -constants::kBallVelocity
                                                 : constants::kBallVelocity;
    }
}

template <typename T1, typename T2>
bool is_intersecting(const T1 &object_a, const T2 &object_b) noexcept
{
    return object_a.right() >= object_b.left() &&
           object_a.left() <= object_b.right() &&
           object_a.bottom() >= object_b.top() &&
           object_a.top() <= object_b.bottom();
}
