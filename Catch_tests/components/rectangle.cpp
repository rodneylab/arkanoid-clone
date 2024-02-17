#include "components.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <raylib.h>

constexpr float kFloatEps{0.001F};
constexpr float kFloatAbsMargin{0.000'001F};

TEST_CASE("It creates rectangle", "[Rectangle(float,float,colour)]")
{
    // arrange
    constexpr float width{16.F};
    constexpr float height{9.F};

    // act
    RectangleComponent rectangle_component{width, height, BLUE};

    REQUIRE_THAT(rectangle_component.width,
                 Catch::Matchers::WithinRel(width, kFloatEps) ||
                     Catch::Matchers::WithinAbs(width, kFloatAbsMargin));
    REQUIRE_THAT(rectangle_component.height,
                 Catch::Matchers::WithinRel(height, kFloatEps) ||
                     Catch::Matchers::WithinAbs(height, kFloatAbsMargin));
    REQUIRE(rectangle_component.colour.r == BLUE.r);
    REQUIRE(rectangle_component.colour.g == BLUE.g);
    REQUIRE(rectangle_component.colour.b == BLUE.b);
    REQUIRE(rectangle_component.colour.a == BLUE.a);
}
