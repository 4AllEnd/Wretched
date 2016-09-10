#pragma once
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <sstream>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <ft2build.h>
#include <freetype/fttypes.h>//shouldn't be necessary but became so

#include <freetype/freetype.h>
#include <freetype/ftcache.h>
#include <freetype/config/ftconfig.h>
#include <GL/glew.h>
#if WIN_32
#include <GL/wglew.h>
#endif

#include <Wretched/Graphics/GLError.h>
#include <Wretched/System/Entity.h>
#include <mutex>

namespace Wretched {
  struct FontLibrary;
  struct Font {
    enum Style: unsigned int {
      Mono = 0,
      Full = 1,
      Bold = 1 << 1,
      Italic = 1 << 2,
      Underlined = 1 << 3,
      Wrapping = 1 << 4
    };
    Font(FontLibrary &lib, const char *);
    ~Font();
    FontLibrary *parent;
    FTC_ScalerRec scale;

    glm::vec4 color;
    void beginraster();
    void endraster();
    void render(const float &x, const float &y, const float unit, const bool &inpixel, const char *, ...);
  protected:
    FTC_SBitCache bcache;
    FTC_ImageCache icache;
    GLuint texture;
  };

  struct Text {
    std::string string;
    glm::vec4 color;

    struct Manager {
      Manager();
      ~Manager();
      std::unordered_map<std::string, Font> fonts;
      FT_Library ft;
      FTC_Manager cache;
      FTC_CMapCache chcache;
      std::string fontdir;
      int swidth, sheight;
      GLuint shader;
      GLuint VA, VBO;
    };
  };

  struct FontLibrary {
    FontLibrary();
    FT_Library ft;
    FTC_Manager cache;
    FTC_CMapCache chcache;
    GLuint shader;
    std::string fontdir;
    GLuint VA, VBO;
    int swidth, sheight;
    void setviewport(const int &, const int &);
    void setshader(const unsigned int &);
    ~FontLibrary();
  };

}

