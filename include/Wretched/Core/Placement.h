#pragma once
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <Wretched/System/Entity.h>
#include <Wretched/Math/GridPosition.h>

namespace Wretched {
  namespace Placement {
    struct Placement {
      GridPosition position;
      glm::quat orientation = glm::quat() * glm::quat(cos(glm::pi<float>() / 2), 0, 0, sin(glm::pi<float>() / 2));
      float getpitch() const {
        return glm::pitch(orientation);
      };

      float getyaw() const {
        return glm::yaw(orientation);
      };

      float getroll() const {
        return glm::roll(orientation);
      };

      glm::vec3 rightdir() const {
        return glm::cross(orientation, glm::vec3(-1, 0, 0));
      }

      glm::vec3 updir() const {
        return glm::cross(orientation, glm::vec3(0, -1, 0));
      }

      glm::vec3 frontdir() const {
        return glm::cross(orientation, glm::vec3(0, 0, 1));
      };
      void move(glm::vec3 &position) {
        this->position.move(position);
      };
      void rotate(glm::quat &rotation) {
        orientation *= rotation;
      };
      void roll(const float &angle) {
        orientation *= glm::quat(cos(angle / 2), 0, 0, sin(angle / 2));
      };

      void pitch(const float &angle) {
        orientation *= glm::quat(cos(angle / 2), sin(angle / 2), 0, 0);
      };

      void yaw(const float &angle) {
        orientation *= glm::quat(cos(angle / 2), 0, sin(angle / 2), 0);
      };
    };
    struct System {};
  };
};