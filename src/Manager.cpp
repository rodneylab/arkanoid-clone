#include "Manager.h"

void Manager::update(float frame_time)
{
    for (auto &entity : _entities)
    {
        entity->update(frame_time);
    }
}

void Manager::draw()
{
    for (auto &entity : _entities)
    {
        entity->draw();
    }
}

void Manager::add_to_group(Entity *entity, Group group)
{
    _grouped_entities[group].emplace_back(entity);
}

std::vector<Entity *> &Manager::get_entities_by_group(Group group)
{
    return _grouped_entities[group];
}

void Manager::refresh()
{
    for (auto i(0U); i < kMaxGroups; ++i)
    {
        auto &temp_vector(_grouped_entities[i]);

        temp_vector.erase(std::remove_if(std::begin(temp_vector),
                                         std::end(temp_vector),
                                         [i](Entity *entity) {
                                             return !entity->is_alive() ||
                                                    !entity->has_group(i);
                                         }),
                          std::end(temp_vector));
    }

    _entities.erase(std::remove_if(std::begin(_entities),
                                   std::end(_entities),
                                   [](const std::unique_ptr<Entity> &entity) {
                                       return !entity->is_alive();
                                   }),
                    std::end(_entities));
}

Entity &Manager::add_entity()
{
    std::unique_ptr<Entity> unique_pointer = std::make_unique<Entity>(*this);
    Entity *entity = unique_pointer.get();
    _entities.emplace_back(std::move(unique_pointer));

    return *entity;
}
