#pragma once
#include <GL/glew.h>
#include <vector>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <Wretched/Math/Math.h>
#include <Wretched/Core/Placement.h>
#include <Wretched/System/Entity.h>
#ifndef W_pi
#define W_pi atan2(0.f, -1.f)
#endif
#ifndef W_degree
#define W_degree atan2(0.f, -1.f)/180.f
#endif
#include <memory>

namespace Wretched {
  namespace Lighting {
    struct Light;

    struct Manager {
      GLuint ssbo, program;
      std::vector<float> LightData;
      Manager();
      ~Manager();
      void load();
      void clear();
      void bind(int binding_index);
      void render(Entity &cam, Entity &light);
    };

    struct Light {
      Placement::Placement placement;
      glm::vec3 color = glm::vec3(1.f);
      float LightType = 0.f;
      float cutoff = W_pi / 4.f, radius = 100.f, attenuation = 1.f, exponent = 1.f;

      enum Type {
        Point,
        Cone,
        Direction
      };
    };
  }
}

