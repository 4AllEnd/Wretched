#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include <typeindex>
//#include <functional>
#include <Wretched/Algorithm/Tree.h>
#include <Wretched/System/IDManager.h>

namespace Wretched {
  typedef unsigned int IDType;
  typedef std::unique_ptr<void, std::function<void(void *)>> component_ptr;

  struct Entity final : std::unordered_map<std::type_index, component_ptr>, Node<Entity> {
  protected:
    Entity() { };

    Entity(const Entity &) { }

    IDType id = 0;
  public:
    const IDType& getid() const {
      return id;
    }

    template<typename T, typename _BaseClass = T, typename ... _Args>
    T *createcomponent(_Args &&...args) {
      component_ptr ptr(new T(std::forward<_Args>(args)...), [](void *ptr) { delete (T *) ptr; });
      operator[](std::type_index(typeid(_BaseClass))) = std::move(ptr);
      return (T *) (operator[](std::type_index(typeid(T))).get());
    }

    template<typename T>
    T *getcomponent() {
      return count(typeid(T)) ? (T *) operator[](std::type_index(typeid(T))).get() : nullptr;
    }

    template<typename T>
    void removecomponent() {
      erase(find(std::type_index(typeid(T))));
    }

    struct Manager {
    private:
      std::vector<std::unique_ptr<Entity>> entity_list;
      IDManager<IDType> entity_id_manager;
    public:
      Entity *createentity();
      const std::vector<std::unique_ptr<Entity>> *getentities() const;
    };
  };
}
