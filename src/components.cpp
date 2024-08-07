#include "components.h"

// Windows workarounds for CloseWindow / ShowCursor errors

#if defined(_WIN32)
#define NOGDI  // All GDI defines and routines
#define NOUSER // All USER defines and routines
#endif

#include <fmt/core.h>

#undef near
#undef far
#include <raylib.h>

#include <iostream>
#include <sstream>
#include <string>

LevelBrick::LevelBrick(const BrickType brick_type,
                       int value,
                       int hits_to_destroy_value)
    : colour(WHITE), points_value(value), hits_to_destroy(hits_to_destroy_value)
{
    switch (brick_type)
    {
    case BrickType::kWhite:
    {
        colour = WHITE;
        break;
    }
    case BrickType::kOrange:
    {
        colour = ORANGE;
        break;
    }
    case BrickType::kCyan:
    {
        colour = SKYBLUE;
        break;
    }
    case BrickType::kGreen:
    {
        colour = GREEN;
        break;
    }
    case BrickType::kRed:
    {
        colour = RED;
        break;
    }
    case BrickType::kBlue:
    {
        colour = BLUE;
        break;
    }
    case BrickType::kMagenta:
    {
        colour = MAGENTA;
        break;
    }
    case BrickType::kYellow:
    {
        colour = YELLOW;
        break;
    }
    case BrickType::kSilver:
    {
        colour = LIGHTGRAY;
        break;
    }
    case BrickType::kGold:
    {
        colour = GOLD;
        break;
    }
    }
}

std::ostream &operator<<(std::ostream &out, const LevelBrick &brick)
{
    out << fmt::format("LevelBrick({{ points: {}, hitsToDestroy: {}}}",
                       brick.points_value,
                       brick.hits_to_destroy);

    return out;
}

std::string to_string(const LevelBrick &brick)
{
    std::ostringstream ssstream;
    ssstream << brick;

    return ssstream.str();
}
