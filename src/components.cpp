#include "components.h"

#include <fmt/core.h>

#include <iostream>
#include <sstream>
#include <string>

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
