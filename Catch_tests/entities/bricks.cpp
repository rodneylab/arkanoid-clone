#include "entities.h"

#include "components.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <flecs.h>
#include <fmt/core.h>
#include <raylib.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <random>

constexpr float kFloatEps{0.001F};
constexpr float kFloatAbsMargin{0.000001F};

TEST_CASE("It creates bricks with expected components and tags",
          "[create_bricks]")
{
    // arrange
    flecs::world world;

    std::random_device rd{};
    std::seed_seq ss{rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd()};
    std::mt19937 mt{ss};
    std::uniform_int_distribution random_row{1, 4};
    std::uniform_int_distribution random_column{1, 11};

    // act
    create_bricks(&world);

    SECTION("It gives each brick a Brick tag")
    {
        // assert
        REQUIRE(world.count<Brick>() == 44);
    }

    // arrange
    const flecs::query<Brick> brick_query{world.query<Brick>()};
    flecs::entity first_brick_entity{brick_query.first()};

    // assert
    REQUIRE(first_brick_entity != 0);

    SECTION("It gives bricks a Rectangle component")
    {
        const std::string random_brick_name =
            fmt::format("Brick_{:02}_{:02}", random_row(mt), random_column(mt));
        flecs::entity first_brick_entity(
            world.lookup(random_brick_name.c_str()));

        REQUIRE(first_brick_entity != 0);

        // assert
        const RectangleComponent *rectangle_component{
            first_brick_entity.get<RectangleComponent>()};
        constexpr float expected_width{60.F};
        constexpr float expected_height{20.F};
        constexpr Color expected_colour{YELLOW};
        REQUIRE(rectangle_component != nullptr);
        REQUIRE(first_brick_entity.has<RectangleComponent>());
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

    SECTION("It gives bricks a Position component")
    {
        const flecs::query<Brick, Position> brick_position_query{
            world.query<Brick, Position>()};

        // check position of top-left brick
        flecs::entity top_left_brick = brick_position_query.find(
            [](const Brick & /* brick */, const Position &position) {
                return position.centre.x == 85 && position.centre.y == 46;
            });
        REQUIRE(top_left_brick != 0);
    }

    SECTION("It gives bricks a CollisionBox component")
    {
        // assert
        const std::string random_brick_name =
            fmt::format("Brick_{:02}_{:02}", random_row(mt), random_column(mt));
        flecs::entity first_brick_entity(
            world.lookup(random_brick_name.c_str()));
        REQUIRE(first_brick_entity != 0);

        const CollisionBox *collision_box{
            first_brick_entity.get<CollisionBox>()};
        constexpr float expected_half_width{30.F};
        constexpr float expected_half_height{10.F};
        REQUIRE(collision_box != nullptr);
        REQUIRE(first_brick_entity.has<CollisionBox>());
        REQUIRE(collision_box->half_width == expected_half_width);
        REQUIRE(collision_box->half_height == expected_half_height);
    }
}
