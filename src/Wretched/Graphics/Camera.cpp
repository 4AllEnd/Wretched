#include <Wretched/Graphics/Camera.h>

namespace Wretched {
  void Camera::update(unsigned int screen_width, unsigned int screen_height) {//combines the position and rotation matrix, updates the projection matrix
    width = screen_width;
    height = screen_height;
    if (zoom > W_pi) zoom = W_pi;
    else if (zoom < 0) zoom = 0;
  };

  void Camera::setcutoff(const float &min, const float &max) {
    mincutoff = min;
    maxcutoff = max;
  }

  glm::mat4 Camera::view(Placement::Placement &placement) const {
    auto ori = glm::toMat4(placement.orientation);
    return glm::lookAt(glm::vec3(0,0,0), glm::vec3(-ori[2]), glm::vec3(ori[1]));//glm::vec3(-ori[2]), glm::vec3(ori[1]));//glm::transpose(glm::toMat4(placement.orientation));
  }

  glm::mat4 Camera::proj() const {
    return glm::infinitePerspective(zoom, (float)width / height, mincutoff);
    //glm::perspective(zoom, (float)width / height, mincutoff, maxcutoff);
  }

  Camera::Camera() : zoom(1.0) {}
}

