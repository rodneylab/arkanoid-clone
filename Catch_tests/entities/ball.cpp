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

TEST_CASE("It creates ball with expected components and tags", "[create_ball]")
{
    // arrange
    flecs::world world;
    SetRandomSeed((unsigned int)time(nullptr));

    // act
    create_ball(&world);
    auto ball_entity(world.entity("Ball"));

    SECTION("It has a Ball tag")
    {
        // assert
        // Test Ball exists with expected label
        REQUIRE(world.lookup("Ball") != 0);

        // Test the ball entity has a ball Tag
        REQUIRE(ball_entity.has<Ball>());
    }

    SECTION("It has a Circle component")
    {
        // assert
        const CircleComponent *circle_component{
            ball_entity.get<CircleComponent>()};
        constexpr float expected_radius{8.F};
        constexpr Color expected_colour{RED};
        REQUIRE(circle_component != nullptr);
        REQUIRE_THAT(
            circle_component->radius,
            Catch::Matchers::WithinRel(expected_radius, kFloatEps) ||
                Catch::Matchers::WithinAbs(expected_radius, kFloatAbsMargin));
        REQUIRE(circle_component->colour.r == expected_colour.r);
        REQUIRE(circle_component->colour.g == expected_colour.g);
        REQUIRE(circle_component->colour.b == expected_colour.b);
        REQUIRE(circle_component->colour.a == expected_colour.a);
    }

    SECTION("It has a Position component")
    {
        // assert
        const Position *position_component{ball_entity.get<Position>()};
        constexpr float expected_centre_x{271.5F};
        constexpr float expected_centre_y{396.F};
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
            ball_entity.get<CollisionBox>()};
        constexpr float expected_collision_box_half_width{8.F};
        constexpr float expected_collision_box_half_height{4.F};
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
        const Velocity *velocity_component{ball_entity.get<Velocity>()};
        constexpr float expected_speed_x{0.8F};
        constexpr float expected_velocity_y{-0.4F};
        REQUIRE(velocity_component != nullptr);
        REQUIRE_THAT(
            fabs(velocity_component->values.x),
            Catch::Matchers::WithinRel(expected_speed_x, kFloatEps) ||
                Catch::Matchers::WithinAbs(expected_speed_x, kFloatAbsMargin));
        REQUIRE_THAT(
            velocity_component->values.y,
            Catch::Matchers::WithinRel(expected_velocity_y, kFloatEps) ||
                Catch::Matchers::WithinAbs(expected_velocity_y,
                                           kFloatAbsMargin));
    }

    SECTION("It has a randomly set velocity direction")
    {
        // Test randomness of the ball initial x velocity, should average close
        // to zero and have min, max equal and opposite in sign arrange
        constexpr std::size_t iterations{128};
        std::array<float, iterations> x_velocities{};
        float velocity_sum{0.F};

        // act
        for (std::size_t i = 0; i < iterations; ++i)
        {
            flecs::world world_temp;
            create_ball(&world_temp);
            auto ball_entity_iter(world_temp.entity("Ball"));
            const Velocity *velocity_component_temp{
                ball_entity_iter.get<Velocity>()};
            const float velocity_x{velocity_component_temp->values.x};
            x_velocities[i] = velocity_x;
            velocity_sum += velocity_x;
        }

        // assert
        constexpr float expected_speed_x{0.8F};
        const auto [min_x_velocity, max_x_velocity] =
            std::minmax_element(x_velocities.begin(), x_velocities.end());
        const float mean_x_velocity{velocity_sum / iterations};
        REQUIRE_THAT(
            *min_x_velocity,
            Catch::Matchers::WithinRel(-1.F * expected_speed_x, kFloatEps) ||
                Catch::Matchers::WithinAbs(-1.F * expected_speed_x,
                                           kFloatAbsMargin));
        REQUIRE_THAT(
            *max_x_velocity,
            Catch::Matchers::WithinRel(expected_speed_x, kFloatEps) ||
                Catch::Matchers::WithinAbs(expected_speed_x, kFloatAbsMargin));
        REQUIRE(fabs(mean_x_velocity) < 0.2F);
    }
}
