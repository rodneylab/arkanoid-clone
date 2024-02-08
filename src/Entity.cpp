#include "Entity.h"

#include "Component.h"
#include "Manager.h"

#include <array>
#include <cassert>

Entity::Entity(Manager &manager)
    : _component_array(std::array<Component *, kMaxComponents>{}),
      _manager(manager)
{
}

void Entity::update(float frame_time)
{
    for (auto &component : _components)
    {
        component->update(frame_time);
    }
}

void Entity::draw()
{
    for (auto &component : _components)
    {
        component->draw();
    }
}

bool Entity::is_alive() const
{
    return _alive;
}

void Entity::destroy()
{
    _alive = false;
}

template <typename T>
bool Entity::has_component() const
{
    return _component_bitset[get_component_type_id<T>()];
}

bool Entity::has_group(Group group) const noexcept
{
    return _group_bitset[group];
}

void Entity::delete_group(Group group) noexcept
{
    _group_bitset[group] = false;
}

void Entity::add_group(Group group) noexcept
{
    _group_bitset[group] = true;
    _manager.add_to_group(this, group);
}

template <typename T, typename... TArgs>
T &Entity::add_component(TArgs &&...arguments)
{
    assert(!has_component<T>());

    T *component(new T(std::forward<TArgs>(arguments)...));
    component->entity = this;
    std::unique_ptr<Component> component_unique_ptr{component};
    _components.emplace_back(std::move(component_unique_ptr));

    _component_array[get_component_type_id<T>()] = component;
    _component_bitset[get_component_type_id<T>()] = true;

    component->init();

    return *component;
}

template <typename T>
T &Entity::get_component() const
{
    assert(has_component<T>());
    auto pointer(_component_array[get_component_type_id<T>()]);

    return *static_cast<T *>(pointer);
}
