#pragma once
#include <cmath>
#include <omp.h>
#include <vector>
#include <unordered_map>
#include <string>
#include <memory>
#include <iostream>
#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h> // Output data structure

#include <assimp/postprocess.h> 
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <Wretched/Core/Placement.h>
#include <Wretched/Graphics/Camera.h>
#include <Wretched/Graphics/Structures.h>
#include <Wretched/System/Entity.h>
#include <Wretched/System/FileData.h>

namespace Wretched {
  enum MaterialTexture {
    BEGIN,
    AMBIENT = BEGIN,
    DIFFUSE,
    SPECULAR,
    END,
    NUMMATTEXTURES = END - BEGIN
  };

  namespace Graphics {
    struct Model;
    struct Object;
    struct ModelSystem;

    struct System {
      System(Entity::Manager &entity_manager);
      ~System(){};
      Entity::Manager *emgr = nullptr;
      Entity *curr_cam = nullptr;
      Entity* _hoveron = nullptr;
      GL::BufferObject curs = GL::BufferObject(GL_SHADER_STORAGE_BUFFER);
      std::unordered_map<std::string, std::shared_ptr<Model>> Models;
      void setcursor(glm::ivec2 cursor) const;
      void updatehover();
      void load(Object &obj, const char *filename);
      void render();
      void render(Model &);
      void finishrender(Model &);
    };

    struct Model {
      Model(System *);
      ~Model();
      Model(std::vector<std::vector<float>> meshdat, std::vector<std::vector<unsigned>> meshindic, std::vector<std::vector<unsigned>> texture_index, std::string fi, System *mgr);
      std::string filename;
      bool loaded = false;
      std::vector<GLuint> VertexObjectArrays;
      std::vector<std::vector<unsigned int>> texturesindex;
      std::vector<unsigned int> VertexArraysSize;
      std::vector<Entity*> instancedata;
      Graphics::System *manager;
    };

    struct Material {
      std::shared_ptr<GL::Texture2D> ambient, diffuse, specular, emissive;
    };

    struct Object {
      Object(System &mgr, const char *filename){
        mgr.load(*this, filename);
      }
      Model *model = nullptr;
      bool nodepthtest = false;
      float scale = 1;
    };
  }
}

