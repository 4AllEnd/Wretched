#include <Wretched/Graphics/Rendering.h>

namespace Wretched {
  namespace Graphics {
    Model::Model(System *mgr): manager(mgr) {}

    Model::~Model() {
      for (auto &Array : VertexObjectArrays)glDeleteBuffers(1, &Array);
    };

    Model::Model(std::vector<std::vector<float>> meshdat, std::vector<std::vector<unsigned int>> meshindic,
                 std::vector<std::vector<unsigned int>> texture_index, std::string fi,
                 System* mgr) : filename(fi), manager(mgr), texturesindex(texture_index) {
      VertexObjectArrays.resize(meshindic.size());
      VertexArraysSize.resize(meshindic.size());
      glGenVertexArrays((GLsizei)meshindic.size(), (GLuint*)&VertexObjectArrays[0]);
      const int b = sizeof(float);
      for (int i = 0; i < meshindic.size(); ++i) {

        glBindVertexArray(VertexObjectArrays[i]);
        GLuint buf;
        glGenBuffers(1, &buf);
        glBindBuffer(GL_ARRAY_BUFFER, buf);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);

        glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexAttribFormat(1, 2, GL_FLOAT, GL_FALSE, 0);
        glVertexAttribFormat(2, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexAttribFormat(3, 3, GL_FLOAT, GL_FALSE, 0);

        glBindVertexBuffer(0, buf, 0, 11 * sizeof(float));
        glBindVertexBuffer(1, buf, 3 * sizeof(float), 11 * sizeof(float));
        glBindVertexBuffer(2, buf, 5 * sizeof(float), 11 * sizeof(float));
        glBindVertexBuffer(3, buf, 8 * sizeof(float), 11 * sizeof(float));

        glBufferData(GL_ARRAY_BUFFER, meshdat[i].size() * sizeof(float), meshdat[i].data(), GL_STATIC_DRAW);
        GLuint bufi;
        glGenBuffers(1, &bufi);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufi);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshindic[i].size() * sizeof(unsigned int), &meshindic[i][0], GL_STATIC_DRAW);

        glBindVertexArray(0);
        VertexArraysSize[i] = (unsigned int)meshindic[i].size();
      }
    }

    template <typename T>
    glm::vec3 idtorgb(T id) {
      T a = (id >> 16) & 0xFF;
      T b = (id >> 8) & 0xFF;
      T c = id & 0xFF;;
      return {a, b, c};
    }

  }
}

