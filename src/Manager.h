#ifndef SRC_MANAGER_H
#define SRC_MANAGER_H

#include "Entity.h"
#include "utility.h"

#include <memory>
#include <vector>

class Entity;

struct Manager
{
public:
    void update(float frame_time);
    void draw();

    void add_to_group(Entity *entity, Group group);

    std::vector<Entity *> &get_entities_by_group(Group group);

    void refresh();

    Entity &add_entity();
    void add_group(Group group) noexcept;

private:
    std::vector<std::unique_ptr<Entity>> _entities;
    std::array<std::vector<Entity *>, kMaxGroups> _grouped_entities;
};

#endif
