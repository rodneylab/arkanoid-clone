#ifndef UTILITY_H
#define UTILITY_H

#include <bitset>
#include <cstddef>

using ComponentID = std::size_t;
using FrameTime = float;
using Group = std::size_t;

constexpr std::size_t kMaxComponents{32};
using ComponentBitset = std::bitset<kMaxComponents>;

constexpr std::size_t kMaxGroups{32};
using GroupBitset = std::bitset<kMaxGroups>;

namespace internal
{
inline ComponentID get_unique_component_id() noexcept
{
    static ComponentID last_id{0U};
    return last_id++;
}
} // namespace internal

#endif
