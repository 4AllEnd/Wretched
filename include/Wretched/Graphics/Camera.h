#pragma once
#define GLM_FORCE_RADIANS
#include <glm/gtx/quaternion.hpp>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <Wretched/System/Settings.h>
#include <Wretched/System/Entity.h>
#include <Wretched/Math/Math.h>
#include <Wretched/Core/Placement.h>
namespace Wretched {
  struct Camera {
    Camera();
    glm::mat4 proj() const;
    glm::mat4 view(Placement::Placement &placement) const;
    float zoom, mincutoff, maxcutoff;
    unsigned int width, height;
    void update(unsigned int screenwidth, unsigned int screenheight);
    void setcutoff(const float &, const float &);
  };
}

