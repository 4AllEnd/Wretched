#include <Wretched/Graphics/Rendering.h>

namespace Wretched {
  namespace Graphics {
    System::System(Entity::Manager &entity_manager) : emgr(&entity_manager) {
      //initialize DevIL library
      ilInit();
      iluInit();
      //bind buffer for object picking
      glBindBuffer(GL_SHADER_STORAGE_BUFFER, curs.name());
      glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(IDType), 0, GL_DYNAMIC_DRAW);
      glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, curs.name());
    };

    void System::load(Object &obj, const char *filename) {
      Assimp::Importer imp;
      const aiScene *scene;

      std::string filen(filename), parentdir(filen.substr(0, filen.find_last_of("\\/"))), ext(
          filen.substr(filen.find_last_of(".")));
      if (Models.count(filen) == 0) {
        scene = imp.ReadFile(filename, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_GenNormals
                                       | aiProcess_JoinIdenticalVertices | aiProcess_OptimizeMeshes |
                                       aiProcess_FixInfacingNormals | aiProcess_GenUVCoords);
        if (scene) {
          aiMemoryInfo mem;
          imp.GetMemoryRequirements(mem);
          unsigned int gee = 0;
          for (unsigned int h = 0; h < scene->mNumMeshes; ++h)gee += scene->mMeshes[h]->mNumVertices;
          printf(
              "...Loading File Model %s meshes:%d, vertices:%d, materials:%d, embedded textures:%d, import size:%dKB\n",
              filen.substr(filen.find_last_of("\\/") + 1).c_str(), scene->mNumMeshes, gee, scene->mNumMaterials,
              scene->mNumTextures, mem.total / 1000);
          std::vector<std::vector<float>> meshes(scene->mNumMeshes, std::vector<float>());
          std::vector<std::vector<unsigned int>> meshesindex(scene->mNumMeshes,
                                                             std::vector<unsigned int>()), textureindex(
              scene->mNumMaterials, std::vector<unsigned int>(6, 0));
          aiString Path;
          for (unsigned int y = 0; y < scene->mNumMaterials; ++y) {
            if (scene->mMaterials[y]->GetTextureCount(aiTextureType_AMBIENT)) {
              scene->mMaterials[y]->GetTexture(aiTextureType_AMBIENT, 0, &Path, 0, 0, 0, 0, 0);
              std::string nPath(Path.C_Str());
              normalizefp(nPath);
              nPath = parentdir + "/" + nPath;
              ILboolean isload = ilLoadImage((char *) nPath.c_str());
              if (isload) {
                ilEnable(IL_ORIGIN_SET);
                ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
                GLuint textureID;
                glGenTextures(1, &textureID);
                glBindTexture(GL_TEXTURE_2D, textureID);
                glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
                auto format = ilGetInteger(IL_IMAGE_FORMAT);
                switch (format) {
                  case IL_RGB :
                    format = GL_RGB;
                    break;
                  case IL_RGBA :
                    format = GL_RGBA;
                    break;
                  case IL_LUMINANCE:
                    format = GL_LUMINANCE;
                    break;
                  case IL_LUMINANCE_ALPHA :
                    format = GL_LUMINANCE_ALPHA;
                    break;
                  case IL_ALPHA :
                    format = GL_ALPHA;
                    break;
                }
                glTexImage2D(GL_TEXTURE_2D, 0, format, ilGetInteger(IL_IMAGE_WIDTH),
                             ilGetInteger(IL_IMAGE_HEIGHT), ilGetInteger(IL_IMAGE_OFFX), format,
                             GL_UNSIGNED_BYTE, ilGetData());
                glGenerateMipmap(GL_TEXTURE_2D);
                textureindex[y][MaterialTexture::AMBIENT] = textureID;
              }
              else {
                printf("AMB LOAD FAILURE: %s\n", nPath.c_str());
              }
            }
            if (scene->mMaterials[y]->GetTextureCount(aiTextureType_DIFFUSE)) {
              scene->mMaterials[y]->GetTexture(aiTextureType_DIFFUSE, 0, &Path, 0, 0, 0, 0, 0);
              std::string nPath(parentdir + "/" + Path.C_Str());
              normalizefp(nPath);
              ILboolean isload = ilLoadImage((char *) nPath.c_str());
              if (isload) {
                ilEnable(IL_ORIGIN_SET);
                ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
                GLuint textureID;
                glGenTextures(1, &textureID);
                glBindTexture(GL_TEXTURE_2D, textureID);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
                auto format = ilGetInteger(IL_IMAGE_FORMAT);
                switch (format) {
                  case IL_RGB :
                    format = GL_RGB;
                    break;
                  case IL_RGBA :
                    format = GL_RGBA;
                    break;
                  case IL_LUMINANCE :
                    format = GL_LUMINANCE;
                    break;
                  case IL_LUMINANCE_ALPHA :
                    format = GL_LUMINANCE_ALPHA;
                    break;
                  case IL_ALPHA :
                    format = GL_ALPHA;
                    break;
                }
                glTexImage2D(GL_TEXTURE_2D, 0, format, ilGetInteger(IL_IMAGE_WIDTH),
                             ilGetInteger(IL_IMAGE_HEIGHT), ilGetInteger(IL_IMAGE_OFFX), format,
                             GL_UNSIGNED_BYTE, ilGetData());
                glGenerateMipmap(GL_TEXTURE_2D);
                textureindex[y][MaterialTexture::DIFFUSE] = textureID;
              }
              else {
                printf("DIFF LOAD FAILURE: %s\n", (char *) nPath.c_str());
              }
            }
            if (scene->mMaterials[y]->GetTextureCount(aiTextureType_SPECULAR)) {
              scene->mMaterials[y]->GetTexture(aiTextureType_SPECULAR, 0, &Path, 0, 0, 0, 0, 0);
              std::string nPath(parentdir + "/" + Path.C_Str());
              normalizefp(nPath);
              ILboolean isload = ilLoadImage((char *) nPath.c_str());
              if (isload) {
                ilEnable(IL_ORIGIN_SET);
                ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
                GLuint textureID;
                glGenTextures(1, &textureID);
                glBindTexture(GL_TEXTURE_2D, textureID);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                auto format = ilGetInteger(IL_IMAGE_FORMAT);
                switch (format) {
                  case IL_RGB :
                    format = GL_RGB;
                    break;
                  case IL_RGBA :
                    format = GL_RGBA;
                    break;
                  case IL_LUMINANCE:
                    format = GL_LUMINANCE;
                    break;
                  case IL_LUMINANCE_ALPHA :
                    format = GL_LUMINANCE_ALPHA;
                    break;
                  case IL_ALPHA :
                    format = GL_ALPHA;
                    break;
                }
                glTexImage2D(GL_TEXTURE_2D, 0, format, ilGetInteger(IL_IMAGE_WIDTH),
                             ilGetInteger(IL_IMAGE_HEIGHT), ilGetInteger(IL_IMAGE_OFFX), format,
                             GL_UNSIGNED_BYTE, ilGetData());
                glGenerateMipmap(GL_TEXTURE_2D);
                textureindex[y][MaterialTexture::SPECULAR] = textureID;
              }
              else {
                printf("LOAD FAILURE: %s\n", (char *) nPath.c_str());
              }
            }
          }
          for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
            std::vector<unsigned int> elementindices;
            auto mesh = scene->mMeshes[i];
            for (unsigned int iface = 0; iface < mesh->mNumFaces; ++iface) {
              auto iindex = mesh->mFaces[iface].mNumIndices;
              for (unsigned int k = 0; k < iindex; ++k) {
                auto hg = mesh->mFaces[iface].mIndices[k];
                elementindices.push_back(hg);
              }
            }
            meshesindex[i] = elementindices;
            const auto r = mesh->mNumVertices;
            meshes[i].reserve(r * 11);
            for (unsigned int a = 0; a < r; ++a) {
              meshes[i].insert(meshes[i].end(), {
                  mesh->mVertices[a].x, mesh->mVertices[a].y, mesh->mVertices[a].z});
              if (mesh->HasTextureCoords(0)) {
                meshes[i].insert(meshes[i].end(), {
                    mesh->mTextureCoords[0][a].x, mesh->mTextureCoords[0][a].y});
              } else {
                meshes[i].insert(meshes[i].end(), {0, 0});
              }
              if (mesh->HasNormals()) {
                meshes[i].insert(meshes[i].end(), {
                    mesh->mNormals[a].x, mesh->mNormals[a].y, mesh->mNormals[a].z});
              } else {
                meshes[i].insert(meshes[i].end(), {0, 0, 0});
              }
              if (mesh->HasTangentsAndBitangents()) {
                meshes[i].insert(meshes[i].end(), {
                    mesh->mTangents[a].x, mesh->mTangents[a].y, mesh->mTangents[a].z});
              } else {
                meshes[i].insert(meshes[i].end(), {0, 0, 0});
              }
            }
          }
          std::vector<std::vector<unsigned int>> mesh_textures(scene->mNumMeshes, {0, 0, 0, 0, 0, 0});
          for (int i = 0; i < meshesindex.size(); ++i) {
            memcpy(mesh_textures[i].data(), textureindex[scene->mMeshes[i]->mMaterialIndex].data(),
                   6 * sizeof(unsigned int));
          }
          Models[filen] = std::make_shared<Model>(meshes, meshesindex, mesh_textures, filen, this);
        }
        else {
          printf("cannot load model %s \n", filen.c_str());
          Models[filen] = std::make_shared<Model>(this);
        }
      }
      obj.model = Models[filen].get();
    }

    void System::setcursor(glm::ivec2 cursor) const {
      if (curr_cam != 0) {
        Camera *cam = curr_cam->getcomponent<Camera>();
        glUniform2i(4, cursor.x, cam->height - cursor.y);
      }
    }

    void System::updatehover() {
      _hoveron = 0;
      glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, curs.name());
      auto *found = (IDType *) glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE);
      if (emgr) {
        for (auto &each : *emgr->getentities()) {
          auto ptr = each.get();
          if (ptr->getid() == *found) {
            _hoveron = ptr;
            *found = 0;
            break;
          }
        }
      }
      glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    }

    void System::render() {
      for (auto &each : *emgr->getentities()) {
        auto *ptr = each->getcomponent<Object>();
        if (ptr != nullptr) {
          ptr->model->instancedata.push_back(each.get());
        }
      }

      glUniform2f(6, .95f, 1.1f);
      //first render
      for (auto &each : Models) {
        render(*each.second.get());
      }
      glUniform2f(6, -0.1f, .95f);
      glColorMaski(1, GL_FALSE, GL_FALSE, GL_FALSE,
                   GL_FALSE);//prevent old normal data from being overwritten in transparency
      glColorMaski(5, GL_FALSE, GL_FALSE, GL_FALSE,
                   GL_FALSE);//prevent old position data from being overwritten in transparency
      ////second render
      for (auto &each : Models) {
        render(*each.second.get());
      }
      glColorMaski(1, GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
      glColorMaski(5, GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
      for (auto &each : Models) {
        finishrender(*each.second.get());
      }
    }

    //need to use UBOs instead of uniforms
    void System::render(Model &ref) {
      if (curr_cam) {
        for (auto &entity : ref.instancedata) {
          auto obj = entity->getcomponent<Object>();
          if (obj->nodepthtest) {
            glDisable(GL_BLEND);
            glDepthMask(GL_FALSE);
          }
          //Camera *cam = System->curr_cam->getcomponent<Camera>();
          auto *camp = curr_cam->getcomponent<Placement::Placement>();
          auto *objplace = entity->getcomponent<Placement::Placement>();
          GridPosition temp = (objplace->position - camp->position);

          glm::vec3 lpos = temp.getabsposition();
          glUniformMatrix3fv(1, 1, GL_FALSE, &(glm::toMat3(objplace->orientation) * obj->scale)[0][0]);
          glUniform3f(2, lpos.x, lpos.y, lpos.z);
          glUniform1ui(3, entity->getid());//to find: more elegant way of ownership
          for (int i(0); i != ref.VertexObjectArrays.size(); ++i) {
            glBindTextures(0, 3, &ref.texturesindex[i][0]);
            ///animation uniform and function here	
            glBindVertexArray(ref.VertexObjectArrays[i]);
            glDrawElements(GL_TRIANGLES, (GLuint) ref.VertexArraysSize[i], GL_UNSIGNED_INT, 0);
            //glDrawElementsInstanced(GL_TRIANGLES, (GLuint)VertexArraysSize[i], GL_UNSIGNED_INT, 0,1);
          }
          if (obj->nodepthtest) {
            glEnable(GL_BLEND);
            glDepthMask(GL_TRUE);
          }
        }
      }
    }

    void System::finishrender(Model &ref) {
      ref.instancedata.resize(0);
    }
  }
}

