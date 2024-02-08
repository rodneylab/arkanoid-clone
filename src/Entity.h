#ifndef SRC_ENTITY_H
#define SRC_ENTITY_H

#include "Component.h"
#include "utility.h"

#include <array>
#include <memory>
#include <vector>

class Component;
class Manager;

using ComponentArray = std::array<Component *, kMaxComponents>;

class Entity
{
public:
    explicit Entity(Manager &manager);

    void update(float frame_time);
    void draw();

    [[nodiscard]] bool is_alive() const;
    void destroy();

    template <typename T>
    [[nodiscard]] bool has_component() const;

    [[nodiscard]] bool has_group(Group group) const noexcept;

    void add_group(Group group) noexcept;
    void delete_group(Group group) noexcept;

    template <typename T, typename... TArgs>
    T &add_component(TArgs &&...arguments);

    template <typename T>
    T &get_component() const;

private:
    Manager &_manager;

    bool _alive{true};
    std::vector<std::unique_ptr<Component>> _components;
    ComponentArray _component_array;
    ComponentBitset _component_bitset;

    GroupBitset _group_bitset;
};

#endif
