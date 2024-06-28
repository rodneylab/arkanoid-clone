#include "entities.h"

#include "components.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <flecs.h>
#include <raylib.h>

#include <algorithm>
#include <array>
#include <cmath>

constexpr float kFloatEps{0.001F};
constexpr float kFloatAbsMargin{0.000'001F};

TEST_CASE("It creates paddle with expected components and tags",
          "[create_paddle]")
{
    // arrange
    flecs::world world;

    // act
    create_paddle(&world);
    auto paddle_entity(world.entity("Paddle"));

    SECTION("It has a Paddle tag")
    {
        // assert
        // Test Ball exists with expected label
        REQUIRE(world.lookup("Paddle") != 0);

        // Test the ball entity has a ball Tag
        REQUIRE(paddle_entity.has<Paddle>());
    }

    SECTION("It has a Rectangle component")
    {
        // assert
        const RectangleComponent *rectangle_component{
            paddle_entity.get<RectangleComponent>()};
        constexpr float expected_width{60.F};
        constexpr float expected_height{20.F};
        constexpr Color expected_colour{RED};
        REQUIRE(rectangle_component != nullptr);
        REQUIRE_THAT(
            rectangle_component->width,
            Catch::Matchers::WithinRel(expected_width, kFloatEps) ||
                Catch::Matchers::WithinAbs(expected_width, kFloatAbsMargin));
        REQUIRE_THAT(
            rectangle_component->height,
            Catch::Matchers::WithinRel(expected_height, kFloatEps) ||
                Catch::Matchers::WithinAbs(expected_height, kFloatAbsMargin));
        REQUIRE(rectangle_component->colour.r == expected_colour.r);
        REQUIRE(rectangle_component->colour.g == expected_colour.g);
        REQUIRE(rectangle_component->colour.b == expected_colour.b);
        REQUIRE(rectangle_component->colour.a == expected_colour.a);
    }

    SECTION("It has a Position component")
    {
        // assert
        const Position *position_component{paddle_entity.get<Position>()};
        constexpr float expected_centre_x{400.F};
        constexpr float expected_centre_y{410.F};
        REQUIRE(position_component != nullptr);
        REQUIRE_THAT(
            position_component->centre.x,
            Catch::Matchers::WithinRel(expected_centre_x, kFloatEps) ||
                Catch::Matchers::WithinAbs(expected_centre_x, kFloatAbsMargin));
        REQUIRE_THAT(
            position_component->centre.y,
            Catch::Matchers::WithinRel(expected_centre_y, kFloatEps) ||
                Catch::Matchers::WithinAbs(expected_centre_y, kFloatAbsMargin));
    }

    SECTION("It has a Collision Box component")
    {
        // assert
        const CollisionBox *collision_box_component{
            paddle_entity.get<CollisionBox>()};
        constexpr float expected_collision_box_half_width{30.F};
        constexpr float expected_collision_box_half_height{10.F};
        REQUIRE(collision_box_component != nullptr);
        REQUIRE_THAT(
            collision_box_component->half_width,
            Catch::Matchers::WithinRel(expected_collision_box_half_width,
                                       kFloatEps) ||
                Catch::Matchers::WithinAbs(expected_collision_box_half_width,
                                           kFloatAbsMargin));
        REQUIRE_THAT(
            collision_box_component->half_height,
            Catch::Matchers::WithinRel(expected_collision_box_half_height,
                                       kFloatEps) ||
                Catch::Matchers::WithinAbs(expected_collision_box_half_height,
                                           kFloatAbsMargin));
    }

    SECTION("It has a Velocity component")
    {
        // assert
        const Velocity *velocity_component{paddle_entity.get<Velocity>()};
        constexpr float expected_velocity_x{0.0F};
        constexpr float expected_velocity_y{0.0F};
        REQUIRE(velocity_component != nullptr);
        REQUIRE_THAT(
            fabs(velocity_component->values.x),
            Catch::Matchers::WithinRel(expected_velocity_x, kFloatEps) ||
                Catch::Matchers::WithinAbs(expected_velocity_x,
                                           kFloatAbsMargin));
        REQUIRE_THAT(
            velocity_component->values.y,
            Catch::Matchers::WithinRel(expected_velocity_y, kFloatEps) ||
                Catch::Matchers::WithinAbs(expected_velocity_y,
                                           kFloatAbsMargin));
    }
}
