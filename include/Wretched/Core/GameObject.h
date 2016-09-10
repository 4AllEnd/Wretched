#pragma once
#include <Wretched/System/Entity.h>
#include <Wretched/System/RuntimeObjectSystem/IObject.h>
namespace Wretched {
  struct GameObject {
    Entity *entity;
    GameObject(Entity *a) : entity(a) {}
    virtual void init() = 0;
    virtual void update(double deltatime) = 0;

  };
 }
