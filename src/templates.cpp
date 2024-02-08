#include "Entity.h"

// NOLINTNEXTLINE(bugprone-suspicious-include) # using three-file approach for C++ templates
#include "Entity.cpp"

template CircleComponent &Entity::
    add_component<CircleComponent, Game *, float const &>(Game *&&,
                                                          float const &);
template PhysicsComponent &Entity::add_component<PhysicsComponent,
                                                 sf::Vector2<float>>(
    sf::Vector2<float> &&);

template PhysicsComponent &Entity::add_component<PhysicsComponent,
                                                 sf::Vector2<float> &>(
    sf::Vector2<float> &);

template PositionComponent &Entity::add_component<PositionComponent,
                                                  sf::Vector2<float>>(
    sf::Vector2<float> &&);

template PositionComponent &Entity::add_component<PositionComponent,
                                                  sf::Vector2<float> const &>(
    sf::Vector2<float> const &);

template RectangleComponent &Entity::
    add_component<RectangleComponent, Game *, sf::Vector2<float> &>(
        Game *&&,
        sf::Vector2<float> &);

template RectangleComponent &Entity::add_component<RectangleComponent,
                                                   Game *,
                                                   sf::Color const &,
                                                   sf::Vector2<float> &>(
    Game *&&,
    sf::Color const &,
    sf::Vector2<float> &);

template PaddleControlComponent &Entity::add_component<
    PaddleControlComponent>();

template PhysicsComponent &Entity::get_component<PhysicsComponent>() const;
template PositionComponent &Entity::get_component<PositionComponent>() const;
