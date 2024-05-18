#include "Component.h"
#include "Entity.h"
#include "Game.h"
#include "Manager.h"

#include <catch2/catch_test_macros.hpp>

#include <memory>

// NOLINTNEXTLINE(misc-use-anonymous-namespace)
TEST_CASE("PositionComponent", "[ComponentTests]")
{
    // arrange
    constexpr float kXPosition{3.F};
    constexpr float kYPosition{4.F};

    // act
    const PositionComponent position_component{sf::Vector2f{3.F, 4.F}};

    // assert
    CHECK(position_component.x() == kXPosition);
    CHECK(position_component.y() == kYPosition);
}

// NOLINTNEXTLINE(misc-use-anonymous-namespace)
TEST_CASE("PhysicsComponentInitialises", "[PositionComponentTests]")
{
    // arrange
    Manager manager;
    auto &entity(manager.add_entity());

    // act
    constexpr float kInitialPositionX{960.F};
    constexpr float kInitialPositionY{540.F};
    entity.add_component<PositionComponent>(
        sf::Vector2f{kInitialPositionX, kInitialPositionY});

    constexpr float kHalfSizeWidth{16.F};
    constexpr float kHalfSizeHeight{9.F};
    entity.add_component<PhysicsComponent>(
        sf::Vector2f{kHalfSizeWidth, kHalfSizeHeight});

    auto &physics_component(entity.get_component<PhysicsComponent>());
    physics_component.on_out_of_bounds =
        [&physics_component](const sf::Vector2f &side) {
            physics_component._velocity = sf::Vector2f{0.F, 0.F};
        };

    // assert
    CHECK(physics_component.x() == kInitialPositionX);
    CHECK(physics_component.y() == kInitialPositionY);
    CHECK(physics_component.left() == kInitialPositionX - kHalfSizeWidth);
    CHECK(physics_component.right() == kInitialPositionX + kHalfSizeWidth);
    CHECK(physics_component.top() == kInitialPositionY - kHalfSizeHeight);
    CHECK(physics_component.bottom() == kInitialPositionY + kHalfSizeHeight);
}

// NOLINTNEXTLINE(misc-use-anonymous-namespace)
TEST_CASE("PhysicsComponentWithNoVelicityDoesNotMove",
          "[PositionComponentTests]")
{
    // arrange
    Manager manager;
    auto &entity(manager.add_entity());

    // act
    constexpr float kInitialPositionX{960.F};
    constexpr float kInitialPositionY{540.F};
    entity.add_component<PositionComponent>(
        sf::Vector2f{kInitialPositionX, kInitialPositionY});

    constexpr float kHalfSizeWidth{16.F};
    constexpr float kHalfSizeHeight{9.F};
    entity.add_component<PhysicsComponent>(
        sf::Vector2f{kHalfSizeWidth, kHalfSizeHeight});

    auto &physics_component(entity.get_component<PhysicsComponent>());
    physics_component.on_out_of_bounds =
        [&physics_component](const sf::Vector2f &side) {
            physics_component._velocity = sf::Vector2f{0.F, 0.F};
        };

    SECTION("Physics component with no velocity does not move")
    {

        constexpr float kUpdateFrameTime{60.F};
        physics_component.update(kUpdateFrameTime);

        // assert
        CHECK(physics_component.x() == kInitialPositionX);
        CHECK(physics_component.y() == kInitialPositionY);
        CHECK(physics_component.left() == kInitialPositionX - kHalfSizeWidth);
        CHECK(physics_component.right() == kInitialPositionX + kHalfSizeWidth);
        CHECK(physics_component.top() == kInitialPositionY - kHalfSizeHeight);
        CHECK(physics_component.bottom() ==
              kInitialPositionY + kHalfSizeHeight);
    }


    SECTION("Physics component with velocity moves")
    {
        constexpr float kVelocityX{1.F};
        constexpr float kVelocityY{2.F};
        physics_component._velocity = sf::Vector2f{kVelocityX, kVelocityY};

        // act
        constexpr float kUpdateFrameTime{1.F};
        physics_component.update(kUpdateFrameTime);

        // assert
        CHECK(physics_component.x() == kInitialPositionX + kVelocityX);
        CHECK(physics_component.y() == kInitialPositionY + kVelocityY);
        CHECK(physics_component.left() ==
              kInitialPositionX - kHalfSizeWidth + kVelocityX);
        CHECK(physics_component.right() ==
              kInitialPositionX + kHalfSizeWidth + kVelocityX);
        CHECK(physics_component.top() ==
              kInitialPositionY - kHalfSizeHeight + kVelocityY);
        CHECK(physics_component.bottom() ==
              kInitialPositionY + kHalfSizeHeight + kVelocityY);
    }
}
