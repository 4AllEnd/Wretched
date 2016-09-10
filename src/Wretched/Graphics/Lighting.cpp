#include <Wretched/Graphics/Lighting.h>

namespace Wretched {
  namespace Lighting {
    Manager::Manager() {
      glGenBuffers(1, &ssbo);
    }

    Manager::~Manager() {
      glDeleteBuffers(1, &ssbo);
    }

    void Manager::load() {
      glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
      glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GLfloat) * LightData.size(), LightData.data(), GL_STREAM_DRAW);
    };

    void Manager::clear() {
      LightData.resize(0);
    }

    void Manager::bind(int binding_index) {
      glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding_index, ssbo);
    }

    void Manager::render(Entity &cam, Entity &light) {
      auto campos = cam.getcomponent<Placement::Placement>();
      auto lightpos = light.getcomponent<Placement::Placement>();
      auto ldata = light.getcomponent<Light>();
      glm::vec3 dir = Wretched::Lighting::Light::Point != ldata->LightType ? glm::axis(lightpos->orientation): glm::vec3(0);
      glm::vec3 p = (lightpos->position - campos->position).getabsposition();
      LightData.insert(LightData.end(),
        {p.x, p.y, p.z, 1.f,
        ldata->color.r, ldata->color.g, ldata->color.b, 1.f,
        dir.x, dir.y, dir.z, ldata->LightType,
        ldata->attenuation, ldata->cutoff, ldata->exponent, ldata->radius
        });
    }
  }
}