#include <Wretched/System/Entity.h>

namespace Wretched {
  Entity* Entity::Manager::createentity() {
    Entity *ent = new Entity();
    ent->id = entity_id_manager.assign();
    entity_list.push_back(std::unique_ptr<Entity>(ent));
    return ent;
  };

  const std::vector<std::unique_ptr<Entity>>* Entity::Manager::getentities() const {
    return &entity_list;
  };
}
